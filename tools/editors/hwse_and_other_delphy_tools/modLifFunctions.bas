Attribute VB_Name = "modLIFFunctions"
'Public NumPoints As Long

Public Type lifCorrupt
  header As Boolean
  threedex As Boolean
End Type

Public Type LiFType
    identifier As String * 8
    version As Long
    flags As Long
    lType(3) As Boolean
    corrupt As lifCorrupt
    Width As Long
    Height As Long
    PalletteCRC As Long
    ImageCRC As Long
    data As Long
    Palette As Long
    Team0 As Long
    Team1 As Long
End Type

'public imagepalette(1 To 256) As hwColourType
'Public TeamEffect0(1 To 256) As Byte
'Public TeamEffect1(1 To 256) As Byte

'Public lifHeader As LiFType
Public Pixel As hwColourType

'Public FilePos As Long
'Public lifData() As hwColourType

Public Body As D3DCOLORVALUE
Public Stripe As D3DCOLORVALUE

' New declaration for lif files!!!
Public Type lifFile
  lifHeader As LiFType
  lifData() As hwColourType
  ImagePalette(1 To 256) As hwColourType
  TeamEffect0(1 To 256) As Byte
  TeamEffect1(1 To 256) As Byte
  NumPoints As Long
End Type

' Used for the thumbnail control
Public Type selectedItem
  ' File name only
  Text As String
  ' Index of this thumbnail
  Index As Integer
  ' Full filename and path
  Filename As String
End Type
Public thumbSelected As selectedItem

' API functions for drawing pixels
Private Declare Function GetPixel Lib "gdi32" (ByVal hDC As Long, ByVal x As Long, ByVal y As Long) As Long
'Private Declare Function SetPixel Lib "gdi32" (ByVal hDC As Long, ByVal x As Long, ByVal y As Long, ByVal Color As Long) As Long
Private Declare Function SetPixelV Lib "gdi32" (ByVal hDC As Long, ByVal x As Long, ByVal y As Long, ByVal Color As Long) As Byte

Private Type SAFEARRAYBOUND
    cElements As Long
    lLbound As Long
End Type

Private Type SAFEARRAY2D
    cDims As Integer
    fFeatures As Integer
    cbElements As Long
    cLocks As Long
    pvData As Long
    Bounds(0 To 1) As SAFEARRAYBOUND
End Type
Private Declare Function VarPtrArray Lib "msvbvm50.dll" Alias "VarPtr" (Ptr() As Any) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (pDst As Any, pSrc As Any, ByVal ByteLen As Long)
Private Type BITMAP
    bmType As Long
    bmWidth As Long
    bmHeight As Long
    bmWidthBytes As Long
    bmPlanes As Integer
    bmBitsPixel As Integer
    bmBits As Long
End Type

Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Private Declare Function GetObjectAPI Lib "gdi32" Alias "GetObjectA" (ByVal hObject As Long, ByVal nCount As Long, lpObject As Any) As Long

Public myLifFile As lifFile
Private m_cDIB As cDIBSection


' Loads the lif file from disk . It populates all the arrays and palette holding arrays
' and sets the global current header info'.

