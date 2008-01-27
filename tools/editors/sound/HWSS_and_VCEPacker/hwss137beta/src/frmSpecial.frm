VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmSpecial 
   BorderStyle     =   3  'Fixed Dialog
   ClientHeight    =   3780
   ClientLeft      =   600
   ClientTop       =   1365
   ClientWidth     =   6315
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3780
   ScaleWidth      =   6315
   ShowInTaskbar   =   0   'False
   Begin VB.Frame fraLoopRange 
      Caption         =   "Loop Range for BNK"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1215
      Left            =   120
      TabIndex        =   14
      Top             =   2520
      Visible         =   0   'False
      Width           =   6135
      Begin MSComctlLib.Slider sldLoopEnd 
         Height          =   255
         Left            =   840
         TabIndex        =   20
         Top             =   840
         Width           =   4215
         _ExtentX        =   7435
         _ExtentY        =   450
         _Version        =   393216
      End
      Begin MSComctlLib.Slider sldLoopStart 
         Height          =   255
         Left            =   840
         TabIndex        =   19
         Top             =   240
         Width           =   4215
         _ExtentX        =   7435
         _ExtentY        =   450
         _Version        =   393216
      End
      Begin VB.Label lblLoopEnd 
         Alignment       =   2  'Center
         BackColor       =   &H80000010&
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   5160
         TabIndex        =   18
         Top             =   840
         Width           =   855
      End
      Begin VB.Label Label3 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "End :"
         Height          =   255
         Left            =   120
         TabIndex        =   17
         Top             =   840
         Width           =   615
      End
      Begin VB.Label lblLoopStart 
         Alignment       =   2  'Center
         BackColor       =   &H80000010&
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   5160
         TabIndex        =   16
         Top             =   240
         Width           =   855
      End
      Begin VB.Label Label2 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Start :"
         Height          =   255
         Left            =   120
         TabIndex        =   15
         Top             =   240
         Width           =   615
      End
   End
   Begin VB.CommandButton cmdSet 
      Caption         =   "Set"
      Height          =   375
      Left            =   4200
      TabIndex        =   13
      Top             =   1320
      Width           =   975
   End
   Begin VB.CommandButton cmdClose 
      Caption         =   "Close"
      Height          =   375
      Left            =   5280
      TabIndex        =   12
      Top             =   1320
      Width           =   975
   End
   Begin VB.Frame fraPanning 
      Caption         =   "Sound Panning for BNK"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1215
      Left            =   120
      TabIndex        =   4
      Top             =   1200
      Visible         =   0   'False
      Width           =   6135
      Begin VB.OptionButton optpan 
         Height          =   375
         Index           =   0
         Left            =   5040
         TabIndex        =   6
         Top             =   480
         Width           =   255
      End
      Begin VB.OptionButton optpan 
         Height          =   375
         Index           =   4
         Left            =   5040
         TabIndex        =   10
         Top             =   720
         Width           =   255
      End
      Begin VB.OptionButton optpan 
         Height          =   375
         Index           =   3
         Left            =   5040
         TabIndex        =   9
         Top             =   240
         Width           =   255
      End
      Begin VB.OptionButton optpan 
         Height          =   375
         Index           =   2
         Left            =   5280
         TabIndex        =   8
         Top             =   480
         Width           =   255
      End
      Begin VB.OptionButton optpan 
         Height          =   375
         Index           =   1
         Left            =   4800
         TabIndex        =   7
         Top             =   480
         Width           =   255
      End
      Begin VB.Label lblpan 
         Alignment       =   2  'Center
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   1320
         TabIndex        =   11
         Top             =   360
         Width           =   1455
      End
      Begin VB.Label Label4 
         Alignment       =   1  'Right Justify
         Caption         =   "Value"
         Height          =   255
         Left            =   120
         TabIndex        =   5
         Top             =   360
         Width           =   735
      End
   End
   Begin VB.Frame fraVol 
      Caption         =   "Sound Volume for BNK"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1215
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Visible         =   0   'False
      Width           =   6135
      Begin MSComctlLib.Slider sldVol 
         Height          =   375
         Left            =   120
         TabIndex        =   3
         Top             =   720
         Width           =   5895
         _ExtentX        =   10398
         _ExtentY        =   661
         _Version        =   393216
         SmallChange     =   5
         Max             =   255
      End
      Begin VB.Label lblSoundVol 
         Alignment       =   2  'Center
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   1320
         TabIndex        =   2
         Top             =   360
         Width           =   1455
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Caption         =   "Volume:"
         Height          =   255
         Left            =   120
         TabIndex        =   1
         Top             =   360
         Width           =   1095
      End
   End
