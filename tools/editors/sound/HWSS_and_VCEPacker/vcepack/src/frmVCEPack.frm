VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "comctl32.ocx"
Begin VB.Form frmVCEPack 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "VCE Packer - Speech Customizer for Homeworld and Cataclysm"
   ClientHeight    =   7140
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   7905
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7140
   ScaleWidth      =   7905
   StartUpPosition =   3  'Windows Default
   Begin MSComDlg.CommonDialog cd 
      Left            =   6120
      Top             =   6480
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Timer Timer1 
      Interval        =   1000
      Left            =   6120
      Top             =   6600
   End
   Begin VB.CommandButton cmdClose 
      Caption         =   "Close"
      Height          =   495
      Left            =   6600
      TabIndex        =   1
      Top             =   6600
      Width           =   1095
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   6375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7695
      _ExtentX        =   13573
      _ExtentY        =   11245
      _Version        =   393216
      Tabs            =   2
      TabsPerRow      =   2
      TabHeight       =   520
      TabCaption(0)   =   "wav->aifr"
      TabPicture(0)   =   "frmVCEPack.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Line1"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label1"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label3"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "dirWavDir"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "drvWavDir"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "lstencout"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "Frame1"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "dirAifrDir"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "drvAifrDir"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "cmdEncode"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "lstFiles"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "picProgress"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "ProgressBar1"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "cmdLengths"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).ControlCount=   14
      TabCaption(1)   =   "VCE Pack"
      TabPicture(1)   =   "frmVCEPack.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Frame2"
      Tab(1).Control(1)=   "chkStrictSubs"
      Tab(1).Control(2)=   "Slider1"
      Tab(1).Control(3)=   "ProgressBar2"
      Tab(1).Control(4)=   "cmdCancel"
      Tab(1).Control(5)=   "cmdBuild"
      Tab(1).Control(6)=   "picProgress2"
      Tab(1).Control(7)=   "lstPackOut"
      Tab(1).Control(8)=   "cmdBrowseVCEOut"
      Tab(1).Control(9)=   "cmdBrowseFileList"
      Tab(1).Control(10)=   "drvSubs"
      Tab(1).Control(11)=   "dirSubs"
      Tab(1).Control(12)=   "Label11"
      Tab(1).Control(13)=   "Label10"
      Tab(1).Control(14)=   "lblcnt"
      Tab(1).Control(15)=   "lblpos"
      Tab(1).Control(16)=   "lblBitrateUsed"
      Tab(1).Control(17)=   "Label9"
      Tab(1).Control(18)=   "lblNumSubtitles"
      Tab(1).Control(19)=   "Label8"
      Tab(1).Control(20)=   "lblNumPhrases"
      Tab(1).Control(21)=   "Label7"
      Tab(1).Control(22)=   "lblVCEOut"
      Tab(1).Control(23)=   "Label6"
      Tab(1).Control(24)=   "lblEncfileList"
      Tab(1).Control(25)=   "Label5"
      Tab(1).Control(26)=   "Label4"
      Tab(1).Control(27)=   "Line2"
      Tab(1).ControlCount=   28
      Begin VB.CommandButton cmdLengths 
         Caption         =   "Get Lengths"
         Height          =   495
         Left            =   5760
         TabIndex        =   51
         Top             =   3360
         Width           =   1215
      End
      Begin VB.Frame Frame2 
         Caption         =   "Game"
         Height          =   1215
         Left            =   -68880
         TabIndex        =   47
         Top             =   1680
         Width           =   1455
         Begin VB.OptionButton Option2 
            Caption         =   "Cataclysm"
            Enabled         =   0   'False
            Height          =   255
            Left            =   120
            TabIndex        =   49
            Top             =   600
            Width           =   1215
         End
         Begin VB.OptionButton Option1 
            Caption         =   "Homeworld"
            Height          =   255
            Left            =   120
            TabIndex        =   48
            Top             =   360
            Value           =   -1  'True
            Width           =   1215
         End
      End
      Begin VB.CheckBox chkStrictSubs 
         Alignment       =   1  'Right Justify
         Caption         =   "Strict Subtitles"
         Height          =   255
         Left            =   -72120
         TabIndex        =   46
         Top             =   2880
         Width           =   1575
      End
      Begin ComctlLib.Slider Slider1 
         Height          =   255
         Left            =   -70560
         TabIndex        =   45
         Top             =   2760
         Width           =   1695
         _ExtentX        =   2990
         _ExtentY        =   450
         _Version        =   327682
         LargeChange     =   1
         Max             =   5
      End
      Begin ComctlLib.ProgressBar ProgressBar2 
         Height          =   255
         Left            =   -72360
         TabIndex        =   44
         Top             =   4440
         Width           =   4695
         _ExtentX        =   8281
         _ExtentY        =   450
         _Version        =   327682
         Appearance      =   1
         Min             =   1
         Max             =   11292
      End
      Begin ComctlLib.ProgressBar ProgressBar1 
         Height          =   255
         Left            =   360
         TabIndex        =   43
         Top             =   4680
         Width           =   7095
         _ExtentX        =   12515
         _ExtentY        =   450
         _Version        =   327682
         Appearance      =   1
      End
      Begin VB.PictureBox picProgress 
         Height          =   255
         Left            =   240
         ScaleHeight     =   195
         ScaleWidth      =   7140
         TabIndex        =   38
         Top             =   4680
         Visible         =   0   'False
         Width           =   7200
      End
      Begin VB.FileListBox lstFiles 
         Height          =   3015
         Left            =   2520
         Pattern         =   "*.wav"
         TabIndex        =   37
         Top             =   1200
         Visible         =   0   'False
         Width           =   495
      End
      Begin VB.CommandButton cmdEncode 
         Caption         =   "Encode"
         Height          =   495
         Left            =   5760
         TabIndex        =   36
         Top             =   3960
         Width           =   1215
      End
      Begin VB.CommandButton cmdCancel 
         Caption         =   "Cancel"
         Height          =   495
         Left            =   -71040
         TabIndex        =   35
         Top             =   3240
         Width           =   1215
      End
      Begin VB.CommandButton cmdBuild 
         Caption         =   "Build VCE"
         Height          =   495
         Left            =   -69720
         TabIndex        =   34
         Top             =   3240
         Width           =   1215
      End
      Begin VB.PictureBox picProgress2 
         Height          =   255
         Left            =   -72360
         ScaleHeight     =   195
         ScaleWidth      =   4635
         TabIndex        =   27
         Top             =   4440
         Visible         =   0   'False
         Width           =   4695
      End
      Begin VB.ListBox lstPackOut 
         BackColor       =   &H8000000B&
         Height          =   1230
         Left            =   -74760
         TabIndex        =   26
         Top             =   4920
         Width           =   7215
      End
      Begin VB.CommandButton cmdBrowseVCEOut 
         Caption         =   "<"
         Height          =   255
         Left            =   -67920
         TabIndex        =   25
         Top             =   1320
         Width           =   255
      End
      Begin VB.CommandButton cmdBrowseFileList 
         Caption         =   "<"
         Height          =   255
         Left            =   -67920
         TabIndex        =   24
         Top             =   960
         Width           =   255
      End
      Begin VB.DriveListBox drvSubs 
         Height          =   315
         Left            =   -74640
         TabIndex        =   21
         Top             =   4560
         Width           =   2055
      End
      Begin VB.DirListBox dirSubs 
         Height          =   2565
         Left            =   -74640
         TabIndex        =   20
         Top             =   2040
         Width           =   2055
      End
      Begin VB.DriveListBox drvAifrDir 
         Height          =   315
         Left            =   2880
         TabIndex        =   16
         Top             =   4200
         Width           =   2295
      End
      Begin VB.DirListBox dirAifrDir 
         Height          =   3015
         Left            =   2880
         TabIndex        =   15
         Top             =   1200
         Width           =   2295
      End
      Begin VB.Frame Frame1 
         Caption         =   "Encoder Settings"
         Height          =   2295
         Left            =   5400
         TabIndex        =   6
         Top             =   840
         Width           =   2055
         Begin VB.Frame Frame3 
            Caption         =   "Game"
            Height          =   975
            Left            =   120
            TabIndex        =   52
            Top             =   240
            Width           =   1815
            Begin VB.OptionButton optEncHW 
               Caption         =   "Homeworld"
               Height          =   255
               Left            =   120
               TabIndex        =   54
               Top             =   360
               Value           =   -1  'True
               Width           =   1455
            End
            Begin VB.OptionButton optEncCat 
               Caption         =   "Cataclysm"
               Enabled         =   0   'False
               Height          =   255
               Left            =   120
               TabIndex        =   53
               Top             =   600
               Width           =   1455
            End
         End
         Begin VB.OptionButton optBr 
            Caption         =   "2048"
            Enabled         =   0   'False
            Height          =   255
            Index           =   5
            Left            =   1440
            TabIndex        =   13
            Top             =   2040
            Visible         =   0   'False
            Width           =   1575
         End
         Begin VB.OptionButton optBr 
            Caption         =   "1024"
            Enabled         =   0   'False
            Height          =   255
            Index           =   4
            Left            =   1440
            TabIndex        =   12
            Top             =   1800
            Visible         =   0   'False
            Width           =   1575
         End
         Begin VB.OptionButton optBr 
            Caption         =   "768"
            Enabled         =   0   'False
            Height          =   255
            Index           =   3
            Left            =   1440
            TabIndex        =   11
            Top             =   1560
            Visible         =   0   'False
            Width           =   1575
         End
         Begin VB.OptionButton optBr 
            Caption         =   "512"
            Enabled         =   0   'False
            Height          =   255
            Index           =   2
            Left            =   360
            TabIndex        =   10
            Top             =   1920
            Width           =   855
         End
         Begin VB.OptionButton optBr 
            Caption         =   "384"
            Enabled         =   0   'False
            Height          =   255
            Index           =   1
            Left            =   360
            TabIndex        =   9
            Top             =   1680
            Value           =   -1  'True
            Width           =   975
         End
         Begin VB.OptionButton optBr 
            Caption         =   "256"
            Enabled         =   0   'False
            Height          =   255
            Index           =   0
            Left            =   1440
            TabIndex        =   8
            Top             =   1320
            Visible         =   0   'False
            Width           =   1575
         End
         Begin VB.Label Label2 
            Caption         =   "Codec bitrate"
            Height          =   255
            Left            =   120
            TabIndex        =   7
            Top             =   1440
            Width           =   1335
         End
      End
      Begin VB.ListBox lstencout 
         BackColor       =   &H8000000B&
         Height          =   1035
         Left            =   360
         TabIndex        =   5
         Top             =   5160
         Width           =   7095
      End
      Begin VB.DriveListBox drvWavDir 
         Height          =   315
         Left            =   360
         TabIndex        =   4
         Top             =   4200
         Width           =   2295
      End
      Begin VB.DirListBox dirWavDir 
         Height          =   3015
         Left            =   360
         TabIndex        =   3
         Top             =   1200
         Width           =   2295
      End
      Begin VB.Label Label11 
         Caption         =   "Offset"
         Height          =   255
         Left            =   -69840
         TabIndex        =   42
         Top             =   3840
         Width           =   1335
      End
      Begin VB.Label Label10 
         Caption         =   "Sound #"
         Height          =   255
         Left            =   -71160
         TabIndex        =   41
         Top             =   3840
         Width           =   975
      End
      Begin VB.Label lblcnt 
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   -71160
         TabIndex        =   40
         Top             =   4080
         Width           =   975
      End
      Begin VB.Label lblpos 
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   -69840
         TabIndex        =   39
         Top             =   4080
         Width           =   1575
      End
      Begin VB.Label lblBitrateUsed 
         Alignment       =   2  'Center
         BorderStyle     =   1  'Fixed Single
         Caption         =   "256"
         Height          =   255
         Left            =   -70440
         TabIndex        =   33
         Top             =   2520
         Width           =   1455
      End
      Begin VB.Label Label9 
         Alignment       =   1  'Right Justify
         Caption         =   "Chosen Bitrate:"
         Height          =   255
         Left            =   -72000
         TabIndex        =   32
         Top             =   2520
         Width           =   1455
      End
      Begin VB.Label lblNumSubtitles 
         Alignment       =   2  'Center
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   -70440
         TabIndex        =   31
         Top             =   2160
         Width           =   1455
      End
      Begin VB.Label Label8 
         Alignment       =   1  'Right Justify
         Caption         =   "Number of Subtitles:"
         Height          =   255
         Left            =   -72120
         TabIndex        =   30
         Top             =   2160
         Width           =   1575
      End
      Begin VB.Label lblNumPhrases 
         Alignment       =   2  'Center
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   -70440
         TabIndex        =   29
         Top             =   1800
         Width           =   1455
      End
      Begin VB.Label Label7 
         Alignment       =   1  'Right Justify
         Caption         =   "Number of New Phrases:"
         Height          =   255
         Left            =   -72480
         TabIndex        =   28
         Top             =   1800
         Width           =   1935
      End
      Begin VB.Label lblVCEOut 
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   -72840
         TabIndex        =   23
         Top             =   1320
         Width           =   4935
      End
      Begin VB.Label Label6 
         Alignment       =   1  'Right Justify
         Caption         =   "Output VCE file:"
         Height          =   255
         Left            =   -74640
         TabIndex        =   22
         Top             =   1320
         Width           =   1575
      End
      Begin VB.Label lblEncfileList 
         BorderStyle     =   1  'Fixed Single
         Height          =   255
         Left            =   -72840
         TabIndex        =   19
         Top             =   960
         Width           =   4935
      End
      Begin VB.Label Label5 
         Caption         =   "Subtitle Files:"
         Height          =   255
         Left            =   -74640
         TabIndex        =   18
         Top             =   1800
         Width           =   1815
      End
      Begin VB.Label Label4 
         Alignment       =   1  'Right Justify
         Caption         =   "Encoded File List:"
         Height          =   255
         Left            =   -74880
         TabIndex        =   17
         Top             =   960
         Width           =   1815
      End
      Begin VB.Label Label3 
         Caption         =   "AIFR directory"
         Height          =   255
         Left            =   2880
         TabIndex        =   14
         Top             =   840
         Width           =   1575
      End
      Begin VB.Label Label1 
         Caption         =   "WAV directory"
         Height          =   255
         Left            =   360
         TabIndex        =   2
         Top             =   840
         Width           =   2055
      End
      Begin VB.Line Line2 
         X1              =   -74880
         X2              =   -67440
         Y1              =   720
         Y2              =   720
      End
      Begin VB.Line Line1 
         X1              =   120
         X2              =   7560
         Y1              =   720
         Y2              =   720
      End
   End
   Begin VB.Label Label12 
      Appearance      =   0  'Flat
      BorderStyle     =   1  'Fixed Single
      Caption         =   $"frmVCEPack.frx":0038
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   120
      TabIndex        =   50
      Top             =   6480
      Width           =   5655
   End
