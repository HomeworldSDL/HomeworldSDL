@echo off
if exist %1\dependtex.mif del %1\dependtex.mif > NUL
echo # > %1\dependtex.mif
for %filename in (%1\*.psd) echo TEXTURES += %@name[%filename].lif >> %1\dependtex.mif

if exist %1\dependgeo.mif del %1\dependgeo.mif > NUL
echo # > %1\dependgeo.mif
for %filename in (%1\*.lws) echo MODELS += %@name[%filename].geo >> %1\dependgeo.mif

