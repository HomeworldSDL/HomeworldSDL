VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "HWSoundStudio"
   ClientHeight    =   10905
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   17190
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   10905
   ScaleWidth      =   17190
   Begin VB.Frame Frame5 
      Caption         =   "I'll fix these sections later...after I get general BNK editing working!"
      Height          =   5655
      Left            =   10320
      TabIndex        =   176
      Top             =   3480
      Visible         =   0   'False
      Width           =   6855
      Begin TabDlg.SSTab SSTab6 
         Height          =   5295
         Left            =   120
         TabIndex        =   177
         Top             =   240
         Width           =   6735
         _ExtentX        =   11880
         _ExtentY        =   9340
         _Version        =   393216
         Tabs            =   2
         TabsPerRow      =   2
         TabHeight       =   520
         TabCaption(0)   =   "BNK Builder"
         TabPicture(0)   =   "frmMain.frx":0000
         Tab(0).ControlEnabled=   -1  'True
         Tab(0).Control(0)=   "Shape14(0)"
         Tab(0).Control(0).Enabled=   0   'False
         Tab(0).Control(1)=   "Label61"
         Tab(0).Control(1).Enabled=   0   'False
         Tab(0).Control(2)=   "Label60"
         Tab(0).Control(2).Enabled=   0   'False
         Tab(0).Control(3)=   "Label44"
         Tab(0).Control(3).Enabled=   0   'False
         Tab(0).Control(4)=   "Label43(0)"
         Tab(0).Control(4).Enabled=   0   'False
         Tab(0).Control(5)=   "prgBnkBuild"
         Tab(0).Control(5).Enabled=   0   'False
         Tab(0).Control(6)=   "Frame3"
         Tab(0).Control(6).Enabled=   0   'False
         Tab(0).Control(7)=   "cmdBnkBuild"
         Tab(0).Control(7).Enabled=   0   'False
         Tab(0).Control(8)=   "fileBnkBuildSource"
         Tab(0).Control(8).Enabled=   0   'False
         Tab(0).Control(9)=   "drvBnkBuildSource"
         Tab(0).Control(9).Enabled=   0   'False
         Tab(0).Control(10)=   "dirBnkBuildSource"
         Tab(0).Control(10).Enabled=   0   'False
         Tab(0).Control(11)=   "lstBnkBuildResults"
         Tab(0).Control(11).Enabled=   0   'False
         Tab(0).Control(12)=   "cmdBrowseNewBnkPath"
         Tab(0).Control(12).Enabled=   0   'False
         Tab(0).Control(13)=   "txtNewBnkFilePath"
         Tab(0).Control(13).Enabled=   0   'False
         Tab(0).ControlCount=   14
         TabCaption(1)   =   "Event Editor"
         TabPicture(1)   =   "frmMain.frx":001C
         Tab(1).ControlEnabled=   0   'False
         Tab(1).Control(0)=   "cmdPathBnkBuildEventfile"
         Tab(1).Control(1)=   "lstBnkEventLutEditorResults"
         Tab(1).Control(2)=   "cmdLUTEventEditorSave"
         Tab(1).Control(3)=   "cmboBnkLutEditSoundNum"
         Tab(1).Control(4)=   "cmboBnkLutEditVarNum"
         Tab(1).Control(5)=   "cmdBnkLutEditBnkPathBrowse"
         Tab(1).Control(6)=   "cmdBnkLutEditProcess"
         Tab(1).Control(7)=   "grdLut"
         Tab(1).Control(8)=   "updnBnkLutEditSeqOffsetNum(0)"
         Tab(1).Control(9)=   "updnBnkLutEditSeqOffsetNum(1)"
         Tab(1).Control(10)=   "updnBnkLutEditSeqOffsetNum(2)"
         Tab(1).Control(11)=   "Label63"
         Tab(1).Control(12)=   "Label64"
         Tab(1).Control(13)=   "lblBnkBuildEventfile"
         Tab(1).Control(14)=   "Label46"
         Tab(1).Control(15)=   "Label43(1)"
         Tab(1).Control(16)=   "Label59"
         Tab(1).Control(17)=   "Label62"
         Tab(1).Control(18)=   "lblBnkLutEditSeq1(0)"
         Tab(1).Control(19)=   "Label65"
         Tab(1).Control(20)=   "lblBnkLutEditBnkFilePath"
         Tab(1).Control(21)=   "lblBnkLutEditSeq1(1)"
         Tab(1).Control(22)=   "lblBnkLutEditSeq1(2)"
         Tab(1).Control(23)=   "imgBnkLutEditArrow(0)"
         Tab(1).Control(24)=   "imgBnkLutEditArrow(1)"
         Tab(1).Control(25)=   "imgBnkLutEditArrow(2)"
         Tab(1).Control(26)=   "Shape14(1)"
         Tab(1).ControlCount=   27
         Begin VB.CommandButton cmdPathBnkBuildEventfile 
            Caption         =   "<"
            Enabled         =   0   'False
            Height          =   255
            Left            =   -68640
            TabIndex        =   200
            Top             =   960
            Width           =   255
         End
         Begin VB.ListBox lstBnkEventLutEditorResults 
            BackColor       =   &H8000000B&
            Height          =   645
            Left            =   -74880
            TabIndex        =   199
            Top             =   4560
            Width           =   6855
         End
         Begin VB.CommandButton cmdLUTEventEditorSave 
            Caption         =   "Save"
            Enabled         =   0   'False
            Height          =   375
            Left            =   -69480
            TabIndex        =   198
            Top             =   4080
            Width           =   1335
         End
         Begin VB.ComboBox cmboBnkLutEditSoundNum 
            Enabled         =   0   'False
            Height          =   315
            Left            =   -74640
            Style           =   2  'Dropdown List
            TabIndex        =   197
            Top             =   1920
            Width           =   1215
         End
         Begin VB.ComboBox cmboBnkLutEditVarNum 
            Enabled         =   0   'False
            Height          =   315
            Left            =   -73080
            Style           =   2  'Dropdown List
            TabIndex        =   196
            Top             =   1920
            Width           =   855
         End
         Begin VB.CommandButton cmdBnkLutEditBnkPathBrowse 
            Caption         =   "<"
            Enabled         =   0   'False
            Height          =   255
            Left            =   -68640
            TabIndex        =   195
            Top             =   1320
            Width           =   255
         End
         Begin VB.CommandButton cmdBnkLutEditProcess 
            Caption         =   "Process"
            Enabled         =   0   'False
            Height          =   375
            Left            =   -72000
            TabIndex        =   192
            Top             =   1920
            Width           =   1095
         End
         Begin VB.TextBox txtNewBnkFilePath 
            Height          =   285
            Left            =   1320
            TabIndex        =   187
            Top             =   960
            Width           =   5175
         End
         Begin VB.CommandButton cmdBrowseNewBnkPath 
            Caption         =   "<"
            Enabled         =   0   'False
            Height          =   255
            Left            =   6360
            TabIndex        =   186
            Top             =   840
            Width           =   255
         End
         Begin VB.ListBox lstBnkBuildResults 
            BackColor       =   &H8000000B&
            Height          =   1035
            Left            =   360
            TabIndex        =   185
            Top             =   4200
            Width           =   6375
         End
         Begin VB.DirListBox dirBnkBuildSource 
            Enabled         =   0   'False
            Height          =   2115
            Left            =   360
            TabIndex        =   184
            Top             =   1560
            Width           =   1695
         End
         Begin VB.DriveListBox drvBnkBuildSource 
            Enabled         =   0   'False
            Height          =   315
            Left            =   360
            TabIndex        =   183
            Top             =   3680
            Width           =   1695
         End
         Begin VB.FileListBox fileBnkBuildSource 
            Height          =   2040
            Left            =   2400
            Pattern         =   "*.aifrb"
            TabIndex        =   182
            Top             =   1560
            Width           =   1815
         End
         Begin VB.CommandButton cmdBnkBuild 
            Caption         =   "Build BNK && LUT"
            Enabled         =   0   'False
            Height          =   375
            Left            =   4680
            TabIndex        =   181
            Top             =   3600
            Width           =   1935
         End
         Begin VB.Frame Frame3 
            Caption         =   "Notes"
            Height          =   2055
            Left            =   4680
            TabIndex        =   179
            Top             =   1440
            Width           =   1935
            Begin VB.Label lblBnkBuildNotes 
               Caption         =   "---This section, and the Event Editor section, are not working right yet."
               Height          =   1575
               Left            =   120
               TabIndex        =   180
               Top             =   360
               Width           =   1695
            End
         End
         Begin MSComctlLib.ProgressBar prgBnkBuild 
            Height          =   225
            Left            =   360
            TabIndex        =   178
            Top             =   3990
            Visible         =   0   'False
            Width           =   6375
            _ExtentX        =   11245
            _ExtentY        =   397
            _Version        =   393216
            Appearance      =   1
         End
         Begin MSFlexGridLib.MSFlexGrid grdLut 
            Height          =   1335
            Left            =   -70680
            TabIndex        =   193
            Top             =   1920
            Width           =   2535
            _ExtentX        =   4471
            _ExtentY        =   2355
            _Version        =   393216
            Rows            =   3
            Cols            =   4
            FixedRows       =   0
            FixedCols       =   0
            Enabled         =   0   'False
            Appearance      =   0
         End
         Begin MSComCtl2.UpDown updnBnkLutEditSeqOffsetNum 
            Height          =   255
            Index           =   0
            Left            =   -74160
            TabIndex        =   194
            Top             =   2640
            Width           =   255
            _ExtentX        =   450
            _ExtentY        =   450
            _Version        =   393216
            Enabled         =   0   'False
         End
         Begin MSComCtl2.UpDown updnBnkLutEditSeqOffsetNum 
            Height          =   255
            Index           =   1
            Left            =   -73320
            TabIndex        =   201
            Top             =   2640
            Width           =   255
            _ExtentX        =   450
            _ExtentY        =   450
            _Version        =   393216
            Enabled         =   0   'False
         End
         Begin MSComCtl2.UpDown updnBnkLutEditSeqOffsetNum 
            Height          =   255
            Index           =   2
            Left            =   -72480
            TabIndex        =   202
            Top             =   2640
            Width           =   255
            _ExtentX        =   450
            _ExtentY        =   450
            _Version        =   393216
            Enabled         =   0   'False
         End
         Begin VB.Label Label63 
            Caption         =   "Sequence of BNK indexes"
            Height          =   255
            Left            =   -74640
            TabIndex        =   214
            Top             =   2400
            Width           =   2415
         End
         Begin VB.Label Label64 
            Caption         =   "Current Variation Sequences"
            Height          =   255
            Left            =   -70680
            TabIndex        =   213
            Top             =   1680
            Width           =   2535
         End
         Begin VB.Label lblBnkBuildEventfile 
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -73800
            TabIndex        =   212
            Top             =   960
            Width           =   5175
         End
         Begin VB.Label Label46 
            Alignment       =   1  'Right Justify
            Caption         =   "event file:"
            Height          =   255
            Left            =   -74880
            TabIndex        =   211
            Top             =   960
            Width           =   975
         End
         Begin VB.Label Label43 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "Edit a SoundFX LUT file"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H8000000F&
            Height          =   255
            Index           =   1
            Left            =   -74880
            TabIndex        =   210
            Top             =   555
            Width           =   6735
         End
         Begin VB.Label Label59 
            Caption         =   "Sound Event"
            Height          =   255
            Left            =   -74640
            TabIndex        =   209
            Top             =   1680
            Width           =   1215
         End
         Begin VB.Label Label62 
            Caption         =   "Variation"
            Height          =   255
            Left            =   -73080
            TabIndex        =   208
            Top             =   1680
            Width           =   855
         End
         Begin VB.Label lblBnkLutEditSeq1 
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Index           =   0
            Left            =   -74640
            TabIndex        =   207
            Top             =   2640
            Width           =   495
         End
         Begin VB.Label Label65 
            Alignment       =   1  'Right Justify
            Caption         =   "BNK file:"
            Height          =   255
            Left            =   -74760
            TabIndex        =   206
            Top             =   1320
            Width           =   855
         End
         Begin VB.Label lblBnkLutEditBnkFilePath 
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -73800
            TabIndex        =   205
            Top             =   1320
            Width           =   5175
         End
         Begin VB.Label lblBnkLutEditSeq1 
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Index           =   1
            Left            =   -73800
            TabIndex        =   204
            Top             =   2640
            Width           =   495
         End
         Begin VB.Label lblBnkLutEditSeq1 
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Index           =   2
            Left            =   -72960
            TabIndex        =   203
            Top             =   2640
            Width           =   495
         End
         Begin VB.Image imgBnkLutEditArrow 
            Height          =   135
            Index           =   0
            Left            =   -70800
            Picture         =   "frmMain.frx":0038
            Top             =   1920
            Visible         =   0   'False
            Width           =   150
         End
         Begin VB.Image imgBnkLutEditArrow 
            Height          =   135
            Index           =   1
            Left            =   -70800
            Picture         =   "frmMain.frx":019A
            Top             =   2160
            Visible         =   0   'False
            Width           =   150
         End
         Begin VB.Image imgBnkLutEditArrow 
            Height          =   135
            Index           =   2
            Left            =   -70800
            Picture         =   "frmMain.frx":02FC
            Top             =   2400
            Visible         =   0   'False
            Width           =   150
         End
         Begin VB.Label Label43 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "Construct a completely new Sound Bank [BNK] file"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H8000000F&
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   191
            Top             =   555
            Width           =   6735
         End
         Begin VB.Label Label44 
            Alignment       =   1  'Right Justify
            Caption         =   "New bnk file:"
            Height          =   255
            Left            =   120
            TabIndex        =   190
            Top             =   960
            Width           =   1095
         End
         Begin VB.Label Label60 
            Caption         =   "Source Dir"
            Height          =   255
            Left            =   360
            TabIndex        =   189
            Top             =   1320
            Width           =   1455
         End
         Begin VB.Label Label61 
            Caption         =   "Sound Files"
            Height          =   255
            Left            =   2400
            TabIndex        =   188
            Top             =   1320
            Width           =   975
         End
         Begin VB.Shape Shape14 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Index           =   0
            Left            =   120
            Shape           =   4  'Rounded Rectangle
            Top             =   480
            Width           =   6855
         End
         Begin VB.Shape Shape14 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Index           =   1
            Left            =   -74880
            Shape           =   4  'Rounded Rectangle
            Top             =   480
            Width           =   6855
         End
      End
   End
   Begin VB.CommandButton cmdAudioEditor 
      BackColor       =   &H80000010&
      Caption         =   "Advanced"
      Enabled         =   0   'False
      Height          =   375
      Left            =   240
      Style           =   1  'Graphical
      TabIndex        =   139
      Top             =   4800
      Width           =   975
   End
   Begin MSComDlg.CommonDialog cd 
      Left            =   480
      Top             =   6720
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton cmdSettings 
      BackColor       =   &H80000010&
      Caption         =   "Settings"
      Enabled         =   0   'False
      Height          =   375
      Left            =   240
      Style           =   1  'Graphical
      TabIndex        =   12
      Top             =   5640
      Width           =   975
   End
   Begin VB.CommandButton cmdDistribute 
      BackColor       =   &H80000010&
      Caption         =   "Distribute"
      Height          =   375
      Left            =   240
      Style           =   1  'Graphical
      TabIndex        =   8
      Top             =   3960
      Width           =   975
   End
   Begin VB.CommandButton cmdVoice 
      BackColor       =   &H80000010&
      Caption         =   "Speech"
      Height          =   375
      Left            =   240
      Style           =   1  'Graphical
      TabIndex        =   4
      Top             =   3120
      Width           =   975
   End
   Begin VB.CommandButton cmdSound 
      BackColor       =   &H80000010&
      Caption         =   "SoundFX"
      Height          =   375
      Left            =   240
      Style           =   1  'Graphical
      TabIndex        =   3
      Top             =   2280
      Width           =   975
   End
   Begin VB.CommandButton cmdMusic 
      BackColor       =   &H80000010&
      Caption         =   "Music"
      Height          =   375
      Left            =   240
      Style           =   1  'Graphical
      TabIndex        =   2
      Top             =   1440
      Width           =   975
   End
   Begin VB.CommandButton cmdConvert 
      BackColor       =   &H80000010&
      Caption         =   "Convert"
      Height          =   375
      Left            =   240
      Style           =   1  'Graphical
      TabIndex        =   1
      Top             =   600
      Width           =   975
   End
   Begin VB.Frame fraConversion 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   6255
      Left            =   1440
      TabIndex        =   5
      Top             =   0
      Width           =   7335
      Begin TabDlg.SSTab SSTab1 
         Height          =   5415
         Left            =   120
         TabIndex        =   7
         Top             =   720
         Width           =   7095
         _ExtentX        =   12515
         _ExtentY        =   9551
         _Version        =   393216
         TabHeight       =   520
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         TabCaption(0)   =   "mp3 -> wav"
         TabPicture(0)   =   "frmMain.frx":045E
         Tab(0).ControlEnabled=   -1  'True
         Tab(0).Control(0)=   "Label3"
         Tab(0).Control(0).Enabled=   0   'False
         Tab(0).Control(1)=   "Shape2"
         Tab(0).Control(1).Enabled=   0   'False
         Tab(0).Control(2)=   "Label4"
         Tab(0).Control(2).Enabled=   0   'False
         Tab(0).Control(3)=   "Label5"
         Tab(0).Control(3).Enabled=   0   'False
         Tab(0).Control(4)=   "dirmp32wav"
         Tab(0).Control(4).Enabled=   0   'False
         Tab(0).Control(5)=   "drvmp32wav"
         Tab(0).Control(5).Enabled=   0   'False
         Tab(0).Control(6)=   "cmdmp32wavconvert"
         Tab(0).Control(6).Enabled=   0   'False
         Tab(0).Control(7)=   "lstmp32wavresults"
         Tab(0).Control(7).Enabled=   0   'False
         Tab(0).Control(8)=   "filemp32wavlist"
         Tab(0).Control(8).Enabled=   0   'False
         Tab(0).ControlCount=   9
         TabCaption(1)   =   "wav -> aifr"
         TabPicture(1)   =   "frmMain.frx":047A
         Tab(1).ControlEnabled=   0   'False
         Tab(1).Control(0)=   "Shape3"
         Tab(1).Control(1)=   "Label6"
         Tab(1).Control(2)=   "Label10"
         Tab(1).Control(3)=   "Label11"
         Tab(1).Control(4)=   "Label12"
         Tab(1).Control(5)=   "Label45"
         Tab(1).Control(6)=   "dirWavSource"
         Tab(1).Control(7)=   "drvWavSource"
         Tab(1).Control(8)=   "fileWav"
         Tab(1).Control(9)=   "cmdwav2aifrEncode"
         Tab(1).Control(10)=   "lstwav2aifrResults"
         Tab(1).Control(11)=   "cmboAifrEncodingBitrate"
         Tab(1).ControlCount=   12
         TabCaption(2)   =   "Extraction"
         TabPicture(2)   =   "frmMain.frx":0496
         Tab(2).ControlEnabled=   0   'False
         Tab(2).Control(0)=   "lblExtractNumSounds"
         Tab(2).Control(0).Enabled=   0   'False
         Tab(2).Control(1)=   "Label69"
         Tab(2).Control(1).Enabled=   0   'False
         Tab(2).Control(2)=   "Label68"
         Tab(2).Control(2).Enabled=   0   'False
         Tab(2).Control(3)=   "Label70"
         Tab(2).Control(3).Enabled=   0   'False
         Tab(2).Control(4)=   "progExtractSounds"
         Tab(2).Control(4).Enabled=   0   'False
         Tab(2).Control(5)=   "cmdExtractSounds"
         Tab(2).Control(5).Enabled=   0   'False
         Tab(2).Control(6)=   "txtExtractFilePath"
         Tab(2).Control(6).Enabled=   0   'False
         Tab(2).Control(7)=   "cmdExtractFilePathBrowse"
         Tab(2).Control(7).Enabled=   0   'False
         Tab(2).Control(8)=   "optExtractBNK"
         Tab(2).Control(8).Enabled=   0   'False
         Tab(2).Control(9)=   "optExtractWXD"
         Tab(2).Control(9).Enabled=   0   'False
         Tab(2).Control(10)=   "optExtractVCE"
         Tab(2).Control(10).Enabled=   0   'False
         Tab(2).ControlCount=   11
         Begin VB.OptionButton optExtractVCE 
            Caption         =   "Voice extraction (VCE)"
            Enabled         =   0   'False
            Height          =   255
            Left            =   -73320
            TabIndex        =   224
            Top             =   2160
            Width           =   3855
         End
         Begin VB.OptionButton optExtractWXD 
            Caption         =   "Music extraction (WXD)"
            Enabled         =   0   'False
            Height          =   255
            Left            =   -73320
            TabIndex        =   223
            Top             =   1800
            Width           =   3735
         End
         Begin VB.OptionButton optExtractBNK 
            Caption         =   "SoundFX extraction (BNK)"
            Height          =   195
            Left            =   -73320
            TabIndex        =   222
            Top             =   1440
            Width           =   4335
         End
         Begin VB.CommandButton cmdExtractFilePathBrowse 
            Caption         =   "<"
            Height          =   255
            Left            =   -68520
            TabIndex        =   221
            Top             =   3000
            Width           =   255
         End
         Begin VB.TextBox txtExtractFilePath 
            Height          =   285
            Left            =   -73920
            TabIndex        =   220
            Top             =   3000
            Width           =   5415
         End
         Begin VB.CommandButton cmdExtractSounds 
            Caption         =   "Extract"
            Height          =   255
            Left            =   -69240
            TabIndex        =   215
            Top             =   3600
            Width           =   1095
         End
         Begin VB.ComboBox cmboAifrEncodingBitrate 
            Height          =   315
            ItemData        =   "frmMain.frx":04B2
            Left            =   -72600
            List            =   "frmMain.frx":04CB
            Style           =   2  'Dropdown List
            TabIndex        =   32
            Top             =   1680
            Width           =   2175
         End
         Begin VB.ListBox lstwav2aifrResults 
            BackColor       =   &H8000000B&
            Height          =   1425
            Left            =   -74760
            TabIndex        =   30
            Top             =   3840
            Width           =   6615
         End
         Begin VB.CommandButton cmdwav2aifrEncode 
            Caption         =   "Encode"
            Height          =   375
            Left            =   -72120
            TabIndex        =   29
            Top             =   2280
            Width           =   1215
         End
         Begin VB.FileListBox filemp32wavlist 
            Height          =   2625
            Left            =   4440
            Pattern         =   "*.mp3"
            TabIndex        =   27
            Top             =   1200
            Width           =   2415
         End
         Begin VB.FileListBox fileWav 
            Height          =   2625
            Left            =   -70200
            Pattern         =   "*.wav"
            TabIndex        =   26
            Top             =   1200
            Width           =   2175
         End
         Begin VB.DriveListBox drvWavSource 
            Height          =   315
            Left            =   -74760
            TabIndex        =   25
            Top             =   3480
            Width           =   1935
         End
         Begin VB.DirListBox dirWavSource 
            Height          =   2340
            Left            =   -74760
            TabIndex        =   24
            Top             =   1200
            Width           =   1935
         End
         Begin VB.ListBox lstmp32wavresults 
            BackColor       =   &H8000000B&
            Height          =   1425
            Left            =   240
            TabIndex        =   17
            Top             =   3840
            Width           =   6615
         End
         Begin VB.CommandButton cmdmp32wavconvert 
            Caption         =   "Convert"
            Height          =   375
            Left            =   2880
            TabIndex        =   16
            Top             =   2160
            Width           =   1215
         End
         Begin VB.DriveListBox drvmp32wav 
            Height          =   315
            Left            =   240
            TabIndex        =   10
            Top             =   3480
            Width           =   2175
         End
         Begin VB.DirListBox dirmp32wav 
            Height          =   2340
            Left            =   240
            TabIndex        =   13
            Top             =   1200
            Width           =   2175
         End
         Begin MSComctlLib.ProgressBar progExtractSounds 
            Height          =   135
            Left            =   -74760
            TabIndex        =   218
            Top             =   4200
            Width           =   6615
            _ExtentX        =   11668
            _ExtentY        =   238
            _Version        =   393216
            BorderStyle     =   1
            Appearance      =   0
         End
         Begin VB.Label Label70 
            Caption         =   "Extraction Type"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   -1  'True
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   -73440
            TabIndex        =   225
            Top             =   1080
            Width           =   2415
         End
         Begin VB.Label Label68 
            Alignment       =   1  'Right Justify
            Caption         =   "File Path"
            Height          =   255
            Left            =   -74760
            TabIndex        =   219
            Top             =   3000
            Width           =   735
         End
         Begin VB.Label Label69 
            Caption         =   "# of Sounds"
            Height          =   255
            Left            =   -74760
            TabIndex        =   217
            Top             =   3600
            Width           =   1095
         End
         Begin VB.Label lblExtractNumSounds 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -73440
            TabIndex        =   216
            Top             =   3600
            Width           =   975
         End
         Begin VB.Label Label45 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "For Speech sounds, use the built-in encoder in the VCEPacker section"
            ForeColor       =   &H8000000D&
            Height          =   735
            Left            =   -72480
            TabIndex        =   128
            Top             =   2880
            Width           =   1935
         End
         Begin VB.Label Label12 
            Alignment       =   2  'Center
            Caption         =   "Encoding Bitrate"
            Height          =   255
            Left            =   -72720
            TabIndex        =   31
            Top             =   1440
            Width           =   2415
         End
         Begin VB.Label Label11 
            Caption         =   "wav list"
            Height          =   255
            Left            =   -70080
            TabIndex        =   28
            Top             =   960
            Width           =   1935
         End
         Begin VB.Label Label10 
            Caption         =   "wav source directory"
            Height          =   255
            Left            =   -74760
            TabIndex        =   23
            Top             =   960
            Width           =   2175
         End
         Begin VB.Label Label6 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "wav -> aifr"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00E0E0E0&
            Height          =   255
            Left            =   -74760
            TabIndex        =   18
            Top             =   555
            Width           =   6615
         End
         Begin VB.Shape Shape3 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Left            =   -74880
            Shape           =   4  'Rounded Rectangle
            Top             =   480
            Width           =   6855
         End
         Begin VB.Label Label5 
            Caption         =   "mp3 list"
            Height          =   255
            Left            =   4560
            TabIndex        =   15
            Top             =   960
            Width           =   2055
         End
         Begin VB.Label Label4 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "mp3 -> wav"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00E0E0E0&
            Height          =   255
            Left            =   240
            TabIndex        =   14
            Top             =   555
            Width           =   6615
         End
         Begin VB.Shape Shape2 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Left            =   120
            Shape           =   4  'Rounded Rectangle
            Top             =   480
            Width           =   6855
         End
         Begin VB.Label Label3 
            Caption         =   "mp3 source file directory"
            Height          =   255
            Left            =   240
            TabIndex        =   11
            Top             =   960
            Width           =   2175
         End
      End
      Begin VB.Label lblConvType 
         BackStyle       =   0  'Transparent
         Caption         =   "Common File Conversions"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0E0E0&
         Height          =   255
         Index           =   0
         Left            =   240
         TabIndex        =   6
         Top             =   315
         Width           =   4095
      End
      Begin VB.Shape Shape1 
         BackColor       =   &H8000000C&
         BackStyle       =   1  'Opaque
         Height          =   375
         Index           =   0
         Left            =   120
         Shape           =   4  'Rounded Rectangle
         Top             =   240
         Width           =   7095
      End
   End
   Begin VB.Frame fraSound 
      Height          =   6255
      Left            =   2040
      TabIndex        =   70
      Top             =   1200
      Width           =   7335
      Begin TabDlg.SSTab SSTab3 
         Height          =   5415
         Left            =   120
         TabIndex        =   72
         Top             =   720
         Width           =   7095
         _ExtentX        =   12515
         _ExtentY        =   9551
         _Version        =   393216
         Tabs            =   2
         TabsPerRow      =   2
         TabHeight       =   520
         TabCaption(0)   =   "aifr -> aifrb"
         TabPicture(0)   =   "frmMain.frx":04F4
         Tab(0).ControlEnabled=   -1  'True
         Tab(0).Control(0)=   "Shape12"
         Tab(0).Control(0).Enabled=   0   'False
         Tab(0).Control(1)=   "Label30"
         Tab(0).Control(1).Enabled=   0   'False
         Tab(0).Control(2)=   "Label31"
         Tab(0).Control(2).Enabled=   0   'False
         Tab(0).Control(3)=   "Label32"
         Tab(0).Control(3).Enabled=   0   'False
         Tab(0).Control(4)=   "dirAifr2AifrbSource"
         Tab(0).Control(4).Enabled=   0   'False
         Tab(0).Control(5)=   "drvAifr2AifrbSource"
         Tab(0).Control(5).Enabled=   0   'False
         Tab(0).Control(6)=   "fileAifr2AifrbList"
         Tab(0).Control(6).Enabled=   0   'False
         Tab(0).Control(7)=   "lstaifr2aifrbresults"
         Tab(0).Control(7).Enabled=   0   'False
         Tab(0).Control(8)=   "cmdaifr2aifrbconvert"
         Tab(0).Control(8).Enabled=   0   'False
         Tab(0).ControlCount=   9
         TabCaption(1)   =   "BNK Editor"
         TabPicture(1)   =   "frmMain.frx":0510
         Tab(1).ControlEnabled=   0   'False
         Tab(1).Control(0)=   "Label33"
         Tab(1).Control(0).Enabled=   0   'False
         Tab(1).Control(1)=   "Shape13"
         Tab(1).Control(1).Enabled=   0   'False
         Tab(1).Control(2)=   "Label34"
         Tab(1).Control(2).Enabled=   0   'False
         Tab(1).Control(3)=   "Label35"
         Tab(1).Control(3).Enabled=   0   'False
         Tab(1).Control(4)=   "Label36"
         Tab(1).Control(4).Enabled=   0   'False
         Tab(1).Control(5)=   "Label37"
         Tab(1).Control(5).Enabled=   0   'False
         Tab(1).Control(6)=   "Label38"
         Tab(1).Control(6).Enabled=   0   'False
         Tab(1).Control(7)=   "Label39"
         Tab(1).Control(7).Enabled=   0   'False
         Tab(1).Control(8)=   "Label40"
         Tab(1).Control(8).Enabled=   0   'False
         Tab(1).Control(9)=   "lblBNKEditorNewSoundOffset"
         Tab(1).Control(9).Enabled=   0   'False
         Tab(1).Control(10)=   "Label42"
         Tab(1).Control(10).Enabled=   0   'False
         Tab(1).Control(11)=   "lblBNKEditorNewSoundLength"
         Tab(1).Control(11).Enabled=   0   'False
         Tab(1).Control(12)=   "Label47"
         Tab(1).Control(12).Enabled=   0   'False
         Tab(1).Control(13)=   "Label67"
         Tab(1).Control(13).Enabled=   0   'False
         Tab(1).Control(14)=   "lblBnkEditorNumBits"
         Tab(1).Control(14).Enabled=   0   'False
         Tab(1).Control(15)=   "txtbnkeditorbnkpath"
         Tab(1).Control(15).Enabled=   0   'False
         Tab(1).Control(16)=   "cmdBNKEditorBNKPath"
         Tab(1).Control(16).Enabled=   0   'False
         Tab(1).Control(17)=   "lstbnkeditorcontents"
         Tab(1).Control(17).Enabled=   0   'False
         Tab(1).Control(18)=   "txtBNKeditorNewSoundPath"
         Tab(1).Control(18).Enabled=   0   'False
         Tab(1).Control(19)=   "cmdBNKEditorNewSoundPath"
         Tab(1).Control(19).Enabled=   0   'False
         Tab(1).Control(20)=   "txtBNKEditorNewBitrate"
         Tab(1).Control(20).Enabled=   0   'False
         Tab(1).Control(21)=   "fraBnkEditFrequency"
         Tab(1).Control(21).Enabled=   0   'False
         Tab(1).Control(22)=   "fraBnkEditNumChannels"
         Tab(1).Control(22).Enabled=   0   'False
         Tab(1).Control(23)=   "lstBNKEditorResults"
         Tab(1).Control(23).Enabled=   0   'False
         Tab(1).Control(24)=   "cmdBnkEditorSaveChanges"
         Tab(1).Control(24).Enabled=   0   'False
         Tab(1).Control(25)=   "cmdBnkEditorSet"
         Tab(1).Control(25).Enabled=   0   'False
         Tab(1).Control(26)=   "Frame2"
         Tab(1).Control(26).Enabled=   0   'False
         Tab(1).Control(27)=   "chkBnkEditLoopOnPause"
         Tab(1).Control(27).Enabled=   0   'False
         Tab(1).Control(28)=   "cmdBnkEditCancel"
         Tab(1).Control(28).Enabled=   0   'False
         Tab(1).Control(29)=   "cmdBnkEditorReset"
         Tab(1).Control(29).Enabled=   0   'False
         Tab(1).Control(30)=   "prgBnkEditorProgress"
         Tab(1).Control(30).Enabled=   0   'False
         Tab(1).ControlCount=   31
         Begin MSComctlLib.ProgressBar prgBnkEditorProgress 
            Height          =   195
            Left            =   -74760
            TabIndex        =   173
            Top             =   3945
            Width           =   3495
            _ExtentX        =   6165
            _ExtentY        =   344
            _Version        =   393216
            Appearance      =   1
         End
         Begin VB.CommandButton cmdBnkEditorReset 
            Caption         =   "Reset"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   -71160
            TabIndex        =   172
            Top             =   3900
            Width           =   1455
         End
         Begin VB.CommandButton cmdBnkEditCancel 
            Caption         =   "Cancel"
            Enabled         =   0   'False
            Height          =   255
            Left            =   -69000
            TabIndex        =   138
            Top             =   2220
            Width           =   975
         End
         Begin VB.CheckBox chkBnkEditLoopOnPause 
            Alignment       =   1  'Right Justify
            Caption         =   "Sound Looping Capability"
            Enabled         =   0   'False
            Height          =   255
            Left            =   -74760
            TabIndex        =   137
            Top             =   3660
            Visible         =   0   'False
            Width           =   2535
         End
         Begin VB.Frame Frame2 
            Height          =   975
            Left            =   -71160
            TabIndex        =   130
            Top             =   2580
            Width           =   3135
            Begin VB.CommandButton BnkEditSetLooping 
               Caption         =   "config"
               Enabled         =   0   'False
               Height          =   255
               Left            =   2280
               TabIndex        =   171
               Top             =   600
               Width           =   735
            End
            Begin VB.CommandButton BnkEditSetPanning 
               Caption         =   "config"
               Enabled         =   0   'False
               Height          =   255
               Left            =   2280
               TabIndex        =   134
               Top             =   360
               Width           =   735
            End
            Begin VB.CommandButton BnkEditSetVol 
               Caption         =   "config"
               Enabled         =   0   'False
               Height          =   255
               Left            =   2280
               TabIndex        =   133
               Top             =   120
               Width           =   735
            End
            Begin VB.Label lblBnkEditLooping 
               Alignment       =   2  'Center
               BorderStyle     =   1  'Fixed Single
               Height          =   255
               Left            =   960
               TabIndex        =   170
               Top             =   600
               Width           =   1215
            End
            Begin VB.Label Label66 
               Alignment       =   1  'Right Justify
               Caption         =   "looping"
               Height          =   255
               Left            =   120
               TabIndex        =   169
               Top             =   600
               Width           =   735
            End
            Begin VB.Label lblBnkEditPanning 
               Alignment       =   2  'Center
               BorderStyle     =   1  'Fixed Single
               Height          =   255
               Left            =   960
               TabIndex        =   136
               Top             =   360
               Width           =   615
            End
            Begin VB.Label lblBnkEditVol 
               Alignment       =   2  'Center
               BorderStyle     =   1  'Fixed Single
               Height          =   255
               Left            =   960
               TabIndex        =   135
               Top             =   120
               Width           =   615
            End
            Begin VB.Label Label49 
               Alignment       =   1  'Right Justify
               Caption         =   "panning"
               Height          =   255
               Left            =   120
               TabIndex        =   132
               Top             =   360
               Width           =   735
            End
            Begin VB.Label Label48 
               Alignment       =   1  'Right Justify
               Caption         =   "volume"
               Height          =   255
               Left            =   120
               TabIndex        =   131
               Top             =   120
               Width           =   735
            End
         End
         Begin VB.CommandButton cmdBnkEditorSet 
            Caption         =   "Set"
            Enabled         =   0   'False
            Height          =   255
            Left            =   -69000
            TabIndex        =   127
            Top             =   1980
            Width           =   975
         End
         Begin VB.CommandButton cmdBnkEditorSaveChanges 
            Caption         =   "Save"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   -69600
            Style           =   1  'Graphical
            TabIndex        =   118
            Top             =   3900
            Width           =   1455
         End
         Begin VB.ListBox lstBNKEditorResults 
            BackColor       =   &H8000000B&
            Height          =   1035
            Left            =   -74760
            TabIndex        =   117
            Top             =   4140
            Width           =   6615
         End
         Begin VB.Frame fraBnkEditNumChannels 
            Enabled         =   0   'False
            Height          =   400
            Left            =   -71160
            TabIndex        =   110
            Top             =   2235
            Width           =   2040
            Begin VB.OptionButton optBNKEditorStereo 
               Caption         =   "stereo"
               Height          =   255
               Left            =   120
               TabIndex        =   112
               Top             =   150
               Width           =   855
            End
            Begin VB.OptionButton optBNKEditorMono 
               Caption         =   "mono"
               Height          =   255
               Left            =   1080
               TabIndex        =   111
               Top             =   150
               Width           =   735
            End
         End
         Begin VB.Frame fraBnkEditFrequency 
            Enabled         =   0   'False
            Height          =   400
            Left            =   -71160
            TabIndex        =   106
            Top             =   1860
            Width           =   2055
            Begin VB.OptionButton optBNKEditor44khz 
               Caption         =   "44khz"
               Height          =   255
               Left            =   120
               TabIndex        =   108
               Top             =   120
               Width           =   855
            End
            Begin VB.OptionButton optBNKEditor22khz 
               Caption         =   "22khz"
               Height          =   255
               Left            =   1080
               TabIndex        =   107
               Top             =   120
               Width           =   855
            End
         End
         Begin VB.TextBox txtBNKEditorNewBitrate 
            Enabled         =   0   'False
            Height          =   285
            Left            =   -71160
            TabIndex        =   104
            Top             =   1620
            Width           =   1215
         End
         Begin VB.CommandButton cmdBNKEditorNewSoundPath 
            Caption         =   "<"
            Enabled         =   0   'False
            Height          =   255
            Left            =   -68400
            TabIndex        =   102
            Top             =   1200
            Width           =   255
         End
         Begin VB.TextBox txtBNKeditorNewSoundPath 
            Enabled         =   0   'False
            Height          =   285
            Left            =   -72120
            TabIndex        =   101
            Top             =   1200
            Width           =   3735
         End
         Begin VB.ListBox lstbnkeditorcontents 
            BeginProperty Font 
               Name            =   "Arial"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   1980
            Left            =   -74760
            Style           =   1  'Checkbox
            TabIndex        =   99
            Top             =   1620
            Width           =   2535
         End
         Begin VB.CommandButton cmdBNKEditorBNKPath 
            Caption         =   "<"
            Height          =   255
            Left            =   -68400
            TabIndex        =   97
            Top             =   840
            Width           =   255
         End
         Begin VB.TextBox txtbnkeditorbnkpath 
            Height          =   285
            Left            =   -73920
            TabIndex        =   95
            Top             =   840
            Width           =   5535
         End
         Begin VB.CommandButton cmdaifr2aifrbconvert 
            Caption         =   "convert"
            Height          =   375
            Left            =   3000
            TabIndex        =   93
            Top             =   2220
            Width           =   1095
         End
         Begin VB.ListBox lstaifr2aifrbresults 
            BackColor       =   &H8000000B&
            Height          =   1230
            Left            =   240
            TabIndex        =   92
            Top             =   3900
            Width           =   6615
         End
         Begin VB.FileListBox fileAifr2AifrbList 
            Height          =   2625
            Left            =   4560
            Pattern         =   "*.aifr"
            TabIndex        =   90
            Top             =   1140
            Width           =   2295
         End
         Begin VB.DriveListBox drvAifr2AifrbSource 
            Height          =   315
            Left            =   240
            TabIndex        =   89
            Top             =   3420
            Width           =   2295
         End
         Begin VB.DirListBox dirAifr2AifrbSource 
            Height          =   2340
            Left            =   240
            TabIndex        =   88
            Top             =   1140
            Width           =   2295
         End
         Begin VB.Label lblBnkEditorNumBits 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -68640
            TabIndex        =   175
            Top             =   1620
            Width           =   495
         End
         Begin VB.Label Label67 
            Alignment       =   1  'Right Justify
            Caption         =   "sample size"
            Height          =   255
            Left            =   -69840
            TabIndex        =   174
            Top             =   1620
            Width           =   1095
         End
         Begin VB.Label Label47 
            Alignment       =   1  'Right Justify
            Caption         =   "special"
            Height          =   255
            Left            =   -72000
            TabIndex        =   129
            Top             =   2700
            Width           =   735
         End
         Begin VB.Label lblBNKEditorNewSoundLength 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -69120
            TabIndex        =   116
            Top             =   3540
            Width           =   975
         End
         Begin VB.Label Label42 
            Alignment       =   1  'Right Justify
            Caption         =   "length"
            Height          =   255
            Left            =   -69960
            TabIndex        =   115
            Top             =   3540
            Width           =   735
         End
         Begin VB.Label lblBNKEditorNewSoundOffset 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -71160
            TabIndex        =   114
            Top             =   3540
            Width           =   1095
         End
         Begin VB.Label Label40 
            Alignment       =   1  'Right Justify
            Caption         =   "offset"
            Height          =   255
            Left            =   -72240
            TabIndex        =   113
            Top             =   3540
            Width           =   975
         End
         Begin VB.Label Label39 
            Alignment       =   1  'Right Justify
            Caption         =   "channels"
            Height          =   255
            Left            =   -72240
            TabIndex        =   109
            Top             =   2340
            Width           =   975
         End
         Begin VB.Label Label38 
            Alignment       =   1  'Right Justify
            Caption         =   "frequency"
            Height          =   255
            Left            =   -72240
            TabIndex        =   105
            Top             =   1980
            Width           =   975
         End
         Begin VB.Label Label37 
            Alignment       =   1  'Right Justify
            Caption         =   "bitrate"
            Height          =   255
            Left            =   -72120
            TabIndex        =   103
            Top             =   1620
            Width           =   855
         End
         Begin VB.Label Label36 
            Alignment       =   1  'Right Justify
            Caption         =   "Replacement Sound"
            Height          =   255
            Left            =   -74040
            TabIndex        =   100
            Top             =   1200
            Width           =   1815
         End
         Begin VB.Label Label35 
            Caption         =   "bnk contents"
            Height          =   255
            Left            =   -74760
            TabIndex        =   98
            Top             =   1440
            Width           =   1575
         End
         Begin VB.Label Label34 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "Replace sounds in an existing Sound Bank [BNK] file"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00E0E0E0&
            Height          =   255
            Left            =   -74760
            TabIndex        =   96
            Top             =   495
            Width           =   6615
         End
         Begin VB.Shape Shape13 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Left            =   -74880
            Shape           =   4  'Rounded Rectangle
            Top             =   420
            Width           =   6855
         End
         Begin VB.Label Label33 
            Caption         =   "bnk file"
            Height          =   255
            Left            =   -74760
            TabIndex        =   94
            Top             =   900
            Width           =   1215
         End
         Begin VB.Label Label32 
            Caption         =   "aifr file list"
            Height          =   255
            Left            =   4560
            TabIndex        =   91
            Top             =   960
            Width           =   1455
         End
         Begin VB.Label Label31 
            Caption         =   "aifr source directory"
            Height          =   255
            Left            =   240
            TabIndex        =   87
            Top             =   900
            Width           =   1695
         End
         Begin VB.Label Label30 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "aifr -> aifrb"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00E0E0E0&
            Height          =   255
            Left            =   240
            TabIndex        =   86
            Top             =   495
            Width           =   6615
         End
         Begin VB.Shape Shape12 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Left            =   120
            Shape           =   4  'Rounded Rectangle
            Top             =   420
            Width           =   6855
         End
      End
      Begin VB.Label Label24 
         BackStyle       =   0  'Transparent
         Caption         =   "Sound Effects Customization"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0E0E0&
         Height          =   255
         Left            =   240
         TabIndex        =   71
         Top             =   330
         Width           =   6855
      End
      Begin VB.Shape Shape8 
         BackColor       =   &H8000000C&
         BackStyle       =   1  'Opaque
         Height          =   375
         Left            =   120
         Shape           =   4  'Rounded Rectangle
         Top             =   240
         Width           =   7095
      End
   End
   Begin VB.Frame fraDist 
      Height          =   6255
      Left            =   2520
      TabIndex        =   76
      Top             =   2400
      Width           =   7335
      Begin TabDlg.SSTab SSTab5 
         Height          =   5415
         Left            =   120
         TabIndex        =   78
         Top             =   720
         Width           =   7095
         _ExtentX        =   12515
         _ExtentY        =   9551
         _Version        =   393216
         TabHeight       =   520
         TabCaption(0)   =   "Music"
         TabPicture(0)   =   "frmMain.frx":052C
         Tab(0).ControlEnabled=   -1  'True
         Tab(0).ControlCount=   0
         TabCaption(1)   =   "Sound"
         TabPicture(1)   =   "frmMain.frx":0548
         Tab(1).ControlEnabled=   0   'False
         Tab(1).ControlCount=   0
         TabCaption(2)   =   "Speech"
         TabPicture(2)   =   "frmMain.frx":0564
         Tab(2).ControlEnabled=   0   'False
         Tab(2).ControlCount=   0
      End
      Begin VB.Label Label26 
         BackStyle       =   0  'Transparent
         Caption         =   "Distribution Files"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0E0E0&
         Height          =   255
         Left            =   240
         TabIndex        =   77
         Top             =   330
         Width           =   6855
      End
      Begin VB.Shape Shape10 
         BackColor       =   &H8000000C&
         BackStyle       =   1  'Opaque
         Height          =   375
         Left            =   120
         Shape           =   4  'Rounded Rectangle
         Top             =   240
         Width           =   7095
      End
   End
   Begin VB.Frame fraSettings 
      Height          =   6255
      Left            =   2880
      TabIndex        =   79
      Top             =   3000
      Width           =   7335
      Begin VB.CommandButton cmdSaveSettings 
         Caption         =   "Save"
         Height          =   375
         Left            =   5880
         TabIndex        =   153
         Top             =   5040
         Width           =   1095
      End
      Begin VB.Frame Frame4 
         BorderStyle     =   0  'None
         Height          =   495
         Left            =   3120
         TabIndex        =   120
         Top             =   1680
         Width           =   855
         Begin VB.OptionButton optFlashOff 
            Caption         =   "Off"
            Height          =   255
            Left            =   120
            TabIndex        =   122
            Top             =   240
            Width           =   1095
         End
         Begin VB.OptionButton optFlashOn 
            Caption         =   "On"
            Height          =   255
            Left            =   120
            TabIndex        =   121
            Top             =   0
            Width           =   1095
         End
      End
      Begin VB.CommandButton cmdSettingsGameDir 
         Caption         =   "<"
         Height          =   255
         Left            =   6960
         TabIndex        =   85
         Top             =   1320
         Width           =   255
      End
      Begin VB.TextBox txtSettingsGameDir 
         Height          =   285
         Left            =   3240
         TabIndex        =   84
         Top             =   1320
         Width           =   3735
      End
      Begin VB.DriveListBox drvSettingsTempWorkDrive 
         Enabled         =   0   'False
         Height          =   315
         Left            =   3240
         TabIndex        =   82
         Top             =   840
         Width           =   1215
      End
      Begin VB.Label Label41 
         Caption         =   "Flashing Notices?"
         Height          =   255
         Left            =   240
         TabIndex        =   119
         Top             =   1800
         Width           =   2775
      End
      Begin VB.Label Label29 
         Caption         =   "Game Directory"
         Height          =   255
         Left            =   240
         TabIndex        =   83
         Top             =   1320
         Width           =   2775
      End
      Begin VB.Label Label28 
         Caption         =   "Drive to use for temporary work folder"
         Height          =   255
         Left            =   240
         TabIndex        =   81
         Top             =   840
         Width           =   3135
      End
      Begin VB.Label Label27 
         BackStyle       =   0  'Transparent
         Caption         =   "Program Settings"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0E0E0&
         Height          =   255
         Left            =   240
         TabIndex        =   80
         Top             =   330
         Width           =   6855
      End
      Begin VB.Shape Shape11 
         BackColor       =   &H8000000C&
         BackStyle       =   1  'Opaque
         Height          =   375
         Left            =   120
         Shape           =   4  'Rounded Rectangle
         Top             =   240
         Width           =   7095
      End
   End
   Begin VB.Frame fraMusic 
      Height          =   6255
      Left            =   3240
      TabIndex        =   19
      Top             =   3720
      Width           =   7335
      Begin TabDlg.SSTab SSTab2 
         Height          =   5415
         Left            =   120
         TabIndex        =   20
         Top             =   720
         Width           =   7095
         _ExtentX        =   12515
         _ExtentY        =   9551
         _Version        =   393216
         Tabs            =   4
         TabsPerRow      =   4
         TabHeight       =   520
         TabCaption(0)   =   "aifr -> aifrx"
         TabPicture(0)   =   "frmMain.frx":0580
         Tab(0).ControlEnabled=   -1  'True
         Tab(0).Control(0)=   "Shape6"
         Tab(0).Control(0).Enabled=   0   'False
         Tab(0).Control(1)=   "Label8"
         Tab(0).Control(1).Enabled=   0   'False
         Tab(0).Control(2)=   "Label17"
         Tab(0).Control(2).Enabled=   0   'False
         Tab(0).Control(3)=   "Label18"
         Tab(0).Control(3).Enabled=   0   'False
         Tab(0).Control(4)=   "diraifr2aifrx"
         Tab(0).Control(4).Enabled=   0   'False
         Tab(0).Control(5)=   "drvaifr2aifrx"
         Tab(0).Control(5).Enabled=   0   'False
         Tab(0).Control(6)=   "fileAifrfiles"
         Tab(0).Control(6).Enabled=   0   'False
         Tab(0).Control(7)=   "cmdAifr2Aifrx"
         Tab(0).Control(7).Enabled=   0   'False
         Tab(0).Control(8)=   "lstaifr2aifrxresults"
         Tab(0).Control(8).Enabled=   0   'False
         Tab(0).Control(9)=   "progAifr2Aifrx"
         Tab(0).Control(9).Enabled=   0   'False
         Tab(0).ControlCount=   10
         TabCaption(1)   =   "Add songs"
         TabPicture(1)   =   "frmMain.frx":059C
         Tab(1).ControlEnabled=   0   'False
         Tab(1).Control(0)=   "Shape7"
         Tab(1).Control(0).Enabled=   0   'False
         Tab(1).Control(1)=   "Label13"
         Tab(1).Control(1).Enabled=   0   'False
         Tab(1).Control(2)=   "Label14"
         Tab(1).Control(2).Enabled=   0   'False
         Tab(1).Control(3)=   "Label16"
         Tab(1).Control(3).Enabled=   0   'False
         Tab(1).Control(4)=   "lstAifrx2Wxd"
         Tab(1).Control(4).Enabled=   0   'False
         Tab(1).Control(5)=   "updnAifrx2wxd"
         Tab(1).Control(5).Enabled=   0   'False
         Tab(1).Control(6)=   "cmdAddAifr2Wxd"
         Tab(1).Control(6).Enabled=   0   'False
         Tab(1).Control(7)=   "cmdRemoveAifr2Wxd"
         Tab(1).Control(7).Enabled=   0   'False
         Tab(1).Control(8)=   "lstaifrx2wxdresults"
         Tab(1).Control(8).Enabled=   0   'False
         Tab(1).Control(9)=   "cmdBuildWXD"
         Tab(1).Control(9).Enabled=   0   'False
         Tab(1).Control(10)=   "Frame1"
         Tab(1).Control(10).Enabled=   0   'False
         Tab(1).Control(11)=   "txtaifr2wxdPath2WXD"
         Tab(1).Control(11).Enabled=   0   'False
         Tab(1).Control(12)=   "cmdaifr2wxdPath2WXD"
         Tab(1).Control(12).Enabled=   0   'False
         Tab(1).Control(13)=   "cmdClear"
         Tab(1).Control(13).Enabled=   0   'False
         Tab(1).Control(14)=   "progCustomWXD"
         Tab(1).Control(14).Enabled=   0   'False
         Tab(1).ControlCount=   15
         TabCaption(2)   =   "Track editor"
         TabPicture(2)   =   "frmMain.frx":05B8
         Tab(2).ControlEnabled=   0   'False
         Tab(2).Control(0)=   "cmdWxhEditHeaderQuickReference"
         Tab(2).Control(1)=   "cmdwxheditprocess"
         Tab(2).Control(2)=   "cmdwxheditsave"
         Tab(2).Control(3)=   "lstwxheditresults"
         Tab(2).Control(4)=   "cmdWXHEditAttach"
         Tab(2).Control(5)=   "txtwxheditbitrate"
         Tab(2).Control(6)=   "chkwxheditskirmish"
         Tab(2).Control(7)=   "lstWXD"
         Tab(2).Control(8)=   "lstWXH"
         Tab(2).Control(9)=   "cmdWXHEditorWXDPath"
         Tab(2).Control(10)=   "cmdWXHEditorWXHPath"
         Tab(2).Control(11)=   "txtWXHEditorWXDPath"
         Tab(2).Control(12)=   "txtWXHEditorWXHPath"
         Tab(2).Control(13)=   "lblwxdeditbitratenote"
         Tab(2).Control(14)=   "Label23"
         Tab(2).Control(15)=   "Label22"
         Tab(2).Control(16)=   "Label21"
         Tab(2).Control(17)=   "Label20"
         Tab(2).Control(18)=   "Label19"
         Tab(2).Control(19)=   "Label9"
         Tab(2).Control(20)=   "Shape5"
         Tab(2).ControlCount=   21
         TabCaption(3)   =   "New Songs/Tracks"
         TabPicture(3)   =   "frmMain.frx":05D4
         Tab(3).ControlEnabled=   0   'False
         Tab(3).Control(0)=   "Label50"
         Tab(3).Control(1)=   "Label51"
         Tab(3).Control(2)=   "Label52"
         Tab(3).Control(3)=   "Label53"
         Tab(3).Control(4)=   "lblNewSongTracksNumSongs"
         Tab(3).Control(5)=   "lblNewSongTracksNumBlanks"
         Tab(3).Control(6)=   "labNewSongTracksSongNum"
         Tab(3).Control(7)=   "lblNewSongTracksSongNum"
         Tab(3).Control(8)=   "Label54"
         Tab(3).Control(9)=   "lblNewSongTracksOffset"
         Tab(3).Control(10)=   "Shape15"
         Tab(3).Control(11)=   "Label55"
         Tab(3).Control(12)=   "Label56"
         Tab(3).Control(13)=   "Label57"
         Tab(3).Control(14)=   "Label58"
         Tab(3).Control(15)=   "dirNewSongTrackDir"
         Tab(3).Control(16)=   "drvNewSongTrackDir"
         Tab(3).Control(17)=   "fileNewSongTrack"
         Tab(3).Control(18)=   "lstNewSongTracksResults"
         Tab(3).Control(19)=   "progNewMusicOverall"
         Tab(3).Control(20)=   "cmdNewSongTracksBuildFiles"
         Tab(3).Control(21)=   "chkNewSongTracksAllSkirmish"
         Tab(3).Control(22)=   "txtNewMusicGameDir"
         Tab(3).Control(23)=   "cmdNewMusicGameDir"
         Tab(3).Control(24)=   "progNewSongTrack"
         Tab(3).ControlCount=   25
         Begin MSComctlLib.ProgressBar progNewSongTrack 
            Height          =   195
            Left            =   -73800
            TabIndex        =   166
            Top             =   3765
            Width           =   5775
            _ExtentX        =   10186
            _ExtentY        =   344
            _Version        =   393216
            Appearance      =   1
         End
         Begin VB.CommandButton cmdNewMusicGameDir 
            Caption         =   "<"
            Height          =   255
            Left            =   -68280
            TabIndex        =   165
            Top             =   1200
            Width           =   255
         End
         Begin VB.TextBox txtNewMusicGameDir 
            Height          =   285
            Left            =   -71280
            TabIndex        =   164
            Top             =   1200
            Width           =   3015
         End
         Begin VB.CheckBox chkNewSongTracksAllSkirmish 
            Caption         =   "All Skirmish"
            Height          =   255
            Left            =   -69480
            TabIndex        =   157
            Top             =   2280
            Width           =   1215
         End
         Begin VB.CommandButton cmdNewSongTracksBuildFiles 
            Caption         =   "Build Files"
            Height          =   375
            Left            =   -70320
            TabIndex        =   156
            Top             =   2640
            Width           =   1575
         End
         Begin MSComctlLib.ProgressBar progNewMusicOverall 
            Height          =   195
            Left            =   -73800
            TabIndex        =   150
            Top             =   3960
            Width           =   5775
            _ExtentX        =   10186
            _ExtentY        =   344
            _Version        =   393216
            Appearance      =   1
         End
         Begin VB.ListBox lstNewSongTracksResults 
            BackColor       =   &H8000000B&
            Height          =   1035
            Left            =   -74880
            TabIndex        =   149
            Top             =   4200
            Width           =   6855
         End
         Begin VB.FileListBox fileNewSongTrack 
            Height          =   2235
            Left            =   -73080
            Pattern         =   "m*.aifrx"
            TabIndex        =   148
            Top             =   1200
            Width           =   1695
         End
         Begin VB.DriveListBox drvNewSongTrackDir 
            Height          =   315
            Left            =   -74880
            TabIndex        =   146
            Top             =   3240
            Width           =   1575
         End
         Begin VB.DirListBox dirNewSongTrackDir 
            Height          =   2115
            Left            =   -74880
            TabIndex        =   145
            Top             =   1200
            Width           =   1575
         End
         Begin MSComctlLib.ProgressBar progCustomWXD 
            Height          =   200
            Left            =   -74880
            TabIndex        =   143
            Top             =   3900
            Visible         =   0   'False
            Width           =   6855
            _ExtentX        =   12091
            _ExtentY        =   344
            _Version        =   393216
            Appearance      =   1
         End
         Begin MSComctlLib.ProgressBar progAifr2Aifrx 
            Height          =   195
            Left            =   240
            TabIndex        =   142
            Top             =   3900
            Visible         =   0   'False
            Width           =   6615
            _ExtentX        =   11668
            _ExtentY        =   344
            _Version        =   393216
            Appearance      =   1
         End
         Begin VB.CommandButton cmdWxhEditHeaderQuickReference 
            Caption         =   "!"
            Height          =   255
            Left            =   -72720
            TabIndex        =   125
            ToolTipText     =   "List the missions that use these tracks"
            Top             =   1800
            Width           =   255
         End
         Begin VB.CommandButton cmdwxheditprocess 
            Caption         =   "process"
            Height          =   255
            Left            =   -72000
            TabIndex        =   124
            Top             =   1560
            Width           =   975
         End
         Begin VB.CommandButton cmdClear 
            Caption         =   "Clear"
            Height          =   375
            Left            =   -73200
            TabIndex        =   123
            Top             =   3360
            Width           =   855
         End
         Begin VB.CommandButton cmdwxheditsave 
            Caption         =   "Save Changes"
            Enabled         =   0   'False
            Height          =   375
            Left            =   -72240
            TabIndex        =   69
            Top             =   3600
            Width           =   1455
         End
         Begin VB.ListBox lstwxheditresults 
            BackColor       =   &H8000000B&
            Height          =   1230
            Left            =   -74760
            TabIndex        =   68
            Top             =   4080
            Width           =   6735
         End
         Begin VB.CommandButton cmdWXHEditAttach 
            Caption         =   "Attach"
            Enabled         =   0   'False
            Height          =   375
            Left            =   -72120
            TabIndex        =   67
            ToolTipText     =   "Set the selected song (in the WXD) to the selected track (in the WXH)"
            Top             =   1920
            Width           =   1215
         End
         Begin VB.TextBox txtwxheditbitrate 
            Height          =   285
            Left            =   -72240
            TabIndex        =   66
            ToolTipText     =   "Make sure this matches the encoded bitrate of the new song or it won't play right!"
            Top             =   3240
            Width           =   1335
         End
         Begin VB.CheckBox chkwxheditskirmish 
            Caption         =   "Skirmish Mode"
            Height          =   255
            Left            =   -72240
            TabIndex        =   64
            ToolTipText     =   "Turning this on allows the use of this track in skirmish matches"
            Top             =   2760
            Width           =   1455
         End
         Begin VB.ListBox lstWXD 
            Height          =   2205
            Left            =   -70320
            TabIndex        =   63
            ToolTipText     =   "These 'songs' are what is played when the track in the 'wxh' file is specified in a mission."
            Top             =   1800
            Width           =   2175
         End
         Begin VB.ListBox lstWXH 
            Height          =   2205
            Left            =   -74760
            TabIndex        =   60
            ToolTipText     =   "The 'tracks' to play.  These point to 'songs' in the wxd file to play"
            Top             =   1800
            Width           =   2055
         End
         Begin VB.CommandButton cmdWXHEditorWXDPath 
            Caption         =   "<"
            Height          =   255
            Left            =   -68400
            TabIndex        =   59
            Top             =   1200
            Width           =   255
         End
         Begin VB.CommandButton cmdWXHEditorWXHPath 
            Caption         =   "<"
            Height          =   255
            Left            =   -71880
            TabIndex        =   58
            Top             =   1200
            Width           =   255
         End
         Begin VB.TextBox txtWXHEditorWXDPath 
            Height          =   285
            Left            =   -71400
            TabIndex        =   57
            Top             =   1200
            Width           =   3015
         End
         Begin VB.TextBox txtWXHEditorWXHPath 
            Height          =   285
            Left            =   -74880
            TabIndex        =   55
            Top             =   1200
            Width           =   3015
         End
         Begin VB.ListBox lstaifr2aifrxresults 
            BackColor       =   &H8000000B&
            Height          =   1035
            Left            =   240
            TabIndex        =   53
            Top             =   4200
            Width           =   6615
         End
         Begin VB.CommandButton cmdAifr2Aifrx 
            Caption         =   "convert"
            Height          =   375
            Left            =   2880
            TabIndex        =   52
            Top             =   2160
            Width           =   1335
         End
         Begin VB.FileListBox fileAifrfiles 
            Height          =   2625
            Left            =   4680
            Pattern         =   "*.aifr"
            TabIndex        =   50
            Top             =   1200
            Width           =   2175
         End
         Begin VB.DriveListBox drvaifr2aifrx 
            Height          =   315
            Left            =   240
            TabIndex        =   49
            Top             =   3480
            Width           =   2175
         End
         Begin VB.DirListBox diraifr2aifrx 
            Height          =   2340
            Left            =   240
            TabIndex        =   48
            Top             =   1200
            Width           =   2175
         End
         Begin VB.CommandButton cmdaifr2wxdPath2WXD 
            Caption         =   "<"
            Height          =   255
            Left            =   -68520
            TabIndex        =   46
            Top             =   1200
            Width           =   255
         End
         Begin VB.TextBox txtaifr2wxdPath2WXD 
            Height          =   285
            Left            =   -72240
            TabIndex        =   45
            Top             =   1200
            Width           =   3735
         End
         Begin VB.Frame Frame1 
            Caption         =   "Notes"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   -1  'True
               Strikethrough   =   0   'False
            EndProperty
            Height          =   1695
            Left            =   -72120
            TabIndex        =   42
            Top             =   1680
            Width           =   3975
            Begin VB.Label Label15 
               Caption         =   $"frmMain.frx":05F0
               Height          =   1335
               Left            =   120
               TabIndex        =   43
               Top             =   240
               Width           =   3735
            End
         End
         Begin VB.CommandButton cmdBuildWXD 
            Caption         =   "Build WXD"
            Height          =   375
            Left            =   -70800
            TabIndex        =   41
            Top             =   3480
            Width           =   1335
         End
         Begin VB.ListBox lstaifrx2wxdresults 
            BackColor       =   &H8000000B&
            Height          =   1035
            Left            =   -74880
            TabIndex        =   40
            Top             =   4200
            Width           =   6855
         End
         Begin VB.CommandButton cmdRemoveAifr2Wxd 
            Caption         =   "Remove"
            Height          =   375
            Left            =   -74040
            TabIndex        =   39
            Top             =   3360
            Width           =   855
         End
         Begin VB.CommandButton cmdAddAifr2Wxd 
            Caption         =   "Add"
            Height          =   375
            Left            =   -74880
            TabIndex        =   38
            Top             =   3360
            Width           =   855
         End
         Begin MSComCtl2.UpDown updnAifrx2wxd 
            Height          =   495
            Left            =   -72360
            TabIndex        =   37
            Top             =   2040
            Width           =   195
            _ExtentX        =   450
            _ExtentY        =   873
            _Version        =   393216
            Enabled         =   -1  'True
         End
         Begin VB.ListBox lstAifrx2Wxd 
            Height          =   2205
            Left            =   -74880
            TabIndex        =   36
            Top             =   1200
            Width           =   2535
         End
         Begin VB.Label Label58 
            Alignment       =   1  'Right Justify
            Caption         =   "Overall:"
            Height          =   255
            Left            =   -74880
            TabIndex        =   168
            Top             =   3960
            Width           =   975
         End
         Begin VB.Label Label57 
            Alignment       =   1  'Right Justify
            Caption         =   "This file:"
            Height          =   255
            Left            =   -74880
            TabIndex        =   167
            Top             =   3720
            Width           =   975
         End
         Begin VB.Label Label56 
            Caption         =   "Game Dir"
            Height          =   255
            Left            =   -71280
            TabIndex        =   163
            Top             =   960
            Width           =   855
         End
         Begin VB.Label Label55 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "Build Completely new, replacement WXD [song] and WXH [track] files"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00E0E0E0&
            Height          =   255
            Left            =   -74880
            TabIndex        =   162
            Top             =   555
            Width           =   6855
         End
         Begin VB.Shape Shape15 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Left            =   -74880
            Shape           =   4  'Rounded Rectangle
            Top             =   480
            Width           =   6855
         End
         Begin VB.Label lblNewSongTracksOffset 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -70200
            TabIndex        =   161
            Top             =   3480
            Width           =   1335
         End
         Begin VB.Label Label54 
            Alignment       =   1  'Right Justify
            Caption         =   "Offset:"
            Height          =   255
            Left            =   -71160
            TabIndex        =   160
            Top             =   3480
            Width           =   855
         End
         Begin VB.Label lblNewSongTracksSongNum 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -70200
            TabIndex        =   159
            Top             =   3120
            Width           =   1335
         End
         Begin VB.Label labNewSongTracksSongNum 
            Alignment       =   1  'Right Justify
            Caption         =   "Song slot:"
            Height          =   255
            Left            =   -71160
            TabIndex        =   158
            Top             =   3120
            Width           =   855
         End
         Begin VB.Label lblNewSongTracksNumBlanks 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -69480
            TabIndex        =   155
            Top             =   1920
            Width           =   1455
         End
         Begin VB.Label lblNewSongTracksNumSongs 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Height          =   255
            Left            =   -69480
            TabIndex        =   154
            Top             =   1560
            Width           =   1455
         End
         Begin VB.Label Label53 
            Alignment       =   1  'Right Justify
            Caption         =   "Number of Blanks"
            Height          =   255
            Left            =   -71280
            TabIndex        =   152
            Top             =   1920
            Width           =   1575
         End
         Begin VB.Label Label52 
            Alignment       =   1  'Right Justify
            Caption         =   "Number of Songs"
            Height          =   255
            Left            =   -71280
            TabIndex        =   151
            Top             =   1560
            Width           =   1575
         End
         Begin VB.Label Label51 
            Caption         =   "Music Files Found"
            Height          =   255
            Left            =   -73080
            TabIndex        =   147
            Top             =   960
            Width           =   1575
         End
         Begin VB.Label Label50 
            Caption         =   "Music Files directory"
            Height          =   255
            Left            =   -74880
            TabIndex        =   144
            Top             =   960
            Width           =   1815
         End
         Begin VB.Label lblwxdeditbitratenote 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "Make sure the bitrate matches!!!!"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   -1  'True
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H000000FF&
            Height          =   375
            Left            =   -72600
            TabIndex        =   126
            Top             =   2280
            Visible         =   0   'False
            Width           =   2175
         End
         Begin VB.Label Label23 
            Caption         =   "Bitrate"
            Height          =   255
            Left            =   -72240
            TabIndex        =   65
            Top             =   3000
            Width           =   735
         End
         Begin VB.Label Label22 
            Alignment       =   1  'Right Justify
            Caption         =   "wxd song listing"
            Height          =   255
            Left            =   -70680
            TabIndex        =   62
            Top             =   1560
            Width           =   2535
         End
         Begin VB.Label Label21 
            Caption         =   "wxh track listings"
            Height          =   255
            Left            =   -74760
            TabIndex        =   61
            Top             =   1560
            Width           =   1455
         End
         Begin VB.Label Label20 
            Caption         =   "Game WXH"
            Height          =   255
            Left            =   -74880
            TabIndex        =   56
            Top             =   960
            Width           =   1695
         End
         Begin VB.Label Label19 
            Caption         =   "Game WXD"
            Height          =   255
            Left            =   -71400
            TabIndex        =   54
            Top             =   960
            Width           =   2055
         End
         Begin VB.Label Label18 
            Caption         =   "aifr files"
            Height          =   255
            Left            =   4800
            TabIndex        =   51
            Top             =   960
            Width           =   1815
         End
         Begin VB.Label Label17 
            Caption         =   "aifr source directory"
            Height          =   255
            Left            =   240
            TabIndex        =   47
            Top             =   960
            Width           =   2655
         End
         Begin VB.Label Label16 
            Caption         =   "Path to Main Game WXD"
            Height          =   255
            Left            =   -72240
            TabIndex        =   44
            Top             =   960
            Width           =   3255
         End
         Begin VB.Label Label14 
            Caption         =   "aifrx file(s) to include"
            Height          =   255
            Left            =   -74760
            TabIndex        =   35
            Top             =   960
            Width           =   1815
         End
         Begin VB.Label Label13 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "Build custom song [wxd] file from aifrx file(s) and attaches to main WXD"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00E0E0E0&
            Height          =   255
            Left            =   -74760
            TabIndex        =   34
            Top             =   555
            Width           =   6615
         End
         Begin VB.Shape Shape7 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Left            =   -74880
            Shape           =   4  'Rounded Rectangle
            Top             =   480
            Width           =   6855
         End
         Begin VB.Label Label9 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "Music Tracks file [wxh] editor"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00E0E0E0&
            Height          =   375
            Left            =   -74760
            TabIndex        =   33
            Top             =   555
            Width           =   6615
         End
         Begin VB.Label Label8 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "aifr -> aifrx conversion"
            BeginProperty Font 
               Name            =   "Tahoma"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00E0E0E0&
            Height          =   255
            Left            =   240
            TabIndex        =   22
            Top             =   555
            Width           =   6615
         End
         Begin VB.Shape Shape6 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Left            =   120
            Shape           =   4  'Rounded Rectangle
            Top             =   480
            Width           =   6855
         End
         Begin VB.Shape Shape5 
            BackColor       =   &H8000000C&
            BackStyle       =   1  'Opaque
            Height          =   375
            Left            =   -74880
            Shape           =   4  'Rounded Rectangle
            Top             =   480
            Width           =   6855
         End
      End
      Begin VB.Label Label7 
         BackStyle       =   0  'Transparent
         Caption         =   "Music Customization"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0E0E0&
         Height          =   255
         Left            =   240
         TabIndex        =   21
         Top             =   315
         Width           =   6855
      End
      Begin VB.Shape Shape4 
         BackColor       =   &H8000000C&
         BackStyle       =   1  'Opaque
         Height          =   375
         Left            =   120
         Shape           =   4  'Rounded Rectangle
         Top             =   240
         Width           =   7095
      End
   End
   Begin VB.Frame fraVoice 
      Height          =   6255
      Left            =   2280
      TabIndex        =   73
      Top             =   1800
      Width           =   7335
      Begin TabDlg.SSTab SSTab4 
         Height          =   5415
         Left            =   120
         TabIndex        =   74
         Top             =   720
         Width           =   7095
         _ExtentX        =   12515
         _ExtentY        =   9551
         _Version        =   393216
         TabHeight       =   520
         TabCaption(0)   =   "aifr -> aifrv"
         TabPicture(0)   =   "frmMain.frx":0712
         Tab(0).ControlEnabled=   -1  'True
         Tab(0).ControlCount=   0
         TabCaption(1)   =   "Phrase replacement"
         TabPicture(1)   =   "frmMain.frx":072E
         Tab(1).ControlEnabled=   0   'False
         Tab(1).ControlCount=   0
         TabCaption(2)   =   "Sentence Arrangement"
         TabPicture(2)   =   "frmMain.frx":074A
         Tab(2).ControlEnabled=   0   'False
         Tab(2).ControlCount=   0
      End
      Begin VB.Label Label25 
         BackStyle       =   0  'Transparent
         Caption         =   "Speech / Voice / Subtitling  Customization"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0E0E0&
         Height          =   255
         Left            =   240
         TabIndex        =   75
         Top             =   330
         Width           =   6855
      End
      Begin VB.Shape Shape9 
         BackColor       =   &H8000000C&
         BackStyle       =   1  'Opaque
         Height          =   375
         Left            =   120
         Shape           =   4  'Rounded Rectangle
         Top             =   240
         Width           =   7095
      End
   End
   Begin VB.Frame fraAdvanced 
      Height          =   6255
      Left            =   2880
      TabIndex        =   140
      Top             =   960
      Width           =   7335
      Begin VB.Label lblConvType 
         BackStyle       =   0  'Transparent
         Caption         =   "Advanced Audio Editing"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0E0E0&
         Height          =   255
         Index           =   1
         Left            =   240
         TabIndex        =   141
         Top             =   315
         Width           =   4095
      End
      Begin VB.Shape Shape1 
         BackColor       =   &H8000000C&
         BackStyle       =   1  'Opaque
         Height          =   375
         Index           =   1
         Left            =   120
         Shape           =   4  'Rounded Rectangle
         Top             =   240
         Width           =   7095
      End
   End
   Begin VB.Shape shpSectionOutline 
      BorderColor     =   &H80000013&
      Height          =   615
      Index           =   6
      Left            =   120
      Shape           =   4  'Rounded Rectangle
      Top             =   4680
      Width           =   1215
   End
   Begin VB.Shape shpSectionOutline 
      BorderColor     =   &H80000013&
      Height          =   615
      Index           =   5
      Left            =   120
      Shape           =   4  'Rounded Rectangle
      Top             =   5520
      Width           =   1215
   End
   Begin VB.Shape shpSectionOutline 
      BorderColor     =   &H80000013&
      Height          =   615
      Index           =   4
      Left            =   120
      Shape           =   4  'Rounded Rectangle
      Top             =   3840
      Width           =   1215
   End
   Begin VB.Shape shpSectionOutline 
      BorderColor     =   &H80000013&
      Height          =   615
      Index           =   3
      Left            =   120
      Shape           =   4  'Rounded Rectangle
      Top             =   3000
      Width           =   1215
   End
   Begin VB.Shape shpSectionOutline 
      BorderColor     =   &H80000013&
      Height          =   615
      Index           =   2
      Left            =   120
      Shape           =   4  'Rounded Rectangle
      Top             =   2160
      Width           =   1215
   End
   Begin VB.Shape shpSectionOutline 
      BorderColor     =   &H80000013&
      Height          =   615
      Index           =   1
      Left            =   120
      Shape           =   4  'Rounded Rectangle
      Top             =   1320
      Width           =   1215
   End
   Begin VB.Shape shpSectionOutline 
      BorderColor     =   &H80000013&
      Height          =   615
      Index           =   0
      Left            =   120
      Shape           =   4  'Rounded Rectangle
      Top             =   480
      Width           =   1215
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      Caption         =   "Functions"
      Height          =   285
      Left            =   30
      TabIndex        =   9
      Top             =   30
      Width           =   1395
   End
   Begin VB.Label Label1 
      BackColor       =   &H8000000C&
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   6255
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   1455
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim addToMusic() As String, addToMusicCount As Integer

