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

#ifndef __ENGINE_H
#define __ENGINE_H

#define MAXSECTORS 1024
#define MAXWALLS 4096
#define MAXSPRITES 4096
#define MAXEXTRAS 4096

#define MAXTILES 4096
#define MAXSTATUS 1024
#define MAXANIMATES 512
#define NUMOPTIONS 6
#define NUMKEYS 17
#define XDIM 320
#define YDIM 200
#define MAXPALOOKUPS 256

/***********************************************************************
 * Structures and typedefs
 **********************************************************************/

struct sectortype
{
    unsigned short wallptr;     // +00  id of 1st wall
    unsigned short wallnum;     // +02  number of walls in this sector
    short ceilingpicnum;        // +04
    short floorpicnum;          // +06
    short ceilingheinum;        // +08
    short floorheinum;          // +0A
    long ceilingz;              // +0C
    long floorz;                // +10
    signed char ceilingshade;   // +14
    signed char floorshade;     // +15
    char ceilingxpanning;       // +16
    char floorxpanning;         // +17
    char ceilingypanning;       // +18
    char floorypanning;         // +19
    char ceilingstat;           // +1A
    char floorstat;             // +1B
    char ceilingpal;            // +1C
    char floorpal;              // +1D
    char visibility;            // +1E
    short lotag;                // +1F
    short hitag;                // +21
    short extra;                // +23
};
//ceilingstat, bit 0: 1 = ceiling parallaxing sky, 0 = not               "P"
//             bit 1: 1 = ceiling groudraw, 0 = not (not supported yet)
//             bit 2: 1 = ceiling swap x&y, 0 = not                      "F"
//             bit 3: 1 = pixel-smooshing mode, 0 = tile-expanding mode  "E"
//             bit 4: 1 = ceiling x-flip                                 "F"
//             bit 5: 1 = ceiling y-flip                                 "F"
//             bit 6: 1 = Relative alignment to first point on wall list "R"
//             bit 7: reserved

//floorstat,   bit 0: 1 = floor parallaxing sky, 0 = not                 "P"
//             bit 1: 1 = floor groudraw, 0 = not                        "G"
//             bit 2: 1 = floor swap x&y, 0 = not                        "F"
//             bit 3: 1 = pixel-smooshing mode, 0 = tile-expanding mode  "E"
//             bit 4: 1 = floor x-flip                                   "F"
//             bit 5: 1 = floor y-flip                                   "F"
//             bit 6: 1 = Relative alignment to first point on wall list "R"
//             bit 7: reserved


struct walltype
{
    long x;                     // +00
    long y;                     // +04
    short point2;               // +08
    short picnum;               // +0A
    short overpicnum;           // +0C
    signed char shade;          // +0E
    short cstat;                // +0F
    unsigned char xrepeat;      // +11
    unsigned char yrepeat;      // +12
    unsigned char xpanning;     // +13
    unsigned char ypanning;     // +14
    short nextsector1;          // +15 sector on other side of wall (-1 single sided)
    short nextwall1;            // +17 wall on other side (-1 for single sided)
    short nextsector2;          // +19 (not used?)
    short nextwall2;            // +1B (not used?)
    short lotag;                // +1D
    short hitag;                // +1F
    short extra;                // +21
};
//cstat, bit 0: 1 = clip xoring, 0 = not                              "B"
//       bit 1: 1 = bottoms of invisible walls swapped, 0 = not       "2"
//       bit 2: 1 = align picture on bottom (for doors), 0 = top      "O"
//       bit 3: 1 = x-flipped, 0 = normal                             "F"
//       bit 4: 1 = masking wall, 0 = not                             "M"
//       bit 5: 1 = 1-way wall, 0 = not                               "1"
//       bit 6: 1 = Wall blocks hitscan, 0 = hitscan goes through     "H"
//       bit 7: 1 = Transluscent, 0 = not                             "T"
//       bit 8: 1 = y-flipped, 0 = normal


struct spritetype
{
    long x;                     // +00
    long y;                     // +04
    long z;                     // +08
    char cstat;                 // +0C
    signed char shade;          // +0D
    unsigned char xrepeat;      // +0E
    unsigned char yrepeat;      // +0F
    short picnum;               // +10
    short ang;                  // +12
    short xvel;                 // +14
    short yvel;                 // +16
    short zvel;                 // +18
    short owner;                // +1A
    short sectnum;              // +1C
    short statnum;              // +1E
    short lotag;                // +20
    short hitag;                // +22
    short extra;                // +24
};
//cstat, bit 0: 1 = Sprite blocks, 0 = not blocking                   "B"
//       bit 1: 1 = 50/50 transluscence, 0 = normal
//       bit 2: 1 = x-flipped, 0 = normal                             "F"
//       bit 3: 1 = y-flipped, 0 = normal
//       bit 4: 1 = Rotating sprite (like masked walls), 0 = normal   "R"
//       bit 5: reserved
//       bit 6: reserved
//       bit 7: reserved


struct extratype
{
    short tag[4];
    short point2;
};

#pragma pack(1);
struct RGB
{
    unsigned char r, g, b;
};
#pragma pack;


/***********************************************************************
 * Engine global variables
 **********************************************************************/

extern long angvel;
/* Contains the current angular velocity of the player, in the range -127 to
127, as modified by keytimerstuff(). */

extern int artfil;
/* DOS handle for currently open art file. */

extern char artfilename[];
/* Name of art file, i.e., "TILES000.ART" */

extern int artfilnum;
/* Number of the currently open art file. */

extern long artsize;
/* The total size in bytes of all the tile pieces.  Ken's engine will try to
allocate a cache this large. */

extern long artversion;
/* The current current art version is now 1.  If artversion is not 1 then
either it's the wrong art version or something is wrong.  */

extern long asm1, asm2, asm3, asm4;
/* These are temporary holding registers for the interface from Ken's C code to
his assembly code. I guess he hasn't figured out Watcom's parameter passing
scheme yet. */

extern char automapping;
/* Set to 1 to automatically mark seen walls to appear in the map */

extern short bunchfirst[];
/* Used internally by drawrooms().  Used as indexes into thewall[] */