End
Attribute VB_Name = "frmVCEPack"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'progress bar
Private WithEvents progress As clsProgressBar, progress2 As clsProgressBar
Attribute progress.VB_VarHelpID = -1

Private Sub cmdBrowseFileList_Click()
    cd.filename = ""
    cd.Filter = "File List (*.log)|*.log"
    
    cd.ShowOpen
    If (cd.filename <> "") Then
        lblEncfileList.Caption = cd.filename
    End If
End Sub

Private Sub cmdBrowseVCEOut_Click()
    cd.filename = ""
    cd.Filter = "New VCE file (*.vce)|*.vce"
    
    cd.ShowOpen
    If (cd.filename <> "") Then
        lblVCEOut.Caption = cd.filename
    End If
End Sub

Private Sub cmdBuild_Click()
    cmdBuild.Enabled = False
    HaltPacking = False
    doNewVCE
    cmdBuild.Enabled = True
End Sub

Private Sub cmdCancel_Click()
    HaltPacking = True
End Sub

Private Sub cmdClose_Click()
    If (MsgBox("Are you sure you want to shutdown VCE Packer?", vbQuestion + vbYesNo, "Okay to shutdown?") = vbYes) Then
        Unload Me
    End If
End Sub

Private Sub cmdEncode_Click()
    Dim i As Integer, cmdstr As String, theBitrateStr, numfileperPercent As Integer
    
    
    If (lstFiles.ListCount = 0) Then
        MsgBox "Choose a directory with WAV files!", vbCritical, "error"
        Exit Sub
    End If
    
    numfileperPercent = 100 / lstFiles.ListCount
    
    ProgressBar1.Min = 1
    ProgressBar1.Max = lstFiles.ListCount + 1
    
    'get the bitrate as a string
    For i = br_256 To br_2048
        If (optBr(i).value = True) Then
            Exit For
        End If
    Next i
    
    'progress.percentage = 0
    cmdEncode.Enabled = False
    
    openEncLogFile
    
    Select Case (i)
        Case br_256:
            theBitrateStr = Str(256)
        Case br_384:
            theBitrateStr = Str(384)
        Case br_512:
            theBitrateStr = Str(512)
        Case br_768:
            theBitrateStr = Str(768)
        Case br_1024:
            theBitrateStr = Str(1024)
        Case br_2048:
            theBitrateStr = Str(2048)
    End Select
    
    ' remember this bitrate
