Attribute VB_Name = "modMain"
Public Sub Main()
    Dim i As Integer, t As Integer, t1name As String, t2name As String, j As Integer
    Dim thecount As Integer
    
    frmOut.Show
    
    t = 11292 / 18
    
    ChDir "c:\tmp\t"
    
    For i = 1 To t - 1
        For j = 0 To 17
            t1name = Trim(Str(j) & ".aifr")
            thecount = j + (18 * i)
            t2name = Trim(Str(thecount) & ".aifr")
            frmOut.lblNum.Caption = Str(thecount)
            FileCopy t1name, t2name
            makeSubT thecount, t2name
            DoEvents
        Next j
    Next i
End Sub

Public Sub makeSubT(which As Integer, theName As String)
    Dim tfn As Integer, tname As String
    
'    breakfilename theName
'    tname = bfn.fullpath & "subs\" & bfn.fnameNoExt & ".txt"
    tname = CurDir & "\subs\" & Trim(Str(which)) & ".txt"
    tfn = FreeFile
    Open tname For Output As #tfn
    Print #tfn, "Subtitle test " & Str(which)
    Close #tfn
End Sub