extern short bunchlast[];
/* Used internally by drawrooms() */

extern long cachelist[];
/* Contains the wall id being used by each cache position */

extern long cachelistend;
/* Contains the index of the last slot used in the tile cache array */

extern long cachelistoffs[];
/* Contains offsets within the tile cache */

extern long cachelistplc;
/* current index into the tile cache array */

extern long cacheplc;
/* current offset into the tile cache */

extern long cachesize;
/* the size of the tile cache */

extern short capturecount;
/* Increments each time a screen capture is done.  Use to sequentially number
capture files. */

extern short chaincount;
/* Used to synchronize two timer interrupt routines with different service
intervals.  At each interrupt, the rate of the slower routine is added to chain
count.  When the chaincount exceeds the rate of the faster routine, the slower
routine is called. */

extern short chainpersecond;
/* The interrupt rate of the user timer interrupt routine installed with
kenchaintimer(), or the 120 Hz clock pulse, whichever is lower */

extern short cliplist[];
/* used by getcliplines() */

extern short clipnum;
/* used by getcliplines() */

extern short clipobjectval[];
/* used by getcliplines() */

extern short clipsectnum;
/* used by getcliplines() */

extern short clipsectorlist[];
/* used by getcliplines() */

extern long clipx1[];
/* used by getcliplines() */

extern long clipx2[];
/* used by getcliplines() */

extern long clipy1[];
/* used by getcliplines() */

extern long clipy2[];
/* used by getcliplines() */

extern long cosglobalang;
/* temporary holder for a cos value */

extern char curbrightness;
/* The current brightness level (0 - 4) */

extern char curpag;
/* used for VESA mode */

extern long dahorizbak;
/* copy of horiz value passed to drawrooms() saved for internal use */

extern long dmost[XDIM];
/* at the beginning of each drawing cycle, startdmost[] gets copied to dmost[] */

extern long dplc[];

extern long dwall[];

extern short editstatus;
/* Set by build.  Alters behavior of certain engine features */

extern char extended;
/* used in key handler to indicate last make code was for extended char */

extern struct extratype extra[MAXEXTRAS];

extern char *frameplace;
/* location of the virtual frame buffer for page 0 */

extern short globalang;
extern long globalbufplc;
extern short globalcursectnum;
extern long globalhoriz;
extern short globalorientation;
extern long globalpal;
extern long globalpalwritten;
extern short globalpicnum;
extern long globalposx;
extern long globalposy;
extern long globalposz;
extern char globalshade;
extern short globalshiftval;
extern long globalx1;
extern long globalx2;
extern long globalxpanning;
extern char globalxshift;
extern long globaly1;
extern long globaly2;
extern long globalypanning;
extern long globalyscale;
extern char globalyshift;
extern long globalzd;
extern long globalzx;

extern char got[MAXSECTORS];

extern char gotpic[MAXTILES >> 3];
/* A bitmapped array of flags for each tile. Not sure how it works. */

extern short headspritesect[MAXSECTORS+1];

extern short headspritestat[MAXSTATUS+1];

extern short highlightcnt;
/* number of points to be highlighted in 2d mode */

extern hitcnt;
/* not used. size unknown */

extern short hitwalls[];

extern char keys[NUMKEYS];

extern volatile char keystatus[256];

extern long lastx[];
/* used internally to sychronize floor and ceiling drawing with walls */

extern short linehighlight;
/* wall to be highlighted in 2d mode */

extern long localtileend;
/* Localtileend is the tile number of the last tile in this art file. */

extern long localtilestart;
/* tile number of the first tile in this art file */

extern long lplc[];

extern long lwall[];

extern long mapversion;
/* version of currently loaded board.  Should be 5 */

extern short maskwall[];
/* list of walls to be drawn in drawmasks().  Value is used as index to
thewall[] and thesector[] */

extern short maskwallcnt;
/* number of walls to be drawn in drawmasks() */

extern short moustat;
/* set to 1 if a mouse is detected */

extern short nextspritesect[MAXSPRITES];

extern short nextspritestat[MAXSPRITES];

extern short numbunches;

extern short numextras;

extern long numframes;

extern short numhits;

extern short numpalookups;
/* number of levels used for depth cueing.  Normal 32, but determined by
palette.dat */

extern short numscans;

extern short numsectors;

extern long numtilefiles;

extern long numtiles;

extern short numwalls;

extern void (__interrupt __far *oldkeyhandler)();

extern char oldreadch;

extern void (__interrupt __far *oldtimerhandler)();

extern void (__interrupt __far *oldzerohandler)();

extern long ooption0;
/* a copy of option[0] - the 3d display mode */

extern char option[NUMOPTIONS];

extern short p2[];
/* used internally for wall drawing.  Creates a linked list of references to
thewall[] */

extern long page;
/* number of the page being rendered */

extern long pageoffset;
/* offset of the active page (in pixels) */

extern struct RGB palette[256];

extern char paletteloaded;
/* Set to 1 after the palette has been loaded. */

extern unsigned char *palookup[MAXPALOOKUPS];
/* The palookup array is an array of pointers that point to the first byte of
each 8K palette lookup table.  All 256 pointers are initialized to NULL by
initengine() except for palookup[0] which is the default 8K palette.  This will
allow you to modify the palette lookup table directly for non-snowy fading
effects, etc.  Each palette lookup table has 32 shades.  Each shade has 256
bytes.  Shade 0 is closest (actual palette brightness) and shade 31 is farthest
(dark usually).  (256*32 = 8192 or 8K) */

extern char parallaxtype;

extern long parallaxyoffs;

extern char pcxheader[];
/* Ken's got his own copy of this structure somewhere in his source file. */

extern unsigned char *pic;
/* Pointer to a large buffer used for the tile cache. Wall offsets are pointers
to locations within this block.  */

