Attribute VB_Name = "modgeneral"
Option Explicit

Enum bitrates
    br_256 = 0
    br_384                      ' 384 is the only "safe" one right now
    br_512
    br_768
    br_1024
    br_2048
End Enum

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
End Type 'for filenames
Type aFN
    filename As String
    fpath As String
    fext As String
    fdrv As String
    fnameNoExt As String
    fullpath As String
End Type

Type vcephrase
    number As Integer
    offset As Long
    subtitlelen As Long
    subt As String
    rawsoundlen As Long
End Type


Private Const NORMAL_PRIORITY_CLASS = &H20&
Private Const INFINITE = -1&

Private Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Private Declare Function CreateProcessA Lib "kernel32" (ByVal lpApplicationName As Long, ByVal lpCommandLine As String, ByVal lpProcessAttributes As Long, ByVal lpThreadAttributes As Long, ByVal bInheritHandles As Long, ByVal dwCreationFlags As Long, ByVal lpEnvironment As Long, ByVal lpCurrentDirectory As Long, lpStartupInfo As STARTUPINFO, lpProcessInformation As PROCESS_INFORMATION) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Global bfn As aFN
Global encLogFN As Integer
Global newAIFRfiles(0 To 11291) As Boolean, newaifrcount As Integer
Global newSubtitles(0 To 11291) As Boolean, newsubtcount As Integer
Global newvcePhrases(0 To 11291) As vcephrase, vfn As Integer, vcelen As Long
Global phFN As Integer, oldPhfn As Integer, tphlutr As phraselutRecord, thisDur As Long
Global taifhdr As aifrHeader, aifrnames(0 To 11291) As String, prevVCELen As Long
Global gblcount As Integer
Global subFilter(0 To 11291) As Boolean, pblengths(0 To 11291) As Long
Global HaltPacking As Boolean, USING_BLANK As Boolean, firstVCEEntry As Long
Global curFname As String

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

Function ShellAndWait(filename As String)
    Dim objScript, ShellApp
    On Error GoTo ERR_OpenForEdit
    Set objScript = CreateObject("WScript.Shell")
    'The second parameter (after the FileNam
    '     e) is the Display Mode (normal w/focus,
    'minimized...even hidden. For more info
    '     visit:
    'http://msdn.microsoft.com/scripting/win


    '     dowshost/doc/wsMthRun.htm
        ' The third parameter is the "Wait for return" parameter.
        ' This should be set to
        ' True for the Wait.
        ShellApp = objScript.Run(filename, 1, True)
        ShellAndWait = True
EXIT_OpenForEdit:
        Exit Function
ERR_OpenForEdit:
        MsgBox Err.description
        GoTo EXIT_OpenForEdit
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

Public Sub verifyFiles()
    If (Dir(App.Path & "\speechphrase_comp.lut") = "") Then
        MsgBox "Couldn't find phrase file [speechphrase_comp.lut]", vbCritical, "Error    "
    End If
    
    If (Dir(App.Path & "\speechsentence_comp.lut") = "") Then
        MsgBox "Couldn't find sentence file [speechsentence_comp.lut]", vbCritical, "Error    "
    End If
    
End Sub

Public Sub openEncLogFile()
    encLogFN = FreeFile
    
    Open frmVCEPack.dirAifrDir.Path & "\encfiles.log" For Output As encLogFN
End Sub

Public Sub closeEncLogFile()
    Close encLogFN
End Sub

Public Sub addToEncLogFile(which As Integer)
    Dim tname As String
    breakfilename frmVCEPack.lstFiles.List(which)
    tname = bfn.fnameNoExt & ".aifr"
    Print #encLogFN, frmVCEPack.dirAifrDir.Path & "\" & tname
End Sub

Public Sub setEncLog()
    frmVCEPack.lblEncfileList.Caption = frmVCEPack.dirAifrDir.Path & "\encfiles.log"
End Sub