Private Sub BnkEditSetLooping_Click()
    frmSpecial.Show
    frmSpecial.Visible = False
    frmSpecial.setType ("LOOPSET")
    frmSpecial.sldLoopStart.Min = 1
    frmSpecial.sldLoopStart.Max = FileLen(txtBNKeditorNewSoundPath.Text)
    frmSpecial.sldLoopEnd.Min = 1
    frmSpecial.sldLoopEnd.Max = frmSpecial.sldLoopStart.Max
    frmSpecial.Visible = True
End Sub

Private Sub BnkEditSetPanning_Click()
    frmSpecial.Show
    frmSpecial.Visible = False
    frmSpecial.setType ("BNKPAN")
    frmSpecial.Visible = True
End Sub

Private Sub BnkEditSetVol_Click()
    frmSpecial.Show
    frmSpecial.Visible = False
    frmSpecial.setType ("BNKVOL")
    frmSpecial.Visible = True
End Sub

Private Sub cmdAddAifr2Wxd_Click()
    Dim tmpPath As String
    
    tmpPath = getFileNameString(cd, "AIFRX files (*.aifrx)|*.aifrx", CD_OPEN)
    If (tmpPath <> "") Then
        addToMusicCount = addToMusicCount + 1
        ReDim Preserve addToMusic(addToMusicCount)
        addToMusic(addToMusicCount - 1) = tmpPath
        breakfilename tmpPath
        lstAifrx2Wxd.AddItem bfn.fnameNoExt
    End If