extern long picanm[MAXTILES];
/*
  Bit:  ³31           24³23           16³15            8³7             0³
        ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
        ³ | | | | | | | ³ | | | | | | | ³ | | | | | | | ³ | | | | | | | ³
        ÀÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄ´
                ³ Anim. ³  Signed char  ³  Signed char  ³   ³  Animate  ³
                ³ Speed ³   Y-center    ³   X-center    ³   ³   number  ³
                ÀÄÄÄÄÄÄÄ´    offset     ³    offset     ³   ÃÄÄÄÄÄÄÄÄÄÄÄÙ
                        ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´   ÀÄÄÄÄÄÄÄÄÄÄ¿
                                                        ³ Animate type:³
                                                        ³ 00 - NoAnm   ³
                                                        ³ 01 - Oscil   ³
                                                        ³ 10 - AnmFd   ³
                                                        ³ 11 - AnmBk   ³
                                                        ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
 */

extern char picsiz[];

extern short pointhighlight;
/* point to be highlighted in 2d mode */

extern posfil;
/* not used */

extern short prevspritesect[MAXSPRITES];

extern short prevspritestat[MAXSPRITES];

extern long qsetmode;
/* number of scanlines for current 16 color mode (480 or 350) */

extern short radarang[];

extern short radarang2[];
/* table of angles used for sweeping across the screen */

extern long randomseed;

extern char readch;

extern long rx1[];
/* Used for wall drawing */

extern long rx2[];
/* Used for wall drawing */

extern long ry1[];
/* Used for wall drawing */

extern long ry2[];
/* Used for wall drawing */

extern long scaredfallz;
/* used by getcliplines and getzrange.  Initially set to 0x4000.  Tells
monsters what height is too high to fall through.  You can set cliptype
parameter to 2 for move_sprite and clipmove. */

extern char *screen;
/* same as frameplace */

extern short searchit;

extern short searchsector;

extern short searchstat;

extern short searchwall;

extern long searchx;

extern long searchy;

extern struct sectortype sector[MAXSECTORS];

extern short sectorborder[];

extern short sectorbordercnt;

extern char show2dsprite[MAXSPRITES>>3];

extern char show2dwall[MAXWALLS>>3];

extern long singlobalang;

extern short sintable[2048];
/* sin table in 2:14 format */

extern char smalltextfont[1024];

extern short smost[];

extern short smostcnt;

extern short smoststart[];

extern short smostwall[];

extern short smostwallcnt;

extern struct spritetype sprite[MAXSPRITES];

extern char spritepal[MAXSPRITES];
/* palettes of sprites to be drawn in drawmasks() */

extern short spritepicnum[MAXSPRITES];
/* tile ids of sprites to be drawn in drawmasks() */

extern long spritesortcnt;
/* number of sprites to be drawn in drawmasks() */

extern long spritex[];
/* screen x locations of sprites to be drawn in drawmasks() */

extern long spritey[];
/* screen y locations of sprites to be drawn in drawmasks() */

extern long sqrtable[2048];

extern short startang;
/* not used */

extern long startdmost[XDIM];

extern long startposx, startposy, startposz;
/* not used */

extern short startsectnum;
/* not used */

extern long startumost[XDIM];

extern long stereofps;
extern char stereopage;
extern long stereopixelwidth;
extern long stereowidth;

extern long svel;
/* Contains the current strafe (sideways) velocity of the player, in the range
-127 to 127, as modified by keytimerstuff(). */

extern long swall[];

extern long swplc[];

extern char tablesloaded;
/* set to 1 when tables have been loaded.  Duh. */

extern long takernum;
/* Reflects the current activity of the engine; tracked for time statistics in
timetakercount[] */

extern short tantable[2048];
/* not used */

extern char tempbuf[];

extern char textfont[1024];

extern short thesector[];
/* list of sector ids for masked walls to be drawn in drawmasks() */

extern short thesprite[MAXSPRITES];
/* list of sprites to be drawn in drawmasks() */

extern short thewall[];
/* list of wall ids for masked walls to be drawn in drawmasks() */

extern char tilefilenum[MAXTILES];
/* list of art file numbers which contain each tile */

extern long tilefileoffs[MAXTILES];
/* offset of the tile within the art file. */

extern short tilesizx[MAXTILES];

extern short tilesizy[MAXTILES];

extern char timenames[][20];
/* text strings for each of the time statistic engine activities */

extern void (__interrupt __far *timerchainaddress)();
/* pointer to the ISR installed with kenchaintimer() */

extern short timerpersecond;
/* The interrupt rate of the user timer interrupt routine installed with
kenchaintimer(), or the 120 Hz clock pulse, whichever is higher.  This
represents the actually interrupt rate of timerhandler(). */

extern long timetakercount[];
/* Used to track engine statistics.  Each array element contains the total
ticks for each activity. */

extern volatile long totalclock;

extern long totalclocklock;
/* Gets set to totalclock at the beginning of drawrooms(). */

extern unsigned char *transluc;
/* table (256 * 256) for combining translucent colors */

extern char translucloaded;
/* Set to 1 when transluc table is in memory */

extern char tsengdriver;
/* not used */

extern long umost[XDIM];
/* at the beginning of each drawing cycle, startumost[] gets copied to umost[] */

extern long uplc[];

extern long uwall[];

extern long vel;
/* Contains the current forward velocity of the player, in the range -127 to
127, as modified by keytimerstuff(). */

extern char vesapageshift;
/* used for page flipping in VESA mode */

extern char vgapal16[48];
/* REG 16 color palette used for writing 16 color screen captures to PCX files */

extern long viewoffset;
/* Linear offset of the upper left of the 3d view window */

extern char visibility;
/* for special effects such as a gun shooting.  A value from 5 to around 15.
13 is normal, the lower the number, the darker.  */

extern struct walltype wall[MAXWALLS];
/* This is the list of all the walls. */

extern long waloff[MAXTILES];
/* pointers to the raw bitmap data for each tile. */

extern long walsiz[MAXTILES];
/* list containing the size of each tile in bytes. */

extern long windowx1, windowx2, windowy1, windowy2;
/* Window coordinates passed to setview(). */

extern long xb1[];
/* Used internally for wall drawing */

extern long xb2[];
/* Used internally for wall drawing */

extern long xdimen;
/* Width of the 3d view window */

extern long xdimenoffs;
/* x offset of the 3d view window.  Not used. */

