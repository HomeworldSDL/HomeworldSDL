VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Subtitle Duration Repair"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   9270
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   9270
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdPhraseFileBrowse 
      Caption         =   "<"
      Height          =   255
      Left            =   7800
      TabIndex        =   2
      Top             =   120
      Width           =   255
   End
   Begin VB.Label lblPhraseFile 
      BorderStyle     =   1  'Fixed Single
      Height          =   255
      Left            =   1440
      TabIndex        =   1
      Top             =   120
      Width           =   6375
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Phrase File:"
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   1095
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
