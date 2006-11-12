The following tools are located in the "tools/win32" directory of the original Homeworld source code. (Relic's pre-compiled Windows versions of these tools live in "tools/bin".) A number of these tools are miscellaneous utilities that don't have any specific relevance to Homeworld, so the aim here is to work out what we need and, more importantly, what we don't... 

-----------------------------------------------------------------------------
[A]lready imported into HomeworldSDL
[Y]es, this tool should be imported
[N]o, it should not be imported 
[?] not sure if this is useful or not

We need to convert [?]s to one of [Y] or [N] and [Y]s to [A]s. Then get the [A]s compiled and working.
-----------------------------------------------------------------------------

[A] Biggie           .BIG file extractor utility
[N] BigTree          Windows equivalent of: `find <path> -not -type d`
[A] BTG              .BTG background editor
[Y] builder          .BIG file creator from directory tree?
[N] CDKeyer          CD registration key checker/registration?
[Y] FEMan            Front End manager; interface editor for menus etc
[N] FileTrunc        extracts chunks of files; similar to head/tail
[?] Fontastic        something to do with fonts...
[?] FXFont           "Front-end font exchange tool"
[?] FXLate           "Front-end translation tool"
[?] FXTract          "Front-end extraction tool"
[?] HWDetect         Direct3d detection?
[?] HWVideoTweaker   not sure what video it tweaks...
[?] IniScript        .ini script validation?
[A] KAS              .KAS -> C converter
[Y] LiFList          creates a list of .LiFs
[?] LWExport         converts <something> to LightWave?
[?] Mission          Mission(Man) editor but doesn't look like it's complete...
[?] NetLogView       network debugging tool?
[?] OpenBTG          .BTG viewer?
[?] Paco             something to do with .LiFs
[N] Patcher          applies patch to homeworld installation's data files
[N] PktView          another network debugging tool?
[N] Plumber          used to determine source of leaked pre-release game discs
[Y] polycount        counts number of polygons in mesh file
[N] Roofer           generates file with random garbage in it (e.g. Extra.big)
[?] Scaly            scales .LWO files
[?] SetLang          allows you to edit language strings in some config file
[?] StackDump        "Analyze a stack dump and display a call stack from the .MAP file"
[?] statviewer       compare two ships' stat/sync files
[N] teapot           something to do with vertex culling (test?)
[?] Vol              something to do with sound


(There may also be some bits and pieces in "tools/batch" that are of use but they mostly appear to be utilities for pulling together the files required for Relic's eventual Gold Master release.)
