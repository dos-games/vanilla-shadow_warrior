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

#define MACT_INPUT 0
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include "string.h"
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"

#define M_RED 102
#define M_BLUE 198

int DispMono = TRUE;

//
// KENS setup variables
//

#define MAX_STAG_INFO 1024
typedef struct
    {
    char name[64];
    long flags;
    }STAG_INFO, *STAG_INFOp;
    
STAG_INFO StagInfo[MAX_STAG_INFO];   

Agetnumber256(char namestart[80], short num, long maxnumber);
getnumber256(char namestart[80], short num, long maxnumber);
getnumber16(char namestart[80], short num, long maxnumber);
int printmessage16(char *);
int printmessage256(char *);

#define NUMOPTIONS 8
#define NUMKEYS 19
extern long mapversion;
static long chainxres[4] = {256, 320, 360, 400};
static long chainyres[11] = {200, 240, 256, 270, 300, 350, 360, 400, 480, 512, 540};
static long vesares[13][2] = {320, 200, 360, 200, 320, 240, 360, 240, 320, 400,
    360, 400, 640, 350, 640, 400, 640, 480, 800, 600,
    1024, 768, 1280, 1024, 1600, 1200};
static char option[NUMOPTIONS] = {0, 0, 0, 0, 0, 0, 1, 0};
static char keys[NUMKEYS] =
    {
    0xc8, 0xd0, 0xcb, 0xcd, 0x2a, 0x9d, 0x1d, 0x39,
    0x1e, 0x2c, 0xd1, 0xc9, 0x33, 0x34,
    0x9c, 0x1c, 0xd, 0xc, 0xf,
    };

extern char buildkeys[NUMKEYS];

#define MODE_3D 200

extern short pointhighlight, linehighlight;
extern short defaultspritecstat;
extern long posx, posy, posz;
extern short cursectnum;
extern short ang;
extern long horiz;
extern short asksave;
short ExtSectorTag[MAXSECTORS][4];
static char tempbuf[256];
extern long qsetmode;
extern short searchsector, searchwall, searchstat;
extern long zmode, kensplayerheight;
char ds[256];

typedef enum {
    CAPTION_NONE,
    CAPTION_DEFAULT,
    CAPTION_NAMES,    
    CAPTION_MOST,    
    CAPTION_ALL,
    CAPTION_MAX,
    };
short CaptionMode = CAPTION_NAMES;

short FindPicNum = 0;

// RIGHT ALT selection key
extern short highlightsector[MAXSECTORS];
extern short highlightsectorcnt;

// RIGHT SHIFT selection key
#define SPRITE_FLAG 16384
extern short highlight[MAXWALLS];       // sprite nums are + 16348
extern short highlightcnt;

// Variables copied with the tab key
extern short temppicnum, tempcstat, templotag, temphitag, tempextra;
extern char tempshade, temppal, tempxrepeat, tempyrepeat;
extern char somethingintab;

void SectorMoveFloorZ(long);
void SectorMoveCeilingZ(long);

//printext16 parameters:
//printext16(long xpos, long ypos, short col, short backcol,
//           char name[82], char fontsize)
//  xpos 0-639   (top left)
//  ypos 0-479   (top left)
//  col 0-15
//  backcol 0-15, -1 is transparent background
//  name
//  fontsize 0=8*8, 1=3*5

//drawline16 parameters:
// drawline16(long x1, long y1, long x2, long y2, char col)
//  x1, x2  0-639
//  y1, y2  0-143  (status bar is 144 high, origin is top-left of STATUS BAR)
//  col     0-15

//Detecting 2D / 3D mode:
//   qsetmode is 200 in 3D mode
//   qsetmode is 350/480 in 2D mode
//
//You can read these variables when F5-F8 is pressed in 3D mode only:
//
//   If (searchstat == 0)  WALL        searchsector=sector, searchwall=wall
//   If (searchstat == 1)  CEILING     searchsector=sector
//   If (searchstat == 2)  FLOOR       searchsector=sector
//   If (searchstat == 3)  SPRITE      searchsector=sector, searchwall=sprite
//   If (searchstat == 4)  MASKED WALL searchsector=sector, searchwall=wall
//
//   searchsector is the sector of the selected item for all 5 searchstat's
//
//   searchwall is undefined if searchstat is 1 or 2
//   searchwall is the wall if searchstat = 0 or 4
//   searchwall is the sprite if searchstat = 3 (Yeah, I know - it says wall,
//                                      but trust me, it's the sprite number)

VOID
ResetKeys(VOID)
    {
    int i;

    for (i = 0; i < SIZ(keystatus); i++)
        {
        KEY_PRESSED(i) = 0;
        }
    }

ExtPreCheckKeys(void)
    {
    ToggleFAF();
    }

void
ExtAnalyzeSprites(void)
    {
    long i;
    spritetype *tspr;

    for (i = 0, tspr = &tsprite[0]; i < spritesortcnt; i++, tspr++)
        {
        tspr->shade += 6;
        if (sector[tspr->sectnum].ceilingstat & 1)
            tspr->shade += sector[tspr->sectnum].ceilingshade;
        else
            tspr->shade += sector[tspr->sectnum].floorshade;
        }
    }

BYTEp
KeyPressed(VOID)
    {
    BYTEp k;

    for (k = (BYTEp) & KEY_PRESSED(0); k < &KEY_PRESSED(SIZ(keystatus)); k++)
        {
        if (*k)
            return (k);
        }

    return (NULL);
    }

BYTEp
KeyPressedRange(BYTEp kb, BYTEp ke)
    {
    BYTEp k;

    for (k = kb; k <= ke; k++)
        {
        if (*k)
            return (k);
        }

    return (NULL);
    }

VOID
ResetKeyRange(BYTEp kb, BYTEp ke)
    {
    BYTEp k;

    for (k = kb; k <= ke; k++)
        {
        *k = 0;
        }
    }


void
ExtInit(void)
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
    
    }

void
ExtUnInit(void)
    {
    uninitgroupfile();
    }

VOID
SetSpriteExtra(VOID)
    {
    SPRITEp sp;

#define DEFAULT_SKILL 2

    // for (sp = sprite; sp < &sprite[MAXSPRITES]; sp++)
    for (sp = sprite; sp < &sprite[MAXSPRITES]; sp++)
        {
        if (sp->picnum == ST1 && sp->owner == -1)
            sp->owner = 0;

        if (sp->extra == -1)
            {
            sp->extra = 0;
            SET(sp->extra, DEFAULT_SKILL);
            }
        }
    }

VOID
ResetSpriteFound(VOID)
    {
    SPRITEp sp;

    for (sp = sprite; sp < &sprite[MAXSPRITES]; sp++)
        {
        RESET(sp->extra, SPRX_FOUND);
        }
    }