Public Function LoadFile(File As String, loadImageData As Boolean)
  errModule = "modLifFunctions"
  errFunction = "LoadFile"
  errParms = File & "|" & loadImageData
  On Error GoTo standardError
  
  'On Error GoTo ErrHandler
  Dim lifFile As Integer
  
  If Dir(File) = "" Then
    LoadFile = False
    Exit Function
  End If
    
  ' Reset menus and bits
  With frmLifPreview
    .mnuViewAlpha.Enabled = False
    .mnuViewTeam0.Enabled = False
    .mnuViewTeam1.Enabled = False
    .mnuViewBaseTeam0.Enabled = False
    .mnuViewBaseTeam1.Enabled = False
    .mnuViewAll.Enabled = False
    .picTeam0.Visible = False
    .picTeam1.Visible = False
  End With
  
  For x = 0 To 3
    myLifFile.lifHeader.lType(x) = False
  Next x
    
  pLog "Loading Lif " & File, 2
  frmLifPreview.Label1.Caption = ""
  
  lifFile = FreeFile
  Open File For Binary As #lifFile
    
  Get #lifFile, , myLifFile.lifHeader.identifier
  Get #lifFile, , myLifFile.lifHeader.version
  Get #lifFile, , myLifFile.lifHeader.flags
  Get #lifFile, , myLifFile.lifHeader.Width
  Get #lifFile, , myLifFile.lifHeader.Height
  Get #lifFile, , myLifFile.lifHeader.PalletteCRC
  Get #lifFile, , myLifFile.lifHeader.ImageCRC
  Get #lifFile, , myLifFile.lifHeader.data
  Get #lifFile, , myLifFile.lifHeader.Palette
  Get #lifFile, , myLifFile.lifHeader.Team0
  Get #lifFile, , myLifFile.lifHeader.Team1
  'dText ("Version: " & lifHeader.version)
  'dText ("Flags: " & lifHeader.flags)
    
  Dim flagBits As String, tmpLong As Long
  ' Convert material.Flags to Long for the flagBits
  tmpLong = myLifFile.lifHeader.flags
  flagBits = DecToBin(tmpLong)
  Debug.Print "Lif: " & tmpLong & ":" & flagBits
  For j = 17 To 1 Step -1
  If Mid$(flagBits, j, 1) = "1" Then
    Select Case j
    Case 16:
      ' 2 - Palette
      myLifFile.lifHeader.lType(0) = True
      pLog "  -> Palette", 2
    Case 14:
      ' 8 - Alpha
      myLifFile.lifHeader.lType(1) = True
      pLog "  -> Alpha", 2
      frmLifPreview.mnuViewAlpha.Enabled = True
    Case 13:
      ' 16 - Team0
      myLifFile.lifHeader.lType(2) = True
      pLog "  -> Team0", 2
      frmLifPreview.mnuViewTeam0.Enabled = True
      frmLifPreview.mnuViewBaseTeam0.Enabled = True
      frmLifPreview.mnuViewAll.Enabled = True
    Case 12:
      ' 32 - Team1
      myLifFile.lifHeader.lType(3) = True
      pLog "  -> Team1", 2
      frmLifPreview.mnuViewTeam1.Enabled = True
      frmLifPreview.mnuViewBaseTeam1.Enabled = True
      frmLifPreview.mnuViewAll.Enabled = True
    End Select
  End If
  Next j
    
  pLog "  -> " & myLifFile.lifHeader.Width & " x " & myLifFile.lifHeader.Height, 2
  'dText ("  -> Palette: " & lifHeader.Palette)
  'dText ("  -> PalCRC: " & lifHeader.PalletteCRC)
  'dText ("  -> ImgCRC: " & lifHeader.ImageCRC)
    
  frmHWSE.txtImgCRC = myLifFile.lifHeader.ImageCRC
  frmHWSE.txtPalCRC = myLifFile.lifHeader.PalletteCRC
  frmHWSE.txtLifFlags = myLifFile.lifHeader.flags
    
  'dText ("  -> Team0: " & lifHeader.Team0)
  'dText ("  -> Team1: " & lifHeader.Team1)
  
  With myLifFile.lifHeader.corrupt
    .header = False
    .threedex = False
  End With
  
  If myLifFile.lifHeader.flags = 0 Then
    frmHWSE.cmdFixLif.Enabled = True
    frmLifPreview.mnuFixLif.Enabled = True
    frmLifPreview.Label1.Caption = "Warning! Corrupted lif detected.  Fix using File -> Fix lif"
    myLifFile.lifHeader.corrupt.header = True
  End If
  If LOF(lifFile) <> (((myLifFile.lifHeader.Width * myLifFile.lifHeader.Height) + 48) + 1024 + 512) Then
    'Debug.Print "It's bigger than ever!"
    If myLifFile.lifHeader.lType(1) = False Then
      'Debug.Print "..and it's coming to a store near you!"
      ' If it's not an Alpha lif...
      frmHWSE.cmdFixLif.Enabled = True
      frmLifPreview.mnuFixLif.Enabled = True
      frmLifPreview.Label1.Caption = "Warning! Corrupted lif detected.  Fix using File -> Fix lif"
      If LOF(lifFile) <> (((myLifFile.lifHeader.Width * myLifFile.lifHeader.Height) + 48) + 1024) Then
        myLifFile.lifHeader.corrupt.threedex = True
      End If
    End If
  End If
    
  If loadImageData = True Then
    myLifFile.NumPoints = myLifFile.lifHeader.Width * myLifFile.lifHeader.Height
    If LOF(lifFile) = (myLifFile.NumPoints * 4) + 48 Then
      If myLifFile.lifHeader.Palette = 0 Then
        myLifFile.lifHeader.lType(1) = True
      End If
    End If
    
    If myLifFile.NumPoints = 0 Then
      Close #lifFile
      LoadFile = False
      Exit Function
    End If
    ReDim myLifFile.lifData(1 To myLifFile.NumPoints)
    Dim i As Long
    
    ' Define a buffer into which we are going to store
    ' the entire lif picture.  This is *much* faster
    ' that reading individual bytes
    Dim tmpBytes() As Byte
    ReDim tmpBytes(1 To myLifFile.NumPoints)
  
    FilePos = myLifFile.lifHeader.data
    Seek #lifFile, FilePos + 1
    If myLifFile.lifHeader.lType(1) = True Then
      ' It's an Alpha LIF
      For i = 1 To myLifFile.NumPoints
        Get #lifFile, , myLifFile.lifData(i)
      Next i
    Else
      If myLifFile.lifHeader.corrupt.threedex = False Then
        
        Get #lifFile, , tmpBytes
        For i = 1 To myLifFile.NumPoints
          myLifFile.lifData(i).r = tmpBytes(i)
        Next i
      Else
        x = 0
        ReDim tmpBytes(1 To (myLifFile.NumPoints * 3))
        Get #lifFile, , tmpBytes
        For i = 1 To (myLifFile.NumPoints * 3) Step 3
          x = x + 1
          myLifFile.lifData(i).r = tmpBytes(i)
          myLifFile.lifData(i).g = tmpBytes(i + 1)
          myLifFile.lifData(i).b = tmpBytes(i + 2)
        Next i
      End If
    End If
 
    FilePos = myLifFile.lifHeader.Palette
    If FilePos > 0 Then
      Seek #lifFile, FilePos + 1
      'dText ("Palette pos: " & Seek(6))
      For i = 1 To 256
        Get #lifFile, , myLifFile.ImagePalette(i).r ' normal / base palette Red value for this index
        Get #lifFile, , myLifFile.ImagePalette(i).g ' normal / base palette Green value for this index
        Get #lifFile, , myLifFile.ImagePalette(i).b ' normal / base palette Blue value for this index
        Get #lifFile, , myLifFile.ImagePalette(i).a ' normal / base palette Alpha/Reserved value for this index
      Next i
    End If

    FilePos = myLifFile.lifHeader.Team0
    If FilePos > 0 Then
      Seek #lifFile, FilePos + 1
      'dText ("Team 0 pos: " & Seek(6))
      For i = 1 To 256
        Get #lifFile, , myLifFile.TeamEffect0(i) ' the team effect body multipliers ..
      Next i
    End If
    
    FilePos = myLifFile.lifHeader.Team1
    If FilePos > 0 Then
      Seek #lifFile, FilePos + 1
      'dText ("Team 1 pos: " & Seek(6))
      For i = 1 To 256
        Get #lifFile, , myLifFile.TeamEffect1(i)
      Next i
    End If
  End If
  
  Debug.Print "Closing " & File
  Close #lifFile

  LoadFile = True
  Exit Function

