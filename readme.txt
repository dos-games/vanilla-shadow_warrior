====================================================================
Shadow Warrior (v1.2 CD Version) Source Code Release - April 1, 2005
====================================================================

LEGAL STUFF
-----------

"Shadow Warrior" is a registered trademark of Apogee Software, Ltd. (a.k.a. 3D Realms).
"Shadow Warrior" copyright 1997 - 2005 3D Realms.  All trademarks and copyrights reserved.

This is the complete source code for Shadow Warrior version 1.2, buildable as detailed in the next section.

The code is licensed under the terms of the GPL (gnu public license).  You should read the entire license (filename "GNU.TXT" in this archive), so you understand your rights and what you can and cannot do with the source code from this release.

All of the Shadow Warrior data files remain copyrighted and licensed by 3D Realms under the original terms.  You cannot redistribute our data or data files from the original game.  You can use our data for personal entertainment or educational purposes.  If you need the data, you can order a Shadow Warrior CD from our store on our website (http://www.3drealms.com).

Please do not contact us for possible commercial exploitation of Shadow Warrior -- we will not be interested.

Please note that this is being released without any kind of support from Apogee Software, Ltd / 3D Realms Entertainment.  We cannot help in getting this running and we do not guarantee that you will be able to get it to work, nor do we guarantee that it won't blow up your computer if you do try and use it.  Use at your own risk.

SPECIAL THANKS
--------------

Very large thanks to Frank Maddin (one of the original Shadow Warrior programmers) for digging up the code and providing some help getting it off the ground.  Also thanks to Jonothan Fowler (responsible for the JonoF port of Duke 3D) for lending his experience with the Build Engine to help track down the last major issues, thus making the released source as full featured and bug free as possible.  I couldn't have done it without these guys.

Thanks also to Ken Silverman and Jim Dose for allowing us to include some of their source in this build, so we have it all in one archive.

Finally, thanks to all the fans of 3D Realms and Shadow Warrior.  You guys emailing us over and over is part of what motivated us to finally put this release together!  We can't wait to see what people do with it.  And we really look forward to being able to play the game under XP, with sound, internet play, hardware acceleration and everything else we've seen evolve out of the Duke Nukem 3D source community. :)

I had fun putting this source release together, especially given the lack of a complete source code archive for the game and the challenges that presented.

Charlie Wiederhold
3D Realms

====================================================================
HOW TO COMPILE THE SOURCE CODE
====================================================================

This source code release was compiled on March 31st, 2005 using the materials in this archive.

This code has been updated to allow Shadow Warrior to compile under the free version of the Watcom compiler available from their webpage (http://www.openwatcom.org).

It was compiled under Open Watcom 1.3 for Windows which is free to download. This means, thankfully, that anybody can work with this code right away without trying to find an out of production compiler or wait for someone to port it to other modern compilers.

Step 1) Install Watcom C/C++ onto your system.

Step 2) When you install, make sure that you select DOS 32 Bit as one of your target environments to build for.

Step 3) Choose a place you want to work from and unzip the contents of this .ZIP file into that directory.

From here you must use the IDE to compile (the IDE is the software that manages your files, options for compiling, editing files, debugger, etc). There is a MAKEFILE included in the extras folder, but I didn't spend the time testing it to see if it works, which I seriously doubt.

USING THE WATCOM IDE
--------------------

- Start up the Watcom IDE and go to File -> Open Project.
- Find the directory where you've unzipped the Source files into, and you should see a SW.WPJ. Select this and hit "OK".
- Click the "Make Current Target" button, or press "F4", or go to the menu Targets -> Make. You'll see a couple Warnings as it compiles, that's normal.
- This will create a SW.EXE file in the same directory where the SW.WPJ was located, which can then be copied in the directory with your Shadow Warrior data and run.

ONCE YOU HAVE SW.EXE COMPILED
-----------------------------