// imported from allen code
VOID
Keys3D(VOID)
    {
    int i;
    
    if (keystatus[KEYSC_RCTRL]  && keystatus[KEYSC_PGUP] )  
        {
        switch (searchstat)
            {
        case 1:                 // Ceiling
            sector[searchsector].ceilingz -= (1024 * 8);
            keystatus[KEYSC_PGUP] = 0;
            break;
        case 2:                 // Floor
            sector[searchsector].floorz -= (1024 * 8);
            keystatus[KEYSC_PGUP] = 0;
            break;
            }

        }

    if (keystatus[KEYSC_RCTRL]  && keystatus[KEYSC_PGDN] )  
        {
        switch (searchstat)
            {
        case 1:                 // Ceiling
            sector[searchsector].ceilingz += (1024 * 8);
            keystatus[KEYSC_PGDN] = 0;
            break;
        case 2:                 // Floor
            sector[searchsector].floorz += (1024 * 8);
            keystatus[KEYSC_PGDN] = 0;
            break;
            }
        }

    if (!keystatus[KEYSC_ENTER] && !keystatus[KEYSC_QUOTE] && keystatus[KEYSC_R])
        {
        switch (searchstat)
            {
        case 0:
        case 4:                 // Wall
            break;
        case 1:                 // Ceiling
            if (sector[searchsector].ceilingstat & 65)
                Message("Ceiling Relative OFF", M_RED);
            else
                Message("Ceiling Relative ON", M_BLUE);
            break;
        case 2:                 // Floor
            if (sector[searchsector].floorstat & 65)
                Message("Floor Relative OFF", M_RED);
            else
                Message("Floor Relative ON", M_BLUE);
            break;
        case 3:                 // Sprite
            break;
            }
        }


    if (keystatus[KEYSC_QUOTE] && keystatus[KEYSC_ENTER])       // ' ENTER
        {
        printext256(0, 0, 15, 0, "Put Graphic ONLY", 0);
        keystatus[KEYSC_ENTER] = 0;
        switch (searchstat)
            {
        case 0:
            wall[searchwall].picnum = temppicnum;
            break;
        case 1:
            sector[searchsector].ceilingpicnum = temppicnum;
            break;
        case 2:
            sector[searchsector].floorpicnum = temppicnum;
            break;
        case 3:
            sprite[searchwall].picnum = temppicnum;
            break;
        case 4:
            wall[searchwall].overpicnum = temppicnum;
            break;
            }
        }

    if (keystatus[KEYSC_QUOTE] && keystatus[KEYSC_C])   // ' C
        {
        keystatus[KEYSC_C] = 0;
        switch (searchstat)
            {

        case 0:
        case 4:
            for (i = 0; i < MAXWALLS; i++)
                {
                if (wall[i].picnum == temppicnum)
                    {
                    wall[i].shade = tempshade;
                    }
                }
            break;
        case 1:
        case 2:
            for (i = 0; i < MAXSECTORS; i++)
                {
                if (searchstat)
                    if (sector[i].ceilingpicnum == temppicnum)
                        {
                        sector[i].ceilingshade = tempshade;
                        }
                if (searchstat == 2)
                    if (sector[i].floorpicnum == temppicnum)
                        {
                        sector[i].floorshade = tempshade;
                        }
                }
            break;
        case 3:
            for (i = 0; i < MAXSPRITES; i++)
                {
                if (sprite[i].picnum == temppicnum)
                    {
                    sprite[i].shade = tempshade;
                    }
                }
            break;
            }
        }

    if (keystatus[KEYSC_QUOTE] && keystatus[KEYSC_T])   // ' T
        {
        keystatus[KEYSC_T] = 0;
        switch (searchstat)
            {
        case 0:
        case 4:
            strcpy(tempbuf, "Wall lotag: ");
            wall[searchwall].lotag =
                getnumber256(tempbuf, wall[searchwall].lotag, 65536L);
            break;
        case 1:
        case 2:
            strcpy(tempbuf, "Sector lotag: ");
            sector[searchsector].lotag =
                getnumber256(tempbuf, sector[searchsector].lotag, 65536L);
            break;
        case 3:
            strcpy(tempbuf, "Sprite lotag: ");
            sprite[searchwall].lotag =
                getnumber256(tempbuf, sprite[searchwall].lotag, 65536L);
            break;
            }
        }

    if (keystatus[KEYSC_QUOTE] && keystatus[KEYSC_H])   // ' H
        {
        keystatus[KEYSC_H] = 0;
        switch (searchstat)
            {
        case 0:
        case 4:
            strcpy(tempbuf, "Wall hitag: ");
            wall[searchwall].hitag =
                getnumber256(tempbuf, wall[searchwall].hitag, 65536L);
            break;
        case 1:
        case 2:
            strcpy(tempbuf, "Sector hitag: ");
            sector[searchsector].hitag =
                getnumber256(tempbuf, sector[searchsector].hitag, 65536L);
            break;
        case 3:
            strcpy(tempbuf, "Sprite hitag: ");
            sprite[searchwall].hitag =
                getnumber256(tempbuf, sprite[searchwall].hitag, 65536L);
            break;
            }
        }

    if (keystatus[KEYSC_QUOTE] && keystatus[KEYSC_S])   // ' S
        {
        keystatus[KEYSC_S] = 0;
        switch (searchstat)
            {
        case 0:
        case 4:
            strcpy(tempbuf, "Wall shade: ");
            wall[searchwall].shade =
                getnumber256(tempbuf, wall[searchwall].shade, 65536L);
            break;
        case 1:
        case 2:
            strcpy(tempbuf, "Sector shade: ");
            if (searchstat == 1)
                sector[searchsector].ceilingshade =
                    getnumber256(tempbuf, sector[searchsector].ceilingshade, 65536L);
            if (searchstat == 2)
                sector[searchsector].floorshade =
                    getnumber256(tempbuf, sector[searchsector].floorshade, 65536L);
            break;
        case 3:
            strcpy(tempbuf, "Sprite shade: ");
            sprite[searchwall].shade =
                getnumber256(tempbuf, sprite[searchwall].shade, 65536L);
            break;
            }
        }

    if (keystatus[KEYSC_QUOTE] && keystatus[KEYSC_V])   // ' V
        {
        keystatus[KEYSC_V] = 0;
        switch (searchstat)
            {
        case 1:
        case 2:
            strcpy(tempbuf, "Sector visibility: ");
            sector[searchsector].visibility =
                getnumber256(tempbuf, sector[searchsector].visibility, 65536L);
            break;
            }
        }
    ShowMessage();
    }                                   // end Keys3D()