extern long xdimenscale;
/* (xdimen << 16) / 320 */

extern long xdimscale;
/* (320 << 16) / xdimen */

extern long yb1[];
/* Used internally for wall drawing */

extern long yb2[];
/* Used internally for wall drawing */

extern long ybot;

extern long ydim16;
/* clip y max for 16 color drawing.  Also determines whether drawing takes
place on the 2d map or on the status bar. */

extern long ydimen;
/* Height of the 3d view window */

extern long ytop;

//*****************

    //These variables are for auto-mapping with the draw2dscreen function.
    //When you load a new board, these bits are all set to 0 - since
    //you haven't mapped out anything yet.  Note that these arrays are
    //bit-mapped.
    //If you want draw2dscreen() to show sprite #54 then you say:
    //   spritenum = 54;
    //   show2dsprite[spritenum>>3] |= (1<<(spritenum&7));
    //And if you want draw2dscreen() to not show sprite #54 then you say:
    //   spritenum = 54;
    //   show2dsprite[spritenum>>3] &= ~(1<<(spritenum&7));

    //Automapping defaults to 0 (do nothing).  If you set automapping to 1,
    //   then in 3D mode, the walls and sprites that you see will show up the
    //   next time you flip to 2D mode.

/*************************************************************************
POSITION VARIABLES:

        POSX is your x - position ranging from 0 to 65535
        POSY is your y - position ranging from 0 to 65535
            (the length of a side of the grid in EDITBORD would be 1024)
        POSZ is your z - position (height) ranging from 0 to 65535, 0 highest.
        ANG is your angle ranging from 0 to 2047.  Instead of 360 degrees, or
             2 * PI radians, I use 2048 different angles, so 90 degrees would
             be 512 in my system.

SPRITE VARIABLES:

    extern short headspritesect[MAXSECTORS+1], headspritestat[MAXSTATUS+1];
    extern short prevspritesect[MAXSPRITES], prevspritestat[MAXSPRITES];
    extern short nextspritesect[MAXSPRITES], nextspritestat[MAXSPRITES];

    Example: if the linked lists look like the following:
         ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
         ³      Sector lists:               Status lists:               ³
         ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
         ³  Sector0:  4, 5, 8             Status0:  2, 0, 8             ³
         ³  Sector1:  16, 2, 0, 7         Status1:  4, 5, 16, 7, 3, 9   ³
         ³  Sector2:  3, 9                                              ³
         ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
    Notice that each number listed above is shown exactly once on both the
        left and right side.  This is because any sprite that exists must
        be in some sector, and must have some kind of status that you define.


Coding example #1:
    To go through all the sprites in sector 1, the code can look like this:

        sectnum = 1;
        i = headspritesect[sectnum];
        while (i != -1)
        {
            nexti = nextspritesect[sectnum];

            //your code goes here
            //ex: printf("Sprite %d is in sector %d\n",i,sectnum);

            i = nexti;
        }

Coding example #2:
    To go through all sprites with status = 1, the code can look like this:

        statnum = 1;        //status 1
        i = headspritestat[statnum];
        while (i != -1)
        {
            nexti = nextspritestat[statnum];

            //your code goes here
            //ex: printf("Sprite %d has a status of 1 (active)\n",i,statnum);

            i = nexti;
        }

             COVERinsertsprite(short sectnum, short statnum);
             deletesprite(short spritenum);
             changespritesect(short spritenum, short newsectnum);
             change_sprite_stat(short spritenum, short newstatnum);

TILE VARIABLES:
        NUMTILES - the number of tiles found TILES.DAT.
        TILESIZX[MAXTILES] - simply the x-dimension of the tile number.
        TILESIZY[MAXTILES] - simply the y-dimension of the tile number.
        WALOFF[MAXTILES] - the actual 32-bit offset pointing to the top-left
                                 corner of the tile.
        PICANM[MAXTILES] - flags for animating the tile.

TIMING VARIABLES:
        TOTALCLOCK - When the engine is initialized, TOTALCLOCK is set to zero.
            From then on, it is incremented 120 times a second by 1.  That
            means that the number of seconds elapsed is totalclock / 120.
        NUMFRAMES - The number of times the draw3dscreen function was called
            since the engine was initialized.  This helps to determine frame
            rate.  (Frame rate = numframes * 120 / totalclock.)

OTHER VARIABLES:

        STARTUMOST[320] is an array of the highest y-coordinates on each column
                that my engine is allowed to write to.  You need to set it only
                once.
        STARTDMOST[320] is an array of the lowest y-coordinates on each column
                that my engine is allowed to write to.  You need to set it only
                once.
        SINTABLE[2048] is a sin table with 2048 angles rather than the
            normal 360 angles for higher precision.  Also since SINTABLE is in
            all integers, the range is multiplied by 16383, so instead of the
            normal -1<sin(x)<1, the range of sintable is -16383<sintable[]<16383
            If you use this sintable, you can possibly speed up your code as
            well as save space in memory.  If you plan to use sintable, 2
            identities you may want to keep in mind are:
                sintable[ang&2047]       = sin(ang * (3.141592/1024)) * 16383
                sintable[(ang+512)&2047] = cos(ang * (3.141592/1024)) * 16383
        OPTION[NUMOPTIONS] - this array is modified by the SETUP.EXE program.
            Right now, option[0] is the graphics mode (0-3)
                          option[1] is the digitized sound (0-5)
                          option[2] is the music (0-1)
                          option[3] is the input type (0-1)
                          option[4] is the serial port (0-2)
            Please look at the SETUP program - the array is set up just like you
                would expect.
        NUMSECTORS - the total number of existing sectors.  Modified every time
            you call the loadboard function.
***************************************************************************/


/***********************************************************************
 * Engine functions
 **********************************************************************/

int addboxlinum( short nSector, short nWall );
/* Called internally by scansector(). */

void allocache( short nTile );
/* Called internally by loadtile() */

long allocatepermanenttile( short tilenume, long xsiz, long ysiz );
/* This function allocates a place on the cache as permanent.  Right now, I
reset the cache every time you call this function so I would recommend calling
this function right after loadpics.  Returns a pointer to the tile bitmap or -1
on failure. */

