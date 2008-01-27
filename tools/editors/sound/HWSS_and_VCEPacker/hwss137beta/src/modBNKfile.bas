Attribute VB_Name = "modBNKfile"
Option Explicit


'----------------------------------------------
' new definition of the bnk format from Rupract of Relic
'----------------------------------------------
'unsigned long id; // defined as 'P' 'C' 'H' '#' where # is the version
'unsigned long priority; // can't remeber if higher value is higher priority or lower value is higher priority
'long pitch; // can't remeber what the range of values for pitch was, but play around with it to get some interesting effects
'long offset;
'long size;
'long loopstart; // position at which loop point starts
'long loopend; // position at which loop point ends
'short int bitrate;
'short int flags;
'unsigned char volume; // 0 - 255
'char pan; // -127 to 127 (0 being center, -63 left, 63 right, -127 or 127 behind)
'short int pad; // to quad align the structure
'WAVEFORMATEX waveformat; // this is defined in MMSYSTEM.H and can be found in the DirectX help file
'short int pad; // to quad align the structure

Public Type WAVEFORMATEX
    wFormatTag As Integer                   ' PCM only (0x0001)
    nChannels As Integer                    ' 0x01 = mono, 0x02 = stereo
    nSamplesPerSec As Long                  ' hertz 44100, 22050, 11025
    nAvgBytesPerSec As Long                 ' nSamplesPerSec * nChannels
    nBlockAlign As Integer                  ' Block alignment, in bytes
    wBitsPerSample As Integer               ' 8 bit or 16 bit
    cbSize As Integer                       ' extra info
End Type

Public Type bnkLut2
    Info As String * 4                      ' PCH0
    priority As Long                        ' priority
    pitch As Long                           ' pitch
    offset As Long                          ' into the base of the BNK
    soundSize As Long                       ' raw sound size
    loopstart As Long                       ' position at which loop point starts (milliseconds?)
    loopend As Long                         ' position at which loop point ends (milliseconds?)
    codecbitrate As Integer                 ' block bitrate used during encoding
    flags As Integer                        ' just guessing, but a 1 here means a looping sound during pause (only Ion Cannon)
    volume As Byte                          ' volume   0-255
    pan As Byte                             ' panning the sound
    pad1 As Integer                         ' filler - 36 bytes to this point
    waveFormat As WAVEFORMATEX              ' 18 bytes here
    pad2 As Integer                         ' 56 bytes total?
End Type


'--------------
'This section blatantly copied from Delphy's BNK tool (hope he doesn't mind)
'--------------

'                       this section here looks like the VCE format...why did I waste
'                       my time decoding it when he had already done it here?!?!
'char info[4]           // 'INFO'
'ulong_t char_count     // length of stringchar
'string[char_count]     // string (no null) for the captioning
'char data[4]           // 'DATA'
'ulong_t sound_size     // size of sound in bytes
'char sound[sound_size] // sound data

'ID as String * 4       'BNK0
'checksum as long       ' --- is this always ignored?
'#patches as long

Public Type bnkLUT
    ' Lookup table at top of bnk file
    Info As String * 4   ' PCH0
    filler As String * 8 ' 2 x Longs
    offset As Long
    soundSize As Long
    unknown3 As Long
    Unknown4 As Long
    codecbitrate As Integer
    Unknown6 As Long
    filler2 As Integer
    Unknown8 As Integer
    Unknown9 As Integer
    bitRate As Long     '---- this is the playback frequency...
    Unknown10 As Long
    Unknown11 As Long
    Unknown12 As Integer
    Filler3 As Integer
End Type

Public Data() As Byte
Public blah As String
Global Const numChannels As Integer = 1


'--------------
' end of blatant copying
'--------------

'----- Now, my stuff
Type bnkHeader
    bnkString As String * 4         'BNK0
    checksum As Long                '4 bytes of checksum data
    noPatches As Long               'how many sound effects are in this BNK file
End Type

Type BnkLst
    theSize As Long
    theBitrate As Integer
    offset As Long
    bReplace As Boolean
    thePath As String
    position As Integer
    frequency As Long
    numchans As Integer
    numbits As Integer
    vol As Byte
    pan As Byte
    loop As Boolean
    loops As Long
    loope As Long
End Type

Global totalBNKEntries As Integer
Global theBnkContents() As BnkLst, OrigBnkContents() As BnkLst
Global bnkFileSize As Long, origFileSize As Long
Global theBnkHeader As bnkHeader

Public Function openSoundNamesFile() As Integer
    Dim tfn As Integer, tfname As String
    
    tfn = FreeFile
    tfname = App.Path & "\sounds.dat"
    On Error GoTo bigerr
    Open tfname For Input As #tfn
    On Error GoTo 0
    
    openSoundNamesFile = tfn
    Exit Function
    
bigerr:
    MsgBox "Couldn't open soundnamesfile!", vbCritical, "Error"
    openSoundNamesFile = -1
End Function

Public Sub closeSoundNamesFile(tfn As Integer)
    Close #tfn
End Sub

Public Function getSoundName(tfn As Integer, bnk As String, which As Integer) As String
    Dim foundb As Boolean, tmpstr As String, tstr2 As String
    
    foundb = False
    
    'find the bnk group
    Seek #tfn, 1
    Do
        Line Input #tfn, tmpstr
        tmpstr = Trim(tmpstr)
        If (Left(tmpstr, 1) = "[") Then
            tstr2 = Mid(tmpstr, 2, InStr(2, tmpstr, ".") - 2)
            If (UCase(tstr2) = UCase(bnk)) Then
                foundb = True
                Exit Do
            End If
        End If
    Loop Until EOF(tfn)
    
    getSoundName = "-"
    If (foundb = False) Then
        Exit Function
    Else
        Do
            Line Input #tfn, tmpstr
            tstr2 = Left(tmpstr, InStr(1, tmpstr, "-") - 1)
            If (Val(tstr2) = which) Then
                getSoundName = Trim(Right(tmpstr, Len(tmpstr) - Len(tstr2)))
                Exit Function
            End If
        Loop Until EOF(tfn)
    End If
    
End Function

Public Function getNumSoundsFromBNK(thePath As String)
    Dim tfn As Integer, tHdr As bnkHeader
    
    'sanity check
    If (Dir(thePath, vbNormal) = "") Then
        MsgBox "Couldn't find the BNK file!", vbCritical, "Error"
        getNumSoundsFromBNK = 0
        Exit Function
    End If
    
    'open the file
    tfn = FreeFile
    Open thePath For Binary Access Read As #tfn
    
    'grab the header
    Get #tfn, , tHdr
    
    'close the file
    Close #tfn
    
    'send back the number of sounds
    getNumSoundsFromBNK = tHdr.noPatches
    
End Function