- If you own Shadow Warrior version 1.2 (registered): You are set... simply copy your new SW.EXE into the directory and run it.

- If you don't own Shadow Warrior: Download the shareware version 1.2 of Shadow Warrior from http://www.3drealms.com (go to Downloads). Install it and copy your new SW.EXE into the directory and run it.
- This should allow you to play the game well enough to test, though unfortunately there will still be some minor issues and any attempt to play the second episode will result in a crash. Your best results will come from owning the full copy of Shadow Warrior version 1.2, which can still be purchased from the 3D Realms website.

This is enough to get you started. Unfortunately nobody at 3D Realms will be able to devote the time to answer any support questions if you do encounter problems. However, go to http://www.3drealms.com/forums.html and you will find people discussing it in the Shadow Warrior Source category who are also probably able to answer questions. Thanks to the prior release of the Duke 3D Source Code, there is already a wealth of knowledge and experienced people around to help out.

====================================================================
MISC NOTES ABOUT THIS RELEASE
====================================================================

- All changes I made from the original are indicated by a "// CTW" style comment. I made as few changes as possible since the fun for people is working on the Shadow Warrior original code, not my personal rewrite/interpretation. Unfortunately, there was a significant amount of work and hacking needed to get this up and running compared to the Duke 3D release. I was provided with a large list of files with no clue how they needed to be organized, what was actually used, etc. Also, several key files were missing entirely and required borrowing from the Duke 3D source code.

- This source includes the Build Engine data (.OBJ files) needed for compiling. The Build Engine is a separate entity from Shadow Warrior, though Shadow Warrior is built upon the Build Engine.

- The KENBUILD.ZIP file contains all of the data you need from Ken Silverman's released version to compile the Build Engine itself. Instructions for doing this are contained within that file. I have tested this with the free versions of Watcom and it works. More information about this code found at Ken's webpage here: http://www.advsys.net/ken/buildsrc/default.htm

- The AUDIOLIB.ZIP file contains all of the data you need from Jim Dose's sound engine that was used in Rise of the Triad, Duke 3D, and Shadow Warrior to compile the actual Audio Library itself. Instructions for doing this are contained in that file. I have not tested this personally to see if it compiles under the free version of Watcom.

- In order for saving/loading to work, you will need to follow the instructions given at the start of the GAME.C file.

- Sound will be sketchy if you are on any modern system and is likely to slow game performance down to a crawl. I don't advise turning sound on until someone is able to update it to a modern audio system.

- Networking is completely untested. Other minor quirks you might encounter are listed at the top of the GAME.C file.

- If you would like to play Shadow Warrior in high res Vesa modes instead of 320x200, download Ken's utility here: http://www.advsys.net/ken/build.htm

- The files in the EXTRAS folder are there purely for curiosity and educational purposes. You can find a whole lot of potentially interesting tests, as well as older versions of the source files that ultimately made it into the game. None of these are necessary and are purely, well, extra.

- Shadow Warrior used DOS/4GW for its DOS Extender. Watcom 1.3 comes with a couple free DOS Extenders; however you will need to bind it to the extender in order to distribute the EXE you create to other computers where it wasn't compiled. How you do this depends on which extender you choose to use. I trust that once you get to the point of distributing an EXE you can figure out how to bind it to the extender if necessary. Since DOS/4GW was a commercial licensed product, we can't distribute the resources we used to do this.

- All references to TEN (Total Entertainment Network) have been commented out of the version that will be compiled, but left in for the curious to look at. The same goes for the 3DFX Glide support. Any source code files that were written by these respective companies are not included in the release for obvious legal reasons.

- The Duke 3D Source Code has taken on a life of its own and gone beyond what any of us originally imagined. Now, on the two year anniversary of that release, we all look forward to seeing the same support finally added to Shadow Warrior so that the dedicated legion of Lo Wang fans can enjoy the game in an all modern environment. 

- And last but not least... "Whoooo wanta some Wang?!"