short animateoffs( short nTile );
/* Returns the current frame for the animating tile based on totalclocklock */

int bunchfront( short, short );
/* Used internally by drawrooms() */

long calcksqrt( long );
/* Used internally to create sqrtable[] */

void calcwalls( short );
/* Used internally by drawrooms() */

int cansee( long x1, long y1, long z1, short sectnum1,long x2, long y2, long z2, short sectnum2 );
/* This function determines whether or not two 3D points can "see" each other
or not.  All you do is pass it the coordinates of a 3D line defined by two 3D
points (with their respective sectors)  The function will return a 1 if the
points can see each other or a 0 if there is something blocking the two points
from seeing each other.  This is how I determine whether a monster can see you
or not. Try playing DOOM1.DAT to fully enjoy this great function! */

void ceilscan( long, long, short );
/* Used internally by calcwalls() */

void changespritesect(short spritenum, short newsectnum);
/* Changes the sector of sprite (spritenum) to the newsector (newsectnum).
This function may become internal to the engine in the move_sprite function.
But this function is necessary since all the sectors have their own
doubly-linked lists of sprites. */

void change_sprite_stat(short spritenum, short newstatnum);
/* Changes the status of sprite (spritenum) to status newstatus).  Newstatus
can be any number from 0 to MAXSTATUS-1.  You can use this function to put a
monster on a list of active sprites when it first sees you. */

void clear2dscreen( void );
/* Clears the drawing page in 16 color mode */

int clipinsidebox( long x1, long y1, long x2, long y2, long walldist );
/* Used in game.c ???  */

void clipmove( long *x, long *y, long *z, short *sectnum, long xvect, long yvect,
    long walldist, long ceildist, long flordist, char cliptype );
/* Moves any object (x, y, z) in any direction at any velocity and will make
sure the object will stay a certain distance from walls (walldist) Pass the
pointers of the starting position (x, y, z).  Then pass the starting position's
sector number as a pointer also.  Also these values will be modified
accordingly.  Pass the direction and velocity by using a vector xvect, yvect).
If you don't fully understand these equations, please call me.  xvect =
velocity * cos(angle) yvect = velocity * sin(angle) Walldist tells how close
the object can get to a wall.  I use 128L as my default.  If you increase
walldist all of a sudden for a certain object, the object might leak through a
wall, so don't do that!  If cliptype is 0, then the clipping is normal (Use 0
to clip you and monsters).  If the cliptype is 1, then the object is clipped to
the same things that hitscan is clipped to (use 1 for all bullets).

Clipmove can either return 0 (touched nothing), 32768+wallnum (wall first
touched), 49152+spritenum (sprite first touched) */

void copytilepiece( long tilenume1, long sourcex1, long sourcey1,
    long xsiz, long ysiz, long tilenume2, long destx1, long desty1 );
/* This function simply copies any section of a source tile to any part of a
destination tile.  It will automatically skip transparent pixels.  It will
wrap-around in the source but not the destination.  If for some reason the
destination tile gets removed from the cache, the destination tile will be
reset to original form.  Use allocatepermanenttile() to prevent the tile from
being purged.  */

void deletesprite( short nSprite );
/* Deletes the sprite. */

void deletespritesect( short nSprite );
/* Remove the sprite from the sector linked list.  Called by deletesprite() */

void deletespritestat( short nSprite );
/* Remove the sprite from the status linked list.  Called by deletesprite() */

void dragpoint( short wallnum, long newx, long newy );
/* This function will drag a point in the exact same way a point is dragged in
2D EDIT MODE using the left mouse button.  Simply pass it which wall to drag
and then pass the new x and y coordinates for that point.  Please use this
function because if you don't and try to drag points yourself, I can guarantee
that it won't work as well as mine and you will get confused.  Note:  Every
wall of course has 2 points.  When you pass a wall number to this function, you
are actually passing 1 point, the left side of the wall given that you are in
the sector of that wall) Got it?  */

void draw2dgrid( long x, long y, short ang, long zoom, short grid );
/* Draws the grid on the 2d screen.  Used internally by draw2dscreen() */

void draw2dscreen( long x, long y, short ang, long zoom, short grid );
/* Draws the 2d screen - this function is a direct replacement for the
drawrooms() and drawmasks() functions.  Be sure to call either qsetmode640350()
or qsetmode640480() first.  When switching back to 3d mode, be sure to call
setgamemode().  IMPORTANT NOTES:  1.  The overwritesprite function should
only be called in 3D mode.  If you do this in 2D mode, junk will be written to
the 2D screen and a crash is possible.  2.  When you switch back to 3D mode,
you should call the permanentwritesprite functions to draw the status bar, or
whatever else you have to draw.  3.  You must call the nextpage() function in
both 2D and 3D modes. */

void drawalls( short );
/* Used internally by drawrooms() */

void drawline16(long x1, long y1, long x2, long y2, char nColor);

void drawmasks( void );
/* This function draws all the sprites and masked walls to the current drawing
page which is not yet shown.  The reason I have the drawing split up into these
2 routines is so you can animate just the sprites that are about to be drawn
instead of having to animate all the sprites on the whole board.  Drawrooms()
prepares these variables:  spritex[], spritey[], spritepicnum[], thesprite[],
and spritesortcnt.  Spritesortcnt is the number of sprites about to be drawn to
the page.  To change the sprite's picnum, simply modify the spritepicnum array
If you want to change other parts of the sprite structure, then you can use the
thesprite array to get an index to the actual sprite number. */

void drawmaskwall( short index );
/* Draws the masked wall referenced in maskwall[index]. */

void drawrooms( long posx, long posy, long posz, short ang, long horiz,
    short cursectnum );
/* This function draws the 3D screen to the current drawing page, which is not
yet shown.  This way, you can overwrite some things over the 3D screen such as
a gun.  Be sure to call the drawmasks() function soon after you call the
drawrooms() function.  To view the screen, use the nextpage() function.  The
nextpage() function should always be called sometime after each draw3dscreen()
function. */

