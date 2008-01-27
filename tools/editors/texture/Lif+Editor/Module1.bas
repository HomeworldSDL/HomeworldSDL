Attribute VB_Name = "Module1"
Option Explicit

Public NumPoints As Long

Public Type LiFType
    Identifier As String * 8
    Version As Long
    Flags As Long
    Width As Long
    Height As Long
    PalletteCRC As Long
    ImageCRC As Long
    Data As Long
    Palette As Long
    Team0 As Long
    Team1 As Long
End Type

Public LiFHeader As LiFType
Type PixelType
    R As Byte
    G As Byte
    B As Byte
    A As Byte
End Type
Public Pixel As PixelType
Public LiFSize As Long
Public FilePos As Long
Public Data() As Byte
Public ImagePalette(1 To 256) As PixelType
Public TeamEffect0(1 To 256) As Byte
Public TeamEffect1(1 To 256) As Byte
Public i As Long
Public Body As PixelType
Public Stripe As PixelType
Public Buffer1() As Byte
Public B1size As Integer

Public Sub LoadFile(File As String)
    Open File For Binary As #1
    LiFSize = LOF(1)
    LiFSize = LiFSize - &H600
    
    FilePos = 1
    Seek #1, FilePos
    Get #1, , LiFHeader.Identifier
    Get #1, , LiFHeader.Version
    Get #1, , LiFHeader.Flags
    Get #1, , LiFHeader.Width
    Get #1, , LiFHeader.Height
    Get #1, , LiFHeader.PalletteCRC
    Get #1, , LiFHeader.ImageCRC
    Get #1, , LiFHeader.Data
    Get #1, , LiFHeader.Palette
    Get #1, , LiFHeader.Team0
    Get #1, , LiFHeader.Team1
    
    NumPoints = LiFHeader.Width * LiFHeader.Height
    
    ReDim Data(1 To NumPoints)
'    FilePos = LiFHeader.Data
'    Seek #1, FilePos + 1
    For i = 1 To NumPoints
        Get #1, , Data(i)
    Next i
'    FilePos = LiFHeader.Palette
'    Seek #1, FilePos
    For i = 1 To 256
        Get #1, , ImagePalette(i).R
        Get #1, , ImagePalette(i).G
        Get #1, , ImagePalette(i).B
        Get #1, , ImagePalette(i).A
    Next i
'    FilePos = LiFHeader.Team0
'    Seek #1, FilePos
    For i = 1 To 256
        Get #1, , TeamEffect0(i)
    Next i
'    FilePos = LiFHeader.Team1
'    Seek #1, FilePos
    For i = 1 To 256
        Get #1, , TeamEffect1(i)
    Next i
    
    Close #1
    
    ApplyTeamEffects
End Sub
Public Sub main()
    ChDir App.Path
    Form1.Show
    DoEvents
End Sub

Public Sub SaveFile(File As String)
    Open File For Binary As #1
    Seek #1, 1
    Put #1, , LiFHeader.Identifier
    Put #1, , LiFHeader.Version
    Put #1, , LiFHeader.Flags
    Put #1, , LiFHeader.Width
    Put #1, , LiFHeader.Height
    Put #1, , LiFHeader.PalletteCRC
    Put #1, , LiFHeader.ImageCRC
    Put #1, , LiFHeader.Data
    Put #1, , LiFHeader.Palette
    Put #1, , LiFHeader.Team0
    Put #1, , LiFHeader.Team1
    For i = 1 To NumPoints
        Put #1, , Data(i)
    Next i
    For i = 1 To 256
        FilePos = Seek(1)
        If FilePos > 2494 Then
            DoEvents
        End If
        Put #1, , ImagePalette(i).R
        Put #1, , ImagePalette(i).G
        Put #1, , ImagePalette(i).B
        Put #1, , ImagePalette(i).A
    Next i
    For i = 1 To 256
        Put #1, , TeamEffect0(i)
    Next i
    For i = 1 To 256
        Put #1, , TeamEffect1(i)
    Next i
    Close #1
End Sub

Public Sub ApplyTeamEffects()
    For i = 1 To 256
        If TeamEffect0(i) <> 0 Then
            ImagePalette(i).R = Body.R * CSng((255 - TeamEffect0(i)) / 255)
            ImagePalette(i).G = Body.G * CSng((255 - TeamEffect0(i)) / 255)
            ImagePalette(i).B = Body.B * CSng((255 - TeamEffect0(i)) / 255)
        End If
    Next i
    For i = 1 To 256
        If TeamEffect1(i) <> 0 Then
            ImagePalette(i).R = Stripe.R * CSng((255 - TeamEffect1(i)) / 255)
            ImagePalette(i).G = Stripe.G * CSng((255 - TeamEffect1(i)) / 255)
            ImagePalette(i).B = Stripe.B * CSng((255 - TeamEffect1(i)) / 255)
        End If
    Next i
End Sub
