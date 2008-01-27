VERSION 5.00
Begin VB.Form frmSplash 
   ClientHeight    =   3585
   ClientLeft      =   60
   ClientTop       =   60
   ClientWidth     =   4155
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   ScaleHeight     =   3585
   ScaleWidth      =   4155
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Interval        =   2000
      Left            =   3480
      Top             =   2640
   End
   Begin VB.Image Image1 
      Height          =   3300
      Left            =   0
      Picture         =   "frmSplash.frx":0000
      Stretch         =   -1  'True
      Top             =   0
      Width           =   3960
   End
End
Attribute VB_Name = "frmSplash"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    Me.Width = Image1.Width + 4 * Screen.TwipsPerPixelX
    Me.Height = Image1.Height + 4 * Screen.TwipsPerPixelY
    Me.Left = Screen.Width / 2 - Me.Width / 2
    Me.Top = Screen.Height / 2 - Me.Height / 2
    Timer1.Interval = 4000
    Timer1.Enabled = True
End Sub

Private Sub Timer1_Timer()
    frmMain.Visible = True
    Unload Me
End Sub