void drawsprite( long x, long y, short nTile, short nSprite, char pal);
/* Draw the specified sprite to the 3d screen.  Called internally by
drawmasks(). */

void florscan( long xb1, long xb2, short );
/* Called internally by calcwalls() */

short getangle( long xvect, long yvect );
/* returns (short)angle; Use this function call to determine the angle between
two points.  For example, if you want a monster to shoot a bullet towards you,
you would get the bullet's angle this way:  sprite[bullet].ang = getangle(posx
- sprite[monst].x, posy - sprite[monst].y); */

void getcliplines( long x, long y, long z, long newX, long newY, short sectnum,
    long walldist, long ceildist, long floordist, char cliptype);
/* generates a list of clip areas for clipmove */

void getmousevalues( short *xMickeys, short *yMickeys, short *buttons );
/* Retrieves mouse info using int 33h, 0Bh and int 33h, 05h */

int getpixel16( long offset );
/* retrieves pixel color addressed by linear pixel number.  Weird.  Used by
screencapture(). */

void getspritescreencoord(short spritesortnum, long *scrx, long *scry);
/* This function returns the actual screen coordinates of a sprite.  It is
useful for locking on to a target.  Use this function between drawrooms and
drawmasks.  Note that spritesortnum is the index into the spritesortcnt arrays,
NOT the normal sprite arrays.  Scrx and scry are actual screen coordinates
ranging from 0-319 and 0-199 respectively. */

void getzrange( long x, long y, long z, short sectnum,
    long *ceilZ, long *ceilHit, long *floorZ, long *floorHit,
    long walldist, char cliptype );
/* Determines the vertical range for an area.  Use this in conjunction with
clipmove.  This function will keep the player from falling off cliffs when
you're too close to the edge.  This function finds the highest and lowest z
coordinates that your clipping BOX can get to.  It must search for all sectors
(and sprites) that go into your clipping box.  This method is better than using
sector[].ceilingz and sector[].floorz because this searches the whole clipping
box for objects, not just 1 point.  Pass x, y, z, sector normally.  Walldist
can be 128.  Cliptype can be 0, 1, or 2. (just like move_sprite and clipmove)
This function returns the z extents in ceilZ and floorZ. It will return the
object hit in ceilHit and floorHit.  CeilHit and floorHit will also be either:
16384+sector (sector first touched) or 49152+spritenum (sprite first touched)
*/


void grouvline( long, long, long, short );
/* used internally by calcwalls() */

void hitscan( long xstart, long ystart, long zstart, short startsectnum,
    long vectorx, long vectory, long vectorz, short *hitsect,
    short *hitwall, short *hitsprite, long *hitx, long *hity, long *hitz );
/* Pass the starting 3D position:  (xstart, ystart, zstart, startsectnum) Then
pass the 3D angle to shoot (defined as a 3D vector):  (vectorx, vectory,
vectorz) Then set up the return values for the object hit:  (hitsect, hitwall,
hitsprite) and the exact 3D point where the ray hits:  (hitx, hity, hitz)

How to determine what was hit:

    Hitsect is always equal to the sector that was hit (always >= 0).

    If the ray hits a sprite then:
        hitsect = thesectornumber
        hitsprite = thespritenumber
        hitwall = -1

    If the ray hits a wall then:
        hitsect = thesectornumber
        hitsprite = -1
        hitwall = thewallnumber

    If the ray hits the ceiling of a sector then:
        hitsect = thesectornumber
        hitsprite = -1
        hitwall = -1
        vectorz < 0 (If vectorz < 0 then you're shooting upward which means
        that you couldn't have hit a floor)

    If the ray hits the floor of a sector then:
        hitsect = thesectornumber
        hitsprite = -1
        hitwall = -1
        vectorz > 0 (If vectorz > 0 then you're shooting downard which means
        that you couldn't have hit a ceiling)
*/

void hline( long, long, long, short );
/* used internally by ceilscan() and floorscan() */

void initcache( void );
/* clears all but permanent tiles from the cache */

void initengine( void );
/* Sets up interrupt vectors for keyboard and timer, and initializes many
variables for the BUILD engine.  You should call this once before any other
functions of the BUILD engine are used. */

void initspritelists( void );
/* Called internally by loadboard().  Creates the linked sector and status
lists based on the sprite sectnum and statnum fields. */

short COVERinsertsprite(short sectnum, short statnum);
/* Returns spritenum.  Whenever you insert a sprite, you must pass it
the sector number, and a status number (statnum).  The status number can be any
number from 0 to MAXSTATUS-1.  Insertsprite works like a memory allocation
function and returns the sprite number.  */

short COVERinsertspritesect( short sectnum );
/* Called internally by COVERinsertsprite().  Adds a sprite to the sector linked
list.  Returns spritenum. */

short COVERinsertspritestat( short statnum );
/* Called internally by COVERinsertsprite().  Adds a sprite to the status linked
list.  Returns spritenum. */

short inside( long x, long y, short sectnum );
/* Tests to see whether the overhead point (x, y) is inside sector (sectnum)
Returns either 0 or 1, where 1 means it is inside, and 0 means it is not. */

void keepaway( long *, long *, long, short );
/* Called internally by clipmove(). */

void kenchaintimer( void (__interrupt __far *datimerchainaddress)(),
    short dachainpersecond );
/* This function makes the engine's timerhandler chain to another timer handler
at any specified interrupt rate.  This function forces IRQ0 to point to my
engine's timerhandler.  Clockspeed and totalclock will be fixed at counting 120
per second regardless of the chaining interrupt rate.  If you call this
function with a NULL pointer, then the engine's timerhandler will not chain
anymore.  */

void __interrupt __far keyhandler( void );
/* Ken's keyboard interrupt routine.  Just say no.  */

void keytimerstuff( void );
/* Gets called during the timer interrupt.  Modifies vel, svel, and angvel
based on whether keys in the keys[] array are pressed. The keys are treated as
accelerators for these values, and the values are damped over time. */

int krand( void );
/* This simply returns a random number.  You can easily set the random seed by
externing the randomseed variable as a long.  This is useful for keeping the
random seed the same on multiple computers when playing multi-player mode. */

