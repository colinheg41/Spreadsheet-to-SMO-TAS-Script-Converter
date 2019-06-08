DISCLAIMER: Know your risks of getting your switch banned before connecting
it to the internet while you have homebrew active.

If you have FTP software set up on your switch, you can use this to make
the process of uploading scripts to your switch more efficient. You need
to have WinSCP installed on your computer.

Edit upload_to_switch.bat in a text editor so that the first line has the IP
address of your switch instead of the "xxx.xxx.xxx.xxx". Here is an example
of how to see your switch's IP address in "ftpd snap! 2.3.0":
https://i.imgur.com/TRrwVpN.jpg

In the example above, the IP address is "192.168.137.226". You also need to change
the directory in the first line of upload_to_switch.bat to be the directory
on your computer where the scripts are to be uploaded.

NOTE: Don't include the ":5000" in your switch's IP address; the program
does that automatically.

NOTE: Your switch must have the FTP software open for your computer to connect
to it, including when running this script.

NOTE: Your switch's IP address can change, such as after rebooting your switch.
You may want to check before running upload_to_switch.bat.