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

// mappal.c  A utility to remap how palette.dat chooses it's color mapping

#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <dos.h>
#include <fcntl.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <direct.h>
#include <bios.h>

struct paltype
{
    unsigned char pal[768];
    unsigned short numshades;
    unsigned char shades[32][256];
    unsigned char translucent[256][256];    
};

typedef struct paltype PAL, *PALp;

int numshades = 0;


/*
============================================================================

                        BASIC GRAPHICS

============================================================================
*/

#define PEL_WRITE_ADR   0x3c8
#define PEL_READ_ADR    0x3c7
#define PEL_DATA        0x3c9
#define PEL_MASK        0x3c6

/*
==============
=
= GetPalette
=
= Return an 8 bit / color palette
=
==============
*/

void GetPalette (unsigned char *pal)
{
    int     i;

    outp (PEL_READ_ADR,0);
    for (i=0 ; i<768 ; i++)
        pal[i] = inp (PEL_DATA)<<2;
}

/*
==============
=
= SetPalette
=
= Sets an 8 bit / color palette
=
==============
*/

void SetPalette (unsigned char *pal)
{
    int     i;

    outp (PEL_WRITE_ADR,0);
    for (i=0 ; i<768 ; i++)
        outp (PEL_DATA, pal[i]);
}


void VGAMode (void)
{
    union REGS      regs;       // scratch for int calls

    regs.w.ax = 0x13;
    int386 (0x10,(const union REGS *)&regs,&regs);
}

void TextMode (void)
{
    union REGS      regs;       // scratch for int calls

    regs.w.ax = 0x3;
    int386 (0x10,(const union REGS *)&regs,&regs);
}


size_t read_palette( FILE *fp,  PALp p )
{
    size_t numelems;

    numelems = fread( p, sizeof(PAL), 1, fp );
//  numelems += fread((short *)&numshades, sizeof(short), 1, fp);

    return(numelems);
}

// Shadow Warrior color ranges
char ranges[14] = {1,31,32,32,16,16,16,16,16,16,32,16,15,1};
// Put black as the last color of each range in the first shade range
char maxrange[14] = {0,31,63,95,111,127,143,159,175,191,223,239,254,255};
float uincr[14];
unsigned char rngstart[14] = {0,1 ,32,64,96 ,112,130,144,160,176,192,224,240,255};
unsigned char   rngend[14] = {0,31,63,95,111,127,143,159,175,191,223,239,254,255};


