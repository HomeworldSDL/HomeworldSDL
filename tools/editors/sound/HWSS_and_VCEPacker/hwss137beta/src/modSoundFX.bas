Attribute VB_Name = "modSoundFX"
Option Explicit

' ------------ LUT files in the SoundFX directory of Homeworld:
' speechsentence_comp.lut = bitrates for the 4 'actors' and their tables of events->sentence relationship - speech section
' speechphrase_comp.lut = for each sentence, show which phrases in the VCE files make it up + the time to display the subtitles
' derelictevents.lut - plays sounds for derelicts
' frequency.lut - 8 band EQ to control the output of sounds over distance
' volume.lut - distance to volume ratio control
' range.lut - not known yet
' spechitevents.lut - not known yet (ship hit events?)
' shipcmnevents.lut - not known yet (common ship events?)
' gunevents.lut - gun shots to guns.bnk relationship
' shipevents.lut - ship sounds (engines, burning, movements, beeps, etc) to ships.bnk relationship
' specexpevents.lut - explosion sounds events to specialeffects.bnk relationship
' uievents.lut - relationship between Homeworld GUI events and ui.bnk file


' --- Notes from RupRact of Relic from the forums.relicnews.com message board
'Ok Mambo,
'
'<< Then, it looks like two integer values 4 and 3, then 0x12 which is 18, which matches
' what Rup said earlier about there being sound events 0-17. >>
'You are correct about the header. The terminology I used is "max variations" (4),
'"events" (3) and "objects" (18). This is in general for all the sfx lut files.
'
'As far as looking in your hex editor, you're on the right track... after the 14 byte
'header, look at the next 30 bytes as a 5x3 table (5 columns, 3 rows). The first column's
'value is the actual number of variations (max of 4 as noted above thats why the table is
'5x3 not 4x3). The next 4 integers should be offsets into the bnk file (or 0 or 0xFF for
'no lookup, can't remember which).
'
'Hope This 'll get you the rest of the way with the event lut files.
'
'<< - volume.lut has lookups based on distances from a sound generating event that
'determine how loud a sound is played at that distance? >>
'Yes that is correct. This allows the sound designer to create custom volume curves for
'many different sound types in the game. I think the engine sounds are grouped by class
'(ie fighter, corvette, frigate, etc) but there are a bunch of special case volume curves
'as well. I'd have to go searching through header files to find them all.
'
'<< - frequency.lut is similar but controls doppler shift effects? >>
'Not quite. This table doesn't affect doppler, it is actually an 8 band EQ. This allows
'the sound designer to change the EQ over distance. A really good example of this is for
'explosions. At far distances all of the high-end is eq'ed out leaving just the bass to
'play through. As you get closer the high end gets faded back in until you hear the full
'frequency of the sound. This is one of the greatest parts of the HW sound engine. It
'gives a similar effect to what EAX and DSound3D do, but the sound designer has full
'control over the EQ over distance which is much more difficult to support using hardware
'(although EAX3.0 is supposed to improve this).
'
'We have a stand alone tool for creating and editing the Volume and Frequency LUT tables.
'I might be able to get this one released to you guys if you want. Its pretty basic and
'wouldn't be giving away any of our "cool technology".
'
'Laters,
'*rup*
'
'-----------------------------------------------------------------------------

Global tOffsets() As Long                  ' used for saving offsets when adding aifrbs to BNKs
Global addedBnkEntries As Integer          ' number of new entries to a BNK file
Global addedBnkEntriesLength() As Long     ' length of each new aifrb file to add to a bnk file

