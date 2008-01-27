Attribute VB_Name = "modWXHfile"
Option Explicit

' This stuff came from Delphy's tools.  I don't have the source to his WXHEditor, so I had to
' determine how it worked and extrapolate the file formats from that.  This might not be entirely
' correct, but enough so to work

Type WXH_header
    'general header
    hString As String * 4       'WXH1       57 58 48 31
    hchecksum As Long           'skip it
    numTracks As Long           ' how many tracks are available?
End Type

Type WXH_track
    'each track
    SongOffset As Long          'offset in wxd for the matching song to this track
    skirmishflag As Byte        '1 = yes, 0 = no  -  track is available in skirmish mode
    fill As Byte                'filler (I guess, it's always 0 for both bytes
    bitRate As Integer          '2 bytes, swapped for intel, showing encoding bitrate
    signature As Long           'not sure, always FFFF02FF
End Type


Type WXD_header
    hString As String * 4       'WXD1            57 58 44 31
    hchecksum As Long           'skip it
End Type

Type wxd_song
    dData(0 To 3) As Byte       'DATA string     44 41 54 41
    songLen As Long             'length of song
    firstbyte As Byte           'first byte of the song (just ignore it)
End Type

Global theWXHheader As WXH_header
Global theMusicTracks() As WXH_track
Global theWXDheader As WXD_header
Global theMusicSongs() As wxd_song
Global numTracks As Integer
Global numSongs As Integer
Global songFileOffsets() As Long
Global theBitrates() As Byte
Global skirmishArray() As Byte
Global theSongsExactLocation() As Long
Global attachedSong() As Integer

