README for Relic audio encoder v1.0
December, 2000
------------------------------------------------
Copyright (c) 1997-2000 Relic Entertainment Inc.


--------
CONTENTS
--------
Getting Started
   Installation
   Usage
   Bitrates
Known Issues
Technical Support


---------------
Getting Started
---------------

Installation
------------
This is a command line tool and can be placed anywhere on your system (preferbly somewhere in your path).

Usage
-----
enc <wav_file> <aifr_file> [block_bitrate [stat_file]]

where:
<wav_file> - the name of the wave file you want to encode, must be an uncompressed wave file.  (REQUIRED)
<aifr_file> - name of the compressed aifr file to create.  (REQUIRED)
[block_bitrate] - data rate to compress the aifr to, see bitrates below.  (NOT REQURED, default = 2048)
[stat_file] - text output file for debuggin info.  (NOT REQUIRED)

Bitrates
--------
Common bitrates table:

bits/block    kbps/channel   quality
  2048           176         Highest quality, used for music in cinematics
  1024            88         Medium quality, used for in-game music
   512            44         Medium quality, used for fleet command and intel speech
   256            22         Lowest quality, used for pilot speech

[block_bitrate] takes the value on the left.

Any bits/block value can be set, but the program will round it to the nearest supported kbps/channel.


------------
Known Issues
------------
None at this time.  Please report any issues to the contacts listed below.


-----------------
Technical Support
-----------------
THIS IS AN UNSUPPORTED TOOL.  Issues will be brought up in the Editing forum on Relic's website (http://www.relic.com/cgi-bin/Ultimate.cgi).  You may also contact Relic at contact@relic.com, we will try to respond within a reasonable amount of time.