short ksqrt( long num );
/* returns square root.  A square root function optimized for integers.  Use
this function only if you want to.  */

short lastwall( short nWall );
/* Returns the number of the wall whose .point equals nWall.  Returns nWall if
no previous wall is found. */

int lintersect( long xa, long ya, long za, long xb, long yb, long zb,
    long xc, long yc, long xd, long yd, long *x, long *y, long *z, char mode );
/* Determines if and where lines ab and cd intersect.  If the lines intersect,
returns 1 and sets x, y, and z to the point of intersection.  Only accounts for
walls, ignores sprites.  If mode == 1, then line ab can be a vector. */

int loadboard( char *filename, long *posx, long *posy, long *posz, short *ang,
    short *cursectnum );
/* Loads the given board file into memory for the BUILD engine.  Returns -1 if
file not found.  If no extension is given, .MAP will be appended to the
filename. */

void loadpalette( void );
/* Loads "PALETTE.DAT".  Gets palette, color lookup tables, and transparency
table. */

int loadpics( char *filename );
/* Loads the given artwork file into memory for the BUILD engine.  Returns -1
if file not found.  If no extension is given, .ART will be appended to the
filename. */

void loadredbluepalette( void );
/* Loads special red/blue palette for stereo vision */

void loadtables( void );
/* Reads "TABLES.DAT" and "SETUP.DAT".  Builds sqrtable[], loads sintable[],
tantable[], radarang[], * textfont, smalltextfont, and setup options. */

void loadtile( short nTile );
/* Loads the specified tile into the cache.  Called internally when
waloff[nTile] == -1 */

void makepalookup( long palnum, char *remapbuf, char red, char green, char blue );
/* This function allows different shirt colors for sprites.  First prepare
remapbuf, which is a 256 byte buffer of chars which the colors to remap.
Palnum can be anywhere from 1-15.  Since 0 is where the normal palette is
stored, it is a bad idea to call this function with palnum = 0.  In BUILD.H
notice I added a new variable, spritepal[MAXSPRITES].  Usually the value of
this is the floorpal the the sector of the sprite.  But if you change it to the
palnum in the code between drawrooms() and drawmasks then the sprite will be
drawn with that remapped palette. The last 3 parameters are the color that the
palette fades to as you get further away.  This color is normally black 0,0,0).
White would be (63,63,63).  This allows you to do fog and other effects.  */

void makeradarang( long width );
/* Builds the radarang2[] array */

void maskwallscan( long xb1, long xb2, long *uwall, long *dwall,
    long *swall, long *lwall );
/* Draws the masked wall.  Called by drawmaskwall() */

int move_sprite(short spritenum, long xchange, long ychange, long zchange,
    long walldist, long ceildist, long flordist, char cliptype, long numtics);
/* This function moves the sprite given by spritenum by the 3 increments,
xchange, ychange, and zchange.  Walldist tells how close the object can get to
a wall.  I use 128L as my default.  If you increase walldist all of a sudden
for a certain object, the object might leak through a wall, so don't do that!
If cliptype is 0, then the clipping is normal (Use 0 to clip you and monsters).
If the cliptype is 1, then the object is clipped to the same things that
hitscan is clipped to (use 1 for all bullets).  Movesprite can either return 0
(touched nothing), 16384+sectnum (ceiling/floor first touched), 32768+wallnum
(wall first touched), 49152+spritenum (sprite first touched). */

void neartag( long x, long y, long z, short sectnum, short ang,  //Starting position & angle
          short *neartagsector,   //Returns near sector if sector[].tag != 0
          short *neartagwall,     //Returns near wall if wall[].tag != 0
          short *neartagsprite,   //Returns near sprite if sprite[].tag != 0
          long *neartaghitdist,   //Returns actual distance to object (scale: 1024=largest grid size)
          long neartagrange );      //Choose maximum distance to scan (scale: 1024=largest grid size)
/* Neartag works sort of like hitscan, but is optimized to scan only close
objects and scan only objects with tags != 0.  Neartag is perfect for the first
line of your space bar code.  It will tell you what door you want to open or
what switch you want to flip. */

void nextpage( void );
/* After a screen is prepared, use this function to view the screen. */

int nextsectorneighborz( short sectnum, long thez, short topbottom, short direction );
/* This function searches z-coordinates of neighboring sectors to find the
closest (next) ceiling starting at the given z-coordinate (thez).  For example,
if you want to find the goal z-coordinate when opening a door, you might want
the door to stop at the next closest neighboring ceiling z-coordinate.  You can
get the z-coordinate this way:

    newz = sector[nextsectorneighborz(sectnum,startz,-1,-1)].ceilingz

    topbottom = -1: search ceilings
                1:  search floors
    direction = -1: search upwards
                1:  search downwards
*/

void overwritesprite(long x, long y, short tilenum, signed char shade,
    char orientation, char dapalnum );
/* Use this function to draw any sprites that must be drawn to the screen for
every single frame, such as a gun or a menu system.

    If Bit 0 of orientation = 0: (x, y) is top-left corner
    If Bit 0 of orientation = 1: (x, y) is middle
    If Bit 1 of orientation = 0: no relation to viewing window
    If Bit 1 of orientation = 1: scale and clip to viewing window
    If Bit 2 of orientation = 0: normal
    If Bit 2 of orientation = 1: 50/50 transluscent!

If it works at full screen, simply set bit 1 of orientation to 1, and it should
automatically scale properly!

Use this function to write sprites over the 3d view.  For example, you can make
a menu system with this function.  Be sure that you call this function for
every single frame after the 3d view is drawn or else it will be flashed on for
only 1 frame.  If you want x and y to be the top left corner, set the
orientation to 0.  If you want x and y to be the middle of the sprite, set the
orientation to 1.  The reason I included the orienation = 1 option is so that
if you want a sprite centered and the size of the tile changes, you don't need
to recompile and guess where the new top left corner is.  Oh yeah, and I forget
to mention that if shade is greater than 32, than overwritesprite does
transluscence.  (Try it out!)  This function will clip the sprite to the
startumost and startdmost arrays.  Dapalnum refers to a palette lookup list
(normally 0).  */