Public Sub SoundEventsParseLutFile(filename As String)
    Dim thdr As guneventslutFileHdr, tfn As Integer, i As Integer, j As Integer
    
    'sanity check...should do this more often
    If (Dir(filename, vbNormal) = "") Then
        MsgBox "Couldn't find file: " & filename, vbCritical, "Error"
        Exit Sub
    End If
    
    tfn = FreeFile
    Open filename For Binary Access Read As #tfn
    
    'get the header
    Get #tfn, , thdr
    
    'set up the display table
    frmMain.grdLut.Rows = thdr.maxvariations
    frmMain.grdLut.Cols = thdr.numevents
    
    'clear and set up the combo boxes
    frmMain.cmboBnkLutEditSoundNum.Clear
    frmMain.cmboBnkLutEditVarNum.Clear
    
    For j = 1 To thdr.numObjects
        frmMain.cmboBnkLutEditSoundNum.AddItem Str(j - 1)
    Next j
    
    For i = 1 To thdr.maxvariations
        frmMain.cmboBnkLutEditVarNum.AddItem Str(i - 1)
    Next i
    
    'set column and row sizes in the display table
    frmMain.grdLut.Height = thdr.maxvariations * frmMain.grdLut.RowHeight(0) + 4 * twipy
    For i = 0 To thdr.numevents - 1
        frmMain.grdLut.ColWidth(i) = (frmMain.grdLut.Width / thdr.numevents) - 3 * twipx
    Next i
    
    'now, do every row
    For j = 1 To thdr.numObjects
        For i = 1 To thdr.maxvariations
        Next i
    Next j
    
    '------- TODO finish this
    
    Close #tfn
End Sub

Public Sub BuildNewBNKFile()
    Dim tfn As Integer, fname As String, i As Integer, numFiles As Integer
    Dim thdr As bnkHeader, tpatch As bnkLut2
    Dim tfname As String, tOffsets() As Long
    
    fname = frmMain.txtNewBnkFilePath.Text
    If (fname = "") Then
        MsgBox "Choose a filename first", vbInformation, "Error"
        Exit Sub
    End If
    
    LogBoxSet frmMain.lstBnkBuildResults
    
    'make a base file...nothing in it
    If (createEmptyBinaryFile(fname) = False) Then
        MsgBox "Couldn't make new BNK file: [" & fname & "]", vbCritical, "Error"
        Exit Sub
    End If
    
    logMsg "Creating new file"
    
    'number of files to add?
    numFiles = frmMain.fileBnkBuildSource.ListCount
    ReDim tOffsets(numFiles - 1)
    
    'build header section
    thdr.bnkString = "BNK0"
    thdr.checksum = &HFE142200              'just copy the guns.bnk checksum field
    thdr.noPatches = numFiles
    
    'add it to the files
    tfn = FreeFile
    Open fname For Binary Access Write As #tfn
    Put #tfn, , thdr
    
    'default info
    tpatch.Info = "PCH0"
    tpatch.loopstart = 0
    tpatch.loopend = 0
    tpatch.priority = 0
    tpatch.pitch = 0
    tpatch.flags = 0
    tpatch.volume = 128
    tpatch.pan = 0
    tpatch.pad1 = &HFFFF
    tpatch.pad2 = &HFFFF
    tpatch.waveFormat.cbSize = 0
    tpatch.waveFormat.nChannels = 1
    tpatch.waveFormat.nAvgBytesPerSec = 44100
    tpatch.waveFormat.wBitsPerSample = 16
    tpatch.waveFormat.nBlockAlign = 2
    tOffsets(0) = numFiles * 56 + (4 * 3)       'size of overall header
    tpatch.offset = tOffsets(0)
    tpatch.soundSize = 0                        'initially
    
    logMsg "Adding Patch headers"
    
    For i = 0 To numFiles - 1
        tfname = frmMain.dirBnkBuildSource.Path & "\" & frmMain.fileBnkBuildSource.List(i)
        tpatch.codecbitrate = wavGetCodecBitrate(tfname)
        tpatch.waveFormat.nSamplesPerSec = wavGetFrequency(tfname)
        tpatch.waveFormat.nChannels = wavGetNumChannels(tfname)
        tpatch.waveFormat.nAvgBytesPerSec = tpatch.waveFormat.nSamplesPerSec * tpatch.waveFormat.nChannels
        ' they'd better be using 16bit samples!
        
        If (i > 0) Then
            tOffsets(i) = tOffsets(i - 1) + tpatch.soundSize
            tpatch.offset = tOffsets(i)
        End If
        
        tpatch.soundSize = FileLen(tfname)      ' put it here so that we don't interfere with the previous file size
       
        Put #tfn, , tpatch                      ' write the patch header
    Next i
    
    Close #tfn                  '- close it up so we can add the files
    
    logMsg "Adding raw sound data from aifrb files"
    
    'add in the aifrb files
    For i = 0 To numFiles - 1
        tfname = frmMain.dirBnkBuildSource.Path & "\" & frmMain.fileBnkBuildSource.List(i)
        If (appendBinaryFiles(fname, tfname, frmMain.prgBnkBuild) = False) Then
            logMsg "Error adding file: [" & frmMain.fileBnkBuildSource.List(i) & "]"
            Exit Sub
        End If
    Next i
    
    logMsg "finished"