VOID
MoreKeys(short searchstat, short searchwall, short searchsector, short pointhighlight)
    {

    typedef int GET_NUM_FUNC(char *, short, long);
    typedef GET_NUM_FUNC *GET_NUM_FUNCp;
    typedef int PRINT_MSG_FUNC(char *);
    typedef PRINT_MSG_FUNC *PRINT_MSG_FUNCp;
    SPRITEp sp;

    int i;
    int value;

    GET_NUM_FUNCp getnumber;
    PRINT_MSG_FUNCp printmessage;

    if (qsetmode == MODE_3D)
        {
        getnumber = getnumber256;
        printmessage = printmessage256;
        }
    else
        {
        getnumber = getnumber16;
        printmessage = printmessage16;

        if (TEST(pointhighlight, SPRITE_FLAG))
            {
            searchstat = 3;
            searchwall = RESET(pointhighlight, SPRITE_FLAG);
            }
        else
            {
            // for now make search stat invalid
            searchstat = 5;
            }
        }
    
    sp = &sprite[searchwall];    

    if (KEY_PRESSED(KEYSC_RALT + 128) && KEY_PRESSED(KEYSC_RCTRL + 128))
        {
        if (KEY_PRESSED(KEYSC_KPMINUS))
            {
            KEY_PRESSED(KEYSC_KPMINUS) = 0;
            visibility = visibility - (visibility >> 3);

            if (visibility < 0)
                visibility = 16348;
            }
        else if (KEY_PRESSED(KEYSC_KPPLUS))
            {
            KEY_PRESSED(KEYSC_KPPLUS) = 0;
            visibility = visibility + (visibility >> 3);

            if (visibility > 16348)
                visibility = 0;
            }
        }


    if (keystatus[KEYSC_QUOTE])
        {
        if (keystatus[KEYSC_Z])
            {
            long val;
            
            keystatus[KEYSC_Z] = 0;
            
            switch (searchstat)
                {
            case 2:
                val = TEST(sector[searchsector].floorstat, FLOOR_STAT_TYPE_MASK)>>7;            
                val = (val + 1)&3;
                RESET(sector[searchsector].floorstat, FLOOR_STAT_TYPE_MASK);            
                SET(sector[searchsector].floorstat, val<<7);
                sprintf(ds,"Floor Type %d",val);
                Message(ds,0);
                break;
            case 1:
                val = TEST(sector[searchsector].ceilingstat, CEILING_STAT_TYPE_MASK)>>7;            
                val = (val + 1)&3;
                RESET(sector[searchsector].ceilingstat, CEILING_STAT_TYPE_MASK);            
                SET(sector[searchsector].ceilingstat, val<<7);
                sprintf(ds,"Ceiling Type %d",val);
                Message(ds,0);
                break;
                }
            }
            
        if (keystatus[KEYSC_RSHIFT] || keystatus[KEYSC_LSHIFT])
            {
            if (keystatus[KEYSC_1])
                {
                keystatus[KEYSC_1] = 0;
                keystatus[KEYSC_SEMI] = 0;

                switch (searchstat)
                    {
                case 3:
                    sprintf(tempbuf, "Sprite tag 11 (shade) (snum = %d): ",searchwall);
                    SPRITE_TAG11(searchwall) =
                        getnumber(tempbuf, SPRITE_TAG11(searchwall), 65536L);
                    break;
                    }

                printmessage("");
                }
                
            if (keystatus[KEYSC_2])
                {
                keystatus[KEYSC_2] = 0;
                keystatus[KEYSC_SEMI] = 0;

                switch (searchstat)
                    {
                case 3:
                    strcpy(tempbuf, "Sprite tag 12 (pal): ");
                    SPRITE_TAG12(searchwall) =
                        getnumber(tempbuf, SPRITE_TAG12(searchwall), 65536L);
                    break;
                    }

                printmessage("");
                }
                
            if (keystatus[KEYSC_3])
                {
                keystatus[KEYSC_3] = 0;

                switch (searchstat)
                    {
                case 3:
                    strcpy(tempbuf, "Sprite tag 13 (xoffset/yoffset): ");
                    SPRITE_TAG13(searchwall) =
                        getnumber(tempbuf, SPRITE_TAG13(searchwall), 65536L);
                    break;
                    }

                printmessage("");
                }
                
            if (keystatus[KEYSC_4])
                {
                keystatus[KEYSC_4] = 0;

                switch (searchstat)
                    {
                case 3:
                    strcpy(tempbuf, "Sprite tag 14 (xrepeat/yrepeat): ");
                    SPRITE_TAG14(searchwall) =
                        getnumber(tempbuf, SPRITE_TAG14(searchwall), 65536L);
                    break;
                    }

                printmessage("");
                }
                
            if (keystatus[KEYSC_5])
                {
                keystatus[KEYSC_5] = 0;

                switch (searchstat)
                    {
                case 3:
                    strcpy(tempbuf, "Sprite tag 15 (z): ");
                    SPRITE_TAG15(searchwall) =
                        getnumber(tempbuf, SPRITE_TAG15(searchwall), 65536L);
                    break;
                    }

                printmessage("");
                }
            }
            
        if (keystatus[KEYSC_1])
            {
            keystatus[KEYSC_1] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                strcpy(tempbuf, "Wall tag 1 (hitag): ");
                wall[searchwall].hitag =
                    getnumber(tempbuf, wall[searchwall].hitag, 65536L);
                break;
            case 1:
            case 2:
                strcpy(tempbuf, "Sector tag 1 (hitag): ");
                sector[searchsector].hitag =
                    getnumber(tempbuf, sector[searchsector].hitag, 65536L);
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 1 (hitag): ");
                SPRITE_TAG1(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG1(searchwall), 65536L);
                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_2])
            {
            keystatus[KEYSC_2] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                strcpy(tempbuf, "Wall tag 2 (lotag): ");
                wall[searchwall].lotag =
                    getnumber(tempbuf, wall[searchwall].lotag, 65536L);
                break;
            case 1:
            case 2:
                strcpy(tempbuf, "Sector tag 2 (lotag): ");
                sector[searchsector].lotag =
                    getnumber(tempbuf, sector[searchsector].lotag, 65536L);
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 2 (lotag): ");
                SPRITE_TAG2(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG2(searchwall), 65536L);
                break;
                }
            printmessage("");
            }

        if (keystatus[KEYSC_3])
            {
            keystatus[KEYSC_3] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                strcpy(tempbuf, "Wall tag 3 (xpanning): ");
                wall[searchwall].xpanning =
                    getnumber(tempbuf, wall[searchwall].xpanning, 65536L);
                break;
            case 1:
            case 2:
                strcpy(tempbuf, "Sector tag 3 (ceilingxpanning): ");
                sector[searchsector].ceilingxpanning =
                    getnumber(tempbuf, sector[searchsector].ceilingxpanning, 65536L);
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 3 (clipdist) : ");
                SPRITE_TAG3(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG3(searchwall), 65536L);

                break;
                }
            printmessage("");
            }

        if (keystatus[KEYSC_4])
            {
            keystatus[KEYSC_4] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                strcpy(tempbuf, "Wall tag 4 (ypanning): ");
                wall[searchwall].ypanning =
                    getnumber(tempbuf, wall[searchwall].ypanning, 65536L);
                break;
            case 1:
            case 2:
                strcpy(tempbuf, "Sector tag 4 (ceilingypanning): ");
                sector[searchsector].ceilingypanning =
                    getnumber(tempbuf, sector[searchsector].ceilingypanning, 65536L);
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 4 (ang) : ");
                SPRITE_TAG4(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG4(searchwall), 65536L);
                break;
                }
            printmessage("");
            }

        if (keystatus[KEYSC_5])
            {
            keystatus[KEYSC_5] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                break;
            case 1:
            case 2:
                strcpy(tempbuf, "Sector tag 5 (floorxpanning): ");
                sector[searchsector].floorxpanning =
                    getnumber(tempbuf, sector[searchsector].floorxpanning, 65536L);
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 5 (xvel) : ");
                SPRITE_TAG5(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG5(searchwall), 65536L);
                break;
                }
            printmessage("");
            }

        if (keystatus[KEYSC_6])
            {
            keystatus[KEYSC_6] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                break;
            case 1:
            case 2:
                strcpy(tempbuf, "Sector tag 6 (floorypanning): ");
                sector[searchsector].floorypanning =
                    getnumber(tempbuf, sector[searchsector].floorypanning, 65536L);
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 6 (yvel) : ");
                SPRITE_TAG6(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG6(searchwall), 65536L);
                break;
                }
            printmessage("");
            }

        if (keystatus[KEYSC_7])
            {
            keystatus[KEYSC_7] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                break;
            case 1:
            case 2:
                strcpy(tempbuf, "Sector tag 7 (floorypanning): ");
                sector[searchsector].floorypanning =
                    getnumber(tempbuf, sector[searchsector].floorypanning, 65536L);
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 7 (zvel 1) <0-255> : ");
                SPRITE_TAG7(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG7(searchwall), 65536L);
                break;
                }
            printmessage("");
            }

        if (keystatus[KEYSC_8])
            {
            keystatus[KEYSC_8] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                break;
            case 1:
            case 2:
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 8 (zvel 2) <0-255> : ");
                SPRITE_TAG8(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG8(searchwall), 65536L);
                break;
                }
            printmessage("");
            }

        if (keystatus[KEYSC_9])
            {
            keystatus[KEYSC_9] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                break;
            case 1:
            case 2:
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 9 (owner 1) <0-255> : ");
                SPRITE_TAG9(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG9(searchwall), 65536L);
                break;
                }
            printmessage("");
            }

        if (keystatus[KEYSC_0])
            {
            keystatus[KEYSC_0] = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                break;
            case 1:
            case 2:
                break;
            case 3:
                strcpy(tempbuf, "Sprite tag 10 (owner 2) <0-255> : ");
                SPRITE_TAG10(searchwall) =
                    getnumber(tempbuf, SPRITE_TAG10(searchwall), 65536L);
                break;
                }
            printmessage("");
            }
        
        }

    if (!keystatus[KEYSC_SEMI])
        return;

    if (keystatus[KEYSC_RSHIFT] || keystatus[KEYSC_LSHIFT])
        {
        if (keystatus[KEYSC_1])
            {
            keystatus[KEYSC_1] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 11 (0 or 1): ");
                value = !!TEST_BOOL11(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL11(sp);
                else
                    RESET_BOOL11(sp);

                break;
                }

            printmessage("");
            }
        }
    else    
        {
        if (keystatus[KEYSC_1])
            {
            keystatus[KEYSC_1] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 1 (0 or 1): ");
                value = !!TEST_BOOL1(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL1(sp);
                else
                    RESET_BOOL1(sp);

                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_2])
            {
            keystatus[KEYSC_2] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 2 (0 or 1): ");
                value = !!TEST_BOOL2(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL2(sp);
                else
                    RESET_BOOL2(sp);

                break;
                }

            printmessage("");
            }


        if (keystatus[KEYSC_3])
            {
            keystatus[KEYSC_3] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 3 (0 or 1): ");
                value = !!TEST_BOOL3(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL3(sp);
                else
                    RESET_BOOL3(sp);

                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_4])
            {
            keystatus[KEYSC_4] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 4 (0 or 1): ");
                value = !!TEST_BOOL4(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL4(sp);
                else
                    RESET_BOOL4(sp);

                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_5])
            {
            keystatus[KEYSC_5] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 5 (0 or 1): ");
                value = !!TEST_BOOL5(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL5(sp);
                else
                    RESET_BOOL5(sp);

                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_6])
            {
            keystatus[KEYSC_6] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 6 (0 or 1): ");
                value = !!TEST_BOOL6(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL6(sp);
                else
                    RESET_BOOL6(sp);

                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_7])
            {
            keystatus[KEYSC_7] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 7 (0 or 1): ");
                value = !!TEST_BOOL7(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL7(sp);
                else
                    RESET_BOOL7(sp);

                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_8])
            {
            keystatus[KEYSC_8] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 8 (0 or 1): ");
                value = !!TEST_BOOL8(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL8(sp);
                else
                    RESET_BOOL8(sp);

                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_9])
            {
            keystatus[KEYSC_9] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 9 (0 or 1): ");
                value = !!TEST_BOOL9(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL9(sp);
                else
                    RESET_BOOL9(sp);

                break;
                }

            printmessage("");
            }

        if (keystatus[KEYSC_0])
            {
            keystatus[KEYSC_0] = 0;

            switch (searchstat)
                {
            case 3:
                strcpy(tempbuf, "Boolean Sprite tag 10 (0 or 1): ");
                value = !!TEST_BOOL10(sp);
                value = getnumber(tempbuf, value, 65536L);

                if (value)
                    SET_BOOL10(sp);
                else
                    RESET_BOOL10(sp);

                break;
                }

            printmessage("");
            }
        }    
    }



