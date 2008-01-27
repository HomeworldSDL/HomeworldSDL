**Homeworld turret tools V1.0 by Ratbag**            15-9-00

Hey,

To get turrets working several things have to be correct.  
First of all your model should be almost finished and ready to put into the game.  It should be all one group/object and preferably textured.  Adding turrets will be the last thing you should do(unless testing of course).  You only need ONE mesh for each TYPE of turret/object, not for each turret, and this has to be placed on the X-axis (if you want it to rotate around its own axis).  All rotation is relative to this axis! ie. it acts as the pivot point.  Best if it is placed with the bottom just above the Z-axis, makes it easier to work out the offsets.

Next use the peodupe tool to duplicate the object so you have as many turret objects as you need and link them with peochain.  The heirarchy is very important so the game knows what objects are attached to what in what order.  No linking, no turrets. **NOTE: peodupe.exe now preserves texture data**

Next the offsets of the turrets have to be changed with peomatrix so they appear in the correct position on the hull.  This is straightforward.

The MEX is as for any normal model with the added complication of VME(Visual MEX Editor) having a bug  that reverses the Z transform and puts your shiny new turret out in space by itself.  Not to worry, it can look after itself so just position the gun point where the thing should be.  (I suppose we could enter the z co-ordinate in the reverse (-35 instead of 35) to see if it lines up with the misplaced turret then take the reverse out before saving, you choose)  

The .shp file can be a bit particular.

The guns must be of Type GUN_NewGimble, with the appropriate limits for traversing angles and very important, the file must contain the lines for gunbindings.
eg.
BindGun CorvetteGun(1) Gun0,Azimuth,Frequency 1
BindGun CorvetteGun(2) Gun1,Azimuth,Frequency 1
etc
where CorvetteGun(X) is the name of the turret mesh/object and GunX is the name of the gunpoint in the MEX file that corresponds to the same turret.

I don't think what the names are is important as long as they they are consistant
Keep in mind that 3dExploration truncates names to 10 characters when it makes a .peo.

Happy modelling

Ratbag


