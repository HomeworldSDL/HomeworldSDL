Attribute VB_Name = "modGeneral"
Option Explicit

Enum CDTYPES
    CD_SAVE
    CD_OPEN
End Enum

Enum workdirenums
    workdir_main = 0
    workdir_mp32wav
    workdir_wav2aifr
    workdir_aifr2aifrx
    workdir_aifr2aifrb
    workdir_aifr2aifrv
    workdir_bnk
    workdir_wxd
    workdir_vce
    workdir_number
End Enum

Enum fnTypes
    fname
    fullfname
    fpath
    fext
    fdrive
End Enum

'for filenames
Type aFN
    filename As String
    fpath As String
    fext As String
    fdrv As String
    fnameNoExt As String
    fullpath As String
End Type

Private Type STARTUPINFO
    cb As Long
    lpReserved As String
    lpDesktop As String
    lpTitle As String
    dwX As Long
    dwY As Long
    dwXSize As Long
    dwYSize As Long
    dwXCountChars As Long
    dwYCountChars As Long
    dwFillAttribute As Long
    dwFlags As Long
    wShowWindow As Integer
    cbReserved2 As Integer
    lpReserved2 As Byte
    hStdInput As Long
    hStdOutput As Long
    hStdError As Long
End Type

Private Type PROCESS_INFORMATION
    hProcess As Long
    hThread As Long
    dwProcessId As Long
    dwThreadID As Long
End Type
Private Const NORMAL_PRIORITY_CLASS = &H20&
Private Const INFINITE = -1&


Private Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Private Declare Function CreateProcessA Lib "kernel32" (ByVal lpApplicationName As Long, ByVal lpCommandLine As String, ByVal lpProcessAttributes As Long, ByVal lpThreadAttributes As Long, ByVal bInheritHandles As Long, ByVal dwCreationFlags As Long, ByVal lpEnvironment As Long, ByVal lpCurrentDirectory As Long, lpStartupInfo As STARTUPINFO, lpProcessInformation As PROCESS_INFORMATION) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long


Global workDir(workdir_number) As String, appDir As String
Global bfn As aFN

Enum extractTypes
    Extract_BNK = 0
    Extract_WXD
    Extract_VCE
End Enum

Global ExtractTypeNum As extractTypes


Public Function getFileNameString(cd As CommonDialog, filter As String, SAVE_OPEN As CDTYPES) As String
    Dim lcd As CommonDialog
    
    Set lcd = cd
    
    lcd.filename = ""
    lcd.filter = filter
    
    If (SAVE_OPEN = CD_OPEN) Then
        lcd.ShowOpen
    Else
        lcd.ShowSave
    End If
    
    getFileNameString = lcd.filename
End Function

