                            ---------------
			      LithUnwrap
                              Version 1.2
                            ---------------


DESCRIPTION
===========
LithUnwrap is a free Windows tool for mapping and editing the 
UV coordinates of low poly models. It can unwrap a 3D model and
export the mapping to a bitmap texture which can then be painted.


RECOMMENDED
===========
 - 3D hardware-accelerated graphics card.
 - 16-bit or 32-bit display mode setting.
 - DirectX 8 or later.   See: www.microsoft.com/directx
 - OpenGL 1.1 or later.  See: www.opengl.org, www.glsetup.com


CONFIGURATION/INFO
==================
See unwrap.ini and unwrap.log for more details.


UV MAPPING
==========
"Select by Face" or "Select by Group" must be checked to apply 
a mapping to the selected faces.

camera      - Map the model from the camera.
decal       - Map the model from one side.
planar      - Map the model from two sides. 
box         - Map the model from six sides. 
cylindrical - Map the model from a cylinder.  
spherical   - Map the model from a sphere. 


BOX MAPPING
===========
Box mapping arranges the faces to the following layout:

 Left  - Top    - Front  
 Right - Bottom - Back

In some cases, you might have groups of polygons that overlap 
each other after applying a general box mapping.  By checking the 
"Separate by group" option, you can separate the mappings of each 
group so they don't overlap.  This option arranges the faces to 
the following layout:

 (group 1)   (group 2)  ...etc.
 L - T - F   L - T - F 
 R - B - B   R - B - B


MODELS
======
Up to 10 primitive models can be created within LithUnwrap: a plane, 
a box, a cone, a cylinder, a sphere, a geosphere, a pyramid, a prism 
(extruded isoceles triangle), a tube, and a torus.  

Upon creation, each model is automatically applied one of the available 
UV mappings, with exception to the tube and torus.  Their UV mappings are 
auto-generated upon creation.


SELECTING
=========
In order to move, scale, rotate, flip, expand, or collapse vertices, 
they must be selected first.

Two basic ways to select vertices is to drag a box around them or 
click on them.  Any selected vertex is then highlighted in red.

Once a vertex is selected, it can be moved by clicking in the 
rectangle that outlines the selection and then dragging it.  


SCALING
=======
To scale UV coordinates freely, click Edit->Scale->Free.  To scale
the selection, click and drag one of the green corner drag boxes.

The S key can be used to quickly enable this feature.


MODIFY GROUPS
=============
In order to assign selected faces to a group, "Select by Face" or 
"Select by Group" must be checked.


DELETE GROUPS
=============
Note that deleting any group will permanently delete the associated 
geometry as well.  Use "Remove..." under Modify Groups instead if
you want to remove the group name and not the geometry.


HIDING/UNHIDING
===============
If you have a large and complex model, you can hide individual groups 
to improve drawing performance.

After a group is hidden, it will not be listed in the "Select by Name"
listbox.  The group must be unhidden first before it can be selected
again.


MERGING
=======
To merge LWO and LUV files, the option "Autoload LUV with LWO" must 
be checked under the Preferences menu.  Also, the two files must have
the same name (e.g. mymodel.lwo, mymodel.luv) and reside in the same 
directory.  


EXPAND/COLLAPSE
===============
Expand/collapse are helper functions for scaling.  Expand scales
the selection to the maximium UV space of 0,0 to 1,1, while collapse 
scales the selection to a single point.  Note that collapsed points 
cannot be expanded afterwards because they are dimensionless.


INFORMATION
===========
bounding box - The min/max values of the axis-aligned bounding 
               box of the model in 3D space.
groups       - The number of assigned groups in the model.
polygons     - The number of n-sided polygons in the model.
triangles    - The number of triangles in the model.  
vertices     - The number of indexed vertices in the model.


UNDO
====
There are 8 levels of undo available.  You can undo vertices that 
were selected, moved, scaled, flipped, rotated, expanded, collapsed,
or mapped.


MATERIALS
=========
Materials in LithUnwrap are simply textures.  They do not have any other 
modifiable properties at the moment.  Any loaded texture is applied to 
the entire model in the scene.

