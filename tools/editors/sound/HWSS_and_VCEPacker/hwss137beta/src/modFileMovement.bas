Attribute VB_Name = "modFileMovement"
Option Explicit

Public Function copyToMP3WavWorkingDir(origFile As String) As String
    Dim tname As String, tmpNum
    
    ' in case they have some funky, long name with weird chars, make a
    ' temp name in the dos 8.3 format so that the LAME encoder won't freak out
    breakfilename origFile
    
    Randomize
    tmpNum = Int((100 * Rnd) + 1)
    tname = workDir(workdir_mp32wav) & "mpwav" & Format(tmpNum, "000") & ".mp3"
 
    While (Dir(tname, vbNormal) <> "")
        tmpNum = Int((100 * Rnd) + 1)
        tname = workDir(workdir_mp32wav) & "mpwav" & Format(tmpNum, "000") & ".mp3"
    Wend
 
    FileCopy origFile, workDir(workdir_mp32wav) & bfn.filename
    Name workDir(workdir_mp32wav) & bfn.filename As tname
    
    copyToMP3WavWorkingDir = tname
    
End Function

Public Sub moveToWavDir(tmpname As String, newname As String, thePath As String)
    Dim newPath As String, newwav As String
    
    newPath = thePath & newname
    
    breakfilename tmpname
    newwav = bfn.fullpath & bfn.fnameNoExt & ".wav"
    
    FileCopy newwav, newPath
    
    Kill (newwav)
End Sub

Public Function copyToWav2AifrWorkingDir(origFile As String) As String
    Dim tname As String, tmpNum
    
    ' in case they have some funky, long name with weird chars, make a
    ' temp name in the dos 8.3 format so that the aifr encoder won't freak out
    breakfilename origFile
    
    Randomize
    tmpNum = Int((100 * Rnd) + 1)
    tname = workDir(workdir_wav2aifr) & "wvaif" & Format(tmpNum, "000") & ".wav"
 
    While (Dir(tname, vbNormal) <> "")
        tmpNum = Int((100 * Rnd) + 1)
        tname = workDir(workdir_wav2aifr) & "wvaif" & Format(tmpNum, "000") & ".wav"
    Wend
 
    FileCopy origFile, workDir(workdir_wav2aifr) & bfn.filename
    Name workDir(workdir_wav2aifr) & bfn.filename As tname
    
    copyToWav2AifrWorkingDir = tname
    
End Function

Public Sub moveToAifrDir(tmpname As String, newname As String, thePath As String)
    Dim newPath As String, oldaif As String
    
    newPath = thePath & newname
    
    breakfilename tmpname
    oldaif = bfn.fullpath & bfn.fnameNoExt & ".aifr"
    
    FileCopy oldaif, newPath
    
    Kill (oldaif)
End Sub

Public Function copyToAifr2AifrxWorkingDir(origFile As String) As String
    Dim tname As String, tmpNum
    
    breakfilename origFile
    
    Randomize
    tmpNum = Int((100 * Rnd) + 1)
    tname = workDir(workdir_aifr2aifrx) & "aif2x" & Format(tmpNum, "000") & ".aifr"
 
    While (Dir(tname, vbNormal) <> "")
        tmpNum = Int((100 * Rnd) + 1)
        tname = workDir(workdir_wav2aifr) & "aif2x" & Format(tmpNum, "000") & ".aifr"
    Wend
 
    FileCopy origFile, workDir(workdir_aifr2aifrx) & bfn.filename
    Name workDir(workdir_aifr2aifrx) & bfn.filename As tname
    
    copyToAifr2AifrxWorkingDir = tname
End Function

Public Sub moveToAifrxDir(tmpname As String, newname As String, thePath As String)
    Dim newPath As String, oldaifrx As String
    
    newPath = thePath & newname
    
    breakfilename tmpname
    oldaifrx = bfn.fullpath & bfn.fnameNoExt & ".aifrx"
    
    FileCopy oldaifrx, newPath
    
    Kill (oldaifrx)
    Kill (tmpname)
End Sub

Public Sub moveToWXDDir(distFile As String, gamedir As String)
    Dim tstr
    
    tstr = gamedir
    breakfilename (distFile)
    
'    If (Dir(gamedir & bfn.filename) <> "") Then
'        Kill gamedir & bfn.filename
'    End If
    
    FileCopy distFile, tstr & bfn.filename
    Kill distFile
    
End Sub


Public Function copyToAifr2AifrbWorkingDir(origFile As String) As String
    Dim tname As String, tmpNum
    
    breakfilename origFile
    
    Randomize
    tmpNum = Int((100 * Rnd) + 1)
    tname = workDir(workdir_aifr2aifrb) & "aif2b" & Format(tmpNum, "000") & ".aifr"
 
    While (Dir(tname, vbNormal) <> "")
        tmpNum = Int((100 * Rnd) + 1)
        tname = workDir(workdir_aifr2aifrb) & "aif2b" & Format(tmpNum, "000") & ".aifr"
    Wend
 
    FileCopy origFile, workDir(workdir_aifr2aifrb) & bfn.filename
    Name workDir(workdir_aifr2aifrb) & bfn.filename As tname
    
    copyToAifr2AifrbWorkingDir = tname
End Function

Public Sub moveToAifrbDir(tmpname As String, newname As String, thePath As String)
    Dim newPath As String, oldaifrx As String
    
    newPath = thePath & newname
    
    breakfilename tmpname
    oldaifrx = bfn.fullpath & bfn.fnameNoExt & ".aifrb"
    
    FileCopy oldaifrx, newPath
    
    Kill (oldaifrx)
    Kill (tmpname)
End Sub