'    lblBitrateUsed.Caption = theBitrateStr
    Slider1.value = i
    
    ' encode the wav files
    For i = 1 To lstFiles.ListCount
        
        breakfilename lstFiles.List(i - 1)
        cmdstr = App.Path & "\enc " & dirWavDir.Path & "\" & lstFiles.List(i - 1)
        cmdstr = cmdstr & " " & dirAifrDir.Path & "\" & bfn.fnameNoExt & ".aifr"
        cmdstr = cmdstr & " " & theBitrateStr
        
        lstencout.AddItem cmdstr
        lstencout.ListIndex = lstencout.ListCount - 1
        
        DoEvents
        ExecCmd cmdstr
'        ShellAndWait cmdstr
'        progress.percentage = progress.percentage + numfileperPercent
'        progress.DrawStatus picProgress
        DoEvents
        
        ProgressBar1.value = i
        
        addToEncLogFile i - 1
    Next i
    cmdEncode.Enabled = True
    closeEncLogFile
    setEncLog
End Sub

Private Sub cmdLengths_Click()
    Dim i As Integer, tfn As Integer, tfname As String, outfn As Integer, outname As String
    Dim playlength As Single, flen As Long, twhdr As wavHeader, tname As String
    Dim playlengthms As Long
    
    If (lstFiles.ListCount = 0) Then
        MsgBox "Choose a directory with WAV files!", vbCritical, "error"
        Exit Sub
    End If

    ' gather the file playback lengths for each wav and save them
    lstencout.AddItem "Getting WAV file lengths"
    
    'open an output file
    outname = dirAifrDir.Path & "\lengths.log"
    If (Dir(outname, vbNormal) <> "") Then
        Kill outname
    End If
    
    outfn = FreeFile
    Open outname For Output As #outfn
        
    For i = 0 To lstFiles.ListCount - 1
        tfn = FreeFile
        tfname = dirWavDir.Path & "\" & lstFiles.List(i)
        
        'open the next file
        flen = FileLen(tfname)
        Open tfname For Binary Access Read As #tfn
        Get #tfn, , twhdr
        Close #tfn
        playlength = (flen - 36) / (twhdr.nSamplesPerSec * (twhdr.nBitsPerSample / 8) * twhdr.nChannels)
        playlengthms = playlength * 1000                           ' convert to milliseconds
        tname = Left(lstFiles.List(i), Len(lstFiles.List(i)) - 4)
        Print #outfn, tname & "," & Trim(Str(playlengthms))
    Next i
    
    lstencout.AddItem "Finished WAV file lengths"
    lstencout.ListIndex = lstencout.ListCount - 1
    
    Close #outfn
