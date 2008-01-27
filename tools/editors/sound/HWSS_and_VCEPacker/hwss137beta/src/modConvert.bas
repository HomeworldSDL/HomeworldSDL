Attribute VB_Name = "modConvert"
Option Explicit

Global Const AIFR_HEADERSIZE = 90

Type aifrxhdr
    datastr As String * 4
    sze As Long
End Type

Type aifrbhdr
    datastr As String * 4
End Type

Public Sub convertMP32Wav(theFile As String)
    Dim cmdLine As String
    
    'run this file (now in 8.3 format) through the lame encoder, set to convert to wav
        
    breakfilename theFile
    If (Dir(bfn.fullpath & "lame.exe") = "") Then
        FileCopy appDir & "lame.exe", bfn.fullpath & "lame.exe"
    End If
    ChDrive bfn.fdrv
    ChDir bfn.fullpath
    
    cmdLine = "lame.exe --decode "
    cmdLine = cmdLine & bfn.filename & " " & bfn.fnameNoExt & ".wav"
    
    'MsgBox cmdLine
    ExecCmd cmdLine
    
    If (Dir(bfn.fnameNoExt & ".wav", vbNormal) <> "") Then
        Kill (theFile)
    End If
    
End Sub

Public Sub convertWav2AIFR(theFile As String, encodingVal As Integer)
    Dim cmdLine As String
    
    'run this file (now in 8.3 format) through the lame encoder, set to convert to wav
        
    breakfilename theFile
    If (Dir(bfn.fullpath & "aifrenc.exe") = "") Then
        FileCopy appDir & "aifrenc.exe", bfn.fullpath & "aifrenc.exe"
    End If
    ChDrive bfn.fdrv
    ChDir bfn.fullpath
   
    cmdLine = "aifrenc "
    cmdLine = cmdLine & bfn.filename & " " & bfn.fnameNoExt & ".aifr "
    cmdLine = cmdLine & encodingVal
    
    'MsgBox cmdLine
    ExecCmd cmdLine
    
    If (Dir(bfn.fnameNoExt & ".aifr", vbNormal) <> "") Then
        Kill (theFile)
    End If
End Sub

Public Function makeAifr4Music(theFile As String) As String
    Dim cnt As Long, ifn As Integer, ofn As Integer, sze As Long, tname As String
    Dim v As Byte, thdr As aifrxhdr
    
    ' get a temporary output name
    breakfilename (theFile)
    tname = bfn.fullpath & bfn.fnameNoExt & ".aifrx"
    thdr.datastr = "DATA"

    ' open the original and new files
    ifn = FreeFile
    Open theFile For Binary Access Read As #ifn
    
    ofn = FreeFile
    Open tname For Binary Access Write As #ofn
    
    ' get the offset size
    thdr.sze = FileLen(theFile) - AIFR_HEADERSIZE         'aifr header is 90 bytes
    
    ' output first info
    Put #ofn, , thdr
    
    'offset and copy the rest
    Seek #ifn, 91                       'go to the 91st byte to get the raw sound data
    
    frmMain.progAifr2Aifrx.Min = 1      'update the UI
    frmMain.progAifr2Aifrx.Max = thdr.sze
    frmMain.progAifr2Aifrx.Visible = True
    
    For cnt = 1 To thdr.sze
        Get #ifn, , v
        Put #ofn, , v
        frmMain.progAifr2Aifrx.Value = cnt
        DoEvents
    Next cnt
    
    frmMain.progAifr2Aifrx.Visible = False
    
    'close both files
    Close #ifn
    Close #ofn
    
End Function


Public Function appendBinaryFiles(orig As String, addon As String, prg As ProgressBar) As Boolean
    Dim ofn As Integer, ifn As Integer, olen As Long, ilen As Long, t As Byte, i As Long
    Dim along As Long, numlongs As Long, blongsonly As Boolean
    
    'make sure they both exist first
    If (Dir(orig, vbNormal) = "") Then
        appendBinaryFiles = False
        Exit Function
    End If
    If (Dir(addon, vbNormal) = "") Then
        appendBinaryFiles = False
        Exit Function
    End If
      
    'get file lengths
    olen = FileLen(orig)
    ilen = FileLen(addon)
    
    'can we copy by longs only?
    If ((ilen Mod 4) = 0) Then
        blongsonly = True
    Else
        blongsonly = False
    End If
    
    numlongs = ilen / 4
        
     If (prg > -1) Then
        prg.Max = numlongs
        prg.Min = 1
        prg.Visible = True
    End If
   
   'open both files
    On Error GoTo appenderr
    ofn = FreeFile
    Open orig For Binary Access Write As #ofn
    ifn = FreeFile
    Open addon For Binary Access Read As #ifn
    On Error GoTo 0
    
    'add new stuff to original file
    Seek #ofn, olen + 1
    For i = 1 To numlongs
        Get #ifn, , along
        Put #ofn, , along
        DoEvents
        If (prg > -1) Then
            prg.Value = i
        End If
    Next i
    If (blongsonly = False) Then
        For i = 1 To (ilen Mod 4)
            Get #ifn, , t
            Put #ofn, , t
            If (prg > -1) Then
                prg.Value = prg.Max
            End If
        Next i
    End If
'
'        If (prg > -1) Then
'            prg.Value = i
'        End If
'
'        DoEvents
'    Next i
    
    If (prg > -1) Then
        prg.Visible = False
    End If
    
    'close both files
    Close #ifn
    Close #ofn
    
    appendBinaryFiles = True
    Exit Function
appenderr:
    appendBinaryFiles = False
End Function


Public Function createEmptyBinaryFile(newFile As String) As Boolean
    Dim nfn As Integer
        
    'does this file already exist?
    If (Dir(newFile, vbNormal) <> "") Then
        createEmptyBinaryFile = False
        Exit Function
    End If
    
    'make a new binary file with nothing in it
    On Error GoTo ceberr
    nfn = FreeFile
    Open newFile For Binary Access Write As #nfn
    On Error GoTo 0
    Close #nfn
    
    createEmptyBinaryFile = True
    Exit Function
ceberr:
    createEmptyBinaryFile = False
End Function

Public Function makeAifr4SoundFX(theFile As String) As String
    Dim cnt As Long, ifn As Integer, ofn As Integer, sze As Long, tname As String
    Dim v As Byte, thdr As aifrbhdr
    
    ' --- Same as aifrx, but without the sound byte length field
    
    ' get a temporary output name
    breakfilename (theFile)
    tname = bfn.fullpath & bfn.fnameNoExt & ".aifrb"
    thdr.datastr = "DATA"

    ' open the original and new files
    ifn = FreeFile
    Open theFile For Binary Access Read As #ifn
    
    ofn = FreeFile
    Open tname For Binary Access Write As #ofn
    
    ' get the offset size
    sze = FileLen(theFile) - AIFR_HEADERSIZE         'aifr header is 90 bytes
    
    ' output first info
    Put #ofn, , thdr
    
    'offset and copy the rest
    Seek #ifn, 91                       'go to the 91st byte to get the raw sound data
    
    For cnt = 1 To sze
        Get #ifn, , v
        Put #ofn, , v
        DoEvents
    Next cnt
    
    'close both files
    Close #ifn
    Close #ofn
    
End Function