errHandler:
  Debug.Print "LoadFile - " & Err.Number & " (" & Err.Description & ")"

  Exit Function
standardError:
  frmError.Show vbModal
End Function

Function GetRed(ByVal lColor As Long) As Long
    GetRed = lColor Mod 256
  Exit Function
standardError:
  frmError.Show vbModal
End Function

Function GetGreen(ByVal lColor As Long) As Long
    GetGreen = (lColor \ &H100) Mod 256
  Exit Function
standardError:
  frmError.Show vbModal
End Function

Function GetBlue(ByVal lColor As Long) As Long
    GetBlue = (lColor \ &H10000) Mod 256
  Exit Function
standardError:
  frmError.Show vbModal
End Function
Public Sub xepDrawSkin(ByVal filterLevel As Byte)
  errModule = "modLifFunctions"
  errFunction = "xepDrawSkin"
  errParms = filterLevel
  On Error GoTo standardError
  
  ' Original code by Shin
  ' Amended on 08/20/00 by Prime_8 for VET compliance .
  ' still needs work .. can be slimmed down some more
  ' load to picture .. i will change it to a picture's hdc later
  ' currently 2.5 second load /populate time
  ' FilterLevel localised for speed and ease of use 091300
  ' fixing / found a bug with Shin's image paintin routine 091300
      
  On Local Error GoTo errHandler
  
  Dim DataIndex As Long
  Dim Index As Long
  Dim XPos As Integer, YPos As Integer
  Dim tmpLong As Long
   
  '--------------------------------
  
  
  Screen.MousePointer = vbHourglass
   
  'If lifHeader.Height < 5 Then Exit Sub
  'If lifHeader.Width < 5 Then Exit Sub
  
  ' setup the picture box to the correct sise..
  frmLifPreview.picLif1.ScaleMode = 3 '3 = pixel 1 = twips 0 = user
  
  frmLifPreview.picLif1.Height = (myLifFile.lifHeader.Height * Screen.TwipsPerPixelY)
  frmLifPreview.picLif1.Width = (myLifFile.lifHeader.Width * Screen.TwipsPerPixelX)
  frmLifPreview.picLif1.ScaleHeight = myLifFile.lifHeader.Height
  frmLifPreview.picLif1.ScaleWidth = myLifFile.lifHeader.Width
  
  frmLifPreview.StatusBar1.Panels(2).Text = myLifFile.lifHeader.Width
  frmLifPreview.StatusBar1.Panels(3).Text = myLifFile.lifHeader.Height
  
  frmLifPreview.picLif1.Cls
  
  'fastDrawLif frmLifPreview.picLif1
  
  Index = 1
  For YPos = 0 To myLifFile.lifHeader.Height - 1
  'For Ypos = 0 To UBound(pic, 1)

    ' let windows do stuff
    ' DoEvents
    For XPos = 0 To myLifFile.lifHeader.Width - 1
    'For Xpos = 0 To UBound(pic, 2)

      If myLifFile.lifHeader.lType(1) = True Then
        ' It's an alpha lif
        Pixel = myLifFile.lifData(Index)
        ' Reset the filterlevel if it's > 2
        If filterLevel > 2 Then
          filterLevel = 1
        End If
      Else
        If myLifFile.lifHeader.corrupt.threedex = False Then
          DataIndex = myLifFile.lifData(Index).r + 1 ' the actual raw Lif data
          Pixel = myLifFile.ImagePalette(DataIndex)
        Else
          ' As it's corrupt, set the index into TeamEffects
          ' to 1
          DataIndex = 1
          Pixel = myLifFile.lifData(Index)
        End If
      End If

      Select Case filterLevel
        Case 1
          drawPixel XPos, YPos, RGB(Pixel.r, Pixel.g, Pixel.b)
          'pic(Xpos, Ypos) = RGB(Pixel.r, Pixel.g, Pixel.b)
        Case 2
          drawPixel XPos, YPos, RGB(Pixel.a, Pixel.a, Pixel.a)
        Case 3
          If myLifFile.TeamEffect0(DataIndex) <> 0 Then
            Pixel.r = Body.r * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
            Pixel.g = Body.g * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
            Pixel.b = Body.b * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
            drawPixel XPos, YPos, RGB(Pixel.r, Pixel.g, Pixel.b)
          Else
            drawPixel XPos, YPos, frmLifPreview.picLif1.BackColor
          End If
        Case 4
          If myLifFile.TeamEffect1(DataIndex) <> 0 Then
            'Debug.Print DataIndex & " -- " & TeamEffect1(DataIndex) & " " & Stripe.R & "/" & Stripe.G & "/" & Stripe.B
            tmpLong = myLifFile.TeamEffect1(DataIndex)
            Pixel.r = ((tmpLong * Stripe.r) / 255)
            Pixel.g = ((tmpLong * Stripe.g) / 255)
            Pixel.b = ((tmpLong * Stripe.b) / 255)
            drawPixel XPos, YPos, RGB(Pixel.r, Pixel.g, Pixel.b)
          Else
            drawPixel XPos, YPos, frmLifPreview.picLif1.BackColor
          End If
        Case 5
          If myLifFile.TeamEffect0(DataIndex) <> 0 Then
            Pixel.r = Body.r * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
            Pixel.g = Body.g * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
            Pixel.b = Body.b * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
          End If
          drawPixel XPos, YPos, RGB(Pixel.r, Pixel.g, Pixel.b)
        Case 6
          If myLifFile.TeamEffect1(DataIndex) <> 0 Then
            tmpLong = myLifFile.TeamEffect1(DataIndex)
            Pixel.r = ((tmpLong * Stripe.r) / 255)
            Pixel.g = ((tmpLong * Stripe.g) / 255)
            Pixel.b = ((tmpLong * Stripe.b) / 255)
          End If
          drawPixel XPos, YPos, RGB(Pixel.r, Pixel.g, Pixel.b)
        Case 7
          If myLifFile.TeamEffect0(DataIndex) <> 0 Then
            Pixel.r = Body.r * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
            Pixel.g = Body.g * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
            Pixel.b = Body.b * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
          End If
          If myLifFile.TeamEffect1(DataIndex) <> 0 Then
            tmpLong = myLifFile.TeamEffect1(DataIndex)
            Pixel.r = ((tmpLong * Stripe.r) / 255)
            Pixel.g = ((tmpLong * Stripe.g) / 255)
            Pixel.b = ((tmpLong * Stripe.b) / 255)
          End If
          drawPixel XPos, YPos, RGB(Pixel.r, Pixel.g, Pixel.b)
      End Select

      Index = Index + 1
    Next XPos
  Next YPos
  '  Debug.Print "Saving " & saveFilename
  
  'CopyMemory ByVal VarPtrArray(pic), 0&, 4
  frmLifPreview.picLif1.Refresh
  
  Screen.MousePointer = vbDefault

  Exit Sub
