This is a converter from a .tsv, that you export from a spreadsheet, to a .txt
that can be used as a script for TAS-nx. Here is a link to TAS-nx:
https://github.com/hamhub7/TAS-nx/releases

I made a video tutorial on how to use these tools. It'll just take a minute or two to
understand the basics, but I go explain more about how to utilize the "modify" tool,
which is very helpful for testing frame windows. Here is the tutorial:
https://youtu.be/WoxXafQNoGQ

----------------------------------------------------------------------------------------

modify.exe reads the parameters from modify.txt to make copies of your .tsv based on
those parameters.

convert.exe reads the .tsv filename from modify.txt and converts .tsv's with that filename
and any with the same filename but "(x)" appended to it, where x is in the range of 0-9.
Each .tsv is converted to a .txt in a format useable with TAS-nx, starting at script0.txt
and counting up from there.

----------------------------------------------------------------------------------------

modify.txt is where you put the lines that you want to insert/overwrite into your scripts.
Each line should be formatted like a .tsv as shown in the example. NOTE: completely blank
lines will count as a blank frame.

settings.txt is the file with settings for you to change.. The lines with dashes at the
front are instructions, and the lines without them are what you change to influence
how the programs run. The recommended advanced settings are the ones I set initially.
Here are the settings explained below:

"File name of .tsv:"
The file name of the .tsv you want to modify and/or convert. It will work with or
without the .tsv extension included. convert.exe will convert all of the following
files, where this setting is set to "example":
"example.tsv", "example(1).tsv", "example(2).tsv", "example(3).tsv", "example(4).tsv", 
"example(5).tsv", "example(6).tsv", "example(7).tsv", "example(8).tsv", "example(9).tsv"

"Do you want to overwrite lines in the existing .tsv or insert between them?"
options: "overwrite", "insert"
"overwrite" will paste the lines in modify.txt over the existing lines. "insert" will
insert the modify.txt lines between the existing ones, and shift all lines below the
insertion point by the appropriate number of frames.

"Frame number to insert/overwrite at:"
Overwriting will begin at the specified frame. For insertion, the existing frame at the
specified number and all frames below it get shift down, and the inserted lines take
their place.

Number of scripts:
Set up to 10. This determines how many total scripts there will be after running
modify.exe. It assumes there is a script0.txt, and then generates new scripts counting up,
up to script9.txt, based on the contents of script0.txt.

--------------------------------------ADVANCED SETTINGS---------------------------------------

"Delete .tsv's upon conversion?"
options: "all", "none", "all except original"
Upon running convert.txt, it will delete the existing .tsv files based on the setting.
"all except original" deletes all except the firstmost .tsv file that exists in the list
described in the "File name of .tsv:" setting above.

"Put .tsv's in a separate folder?"
options: "yes", "no"
If set to "yes", a folder called "tsv" is created if it doesn't exist already, and all
.tsv files read from or written to by modify.exe will be moved to the folder when the
program is ran.

"Put scripts in a separate folder?"
options: "yes", "no"
If set to "yes", a folder called "scripts" is created if it doesn't exist already, and all
script files read from or written to by convert.exe and modify.exe will be moved to the
folder when the program is ran. If set to "yes" and modify.exe is set to modify scripts
instead of .tsv's, then the program will expect a script0.txt file in the "scripts" folder.

"Modify scripts instead of .tsv's i.e. modify after or before conversion?"
options: "script"/"after", ".tsv"/"before"
Set to one of the above four options; do not actually try to set it to '''script/after'''.
"script" and "after" are equivalent, and ".tsv" and "before" are equivalent.
If set to "before", modify.exe will create duplicate .tsv's from the base .tsv file.
If set to "after", the program will instead duplicate scripts from script0.txt. It is
recommended to use the "after" setting, because it creates less clutter of files, as the
duplicate .tsv files are not strictly needed for anything.

----------------------------------------------------------------------------------------

Links:

TAS-nx: https://github.com/hamhub7/TAS-nx

SMO TAS Server: https://discord.gg/mcqjnTS

SMO Speedrunning Server: https://discord.gg/pN4f7Cg
