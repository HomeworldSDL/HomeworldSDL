Attribute VB_Name = "aifrDef"
Option Explicit

Type aifrHeader
    'form section
    formStr As String * 4           '  "FORM"
    ckDataSize(0 To 3) As Byte      '  current chunk size plus header
    aifcstr As String * 4           '  "AIFC" = aiff with compression
    
    'version chunk
    fverStr As String * 4           ' "FVER"
    vckDataSize(0 To 3) As Byte     ' 4
    vtimestamp As Long              ' timestamp created
    
    'common chunk
    commStr As String * 4           ' "COMM"
    cckDataSize(0 To 3) As Byte     ' 39    0x27
    numChannels(0 To 1) As Byte     ' 2=stereo, 1=mono
    numSamples(0 To 3) As Byte      ' number of samples = (chunkSize / numChannels / (codecbitrate)) / (8 bits/byte)
    sampleSize(0 To 1) As Byte      ' 8 bit sound, 16 bit sound
    sRateSpecial(0 To 3) As Byte    ' this is actually an encoded, extended ieee long (10bytes) - good for storing floating point numbers
                                    '   for ease of understanding it will be the following (Hz):
                                    '   11025 = 40 0C AC 44,  22050 = 40 0D AC 44,  44100 = 40 0E AC 44
    filler1(0 To 5) As Byte              ' all zeros
    compType As String * 4          ' "COMP"
    cNameLength As Byte             ' 16 characters follow
    compName As String * 16         ' "Relic Codec v1.6"
    cnameNull As Byte               ' null string delimiter  = 0
    
    'sound chunk
    ssndStr As String * 4           ' "SSND"
    sckDataSize(0 To 3) As Byte     ' filesize - 80 (written in swapped order -- normal viewing in hex editor)
    filler2 As Long                 ' 0
    filler3 As Long                 ' 0
    codecbitrate(0 To 1) As Byte    ' written in swapped order
    
    'raw sound data follows this
    
End Type

Public Function reverseBytesToLong(b() As Byte) As Long
    Dim tLong As Long
    
    tLong = (b(0) * (2 ^ 24)) + (b(1) * (2 ^ 16)) + (b(2) * (2 ^ 8)) + b(3)
    reverseBytesToLong = tLong
End Function

Public Function reverseBytesToInt(b() As Byte) As Integer
    Dim tInt As Integer
    
    tInt = (b(0) * (2 ^ 8)) + b(1)
    reverseBytesToInt = tInt
End Function

Public Function retrieveSoundTimeLength(sampRate As Long, numSamps As Long, numChans As Integer, numBytesPerSamp As Integer) As Long
    Dim tmpval As Integer
    
    
    ' this is only approximate since the sample rate may vary slightly
    tmpval = sampRate / (numSamps * numChans * numBytesPerSamp)
    
    If (tmpval > 256) Then
        tmpval = 256                ' are we really going to have 4 minute subtitles?
    End If
    
    retrieveSoundTimeLength = tmpval
End Function

Public Function getSampleRate(sr() As Byte) As Long
    Dim tval As Long
    
    Select Case sr(1)
        Case &HC
            tval = 11025
        Case &HD
            tval = 22050
        Case &HE
            tval = 44100
    End Select
    getSampleRate = tval
End Function