errHandler:
  Debug.Print "xepDrawSkin - " & Err.Number & " (" & Err.Description & ")"
  Exit Sub

standardError:
  frmError.Show vbModal
End Sub

Private Function drawPixel(XPos As Integer, YPos As Integer, colour As Long)

  SetPixelV frmLifPreview.picLif1.hDC, XPos, YPos, colour

End Function

Public Sub fix3dexLif(Filename As String)
  errModule = "modLifFunctions"
  errFunction = "fix3dexLif"
  errParms = Filename
  On Error GoTo standardError

  Dim lifPixelData As Byte
  Dim startOfData As Long
  Dim startOfPalette As Long
  Dim startOfTeam0 As Long
  Dim startOfTeam1 As Long
  'Dim palEntries(8192) As hwColourType
  Dim palEntries(256) As hwColourType
  
  Dim maxPalEntries As Long
  Dim tempPalEntry As hwColourType
  Dim lifAlpha As Boolean

  Screen.MousePointer = vbHourglass

  'Open .List1.List(.List1.ListIndex) & ".lif" For Binary As #6
  'Open "new" & .List1.List(.List1.ListIndex) & ".lif" For Binary As #7
  Open Filename For Binary As #6
  Open Filename & ".new" For Binary As #7
  
  pLog "Writing LiF " & Filename & ".new", 1
  
  Get #6, , myLifFile.lifHeader.identifier
  Put #7, , myLifFile.lifHeader.identifier
  Get #6, , myLifFile.lifHeader.version
  Put #7, , myLifFile.lifHeader.version
  Get #6, , myLifFile.lifHeader.flags
  Put #7, , myLifFile.lifHeader.flags
  Get #6, , myLifFile.lifHeader.Width
  Put #7, , myLifFile.lifHeader.Width
  Get #6, , myLifFile.lifHeader.Height
  Put #7, , myLifFile.lifHeader.Height
  Get #6, , myLifFile.lifHeader.PalletteCRC
  Put #7, , myLifFile.lifHeader.PalletteCRC
  Get #6, , myLifFile.lifHeader.ImageCRC
  Put #7, , myLifFile.lifHeader.ImageCRC
  Get #6, , myLifFile.lifHeader.data
  Put #7, , myLifFile.lifHeader.data
  Get #6, , myLifFile.lifHeader.Palette
  Put #7, , myLifFile.lifHeader.Palette
  Get #6, , myLifFile.lifHeader.Team0
  Put #7, , myLifFile.lifHeader.Team0
  Get #6, , myLifFile.lifHeader.Team1
  Put #7, , myLifFile.lifHeader.Team1
  myLifFile.NumPoints = myLifFile.lifHeader.Width * myLifFile.lifHeader.Height
  
  If LOF(6) <> ((myLifFile.NumPoints + 48) + 1024 + 512) Then
    pLog "-- 3dex lif?", 2
    ' This is a totally oversize LIF and we need to work
    ' it all out
    maxPalEntries = -1
    
    ' Check if it's an Alpha lif
    If LOF(6) = (myLifFile.NumPoints * 4) + 48 Then
      lifAlpha = True
      pLog "-- This is an Alpha lif", 2
    End If
    
    startOfData = (Seek(7) - 1)
    ' Start of actual image writing code
    ' Normally this would output a LIF file, however
    ' we need quantise the image first.
    Dim tmpLong As Long
    
    Dim oPic As New StdPicture
    Set oPic = frmLifPreview.Image1.Picture
   
    Set m_cDIB = New cDIBSection
    m_cDIB.CreateFromPicture oPic
    
    Dim eD As EDSSColourDepthConstants
    Dim eM As EDSSColourReductionConstants
    Dim cDIBSave As New cDIBSectionSave
    eD = edss256Colour
    sI = Filename & ".bmp"
    
    cDIBSave.Save sI, m_cDIB, , eD, edssGeneratePalette
    
    frmLifPreview.picLif1 = LoadPicture(Filename & ".bmp")
    frmLifPreview.picLif1.Refresh
    Kill Filename & ".bmp"

    For y = 0 To (myLifFile.lifHeader.Height - 1)
      For x = 0 To (myLifFile.lifHeader.Width - 1)
    'For i = 1 To (lifHeader.Width * lifHeader.Height)
        'Get #6, , lifPixelData
        'tempPalEntry.r = lifPixelData
        'Get #6, , lifPixelData
        'tempPalEntry.g = lifPixelData
        'Get #6, , lifPixelData
        'tempPalEntry.b = lifPixelData
        ' Get the palette from the loaded picture
        ' instead
        tmpLong = frmLifPreview.picLif1.Point(x, y)
        tempPalEntry.r = GetRed(tmpLong)
        tempPalEntry.g = GetGreen(tmpLong)
        tempPalEntry.b = GetBlue(tmpLong)
              
        If lifAlpha = True Then
          Get #6, , lifPixelData
          tempPalEntry.a = lifPixelData
        Else
          tempPalEntry.a = 255
        End If
      
        If maxPalEntries = -1 Then
          ' Start the list
          maxPalEntries = 0
          palEntries(maxPalEntries).r = tempPalEntry.r
          palEntries(maxPalEntries).g = tempPalEntry.g
          palEntries(maxPalEntries).b = tempPalEntry.b
          palEntries(maxPalEntries).a = tempPalEntry.a
          pLog "-- Adding entry " & 0 & ": " & palEntries(w).r & "/" & palEntries(w).g & "/" & palEntries(w).b & "/" & palEntries(w).a, 2
          Put #7, , Chr$(0)
        Else
          ' Search for a match and if none found then add one
          found = False
          For w = 0 To maxPalEntries
            If tempPalEntry.r = palEntries(w).r And tempPalEntry.g = palEntries(w).g And tempPalEntry.b = palEntries(w).b Then
              found = True
              Exit For
            End If
          Next w
          If found = True Then
            If w <= 255 Then
              Put #7, , Chr$(w)
            Else
              Put #7, , Chr$(255)
            End If
          Else
            maxPalEntries = maxPalEntries + 1
            palEntries(maxPalEntries).r = tempPalEntry.r
            palEntries(maxPalEntries).g = tempPalEntry.g
            palEntries(maxPalEntries).b = tempPalEntry.b
            palEntries(maxPalEntries).a = tempPalEntry.a
            pLog "++ Adding entry " & maxPalEntries & ": " & palEntries(maxPalEntries).r & "/" & palEntries(maxPalEntries).g & "/" & palEntries(maxPalEntries).b & "/" & palEntries(maxPalEntries).a, 2
            If maxPalEntries <= 255 Then
              Put #7, , Chr$(maxPalEntries)
            Else
              Put #7, , Chr$(255)
            End If
          End If
        End If
      Next x
    Next y
    
    pLog "-- Writing pallete", 1
    startOfPalette = (Seek(7) - 1)
    For i = 0 To 255
      Put #7, , palEntries(i)
    Next i
        
    pLog "-- Writing Team0", 1
    startOfTeam0 = (Seek(7) - 1)
    lifPixelData = 0
    For i = 1 To 256
      Put #7, , lifPixelData
    Next i
    
    pLog "-- Writing Team1", 1
    startOfTeam1 = (Seek(7) - 1)
    lifPixelData = 0
    For i = 1 To 256
      Put #7, , lifPixelData
    Next i

  Else
    pLog "-- Relic non-used Lif", 2
    startOfData = (Seek(7) - 1)
    FilePos = lifHeader.data
    Seek #6, FilePos + 1
    For i = 1 To NumPoints
      Get #6, , lifPixelData
      Put #7, , lifPixelData
    Next i
  
    startOfPalette = (Seek(7) - 1)
    FilePos = lifHeader.Palette
    Seek #6, FilePos + 1
    For i = 1 To 256
      Get #6, , lifPixelData ' R
      Put #7, , lifPixelData ' R
      Get #6, , lifPixelData ' G
      Put #7, , lifPixelData ' G
      Get #6, , lifPixelData ' B
      Put #7, , lifPixelData ' B
      Get #6, , lifPixelData ' A
      Put #7, , lifPixelData ' A
    Next i
  
    startOfTeam0 = (Seek(7) - 1)
    FilePos = lifHeader.Team0
    If FilePos > 0 Then
      Seek #6, FilePos + 1
      For i = 1 To 256
        Get #6, , lifPixelData
        Put #7, , lifPixelData
      Next i
    Else
      pLog "-- Fixing Team0", 1
      lifPixelData = 0
      For i = 1 To 256
        Put #7, , lifPixelData
      Next i
    End If
  
    startOfTeam1 = (Seek(7) - 1)
    FilePos = lifHeader.Team1
    If FilePos > 0 Then
      Seek #6, FilePos + 1
      For i = 1 To 256
        Get #6, , lifPixelData
        Put #7, , lifPixelData
      Next i
    Else
      pLog "-- Fixing Team1", 1
      lifPixelData = 0
      For i = 1 To 256
        Put #7, , lifPixelData
      Next i
    End If
  End If
  
  Close #6
  
  pLog "-- Writing flags", 1
  Seek #7, 13
  If lifHeader.flags = 0 Then
    lifHeader.flags = 50
    Put #7, , lifHeader.flags
  End If
  
  pLog "-- Writing offsets", 1
  pLog "-- Data: " & startOfData & " Palette: " & startOfPalette & " Team0: " & startOfTeam0 & " Team1: " & startOfTeam1, 2
  Seek #7, 33
  Put #7, , startOfData
  Put #7, , startOfPalette
  Put #7, , startOfTeam0
  Put #7, , startOfTeam1

  Close #7
  
  pLog "Renaming " & Filename & " as " & Filename & ".old", 1
  
  FileCopy Filename, Filename & ".old"
  FileCopy Filename & ".new", Filename
  Kill Filename & ".new"
  
  MsgBox "Fix Lif done!"
  pLog "Fix Lif done!", 1
  
  ' Update ListView
  If frmHWSE.lstTextures.selectedItem Is Nothing Then
  Else
    frmHWSE.lstTextures.selectedItem.SubItems(3) = lifHeader.flags
  End If
  
  frmLifPreview.Label1.Caption = ""
  frmLifPreview.Label1.Refresh
  
  Screen.MousePointer = vbDefault

  LoadFile Filename, False
  'xepDrawSkin 1, filename & ".bmp"
  
  frmHWSE.cmdFixLif.Enabled = False
  Exit Sub