End Sub

Public Sub BuildMatchingLUTFile()
    Dim thdr As guneventslutFileHdr, trec As gunEventsLUTFileRecord, tentry As gunEvnt5x3table
    Dim tstr As String, tfn As Integer, i As Integer, j As Integer, k As Integer
    
    LogBoxSet frmMain.lstBnkBuildResults
    
    logMsg "Making associate LUT file (same name...rename later)"

    tstr = frmMain.txtNewBnkFilePath.Text
    
    breakfilename tstr
    
    tstr = bfn.fullpath & bfn.fnameNoExt & ".lut"
    
    ' delete any old ones
    If (Dir(tstr, vbNormal) <> "") Then
        Kill (tstr)
        logMsg "Removing old file: " & bfn.fnameNoExt & ".lut"
    End If
    
    'new file
    tfn = FreeFile
    Open tstr For Binary Access Write As #tfn
    
    ' make the header
    thdr.signature = "PKG0"
    thdr.checksum = &HFE221400      ' copy the original from gunevents.lut
    thdr.numevents = 3              ' from original
    thdr.maxvariations = 4          ' from original
    thdr.numObjects = frmMain.fileBnkBuildSource.ListCount  ' same as number of aifrb files
    
    'write it
    Put #tfn, , thdr
    
    'make records
    For i = 0 To thdr.numObjects - 1
        
        For j = 0 To thdr.numevents - 1
            
            ' first one is the index of the matching sound... we only do one per event per object at this point
            trec.lookupTable(j).varNum = 1
            trec.lookupTable(j).offset(0) = i
            
            ' fill in the rest with blanks
            For k = 1 To thdr.maxvariations - 1
                trec.lookupTable(j).offset(k) = &HFFFF
            Next k
        Next j
        
        'write it
        Put #tfn, , trec
    Next i
    
    'close it
    Close #tfn
End Sub


Public Sub getSoundAttributesForBnkEdit()
    Dim tstr As String, tstr1 As String, which As Integer
    
    breakfilename (frmMain.txtBNKeditorNewSoundPath.Text)
    
    LogBoxSet frmMain.lstBNKEditorResults
    
    ' does a log file exist for this sound?
    If (Dir(bfn.fullpath & bfn.fnameNoExt & ".log", vbNormal) = "") Then
        logMsg "No log file found for " & bfn.filename & "... fill it in yourself"
        Exit Sub
    End If
    
    tstr1 = bfn.fullpath & bfn.fnameNoExt & ".log"
    
    If (wavGetFrequency(tstr1) = 44100) Then
        frmMain.optBNKEditor44khz.Value = True
    Else
        frmMain.optBNKEditor22khz.Value = True
    End If
    
    If (wavGetNumChannels(tstr1) = 2) Then
        frmMain.optBNKEditorStereo.Value = True
    Else
        frmMain.optBNKEditorMono.Value = True
    End If
    
    frmMain.txtBNKEditorNewBitrate.Text = wavGetCodecBitrate(tstr1)
    frmMain.lblBnkEditorNumBits.Caption = Str(wavGetNumBits(tstr1))
    
    ' esoteric stuff
    which = frmMain.lstbnkeditorcontents.ListIndex
    frmMain.lblBnkEditVol.Caption = Str(theBnkContents(which).vol)
    frmMain.lblBnkEditPanning.Caption = Str(theBnkContents(which).pan)
    If (theBnkContents(which).loop = True) Then
        frmMain.lblBnkEditLooping.Caption = Str(theBnkContents(which).loops) & "-" & Str(theBnkContents(which).loope)
    Else
        frmMain.lblBnkEditLooping.Caption = "N/A"
    End If
    
    'add the count
    addedBnkEntries = addedBnkEntries + 1
    
    
    frmMain.lblBNKEditorNewSoundOffset.Caption = "-" 'Str(theBnkContents(totalBNKEntries - 1).offset + theBnkContents(totalBNKEntries - 1).theSize)
    frmMain.lblBNKEditorNewSoundLength.Caption = Str(FileLen(frmMain.txtBNKeditorNewSoundPath.Text) - AIFR_HEADERSIZE)
    
    'committing these changes are done in the SET button section

    Exit Sub
    