End Sub

Private Sub dirWavDir_Change()
    lstFiles.Path = dirWavDir.Path
    lstencout.AddItem "Found " & Str(lstFiles.ListCount) & " WAV files in this directory"
    lstencout.ListIndex = lstencout.ListCount - 1
End Sub

Private Sub drvAifrDir_Change()
    dirAifrDir.Path = drvAifrDir.Drive
End Sub

Private Sub drvSubs_Change()
    dirSubs.Path = drvSubs.Drive
End Sub

Private Sub drvWavDir_Change()
    dirWavDir.Path = drvWavDir.Drive
End Sub

Private Sub Form_Load()
'    initProgressBars
    lblcnt.Caption = "0"
    
'    picProgress.Width = 400 * (Screen.TwipsPerPixelX)
'    picProgress.Left = Me.Width / 2 - picProgress.Width / 2
    
    verifyFiles
    Slider1.value = br_384
    lblBitrateUsed.Caption = "384"
End Sub

Private Sub initProgressBars()
    Set progress = New clsProgressBar
    Set progress2 = New clsProgressBar
    
    progress.BackColor = RGB(0, 0, 0)
    progress.ForeColor = RGB(0, 255, 0)
    progress.InitProgress picProgress
    
    progress2.BackColor = RGB(0, 0, 0)
    progress2.ForeColor = RGB(0, 255, 0)
    progress2.InitProgress picProgress2
   
