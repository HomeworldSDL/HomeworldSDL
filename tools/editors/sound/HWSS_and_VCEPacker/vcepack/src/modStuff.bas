Attribute VB_Name = "modStuff"
Option Explicit

Type aFN
    filename As String
    fpath As String
    fext As String
    fdrv As String
    fnameNoExt As String
    fullpath As String
End Type

Global bfn As aFN

Public Sub breakfilename(aStr As String)
    Dim i As Integer, tmpchar As String, tmpstr As String, holder As Integer
    
    'cheap and easy way to get the elements of a filename and its path
    
    bfn.fdrv = ""
    bfn.fext = ""
    bfn.filename = ""
    bfn.fnameNoExt = ""
    bfn.fpath = ""
    
    tmpstr = ""
    
    'get extension
    For i = Len(aStr) To 1 Step -1
        tmpchar = Mid(aStr, i, 1)
        If (tmpchar <> ".") Then
            tmpstr = tmpstr & tmpchar
        Else
            Exit For
        End If
    Next i
    
    bfn.fext = reverseStr(tmpstr)
    holder = i - 1
    
    'get name of file
    tmpstr = ""
    For i = holder To 1 Step -1
        tmpchar = Mid(aStr, i, 1)
        If (tmpchar <> "\") Then
            tmpstr = tmpstr & tmpchar
        Else
            Exit For
        End If
    Next i
    
    bfn.fnameNoExt = reverseStr(tmpstr)
    ' full name and extension
    bfn.filename = bfn.fnameNoExt & "." & bfn.fext
    
    holder = i - 1
    
    'get drive letter
    bfn.fdrv = Left(aStr, 1)
    
    'get path
    tmpstr = ""
    For i = 4 To holder             '4 skips the letter, the colon and the 1st slash
        tmpchar = Mid(aStr, i, 1)
        tmpstr = tmpstr & tmpchar
    Next i
    
    bfn.fpath = tmpstr
    
    bfn.fullpath = bfn.fdrv & ":\" & bfn.fpath & "\"
End Sub

Public Function reverseStr(aStr As String) As String
    Dim i As Integer, tmpstr As String
    
    tmpstr = ""
    For i = Len(aStr) To 1 Step -1
        tmpstr = tmpstr & Mid(aStr, i, 1)
    Next i
    
    reverseStr = tmpstr
End Function

