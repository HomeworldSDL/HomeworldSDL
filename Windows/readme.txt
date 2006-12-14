
 ** Windows HWSDL README **

//troubleshooting
//using ignoreBigfiles command line option
//bugs


//troubleshooting
> if you're freezing or crashing too much you may wish to use /window to avoid fullscreen.
for example: hwsdl.exe /window


//using ignoreBigfiles command line option
hwsdl.exe /ignoreBigfiles /prepath c:\hwsdl\data
(you may also wish to use /window)

if you extract all the data from Homeworld.big and Update.big and put it in (for example) c:\hwsdl\data
and also move HW_Comp.vce and HW_Music.wxd there, then you'll be able to avoid the .big file problems when using compiled builds from MSVC++ 2005.

On Windows one can use this to extract .big files:
http://www.google.com/search?&q=WinBig+Begust


//bugs
> if you're close enough in view: hwsdl crashes when a ship displays 'hit effects'
Workaround: turn off 'hit effects' in video options
- sometimes it still happens when you're looking farther away at the combat
- i haven't had it happen while looking in sensors manager.
(confirmed in rev64 also)

> sensors manager ship move rotation bug
select a ship of yours > sensors manager > press m to move ship and now try rotating around (bug! it becomes uncontrollable and spins around fast)
- press m to abort move, and now rotate around (works)
- while the bug is rotating you around fast, left click once to temporarily fix the fast rotation.
(confirmed in rev64 also)

> no BTG background and reduced stars due to code workaround to avoid crashing during level loads.
(confirmed this workaround was needed in rev64 also)

> no sound after changing resolution, and then when exiting HWSDL, it exits or crashes.

> hwsdl freezes if exited (possibly somewhat similar to how the linux build does without using linuxfixme)

> aiPlayerstartup (or something like that) assert
- you might hit this on some level loads
