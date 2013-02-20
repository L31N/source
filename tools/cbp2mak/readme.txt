/*
  cbp2mak - Copyright © 2008 grischka

  cbp2mak is free software, released under the GNU General Public License
  (GPL version 2). For details see:

  http://www.fsf.org/licenses/gpl.html

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.
*/

Purpose:
    Convert CodeBlocks project/workspace files to makefiles

Build:
    On Linux / Windows:
      $ make

    Note: cbp2mak is made in 3 steps:
    1) build the tool xml2c
    2) use xml2c to convert the project/workspace templates cb[pw].xml to .c/.h
    3) build cbp2mak which includes cb[pw].[ch]

Usage:
    cbp2mak [-C directory] [-nmake] infile.cbp/.workspace [outfile]

Example:
    generate makefile for CodeBlocks core:
      $ cbp2mak -C cb/src CodeBlocks.cbp
    generate makefile for CodeBlocks contrib:
      $ cbp2mak -C cb/src ContribPlugins.workspace
    build:
      $ make -C cb/src -f CodeBlocks.cbp.mak
      $ make -C cb/src -f ContribPlugins.workspace.mak

    You might want to adjust some hardcoded paths in the makefile template
    or specify _WX=/path/to/wx and _CB=... at the make commanline.

Todo:
    - add support for virtual targets (currently it builds all targets
      that it finds)
    - probably more ....


22.12.2008
-- grischka@users.sourceforge.net

### http://forums.codeblocks.org/index.php/topic,9731.msg68486.html#msg68486