End Sub

Private Sub cmdaifr2aifrbconvert_Click()
    Dim i As Integer, tname As String
    
    If (fileAifr2AifrbList.ListCount = 0) Then
        MsgBox "Locate the AIFR's first!", vbInformation, "Error"
        Exit Sub
    End If
    
    LogBoxSet lstaifr2aifrbresults
    
    cmdaifr2aifrbconvert.Enabled = False
    
    'loop through all aifr files listed
    For i = 0 To fileAifr2AifrbList.ListCount - 1
        logMsg "- Processing "
        
        fileAifr2AifrbList.ListIndex = i
        
        breakfilename fileAifr2AifrbList.filename
        logMsg "Copying " & bfn.filename & " to working dir"
        
        tname = copyToAifr2AifrbWorkingDir(fileAifr2AifrbList.Path & "\" & fileAifr2AifrbList.filename)
        
        logMsg "Stripping out AIFR header"
        
        makeAifr4SoundFX (tname)
        
        breakfilename fileAifr2AifrbList.filename
        logMsg "Copying back to source dir"
        
        moveToAifrbDir tname, bfn.fnameNoExt & ".aifrb", dirAifr2AifrbSource.Path & "\"
    Next i
    
    logMsg "Finished converting AIFR to AIFRB !"
    
    cmdaifr2aifrbconvert.Enabled = True
End Sub

Private Sub cmdAifr2Aifrx_Click()
    Dim i As Integer, tname As String
    
    If (fileAifrfiles.ListCount = 0) Then
        MsgBox "Choose a directory with AIFR files in it first!", vbCritical, "Error"
        Exit Sub
    End If
    
    LogBoxSet lstaifr2aifrxresults
    
    cmdAifr2Aifrx.Enabled = False
    
    For i = 0 To fileAifrfiles.ListCount - 1
        logMsg "- Processing "
        fileAifrfiles.ListIndex = i
        
        breakfilename fileAifrfiles.filename
        logMsg "Copying " & bfn.filename & " to working dir"
        tname = copyToAifr2AifrxWorkingDir(fileAifrfiles.Path & "\" & fileAifrfiles.filename)
        
        logMsg "Stripping out AIFR header"
        makeAifr4Music (tname)
        
        breakfilename fileAifrfiles.filename
        logMsg "Copying back to source dir"
        moveToAifrxDir tname, bfn.fnameNoExt & ".aifrx", diraifr2aifrx.Path & "\"
    Next i
    
    cmdAifr2Aifrx.Enabled = True
End Sub

Private Sub cmdaifr2wxdPath2WXD_Click()
    Dim tmpPath As String
    tmpPath = getFileNameString(cd, "WXD File (*.wxd)|*.wxd", CD_OPEN)
    If (tmpPath <> "") Then
        txtaifr2wxdPath2WXD.Text = tmpPath
    End If
End Sub

Private Sub cmdAudioEditor_Click()
    fraConversion.Visible = False
    fraVoice.Visible = False
    fraSound.Visible = False
    fraDist.Visible = False
    fraMusic.Visible = False
    fraSettings.Visible = False
    fraAdvanced.Visible = True
    setoutline (6)
End Sub

Private Sub cmdBnkBuild_Click()
    
    If (txtNewBnkFilePath.Text = "") Then
        MsgBox "Make sure you set the path for the new BNK file first", vbInformation, "Error"
        Exit Sub
    End If
    
    BuildNewBNKFile
    'BuildMatchingLUTFile        ---- which one of these is broken?
End Sub

Private Sub cmdBnkEditCancel_Click()
    Dim which As Integer
    which = lstbnkeditorcontents.ListIndex
    txtBNKEditorNewBitrate.Text = Str(OrigBnkContents(which).theBitrate)
    If (OrigBnkContents(which).frequency = 22050) Then
        optBNKEditor22khz.Value = True
    Else
        optBNKEditor44khz.Value = True
    End If
    If (OrigBnkContents(which).numChans = 2) Then
        optBNKEditorStereo.Value = True
    Else
        optBNKEditorMono.Value = True
    End If
    Me.txtBNKeditorNewSoundPath.Text = ""
    lstbnkeditorcontents.Selected(which) = False
    theBnkContents(which).bReplace = False
End Sub

Private Sub cmdBNKEditorBNKPath_Click()
    Dim tmpPath As String
    tmpPath = getFileNameString(cd, "BNK Files (*.bnk)|*.bnk", CD_OPEN)
    If (tmpPath <> "") Then
        txtbnkeditorbnkpath.Text = tmpPath
        examineBNKfile
    End If
End Sub

Private Sub cmdBNKEditorNewSoundPath_Click()
    Dim tmpPath As String
    tmpPath = getFileNameString(cd, "AIFRb files (*.aifrb)|*.aifrb", CD_OPEN)
    If (tmpPath <> "") Then
        txtBNKeditorNewSoundPath.Text = tmpPath
        getSoundAttributesForBnkEdit
    End If
End Sub

Private Sub cmdBnkEditorReset_Click()
    
    If (MsgBox("Are you sure you want to reset the BNK editor and discard all changes?", vbQuestion + vbYesNo, "Okay to RESET?") = vbYes) Then
            
            txtBNKEditorNewBitrate.Text = ""
            txtBNKeditorNewSoundPath.Text = ""
            txtbnkeditorbnkpath.Text = ""
            
            lblBnkEditLooping.Caption = ""
            lblBnkEditVol.Caption = ""
            lblBnkEditPanning.Caption = "'"
            lblBNKEditorNewSoundOffset.Caption = ""
            lblBNKEditorNewSoundLength.Caption = ""
            
            optBNKEditor22khz.Value = True
            optBNKEditorMono.Value = True
            
            ReDim OrigBnkContents(1)
            ReDim theBnkContents(1)
            ReDim tOffsets(1)
            ReDim addedBnkEntriesLength(1)
            
            lstbnkeditorcontents.Clear
            totalBNKEntries = 0
            bnkFileSize = 0
            origFileSize = 0
            addedBnkEntries = 0
            
            lstBNKEditorResults.Clear
    End If
End Sub

Private Sub cmdBnkEditorSaveChanges_Click()
    
    If (MsgBox("Are you sure you want to save changes to this BNK file?", _
                vbQuestion + vbYesNo, _
                "Okay to Change?") = vbNo) Then
        Exit Sub
    End If
    
    addNewEntriesToExistingBNK
    alterExistingBNKHeaders
End Sub

Private Sub cmdBnkEditorSet_Click()
    Dim which As Integer, frq As Long
    
    'the user better have these values right!
    
    which = Me.lstbnkeditorcontents.ListIndex
    If (optBNKEditor22khz.Value = True) Then
        frq = 22050
    Else
        frq = 44100
    End If
    
    theBnkContents(which).bReplace = True
    theBnkContents(which).thePath = frmMain.txtBNKeditorNewSoundPath.Text
    theBnkContents(which).theSize = Val(frmMain.lblBNKEditorNewSoundLength.Caption)
    
    'save the changes to the changeable array only!
    theBnkContents(which).theBitrate = Val(txtBNKEditorNewBitrate.Text)
    theBnkContents(which).frequency = frq
    theBnkContents(which).numbits = Val(lblBnkEditorNumBits.Caption)
    If (optBNKEditorMono.Value = True) Then
        theBnkContents(which).numChans = 1
    Else
        theBnkContents(which).numChans = 2
    End If
    
    'set the add point
    theBnkContents(which).offset = bnkFileSize + 4                      ' compensate for "DATA" string
    
    'adjust the file size
    bnkFileSize = bnkFileSize + addedBnkEntriesLength(which)            ' new end of BNK file
    
    'just make sure
'    theBnkContents(which).bReplace = True
End Sub

Private Sub cmdBnkLutEditBnkPathBrowse_Click()
    cd.filename = ""
    cd.filter = "BNK files (*.bnk)|*.bnk"
    cd.ShowOpen
    If (cd.filename <> "") Then
        lblBnkLutEditBnkFilePath.Caption = cd.filename
    End If
End Sub

Private Sub cmdBnkLutEditProcess_Click()
    If (lblBnkBuildEventfile.Caption = "") Or (lblBnkLutEditBnkFilePath.Caption = "") Then
        MsgBox "Choose a LUT and BNK file first!", vbInformation, "Error"
        Exit Sub
    End If
    SoundEventsParseLutFile (lblBnkBuildEventfile.Caption)
End Sub

Private Sub cmdBrowseNewBnkPath_Click()
    Dim tmpPath As String
    tmpPath = getFileNameString(cd, "BNK Files (*.bnk)|*.bnk", CD_SAVE)
    If (tmpPath <> "") Then
        txtNewBnkFilePath.Text = tmpPath
    ElseIf (Dir(tmpPath, vbNormal) <> "") Then
        MsgBox "Make sure you make a NEW BNK, not an existing one!", vbCritical, "Error"
        Exit Sub
    End If

End Sub

Private Sub cmdBuildWXD_Click()
    Dim i As Integer
    
    If (lstAifrx2Wxd.ListCount = 0 Or txtaifr2wxdPath2WXD.Text = "") Then
        MsgBox "Add items to the list box and set path before hitting this button!", vbInformation, "Error"
        Exit Sub
    End If
    
    LogBoxSet lstaifrx2wxdresults
    
    cmdBuildWXD.Enabled = False
    
    'make an empty file to start with
    logMsg "Creating new file"
    If (Dir(workDir(workdir_wxd) & "custom.wxd") <> "") Then
        logMsg "Deleting old, temp custom.wxd"
        Kill (workDir(workdir_wxd) & "custom.wxd")
    End If
    If (createEmptyBinaryFile(workDir(workdir_wxd) & "custom.wxd") = False) Then
        logMsg "*** empty binary creation FAILED!!! Halting"
        Exit Sub
    End If
        
    'loop through all entries and append them to make a new custom.wxd file
    For i = 0 To addToMusicCount - 1
        If (addToMusic(i) <> "") Then
            breakfilename addToMusic(i)
            logMsg "-- Adding " & bfn.fnameNoExt
            If (appendBinaryFiles(workDir(workdir_wxd) & "custom.wxd", addToMusic(i), frmMain.progCustomWXD) = False) Then
                logMsg "*** append file action FAILED!!! Halting"
                Exit Sub
            End If
        End If
    Next i
    
    'tell user that the distribution part is ready
    logMsg "Distribution file custom.wxd created!"
    
    'make a backup first
    breakfilename txtaifr2wxdPath2WXD.Text
    If (Dir(bfn.fullpath & bfn.fnameNoExt & ".bak", vbNormal) <> "") Then
        logMsg "deleting old backup file!"
        Kill bfn.fullpath & bfn.fnameNoExt & ".bak"
    End If
    logMsg "making backup of game music wxd file -- PLEASE WAIT..."
    Name txtaifr2wxdPath2WXD.Text As bfn.fullpath & bfn.fnameNoExt & ".bak"
    FileCopy bfn.fullpath & bfn.fnameNoExt & ".bak", txtaifr2wxdPath2WXD.Text
    
    'now, add this custom.wxd to the main music file
    logMsg "Adding custom.wxd to main music wxd"
    If (appendBinaryFiles(txtaifr2wxdPath2WXD.Text, workDir(workdir_wxd) & "custom.wxd", frmMain.progCustomWXD) = False) Then
        logMsg "*** error appending custom.wxd to HW_Music.wxd!!! Halting"
        Exit Sub
    End If
    
    'move this distribution file to the directory where the hw_music.wxd file is found
    logMsg "Moving custom.wxd to game dir"
    breakfilename txtaifr2wxdPath2WXD.Text
    moveToWXDDir workDir(workdir_wxd) & "custom.wxd", bfn.fullpath
    
    'clean up
    logMsg "Clearing aifrx array"
    addToMusicCount = 0
    ReDim addToMusic(1)
    lstAifrx2Wxd.Clear
    
    cmdBuildWXD.Enabled = True
End Sub

Private Sub cmdClear_Click()
    If (MsgBox("Are you sure you want to clear the list?", vbYesNo + vbQuestion, "Okay to clear?") = vbYes) Then
        lstAifrx2Wxd.Clear
        ReDim addToMusic(1)
        addToMusic(0) = ""
        addToMusic(1) = ""
        addToMusicCount = 0
    End If
End Sub

Private Sub cmdDistribute_Click()
    fraConversion.Visible = False
    fraVoice.Visible = False
    fraSound.Visible = False
    fraDist.Visible = True
    fraMusic.Visible = False
    fraSettings.Visible = False
    fraAdvanced.Visible = False
    setoutline (4)
End Sub

Private Sub cmdExtractFilePathBrowse_Click()
    Dim tmpPath As String, typeStr As String, numSounds As Integer
    
    If (optExtractBNK.Value = True) Then
        typeStr = "BNK Files (*.bnk)|*.bnk"
        ExtractTypeNum = Extract_BNK
    ElseIf (optExtractWXD.Value = True) Then
        typeStr = "WXH Files (*.wxh)|*.wxh"
        ExtractTypeNum = Extract_WXD
    Else
        typeStr = "VCE Files (*.vce)|*.vce"
        ExtractTypeNum = Extract_VCE
    End If
    
    tmpPath = getFileNameString(cd, typeStr, CD_OPEN)
    If (tmpPath <> "") Then
        txtExtractFilePath.Text = tmpPath
        
        Select Case ExtractTypeNum
            Case Extract_BNK
                numSounds = getNumSoundsFromBNK(tmpPath)
            Case Extract_WXD
                numSounds = getNumSongsFromWXD(tmpPath)
            Case Extract_VCE
'                numSounds = getNumPhrasesFromVCE(tmpPath)
        End Select
        
        lblExtractNumSounds.Caption = numSounds
    End If
End Sub

Private Sub cmdExtractSounds_Click()
    Select Case ExtractTypeNum
        Case Extract_BNK
            BNKExtractSounds (txtExtractFilePath.Text)
        Case Extract_WXD
'            WXDExtractSongs (txtExtractFilePath.Text)
        Case Extract_VCE
'            VCEExtractPhrases (txtExtractFilePath.Text)
    End Select
End Sub

Private Sub cmdmp32wavconvert_Click()
    Dim i As Integer, tStr As String
    
    If (filemp32wavlist.ListCount = 0) Then
        MsgBox "No mp3 files found in this directory!", vbCritical, "Error"
        Exit Sub
    End If
    
    LogBoxSet lstmp32wavresults
    
    cmdmp32wavconvert.Enabled = False
    
    For i = 0 To filemp32wavlist.ListCount - 1
        filemp32wavlist.ListIndex = i
        breakfilename (filemp32wavlist.Path & "\" & filemp32wavlist.filename)
        
        logMsg "-Copying: " & bfn.filename
        tStr = copyToMP3WavWorkingDir(filemp32wavlist.Path & "\" & filemp32wavlist.filename)
        
        DoEvents
        
        breakfilename (filemp32wavlist.Path & "\" & filemp32wavlist.filename)
        logMsg "Converting: " & bfn.filename
        convertMP32Wav (tStr)
        
        DoEvents
        
        breakfilename (filemp32wavlist.Path & "\" & filemp32wavlist.filename)
        logMsg "Moving: " & bfn.filename
        moveToWavDir tStr, bfn.fnameNoExt & ".wav", bfn.fullpath
        
        DoEvents
    
    Next i
    
    cmdmp32wavconvert.Enabled = True
End Sub

Private Sub cmdMusic_Click()
    fraConversion.Visible = False
    fraVoice.Visible = False
    fraSound.Visible = False
    fraDist.Visible = False
    fraMusic.Visible = True
    fraSettings.Visible = False
    fraAdvanced.Visible = False
    setoutline (1)
End Sub

Private Sub cmdConvert_Click()
    fraMusic.Visible = False
    fraSound.Visible = False
    fraVoice.Visible = False
    fraDist.Visible = False
    fraConversion.Visible = True
    fraSettings.Visible = False
    fraAdvanced.Visible = False
    setoutline (0)
End Sub

Private Sub cmdNewMusicGameDir_Click()
    cd.filename = ""
    cd.filter = "Homeworld/Cataclysm EXE (*.exe)|*.exe"
    
    cd.ShowOpen
    If (cd.filename <> "") Then
        breakfilename cd.filename
        txtNewMusicGameDir.Text = bfn.fullpath
    End If
End Sub

Private Sub cmdNewSongTracksBuildFiles_Click()
    If (txtNewMusicGameDir.Text = "" Or fileNewSongTrack.ListCount = 0) Then
        MsgBox "Set the Game directory and find the song files first!", vbCritical, "Error"
        Exit Sub
    End If
    
    
    cmdNewSongTracksBuildFiles.Enabled = False
    
    buildNewMusicFiles
    
    cmdNewSongTracksBuildFiles.Enabled = True
    
End Sub

Private Sub cmdPathBnkBuildEventfile_Click()
    cd.filename = ""
    cd.filter = "LUT Files (*.lut)|*.lut"
    cd.DialogTitle = "Gunevents.lut,UI.lut,derelictevents.lut,specexpevents.lut, etc"
    cd.ShowOpen
    If (cd.filename <> "") Then
        lblBnkBuildEventfile.Caption = cd.filename
    End If
End Sub

Private Sub cmdRemoveAifr2Wxd_Click()
    If (lstAifrx2Wxd.ListIndex = -1) Then
        MsgBox "Select an item in the list box before hitting this button!", vbInformation, "Error"
        Exit Sub
    End If
    
    If (MsgBox("Are you sure you want to remove this item?", vbYesNo + vbQuestion, "Okay to remove?") = vbYes) Then
        addToMusic(lstAifrx2Wxd.ListIndex) = ""
        lstAifrx2Wxd.RemoveItem (lstAifrx2Wxd.ListIndex)
        ' don't worry about changing the array count, the fact that it is an empty string
        ' will cause it to be skipped.  We'll reset the count after they build the wxd
    End If
End Sub

Private Sub cmdSettings_Click()
    fraMusic.Visible = False
    fraVoice.Visible = False
    fraConversion.Visible = False
    fraDist.Visible = False
    fraSound.Visible = False
    fraSettings.Visible = True
    fraAdvanced.Visible = False
    setoutline (5)
End Sub

Private Sub cmdSettingsGameDir_Click()
    cd.filename = ""
    cd.filter = "EXE file(*.exe)|*.exe"
    cd.ShowOpen '("Find the Homeworld/Cataclysm EXE file!")
    If (cd.filename <> "") Then
        txtSettingsGameDir.Text = cd.filename
    End If
End Sub

Private Sub cmdSound_Click()
    fraMusic.Visible = False
    fraVoice.Visible = False
    fraConversion.Visible = False
    fraDist.Visible = False
    fraSound.Visible = True
    fraSettings.Visible = False
    fraAdvanced.Visible = False
    setoutline (2)
End Sub

Private Sub cmdVoice_Click()
    fraMusic.Visible = False
    fraVoice.Visible = True
    fraConversion.Visible = False
    fraDist.Visible = False
    fraSound.Visible = False
    fraSettings.Visible = False
    fraAdvanced.Visible = False
    setoutline (3)
End Sub

Private Sub cmdwav2aifrEncode_Click()
    Dim i As Integer, encval As Integer, cmdLine As String, tfile As String, origfilename As String
    
    If (fileWav.ListCount = 0) Or (cmboAifrEncodingBitrate.ListIndex = -1) Then
        MsgBox "You must choose and encoding bitrate and the directory with at " & _
                "least one wav file first!", vbInformation, "Error"
        Exit Sub
    End If
    
    LogBoxSet lstwav2aifrResults
    cmdwav2aifrEncode.Enabled = False
    
    encval = cmboAifrEncodingBitrate.Text
    
    For i = 0 To fileWav.ListCount - 1
        fileWav.ListIndex = i
        origfilename = dirWavSource.Path & "\" & fileWav.filename
        
        logMsg "- Copying to work dir"
        tfile = copyToWav2AifrWorkingDir(origfilename)
                
        DoEvents
        
        logMsg "* AIFR encoding, bitrate=" & encval
        convertWav2AIFR tfile, encval
        
        DoEvents
        
        breakfilename (dirWavSource.Path & "\" & fileWav.filename)
        logMsg "- Moving back to orig dir"
        moveToAifrDir tfile, bfn.fnameNoExt & ".aifr", bfn.fullpath
        
        'make the log file from the wav header
        If (makeAifrLogFromWav(origfilename, encval) = False) Then
            logMsg "*** error making log from wav!!! Halting"
            Exit Sub
        End If
    Next i
    
    cmdwav2aifrEncode.Enabled = True
   
End Sub

Private Sub cmdWXHEditAttach_Click()
    LogBoxSet lstwxheditresults
    
    'attach selected song (in music library[wxd]) to selected track (in header[wxh])
    lstWXH.List(lstWXH.ListIndex) = Str(lstWXH.ListIndex + 1) & "   [" & Trim(Str(lstWXD.ListIndex + 1)) & "]*"
    attachedSong(Val(lstWXH.ListIndex)) = Val(lstWXD.ListIndex)
    
    'adjust the matching values
    theMusicTracks(Val(lstWXH.ListIndex)).bitRate = Val(txtwxheditbitrate.Text)
    
    If (chkwxheditskirmish.Value = 1) Then
        theMusicTracks(Val(lstWXH.ListIndex)).skirmishflag = 1
    Else
        theMusicTracks(Val(lstWXH.ListIndex)).skirmishflag = 0
    End If
    
    theMusicTracks(Val(lstWXH.ListIndex)).signature = theMusicTracks(0).signature
    theMusicTracks(Val(lstWXH.ListIndex)).SongOffset = theSongsExactLocation(attachedSong(Val(lstWXH.ListIndex)))
    
    'unhighlight both and disable the attach button
    cmdWXHEditAttach.Enabled = False
    'lstWXH.ListIndex = -1
    'lstWXD.ListIndex = -1
    lblwxdeditbitratenote.Visible = False
    
    logMsg "Attaching song to track in wxh"

    'enable the save button
    cmdwxheditsave.Enabled = True
    
    'disable the attach button
    cmdWXHEditAttach.Enabled = False
End Sub

Private Sub cmdWxhEditHeaderQuickReference_Click()
    Dim tmpstr As String
    
    'just a quick reference if this is the homeworld wxd
    tmpstr = "Track listings for homeworld's standard wxh file: " & vbCrLf & vbCrLf & _
        "track 1    : title screen, missions 2 & 4" & vbCrLf & _
        "track 2-9  : multiplayer game music " & vbCrLf & _
        "track 10   : multiplayer battle music " & vbCrLf & _
        "track 11   : multiplayer game music " & vbCrLf & _
        "track 12   : ship launch (adiago)" & vbCrLf & _
        "track 13-16: multiplayer battle music " & vbCrLf & _
        "track 17   : cinematics/briefing" & vbCrLf & _
        "track 18-19: adiago (short) " & vbCrLf & _
        "track 20-25: cinematics/briefing" & vbCrLf & _
        "track 26   : multiplayer in game music" & vbCrLf & _
        "track 27   : cinematics/briefing" & vbCrLf & _
        "track 28   : multiplayer in game music" & vbCrLf & _
        "track 29   : end titles (YES song)" & vbCrLf & _
        "track 30   : Sierra intro" & vbCrLf & _
        "track 31   : Relic intro" & vbCrLf & _
        "track 32-47: cinematics/briefing" & vbCrLf & _
        "track 48   : End sequence (Adiago at Hiigara)"
        
    MsgBox tmpstr, vbInformation, "WXH track listings"
    

End Sub

Private Sub cmdWXHEditorWXDPath_Click()
    Dim tmpPath As String
    tmpPath = getFileNameString(cd, "WXD file (*.wxd)|*.wxd", CD_OPEN)
    If (tmpPath <> "") Then
        txtWXHEditorWXDPath.Text = tmpPath
    End If
    
End Sub

Private Sub cmdWXHEditorWXHPath_Click()
    Dim tmpPath As String
    tmpPath = getFileNameString(cd, "WXH file (*.wxh)|*.wxh", CD_OPEN)
    If (tmpPath <> "") Then
        txtWXHEditorWXHPath.Text = tmpPath
    End If
End Sub

Private Sub cmdwxheditprocess_Click()
    Dim whfn As Integer, wdfn As Integer, i As Long, scount As Integer, dataLong As Long
    Dim curpos As Long, tmp As Long, theSongNumber As Integer
    
    'did they specify the files?
    If (txtWXHEditorWXHPath.Text = "" Or txtWXHEditorWXDPath.Text = "") Then
        MsgBox "Get the locations of both the wxh and wxd files first!!!!", vbCritical, "Error"
        Exit Sub
    End If
    
    'clear both panes
    lstWXH.Clear
    lstWXD.Clear
    
    'open both files
    On Error GoTo loadwxerr
    whfn = FreeFile
    Open txtWXHEditorWXHPath.Text For Binary Access Read As #whfn
    wdfn = FreeFile
    Open txtWXHEditorWXDPath.Text For Binary Access Read As #wdfn
    On Error GoTo 0
    
    'get number of tracks from WXH file
    Get #whfn, , theWXHheader
    numTracks = theWXHheader.numTracks
    ReDim theMusicTracks(numTracks)
    
    '--- change: in case we build a custom wxd that has less songs that tracks (the wxh must have 48 entries, but the wxd may only have a few songs...initialize them to point to the first one.
    For i = 0 To numTracks - 1
        theMusicTracks(i).SongOffset = 8   'this would be the offset from start of the first song
    Next i
    '--- end of change
    
    'zip through the songs file and count the number of songs
    scount = 0
    curpos = 0
    Seek #wdfn, 9           'jump past the header
    
    'get the address locations of the songs from the wxd, incase we've been adding songs to the wxd after the wxh was made
    Do
        'grab 4 bytes at a time
        Get #wdfn, , dataLong
'        curpos = curpos + 1
        curpos = Seek(wdfn) - 1             ' adjust for non-vb offsetting
        
        'does it = "DATA" string (backwards because of crappy intel format)
        If (Hex(dataLong) = "41544144") Then
            scount = scount + 1
            numSongs = scount
            
            ReDim Preserve theSongsExactLocation(scount - 1)
'            theSongsExactLocation(scount - 1) = (curpos - 1) * 4
            theSongsExactLocation(scount - 1) = curpos - 4      ' back up to include the DATA string
            
'            ReDim Preserve songFileOffsets(scount - 1)     ... why was I doing this twice?
'            songFileOffsets(scount - 1) = (curpos - 1) * 4
'            lstWXD.AddItem Str(scount) & "   [0x" & Hex(songFileOffsets(scount - 1)) & "]"
            lstWXD.AddItem Str(scount) & "   [0x" & Hex(theSongsExactLocation(scount - 1)) & "]"
            ReDim Preserve theMusicSongs(scount)

            'grab the length and offset into the file
            Get #wdfn, , theMusicSongs(scount - 1).songLen
            tmp = curpos + theMusicSongs(scount - 1).songLen + 1 + 4 'add for for the DATA string offset
'            tmp = tmp + 8 + 4               ' add in the file header and the DATA string sizes
'            curpos = tmp / 4
            
            Seek #wdfn, tmp
        End If
        
        DoEvents
    Loop Until EOF(wdfn)
    
    ReDim attachedSong(0 To numTracks - 1)
    
    'get the info from the header file
    For i = 0 To numTracks - 1
        Get #whfn, , theMusicTracks(i)
        theSongNumber = findMatchingSong(theMusicTracks(i).SongOffset)
        lstWXH.AddItem Str(i + 1) & "   [" & Trim(Str(theSongNumber)) & "]"
        attachedSong(i) = theSongNumber
    Next i
    
    Close #whfn
    Close #wdfn
    
    Exit Sub
    
loadwxerr:
    LogBoxSet lstwxheditresults
    logMsg "*** error opening one of the files!! Halting"
End Sub

Private Sub cmdwxheditsave_Click()
    Dim nwhfn As Integer, nwhfname As String, i As Integer
    Dim tmpWxhHeader As WXH_header, tmpWxhTrack As WXH_track, tmp As Integer, tmpstr As String
    
    'open a file in our working directory
    nwhfname = workDir(workdir_wxd) & "hw_music.wxh"
    If (Dir(nwhfname, vbNormal) <> "") Then
        Kill (nwhfname)
    End If
    nwhfn = FreeFile
    On Error GoTo tempwxhfileerr
    Open nwhfname For Binary Access Read Write As #nwhfn
    On Error GoTo 0
    
    'write the header
    tmpWxhHeader.hString = "WXH1"
    tmpWxhHeader.hchecksum = theWXHheader.hchecksum
    tmpWxhHeader.numTracks = lstWXH.ListCount
    Put #nwhfn, , tmpWxhHeader
    
    LogBoxSet lstwxheditresults
    logMsg "Building temp wxh file for editing"
    
   'for each one in wxh
    For i = 0 To lstWXH.ListCount - 1
        logMsg "adding entry for track # " & Str(i + 1)
        tmpWxhTrack.bitRate = theMusicTracks(i).bitRate
        tmpWxhTrack.skirmishflag = theMusicTracks(i).skirmishflag
        tmpWxhTrack.fill = 0
        tmpWxhTrack.signature = theMusicTracks(i).signature
        tmpWxhTrack.SongOffset = theMusicTracks(i).SongOffset
        
        'write it to the temp file
        Put #nwhfn, , tmpWxhTrack
    Next i
    
    Close #nwhfn

    'make a backup of the original and copy the new one into that dir
    logMsg "backing up original and copying new one"
    breakfilename txtWXHEditorWXHPath.Text
    If (Dir(bfn.fullpath & bfn.fnameNoExt & ".bak", vbNormal) <> "") Then
        Kill (bfn.fullpath & bfn.fnameNoExt & ".bak")
    End If
    Name txtWXHEditorWXHPath.Text As bfn.fullpath & bfn.fnameNoExt & ".bak"
    FileCopy nwhfname, txtWXHEditorWXHPath.Text
    logMsg "deleting temp wxh file"
    Kill nwhfname
    
    'clean out the asterisks
    For i = 0 To frmMain.lstWXH.ListCount - 1
        If (InStr(1, frmMain.lstWXH.List(i), "*") <> 0) Then
            frmMain.lstWXH.List(i) = Left(frmMain.lstWXH.List(i), Len(frmMain.lstWXH.List(i)) - 1)
        End If
    Next i
    
    'disable the save button
    cmdwxheditsave.Enabled = False
    Exit Sub
tempwxhfileerr:
    logMsg "*** error while writing temp WXH file!!! Halting"
End Sub

Private Sub dirAifr2AifrbSource_Change()
    fileAifr2AifrbList.Path = dirAifr2AifrbSource.Path
End Sub

Private Sub diraifr2aifrx_Change()
    fileAifrfiles.Path = diraifr2aifrx.Path
End Sub

Private Sub dirBnkBuildSource_Change()
    fileBnkBuildSource.Path = dirBnkBuildSource.Path
End Sub

Private Sub dirmp32wav_Change()
    filemp32wavlist.Path = dirmp32wav.Path
End Sub

Private Sub dirNewSongTrackDir_Change()
    fileNewSongTrack.Path = dirNewSongTrackDir.Path
End Sub

Private Sub dirWavSource_Change()
    fileWav.Path = dirWavSource.Path
End Sub

Private Sub drvAifr2AifrbSource_Change()
    dirAifr2AifrbSource.Path = drvAifr2AifrbSource.Drive
End Sub

Private Sub drvaifr2aifrx_Change()
    diraifr2aifrx.Path = drvaifr2aifrx.Drive
End Sub

Private Sub drvBnkBuildSource_Change()
    dirBnkBuildSource.Path = drvBnkBuildSource.Drive
End Sub

Private Sub drvmp32wav_Change()
    dirmp32wav.Path = drvmp32wav.Drive
End Sub

Private Sub drvNewSongTrackDir_Change()
    dirNewSongTrackDir.Path = drvNewSongTrackDir.Drive
End Sub

Private Sub drvWavSource_Change()
    dirWavSource.Path = drvWavSource.Drive
End Sub

Private Sub Form_Load()
    Me.Caption = "HWSoundStudio v" & Trim(Str(App.Major)) & "." & Trim(Str(App.Minor)) & Trim(Str(App.Revision))
    getScreenDims
    
    Me.Height = 452 * twipy '6270
    Me.Width = 602 * twipx '8940
    
    fraMusic.Left = 104 * twipx
    fraMusic.Top = 0
    fraMusic.Visible = False
    fraSound.Left = 104 * twipx
    fraSound.Top = 0
    fraSound.Visible = False
    fraVoice.Left = 104 * twipx
    fraVoice.Top = 0
    fraVoice.Visible = False
    fraDist.Left = 104 * twipx
    fraDist.Top = 0
    fraDist.Visible = False
    fraSettings.Left = 104 * twipx
    fraSettings.Top = 0
    fraSettings.Visible = False
    fraAdvanced.Visible = False
    fraAdvanced.Top = 0
    fraAdvanced.Left = 104 * twipx

    mainSetup
    
    setoutline (0)
End Sub

Private Sub lstbnkeditorcontents_Click()
    fillInCurBnkSndInfo lstbnkeditorcontents.ListIndex
    
    If (lstbnkeditorcontents.Selected(lstbnkeditorcontents.ListIndex) = True) Then
        cmdBNKEditorNewSoundPath.Enabled = True
        txtBNKEditorNewBitrate.Enabled = True
        fraBnkEditFrequency.Enabled = True
        fraBnkEditNumChannels.Enabled = True
        BnkEditSetVol.Enabled = True
        BnkEditSetPanning.Enabled = True
        BnkEditSetLooping.Enabled = True
        chkBnkEditLoopOnPause.Enabled = True
        enableSetCancelBnkEdit
        theBnkContents(lstbnkeditorcontents.ListIndex).bReplace = True
    Else
        cmdBNKEditorNewSoundPath.Enabled = False
        txtBNKEditorNewBitrate.Enabled = False
        fraBnkEditFrequency.Enabled = False
        fraBnkEditNumChannels.Enabled = False
        cmdBnkEditorSet.Enabled = False
        BnkEditSetVol.Enabled = False
        BnkEditSetPanning.Enabled = False
        BnkEditSetLooping.Enabled = False
        cmdBnkEditCancel.Enabled = False
        chkBnkEditLoopOnPause.Enabled = False
        theBnkContents(lstbnkeditorcontents.ListIndex).bReplace = False
    End If
End Sub

Private Sub lstWXD_Click()
    If (lstWXH.ListIndex <> -1) Then
        cmdWXHEditAttach.Enabled = True
        lblwxdeditbitratenote.Visible = True
    End If
End Sub

Private Sub lstWXH_Click()
    'if we've highlighted one item in each box, then enable the attach button and show the bitrate note
    If (lstWXD.ListIndex <> -1) Then
        cmdWXHEditAttach.Enabled = True
        lblwxdeditbitratenote.Visible = True
    End If
    
    ' set the skirmish flag if we need to
    If (theMusicTracks(Val(lstWXH.ListIndex)).skirmishflag = 1) Then
        chkwxheditskirmish.Value = 1
    Else
        chkwxheditskirmish.Value = 0
    End If
    txtwxheditbitrate.Text = Trim(Str(CInt(theMusicTracks(Val(lstWXH.ListIndex)).bitRate)))

End Sub

Private Sub optBNKEditor22khz_Click()
    enableSetCancelBnkEdit
End Sub

Private Sub optBNKEditor44khz_Click()
    enableSetCancelBnkEdit
End Sub

Private Sub optBNKEditorMono_Click()
    enableSetCancelBnkEdit
End Sub

Private Sub optBNKEditorStereo_Click()
    enableSetCancelBnkEdit
End Sub

Private Sub SSTab1_Click(PreviousTab As Integer)
    If SSTab1.Tab = 1 Then
        'tell them about the limitations currently found
        Load frmNotice
        frmNotice.Visible = False
        frmNotice.lblNote.Caption = "All WAVs must be at least 1 second long or the " & _
                "encoder won't work right!  More limitations apply to certain types " & _
                "of encoded sounds."
        frmNotice.tmrflash.Enabled = True
        frmNotice.Visible = True
    End If
End Sub

Private Sub SSTab2_Click(PreviousTab As Integer)
    If (SSTab2.Tab = 3) Then
        If (checkForBlankAifrx() = False) Then
            MsgBox "Couldn't find blank.aifrx! Copy to the HWSS directory first!", vbCritical, "Error"
            SSTab2.Tab = PreviousTab
        End If
    End If
    
End Sub

Private Sub SSTab3_Click(PreviousTab As Integer)
    If SSTab3.Tab = 2 Then
        lblBnkBuildNotes.Caption = "Make sure that all sounds have the following attributes:" & _
                                    vbCrLf & vbCrLf & _
                                    "frequency=16 khz" & vbCrLf & "samplesize=16-bit" & vbCrLf & "num channels=1"
                                    
    End If
End Sub

Private Sub updnAifrx2wxd_DownClick()
    Dim entrynum As Integer, tmpstr As String
    
    If (lstAifrx2Wxd.ListIndex = -1) Then
        MsgBox "Select an item in the list box before hitting this button!", vbInformation, "Error"
        Exit Sub
    End If
    
    entrynum = lstAifrx2Wxd.ListIndex
    
    'make sure it isn't already at the bottom
    If (entrynum = lstAifrx2Wxd.ListCount - 1) Then
        Exit Sub
    End If
    
    'move it in the list first
    tmpstr = lstAifrx2Wxd.Text
    lstAifrx2Wxd.RemoveItem entrynum
    lstAifrx2Wxd.AddItem tmpstr, (entrynum + 1)
    
    'move it in the array next
    tmpstr = addToMusic(entrynum + 1)
    addToMusic(entrynum + 1) = addToMusic(entrynum)
    addToMusic(entrynum) = tmpstr
End Sub

Private Sub updnAifrx2wxd_UpClick()
    Dim entrynum As Integer, tmpstr As String
    
    If (lstAifrx2Wxd.ListIndex = -1) Then
        MsgBox "Select an item in the list box before hitting this button!", vbInformation, "Error"
        Exit Sub
    End If
    
    entrynum = lstAifrx2Wxd.ListIndex
    
    'make sure it isn't already at the top
    If (entrynum = 0) Then
        Exit Sub
    End If
    
    'move it in the list first
    tmpstr = lstAifrx2Wxd.Text
    lstAifrx2Wxd.RemoveItem entrynum
    lstAifrx2Wxd.AddItem tmpstr, (entrynum - 1)
    
    'move it in the array next
    tmpstr = addToMusic(entrynum - 1)
    addToMusic(entrynum - 1) = addToMusic(entrynum)
    addToMusic(entrynum) = tmpstr
End Sub


Private Function findMatchingSong(theOffset As Long) As String
    Dim i As Integer, theNum As Integer
    
    For i = 1 To numSongs
        If (theSongsExactLocation(i - 1) = theOffset) Then
            findMatchingSong = i
            Exit Function
        End If
    Next i
End Function

Private Function setoutline(which As Integer)
    Dim i As Integer
    
    For i = 0 To 6
        shpSectionOutline(i).Visible = False
    Next i
    
    shpSectionOutline(which).Visible = True
End Function

Private Sub examineBNKfile()
    Dim fn As Integer, i As Integer, aBnkEntry As bnkLUT, dbgstr As String, tbnkentry As bnkLut2
    Dim tfn As Integer, tStr As String
    
    'open the selected bnk file
    fn = FreeFile
    On Error GoTo bnk_open_err
    Open txtbnkeditorbnkpath.Text For Binary Access Read As #fn
    On Error GoTo 0
    
    LogBoxSet lstBNKEditorResults

    logMsg "Examining the BNK file"
    
    ' read the header
    Get #fn, , theBnkHeader         ' a global variable to hold the header
    
    ' how many patches (sound)
    totalBNKEntries = theBnkHeader.noPatches
    ReDim theBnkContents(0 To totalBNKEntries - 1)
    ReDim OrigBnkContents(0 To totalBNKEntries - 1)
    ReDim addedBnkEntriesLength(0 To totalBNKEntries - 1)
    dbgstr = ""
    
    'sound names
    tfn = openSoundNamesFile
    breakfilename (txtbnkeditorbnkpath.Text)
    
    ' fill in the array
    For i = 1 To totalBNKEntries
        theBnkContents(i - 1).bReplace = False      ' this array is the one that changes
        theBnkContents(i - 1).thePath = ""
        theBnkContents(i - 1).position = 0
        
        Get #fn, , tbnkentry
    
        theBnkContents(i - 1).offset = tbnkentry.offset
        theBnkContents(i - 1).theBitrate = tbnkentry.codecbitrate
        theBnkContents(i - 1).theSize = tbnkentry.soundSize
        theBnkContents(i - 1).frequency = tbnkentry.waveFormat.nSamplesPerSec
        theBnkContents(i - 1).numbits = tbnkentry.waveFormat.wBitsPerSample
        theBnkContents(i - 1).vol = tbnkentry.volume
        theBnkContents(i - 1).pan = tbnkentry.pan
        theBnkContents(i - 1).numChans = tbnkentry.waveFormat.nChannels
        theBnkContents(i - 1).loops = tbnkentry.loopstart
        theBnkContents(i - 1).loope = tbnkentry.loopend
        If (tbnkentry.flags = 1) Then
            theBnkContents(i - 1).loop = True
        Else
            theBnkContents(i - 1).loop = False
        End If
        
        OrigBnkContents(i - 1) = theBnkContents(i - 1)  ' copy to the unchanging array.
        
        'get the sound names (if we have them)
        If (tfn <> -1) Then
            tStr = getSoundName(tfn, bfn.fnameNoExt, i)     ' hold over from old version of code
            If (tStr = "-") Then
                lstbnkeditorcontents.AddItem "#" & Str(i)
            Else
                lstbnkeditorcontents.AddItem "#" & Str(i) & " " & tStr
            End If
        Else
            lstbnkeditorcontents.AddItem "entry #" & Str(i)
        End If
    Next i
    
    If (tfn <> -1) Then
        closeSoundNamesFile tfn
    End If
    
    Close #fn
    
    'finally, note the filesize, this will be the start offset of the first added sound
    bnkFileSize = FileLen(txtbnkeditorbnkpath.Text)
    origFileSize = bnkFileSize
    
    Exit Sub
bnk_open_err:
    MsgBox "An error occurred while trying to open the specified BNK file!", vbCritical, "Error"
End Sub

Public Sub enableSetCancelBnkEdit()
    cmdBnkEditorSet.Enabled = True
    cmdBnkEditCancel.Enabled = True
End Sub
