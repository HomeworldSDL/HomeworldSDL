All,

This is the first release of a utility I call peoTool, but which
is built upon a combination of codebases, primarly peoMat and
Mad165 from Spooky.

List of new features:
- Full preview of object (untextured for now)
- Any matrix changes are automatically updated on preview window
- Red, Green, Blue colours for material properties
- Quick backup of PEO in case you make any mistakes :)
- Conversion to Cataclysm for PEO files included
- Texture preview, including LIF details

Note that at present the Save function is disabled, however
when you Apply the material properties it automatically saves
the information.

Also note that although you *can* change the RGB values of the
material, it wont update the actual values.  This will come 
later.

Spooky,

I've noticed that if you *first* select a node from the TreeView
that has the local6 and local11 values set to something other
that 1, then it doesn't seem to fill in the rotX text box (ie
it's blank).  Since the parent object nearly always has these
values set to 1, then if you select the parent then a child it's
fine.

I had to add a default setting of zero before all the rotation
and whatever where calculated.

 1  2  3  4
 5  6  7  8
 9 10 11 12
13 14 15 16

I also noticed that, if you take the diagram above as an
example, you aren't doing anything with 6, 7, 10 and 11.

Can you tell me what these values are? (I think 6 and 11 are
scaleY and scaleZ)

Anyway, thats it.  Download link: 

Regards
Delphy