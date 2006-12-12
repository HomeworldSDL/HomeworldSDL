
 ** Windows HWSDL README **

//bugs
> if you're close enough in view: hwsdl crashes when a ship kills another ship
- sometimes it still happens when you're looking farther away at the combat
- i haven't had it happen while looking in sensors manager.

> sensors manager ship move rotation bug
select a ship of yours > sensors manager > press m to move ship and now try rotating around (bug! it becomes uncontrollable and spins around fast)
- press m to abort move, and now rotate around (works)
- while the bug is rotating you around fast, left click once to temporarily fix the fast rotation.

> no BTG background and reduced stars due to code workaround to avoid crashing during level loads.

> no sound after changing resolution, and then when exiting HWSDL, it exits or crashes.

> hwsdl freezes if exited (possibly somewhat similar to how the linux build does without using linuxfixme)

> aiPlayerstartup (or something like that) assert
- you might hit this on some level loads