void main()
{
    char buf[80];
    FILE *fp;
    PALp pal;
    int numelems=0;
    unsigned char *screen,*byteimage,temppal[768];;
    short i,j,k=0;


    screen = (char *)0xa0000;

    pal = malloc(sizeof(PAL));
    memset(pal,0,sizeof(PAL));

    byteimage = (char *)malloc((320*200));
    memset(byteimage,0,(320*200));


    VGAMode();

    fp = fopen("palette.dat","rb");

    if( fp == NULL ) 
    {
        printf("File not found!\n");
    }

    numelems = read_palette( fp, pal );
    fclose(fp);
    if(numelems == 0) goto EXIT;

    numshades = 32;
    if(numshades <= 0 || numshades > 255) 
    {
        printf("Invalid shade range!\n");
        TextMode();
        exit(0);
    }

#if 0
    if( numelems != 0 )
    {
        short shadestep,clrndx=0;
        float u,uinc,vinc,vtmp,shadeinc[14];
        unsigned int clrsum1,clrsum2;
        unsigned char skip=0;

        SetPalette(pal->pal);

        // Show current shade array
        k=0;
        for(i=0; i<numshades; i++)
        {
            for(j=0; j<256; j++)
            {
                screen[(k*320)+j] = pal->shades[i][j];
            }
            k+=2;
        }

        // Init vars
        for(i=0; i<14; i++)
        {
            uincr[i] = (float)ranges[i]/(float)numshades;
            shadeinc[i] = 0;
        }

        // Make new shade array
        for(i=0; i<numshades; i++)
        {
            // Color 0 Black, doesn't shade down
            pal->shades[i][0] = 0;

            // Gray shades 31
            uinc = uincr[1]; // This is the number of total incs for color range
            u=1+shadeinc[1];    // Init u to the starting color for this range
            vtmp = 32-u;
            vinc=vtmp/(float)ranges[1];
            for(j=1; j<32; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }           
            shadeinc[1] += uinc;   // This is the current start color for this iteration of loop

            // Dirt browns 32
            uinc = uincr[2]; // This is the number of total incs for color range
            u=32+shadeinc[2];   // Init u to the starting color for this range
            vtmp = 64-u;
            vinc=vtmp/(float)ranges[2];
            for(j=32; j<64; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[2] += uinc;   // This is the current start color for this iteration of loop

            // Reddish browns 32
            uinc = uincr[3]; // This is the number of total incs for color range
            u=64+shadeinc[3];   // Init u to the starting color for this range
            vtmp = 96-u;
            vinc=vtmp/(float)ranges[3];
            for(j=64; j<96; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[3] += uinc;   // This is the current start color for this iteration of loop

            // Dull reds 16
            uinc = uincr[4]; // This is the number of total incs for color range
            u=96+shadeinc[4];   // Init u to the starting color for this range
            vtmp = 112-u;
            vinc=vtmp/(float)ranges[4];
            for(j=96; j<112; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[4] += uinc;   // This is the current start color for this iteration of loop

            // Bright reds 16
            uinc = uincr[5]; // This is the number of total incs for color range
            u=112+shadeinc[5];  // Init u to the starting color for this range
            vtmp = 128-u;
            vinc=vtmp/(float)ranges[5];
            for(j=112; j<128; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[5] += uinc;   // This is the current start color for this iteration of loop

            // Yellows 16
            uinc = uincr[6]; // This is the number of total incs for color range
            u=128+shadeinc[6];  // Init u to the starting color for this range
            vtmp = 144-u;
            vinc=vtmp/(float)ranges[6];
            for(j=128; j<144; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[6] += uinc;   // This is the current start color for this iteration of loop

            // Bright greens 16
//          uinc = (float)ranges[7]/(float)numshades; // This is the number of total incs for color range
            uinc = uincr[7];
            u=144+shadeinc[7];  // Init u to the starting color for this range
            vtmp = 160-u;
            vinc=vtmp/(float)ranges[7];
            for(j=144; j<160; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[7] += uinc;   // This is the current start color for this iteration of loop

            // Olive greens 16
            uinc = uincr[8]; // This is the number of total incs for color range
            u=160+shadeinc[8];  // Init u to the starting color for this range
            vtmp = 176-u;
            vinc=vtmp/(float)ranges[8];
            for(j=160; j<176; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[8] += uinc;   // This is the current start color for this iteration of loop

            // Tree greens 16
            uinc = uincr[9]; // This is the number of total incs for color range
            u=176+shadeinc[9];  // Init u to the starting color for this range
            vtmp = 192-u;
            vinc=vtmp/(float)ranges[9];
            for(j=176; j<192; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[9] += uinc;   // This is the current start color for this iteration of loop

            // Blues 32
            uinc = uincr[10]; // This is the number of total incs for color range
            u=192+shadeinc[10]; // Init u to the starting color for this range
            vtmp = 224-u;
            vinc=vtmp/(float)ranges[10];
            for(j=192; j<224; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[10] += uinc;   // This is the current start color for this iteration of loop

            // Purples 16
            uinc = uincr[11]; // This is the number of total incs for color range
            u=224+shadeinc[11]; // Init u to the starting color for this range
            vtmp = 240-u;
            vinc=vtmp/(float)ranges[11];
            for(j=224; j<240; j++)
            {
                pal->shades[i][j] = (char)u;
                u += vinc;
            }
            shadeinc[11] += uinc;   // This is the current start color for this iteration of loop

            // Fire 15
//          uinc = (float)ranges[12]/(float)numshades; // This is the number of total incs for color range
//          shadeinc[11] += uinc;   // This is the current start color for this iteration of loop
//          u=240+shadeinc[11]; // Init u to the starting color for this range
//          vtmp = 255-u;
//          vinc=vtmp/(float)ranges[12];
            for(j=240; j<255; j++)
            {
                pal->shades[i][j] = pal->shades[0][j];
//              pal->shades[i][j] = (char)u;
//              u += vinc;
            }

            // Color 255 doesn't shade down!
            pal->shades[i][255] = 255; 

            
//          shadeinc += shadestep;  // Step to next color

//          clrndx++;
//          if(clrndx >= 2) 
//              clrndx = 0;
        }




        k = 100;
        // Show array
        for(i=0; i<32; i++)
        {
            for(j=0; j<256; j++)
            {
                screen[(k*320)+j] = pal->shades[i][j];
            }
            k+=2;
        }
    }
    
    getch();
#endif
#if 0
    GetPalette(pal->pal);
    WriteLBMfile("shades.lbm",screen,320,200,pal->pal);
#endif

    LoadLBM("shades.lbm",&byteimage,&temppal);
    VGAMode();
    SetPalette(pal->pal);
    memcpy(screen,byteimage,(320*200));
    getch();

#if 1

    // Read shades
    k = 0;
    for(i=0; i<32; i++)
    {
        unsigned char col;

        for(j=0; j<256; j++)
        {
            col=screen[(k*320)+j];
            pal->shades[i][j]=col;
        }
        k+=2;
    }


    fp = fopen("palette.dat","wb+");

    if( fp == NULL ) 
    {
        printf("Could not create palette.dat!\n");
    }else
    {
        fwrite( pal, sizeof(PAL), 1, fp );
    }

    fclose(fp);
#endif

EXIT:
    TextMode();

}