Public Sub countAIFRFiles()
    Dim i As Integer, tmpname As String, fn As Integer, tStr As String
    
    If (frmVCEPack.lblEncfileList.Caption = "") Then
        MsgBox "Set the encoded file list first!", vbCritical, "Error"
        Exit Sub
    End If
    
    For i = 0 To 11291
        newAIFRfiles(i) = False
        aifrnames(i) = ""
        DoEvents
    Next i
    
    frmVCEPack.lstPackOut.AddItem "Counting available aifr files"
    frmVCEPack.lstPackOut.ListIndex = frmVCEPack.lstPackOut.ListCount - 1
    
    fn = FreeFile
    
    Open frmVCEPack.lblEncfileList.Caption For Input As #fn
    
    Do
        On Error Resume Next
        
        Line Input #fn, tStr
        If (Dir(tStr, vbNormal) <> "") Then
            breakfilename tStr
            newAIFRfiles(Val(bfn.fnameNoExt)) = True
            aifrnames(Val(bfn.fnameNoExt)) = tStr
            newaifrcount = newaifrcount + 1
        End If
        
        On Error GoTo 0
        
        DoEvents
    Loop Until (EOF(fn))
    
    Close #fn
End Sub

Public Sub getSubtitleFiles()
    Dim i As Integer, tname As String
    
    frmVCEPack.lstPackOut.AddItem "Loading subtitle files"
    frmVCEPack.lstPackOut.ListIndex = frmVCEPack.lstPackOut.ListCount - 1

    For i = 0 To 11291
        tname = Trim(Str(i) & ".txt")
        If (Dir(frmVCEPack.dirSubs.Path & "\" & tname) = "") Then
            If (subFilter(i) = True) Then
                If (frmVCEPack.chkStrictSubs.value = 1) Then
                    MsgBox "Missing subtitle file: " & tname, vbCritical, "Halting"
                    HaltPacking = True
                    Exit Sub
                End If
            Else
                newSubtitles(i) = False
            End If
        Else
            If (subFilter(i) = True) Then
                newSubtitles(i) = True
                newsubtcount = newsubtcount + 1
            Else
                newSubtitles(i) = False
            End If
        End If
        DoEvents
    Next i

End Sub

Public Sub showInfo()
    frmVCEPack.lblNumPhrases.Caption = Str(newaifrcount)
    frmVCEPack.lblNumSubtitles.Caption = Str(newsubtcount)
End Sub

Public Sub addPhraseFileEntry()
    Dim i As Integer, tLong As Long
    
    getOldPhraseFileEntry
    tLong = tphlutr.offsets(3)
    tphlutr.duration = getDuration
    tphlutr.numoffsets = 1
    tphlutr.offsets(0) = prevVCELen
    For i = 1 To 6
        tphlutr.offsets(i) = tLong
    Next i
    
    Put #phFN, , tphlutr
End Sub

Public Sub getOldPhraseFileEntry()
    Get #oldPhfn, , tphlutr
End Sub

Public Sub getPhraseOffset()

End Sub

Public Sub openNewVCE()
    Dim tvceh As VCE_Header
    
    vfn = FreeFile
    Open frmVCEPack.lblVCEOut.Caption For Binary Access Write As #vfn
    
    'put vce header
    tvceh.signature = "VCE0"
    tvceh.checksum = &H7D54CC80         ' just use original checksum
    Put #vfn, , tvceh
    
    vcelen = 8                          ' size of this header
End Sub

Public Sub closeNewVCE()
    Close #vfn
End Sub

Public Function addPhraseAifrToVCE() As Boolean
    Dim tfn As Integer, ibyte As Byte, pos As Long, t As Long
    
    If (Dir(frmVCEPack.dirAifrDir.Path & "\tmp.aifrx") = "") Then
        addPhraseAifrToVCE = False
        Exit Function
    End If
    
    tfn = FreeFile
    
    Open frmVCEPack.dirAifrDir.Path & "\tmp.aifrx" For Binary Access Read As #tfn
    
    For t = 1 To FileLen(frmVCEPack.dirAifrDir.Path & "\tmp.aifrx")
        Get #tfn, , ibyte
        Put #vfn, , ibyte
        DoEvents
    Next t
    
    pos = Seek(vfn)
    frmVCEPack.lblpos.Caption = Str(pos)
    
    Close #tfn
End Function

Public Sub setBitratesInSentenceFile()
    Dim fn As Integer, fname As String, thdr As speechlutHeader, i As Integer, fn2 As Integer, ibyte As Byte
        
    fname = App.Path & "\speechsentence_comp.lut"
        
    frmVCEPack.lstPackOut.AddItem "Setting sentence lut file bitrates"
    frmVCEPack.lstPackOut.ListIndex = frmVCEPack.lstPackOut.ListCount - 1

    'we checked earlier, it should be here!"
    'make a copy
    If (Dir(App.Path & "\speechsentence_comp.bak") <> "") Then
        Kill (App.Path & "\speechsentence_comp.bak")
    End If
    
    Name fname As App.Path & "\speechsentence_comp.bak"
    fn = FreeFile
    fname = App.Path & "\speechsentence_comp.bak"
    Open fname For Binary Access Read As #fn
    fn2 = FreeFile
    Open App.Path & "\speechsentence_comp.lut" For Binary Access Write As #fn2
    
    Get #fn, , thdr
    
    'set them all to the same value
    For i = 0 To 3
        thdr.compbitrate(i) = Val(frmVCEPack.lblBitrateUsed.Caption)
    Next i

    'write the header back out
    Put #fn2, , thdr
    
    Do
        Get #fn, , ibyte
        Put #fn2, , ibyte
        DoEvents
    Loop Until EOF(fn)
    
    Close #fn
    Close #fn2
End Sub

Public Sub buildIntermediateAif(filename As String)
    Dim fname As String, fn As Integer, thelen As Long, ndx As Integer, fn2 As Integer, ibyte As Byte
    Dim fourstring As String * 4, longval As Long, icount As Long, i As Long
    Dim tvcerech As VCE_SoundEntry_header, tvcerec1 As VCE_SoundEntry
    

    breakfilename filename
    
    'ndx = Val(bfn.fnameNoExt)
    ndx = gblcount
    
    newvcePhrases(ndx).number = ndx
    If (newSubtitles(ndx) = True) Then
        newvcePhrases(ndx).subtitlelen = FileLen(frmVCEPack.dirSubs & "\" & Trim(bfn.fnameNoExt) & ".txt")
        newvcePhrases(ndx).subt = getSubT(frmVCEPack.dirSubs & "\" & Trim(bfn.fnameNoExt) & ".txt")
    Else
        newvcePhrases(ndx).subt = ""
        newvcePhrases(ndx).subtitlelen = 0
    End If
    
    'delete old temp file
    If (Dir(frmVCEPack.dirAifrDir.Path & "\tmp.aifrx") <> "") Then
        Kill frmVCEPack.dirAifrDir.Path & "\tmp.aifrx"
    End If
    
    fn = FreeFile
    Open filename For Binary Access Read As #fn
    fn2 = FreeFile
    Open (frmVCEPack.dirAifrDir.Path & "\tmp.aifrx") For Binary Access Write As #fn2
    
    thelen = FileLen(filename) - 90
'    Seek #fn, thelen
    Get #fn, , taifhdr
    
    'put prelim stuff
    tvcerech.InfoString = "INFO"
    tvcerech.textLength = newvcePhrases(ndx).subtitlelen
    tvcerec1.DataString = "DATA"
    tvcerec1.soundSize = thelen
    
    ' add the vce entry header plus the text length plus the raw sound length
    icount = 4 + 4 + 4 + 4 + tvcerech.textLength + tvcerec1.soundSize
    
    Put #fn2, , tvcerech
    If (tvcerech.textLength <> 0) Then
        Put #fn2, , newvcePhrases(ndx).subt
    End If
    Put #fn2, , tvcerec1
    
    'raw sound data goes next
    'Do
    'Debug.Print (Seek(fn))
    For i = 1 To tvcerec1.soundSize
        Get #fn, , ibyte
        Put #fn2, , ibyte
        DoEvents
    Next i
    'Loop Until (EOF(fn))
    
    prevVCELen = vcelen
    vcelen = vcelen + icount
    
    Close #fn
    Close #fn2
End Sub

Public Function getSubT(thefile As String)
    Dim fn As Integer, tStr As String, tchar As Byte, i As Integer
    
    fn = FreeFile
'    Open thefile For Input As #fn
    Open thefile For Binary Access Read As #fn
    
    tStr = ""
    'Do
    For i = 1 To FileLen(thefile)
        Get #fn, , tchar
        If (tchar = &HD) Or (tchar = &HA) Then
            tchar = tchar
        End If
        tStr = tStr & Chr(tchar)
    Next i
'    Loop Until (EOF(fn))
    
'    Line Input #fn, tStr
    
    
'    For i = 1 To Len(tStr)
'        tchar = Mid(tStr, i, 1)
'        If (tchar = vbCr Or tchar = vbLf) Then
'            tchar = tchar
'        End If
'    Next i
    
    Close #fn
    getSubT = tStr

End Function

Function openNewPhraseLutFile()
    Dim tphlutr As phraselutFile
    
    If (Dir(App.Path & "\speechphrase_comp.bak", vbNormal) <> "") Then
        Kill (App.Path & "\speechphrase_comp.bak")
    End If
    
    Name (App.Path & "\speechphrase_comp.lut") As (App.Path & "\speechphrase_comp.bak")
    
    oldPhfn = FreeFile
    Open App.Path & "\speechphrase_comp.bak" For Binary Access Read As oldPhfn
    
    phFN = FreeFile
    Open App.Path & "\speechphrase_comp.lut" For Binary Access Write As #phFN
    
    'copy the header
    Get oldPhfn, , tphlutr
    Put #phFN, , tphlutr
        
End Function

Function closeNewPhraseLutFile()
    Close #phFN
    Close #oldPhfn
End Function

Public Function getDuration() As Long
'    Dim numSamps1 As Long, numSampsPerSecond1 As Long, numChans1 As Integer, numBytesPerSamp1 As Integer
'
'    numSamps1 = reverseBytesToLong(taifhdr.numSamples)
'    numChans1 = reverseBytesToInt(taifhdr.numChannels)
'    numBytesPerSamp1 = reverseBytesToInt(taifhdr.sampleSize) / 8
'    numSampsPerSecond1 = getSampleRate(taifhdr.sRateSpecial)
'
'    getDuration = retrieveSoundTimeLength(numSampsPerSecond1, numSamps1, numChans1, numBytesPerSamp1) * 1000

    breakfilename curFname
    
    ' --- these better just be numbers for filenames, else this won't work!
    getDuration = pblengths(Val(bfn.fnameNoExt))


End Function

Public Function getNextAIFRName(cur As Integer) As String
    Dim i As Integer
    
    For i = cur To 11291
        If (aifrnames(i) <> "") Then            'if we have a custom one
            getNextAIFRName = aifrnames(i)
            curFname = aifrnames(i)
            gblcount = i
            Exit For
        Else                                    ' else just do a silent one
            getNextAIFRName = App.Path & "\blank.aifr"
            curFname = aifrnames(i)
            Exit For
        End If
    Next i
End Function

Public Sub getSubtitleFilter()
    Dim i As Integer, sfn As Integer, sfname As String, which As String, onoff As String
    
    frmVCEPack.lstPackOut.AddItem "Getting Subtitle filter"
    frmVCEPack.lstPackOut.ListIndex = frmVCEPack.lstPackOut.ListCount - 1
    
    sfname = App.Path & "\filter.dat"
    If (Dir(sfname, vbNormal) = "") Then
        MsgBox "HEY, I can't find the filter.dat file!  I won't allow subtitles without it!", vbCritical, "File Not Found!"
        Exit Sub
    End If
    
    sfn = FreeFile
    Open sfname For Input As #sfn
    
    For i = 0 To 11291
        Input #sfn, which, onoff
        which = Trim(which)
        onoff = Trim(onoff)
        If (onoff = "0") Then
            subFilter(i) = False
        Else
            subFilter(i) = True
        End If
    Next i
    
    Close #sfn
End Sub

Public Sub addBlankSound()
    Dim blfn As Integer, bfname As String, icount As Long, i As Long
    Dim tvcerech As VCE_SoundEntry_header, tvcerec1 As VCE_SoundEntry
    Dim j As Integer, thelen As Long, ibyte As Byte, pos As Long
    
    ' this goes in as the first vce entry... all blank phrases point to this one!
    
    firstVCEEntry = vcelen          ' this should be 8 at this point
    
    blfn = FreeFile
    bfname = App.Path & "\blank.aifr"
    
    Open bfname For Binary Access Read As #blfn
    
    ' ---------------------- copied from buildIntermediateAifrFile
    thelen = FileLen(bfname) - 90
    Get #blfn, , taifhdr
    
    'put prelim stuff
    tvcerech.InfoString = "INFO"
    tvcerech.textLength = 0
    tvcerec1.DataString = "DATA"
    tvcerec1.soundSize = thelen
    
    ' add the vce entry header plus the text length plus the raw sound length
    icount = 4 + 4 + 4 + 4 + tvcerech.textLength + tvcerec1.soundSize
    
    Put #vfn, , tvcerech        ' add the tmp headers to vce
    Put #vfn, , tvcerec1
    
    'raw sound data goes next
    'Debug.Print (Seek(fn))
    For i = 1 To tvcerec1.soundSize
        Get #blfn, , ibyte
        Put #vfn, , ibyte
        DoEvents
    Next i
    
    prevVCELen = vcelen
    vcelen = vcelen + icount
    pos = Seek(vfn)
    frmVCEPack.lblpos.Caption = Str(pos)

    ' --------------------- end of copy
    
    Close #blfn
    
End Sub

Public Sub addBlankPhraseEntry()
    Dim i As Integer, tLong As Long
    
    getOldPhraseFileEntry
    tLong = tphlutr.offsets(3)
'    tphlutr.duration = getDuration
    tphlutr.duration = 0                    'blank entries shouldn't have subtitles
    tphlutr.numoffsets = 1
    tphlutr.offsets(0) = firstVCEEntry
    For i = 1 To 6
        tphlutr.offsets(i) = tLong
    Next i
    
    Put #phFN, , tphlutr

End Sub

Public Sub getPlaybackLengths()
    Dim tfn As Long, i As Integer, tname As String, nextname As String
    Dim pname As Integer, plength As Long
    
    frmVCEPack.lstPackOut.AddItem "getting playback lengths"
    frmVCEPack.lstPackOut.ListIndex = frmVCEPack.lstPackOut.ListCount - 1
    
    'prefill
    For i = 0 To 11291
        pblengths(i) = 0
    Next i
    
    breakfilename frmVCEPack.lblEncfileList.Caption
    
    tname = bfn.fullpath & "\lengths.log"
    If (Dir(tname, vbNormal) = "") Then
        frmVCEPack.lstPackOut.AddItem "COULDN'T FIND LENGTHS.LOG!"
        frmVCEPack.lstPackOut.AddItem "subtitles won't display!"
        frmVCEPack.lstPackOut.ListIndex = frmVCEPack.lstPackOut.ListCount - 1
        Exit Sub
    End If
    
    tfn = FreeFile
    Open tname For Input As #tfn
    
    '==========these files better be just numbers for filenames!!!!
    
    'loop through all entries
    Do
        Input #tfn, pname, plength
        pblengths(Val(pname)) = Val(plength)
    Loop Until EOF(tfn)
    
    Close #tfn
End Sub