To toggle off the material in the Editor window, click Edit->Material->Visible.
Note that you still will be able to see the texture in the Preview window.

The B key can be used to quickly toggle this feature.


CHECKER
=======
A checker pattern can be used to visually locate UV mapping disortion
on a model.

To apply a checker material to your model, click File->Material->Checker.
You must have Textured or Wireframe+Textured rendering enabled to 
see the checker material.  More options can be found under the Preferences
menu option.

Since the checker material uses a 256x256 bitmap, choose a UV tiling value
that is divisible by 256, such as 4, to evenly space out the checker pattern. 

The C key can be used to quickly toggle this feature.


WELDING
=======
To weld UV coordinates, click Edit->Weld.  The tolerance value is the 
minimum distance necessary for welding between any two UV coordinates. 

Weld = 0 < distance < tolerance (UV space)


ALIGN FACE
==========
To align a face to a major axis or to the camera, "Select by Face" must
be selected.  If you select more than one face, the alignment will be
the averaged sum of all selected face normals.


PICKING
=======
In some cases, it might be easier to select faces from a 3D perspective
view instead of a 2D view.

To select faces in Preview Mode, you must enable Wireframe+Textured or
Wireframe rendering first.  Then toggle the "Select Faces" menu option in
the Preview Mode floating menu.  The S key can also be used to quickly
toggle this feature. 

After you have selected your faces, toggle "Select Faces" again to lock 
your selection.

To deselect everything, click in empty space.


SHORTCUT KEYS
=============

 Editor Window
 =============
  Ctrl+Z = Undo

  Q = select mode
  W = move freely
  E = zoom freely
  R = zoom region

  B = toggle visible material 
  C = toggle checker material
  S = scale selection
     
 Preview Window
 ==============
  P = 3D perspective view
  F = front orthogonal view
  B = back orthogonal view
  L = left orthogonal view
  R = right orthogonal view
  T = top orthogonal view
  Y = bottom orthogonal view
  C = toggle checker material


TEXTURES
========
For best performance and compatibility, there are a couple of rules
you should follow when loading textures:

1. Use textures that have dimensions that are powers of 2.
  (e.g. 2^n = 64, 128, 256, 512, etc...)

2. Use square textures. (e.g. 128x128, 256x256, etc...)

3. Avoid using very large textures.  Some 3D cards may not support 
   dimensions larger than 256x256. (e.g. older Voodoo cards)


GAME FORMATS					VERSION
============					=======
LUV - Tread Marks				   -
PRM - Re-Volt					   -
MDL - Quake 1                                      6
MD2 - Quake 2					   8
MD3 - Quake 3 Arena				  15
NOD - Vampire: The Masquerade - Redemption         7
SMF - 4x4 Evolution			           4
SOD - Star Trek Armada			     1.6, 1.7, 1.8
MDL - Serious Sam                                 10
ACT - Genesis3D					 0.241
3DO - Jedi Knight: Dark Forces 2                2.1, 2.2
L3D - Black & White                                -
MDL - Half Life                                   10
POF - Freespace 1/2                              21.17
OOF - Descent 3					 23.00
PEO - Homeworld                                  1026

If you would like to see a game format supported, send me
the file format specification and I will see what I can do.


GRAPHIC LIBRARIES
=================
LithUnwrap uses the JPEG Software Package (Release 6b) from the 
Independent JPEG Group to read JPG files.  A file named lithlib.dll
should reside in the same directory as the main program.


CONTACT
=======
Author: Brad Bolthouse
Email: bbolthou@bigfoot.com
URL: http://www.geocities.com/lithunwrap


FINAL
=====
This program is free!  Gratis!  Frei!  Libre!  Libero!  Vrij!

You are allowed to distribute this program freely as long as it is 
not for profit.

The author assumes no liability for damages, direct or consequential, 
which may result from the use of this program.

All trademarks and trade names are properties of their respective 
owners.

Copyright © 2001.  All Rights Reserved.
