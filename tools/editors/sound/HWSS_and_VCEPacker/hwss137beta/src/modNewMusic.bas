Attribute VB_Name = "modNewMusic"
Option Explicit

Dim newMusicNumTracks As Integer, MtrFN As Integer, MsngFN As Integer
Dim newMusicTrackLoc As Long, newMusicSongLoc As Long, newMusicBlankLoc As Long
Dim newMusicCurBR As Integer, newMusicFileList() As Boolean

'support for the new tab in music section: New Songs/Tracks
' - This way, we can build a completely new wxh and wxd without the complicated
' - mess of concatenation and such...

Public Function checkForBlankAifrx() As Boolean
    checkForBlankAifrx = False
    
    If (Dir(App.Path & "\blank.aifrx", vbNormal) <> "") Then
        checkForBlankAifrx = True
    End If
End Function

Public Function addNextSongToWXD(cnt As Integer) As Boolean
    Dim tfname As String, tfn As Integer, ibyte As Byte, i As Long, tnum As String
    Dim along As Long, blongsonly As Boolean, newfileLen As Long
    
    tnum = Trim(Str(cnt))                       ' hmm... maybe I should pull this out on its own
    If (Len(tnum) < 2) Then
        tnum = "0" & tnum
    End If
    tfname = frmMain.dirNewSongTrackDir.Path & "\m" & tnum & ".aifrx"
    
    tfn = FreeFile
    On Error GoTo addnewmusicNextSongerr
    Open tfname For Binary Access Read As #tfn
    On Error GoTo 0
    
    newfileLen = FileLen(tfname)
    frmMain.progNewSongTrack.Min = 1
    frmMain.progNewSongTrack.Max = newfileLen / 4
    
    
    If (newfileLen Mod 4) = 0 Then
        blongsonly = True
    Else
        blongsonly = False
    End If
    
    For i = 1 To (newfileLen / 4)
        Get #tfn, , along
        Put #MsngFN, , along
        DoEvents
        frmMain.progNewSongTrack.Value = i
    Next i
    If (blongsonly = False) Then
        For i = 1 To (newfileLen Mod 4)
            Get #tfn, , ibyte
            Put #MsngFN, , ibyte
            frmMain.progNewSongTrack.Value = frmMain.progNewSongTrack.Max
        Next i
    End If
    
    ' byte-by-byte... so we can see the nice progress bar...
'    For i = 1 To FileLen(tfname)
'        Get #tfn, , ibyte
'        Put #MsngFN, , ibyte
'        DoEvents
'        frmMain.progNewSongTrack.Value = i
'    Next i
    
    newMusicSongLoc = Seek(MsngFN) - 1
    addNextSongToWXD = True
    
    Close #tfn
    Exit Function
    
addnewmusicNextSongerr:
    MsgBox "Error opening the song file: tfname", vbCritical, "Error"
    addNextSongToWXD = False
End Function

Public Sub addAssociatedTrackToWXH(cnt As Integer)
    Dim trEntryHdr As WXH_track
    
    'build the track entry
    trEntryHdr.bitRate = newMusicCurBR
    trEntryHdr.fill = 0
    trEntryHdr.signature = &HFFFF02FF       ' what does this mean?
    trEntryHdr.skirmishflag = frmMain.chkNewSongTracksAllSkirmish.Value ' globally setting skirmish flag when we build a wxd/wxh combo from scratch
    trEntryHdr.SongOffset = newMusicSongLoc
    frmMain.lblNewSongTracksOffset.Caption = Str(newMusicSongLoc)
    
    'put it in the wxh file
    Put #MtrFN, , trEntryHdr
End Sub

Public Function addFirstBlankSongToWXD() As Boolean
    Dim theFile As String, ibyte As Byte, i As Long, tfn As Integer
    
    theFile = App.Path & "\blank.aifrx"
    On Error GoTo newmusicaddblankerr
    tfn = FreeFile
    Open theFile For Binary Access Read As #tfn
    On Error GoTo 0
    
    LogBoxSet frmMain.lstNewSongTracksResults
    
    frmMain.progNewSongTrack.Min = 1
    frmMain.progNewSongTrack.Max = FileLen(theFile)
    logMsg "Adding initial blank entry"
    
    newMusicBlankLoc = Seek(MsngFN) - 1             ' for blank tracks to point to
    
    'put the blank file into the new wxd file
    For i = 1 To FileLen(theFile)
        Get #tfn, , ibyte
        Put #MsngFN, , ibyte
        DoEvents
        frmMain.progNewSongTrack.Value = i
    Next i
    
    newMusicSongLoc = Seek(MsngFN) - 1                ' save the new location
    
    addFirstBlankSongToWXD = True
    Close #tfn
    Exit Function
    
