aeinfo-samurize README
======================

aeinfo-samurize is (c) 2006 by
  Christian Unger <coder at breakbe D0T at>
and distributed under the terms of the GNU Lesser General Public License (LGPL).
See the file ./COPYING in the source distribution for more information.

The source of aeinfo-samurize is part of the aeinfo-project and can be found at
  http://breakbe.at/development/aquaero/

Revision: $Id: README.txt 34 2006-12-17 16:07:48Z matt $

Please send bugreports to <coder at breakbe D0T at>.

--------------------------------------------------------------------------------

Installation:

 1. Extract the DLL to Samurize-"plugins"-Folder
 2. make sure, the aeinfo-daemon "aquaerod" is running 
    (the daemon-version does not matter - any version >=2.0beta2 will do)
 3. (re-)start samurize(?!)

Configuration (within samurize):

 1. Open the samurize-editor
 2. "Add Meter" > "Add Plugin" and set the "Draw Type" to "Progressbar"
 3. Under the Tab "Source":
  3.1 select the plugin "aeinfo_samurize.dll"
  3.2 select the function "getValue" and enter "fan.1.setPower" at the popup

Note: For a complete list of fieldnames (parameter for the function "getValue")
have look at the output of "aeinfo help fields"


-have fun

