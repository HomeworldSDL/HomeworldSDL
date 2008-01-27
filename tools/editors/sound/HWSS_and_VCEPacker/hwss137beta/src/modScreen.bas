Attribute VB_Name = "modScreen"
Option Explicit

Global twipx As Integer, twipy As Integer

Public Sub getScreenDims()
    twipx = Screen.TwipsPerPixelX
    twipy = Screen.TwipsPerPixelY
End Sub