newmusicaddblankerr:
    addFirstBlankSongToWXD = False
    MsgBox "Couldn't add blank.aifrx to the new hw_music.wxd file! Error=[" & Err.description & "] - Aborting", vbCritical, "Error"
    Close #tfn
End Function

Public Sub addBlankEntryToWXH(tracknum As Integer)
    Dim trEntryHdr As WXH_track
    
    'build the track entry
    trEntryHdr.bitRate = 256                ' the blank music song is encoded at 256
    trEntryHdr.fill = 0
    trEntryHdr.signature = &HFFFF02FF
    trEntryHdr.skirmishflag = frmMain.chkNewSongTracksAllSkirmish.Value
    trEntryHdr.SongOffset = newMusicBlankLoc
    frmMain.lblNewSongTracksOffset.Caption = Str(newMusicBlankLoc)

    'write it out
    Put #MtrFN, , trEntryHdr
End Sub

Public Function getCodecBitrateForSong(songNum As Integer) As Boolean
    Dim tfn As Integer, tfname As String, tStr As String, tnum As String
    
    tnum = Trim(Str(songNum))               ' looks funny, but str() adds a leading space
    If (Len(tnum) < 2) Then                 ' put it in 00 format... too lazy to use format() function I guess
        tnum = "0" & tnum
    End If
    
    tfname = frmMain.dirNewSongTrackDir.Path & "\m" & tnum & ".log"
    tfn = FreeFile
    
    On Error GoTo newmusicGetBitrateErr
    Open tfname For Input As #tfn
    On Error GoTo 0
    
    'skip the first two entries
    Line Input #tfn, tStr
    Line Input #tfn, tStr
    
    'now, get the codec line
    Line Input #tfn, tStr
    tStr = Right(tStr, Len(tStr) - Len("codecbitrate="))
    
    Close #tfn                                      ' close the file
    
    newMusicCurBR = Val(tStr)                       ' get the bitrate value
    getCodecBitrateForSong = True                   ' we were successful
    Exit Function
    
newmusicGetBitrateErr:
    MsgBox "Error retrieving bitrate from the log file for this aifrx!!!!!", vbCritical, "Error"
    getCodecBitrateForSong = False
End Function

Public Sub buildNewMusicFiles()
    Dim i As Integer, tStr As String, tnum As Integer, nextnum As Integer, tsch As String
    Dim numOverallSegments As Integer
    
    newMusicNumTracks = 48                  ' I tried adding more tracks and songs, but HW didn't allow them to play
    If (newMusicOpenFiles() = False) Then   ' open the new files
        Exit Sub
    End If
    
    LogBoxSet frmMain.lstNewSongTracksResults
    
    logMsg "building headers for wxh/wxd files"
    newMusicMakeHeaders                     ' add file headers
    
    If (addFirstBlankSongToWXD() = False) Then
        Exit Sub
    End If
    logMsg "adding blank sound"

    nextnum = 0
    ReDim newMusicFileList(newMusicNumTracks - 1)   ' count from 0, remember?
    
    'clear the array
    For i = 0 To newMusicNumTracks - 1
        newMusicFileList(i) = False
    Next i
    
    numOverallSegments = 0
    frmMain.progNewMusicOverall.Value = 1
    
    'flag those we do have
    For i = 0 To frmMain.fileNewSongTrack.ListCount - 1     ' files had better be m##.aifrx format
        tStr = frmMain.fileNewSongTrack.List(i)
        tsch = Mid(tStr, 2, 2)                              ' get the numbers after the letter "m", example m02.aifrx = new song #02
        tnum = Val(tsch)                                    ' change from string to a number
        If (tnum < 0) Or (tnum > newMusicNumTracks) Then    ' in bounds?
            'skip it                                          wow, I must have been up late doing this....
        Else
            newMusicFileList(tnum) = True                   ' flag the one we just found
            numOverallSegments = numOverallSegments + 1
        End If
    Next i
    
    LogBoxSet frmMain.lstNewSongTracksResults
    
    ' visual stuff
    frmMain.progNewMusicOverall.Min = 1
    frmMain.progNewMusicOverall.Max = numOverallSegments + 1
    frmMain.lblNewSongTracksNumBlanks.Caption = "0"
    frmMain.lblNewSongTracksNumSongs.Caption = "0"
    frmMain.lblNewSongTracksSongNum.Caption = "0"
    frmMain.lblNewSongTracksOffset.Caption = "0"
    logMsg "Adding songs"
    
    'now, do the building
    For i = 0 To newMusicNumTracks - 1
        frmMain.lblNewSongTracksSongNum.Caption = Str(Val(frmMain.lblNewSongTracksSongNum.Caption) + 1)         ' was I drunk when I coded this?
        If (newMusicFileList(i) = True) Then            ' does it exist in our list?
            If (getCodecBitrateForSong(i) <> False) Then                     ' if so, do your thing...
                addAssociatedTrackToWXH i               ' we add the wxh entry first because of the way we calculate newMusicSongLoc
                addNextSongToWXD i
                frmMain.lblNewSongTracksNumSongs.Caption = Str(Val(frmMain.lblNewSongTracksNumSongs.Caption) + 1)
                frmMain.progNewMusicOverall.Value = frmMain.progNewMusicOverall.Value + 1
            Else
                MsgBox "Couldn't get the codec bitrate from the log file!...stopping", vbCritical, "Error"
                Exit Sub
            End If
        Else
            addBlankEntryToWXH i                        ' just set this track to point to the blank sound... no music for this track until it is changed
            frmMain.lblNewSongTracksNumBlanks.Caption = Str(Val(frmMain.lblNewSongTracksNumBlanks.Caption) + 1)
        End If