End Sub

Private Sub progress_ProgressDone()
'    Timer1.Enabled = False
'    MsgBox "done"
End Sub

Private Sub progress2_ProgressDone()
'    Timer1.Enabled = False
    MsgBox "done"
End Sub

'Private Sub Timer1_Timer()
'    progress.value = progress.value + 48
'    progress.percentage = progress.percentage + 8
'    lblcnt.Caption = Str(progress.percentage)
'    progress.DrawStatus picProgress
'End Sub

Private Sub PackNewVCE()
    Dim i As Integer, fname As String
    
    addBlankSound                       ' just in case we need a blank sound
    gblcount = 0
    For i = 0 To 11291
        fname = getNextAIFRName(i)
        lblcnt.Caption = Str(i + 1)
        
        ' if blank sound
        If (fname = App.Path & "\blank.aifr") Then
            addBlankPhraseEntry
        Else
            buildIntermediateAif (fname)
            addPhraseAifrToVCE
            addPhraseFileEntry
        End If
        
        If (HaltPacking = True) Then
            MsgBox "Emergency Halt!", vbCritical, "Halting"
            Exit Sub
        End If
        ProgressBar2.value = i + 1
        
'        List1.AddItem "done " & Str(i)
 '       List1.ListIndex = List1.ListCount - 1
    Next i
    