getatterr:
    logMsg "Couldn't open log file for sound " & bfn.filename
End Sub

Public Sub fillInCurBnkSndInfo(which As Integer)
    'fill in current snd info
    If (theBnkContents(which).bReplace = True) Then
        If (theBnkContents(which).frequency = 22050) Then
            frmMain.optBNKEditor44khz.Value = False
            frmMain.optBNKEditor22khz.Value = True
        Else
            frmMain.optBNKEditor44khz.Value = True
            frmMain.optBNKEditor22khz.Value = False
        End If
        If (theBnkContents(which).numChans = 1) Then
            frmMain.optBNKEditorStereo.Value = False
            frmMain.optBNKEditorMono.Value = True
        Else
            frmMain.optBNKEditorStereo.Value = True
            frmMain.optBNKEditorMono.Value = False
        End If
        frmMain.txtBNKEditorNewBitrate.Text = Str(theBnkContents(which).theBitrate)
        frmMain.txtBNKeditorNewSoundPath.Text = theBnkContents(which).thePath
        frmMain.lblBnkEditPanning.Caption = Str(theBnkContents(which).pan)
        frmMain.lblBnkEditVol.Caption = Str(theBnkContents(which).vol)
        If (theBnkContents(which).loop = True) Then
            frmMain.lblBnkEditLooping = Str(theBnkContents(which).loops) & "-" & Str(theBnkContents(which).loope)
        Else
            frmMain.lblBnkEditLooping = "n/a"
        End If
    Else
        If (OrigBnkContents(which).frequency = 22050) Then
            frmMain.optBNKEditor44khz.Value = False
            frmMain.optBNKEditor22khz.Value = True
        Else
            frmMain.optBNKEditor44khz.Value = True
            frmMain.optBNKEditor22khz.Value = False
        End If
        If (OrigBnkContents(which).numChans = 1) Then
            frmMain.optBNKEditorStereo.Value = False
            frmMain.optBNKEditorMono.Value = True
        Else
            frmMain.optBNKEditorStereo.Value = True
            frmMain.optBNKEditorMono.Value = False
        End If
        frmMain.txtBNKEditorNewBitrate.Text = Str(OrigBnkContents(which).theBitrate)
        frmMain.txtBNKeditorNewSoundPath.Text = ""
        frmMain.lblBnkEditPanning.Caption = Str(OrigBnkContents(which).pan)
        frmMain.lblBnkEditVol.Caption = Str(OrigBnkContents(which).vol)
        If (theBnkContents(which).loop = True) Then
            frmMain.lblBnkEditLooping = Str(OrigBnkContents(which).loops) & "-" & Str(OrigBnkContents(which).loope)
        Else
            frmMain.lblBnkEditLooping = "n/a"
        End If
    End If
    
End Sub

