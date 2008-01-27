Attribute VB_Name = "lutdefs"
Option Explicit

' forget that modSpeechLut file, this is the real deal!

Type phraselutFile
    fileID As String * 4                    ' "LUT2"
    checksum As Long                        ' yeah, whatever
    numlongsPerRecord As Integer            ' 10 for phrases
    numsentences As Integer                 ' 11292 for commercial homeworld
End Type

Type phraselutRecord
    duration As Long                        ' duration (probably milliseconds) - used to time display of subtitles?
    probability As Long                     ' what is the weighting for how often this variation should be played? (this is a percentage... i.e. 33 = 1/3 of the time)
    numoffsets As Long                      ' how many phrases make up this sentence? - we force to 1 for now
    offsets(0 To 6) As Long                 ' indexes into the voice (VCE file) as addresses - homeworld can be up to 7
End Type

Type phraselutRecordCataclysm
    duration As Long                        ' duration (probably milliseconds) - used to time display of subtitles?
    probability As Long                     ' what is the weighting for how often this variation should be played? (this is a percentage... i.e. 33 = 1/3 of the time)
    numoffsets As Long                      ' how many phrases make up this sentence? - we force to 1 for now
    offsets(0 To 1) As Long                 ' indexes into the voice (VCE file) as addresses - Cataclysm uses only a max of 2
End Type

Type speechlutHeader
    fileID As String * 4                    ' the signature "LUT1"
    checksum As Long                        ' who cares?
    numlongsPerRecord As Integer            ' how many long words per record --- 4
    numevents As Integer                    ' number of events for the speech --- 821 for Homeworld, 863 for both Beast and Somtaaw for Cataclysm
    numactors As Integer                    ' number of actors = 4 - only 1 for Cataclysm.. use first compbitrate entry for Cataclysm
    compbitrate(0 To 3) As Integer          ' compression bitrate for each actor (768, 384, 384, 384) - cataclysm will only use the first, and that will be 512
End Type

Type speechlutRecord                        ' 8 bytes for each record - 2 bytes each field
    priority As Integer                     ' 0 - 3
    numvariations As Integer                ' how many different ways there are to say this event
    maxvariable As Integer                  ' what is the range for the variable in this sentence
    index As Integer                        ' into the phrase table
End Type
    
' -------- gunevents.lut file is from inspection and help from Rupract of Relic

Type guneventslutFileHdr                    ' ---- common for SFX LUT files (shipevents.lut, uievents.lut, etc)
    signature As String * 4                 ' PKG0
    checksum As Long                        ' does anyone ever check these?
    maxvariations As Integer                ' set to 4 at the moment
    numevents As Integer                    ' set to 3
    numObjects As Integer                   ' 18 total (0-17) for commercial homeworld
                                            ' 14 bytes to this point
End Type

Type gunEvnt5x3table                        ' there will be guneventslutFileHdr.maxvariations + 1 of these per 30 byte record!
    varNum As Integer                       ' which variation is this
    offset(0 To 3) As Integer               ' offsets into guns.bnk - random, according to RupRact
End Type

Type gunEventsLUTFileRecord
    lookupTable(0 To 2) As gunEvnt5x3table  ' set to guneventslutFileHdr.numevents!
                                            ' 30 bytes per record (per numObjects)
End Type