void
ExtCheckKeys(void)
    {
    extern short f_c;

    FAF_AfterDrawRooms();
    
    // try it every time through the loop
    SetSpriteExtra();
    sector[0].extra = visibility;

    if (qsetmode == 200)                // In 3D mode
        {
#define AVERAGEFRAMES 16
        static long frameval[AVERAGEFRAMES], framecnt = 0;
        long i;

        i = totalclock;
        if (i != frameval[framecnt])
            {
            sprintf(tempbuf, "%ld", ((120 * AVERAGEFRAMES) / (i - frameval[framecnt])) + f_c);
            printext256(0L, 0L, 1, -1, tempbuf, 1);
            frameval[framecnt] = i;
            }
        framecnt = ((framecnt + 1) & (AVERAGEFRAMES - 1));

        }

    MoreKeys(searchstat, searchwall, searchsector, pointhighlight);

    if (qsetmode == 200)                // In 3D mode
        {
        Keys3D();

        if (KEY_PRESSED(KEYSC_W))
            {
            KEY_PRESSED(KEYSC_W) = 0;

            switch (searchstat)
                {
            case 0:
            case 4:
                wall[searchwall].picnum = temppicnum;
                break;
            case 1:
                sector[searchsector].ceilingpicnum = temppicnum;
                break;
            case 2:
                sector[searchsector].floorpicnum = temppicnum;
                break;
            case 3:
                sprite[searchwall].picnum = temppicnum;
                break;
                }
            }

        // calling this twice seems to speed up the movement
        editinput();
        editinput();
        }
    else
        {

        if (KEY_PRESSED(KEYSC_QUOTE) && KEY_PRESSED(KEYSC_M))
            {
            KEY_PRESSED(KEYSC_M) = FALSE;
            ShadeMenu();
            }

        // greater than & less than keys
        if (KEY_PRESSED(KEYSC_COMMA) || KEY_PRESSED(KEYSC_PERIOD))
            {
            KEY_PRESSED(KEYSC_COMMA) = KEY_PRESSED(KEYSC_PERIOD) = FALSE;
            FindNextSprite(FindPicNum);
            }

        // c for clip boxes
        // will find out
        if (KEY_PRESSED(KEYSC_QUOTE) && KEY_PRESSED(KEYSC_V))
            {
            KEY_PRESSED(KEYSC_V) = FALSE;
            CaptionMode++;
            if (CaptionMode >= CAPTION_MAX)
                CaptionMode = 0;
            }

        if (keystatus[KEYSC_QUOTE] && keystatus[KEYSC_D])
            {
            keystatus[KEYSC_D] = 0;
            SetClipdist2D();
            }

        }
    }