End Sub

Private Sub doNewVCE()
    
    countAIFRFiles
    getSubtitleFilter               ' - new on 3/13/2002
    getPlaybackLengths              ' - new on 5/20/2002
    
    If (HaltPacking = True) Then
        MsgBox "Emergency Halt!", vbCritical, "Halting"
        Exit Sub
    End If
    
    getSubtitleFiles
    showInfo
    setBitratesInSentenceFile
    openNewPhraseLutFile
    openNewVCE
    
    lstPackOut.AddItem "Packing New VCE and making Phrase lut file"
    lstPackOut.ListIndex = frmVCEPack.lstPackOut.ListCount - 1

    PackNewVCE
    
    closeNewPhraseLutFile
    closeNewVCE
    
    lstPackOut.AddItem "FINISHED!"
    lstPackOut.ListIndex = frmVCEPack.lstPackOut.ListCount - 1

End Sub

Private Sub Slider1_Click()
    Select Case (Slider1.value)
        Case 0
            lblBitrateUsed.Caption = "256"
        Case 1
            lblBitrateUsed.Caption = "384"
        Case 2
            lblBitrateUsed.Caption = "512"
        Case 3
            lblBitrateUsed.Caption = "768"
        Case 4
            lblBitrateUsed.Caption = "1024"
        Case 5
            lblBitrateUsed.Caption = "2048"
    End Select
End Sub

Private Sub SSTab1_DblClick()
    picProgress.Visible = False
End Sub
