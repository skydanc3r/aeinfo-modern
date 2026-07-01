Big things:

Medium things:
        - make a clean cut and split the code into separate modules (.h and .cpp files)

Minor changes:
        - display the powersensor.2 in short-mode
        - display the tubemeter readings in short-mode
        - display the aquastream(s) in short-mode
        - csv-output-mode?!
        - make use of autoconf/automake?!
        - one should be able to turn off the use of colors at runtime
	- "prgrammable" (optional) automatic refresh of HTML-Pages

Some "let's think about that" ideas:
        - make matching of field-names case-insensitive?!
        - if fan.4 is used as a flowmeter, there should be fields like flowmeter.2.XXX
        - it would be nice, if we had a real Introspector-class...


Done stuff:
        - colorful HTML-bargraphs for sensors!
        - aquaero should only be accessed at some specific sub-commands only
        - create a aquaerod (daemon) that does the readings in the background
          it would be nice, if aeinfo could access aquaero directly, if
          aquaerod is not running...
        - create a webserver
