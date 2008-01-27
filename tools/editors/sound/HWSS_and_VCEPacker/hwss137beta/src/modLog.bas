Attribute VB_Name = "modLog"
Option Explicit


Dim log As ListBox

Public Sub LogBoxSet(lst As ListBox)
    Set log = lst
End Sub

Public Sub logMsg(aStr As String)
    log.AddItem aStr
    log.ListIndex = log.ListCount - 1
End Sub