Public Sub breakfilename(aStr As String)
    Dim i As Integer, tmpchar As String, tmpstr As String, holder As Integer
    
    'cheap and easy way to get the elements of a filename and its path
    
    bfn.fdrv = ""
    bfn.fext = ""
    bfn.filename = ""
    bfn.fnameNoExt = ""
    bfn.fpath = ""
    
    tmpstr = ""
    
    'get extension
    For i = Len(aStr) To 1 Step -1
        tmpchar = Mid(aStr, i, 1)
        If (tmpchar <> ".") Then
            tmpstr = tmpstr & tmpchar
        Else
            Exit For
        End If
    Next i
    
    bfn.fext = reverseStr(tmpstr)
    holder = i - 1
    
    'get name of file
    tmpstr = ""
    For i = holder To 1 Step -1
        tmpchar = Mid(aStr, i, 1)
        If (tmpchar <> "\") Then
            tmpstr = tmpstr & tmpchar
        Else
            Exit For
        End If
    Next i
    
    bfn.fnameNoExt = reverseStr(tmpstr)
    ' full name and extension
    bfn.filename = bfn.fnameNoExt & "." & bfn.fext
    
    holder = i - 1
    
    'get drive letter
    bfn.fdrv = Left(aStr, 1)
    
    'get path
    tmpstr = ""
    For i = 4 To holder             '4 skips the letter, the colon and the 1st slash
        tmpchar = Mid(aStr, i, 1)
        tmpstr = tmpstr & tmpchar
    Next i
    
    bfn.fpath = tmpstr
    
    bfn.fullpath = bfn.fdrv & ":\" & bfn.fpath & "\"
End Sub

Public Function reverseStr(aStr As String) As String
    Dim i As Integer, tmpstr As String
    
    tmpstr = ""
    For i = Len(aStr) To 1 Step -1
        tmpstr = tmpstr & Mid(aStr, i, 1)
    Next i
    
    reverseStr = tmpstr
End Function

Public Sub mainSetup()
    Dim i As Integer
    
    'change to read from an INI file
    workDir(workdir_main) = "c:\wrkhwss\"
    workDir(workdir_mp32wav) = workDir(workdir_main) & "m2w\"
    workDir(workdir_wav2aifr) = workDir(workdir_main) & "w2ar\"
    workDir(workdir_aifr2aifrb) = workDir(workdir_main) & "ar2arb\"
    workDir(workdir_aifr2aifrx) = workDir(workdir_main) & "ar2arx\"
    workDir(workdir_aifr2aifrv) = workDir(workdir_main) & "ar2arv\"
    workDir(workdir_bnk) = workDir(workdir_main) & "bnk\"
    workDir(workdir_wxd) = workDir(workdir_main) & "wxd\"
    workDir(workdir_vce) = workDir(workdir_main) & "vce\"
    
    appDir = App.Path & "\"
    
    'check for working subdirs
    For i = 0 To workdir_number - 1
        If (Dir(workDir(i), vbDirectory) = "") Then
            MkDir workDir(i)
        End If
    Next i
    
    
    frmMain.filemp32wavlist.Path = frmMain.dirmp32wav.Path
    frmMain.optExtractBNK.Value = True
    
    addedBnkEntries = 0
    
End Sub

Public Sub ExecCmd(cmdLine As String)



Dim Proc As PROCESS_INFORMATION
Dim start As STARTUPINFO
Dim ret&

'Shell cmdline, vbNormalFocus
'Exit Sub

' Initialize the STARTUPINFO structure:
start.cb = Len(start)

' Start the shelled application:
ret& = CreateProcessA(0&, cmdLine, 0&, 0&, 1&, NORMAL_PRIORITY_CLASS, 0&, 0&, start, Proc)
' Wait for the shelled application to finish:
ret& = WaitForSingleObject(Proc.hProcess, INFINITE)
ret& = CloseHandle(Proc.hProcess)
End Sub

'Generate a 32 Bit little endian
Public Function MKLEndian32(l As Long) As String

Dim b0 As Byte, b1 As Byte, b2 As Byte, b3 As Byte
    b0 = (l And &HFF&)
    b1 = (l And &HFF00&) \ &H100&
    b2 = (l And &HFF0000) \ &H10000
    b3 = (l And &H7F000000) \ &H1000000 - 128 * (l < 0)
    MKLEndian32 = Chr(b0) & Chr(b1) & Chr(b2) & Chr(b3)
End Function

'Generate a 32 Bit big endian
Public Sub MKBEndian32(l As Long, ByRef tArray() As Byte)
    Dim b0 As Byte, b1 As Byte, b2 As Byte, b3 As Byte

    b0 = (l And &HFF&)
    b1 = (l And &HFF00&) \ &H100&
    b2 = (l And &HFF0000) \ &H10000
    b3 = (l And &HFF000000) \ &H1000000
    
    tArray(0) = b0
    tArray(1) = b1
    tArray(2) = b2
    tArray(3) = b3
End Sub

Public Sub MKBEndian32_2(l As Long, ByRef tArray() As Byte)
    Dim b0 As Byte, b1 As Byte, b2 As Byte, b3 As Byte

    b0 = (l And &HFF&)
    b1 = (l And &HFF00&) \ &H100&
    b2 = (l And &HFF0000) \ &H10000
    b3 = (l And &HFF000000) \ &H1000000
    
    tArray(3) = b0
    tArray(2) = b1
    tArray(1) = b2
    tArray(0) = b3
End Sub

