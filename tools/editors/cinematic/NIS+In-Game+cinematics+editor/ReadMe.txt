NisEdit

  This program will display the paths of objects within
the nis files of Homeworld and Homeworld:Cataclysm.

  The first thing to do is to use the BigViewer and 
extract the nis directory into the corresponding 
Homeworld directory.  Also, this program uses OpenGL
to view the ship paths.

  Next, run the NisViewer and select 'Homeworld' or 
'Cataclysm' in the combobox.  Click 'Load' and select
one of the .nis files (selecting the .script file will
also work).  The file will be displayed and you will be
able to traverse the structures of the .nis file and the
.script file.

 There are four panes displayed.  The first (the one on the left)
displays the objects within the .nis file.  Generally that
will be the strings, objects(ships), and the camera. Clicking 
on the filename in the treeview will display the header 
information of the file in the middle panes. Clicking on 
the strings will display the strings (ship names) in the middle 
panes.  Expanding the Objects will show all the objects(ships) 
within the file.  Clicking on the ship names will show the
header information for the ship.  Expanding further and clicking 
on 'path' will show the path the ship follows within the scene.
This of course will bring up the scene view.  The view will be 
discussed later.  Now back to the panes.
  The middle panes show the contents of the structure and 
how it looks in the file.  The top pane displays the hex 
view of the data at the position it is in the file.  The 
bottom pane shows the structure of the data in a readable 
format.  Some of the data may not be complete in describing 
the data, as I haven't figured out what everything is doing.
So there will be areas where I have put Unknown and ???.
  The right pane shows the .script file for easy reference.

The 'view' shows the paths of all the ships with the path 
of the ship selected highlighted. (The path of the ship must
be selected to be highlighted). The view can be moved with 
the arrow keys or the mouse.

KeyBoard:
Up_Arrow - Move viewpoint forward into scene.
Down_Arrow - Move viewpoint backward outward from the scene.
Left_Arrow - Rotate view to the left.
Right_Arrow - Rotate view to the right.
Shift Left_Arrow - Move viewpoint left.
Shift Right_Arrow - Move viewpoint right.

Mouse:
Move the mouse over the view window.
Hold down the right mouse button to move/rotate the viewpoint.
  Move forward and backward to move the viewpoint forward and
    backward.
  Move left and right to rotate the view left and right.

Hold down the left and right mouse button to move the viewpoint up and
down, left and right.


When a path is selected in the treeview the values within the lower middle
pane can be edited. 

After editing, clicking on the save button will allow you to save the nis
file. Just specify the filename to save as. 

Have fun :-)



