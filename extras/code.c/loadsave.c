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

loadboard(char *filename, long *daposx, long *daposy, long *daposz,
             short *daang, short *dacursectnum)
{
    short fil, i, numsprites;

    i = strlen(filename)-1;
    if (filename[i] == 255) { filename[i] = 0; i = 1; } else i = 0;
    if ((fil = kopen4load(filename,i)) == -1)
        { mapversion = 7L; return(-1); }

    kread(fil,&mapversion,4);
    if (mapversion != 7L) return(-1);

    initspritelists();

    clearbuf((long)(&show2dsector[0]),(long)((MAXSECTORS+3)>>5),0L);
    clearbuf((long)(&show2dsprite[0]),(long)((MAXSPRITES+3)>>5),0L);
    clearbuf((long)(&show2dwall[0]),(long)((MAXWALLS+3)>>5),0L);

    kread(fil,daposx,4);
    kread(fil,daposy,4);
    kread(fil,daposz,4);
    kread(fil,daang,2);
    kread(fil,dacursectnum,2);

    kread(fil,&numsectors,2);
    kread(fil,&sector[0],sizeof(sectortype)*numsectors);

    kread(fil,&numwalls,2);
    kread(fil,&wall[0],sizeof(walltype)*numwalls);

    kread(fil,&numsprites,2);
    kread(fil,&sprite[0],sizeof(spritetype)*numsprites);

    for(i=0;i<numsprites;i++)
        COVERinsertsprite(sprite[i].sectnum,sprite[i].statnum);

        //Must be after loading sectors, etc!
    updatesector(*daposx,*daposy,dacursectnum);

    kclose(fil);
    return(0);
}

saveboard(char *filename, long *daposx, long *daposy, long *daposz,
             short *daang, short *dacursectnum)
{
    short fil, i, j, numsprites;

    if ((fil = open(filename,O_BINARY|O_TRUNC|O_CREAT|O_WRONLY,S_IWRITE)) == -1)
        return(-1);
    write(fil,&mapversion,4);

    write(fil,daposx,4);
    write(fil,daposy,4);
    write(fil,daposz,4);
    write(fil,daang,2);
    write(fil,dacursectnum,2);

    write(fil,&numsectors,2);
    write(fil,&sector[0],sizeof(sectortype)*numsectors);

    write(fil,&numwalls,2);
    write(fil,&wall[0],sizeof(walltype)*numwalls);

    numsprites = 0;
    for(j=0;j<MAXSTATUS;j++)
    {
        i = headspritestat[j];
        while (i != -1)
        {
            numsprites++;
            i = nextspritestat[i];
        }
    }
    write(fil,&numsprites,2);

    for(j=0;j<MAXSTATUS;j++)
    {
        i = headspritestat[j];
        while (i != -1)
        {
            write(fil,&sprite[i],sizeof(spritetype));
            i = nextspritestat[i];
        }
    }

    close(fil);
    return(0);
}

