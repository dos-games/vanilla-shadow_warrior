//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2005 - 3D Realms Entertainment

This file is NOT part of Shadow Warrior version 1.2
However, it is either an older version of a file that is, or is
some test code written during the development of Shadow Warrior.
This file is provided purely for educational interest.

Shadow Warrior is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

void ExtInit(void)
{
    long fil;
    int ch=0;
    
    printf("\n------------------------------------------------------------------------------\n");
    printf("SWBUILD.EXE for Shadow Warrior - ALPHA v0.9\n\n");
    printf("Copyright (c) 1993 - 1996 Ken Silverman, 3D Realms Entertainment.\n");
    printf("This version of SWBUILD was created for Shadow Warrior and parts were modified\n");
    printf("by Frank Maddin, Jim Norwood and Allen H. Blum III.\n");
    printf("\n");
    printf("IMPORTANT:  This editor and associated tools and utilities are NOT\n");
    printf("shareware and may NOT be freely distributed to any BBS, CD, floppy, or\n");
    printf("any other media.  These tools may NOT be sold or repackaged for sale in\n");
    printf("a commercial product.\n");
    printf("\n");
    printf("Any levels created with these editors and tools may only be used with the\n");
    printf("full (registered) copy of Shadow Warrior, and not the shareware version.\n");
    printf("Please refer to LICENSE.DOC for further information on levels created with\n");
    printf("SWBUILD.EXE.\n");
    printf("\n");
    printf("Press <Y> if you have read and accepted the terms of LICENSE.DOC,\n");
    printf("or any other key to abort the program. \n");
    printf("\n");
    ch=getch();

   if(ch=='y' || ch=='Y') // if user press Y
   {

    VOID InitPalette(VOID);
    long i, fil;

    initgroupfile("sw.grp");
    if ((fil = open("setup.dat", O_BINARY | O_RDWR, S_IREAD)) != -1)
        {
        read(fil, &option[0], NUMOPTIONS);
        read(fil, &keys[0], NUMKEYS);
        memcpy((void *) buildkeys, (void *) keys, NUMKEYS);     // Trick to make build
        // use setup.dat keys
        close(fil);
        }
    if (option[4] > 0)
        option[4] = 0;
    initmouse();

    initengine();
    vidoption = option[0];
    xdim = vesares[option[6] & 15][0];
    ydim = vesares[option[6] & 15][1];

    #if 0
    switch (option[0])
        {
    case 0:
        initengine(0, chainxres[option[6] & 15], chainyres[option[6] >> 4]);
        break;
    case 1:
        initengine(1, vesares[option[6] & 15][0], vesares[option[6] & 15][1]);
        break;
    default:
        initengine(option[0], 320L, 200L);
        }
    #endif

    InitPalette();

    defaultspritecstat = 0;             // use old centering mode
    kensplayerheight = 58;
    zmode = 0;

   }// end user press Y
   else
   {
    printf("------------------------------------------------------------------------------\n");
    exit(0);
   }
}
