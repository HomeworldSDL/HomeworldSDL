VERSION 5.00
Begin VB.Form frmNotice 
   BorderStyle     =   3  'Fixed Dialog
   ClientHeight    =   1590
   ClientLeft      =   45
   ClientTop       =   45
   ClientWidth     =   4830
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1590
   ScaleWidth      =   4830
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdclose 
      Caption         =   "close"
      Height          =   255
      Left            =   2040
      TabIndex        =   1
      Top             =   1200
      Width           =   855
   End
   Begin VB.Timer tmrflash 
      Enabled         =   0   'False
      Interval        =   300
      Left            =   3720
      Top             =   600
   End
   Begin VB.Label lblNote 
      Alignment       =   2  'Center
      Caption         =   "#"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1335
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4575
   End
   Begin VB.Shape shpOutline 
      BorderWidth     =   3
      Height          =   1575
      Left            =   0
      Shape           =   4  'Rounded Rectangle
      Top             =   0
      Width           =   4815
   End
End
Attribute VB_Name = "frmNotice"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim which As Integer

Private Sub cmdclose_Click()
    tmrflash.Enabled = False
    Unload Me
End Sub

Private Sub Form_Load()
    which = 0
    Me.Left = frmMain.Left + frmMain.Width / 2 - Me.Width / 2
    Me.Top = frmMain.Top + frmMain.Height / 2 - Me.Height / 2
    
End Sub

Private Sub tmrflash_Timer()
    If (which = 0) Then
        which = 1
        shpOutline.BorderColor = RGB(255, 0, 0)
    Else
        which = 0
        shpOutline.BorderColor = RGB(0, 0, 0)
    End If
End Sub
