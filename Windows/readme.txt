
 ** Windows HWSDL README **

//troubleshooting
//using ignoreBigfiles command line option
//bugs

//using ignoreBigfiles command line option
hwsdl.exe /ignoreBigfiles /prepath c:\hwsdl\data
(you may also wish to use /window)

if you extract all the data from Homeworld.big and Update.big and put it in (for example) c:\hwsdl\data
and also move HW_Comp.vce and HW_Music.wxd there, then you'll be able to avoid the .big file problems when using compiled builds from VC++ 2005/8.

On Windows one can use this to extract .big files:
http://www.google.com/search?&q=WinBig+Begust


//bugs
> if you're close enough in view: hwsdl crashes when a ship displays 'hit effects'
Workaround: turn off 'hit effects' in video options. This fixes the crash.
'hit effects' are the effects drawn on ships hit by weapons.

without workaround:
- sometimes it still happens when you're looking farther away at the combat
- i haven't had it happen while in sensors manager.
(confirmed in rev64 also)

> ship move rotation bug
where: this bug only occurs when not in fullscreen mode (eg: if you're using /window)

how: select a ship of yours, press m to move ship and now try rotating around (bug! it becomes uncontrollable and spins around fast)
press m to abort move, and now rotate around (works)
- while the bug is rotating you around fast, left click once to temporarily fix the fast rotation.
(confirmed in rev64 also)

> no BTG background and reduced stars due to code workaround to avoid crashing during level loads.
(confirmed this workaround was needed in rev64 also)

> no sound after changing resolution
workaround: change resolution and restart hwsdl to get sound

> aiplayerChooseEnemies assert
- you might hit this on some level loads


//troubleshooting
> if HWSDL ever freezes consistently too much (are you modifying it?) you may wish to use /window to avoid fullscreen.
for example: hwsdl.exe /window