void
ExtLoadMap(const char *mapname)
    {
    SPRITEp sp;
    int i;
    
    BuildStagTable();

    SetSpriteExtra();

    for (i = 0; i < MAXWALLS; i++)
        {
        if (wall[i].extra != 0)
            wall[i].extra = 0;
        }


#if 0
    Old visibility New visibility
     8->16384
     9->8192
     10->4096
     11->2048
     12->1024
     13->512
     14->256
     15->128
#endif

    // if in valid range set visiblity for the map
    if (sector[0].extra != -1 && sector[0].extra > 0 && sector[0].extra < 16384)
         visibility = sector[0].extra;

    else
        // if NOT in valid range set a default
        visibility = 2;
    }

void
ExtSaveMap(const char *mapname)
    {
    SPRITEp sp;
    int i;

    SetSpriteExtra();
    ResetSpriteFound();

    for (i = 0; i < MAXWALLS; i++)
        {
        if (wall[i].extra != 0)
            wall[i].extra = 0;
        }
    }

const char *
ExtGetSectorCaption(short sectnum)
    {
    if ((sector[sectnum].lotag | sector[sectnum].hitag) == 0)
        {
        tempbuf[0] = 0;
        }
    else
        {
        sprintf(tempbuf, "%d,%d", sector[sectnum].hitag,
            sector[sectnum].lotag);
        }
    return (tempbuf);
    }

const char *
ExtGetWallCaption(short wallnum)
    {
    if ((wall[wallnum].lotag | wall[wallnum].hitag) == 0)
        {
        tempbuf[0] = 0;
        }
    else
        {
        sprintf(tempbuf, "%d,%d", wall[wallnum].hitag,
            wall[wallnum].lotag);
        }
    return (tempbuf);
    }

const char *
ExtGetSpriteCaption(short spritenum)
    {
    SPRITEp sp = &sprite[spritenum];
    CHARp p = "";
    CHAR name[64];
    CHAR tp[30];
    CHAR multi_str[30] = "";

    // Non ST1 sprites that are tagged like them
    if (TEST_BOOL1(sp) && sp->picnum != ST1)
        {
        sprintf(name,"%s,",StagInfo[SP_TAG1(sp)].name);
        p = name;
        }
    else
    switch (sp->picnum)
        {
    case ST1:
        sprintf(name,"*%s,",StagInfo[SP_TAG1(sp)].name);
        p = name;
        break;
    case ST_QUICK_JUMP:
        p = "QJ,";
        break;
    case ST_QUICK_JUMP_DOWN:
        p = "QJD,";
        break;
    case ST_QUICK_SUPER_JUMP:
        p = "QSJ,";
        break;
    case ST_QUICK_EXIT:
        p = "QEXIT,";
        break;
    case ST_QUICK_SCAN:
        p = "QSCAN,";
        break;
    case NINJA_RUN_R0:
        p = "NINJA,";
        break;
    case GORO_RUN_R0:
        p = "GAURD,";
        break;
    case COOLIE_RUN_R0:
        p = "COOLIE,";
        break;
    case COOLG_RUN_R0:
        p = "GHOST,";
        break;
    case RIPPER_RUN_R0:
        p = "RIPPER,";
        break;
    case SKEL_RUN_R0:
        p = "SKEL,";
        break;
    case HORNET_RUN_R0:
        p = "HORNET,";
        break;
    case SERP_RUN_R0:
        p = "SERP,";
        break;
    case SKULL_R0:
        p = "SKULL,";
        break;

    case ICON_STAR:
        p = "STAR,";
        break;
    case ICON_LG_MINE:
        p = "LG_MINE,";
        break;
    case ICON_GRENADE_LAUNCHER:
        p = "GRENADE_LAUNCHER,";
        break;
    case ICON_LG_GRENADE:
        p = "LG_GRENADE,";
        break;
    case ICON_MICRO_GUN:
        p = "MICRO_GUN,";
        break;
    case ICON_MICRO_BATTERY:
        p = "MICRO_BATTERY,";
        break;
    case ICON_LG_SHOTSHELL:
        p = "LG_SHOTSHELL,";
        break;
    case ICON_ROCKET:
        p = "ROCKET,";
        break;
    case ICON_LG_ROCKET:
        p = "LG_ROCKET,";
        break;
    case ICON_UZI:
        p = "UZI,";
        break;
    case ICON_LG_UZI_AMMO:
        p = "LG_UZI_AMMO,";
        break;
    case ICON_GUARD_HEAD:
        p = "FIRE,";
        break;
    case ICON_HEART:
        p = "HEART,";
        break;
    case ICON_HEART_LG_AMMO:
        p = "HEART_LG_AMMO,";
        break;

    case ICON_SPELL:
        p = "SPELL,";
        break;
    case ICON_EXPLOSIVE_BOX:
        p = "CRATE,";
        break;
    case ICON_MEDKIT:
        p = "MEDKIT,";
        break;
    case ICON_ENVIRON_SUIT:
        p = "EVIRON_SUIT,";
        break;
    case ICON_CLOAK:
        p = "SMOKE_BOMB,";
        break;
    case ICON_FLY:
        p = "FLY,";
        break;
    case ICON_GOROAMMO:
        p = "GAURDAMMO,";
        break;
        /*
    case ICON_GAUNTAMMO:
        p = "GAUNTAMMO,";
        break;
        */
    case ICON_RINGAMMO:
        p = "RINGAMMO,";
        break;
    case ICON_NAPALMAMMO:
        p = "NAPALMAMMO,";
        break;
    case ICON_GRENADE:
        p = "GRENADE,";
        break;
    case ICON_NIGHT_VISION:
        p = "NIGHT_VISION,";
        break;
    case ICON_ARMOR:
        p = "ARMOR,";
        break;
    case RED_KEY:
        p = "RED KEY,";
        break;
    case BLUE_KEY:
        p = "BLUE KEY,";
        break;
    case GREEN_KEY:
        p = "GREEN KEY,";
        break;
    case YELLOW_KEY:
        p = "YEL KEY,";
        break;
    case RED_KEY_STATUE:
        p = "RED STAT,";
        break;
    case BLUE_KEY_STATUE:
        p = "BLUE STAT,";
        break;
    case GREEN_KEY_STATUE:
        p = "GREEN STAT,";
        break;
    case YELLOW_KEY_STATUE:
        p = "YEL STAT,";
        break;
    default:
        p = "";
        break;
        }

    // multi
    if (sp->picnum != ST1)
        {
        if (TEST(sp->extra, SPRX_MULTI_ITEM))
            strcpy(multi_str, "MULTI,");
        }    

    // track    
    if (sp->picnum >= TRACK_SPRITE && sp->picnum < TRACK_SPRITE + 100)
        {
        sprintf(tp, "T%d,", sp->picnum - TRACK_SPRITE);
        p = tp;
        }
             
    switch (CaptionMode)
        {
        case CAPTION_NONE:
            tempbuf[0] = NULL;
            break;
            
        case CAPTION_DEFAULT:
            if ((sprite[spritenum].lotag | sprite[spritenum].hitag) == 0)
                tempbuf[0] = NULL;
            else
                sprintf(tempbuf, "%d,%d", sprite[spritenum].hitag, sprite[spritenum].lotag);
            break;
            
            
        case CAPTION_NAMES:
        
            if ((sprite[spritenum].lotag | sprite[spritenum].hitag) == 0)
                sprintf(tempbuf, "%s%s", p, multi_str);
            else
                // name and numbers - name only prints if not null string
                sprintf(tempbuf, "%s%s%d,%d", p, multi_str, sprite[spritenum].hitag, sprite[spritenum].lotag);
                
            break;
            
        case CAPTION_MOST:
            if ((sprite[spritenum].lotag | sprite[spritenum].hitag) == 0)
                sprintf(tempbuf, "%s%s", p, multi_str);
            else    
                sprintf(tempbuf, "%s%s%d,%d,%d,%d,%d,%d", p, multi_str, 
                    SPRITE_TAG1(spritenum),
                    SPRITE_TAG2(spritenum),
                    SPRITE_TAG3(spritenum),
                    SPRITE_TAG4(spritenum),
                    SPRITE_TAG5(spritenum),
                    SPRITE_TAG6(spritenum));
            break;
            
        case CAPTION_ALL:
            if (sp->hitag == SO_CLIP_BOX)
                DrawClipBox(spritenum);
            
            if ((sprite[spritenum].lotag | sprite[spritenum].hitag) == 0)
                sprintf(tempbuf, "%s%s", p, multi_str);
            else    
                sprintf(tempbuf, "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", p, multi_str, 
                    SPRITE_TAG1(spritenum),
                    SPRITE_TAG2(spritenum),
                    SPRITE_TAG3(spritenum),
                    SPRITE_TAG4(spritenum),
                    SPRITE_TAG5(spritenum),
                    SPRITE_TAG6(spritenum),
                    SPRITE_TAG7(spritenum),
                    SPRITE_TAG8(spritenum),
                    SPRITE_TAG9(spritenum),
                    SPRITE_TAG10(spritenum));
            break;
        }
        

    return (tempbuf);
    }