End
Attribute VB_Name = "frmSpecial"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
    
Dim theType As String

Private Sub cmdClose_Click()
    Unload Me
End Sub

Private Sub cmdSet_Click()
    Dim which As Integer
    
    which = frmMain.lstbnkeditorcontents.ListIndex
    
    Select Case theType
        Case "BNKVOL"
            frmMain.lblBnkEditVol.Caption = lblSoundVol.Caption
            theBnkContents(which).vol = Val(lblSoundVol.Caption)
        Case "BNKPAN"
            frmMain.lblBnkEditPanning.Caption = lblpan.Caption
            theBnkContents(which).pan = Val(lblpan.Caption)
        Case "LOOPSET"
            
            If (Val(lblLoopEnd.Caption) = 0) Or (Val(lblLoopStart.Caption) = Val(lblLoopEnd.Caption)) Then
                frmMain.chkBnkEditLoopOnPause.Value = 0
                theBnkContents(which).loop = False
                theBnkContents(which).loops = 0
                theBnkContents(which).loope = 0
                frmMain.lblBnkEditLooping.Caption = "N/A"
            Else
                frmMain.chkBnkEditLoopOnPause.Value = 1
                theBnkContents(which).loop = True
                theBnkContents(which).loops = Val(lblLoopStart.Caption)
                theBnkContents(which).loope = Val(lblLoopEnd.Caption)
                frmMain.lblBnkEditLooping.Caption = lblLoopStart.Caption & "-" & lblLoopEnd.Caption
            End If
    End Select
    
    Unload Me
End Sub

Public Sub setType(which As String)
    theType = which
    Select Case (theType)
        Case "BNKVOL"
            fraVol.Visible = True
            fraPanning.Visible = False
            fraVol.Top = 0
            fraVol.Left = 120
            lblSoundVol.Caption = frmMain.lblBnkEditVol.Caption
            sldVol.Value = Val(lblSoundVol.Caption)
            Me.Caption = "Volume Control"
        Case "BNKPAN"
            fraPanning.Visible = True
            fraVol.Visible = False
            fraPanning.Top = 0
            fraPanning.Left = 120
            lblpan.Caption = frmMain.lblBnkEditPanning.Caption
            Me.Caption = "Sound Panning Control"
            Select Case lblpan.Caption
                Case "0"
                    optpan(4).Value = True
                Case "-63"
                    optpan(1).Value = True
                Case "63"
                    optpan(2).Value = True
                Case "-127"
                    optpan(0).Value = True
                Case "127"
                    optpan(3).Value = True
            End Select
        Case "LOOPSET"
            fraLoopRange.Visible = True
            fraLoopRange.Top = 0
            fraLoopRange.Left = 120
            lblLoopStart.Caption = 0
            lblLoopEnd.Caption = 0
            Me.Caption = "Set the Sound Looping Range"
    End Select
End Sub

Private Sub Form_Activate()
    sldLoopStart.LargeChange = sldLoopStart.Max / 10
    sldLoopEnd.LargeChange = sldLoopEnd.Max / 10
    sldLoopStart.SmallChange = sldLoopStart.Max / 50
    sldLoopEnd.SmallChange = sldLoopStart.Max / 50
    
End Sub

Private Sub optpan_Click(Index As Integer)
    Select Case Index
        Case 0:
            lblpan.Caption = Str(0)
        Case 1:
            lblpan.Caption = Str(-63)
        Case 2:
            lblpan.Caption = Str(63)
        Case 3:
            lblpan.Caption = Str(-127)
        Case 4:
            lblpan.Caption = Str(127)
    End Select
    
End Sub

Private Sub sldLoopEnd_Change()
    lblLoopEnd.Caption = Trim(Str(sldLoopEnd.Value))
    If (sldLoopEnd.Value < sldLoopStart.Value) Then
        sldLoopStart.Value = sldLoopEnd.Value
    End If
End Sub

Private Sub sldLoopStart_Change()
    lblLoopStart.Caption = Trim(Str(sldLoopStart.Value))
    If (sldLoopStart.Value > sldLoopEnd.Value) Then
        sldLoopEnd.Value = sldLoopStart.Value
    End If
End Sub

Private Sub sldVol_Click()
    lblSoundVol.Caption = Str(sldVol.Value)
End Sub