void permanentwritesprite(long thex, long they, short tilenum, signed char shade,
    long cx1, long cy1, long cx2, long cy2 );
/* Added permanentwritesprite function for status bars or other sections of the
screen that will not be overwritten by the engine.  The format of this function
is like overwritesprite except that the x and y are always top left corner, no
orientation variable, and no translucence.  The 4 last parameters (cx1, cy1) -
(cx2, cy2) define a rectangular clipping window of where permanentwritesprite
can draw to.  */

void precache( void );
/* Load all tiles used in the current map into the cache */

void printext16(int x, int y, short nForeColor, short nBackColor, char *text, char nFont);
/* color -1 is transparent, nFont 0=8*8, 1=3*5 */

void printext256(int x, int y, short nForeColor, short nBackColor, char *text, char nFont);
/* color -1 is transparent, nFont 0=8*8, 1=3*5 */

void printext( long x, long y, char buffer[42], short tilenum, char invisiblecol );
/* Use this function to print text anywhere on the screen from a font that you
can create in EDITART.  Please see my example font in TILES.ART to see how I
lay out the user-defined font.  X ranges from 0-319. Y ranges from 0-199.  The
buffer is the string to print.  Tilenum specifies which font to use.
Invisiblecol tells printext what color to draw the transparent pixels.  If
invisiblecol is 255 then the transpararent pixels are still transparent. */

void printmessage16(char *);
/* quick way to display message in the top right of status bar in 2d mode */

void __interrupt __far printscreeninterrupt( void );
/* Gets called when the PrScr key is pressed,  Currently passes it to INT 5 */

void qsetmode640350( void );
/* Set to the 2D map mode #1 (640*350*16) */

void qsetmode640480( void );
/* Set to the 2D map mode #2 (640*480*16) */

int raytrace( long, long, long, long, long *, long * );
/* Used internally by clipmove(). */

void resettiming( void );
/* Resets timing, such as setting totalclock = 0.  Also resets other timers.
This is for use with the showengineinfo */

void rotatepoint( long xpivot, long ypivot, long x, long y, short dAng,
    long *newX, long *newY );
/* Rotatepoint will rotate point(x,y) around point(xpivot,ypivot) by the
dAng value.  The resultant point will be s */

void rotatesprite( long sx, long sy, long z, short a, short picnum, char trans );
/* (sx, sy) is the center of the sprite to draw defined as screen coordinates
shifted up by 16.  (z) is the zoom << 16.  Ex: 0x20000 is zoomed in 2X and
0x8000 is zoomed out 2X.  (a) is the angle (0 is straight up) picnum) is the
tile number.  Rotatesprite clips to the same area as overwritesprite but does
not scale or do transluscence yet.  */

int saveboard( char *filename, long *posx, long *posy, long *posz, short *ang,
    short *cursectnum );
/* Saves the given board from memory inro the specified filename.  Returns -1
if unable to save.  If no extension is given, .MAP will be appended to the
filename. */

void scansector( short nSector );
/* Used internally by drawrooms() and drawalls(). */

void screencapture( void );
/* Capture the screen and save it as a .BMP file.  I don't know why my .BMP
format isn't compatible with other programs.  Because you're lame, Ken! */

void setbrightness(char brightness);
/* Simply call this function where brightness ranges from 0 to 4.  Brightness
defaults to 0.  Levels 1-4 are all brighter than 0.  If you switch between 2D &
3D modes, the engine will remember the current brightness level.  */

void setgamemode( void );
/* This function sets the video mode to 320*200*256color graphics.  Since BUILD
supports several different modes including mode x, mode 13h, and other special
modes, I don't expect you to write any graphics output functions.  Soon I have
all the necessary functions)  If for some reason, you use your own graphics
mode, you must call this function again before using the BUILD drawing
functions. */

void setsprite(short spritenum, long newx, long newy, long newz);
/* This function simply sets the sprite's position to a specified coordinate
newx, newy, newz) without any checking to see whether the position is
valid or not.  You could directly modify the sprite[].x, sprite[].y, and
sprite[].z values, but if you use my function, the sprite is guaranteed to
be in the right sector. */

void setverts( long, long, long, long, long, long );
/* Used internally by stereoinit() */

void setvesamode( long mode );
/* Initializes hires VESA 3d mode */

void setview( long x1, long y1, long x2, long y2 );
/* Change the size of the 3d view window */

void showengineinfo( void );
/* Use this function after setting to text mode to view some statics about the
engine, such as frame rate.  */

int spritewallfront( short nSprite, short nWall );
/* determines z order of sprites and masked walls.  Used internally by
drawmasks(). */

void stereoblit( int page, int screen );
/* flips pages in stereo view mode */

void stereoinit( void );
/* initializes stereo view mode */

void __interrupt __far timerhandler( void );
/* Ken's timer interrupt routine. */

void transmaskvline( long );
/* Used internally by transmaskwallscan(). */

void transmaskwallscan( long xb1, long xb2 );
/* Called by drawmaskwall() for walls that are translucent. */

void uninitengine( void );
/* Restores interrupt vectors for keyboard and timer, and frees buffers.  You
should call this once at the end of the program before quitting to dos. */

void updatesector( long x, long y, short *sectnum );
/* This function updates the sector number according to the x and y values
passed to it.  Be careful when you use this function with sprites because
remember that the sprite's sector number should not be modified directly.  If
you want to update a sprite's sector, I recomment using setsprite(). */

int wallfront( short nWall1, short nWall2);
/* determines wall z order */

int wallmost( long *, long, long );
/* used internally in calcwalls() */

void wallscan( long xb1, long xb2, long *uwall, long *dwall, long *swplc, long *lplc );
/* Draws walls.  Called internally by calcwalls(). */

void __interrupt __far zerohandler( void );
/* Ken's spiffy divide by zero handler */

/***********************************************************************
 * Assembly helper functions
 **********************************************************************/
void fixtransluscence( void * );
/* Fixup self modifying assembly to point to the current translucence table */

#endif