Public Sub addNewEntriesToExistingBNK()
    Dim i As Integer, sSize As Long, tstr As String
    
    sSize = FileLen(frmMain.txtbnkeditorbnkpath.Text)   'current BNK file size
    ReDim tOffsets(totalBNKEntries)                     'resize our offsets array
    
    LogBoxSet frmMain.lstBNKEditorResults
    logMsg "backing up original BNK"
    
    breakfilename (frmMain.txtbnkeditorbnkpath.Text)    'break the filename for easier usage
    tstr = bfn.fullpath & bfn.fnameNoExt & ".bak"       'backup filename
    If (Dir(tstr, vbNormal) <> "") Then
        logMsg "deleting old backup"
        Kill tstr
    End If
    
    FileCopy frmMain.txtbnkeditorbnkpath.Text, tstr     'make a backup copy
    
    frmMain.prgBnkEditorProgress.Value = 0
    frmMain.prgBnkEditorProgress.Min = 0
    frmMain.prgBnkEditorProgress.Max = sSize
    
    For i = 0 To totalBNKEntries - 1
        If (theBnkContents(i).bReplace = True) Then
            breakfilename theBnkContents(i).thePath
            logMsg "Concatenating " & bfn.filename
            tOffsets(i) = sSize                         'offset of this entry is at the end of the file
            appendBinaryFiles frmMain.txtbnkeditorbnkpath.Text, theBnkContents(i).thePath, frmMain.prgBnkEditorProgress
            sSize = FileLen(frmMain.txtbnkeditorbnkpath.Text)   'update the offset
        Else
            tOffsets(i) = -1                            'flag it off
        End If
    Next i
End Sub

Public Sub alterExistingBNKHeaders()
    Dim i As Integer, fn As Integer, tpatch As bnkLut2, fnout As Integer, tpath As String, twav As WAVEFORMATEX
    Dim tBnkHdr As bnkHeader, tstr As String, ibyte As Byte
    
    fn = FreeFile
    On Error GoTo openerr
    Open (frmMain.txtbnkeditorbnkpath.Text) For Binary Access Read As #fn   'open the BNK file
    fnout = FreeFile
    tpath = workDir(workdir_bnk) & "custom.bnk"
    If (Dir(tpath, vbNormal) <> "") Then
        Kill tpath
    End If
    Open tpath For Binary Access Write As #fnout        'open a temporary bnk file
    On Error GoTo 0
    
    LogBoxSet frmMain.lstBNKEditorResults
    
    logMsg "Building working BNK file"
    Get #fn, , tBnkHdr                                  'copy the file header
    Put #fnout, , tBnkHdr
    
    twav.cbSize = 0                                     'common settings.....
    twav.wFormatTag = 1
    tpatch.Info = "PCH0"
    tpatch.pad1 = 0
    tpatch.pad2 = 0
    tpatch.pitch = 0
    tpatch.priority = 0
    
    logMsg "Patching headers"
    
    For i = 0 To totalBNKEntries - 1                    'go back and fix the headers now
        If (theBnkContents(i).bReplace = True) Then
                                                        'fill in the WAVEFORMATEX structure
            twav.nChannels = theBnkContents(i).numChans
            twav.wBitsPerSample = theBnkContents(i).numbits
            twav.nSamplesPerSec = theBnkContents(i).frequency
            twav.nBlockAlign = (twav.wBitsPerSample / 8) * twav.nChannels
            twav.nAvgBytesPerSec = twav.nSamplesPerSec * twav.nBlockAlign
            
                                                        'fill in the patch header
            tpatch.codecbitrate = theBnkContents(i).theBitrate
            tpatch.loopstart = theBnkContents(i).loops
            tpatch.loopend = theBnkContents(i).loope
            tpatch.offset = tOffsets(i) + 4             'add 4 to get past DATA string
            tpatch.pan = theBnkContents(i).pan
            tpatch.soundSize = theBnkContents(i).theSize
            tpatch.volume = theBnkContents(i).vol
            
            tpatch.waveFormat = twav                    'copy in the WAVEFORMATEX structure
            Put #fnout, , tpatch                        'write new patch header out
            Get #fn, , tpatch                           'and just read over the old one that we are skipping
        Else
            Get #fn, , tpatch                           'just copy the existing one
            Put #fnout, , tpatch
        End If
    Next i
    
    '-------- now, copy the rest of the file
    logMsg "Copying body of BNK file now"
    frmMain.prgBnkEditorProgress.Min = 0
    frmMain.prgBnkEditorProgress.Max = FileLen(frmMain.txtbnkeditorbnkpath.Text) - (totalBNKEntries * 56) - 12 ' 56=size of one patch, 12=size of file header
    frmMain.prgBnkEditorProgress.Visible = True
    frmMain.prgBnkEditorProgress.Value = 0
    frmMain.cmdBnkEditorReset.Enabled = False
    frmMain.cmdBnkEditorSaveChanges.Enabled = False
    Do
        Get #fn, , ibyte
        Put #fnout, , ibyte
        If (frmMain.prgBnkEditorProgress.Value + 1 < frmMain.prgBnkEditorProgress.Max) Then
            frmMain.prgBnkEditorProgress.Value = frmMain.prgBnkEditorProgress + 1
        End If
        DoEvents
    Loop Until EOF(fn)
    frmMain.cmdBnkEditorReset.Enabled = True
    frmMain.cmdBnkEditorSaveChanges.Enabled = True
    
    Close #fn                                           'close the BNK file
    Close #fnout
    
    logMsg "renaming files"
    
    breakfilename (frmMain.txtbnkeditorbnkpath.Text)
    tstr = bfn.fullpath & bfn.fnameNoExt & ".bak2"
    If (Dir(tstr, vbNormal) <> "") Then
        logMsg "destroying old backup file first"
        Kill tstr
    End If
    
    Name frmMain.txtbnkeditorbnkpath.Text As tstr       'original file now has a BAK2 extension
    FileCopy tpath, bfn.fullpath & bfn.filename         'copy working file as real file
    Kill tpath                                          'destroy old working file
    
