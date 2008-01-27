VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsProgressBar"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'**************************************
' Name: A custom progressbar class modul
'     e - Updated
' Description:I created this class modul
'     e to turn a picturebox into a progressba
'     r. It is very simple right now. You can
'     se the back and forecolor then set the v
'     alue and (update) ! wether to show perce
'     nt or not !. Please tell me what you thi
'     nk (and vote please) By the way. Most co
'     lor combo's work. All the vb color const
'     ants do, u know vbblack, vb red so on. B
'     ut if u usse long or hex values, they so
'     metimes go different colors.
' By: Coding Genius
'
' Side Effects:If you use hex or long va
'     lues when setting the colors sometimes t
'     he oclors ocme out wrong because of the
'     binary operators being used but all the
'     vbcolorconstants work, vbblac, vbgreen,
'     vbblue and so on. Most colors work.
'
'This code is copyrighted and has' limited warranties.Please see http://w
'     ww.Planet-Source-Code.com/xq/ASP/txtCode
'     Id.28251/lngWId.1/qx/vb/scripts/ShowCode
'     .htm'for details.'**************************************

Option Explicit
Private mvarValue As Integer
Private mvarForeColor As Long
Private mvarBackColor As Long
Private mvarDrawText As Boolean
Private mvPercentage As Integer
Private mvWidthPerPercent As Integer
Public Event ProgressDone()


Public Property Let BackColor(ByVal vData As Double)
    mvarBackColor = vData
End Property


Public Property Get BackColor() As Double
    mvarBackColor = BackColor
End Property


Public Property Let ForeColor(ByVal vData As Double)
    mvarForeColor = vData
End Property


Public Property Get ForeColor() As Double
    mvarForeColor = ForeColor
End Property


Public Property Let value(ByVal vData As Integer)
    mvarValue = vData
End Property


Public Property Get value() As Integer
    value = mvarValue
End Property


Public Property Let percentage(ByVal prcnt As Integer)
' my modification
    If (prcnt < 0) Then
        mvPercentage = 0
    ElseIf (prcnt > 100) Then
        mvPercentage = 100
    Else
        mvPercentage = prcnt
    End If
    
    value = mvWidthPerPercent * mvPercentage
End Property

Public Property Get percentage() As Integer
' my modification
    percentage = mvPercentage
End Property


Public Property Get DrawText() As Boolean
    DrawText = mvarDrawText
End Property


Public Property Let DrawText(ByVal vData As Boolean)
    mvarDrawText = vData
End Property


Private Sub Class_Initialize()
    mvarValue = 0
    mvarBackColor = &H8000000F
    mvarForeColor = &HFF0000
    mvarDrawText = True
End Sub


Public Sub InitProgress(ByVal PicBar As PictureBox)


    With PicBar
        .ScaleMode = vbPixels
        .AutoRedraw = True
        .FontBold = True
        .BackColor = mvarBackColor
        .ForeColor = mvarForeColor
    End With
    
    mvWidthPerPercent = (PicBar.Width / Screen.TwipsPerPixelX) / 100
End Sub


Public Sub DrawStatus(ByVal PicBar As PictureBox)
    PicBar.Cls


    If mvarDrawText Then
        PicBar.CurrentX = (PicBar.ScaleWidth / 2) - (PicBar.TextWidth(CInt((PicBar.TextWidth(mvarValue / PicBar.Width) * 100))) / 2)
        PicBar.CurrentY = (PicBar.ScaleHeight / 2) - (PicBar.TextHeight("1") / 2)
    End If


    If mvarValue >= PicBar.ScaleWidth Then
        RaiseEvent ProgressDone


        If mvarDrawText Then
            PicBar.Print "100%"
        End If
    Else


        If mvarDrawText Then
            PicBar.Print CStr(Round(((mvarValue / PicBar.ScaleWidth) * 100), 0) & "%")
        End If
    End If
    PicBar.Line (0, 0)-((mvarValue / PicBar.ScaleWidth) * PicBar.ScaleWidth, PicBar.ScaleHeight), mvarForeColor Xor mvarBackColor, BF
    PicBar.Refresh
End Sub
'USE LIKE THIS:
'Option Explicit
'Private WithEvents progress As clsProgressBar
'
'
'Private Sub Form_Load()
'    Set progress = New clsProgressBar
'    progress.BackColor = vblack
'    progress.ForeColor = vbGreen
'    progress.InitProgress Picture1
'End Sub
'
'
'Private Sub progress_ProgressDone()
'    Timer1.Enabled = False
'    MsgBox "done"
'End Sub
'
'
'Private Sub Timer1_Timer()
'    progress.value = progress.value + 10
'    progress.DrawStatus Picture1
'End Sub
'Thats how you use it!!!!!!!!!!!!!!!!!!!
'     !!!!!!!!

