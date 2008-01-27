Attribute VB_Name = "modMain"
Option Explicit

Public Sub Main()
    Dim tfn As Integer, dur As Long, outfn As Integer, i As Integer
    Dim thdr As phraselutFile, trec As phraselutRecord
    
    tfn = FreeFile
    Open App.Path & "\phrlut.lut" For Binary Access Read As #tfn
    outfn = FreeFile
    Open App.Path & "\englengths.log" For Output As #outfn
    
    Get #tfn, , thdr
    
    For i = 0 To thdr.numsentences - 1
        Get #tfn, , trec
        Print #outfn, Trim(Str(i)) & "," & Trim(Str(trec.duration))
    Next i
    
    Close #tfn
    Close #outfn
End Sub

