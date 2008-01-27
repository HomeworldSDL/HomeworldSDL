Attribute VB_Name = "modVCEDef"
Option Explicit

'I hope this is right...

Type VCE_Header
    signature As String * 4         'VCE0   56 43 45 30
    checksum As Long                'hope it's not heeded...
End Type

'This is old, outdated format
'Type VCE_SoundEntry_No_Text
'    InfoString(0 To 3) As Byte      'INFO   49 4E 46 4F
'    Blanksection As Long            'always 00 00 00 00
'    DataString(0 To 3) As Byte      'DATA   44 41 54 41
'    soundSize As Long               'how many bytes after DATA until the next SoundEntry
'
    'after this is the sound data...
'
'End Type

Type VCE_SoundEntry_header
    InfoString As String * 4        'INFO  49 4E 46 4F
    textLength As Long              'length of the following text subtitling
End Type

' between these two would be the subtitle text, unless textLength is 0

Type VCE_SoundEntry
    DataString As String * 4        'DATA 44 41 54 41
    soundSize As Long               'how many bytes after this field until next VCE_SoundEntry_header
    'raw sound data after
End Type

Type phraseInfo
    offset As Long
    description As String
    hasSubtitling As Boolean
End Type
    

Global totalPhrases As Long