VOID
DrawClipBox(short spritenum)
    {
    long x, y;
    long radius;
    extern long zoom;

    x = mulscale14(sprite[spritenum].x - posx, zoom);
    y = mulscale14(sprite[spritenum].y - posy, zoom);

    x += 320;
    y += 200;

    radius = mulscale14(sprite[spritenum].lotag, zoom);

#define BOX_COLOR 3
    // upper
    drawline16(x - radius, y - radius, x + radius, y - radius, BOX_COLOR);
    // lower
    drawline16(x - radius, y + radius, x + radius, y + radius, BOX_COLOR);
    // left
    drawline16(x - radius, y - radius, x - radius, y + radius, BOX_COLOR);
    // right
    drawline16(x + radius, y - radius, x + radius, y + radius, BOX_COLOR);
    }

void
ExtShowSectorData(short sectnum)        // F5
    {
    if (qsetmode == 200)                // In 3D mode
        return;

    clearmidstatbar16();                // Clear middle of status bar

    sprintf(tempbuf, "Sector %d", sectnum);
    printext16(8, 32, 11, -1, tempbuf, 0);

    printext16(8, 48, 11, -1, "8*8 font: ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789", 0);
    printext16(8, 56, 11, -1, "3*5 font: ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789", 1);

    drawline16(320, 68, 344, 80, 4);    // Draw house
    drawline16(344, 80, 344, 116, 4);
    drawline16(344, 116, 296, 116, 4);
    drawline16(296, 116, 296, 80, 4);
    drawline16(296, 80, 320, 68, 4);
    }

void
ExtShowWallData(short wallnum)          // F6
    {
    if (qsetmode == 200)                // In 3D mode
        return;

    clearmidstatbar16();                // Clear middle of status bar

    sprintf(tempbuf, "Wall %d", wallnum);
    printext16(8, 32, 11, -1, tempbuf, 0);
    }

void
ExtShowSpriteData(short spritenum)      // F6
    {
    if (qsetmode == 200)                // In 3D mode
        return;

    clearmidstatbar16();                // Clear middle of status bar

    sprintf(tempbuf, "Sprite %d", spritenum);
    printext16(8, 32, 11, -1, tempbuf, 0);
    }

void
ExtEditSectorData(short sectnum)        // F7
    {
    short key_num;

    if (qsetmode == 200)                // In 3D mode
        return;

    if (KEY_PRESSED(KEYSC_1))
        {
        sprintf(tempbuf, "Sector (%ld) Extra Tag 1: ", sectnum);
        key_num = 0;
        }
    else if (KEY_PRESSED(KEYSC_2))
        {
        sprintf(tempbuf, "Sector (%ld) Extra Tag 2: ", sectnum);
        key_num = 1;
        }
    else if (KEY_PRESSED(KEYSC_3))
        {
        sprintf(tempbuf, "Sector (%ld) Extra Tag 3: ", sectnum);
        key_num = 2;
        }
    else if (KEY_PRESSED(KEYSC_4))
        {
        sprintf(tempbuf, "Sector (%ld) Extra Tag 4: ", sectnum);
        key_num = 3;
        }
    else
        return;

    ExtSectorTag[sectnum][key_num] = getnumber16(tempbuf, ExtSectorTag[sectnum][key_num], 65536L);

    printmessage16("");                 // Clear message box (top right of
    // status bar)
    ExtShowSectorData(sectnum);
    }

void
ExtEditWallData(short wallnum)          // F8
    {
    short nickdata;

    if (qsetmode == 200)                // In 3D mode
        return;

    sprintf(tempbuf, "Wall (%ld): ", wallnum);
    nickdata = 0;
    nickdata = getnumber16(tempbuf, nickdata, 65536L);

    printmessage16("");                 // Clear message box (top right of
    // status bar)
    ExtShowWallData(wallnum);
    }

void
ExtEditSpriteData(short spritenum)      // F8
    {
    BYTEp key;
    short data;
    SPRITEp sp;

    SetSpriteExtra();

    sp = &sprite[spritenum];

    if (qsetmode == 200)                // In 3D mode
        return;

    clearmidstatbar16();                // Clear middle of status bar
    printext16(8, 32, 11, -1, "(1/F8)   Skill Level", 0);
    printext16(8, 32 + 8, 11, -1, "(3/F10)  Find Sprite", 0);
    printext16(8, 32 + 16, 11, -1, "(4)      Dbug Toggle ", 0);
    printext16(8, 32 + 24, 11, -1, "(5)      Multi-Player Item Toggle", 0);

    while (KEY_PRESSED(KEYSC_F8));

    ResetKeys();

    while ((key = KeyPressed()) == NULL);

    if (key == &KEY_PRESSED(KEYSC_1) || key == &KEY_PRESSED(KEYSC_F8))
        {
        *key = FALSE;

        sprintf(tempbuf, "Sprite (%ld) Skill Level (0-4) : ", spritenum);

        data = TEST(sp->extra, SPRX_SKILL);

        data = getnumber16(tempbuf, data, 65536L);

        if (data > 4)
            data = 4;

        RESET(sp->extra, SPRX_SKILL);
        SET(sp->extra, data);
        }
    else if (key == &KEY_PRESSED(KEYSC_3) || key == &KEY_PRESSED(KEYSC_F10))
        {
        *key = FALSE;
        FindSprite(sprite[spritenum].picnum);
        }
    else if (key == &KEY_PRESSED(KEYSC_4))
        {
        *key = FALSE;

        FLIP(sprite[spritenum].extra, SPRX_BLOCK);
        }
    else if (key == &KEY_PRESSED(KEYSC_5))
        {
        *key = FALSE;

        FLIP(sprite[spritenum].extra, SPRX_MULTI_ITEM);
        }

    printmessage16("");

    clearmidstatbar16();                // Clear middle of status bar

    sprintf(tempbuf, "Skill = %d", TEST(sp->extra, SPRX_SKILL));
    printext16(8, 32, 11, -1, tempbuf, 0);

    sprintf(tempbuf, "Debug = %d", !!TEST(sp->extra, SPRX_BLOCK));
    printext16(8, 32 + 16, 11, -1, tempbuf, 0);

    sprintf(tempbuf, "Multi Item = %d", !!TEST(sp->extra, SPRX_MULTI_ITEM));
    printext16(8, 32 + 24, 11, -1, tempbuf, 0);
    }

