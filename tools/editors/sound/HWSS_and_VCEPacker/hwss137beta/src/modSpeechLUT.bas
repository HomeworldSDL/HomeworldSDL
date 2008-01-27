Attribute VB_Name = "modSpeechLUT"
Option Explicit
'-=============-
' This is out of date... use lutdefs.bas instead


'-------------------------------
'In the speechphrase_comp.lut
'-------------------------------
'format of the LUT file
Type SpPhrFileHeader
    lutVersion As String * 4        'LUT2    4C 55 54 32
    checksum As Long                    'just a guess... matches 2nd 4 bytes in speechsentence_comp.lut
    unknown1 As Integer                 'not sure yet
    noRecords As Integer                'should be 11292 sentences
    
End Type

'I think they are allowing up to 7 phrases to be combined to make a sentence...
'...this is wacky
Type SpPhrRecordReal
    unValue As Long                     'no idea...
    unId As Long                        'an unknown identifier
    numRecs As Long                     'how many voice offsets are held (total of 7?)
    offset(0 To 6) As Long              '7 Longs = 7 voice offsets for this record
End Type

Type SpPhrRecord
    unValue As Long                     'no idea...
    unId As Long                        'an unknown identifier
    numRecs As Long                     'how many voice offsets are held (total of 7?)
    offset(0 To 6) As Long              '7 Longs = 7 voice offsets for this record
    changed As Boolean                  'keep track
End Type

Type phrEntry
    offset As Long
    desc As String
End Type

Global unValueHi As Long, unValueLo As Long
Global phrEntries() As phrEntry
Global theRecs() As SpPhrRecord
Global bChangeSentence As Boolean, storePhraseSelection As Long
Global newPhrases4sentence(0 To 6) As String, newPhraseCount As Integer

'-------------------------------
'In the speechsentence_comp.lut
'-------------------------------
Type SpSentenceLutHdr
    lutStr As String * 4                'LUT    4c 55 54 31
    matchNum As Long                    'matches 2nd 4 bytes in speechphrase.lut
    unk1 As Integer
    numEntries As Integer
    unk2 As Long
    unk3 As Long
    unk4 As Integer                     'skip first 5 longs
End Type

Type SpSentenceLutEntry
    vals(0 To 7) As Long
End Type