Exit Sub

openerr:
    MsgBox "Couldn't open the BNK file for header editing!", vbCritical, "Error"
    
End Sub
    
Public Sub BNKExtractSounds(tpath As String)
    Dim tfn As Integer, tnum As Integer, tLong As Long, tbyte As Byte, tcalc As Integer, thdr As bnkHeader
    Dim tpatch As bnkLut2, tout As Integer, tpathout As String, i As Integer, curpos As Long, newpos As Long
    Dim newDir As String, touthdr As aifrHeader, tnumsamples As Long
    Dim trry(0 To 3) As Byte, j As Long, bExtrabytes As Boolean
    
    'test little->big endian conversion test
    'tLong = &H12345678
    'MKBEndian32 tLong, trry
    'MsgBox "In: " & Hex(tLong) & vbCrLf & vbCrLf & " out: " & Hex(trry(0)) & Hex(trry(1)) & Hex(trry(2)) & Hex(trry(3)), vbInformation, "test"
    
    frmMain.cmdExtractSounds.Enabled = False
    
    'make a directory under the current directory
    breakfilename (tpath)
    
    newDir = bfn.fullpath & bfn.fnameNoExt & "_bnk\"
    
    If (Dir(newDir, vbDirectory) = "") Then
        MkDir (newDir)
    End If
    
    'open files
    tfn = FreeFile
    Open tpath For Binary Access Read As #tfn
    
    'get num files to process
    Get #tfn, , thdr
    
    'how many sounds        === this doesn't take into account extra, unused sounds from modded BNKs!
    tnum = thdr.noPatches
    
    'prefill common stuff
    touthdr.formStr = "FORM"
    touthdr.aifcstr = "AIFC"
    touthdr.fverStr = "FVER"
    touthdr.commStr = "COMM"
    touthdr.compType = "COMP"
    touthdr.ssndStr = "SSND"
    touthdr.compName = "Relic Codec v1.6"
    touthdr.cnameNull = 0
    touthdr.cNameLength = 16
    touthdr.filler2 = 0
    touthdr.filler3 = 0
    touthdr.filler1(0) = 0
    touthdr.filler1(1) = 0
    touthdr.filler1(2) = 0
    touthdr.filler1(3) = 0
    touthdr.filler1(4) = 0
    touthdr.vckDataSize(0) = 0
    touthdr.vckDataSize(1) = 0
    touthdr.vckDataSize(2) = 0
    touthdr.vckDataSize(3) = 4
    touthdr.vtimestamp = &H405180A2
    touthdr.cckDataSize(0) = 0
    touthdr.cckDataSize(1) = 0
    touthdr.cckDataSize(2) = 0
    touthdr.cckDataSize(3) = 39
    
    frmMain.progExtractSounds.Min = 0
    frmMain.progExtractSounds.Max = tnum
    
    'loop through the patches
    For i = 1 To tnum
        'get next patch
        Get #tfn, , tpatch
        
        'save current position
        curpos = Seek(tfn)
        
        'start new file
        tpathout = newDir & bfn.fnameNoExt & Trim(Str(i)) & ".aifr"
        tout = FreeFile
        Open tpathout For Binary Access Write As #tout
        
        'finish the header
        MKBEndian32_2 (tpatch.soundSize + AIFR_HEADERSIZE - 8), trry
        touthdr.ckDataSize(0) = trry(0): touthdr.ckDataSize(1) = trry(1): touthdr.ckDataSize(2) = trry(2): touthdr.ckDataSize(3) = trry(3)
        touthdr.numChannels(0) = 0
        touthdr.numChannels(1) = tpatch.waveFormat.nChannels
        tnumsamples = (tpatch.soundSize / tpatch.waveFormat.nChannels / (tpatch.codecbitrate / 8))
        MKBEndian32_2 tnumsamples, trry
        touthdr.numSamples(0) = trry(0): touthdr.numSamples(1) = trry(1): touthdr.numSamples(2) = trry(2): touthdr.numSamples(3) = trry(3)
        touthdr.sampleSize(0) = 0
        touthdr.sampleSize(1) = tpatch.waveFormat.wBitsPerSample
        If (tpatch.waveFormat.nSamplesPerSec = 11025) Then
            touthdr.sRateSpecial(0) = &H40: touthdr.sRateSpecial(1) = &HC: touthdr.sRateSpecial(2) = &HAC: touthdr.sRateSpecial(3) = &H44
        ElseIf (tpatch.waveFormat.nSamplesPerSec = 22050) Then
            touthdr.sRateSpecial(0) = &H40: touthdr.sRateSpecial(1) = &HD: touthdr.sRateSpecial(2) = &HAC: touthdr.sRateSpecial(3) = &H44
        Else
            touthdr.sRateSpecial(0) = &H40: touthdr.sRateSpecial(1) = &HE: touthdr.sRateSpecial(2) = &HAC: touthdr.sRateSpecial(3) = &H44
        End If
        MKBEndian32_2 tpatch.soundSize, trry
        touthdr.sckDataSize(0) = trry(0): touthdr.sckDataSize(1) = trry(1): touthdr.sckDataSize(2) = trry(2): touthdr.sckDataSize(3) = trry(3)
        tLong = CLng(tpatch.codecbitrate)
        MKBEndian32_2 tLong, trry
        touthdr.codecbitrate(0) = trry(2): touthdr.codecbitrate(1) = trry(3)
        
        'write the header
        Put #tout, , touthdr
        
        'seek to the raw sound start position (+1 to adjust for diff between vb and c)
        newpos = tpatch.offset + 1
        Seek #tfn, newpos
        
        'copy this raw sound data to the output file - try to do it 4 bytes at a time
        If ((tpatch.soundSize Mod 4) <> 0) Then
            bExtrabytes = True
        Else
            bExtrabytes = False
        End If
        
        'copy 4 at a time first
        For j = 1 To (tpatch.soundSize \ 4)
            Get #tfn, , tLong
            Put #tout, , tLong
        Next j
        
        'if there are any leftover bytes, copy them too
        For j = 1 To (tpatch.soundSize Mod 4)
            Get #tfn, , tbyte
            Put #tout, , tbyte
        Next j
        
        'close this file
        Close #tout
        
        'jump back to the same spot in the header we just left
        Seek #tfn, curpos
        
        'update progress bar
        frmMain.progExtractSounds.Value = i
        
        ' let windows run
        DoEvents
        
    Next i
    
    frmMain.progExtractSounds.Value = tnum
    frmMain.cmdExtractSounds.Enabled = True
    
    'close files
    Close #tfn
'    Close #tout
End Sub
