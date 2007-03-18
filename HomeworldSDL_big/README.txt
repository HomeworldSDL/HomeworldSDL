Please ONLY add files that are different from the official Relic .big versions. When you want to add a new file please do the following:

- `svn add` the original version from "Homeworld.big" and `svn commit`
- copy the version from "Update.big" over the top and `svn commit`
- make your own changes to the file and `svn commit` again

The intent here is to track the differences made to the file throughout its history. (Note that the file in question may not have copies in all of the .big files.)

----

To convert this directory into an actual .big file you will need to compile tools/biggie. Then:

    cd <root path of working copy>
    ./HomeworldSDL_big/convert_directory_to_big_file

This will create "HomeworldSDL.big" in the root directory of the working copy.(Note the difference in name between the big file itself ".big" and the seed directory "_big".) This README and the creation script will not be added to the generated .big file.

