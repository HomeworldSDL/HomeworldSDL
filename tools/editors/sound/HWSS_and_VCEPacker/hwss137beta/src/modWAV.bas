Attribute VB_Name = "modWAV"
' just a quick note on the format of a wav file

Type wavHeader
    riffStr As String * 4       ' always "RIFF"
    filesze As Long             ' filesize minus 8 bytes
    waveStr As String * 4       ' always "WAVE"
    ckID As String * 4          ' always "fmt "
    nChunkSize As Long          ' size of the fmt chunk (usually 16?)
    wFormatTag As Integer       ' if it is 1, then it is PCM
    nChannels As Integer        ' 2 if stereo, 1 if mono
    nSamplesPerSec As Long      ' frequency (11025, 22050, 44100)
    nAvgBytesPerSec As Long     ' data rate for real time playback - easy if it is PCM
    nBlockAlign As Integer      ' bytes/sample * num channels for PCM
    nBitsPerSample As Integer   ' 8 or 16 bit for PCM
End Type

Public Function makeAifrLogFromWav(wavfile As String, bRate As Integer) As Boolean
    Dim tfn As Integer, thdr As wavHeader, ofn As Integer, ofname As String, tsize As Long, pbl As Integer
    
    'open the file
    tfn = FreeFile
    On Error GoTo wavhdrerr
    Open wavfile For Binary Access Read As #tfn
    On Error GoTo 0
    
    'get the header
    Get #tfn, , thdr
    
    'close the file
    Close #tfn
    
    'open the log file to write to
    breakfilename wavfile
    ofname = bfn.fullpath & bfn.fnameNoExt & ".log"
    ofn = FreeFile
    On Error GoTo wavhdrerr
    Open ofname For Output As #ofn
    On Error GoTo 0
    
    'write the info
    Print #tfn, "[" & bfn.fnameNoExt & "]"
    Print #tfn, "numchannels=" & Str(thdr.nChannels)
    Print #tfn, "codecbitrate=" & Str(bRate)
    Print #tfn, "frequency=" & Str(thdr.nSamplesPerSec)
    Print #tfn, "bits=" & Str(thdr.nBitsPerSample)
    
    'get playback length
    tsize = FileLen(wavfile) - 36                   'size of wav header
    pbl = (tsize / thdr.nChannels) / thdr.nSamplesPerSec
    Print #tfn, "playbacklength=" & Str(pbl)        ' this is used in speech section for subtitle display time
    
    'close it
    Close #ofn
    
    makeAifrLogFromWav = True
    Exit Function
wavhdrerr:
    makeAifrLogFromWav = False
End Function

Public Function wavGetFrequency(cofile As String) As Long
    Dim tname As String, tline As String, tstr As String, tfn As Integer
    
    ' strip out the extension from this cofile name and get the log file form it...hope it exists!!!!
    breakfilename (cofile)
    tname = bfn.fullpath & bfn.fnameNoExt & ".log"
    If (Dir(tname, vbNormal) = "") Then
        MsgBox "Couldn't find the log file for sound file: " & bfn.fnameNoExt, vbCritical, "Error"
        Exit Function
    End If
    
    tfn = FreeFile
    Open tname For Input As #tfn
    'skip first 3 lines
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    '4th is the one we want
    Line Input #tfn, tstr
    tstr = Right(tstr, Len(tstr) - Len("frequency="))
    Close #tfn
    
    wavGetFrequency = CLng(Val(tstr))
End Function

Public Function wavGetNumChannels(cofile As String) As Integer
    Dim tname As String, tline As String, tstr As String, tfn As Integer
    
    ' strip out the extension from this cofile name and get the log file form it...hope it exists!!!!
    breakfilename (cofile)
    tname = bfn.fullpath & bfn.fnameNoExt & ".log"
    If (Dir(tname, vbNormal) = "") Then
        MsgBox "Couldn't find the log file for sound file: " & bfn.fnameNoExt, vbCritical, "Error"
        Exit Function
    End If
    
    tfn = FreeFile
    Open tname For Input As #tfn
    'skip first line
    Line Input #tfn, tstr
    '2nd is the one we want
    Line Input #tfn, tstr
    tstr = Right(tstr, Len(tstr) - Len("numchannels="))
    Close #tfn
    
    wavGetNumChannels = CInt(Val(tstr))
End Function

Public Function wavGetCodecBitrate(cofile As String) As Integer
    Dim tname As String, tline As String, tstr As String, tfn As Integer
    
    ' strip out the extension from this cofile name and get the log file form it...hope it exists!!!!
    breakfilename (cofile)
    tname = bfn.fullpath & bfn.fnameNoExt & ".log"
    If (Dir(tname, vbNormal) = "") Then
        MsgBox "Couldn't find the log file for sound file: " & bfn.fnameNoExt, vbCritical, "Error"
        Exit Function
    End If
    
    tfn = FreeFile
    Open tname For Input As #tfn
    'skip 2 lines
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    '3rd is the one we want
    Line Input #tfn, tstr
    tstr = Right(tstr, Len(tstr) - Len("codecbitrate="))
    Close #tfn
    
    wavGetCodecBitrate = CInt(Val(tstr))
End Function


Public Function wavGetPlaybackLength(cofile As String) As Long
    Dim tname As String, tline As String, tstr As String, tfn As Integer
    
    ' strip out the extension from this cofile name and get the log file form it...hope it exists!!!!
    breakfilename (cofile)
    tname = bfn.fullpath & "\" & bfn.fnameNoExt & ".log"
    If (Dir(tname, vbNormal) = "") Then
        MsgBox "Couldn't find the log file for sound file: " & bfn.fnameNoExt, vbCritical, "Error"
        Exit Function
    End If
    
    tfn = FreeFile
    Open tname For Input As #tfn
    'skip 5 lines
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    '6th is the one we want
    Line Input #tfn, tstr
    tstr = Right(tstr, Len(tstr) - Len("playbacklength="))
    Close #tfn
    
    wavGetPlaybackLength = CLng(Val(tstr))
End Function

Public Function wavGetNumBits(cofile As String) As Long
    Dim tname As String, tline As String, tstr As String, tfn As Integer
    
    ' strip out the extension from this cofile name and get the log file form it...hope it exists!!!!
    breakfilename (cofile)
    tname = bfn.fullpath & bfn.fnameNoExt & ".log"
    If (Dir(tname, vbNormal) = "") Then
        MsgBox "Couldn't find the log file for sound file: " & bfn.fnameNoExt, vbCritical, "Error"
        Exit Function
    End If
    
    tfn = FreeFile
    Open tname For Input As #tfn
    'skip first 4 lines
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    Line Input #tfn, tstr
    '5th is the one we want
    Line Input #tfn, tstr
    tstr = Right(tstr, Len(tstr) - Len("bits="))
    Close #tfn
    
    wavGetNumBits = CLng(Val(tstr))
End Function