standardError:
  frmError.Show vbModal
End Sub

'Private Sub fastDrawLif(ByRef picBuff As PictureBox)
'    ' these are used to address the pixel using matrices
'
'    Dim pict() As Byte
'    'Dim pict2() As Byte
'    Dim sa As SAFEARRAY2D, bmp As BITMAP
'    'Dim sa2 As SAFEARRAY2D, bmp2 As BITMAP
'    Dim Xpos As Long, Ypos As Long
'    Dim rgbOffset As Long, xOffset As Long
'    Dim r As Long, g As Long, b As Long
'    Dim i As Long, j As Long, yMax As Long, xMax As Long
'
'    ' get bitmap info
'    'GetObjectAPI picImage.Picture, Len(bmp), bmp 'dest
'    GetObjectAPI picBuff.Picture, Len(bmp), bmp 'source
'
'    ' exit if not a supported bitmap
'    'If bmp.bmBitsPixel <> 24 Then
'    '    MsgBox " 24-bit bitmaps only", vbCritical
'    '    Exit Sub
'    'End If
'
'    ' have the local matrix point to bitmap pixels
'    With sa
'        .cbElements = 1
'        .cDims = 2
'        .Bounds(0).lLbound = 0
'        .Bounds(0).cElements = bmp.bmHeight
'        .Bounds(1).lLbound = 0
'        .Bounds(1).cElements = bmp.bmWidthBytes
'        .pvData = bmp.bmBits
'    End With
'    CopyMemory ByVal VarPtrArray(pict), VarPtr(sa), 4
'
'    rgbOffset = m_iOffset * 3
'    yMax = UBound(pict, 2) - m_iOffset
'    xMax = UBound(pict, 1) - 3 - rgbOffset
'
'
'    Index = 1
'
'    For Xpos = rgbOffset To xMax Step 3
'        For Ypos = m_iOffset To yMax
'
'          If myLifFile.lifHeader.lType(1) = True Then
'            ' It's an alpha lif
'            Pixel = myLifFile.lifData(Index)
'            ' Reset the filterlevel if it's > 2
'            If filterLevel > 2 Then
'              filterLevel = 1
'            End If
'          Else
'            If myLifFile.lifHeader.corrupt.threedex = False Then
'              DataIndex = myLifFile.lifData(Index).r + 1 ' the actual raw Lif data
'              Pixel = myLifFile.imagepalette(DataIndex)
'            Else
'              ' As it's corrupt, set the index into TeamEffects
'              ' to 1
'              DataIndex = 1
'              Pixel = myLifFile.lifData(Index)
'            End If
'          End If
'
'          Select Case filterLevel
'            Case 1
'              drawPixel Xpos, Ypos, Pixel, pict
'              'pic(xPos, yPos) = RGB(Pixel.r, Pixel.g, Pixel.b)
'              'pict2(X, Y) = rR: pict2(X + 1, Y) = rG: pict2(X + 2, Y) = rB
'            Case 2
'              'drawPixel xPos, yPos, RGB(Pixel.a, Pixel.a, Pixel.a), ddSurface
'            Case 3
'              If myLifFile.TeamEffect0(DataIndex) <> 0 Then
'                Pixel.r = Body.r * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'                Pixel.g = Body.g * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'                Pixel.b = Body.b * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'                'drawPixel xPos, yPos, RGB(Pixel.r, Pixel.g, Pixel.b), ddSurface
'              Else
'                'drawPixel xPos, yPos, frmLifPreview.picLif1.BackColor, ddSurface
'              End If
'            Case 4
'              If myLifFile.TeamEffect1(DataIndex) <> 0 Then
'                'Debug.Print DataIndex & " -- " & TeamEffect1(DataIndex) & " " & Stripe.R & "/" & Stripe.G & "/" & Stripe.B
'                tmpLong = myLifFile.TeamEffect1(DataIndex)
'                Pixel.r = ((tmpLong * Stripe.r) / 255)
'                Pixel.g = ((tmpLong * Stripe.g) / 255)
'                Pixel.b = ((tmpLong * Stripe.b) / 255)
'                'drawPixel xPos, yPos, RGB(Pixel.r, Pixel.g, Pixel.b), ddSurface
'              Else
'                'drawPixel xPos, yPos, frmLifPreview.picLif1.BackColor, ddSurface
'              End If
'            Case 5
'              If myLifFile.TeamEffect0(DataIndex) <> 0 Then
'                Pixel.r = Body.r * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'                Pixel.g = Body.g * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'                Pixel.b = Body.b * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'              End If
'              'drawPixel xPos, yPos, RGB(Pixel.r, Pixel.g, Pixel.b), ddSurface
'            Case 6
'              If myLifFile.TeamEffect1(DataIndex) <> 0 Then
'                tmpLong = myLifFile.TeamEffect1(DataIndex)
'                Pixel.r = ((tmpLong * Stripe.r) / 255)
'                Pixel.g = ((tmpLong * Stripe.g) / 255)
'                Pixel.b = ((tmpLong * Stripe.b) / 255)
'              End If
'              'drawPixel xPos, yPos, RGB(Pixel.r, Pixel.g, Pixel.b), ddSurface
'            Case 7
'              If myLifFile.TeamEffect0(DataIndex) <> 0 Then
'                Pixel.r = Body.r * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'                Pixel.g = Body.g * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'                Pixel.b = Body.b * CSng((255 - myLifFile.TeamEffect0(DataIndex)) / 255)
'              End If
'              If myLifFile.TeamEffect1(DataIndex) <> 0 Then
'                tmpLong = myLifFile.TeamEffect1(DataIndex)
'                Pixel.r = ((tmpLong * Stripe.r) / 255)
'                Pixel.g = ((tmpLong * Stripe.g) / 255)
'                Pixel.b = ((tmpLong * Stripe.b) / 255)
'              End If
'              'drawPixel xPos, yPos, RGB(Pixel.r, Pixel.g, Pixel.b), ddSurface
'          End Select
'
'          Index = Index + 1
'
'            'Debug.Print rR, rG, rB, vbCrLf
'        Next Ypos
'    Next Xpos
'
'    ' clear the temporary array descriptor
'    ' without destroying the local temporary array
'    CopyMemory ByVal VarPtrArray(pict), 0&, 4
'    'CopyMemory ByVal VarPtrArray(pict2), 0&, 4
'
'    picBuff.Refresh
'
'End Sub