void
PlaxSetShade(void)
    {
    short data;
    short shade;
    long i, count = 0;

    if (qsetmode == 200)                // In 3D mode
        return;

    sprintf(tempbuf, "Plax Sky set shade to #: ");
    shade = getnumber16(tempbuf, 0, 65536L);

    if (shade == 0)
        return;

    for (i = 0, count = 0; i < numsectors; i++)
        {
        if (TEST(sector[i].ceilingstat, CEILING_STAT_PLAX))
            {
            sector[i].ceilingshade = shade;
            count++;
            }
        }

    printmessage16("");

    clearmidstatbar16();                // Clear middle of status bar

    sprintf(tempbuf, "%d Plax Sky shades set.", count);
    printext16(8, 32, 11, -1, tempbuf, 0);
    }

void
PlaxAdjustShade(void)
    {
    short data;
    short shade;
    long i, count = 0;

    if (qsetmode == 200)                // In 3D mode
        return;

    sprintf(tempbuf, "Plax Sky adjust shade by (+10000 for negative): ");
    shade = getnumber16(tempbuf, 0, 65536L);

    if (shade == 0)
        return;

    if (shade > 10000)
        {
        shade = -(shade - 10000);
        }


    for (i = 0; i < numsectors; i++)
        {
        if (TEST(sector[i].ceilingstat, CEILING_STAT_PLAX))
            {
            sector[i].ceilingshade += shade;
            }
        }

    printmessage16("");

    clearmidstatbar16();                // Clear middle of status bar

    sprintf(tempbuf, "%d Plax Sky shades adjusted.", count);
    printext16(8, 32, 11, -1, tempbuf, 0);
    }

void
AdjustShade(void)
    {
    short data;
    short shade;
    long i, count;
    short SpriteNum, NextSprite;

    if (qsetmode == 200)                // In 3D mode
        return;

    sprintf(tempbuf, "Adjust amount (+10000 for negative): ");
    shade = getnumber16(tempbuf, 0, 65536L);

    if (shade == 0)
        return;

    if (shade > 10000)
        {
        shade = -(shade - 10000);
        }

    if (highlightsectorcnt > -1)
        {
        short startwall, endwall;
        short i, j;

        for (i = 0; i < highlightsectorcnt; i++)
            {
            sector[highlightsector[i]].floorshade += shade;
            sector[highlightsector[i]].ceilingshade += shade;

            TRAVERSE_SPRITE_SECT(headspritesect[highlightsector[i]], SpriteNum, NextSprite)
                {
                sprite[SpriteNum].shade += shade;
                }

            startwall = sector[highlightsector[i]].wallptr;
            endwall = startwall + sector[highlightsector[i]].wallnum - 1;

            for (j = startwall; j <= endwall; j++)
                {
                if (!TEST(wall[j].extra, 0x1))
                    {
                    SET(wall[j].extra, 0x1);
                    wall[j].shade += shade;
                    }

                if (!TEST(wall[wall[j].nextwall].extra, 0x1))
                    {
                    SET(wall[wall[j].nextwall].extra, 0x1);
                    wall[wall[j].nextwall].shade += shade;
                    }

                }
            }

        for (i = 0; i < MAXWALLS; i++)
            {
            RESET(wall[j].extra, 0x1);
            }

        }
    else
        {
        for (i = count = 0; i < numwalls; i++)
            {
            wall[i].shade += shade;
            }

        for (i = 0; i < numsectors; i++)
            {
            sector[i].floorshade += shade;
            sector[i].ceilingshade += shade;

            TRAVERSE_SPRITE_SECT(headspritesect[i], SpriteNum, NextSprite)
                {
                sprite[SpriteNum].shade += shade;
                }
            }
        }

    printmessage16("");

    clearmidstatbar16();                // Clear middle of status bar
    }


void
SetClipdist2D(void)
    {
    short dist;
    long i;
    short num;

    if (qsetmode == 200)                // In 3D mode
        return;

    if (highlightcnt <= -1)
        return;

    sprintf(tempbuf, "Sprite Clipdist: ");
    dist = getnumber16(tempbuf, 0, 65536L);

    for (i = 0; i < highlightcnt; i++)
        {
        if (TEST(highlight[i], SPRITE_FLAG))
            {
            num = RESET(highlight[i], SPRITE_FLAG);
            sprite[highlight[i]].clipdist = dist;
            }
        }

    printmessage16("");

    clearmidstatbar16();                // Clear middle of status bar
    }

void
AdjustVisibility(void)
    {
    short data;
    short vis;
    long i, count = 0;

    if (qsetmode == 200)                // In 3D mode
        return;

    sprintf(tempbuf, "Adjust non-zero vis sectors by (+10000 for neg): ");
    vis = getnumber16(tempbuf, 0, 65536L);

    if (vis == 0)
        return;

    if (vis > 10000)
        {
        vis = -(vis - 10000);
        }

    if (highlightsectorcnt > -1)
        {
        short i, j;

        for (i = 0; i < highlightsectorcnt; i++)
            {
            if (sector[highlightsector[i]].visibility != 0)
                {
                count++;
                sector[highlightsector[i]].visibility += vis;
                }
            }
        }
    else
        for (i = 0; i < numsectors; i++)
            {
            if (sector[i].visibility != 0)
                {
                count++;
                sector[i].visibility += vis;
                }
            }

    printmessage16("");

    clearmidstatbar16();                // Clear middle of status bar

    sprintf(tempbuf, "%d Vis adjusted.", count);
    printext16(8, 32, 11, -1, tempbuf, 0);
    }

void
FindSprite(short picnum)
    {
    long i, count;
    short SpriteNum, NextSprite;
    SPRITEp sp;

    if (qsetmode == 200)                // In 3D mode
        return;

    if (picnum == 0)
        {
        sprintf(tempbuf, "Find sprite (tile number): ");
        picnum = getnumber16(tempbuf, 0, 65536L);
        }

    FindPicNum = picnum;

    ResetSpriteFound();

    // go to the first one
    for (i = 0; i < numsectors; i++)
        {
        TRAVERSE_SPRITE_SECT(headspritesect[i], SpriteNum, NextSprite)
            {
            sp = &sprite[SpriteNum];

            if (sp->picnum == picnum)
                {
                SET(sp->extra, SPRX_FOUND);
                cursectnum = sp->sectnum;
                posx = sp->x;
                posy = sp->y;
                posz = sp->z - kensplayerheight;
                return;
                }

            }
        }

    printmessage16("");

    clearmidstatbar16();                // Clear middle of status bar
    }