'        frmMain.progNewMusicOverall.Value = i + 1
    Next i
    
    newMusicCloseNewFiles                   'close the files
End Sub

Public Function newMusicOpenFiles() As Boolean
    Dim thedir As String
    
    ' make sure we have the subdir first...make them do it!
    thedir = frmMain.txtNewMusicGameDir.Text
    If (Dir(thedir & "soundfx", vbDirectory) = "") Then
        MsgBox "You need to have a directory named soundfx underneath your main game directory for this to work!!!", vbCritical, "Error"
        newMusicOpenFiles = False
        Exit Function
    End If
    
    LogBoxSet frmMain.lstNewSongTracksResults
    
    ' backup old files first! Start with wxh backup
    If (Dir(thedir & "soundfx\hw_music.wxh", vbNormal) <> "") Then
        If (Dir(thedir & "soundfx\hw_music.hbk", vbNormal) <> "") Then
            logMsg "deleting backup wxh file"
            Kill (thedir & "soundfx\hw_music.hbk")
        End If
        Name thedir & "soundfx\hw_music.wxh" As thedir & "soundfx\hw_music.hbk"
    End If
    
    'Delete old wxd backup
    If (Dir(thedir & "hw_music.bak", vbNormal) <> "") Then
        logMsg "deleting backup wxd file!"
        Kill (thedir & "hw_music.bak")
    End If
    Name thedir & "hw_music.wxd" As thedir & "hw_music.bak"
    
    'open tracks (wxh) file and songs (wxd) file - set the global file handle numbers
    MtrFN = FreeFile
    On Error GoTo newmusicfileerr
    Open thedir & "soundfx\hw_music.wxh" For Binary Access Write As #MtrFN
    MsngFN = FreeFile
    Open thedir & "hw_music.wxd" For Binary Access Write As #MsngFN
    On Error GoTo 0
    
    newMusicOpenFiles = True
    Exit Function
    
newmusicfileerr:
    newMusicOpenFiles = False
    MsgBox "Error opening new wxh and/or wxd files for New Music!", vbCritical, "Error"
End Function

Public Sub newMusicCloseNewFiles()
    Close #MtrFN
    Close #MsngFN
End Sub

Public Sub newMusicMakeHeaders()
    Dim trhdr As WXH_header, snghdr As WXD_header
    
    trhdr.hString = "WXH1"
    trhdr.hchecksum = &HC23D4E4E                'same as original
    trhdr.numTracks = newMusicNumTracks         'exe limits to 48
    
    Put #MtrFN, , trhdr
    
    snghdr.hString = "WXD1"
    snghdr.hchecksum = &HC23D4E4E               ' strangely, same as wxh
    
    Put #MsngFN, , snghdr
End Sub

Public Function getNumSongsFromWXD(tpath As String)
    Dim tfn As Integer, thdr As WXH_header
    
    'sanity check
    If (Dir(tpath, vbNormal) = "") Then
        MsgBox "Couldn't find the WXD/WXH files at the specified location!", vbExclamation, "Error"
        getNumSongsFromWXD = 0
    End If
    
    tfn = FreeFile
    Open tpath For Binary Access Read As #tfn
    
    'get the header
    Get #tfn, , thdr
    
    Close #tfn
    
    getNumSongsFromWXD = thdr.numTracks

End Function

Public Sub WXDExtractSongs(tpath As String)
    Dim tfn As Integer, tlong As Long, tbyte As Byte, twxhdr As WXH_header, twxdhdr As WXD_header
    Dim twxhentry As WXH_track, twxdsong As wxd_song, bextra As Boolean
    Dim tout As Integer, tStr As String
    
    Load frmNotice
    frmNotice.Visible = False
    frmNotice.lblNote.Caption = "WXH and WXD must be in the same, working directory for song extraction!"
    frmNotice.tmrflash.Enabled = True
    frmNotice.Visible = True

End Sub