void
FindNextSprite(short picnum)
    {
    long i, count;
    short SpriteNum, NextSprite;
    SPRITEp sp;
    short animlen;

    if (qsetmode == 200)                // In 3D mode
        return;

    for (i = 0; i < numsectors; i++)
        {
        TRAVERSE_SPRITE_SECT(headspritesect[i], SpriteNum, NextSprite)
            {
            sp = &sprite[SpriteNum];

            if (sp->picnum == picnum && !TEST(sp->extra, SPRX_FOUND))
                {
                SET(sp->extra, SPRX_FOUND);
                cursectnum = sp->sectnum;
                posx = sp->x;
                posy = sp->y;
                posz = sp->z - kensplayerheight;
                return;
                }
            }
        }

    printmessage16("");

    clearmidstatbar16();                // Clear middle of status bar
    }


void
ShadeMenu(void)                         // F8
    {
    BYTEp key;

    if (qsetmode == 200)                // In 3D mode
        return;

    clearmidstatbar16();                // Clear middle of status bar
    printext16(8, 32, 11, -1, "(1) Plax Set ", 0);
    printext16(8, 32 + 8, 11, -1, "(2) Plax Adjust ", 0);
    printext16(8, 32 + 16, 11, -1, "(3) Shade Adjust ", 0);
    printext16(8, 32 + 24, 11, -1, "(4) Visibility ", 0);

    ResetKeys();

    while ((key = KeyPressed()) == NULL);

    if (key == &KEY_PRESSED(KEYSC_1))
        {
        *key = FALSE;
        PlaxSetShade();
        }
    else if (key == &KEY_PRESSED(KEYSC_2))
        {
        *key = FALSE;
        PlaxAdjustShade();
        }
    else if (key == &KEY_PRESSED(KEYSC_3))
        {
        *key = FALSE;
        AdjustShade();
        }
    else if (key == &KEY_PRESSED(KEYSC_4))
        {
        *key = FALSE;
        AdjustVisibility();
        }
    }

faketimerhandler()
    {
//    extern long posx,posy,posz,vel,svel,angvel;
//    extern short ang;
    }

//Just thought you might want my getnumber16 code
/*
getnumber16(char namestart[80], short num, long maxnumber)
{
        char buffer[80];
        long j, k, n, danum, oldnum;

        danum = (long)num;
        oldnum = danum;
        while ((KEY_PRESSED(0x1c) != 2) && (KEY_PRESSED(0x1) == 0))  //Enter, ESC
        {
                sprintf(&buffer,"%s%ld_ ",namestart,danum);
                printmessage16(buffer);

                for(j=2;j<=11;j++)                //Scan numbers 0-9
                        if (KEY_PRESSED(j) > 0)
                        {
                                KEY_PRESSED(j) = 0;
                                k = j-1;
                                if (k == 10) k = 0;
                                n = (danum*10)+k;
                                if (n < maxnumber) danum = n;
                        }
                if (KEY_PRESSED(0xe) > 0)    // backspace
                {
                        danum /= 10;
                        KEY_PRESSED(0xe) = 0;
                }
                if (KEY_PRESSED(0x1c) == 1)   //L. enter
                {
                        oldnum = danum;
                        KEY_PRESSED(0x1c) = 2;
                        asksave = 1;
                }
        }
        KEY_PRESSED(0x1c) = 0;
        KEY_PRESSED(0x1) = 0;
        return((short)oldnum);
}
*/

static char messagecolor = 31;
static unsigned short messagedelay = 0;
static char messagebuf[1024];

void
Message(char *string, char color)
    {
    sprintf(messagebuf, string, 0);
    messagedelay = 512;
    messagecolor = color;
    }

void
ShowMessage()
    {
    if (messagedelay < 1)
        return;
    messagedelay--;
    printext256(1 * 4, 1 * 8, 1, 0, messagebuf, 1);
    }

getnumber256(char namestart[80], short num, long maxnumber)
    {
    char buffer[80];
    char neg = 0;
    long j, k, n, danum, oldnum;

    danum = (long) num;
    oldnum = danum;
    if (danum < 0)
        {
        neg = 1;
        danum *= -1;
        }
    while ((keystatus[0x1c] != 2) && (keystatus[0x1] == 0))
        {
        drawrooms(posx, posy, posz, ang, horiz, cursectnum);
        drawmasks();

        if (!neg)
            sprintf(&buffer, "%s%ld_ ", namestart, danum);
        else
            sprintf(&buffer, "%s-%ld_ ", namestart, danum);
        printmessage256(buffer);
        nextpage();

        for (j = 2; j <= 11; j++)
            if (keystatus[j] > 0)
                {
                keystatus[j] = 0;
                k = j - 1;
                if (k == 10)
                    k = 0;
                n = (danum * 10) + k;
                if (n < maxnumber)
                    danum = n;
                }
        if (keystatus[0xe] > 0)         // backspace
            {
            danum /= 10;
            if (danum == 0)
                neg = 0;
            keystatus[0xe] = 0;
            }
        if (keystatus[0xc] > 0)         // neg
            {
            neg = !neg;
            keystatus[0xc] = 0;
            }
        if (keystatus[0x1c] == 1)
            {
            oldnum = danum;
            if (neg)
                oldnum *= -1;
            keystatus[0x1c] = 2;
            asksave = 1;
            }
        }
    keystatus[0x1c] = 0;
    keystatus[0x1] = 0;

    return ((short) oldnum);
    }

getnumber16(char namestart[80], short num, long maxnumber)
    {
    char buffer[80];
    char neg = 0;
    long j, k, n, danum, oldnum;

    danum = (long) num;
    oldnum = danum;
    if (danum < 0)
        {
        neg = 1;
        danum *= -1;
        }
    while ((keystatus[0x1c] != 2) && (keystatus[0x1] == 0))
        {
        if (!neg)
            sprintf(&buffer, "%s%ld_ ", namestart, danum);
        else
            sprintf(&buffer, "%s-%ld_ ", namestart, danum);
        printmessage16(buffer);

        for (j = 2; j <= 11; j++)
            if (keystatus[j] > 0)
                {
                keystatus[j] = 0;
                k = j - 1;
                if (k == 10)
                    k = 0;
                n = (danum * 10) + k;
                if (n < maxnumber)
                    danum = n;
                }
        if (keystatus[0xe] > 0)         // backspace
            {
            danum /= 10;
            if (danum == 0)
                neg = 0;
            keystatus[0xe] = 0;
            }
        if (keystatus[0xc] > 0)         // neg
            {
            neg = !neg;
            keystatus[0xc] = 0;
            }
        if (keystatus[0x1c] == 1)
            {
            oldnum = danum;
            if (neg)
                oldnum *= -1;
            keystatus[0x1c] = 2;
            asksave = 1;
            }
        }
        
    keystatus[0x1c] = 0;
    keystatus[0x1] = 0;
    
    return ((short) oldnum);
    }

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
    
    ResetBuildFAF();

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


    
VOID BuildStagTable(VOID)
    {    
    #define MAKE_STAG_TABLE
    #include "stag.h"
    #undef  MAKE_STAG_TABLE
    }
