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

#ifndef GAME_H        

#define GAME_H

#include "mytypes.h"
#include "proto.h"
#include "types.h"
#include "keyboard.h"

#pragma aux dmulscale =\
	"imul ebx",\
	"xchg esi, eax",\
	"mov ebx, edx",\
	"imul edi",\
	"add eax, esi",\
	"adc edx, ebx",\
	"shrd eax, edx, cl",\
	parm [eax][ebx][esi][edi][ecx]\
	modify [edx]\

#pragma aux dmulscale16 =\
	"imul ebx",\
	"xchg esi, eax",\
	"mov ebx, edx",\
	"imul edi",\
	"add eax, esi",\
	"adc edx, ebx",\
	"shrd eax, edx, 16",\
	parm [eax][ebx][esi][edi]\
	modify [edx]\

#pragma aux koutp =\
	"out dx, al",\
	parm [edx][eax]\

#pragma aux koutpw =\
	"out dx, ax",\
	parm [edx][eax]\

#pragma aux kinp =\
	"in al, dx",\
	parm [edx]\

#pragma aux ksgn =\
	"add ebx, ebx",\
	"sbb eax, eax",\
	"cmp eax, ebx",\
	"adc eax, 0",\
	parm [ebx]\

//////////////////////////////

#pragma aux setvmode =\
	"int 0x10",\
	parm [eax]\

#pragma aux copybuf =\
	"rep movsd",\
	parm [esi][edi][ecx]\

#pragma aux klabs =\
	"test eax, eax",\
	"jns skipnegate",\
	"neg eax",\
	"skipnegate:",\
	parm [eax]\

	//eax = (unsigned min)umin(eax,ebx)
#pragma aux umin =\
	"sub eax,ebx",\
	"sbb ecx,ecx",\
	"and eax,ecx",\
	"add eax,ebx",\
	parm [eax][ebx]\
	modify [ecx]\

	//eax = (unsigned max)umax(eax,ebx)
#pragma aux umax =\
	"sub eax,ebx",\
	"sbb ecx,ecx",\
	"not ecx",\
	"and eax,ecx",\
	"add eax,ebx",\
	parm [eax][ebx]\
	modify [ecx]\

#pragma aux mulscale =\
	"imul ebx",\
	"shrd eax, edx, cl",\
	parm [eax][ebx][ecx]\
	modify [edx]\

#pragma aux divscale =\
	"mov edx, eax",\
	"sar edx, 31",\
	"shld edx, eax, cl",\
	"sal eax, cl",\
	"idiv ebx",\
	parm [eax][ebx][ecx]\
	modify [edx]\

#pragma aux drawpixel =\
	"mov byte ptr [edi], al",\
	parm [edi][eax]\


// Turn warning off for unreferenced variables.
// I really should fix them at some point
#pragma off(unreferenced)

#ifdef MAIN
	#define EXTRN
#else
	#define EXTRN extern
#endif

#define DEBUG 1

#if DEBUG
void _Assert(char *, char *, unsigned);
#define ASSERT(f) \
    if (f)        \
	;         \
    else          \
	_Assert(#f,__FILE__,__LINE__);
#else        
#define ASSERT(f)
#endif


//
// Map directions/degrees
//


// old map def

#if FALSE
	       x++
	2047 ^ 0
	     |
	     |
	     |
	     |
	     |
	     |
<---------------------------->
1536         |             512
y--          |             y++
	     |
	     |
	     |
	     |
	     V 1024
	       x--

#endif        

// new map def      

#if FALSE
	       y--
	     ^ 1536
	     |
	     |
	     |
	     |
	     |
	     |            2047
<---------------------------->
1024         |              0 
x--          |             x++
	     |
	     |
	     |
	     |
	     V 512 
	       y++

#endif              
	      
////
//
// Conditional defines
//
////

#define PANEL 0
#define MENU 1  


////
//
// Misc Defines
//
////

    #define LT_GREY         (16 * 0 + 1) 
    #define DK_GREY         (16 * 1)
    #define LT_BROWN        (16 * 2)
    #define DK_BROWN        (16 * 3)
    #define LT_TAN          (16 * 4)
    #define DK_TAN          (16 * 5)
    #define MAUVE           (16 * 6)
    #define RED             (16 * 7)
    #define YELLOW_BROWN    (16 * 8)   
    #define YELLOW_GREEN    (16 * 9)
    #define GREY_GREEN      (16 * 10) 
    #define GREEN           (16 * 11)
    #define BLUE_GREY       (16 * 12)
    #define BLUE            (16 * 13)
    #define GREY            (16 * 14)
    #define YELLOW_ORANGE   (16 * 15)

    //
    // Palette numbers and meanings
    //
    
    #define PALETTE_DEFAULT             0
    // blue sword blade test
    #define PALETTE_SWORD               1
	// for menu highlighting
	#define PALETTE_MENU_HIGHLIGHT          2
    // used for the elector gore pieces
    #define PALETTE_ELECTRO_GORE        3
    // turns ninjas belt and headband red
    #define PALETTE_BASIC_NINJA         4
    // turns ninjas belt and headband red
    #define PALETTE_RED_NINJA           6
    // used for the mother ripper - she is bigger/stronger/brown
    #define PALETTE_BROWN_RIPPER        7
    // used for the red skeletor priest gore
    #define PALETTE_SKEL_GORE           8
    // reserved diving palette this is copied over the default palette
    // when needed - NOTE: could move this to a normal memory buffer if palette
    // slot is needed.
    #define PALETTE_DIVE                9
    #define PALETTE_DIVE_LAVA           10
    // turns ALL colors to shades of GREEN/BLUE/RED
    #define PALETTE_GREEN_LIGHTING      11
    // Turns mask color to black
    #define PALETTE_MASK_TO_BLACK       12
    #define PALETTE_BLUE_LIGHTING       13
    #define PALETTE_RED_LIGHTING        14
    
    // for brown bubbling sludge
    #define PALETTE_SLUDGE              15

    // Player 0 uses default palette - others use these
    // turns ninja's vests (when we get them) into different color ranges
    #define PALETTE_PLAYER0             16
    #define PALETTE_PLAYER1             17
    #define PALETTE_PLAYER2             18
    #define PALETTE_PLAYER3             19
    #define PALETTE_PLAYER4             20
    #define PALETTE_PLAYER5             21
    #define PALETTE_PLAYER6             22
    #define PALETTE_PLAYER7             23
    #define PALETTE_PLAYER8             24
    #define PALETTE_PLAYER9             25
    #define PALETTE_PLAYER10            26
    #define PALETTE_PLAYER11            27    
    #define PALETTE_PLAYER12            28
    #define PALETTE_PLAYER13            29
    #define PALETTE_PLAYER14            30
    #define PALETTE_PLAYER15            31
  
//////////////////////////////////////////////////////    
//    
// KEYBOARD    
//    
//////////////////////////////////////////////////////    

//
// Defines
//

#ifndef MACT_INPUT
    #define MACT_INPUT 1
#endif

#define MACT_CONTROL 1
#define MACT_MOMENTUM 1

#if MACT_INPUT
    #undef  KEYSC_UP
    #define KEYSC_UP        sc_UpArrow
    #undef  KEYSC_DOWN
    #define KEYSC_DOWN      sc_DownArrow
    #undef  KEYSC_LEFT
    #define KEYSC_LEFT      sc_LeftArrow
    #undef  KEYSC_RIGHT
    #define KEYSC_RIGHT     sc_RightArrow
    #undef  KEYSC_INS
    #define KEYSC_INS       sc_Insert
    #undef  KEYSC_DEL
    #define KEYSC_DEL       sc_Delete
    #undef  KEYSC_HOME
    #define KEYSC_HOME      sc_Home
    #undef  KEYSC_END
    #define KEYSC_END       sc_End
    #undef  KEYSC_PGUP
    #define KEYSC_PGUP      sc_PgUp
    #undef  KEYSC_PGDN
    #define KEYSC_PGDN      sc_PgDn
    
    #define KEYSC_RALT      sc_RightAlt
    #define KEYSC_RCTRL     sc_RightControl
    #define KEYSC_KPSLASH   sc_kpad_Slash
    #define KEYSC_KPENTER   sc_kpad_Enter
    #define KEYSC_PRINTSCREEN sc_PrintScreen
    #define KEYSC_LASTSC      sc_LastScanCode
    
    #define KEYSC_KP_1      sc_kpad_1     
    #define KEYSC_KP_2      sc_kpad_2     
    #define KEYSC_KP_3      sc_kpad_3     
    #define KEYSC_KP_4      sc_kpad_4     
    #define KEYSC_KP_6      sc_kpad_6     
    #define KEYSC_KP_5     sc_kpad_5     
    #define KEYSC_KP_7      sc_kpad_7     
    #define KEYSC_KP_8      sc_kpad_8     
    #define KEYSC_KP_9      sc_kpad_9     
    #define KEYSC_KP_0      sc_kpad_0     
    #define KEYSC_KPMINUS  sc_kpad_Minus 
    #define KEYSC_KPPLUS   sc_kpad_Plus  
    #define KEYSC_KPPERIOD sc_kpad_Period
    
    #define KEYSC_EUP        sc_UpArrow
    #define KEYSC_EDOWN      sc_DownArrow
    #define KEYSC_ELEFT      sc_LeftArrow
    #define KEYSC_ERIGHT     sc_RightArrow
    #define KEYSC_EINS       sc_Insert
    #define KEYSC_EDEL       sc_Delete
    #define KEYSC_EHOME      sc_Home
    #define KEYSC_EEND       sc_End
    #define KEYSC_EPGUP      sc_PgUp
    #define KEYSC_EPGDN      sc_PgDn
    
    #undef KB_KeyPressed
    #define KB_KeyPressed( scan ) \
       ( KB_KeyDown[ ( scan ) ])
    #define KEY_PRESSED(sc) KB_KeyPressed((sc))
    #define PKEY_PRESSED(sc) KB_KeyPressed((sc))
#else
    #define KEYSC_RALT      (KEYSC_ALT+128)
    #define KEYSC_RCTRL     (KEYSC_CTRL+128)
    #define KEYSC_KPSLASH   (KEYSC_SLASH+128)
    #define KEYSC_KPENTER   (KEYSC_ENTER+128)
    #define KEYSC_PRINTSCREEN sc_PrintScreen
    #define KEYSC_LASTSC      sc_LastScanCode
    
    #define KEYSC_KP_1       (KEYSC_1+128)      
    #define KEYSC_KP_2       (KEYSC_2+128)      
    #define KEYSC_KP_3       (KEYSC_3+128)      
    #define KEYSC_KP_4       (KEYSC_4+128)      
    #define KEYSC_KP_5       (KEYSC_5+128)      
    #define KEYSC_KP_6       (KEYSC_6+128)      
    #define KEYSC_KP_7       (KEYSC_7+128)      
    #define KEYSC_KP_8       (KEYSC_8+128)      
    #define KEYSC_KP_9       (KEYSC_9+128)      
    #define KEYSC_KP_0       (KEYSC_0+128)      
    #define KEYSC_KPMINUS   (KEYSC_MINUS+128)  
    #define KEYSC_KPPLUS    (KEYSC_PLUS+128)   
    #define KEYSC_KPPERIOD  (KEYSC_PERIOD+128) 
    
    #define KEYSC_EUP        (KEYSC_UP+128)   
    #define KEYSC_EDOWN      (KEYSC_DOWN+128) 
    #define KEYSC_ELEFT      (KEYSC_LEFT+128) 
    #define KEYSC_ERIGHT     (KEYSC_RIGHT+128)
    #define KEYSC_EINS       (KEYSC_INS+128)  
    #define KEYSC_EDEL       (KEYSC_DEL+128)  
    #define KEYSC_EHOME      (KEYSC_HOME+128) 
    #define KEYSC_EEND       (KEYSC_END+128)  
    #define KEYSC_EPGUP      (KEYSC_PGUP+128) 
    #define KEYSC_EPGDN      (KEYSC_PGDN+128) 
    
    #define KEY_PRESSED(sc) keystatus[(sc)]
    #define PKEY_PRESSED(sc) keystatus[(sc)]
#endif
    
//
// NETWORK - REDEFINABLE SHARED (SYNC) KEYS BIT POSITIONS
//
    
    #define SK_RUN        0
    #define SK_SHOOT      1
    #define SK_OPERATE    2
    #define SK_JUMP       3
    #define SK_CRAWL      4
    #define SK_LOOK_UP    5
    #define SK_LOOK_DOWN  6 
    
    #define SK_MASTER_SWITCH 7   
    #define SK_MULTI_VIEW 8 
    
    #define SK_TURN_180   9   
    #define SK_RUN_LOCK   10

    #define SK_MESSAGE    11  
    #define SK_SNAP_UP    12
    #define SK_SNAP_DOWN  13
    #define SK_CRAWL_LOCK 14
    #define SK_FLY        15
    
    // weapons takes up 10 bits
    #define SK_WEAPON     16
    #define SK_WEAPON1    17
    #define SK_WEAPON2    18
    #define SK_WEAPON3    19
    #define SK_WEAPON4    20
    #define SK_WEAPON5    21
    #define SK_WEAPON6    22
    #define SK_WEAPON7    23
    #define SK_WEAPON8    24
    #define SK_WEAPON9    25
    
    #define SK_SPELL_LEFT   26   
    #define SK_SPELL_RIGHT  27
    #define SK_SPELL_NUMBER 28
    #define SK_SPELL_CAST   29
    

    // REDEFINABLE PLAYER KEYS NUMBERS

    #define PK_FORWARD      0
    #define PK_BACKWARD     1
    #define PK_LEFT         2
    #define PK_RIGHT        3
    #define PK_RUN          4
    #define PK_STRAFE       5
    #define PK_SHOOT        6
    #define PK_OPERATE      7
    #define PK_JUMP         8
    #define PK_CRAWL        9
    #define PK_LOOK_UP      10
    #define PK_LOOK_DOWN    11
    #define PK_STRAFE_LEFT  12 
    #define PK_STRAFE_RIGHT 13 
    #define PK_MAP          14  
    #define PK_MULTI_VIEW   15
    #define PK_ZOOM_IN      16  
    #define PK_ZOOM_OUT     17
    #define PK_MESSAGE      18  

//    #define PKEY(num) KEY_PRESSED(keys[num])

// OVER and UNDER water macros    
#define SpriteInDiveArea(sp) (TEST(sector[(sp)->sectnum].extra, SECTFX_DIVE_AREA) ? TRUE : FALSE)   
#define SpriteInUnderwaterArea(sp) (TEST(sector[(sp)->sectnum].extra, SECTFX_UNDERWATER|SECTFX_UNDERWATER2) ? TRUE : FALSE)   

#define SectorIsDiveArea(sect) (TEST(sector[sect].extra, SECTFX_DIVE_AREA) ? TRUE : FALSE)   
#define SectorIsUnderwaterArea(sect) (TEST(sector[sect].extra, SECTFX_UNDERWATER|SECTFX_UNDERWATER2) ? TRUE : FALSE)   
    
// Key Press Flags macros
#define FLAG_KEY_PRESSED(pp,sync_key) TEST(pp->KeyPressFlags,1<<sync_key)
#define FLAG_KEY_RELEASE(pp,sync_key) RESET(pp->KeyPressFlags,1<<sync_key)
#define FLAG_KEY_RESET(pp,sync_key) SET(pp->KeyPressFlags,1<<sync_key)
    
// syncbit manipulation macros
// key_test MUST be a boolean - force it to be
#define SET_SYNC_KEY(player, sync_num, key_test) SET((player)->sync.bits, ((!!(key_test)) << (sync_num)))
#define TEST_SYNC_KEY(player, sync_num) TEST((player)->sync.bits, (1 << (sync_num)))
#define RESET_SYNC_KEY(player, sync_num) RESET((player)->sync.bits, (1 << (sync_num)))

#define TRAVERSE_SPRITE_SECT(l, o, n)    for ((o) = (l); (n) = nextspritesect[o], (o) != -1; (o) = (n))
#define TRAVERSE_SPRITE_STAT(l, o, n)    for ((o) = (l); (n) = nextspritestat[o], (o) != -1; (o) = (n))
#define TRAVERSE_CONNECT(i)   for (i = connecthead; i != -1; i = connectpoint2[i])


#define NORM_ANGLE(ang) (((ang) + 2048) & 2047)
#define ANGLE_2_PLAYER(pp,x,y) (NORM_ANGLE(getangle(pp->posx-(x), pp->posy-(y))))

#define MAP(x,y) Map[(x)>>8][(y)>>8]

#define RANDOM_DEBUG 0 // Set this to 1 for network testing.
#if RANDOM_DEBUG
int RandomRange(int, char*, unsigned);
#define RANDOM_P2(pwr_of_2) (MOD_P2(krand(__FILE__,__LINE__),(pwr_of_2)))
#define RANDOM_RANGE(range) (RandomRange(range,__FILE__,__LINE__))
#define RANDOM() (krand(__FILE__,__LINE__))
#else
int RandomRange(int);
#define RANDOM_P2(pwr_of_2) (MOD_P2(krand(),(pwr_of_2)))
#define RANDOM_RANGE(range) (RandomRange(range))
#define RANDOM() (krand())
#endif

#define MOVEx(vel,ang) (((long)(vel) * (long)sintable[NORM_ANGLE((ang) + 512)]) >> 14)
#define MOVEy(vel,ang) (((long)(vel) * (long)sintable[NORM_ANGLE((ang))]) >> 14)

#define DIST(x1, y1, x2, y2) ksqrt( SQ((x1) - (x2)) + SQ((y1) - (y2)) )

#define PIC_SIZX(sn) (tilesizx[sprite[sn].picnum])
#define PIC_SIZY(sn) (tilesizy[sprite[sn].picnum])

// Distance macro - tx, ty, tmin are holding vars that must be declared in the routine
// that uses this macro
#define DISTANCE(x1, y1, x2, y2, dist, tx, ty, tmin) \
  {                                    \
  tx = labs(x2-x1);                    \
  ty = labs(y2-y1);                    \
  tmin = min(tx,ty);                   \
  dist = tx + ty - DIV2(tmin);         \
  }

#define SPRITE_SIZE_X(sp_num)   ((sprite[sp_num].xrepeat == 64) ?                         \
				    tilesizx[sprite[sp_num].picnum] :                   \
				    ((sprite[sp_num].xrepeat * tilesizx[sprite[sp_num].picnum]) >> 6) \
				)  
				      
#define SPRITE_SIZE_Y(sp_num)   ((sprite[sp_num].yrepeat == 64) ?                          \
				    tilesizy[sprite[sp_num].picnum] :                    \
				    ((sprite[sp_num].yrepeat * tilesizy[sprite[sp_num].picnum]) >> 6) \
				)       
				    
#define SPRITE_SIZE_Z(sp_num)   ((sprite[sp_num].yrepeat == 64) ?                          \
				    Z(tilesizy[sprite[sp_num].picnum]) :                 \
				    ((sprite[sp_num].yrepeat * tilesizy[sprite[sp_num].picnum]) << 2) \
				)      

#define SPRITEp_SIZE_X(sp)   (((sp)->xrepeat == 64) ?                         \
				    tilesizx[(sp)->picnum] :                   \
				    (((sp)->xrepeat * tilesizx[(sp)->picnum]) >> 6) \
				)  
				      
#define SPRITEp_SIZE_Y(sp)   (((sp)->yrepeat == 64) ?                          \
				    tilesizy[(sp)->picnum] :                    \
				    (((sp)->yrepeat * tilesizy[(sp)->picnum]) >> 6) \
				)       
				    
#define SPRITEp_SIZE_Z(sp)   (((sp)->yrepeat == 64) ?                          \
				    Z(tilesizy[(sp)->picnum]) :                 \
				    (((sp)->yrepeat * tilesizy[(sp)->picnum]) << 2) \
				)      

// Given a z height and sprite return the correct y repeat value                 
#define SPRITEp_SIZE_X_2_XREPEAT(sp, x) ((zh*64)/tilesizx[(sp)->picnum]) 
// Given a z height and sprite return the correct y repeat value                 
#define SPRITEp_SIZE_Z_2_YREPEAT(sp, zh) ((zh)/(4*tilesizy[(sp)->picnum])) 
#define SPRITEp_SIZE_Y_2_YREPEAT(sp, y) ((y*64)/tilesizy[(sp)->picnum]) 
				

// x & y offset of tile
#define TILE_XOFF(picnum) ((CHAR)TEST(picanm[(picnum)] >> 8, 0xFF))
#define TILE_YOFF(picnum) ((CHAR)TEST(picanm[(picnum)] >> 16, 0xFF))

// x & y offset of current sprite tile
#define SPRITEp_XOFF(sp) ((CHAR)TEST(picanm[(sp)->picnum] >> 8, 0xFF))
#define SPRITEp_YOFF(sp) ((CHAR)TEST(picanm[(sp)->picnum] >> 16, 0xFF))

// Z size of top (TOS) and bottom (BOS) part of sprite
#define SPRITEp_SIZE_TOS(sp) (DIV2(SPRITEp_SIZE_Z(sp)) + Z(SPRITEp_YOFF(sp)))
#define SPRITEp_SIZE_BOS(sp) (DIV2(SPRITEp_SIZE_Z(sp)) - Z(SPRITEp_YOFF(sp)))

// acual Z for TOS and BOS - handles both WYSIWYG and old style
#define SPRITEp_TOS(sp) (TEST((sp)->cstat, CSTAT_SPRITE_YCENTER) ? \
			((sp)->z - SPRITEp_SIZE_TOS(sp)) :         \
			((sp)->z - SPRITEp_SIZE_Z(sp)))

#define SPRITEp_BOS(sp) (TEST((sp)->cstat, CSTAT_SPRITE_YCENTER) ? \
			((sp)->z + SPRITEp_SIZE_BOS(sp)) :         \
			(sp)->z)
				
#define Z(value) ((long)(value) << 8)
#define PIXZ(value) ((long)(value) >> 8)

#define SQ(val) ((val) * (val))

#define FACING_PLAYER(pp,sp) (labs(GetDeltaAngle((sp)->ang, NORM_ANGLE(getangle((pp)->posx - (sp)->x, (pp)->posy - (sp)->y)))) < 512)
#define PLAYER_FACING(pp,sp) (labs(GetDeltaAngle((pp)->pang, NORM_ANGLE(getangle((sp)->x - (pp)->posx, (sp)->y - (pp)->posy)))) < 320)
#define FACING(sp1,sp2) (labs(GetDeltaAngle((sp)->ang, NORM_ANGLE(getangle((sp1)->x - (sp2)->x, (sp1)->y - (sp2)->y)))) < 512)

#define FACING_PLAYER_RANGE(pp,sp,range) (labs(GetDeltaAngle((sp)->ang, NORM_ANGLE(getangle((pp)->posx - (sp)->x, (pp)->posy - (sp)->y)))) < (range))
#define PLAYER_FACING_RANGE(pp,sp,range) (labs(GetDeltaAngle((pp)->pang, NORM_ANGLE(getangle((sp)->x - (pp)->posx, (sp)->y - (pp)->posy)))) < (range))
#define FACING_RANGE(sp1,sp2,range) (labs(GetDeltaAngle((sp)->ang, NORM_ANGLE(getangle((sp1)->x - (sp2)->x, (sp1)->y - (sp2)->y)))) < (range))

// two vectors 
// can determin direction
#define DOT_PRODUCT_2D(x1,y1,x2,y2) (mulscale((x1),(x2),16) + mulscale((y1),(y2),16))
#define DOT_PRODUCT_3D(x1,y1,z1,x2,y2,z2) (mulscale((x1),(x2),16) + mulscale((y1),(y2),16) + mulscale((z1),(z2),16))

// just determine if the player is moving
#define PLAYER_MOVING(pp) ((pp)->xvect|(pp)->yvect)
    
#define KEY_EXT(scan) (KEY_PRESSED(scan) | KEY_PRESSED(scan+128))

//#define LOW(long_ptr) ( *((short *)(long_ptr)) )
//#define HIGH(long_ptr) ( *(((short *)(long_ptr)) + 1) )

//#define LOW_TAG(sectnum) ( *((short *)&sector[(sectnum)].tag) )
//#define HIGH_TAG(sectnum) ( *(((short *)&sector[(sectnum)].tag) + 1) )

#define LOW_TAG(sectnum) ( sector[sectnum].lotag )
#define HIGH_TAG(sectnum) ( sector[sectnum].hitag )

//#define LOW_TAG_SPRITE(spnum) ( *((short *)&sprite[(spnum)].tag) )
//#define HIGH_TAG_SPRITE(spnum) ( *(((short *)&sprite[(spnum)].tag) + 1) )

#define LOW_TAG_SPRITE(spnum) ( sprite[(spnum)].lotag )
#define HIGH_TAG_SPRITE(spnum) ( sprite[(spnum)].hitag )


//#define LOW_TAG_WALL(wallnum) ( *((short *)&wall[(wallnum)].tag) )
//#define HIGH_TAG_WALL(wallnum) ( *(((short *)&wall[(wallnum)].tag) + 1) )

#define LOW_TAG_WALL(wallnum) ( wall[(wallnum)].lotag )
#define HIGH_TAG_WALL(wallnum) ( wall[(wallnum)].hitag )

#define CEILING_DIST (Z(4))
#define FLOOR_DIST (Z(4))

// Attributes for monochrome text
#define MDA_BLANK          0x00
#define MDA_NORMAL         0x07
#define MDA_BLINK          0x87
#define MDA_HIGH           0x0F
#define MDA_HIGHBLINK      0x8F
#define MDA_UNDER          0x01
#define MDA_UNDERBLINK     0x81
#define MDA_UNDERHIGH      0x09
#define MDA_UNDERHIGHBLINK 0x89
#define MDA_REVERSE        0x70
#define MDA_REVERSEBLINK   0xF0

// defines for move_sprite return value
#define HIT_MASK (BIT(14)|BIT(15))
#define HIT_SPRITE (BIT(14)|BIT(15))
#define HIT_WALL   BIT(15)
#define HIT_SECTOR BIT(14)

#define NORM_SPRITE(val) ((val) & (SIZ(sprite) - 1))
#define NORM_WALL(val) ((val) & (SIZ(wall) - 1))
#define NORM_SECTOR(val) ((val) & (SIZ(sector) - 1))

// overwritesprite flags
#define OVER_SPRITE_MIDDLE      (BIT(0))
#define OVER_SPRITE_VIEW_CLIP   (BIT(1))
#define OVER_SPRITE_TRANSLUCENT (BIT(2))
#define OVER_SPRITE_XFLIP       (BIT(3))
#define OVER_SPRITE_YFLIP       (BIT(4))

// rotatesprite flags
#define ROTATE_SPRITE_TRANSLUCENT   (BIT(0))
#define ROTATE_SPRITE_VIEW_CLIP     (BIT(1)) // clip to view
#define ROTATE_SPRITE_XFLIP         (BIT(2))
#define ROTATE_SPRITE_IGNORE_START_MOST (BIT(3)) // don't clip to startumost
#define ROTATE_SPRITE_SCREEN_CLIP   (BIT(1)|BIT(3)) // use window
#define ROTATE_SPRITE_CORNER        (BIT(4)) // place sprite from upper left corner

#define RS_SCALE                    BIT(16)

// system defines for status bits
#define CEILING_STAT_PLAX           BIT(0)
#define CEILING_STAT_SLOPE          BIT(1)
#define CEILING_STAT_SWAPXY         BIT(2)
#define CEILING_STAT_SMOOSH         BIT(3)
#define CEILING_STAT_XFLIP          BIT(4)
#define CEILING_STAT_YFLIP          BIT(5)
#define CEILING_STAT_RELATIVE       BIT(6)

#define FLOOR_STAT_PLAX           BIT(0)
#define FLOOR_STAT_SLOPE          BIT(1)
#define FLOOR_STAT_SWAPXY         BIT(2)
#define FLOOR_STAT_SMOOSH         BIT(3)
#define FLOOR_STAT_XFLIP          BIT(4)
#define FLOOR_STAT_YFLIP          BIT(5)
#define FLOOR_STAT_RELATIVE       BIT(6)

#define CSTAT_WALL_BLOCK            BIT(0)
#define CSTAT_WALL_BOTTOM_SWAP      BIT(1)
#define CSTAT_WALL_ALIGN_BOTTOM     BIT(2)
#define CSTAT_WALL_XFLIP            BIT(3)
#define CSTAT_WALL_MASKED           BIT(4)
#define CSTAT_WALL_1WAY             BIT(5)
#define CSTAT_WALL_BLOCK_HITSCAN    BIT(6)
#define CSTAT_WALL_TRANSLUCENT      BIT(7)
#define CSTAT_WALL_YFLIP            BIT(8)

//cstat, bit 0: 1 = Blocking sprite (use with clipmove, getzrange)    "B"
//       bit 1: 1 = 50/50 transluscence, 0 = normal                   "T"
//       bit 2: 1 = x-flipped, 0 = normal                             "F"
//       bit 3: 1 = y-flipped, 0 = normal                             "F"
//       bits 5-4: 00 = FACE sprite (default)                         "R"
//                 01 = WALL sprite (like masked walls)
//                 10 = FLOOR sprite (parallel to ceilings&floors)
//                 11 = SPIN sprite (face sprite that can spin 2draw style - not done yet)
//       bit 6: 1 = 1-sided sprite, 0 = normal                        "1"
//       bit 7: 1 = Real centered centering, 0 = foot center          "C"
//       bit 8: 1 = Blocking sprite (use with hitscan)                "H"
//       bit 9: reserved
//       bit 10: reserved
//       bit 11: reserved
//       bit 12: reserved
//       bit 13: reserved
//       bit 14: reserved
//       bit 15: 1 = Invisible sprite, 0 = not invisible

#define CSTAT_SPRITE_BLOCK          BIT(0)
#define CSTAT_SPRITE_TRANSLUCENT    BIT(1)
#define CSTAT_SPRITE_XFLIP          BIT(2)
#define CSTAT_SPRITE_YFLIP          BIT(3)
#define CSTAT_SPRITE_WALL           BIT(4)
#define CSTAT_SPRITE_FLOOR          BIT(5)
#define CSTAT_SPRITE_SPIN           (BIT(4)|BIT(5))
#define CSTAT_SPRITE_ONE_SIDE       BIT(6)
#define CSTAT_SPRITE_YCENTER        BIT(7)
#define CSTAT_SPRITE_BLOCK_HITSCAN  BIT(8)
#define CSTAT_SPRITE_INVISIBLE      BIT(15)

// break up of picanm[]
#define TILE_ANIM_NUM (0xF|BIT(4)|BIT(5))                                                       
#define TILE_ANIM_TYPE (BIT(6)|BIT(7))                                                        
#define TILE_SPEED (0xF << 20)                                                        

#if DEBUG
#define DPRINT(line,str) DebugPrint(line,str);
#else
#define DPRINT(line,str)
#endif

#define DESIGN 1

VOID 
PokeStringMono(BYTE Attr, BYTEp String);

#if 1
extern int DispMono;
#define SPRINTF(a,b,c,d,e,f,g,h,i,j,k) sprintf(a,b,c,d,e,f,g,h,i,j,k)
#define MONO_PRINT(str) if (DispMono) PokeStringMono(MDA_NORMAL, str)
#else
#define SPRINTF(a,b,c,d,e,f,g,h,i,j,k)
#define MONO_PRINT(str)
#endif


#define PRINT(line,str) DebugPrint(line,str)

#define SIZ(array) (sizeof(array)/sizeof(array[0]))


//
// Directions
//

#define DEGREE_45 256
#define DEGREE_90 512

//
// nextsectorneighborz defines - what a god-awful name!
//

#define SEARCH_DOWN 1
#define SEARCH_UP   -1
#define SEARCH_FLOOR 1
#define SEARCH_CEILING -1

#define UP_DIR -1
#define DOWN_DIR 1

#define OPEN 0
#define CLOSE 1

////
//
// Directional enumerations
//
////

enum DirOrd
    {
    ORD_NORTH, ORD_NE, ORD_EAST, ORD_SE, ORD_SOUTH, ORD_SW, ORD_WEST, ORD_NW
    };

enum Dir8
    {
    NORTH   = ORD_NORTH * DEGREE_45,
    NE      = ORD_NE    * DEGREE_45,
    EAST    = ORD_EAST  * DEGREE_45,
    SE      = ORD_SE    * DEGREE_45,
    SOUTH   = ORD_SOUTH * DEGREE_45,
    SW      = ORD_SW    * DEGREE_45,
    WEST    = ORD_WEST  * DEGREE_45,
    NW      = ORD_NW    * DEGREE_45,
    };

typedef enum Dir8 DIR8;

// Auto building enumerations

#define MAKE_CONPIC_ENUM
enum conpic_id
{
   #include "conpic.h"
};

typedef enum conpic_id CONPIC_ID;
#undef MAKE_CONPIC_ENUM

#define DIGI_ENUM
enum digi 
{
    #include "digi.h"
};
#undef DIGI_ENUM


////
//
// State declarations
//
////


// Forward declarations
struct STATEstruct;
typedef struct STATEstruct STATE, *STATEp;

//struct PIC_STATEstruct;
//typedef struct PIC_STATEstruct PIC_STATE, *PIC_STATEp;

struct PANEL_STATEstruct;
typedef struct PANEL_STATEstruct PANEL_STATE, *PANEL_STATEp;

struct PLAYERstruct;
typedef struct PLAYERstruct PLAYER, *PLAYERp;

struct PERSONALITYstruct;
typedef struct PERSONALITYstruct PERSONALITY, *PERSONALITYp;

struct ATTRIBUTEstruct;
typedef struct ATTRIBUTEstruct ATTRIBUTE, *ATTRIBUTEp;

struct SECTOR_OBJECTstruct;
typedef struct SECTOR_OBJECTstruct SECTOR_OBJECT, *SECTOR_OBJECTp;

struct PANEL_SPRITEstruct;
typedef struct PANEL_SPRITEstruct PANEL_SPRITE, *PANEL_SPRITEp;

struct ANIMstruct;
typedef struct ANIMstruct ANIM, *ANIMp;

typedef int ANIMATOR(SHORT SpriteNum);
typedef ANIMATOR *ANIMATORp;

typedef spritetype SPRITE, *SPRITEp;
typedef sectortype SECTOR, *SECTORp;
typedef walltype WALL, *WALLp;

typedef struct
    {
    short sector;
    long depth;
    }WATER, *WATERp;

#define MAX_WATER 100    
extern WATER Water[MAX_WATER];   

typedef struct
    {
    SECTORp sectp;
    long ceilingz;
    }ACTOR_BLOCK, *ACTOR_BLOCKp;

#define MAX_ACTOR_BLOCK 64    
extern ACTOR_BLOCK ActorBlock[MAX_ACTOR_BLOCK];   

struct STATEstruct
{
    short     Pic;
    int       Tics;
    ANIMATORp Animator;

    STATEp   NextState;
};

//
// State Flags
//

#define SF_TICS_MASK 0xFFFF
#define SF_QUICK_CALL BIT(16)
#define SF_PLAYER_FUNC BIT(17) // only for players to execute
#define SF_TIC_ADJUST BIT(18) // use tic adjustment for these frames


// Panel State flags

#define psf_NoInterrupt BIT(16)
#define psf_Interrupt   BIT(17)
#define psf_QuickCall    BIT(23)




///////////////////////////////////////////////////////////////////////////////////////////
//
// Weapon
//
///////////////////////////////////////////////////////////////////////////////////////////

enum Weapon
    {
    WPN_SWORD, 
    WPN_STAR, 
    WPN_UZI,
    WPN_BOW, 
    WPN_FIREBALL,
    WPN_ELECTRO,
    WPN_HEART,
    WPN_TORNADO,
    MAX_WEAPONS
    };

// weapons that not missile type sprites    
#define WPN_NM_LAVA (-8)

typedef struct
    {
    VOID (*Init)(PLAYERp);
    BYTE Damage;
    } WPN_DATA, *WPN_DATAp;

extern WPN_DATA WeaponData[MAX_WEAPONS];

// bit arrays that determine if a) Weapon has no ammo b) Weapon is the ammo (no weapon exists)
extern long WeaponHasNoAmmo, WeaponIsAmmo;

enum Spell
    {
    SPELL_RING,
    MAX_SPELLS 
    };

VOID InitWeaponStar(PLAYERp);
VOID InitWeaponBow(PLAYERp);
VOID InitWeaponUzi(PLAYERp);
VOID InitWeaponSword(PLAYERp);
VOID InitWeaponTornado(PLAYERp);
VOID InitWeaponFireball(PLAYERp);
VOID InitWeaponElectro(PLAYERp);
VOID InitWeaponHeart(PLAYERp);


extern VOID (*InitWeapon[MAX_WEAPONS])(PLAYERp);



///////////////////////////////////////////////////////////////////////////////////////////
//
// Player 
//
///////////////////////////////////////////////////////////////////////////////////////////

#define MAX_SW_PLAYERS (8)

extern PLAYER Player[MAX_SW_PLAYERS+1];

extern long DemoRecCnt;
extern BOOL DemoRecording, DemoPlaying;       //Can only record 1-player game

typedef struct
    {
    char map_name[13];
    char numplayers;
    }DEMO_HEADER, *DEMO_HEADERp;
    
typedef struct
    {
    long x,y,z;
    }DEMO_START_POS, *DEMO_START_POSp;

typedef struct 
    {
    SHORT vel;
    SHORT svel;
    CHAR angvel;
    LONG bits;
    }SW_PACKET;
    
extern SW_PACKET loc;    

#define PACK 1

/*    
typedef struct
    {
    long syncbits;
    signed short syncvel;
    signed short syncsvel;
    signed char syncangvel;
    }DEMO, *DEMOp;
*/
#define OLD_DEMO 1

#if OLD_DEMO    
#define DEMO_MAX 16384    
extern SW_PACKET DemoRec[DEMO_MAX];    
#else
#define DEMO_BUFFER_MAX 4096
extern SW_PACKET DemoBuffer[DEMO_BUFFER_MAX];    
#endif
    
enum TimedSpellNames
    {
    TSPELL_POWERUP, TSPELL_TURBO, TSPELL_DRAGON, TSPELL_FLY, TSPELL_INVINCIBLE, TSPELL_SHADOW, MAX_TSPELL
    };    

enum PlayerDeathTypes
    {
    PLAYER_DEATH_FLIP, PLAYER_DEATH_CRUMBLE, PLAYER_DEATH_EXPLODE, PLAYER_DEATH_RIPPER 
    };
	
    
struct PLAYERstruct
    {
    // variable that fit in the sprite or user structure
    long posx, posy, posz;
    // interpolation
    long oposx, oposy, oposz;
    short oang;

    long jump_count, jump_speed;     // jumping
    short down_speed, up_speed; // diving
    long climb_ndx;
    long hiz,loz;
    long ceiling_dist,floor_dist;
    SECTORp hi_sectp, lo_sectp;
    SPRITEp hi_sp, lo_sp;
    
    long xvect, yvect;
    long oxvect, oyvect;
    long friction;
    long slide_xvect, slide_yvect;
    short slide_ang;
    long slide_dec;
    
    short view_outside_dang;  // outside view delta ang
    
    short pang,cursectnum,lastcursectnum;
    short turn180_target; // 180 degree turn

    // variables that do not fit into sprite structure
    long horiz,hvel,tilt,tilt_dest;
    long oldposx,oldposy;
    long RevolveX, RevolveY;
    short RevolveDeltaAng, RevolveAng;
    
    // under vars are for wading and swimming
    short PlayerSprite, PlayerUnderSprite;
    SPRITEp SpriteP, UnderSpriteP;
    
    short MaxHealth;
    
    short pnum; // carry along the player number
    
    short LadderSector,LadderAngle;
    long lx,ly; // ladder x and y
    short JumpDuration;
    short WadeDepth;
    short bob_amt;
    short bob_ndx;
    long bob_z;
    
    //Multiplayer syncing variables
    SW_PACKET sync;
    SW_PACKET fsync;
    SW_PACKET fosync;

    //FIFO queue to hold values while faketimerhandler is called from within the drawing routing
    #define MOVEFIFOSIZ 256
    SW_PACKET fifosync[MOVEFIFOSIZ];
    
    void (*DoPlayerAction)(PLAYERp);
    long Flags;
    long KeyPressFlags;
    
    
    SECTOR_OBJECTp sop_control; // sector object pointer
    SECTOR_OBJECTp sop_riding; // sector object pointer

    struct 
    {
    PANEL_SPRITEp Next, Prev;
    }PanelSpriteList;

    // Key stuff
    #define NUM_KEYS 4
    char HasKey[NUM_KEYS];
    
    // Weapon stuff
    long WpnFlags;
    short WpnAmmo[MAX_WEAPONS];
    short WpnNum;
    PANEL_SPRITEp CurWpn;
    PANEL_SPRITEp Wpn[MAX_WEAPONS];
    
    
    // Spell stuff
    short SpellFlags;
    short SpellPowerFlags;
    short SpellNum;
    short Mana;
    PANEL_SPRITEp TimedSpellSprite[MAX_TSPELL];  // Timed spell
    short TimedSpellTics[MAX_TSPELL];
    short TimedSpellFlashCount[MAX_TSPELL];    
    
    // Death stuff
    short DeathType;
    short Kills;
    short Killer;  //who killed me
    
    CHAR  cheating;
    };
    

//
// Player Flags
//    
    
#define PF_DEAD             (BIT(1))
#define PF_JUMPING          (BIT(2))
#define PF_FALLING          (BIT(3))
#define PF_LOCK_CRAWL       (BIT(4))
#define PF_LOCK_HORIZ       (BIT(5))
#define PF_LOOKING          (BIT(6))
#define PF_PLAYER_MOVED     (BIT(7))
#define PF_PLAYER_RIDING    (BIT(8))
#define PF_FALL_BACKWARD    (BIT(9))

#define PF_FLYING           (BIT(11))
#define PF_WEAPON_RETRACT   (BIT(12))
#define PF_PICKED_UP_AN_UZI (BIT(13))
#define PF_CRAWLING         (BIT(14))
#define PF_CLIMBING         (BIT(15))
#define PF_SWIMMING         (BIT(16))
#define PF_DIVING           (BIT(17))
#define PF_DIVING_IN_LAVA   (BIT(18))
#define PF_TWO_UZI          (BIT(19))
#define PF_LOCK_RUN         (BIT(20))
#define PF_TURN_180         (BIT(21))
#define PF_DEAD_HEAD        (BIT(22)) // are your a dead head
#define PF_HEAD_CONTROL     (BIT(23)) // have control of turning when a head?
#define PF_CLIP_CHEAT       (BIT(24)) // cheat for wall clipping
#define PF_SLIDING          (BIT(25)) // cheat for wall clipping
#define PF_VIEW_FROM_OUTSIDE   (BIT(26)) 

///////////////////////////////////////////////////////////////////////////////////////////
//
// Multi - Player 
//
///////////////////////////////////////////////////////////////////////////////////////////

enum MultiGameTypes
    {
    MULTI_GAME_NONE,
    MULTI_GAME_DEATH_MATCH,
    MULTI_GAME_COOPERATIVE
    };

extern SHORT MultiGameType;    // defaults to NONE

///////////////////////////////////////////////////////////////////////////////////////////
//
// Actor
//
///////////////////////////////////////////////////////////////////////////////////////////

//
// Hit Points
//

#define HEALTH_RIPPER            75
#define HEALTH_MOMMA_RIPPER      175
#define HEALTH_NINJA             60
#define HEALTH_RED_NINJA         75
#define HEALTH_COOLIE            40
#define HEALTH_COOLIE_GHOST      65
#define HEALTH_SKEL_PRIEST       70
#define HEALTH_GORO              130
#define HEALTH_HORNET            20
#define HEALTH_SKULL             5

#define HEALTH_SERP_GOD          8000
#define HEALTH_LAVA_GOD          12500
#define HEALTH_LIZ_GOD           15000

//
// Action Set Structure
//

typedef struct
{
#define MAX_ACTOR_CLOSE_ATTACK 2
#define MAX_ACTOR_ATTACK 6
STATEp *Stand;
STATEp *Run;
STATEp *Jump;
STATEp *Fall;
STATEp *Crawl;
STATEp *Swim;
STATEp *Fly;
STATEp *Rise;
STATEp *Sit;
STATEp *Look;
STATEp *Climb;
STATEp *Pain;
STATEp *Death1;
STATEp *Death2;
STATEp *Dead;
STATEp *DeathJump;
STATEp *DeathFall;

STATEp *CloseAttack[MAX_ACTOR_CLOSE_ATTACK];
short  CloseAttackPercent[MAX_ACTOR_CLOSE_ATTACK];

STATEp *Attack[MAX_ACTOR_ATTACK];
short  AttackPercent[MAX_ACTOR_ATTACK];

STATEp *Special[2];
STATEp *Duck;
STATEp *Dive;
}ACTOR_ACTION_SET,*ACTOR_ACTION_SETp;

//
// User Extension record
//

typedef struct
{
    //
    // Variables that can be used by actors and Player
    //

    STATEp State;
    STATEp *Rot;
    STATEp StateStart;
    STATEp StateEnd;
    STATEp *StateFallOverride; // a bit kludgy - override std fall state

    ANIMATORp ActorActionFunc;
    ACTOR_ACTION_SETp ActorActionSet;
    PERSONALITYp Personality;
    ATTRIBUTEp Attrib;
    
    long ox, oy, oz;
    
    long Flags;
    long Tics;
    
    short RotNum;
    short ID;
    
    // Health/Pain related
    short Health;
    short MaxHealth;
    short LastDamage;           // last damage amount taken
    short PainThreshold;       // amount of damage that can be taken before
				// going into pain frames.
    
    // jump & fall
    short jump_speed;
    short jump_grav;
    
    // clipmove
    short ceiling_dist;
    short floor_dist;
    short lo_step;
    long hiz,loz;
    long zclip; // z height to move up for clipmove
    SECTORp hi_sectp, lo_sectp;
    SPRITEp hi_sp, lo_sp;
    
    long update_range;
    
    short   SpriteNum;
    short   Attach;  // attach to sprite if needed - electro snake
    SPRITEp SpriteP;

    // if a player's sprite points to player structure
    PLAYERp PlayerP; 
    short Sibling;

    
    //
    // Possibly used by both.
    //
    
    // precalculated vectors
    long xchange,ychange,zchange;
    
    long  z_tgt;
    
    // velocity
    long  vel_tgt;
    short vel_rate;
    BYTE speed; // Ordinal Speed Range 0-3 from slow to fast
    
    // spinning - tornado spell
    short spin_ang;
    short spin_vel;
    short spin_orig_ang;
    
    short Counter;
    short DamageTics;
    short BladeDamageTics;
    
    short Target;
    short Radius;    // for distance checking
    long  OverlapZ;  // for z overlap variable
    
    // 
    // Only have a place for actors
    //
    
    // lightning spell - make a structure and put a pointer here
    short zap[4];
    
    // target player for the enemy - can only handle one player at at time
    PLAYERp tgt_player;
    
    // scaling
    short scale_speed;
    unsigned short scale_value;
    short scale_tgt;
    
    // zig zagging
    short ZigZagDist;
    short ZigZagAng;
    short ZigZagDir;
    
    short Dist;
    short TargetDist;
    short WaitTics;
    
    // track
    short track;
    short point;
    short track_dir;
    long  track_vel;
    
    // sliding variables - slide backwards etc
    short slide_ang;     
    long  slide_vel;     
    short slide_dec;

    long  sx,sy,sz;
    short sang;
    char spal;  // save off default palette number
    
    long ret; //holder for move_sprite return value
    
    // Need to get rid of these flags
    long  Flag1;
    
    CHAR  LastWeaponNum;
    CHAR  WeaponNum;
     
}USER,*USERp;

// sprite->extra flags
#define SPRX_SKILL              (BIT(0) | BIT(1) | BIT(2))
#define SPRX_AGGRESSIVE         (BIT(3) | BIT(4))
#define SPRX_RESTORE            (BIT(5))
#define SPRX_FOUND              (BIT(6))    // used for finding sprites in build
#define SPRX_BRIDGE             (BIT(7))    // exploding type bridges 
#define SPRX_DEBUG              (BIT(8))    // debug sprites
#define SPRX_MULTI_ITEM         (BIT(9))    // multi player item
#define SPRX_PLAYER_OR_ENEMY    (BIT(10))   // for checking quickly if sprite is a 
					    // player or actor
#define SPRX_BLADE              (BIT(11))   // blade sprite 

// User->Flags flags
#define SPR_ATTACKED            BIT(1) // Is sprite being attacked?
#define SPR_TARGET              BIT(2) // Is sprite a target of a weapon?
#define SPR_AWARE_OF_PLAYER     BIT(3) // Is sprite aware of the player?
#define SPR_ELECTRO_TOLERANT    BIT(4) // Electro spell does not slow actor
#define SPR_JUMPING             BIT(5) // Actor is jumping
#define SPR_FALLING             BIT(6) // Actor is falling
#define SPR_CLIMBING            BIT(7) // Actor is falling
#define SPR_DEATH               BIT(8) // Actor is dying 
#define SPR_SPINNING            BIT(9) // Actor is spinning (tornado spell) 

#define SPR_ZDIFF_MODE          BIT(10) // For following tracks at different z heights
#define SPR_SPEED_UP            BIT(11) // For following tracks at different speeds
#define SPR_SLOW_DOWN           BIT(12) // For following tracks at different speeds
#define SPR_DONT_UPDATE_ANG     BIT(13) // For tracks - don't update the angle for a while

#define SPR_ATTACHED            BIT(14) // sprite is part of a sector object
#define SPR_SUICIDE             BIT(15) // sprite is set to kill itself

#define SPR_RUN_AWAY            BIT(16) // sprite is in "Run Away" track mode.
#define SPR_FIND_PLAYER         BIT(17) // sprite is in "Find Player" track mode.

#define SPR_SWIMMING            BIT(18) // Actor is swimming
#define SPR_WAIT_FOR_PLAYER     BIT(19) // Track Mode - Actor is waiting for player to come close
#define SPR_WAIT_FOR_TRIGGER    BIT(20) // Track Mode - Actor is waiting for player to trigger
#define SPR_SLIDING             BIT(21) // Actor is sliding
#define SPR_ON_SO_SECTOR        BIT(22) // sprite is on a sector object sector

#define SPR_SHADE_DIR           BIT(23) // sprite is on a sector object sector
#define SPR_XFLIP_TOGGLE        BIT(24) // sprite rotation xflip bit
#define SPR_NO_SCAREDZ          BIT(25) // not afraid of falling

#define SPR_POWER               BIT(26) // Powered up weapon
#define SPR_SKIP2               BIT(27) // 20 moves ps
#define SPR_SKIP4               BIT(28) // 10 moves ps

#define SPR_BOUNCE              BIT(29) // For shrapnal types that can bounce once
#define SPR_UNDERWATER          BIT(30) // For missiles etc 


EXTRN USERp User[MAXSPRITES];

typedef struct
	{
	short Xdim, Ydim, ScreenSize;
	}BORDER_INFO,*BORDER_INFOp;        


typedef struct
    {
    short high;
    }RANGE,*RANGEp;

    
///////////////////////////////////////////////////////////////////////////////////////////
//
// Sector Stuff - Sector Objects and Tracks
//
///////////////////////////////////////////////////////////////////////////////////////////

// flags in EXTRA variable
#define SECTFX_SINK                  BIT(0)
#define SECTFX_OPERATIONAL           BIT(1)
//#define SECTFX_DAMAGE                BIT(2)
#define SECTFX_CURRENT               BIT(3)
#define SECTFX_SPECIAL               BIT(4)
#define SECTFX_NO_RIDE               BIT(5) // moving sector - don't ride it
#define SECTFX_TELEPORT              BIT(6)
#define SECTFX_DIVE_AREA             BIT(7) // Diving area
#define SECTFX_UNDERWATER            BIT(8) // Underwater area
#define SECTFX_UNDERWATER2           BIT(9) // Underwater area

#define SECTFX_LIQUID_MASK           (BIT(10)|BIT(11)) // only valid for sectors with depth
#define SECTFX_LIQUID_NONE           (0)
#define SECTFX_LIQUID_LAVA           BIT(10)
#define SECTFX_LIQUID_WATER          BIT(11)

// flags in sector USER structure
#define SECTFU_SO_DONT_BOB          BIT(0)
#define SECTFU_SO_SINK_DEST         BIT(1)
#define SECTFU_SO_DONT_SINK         BIT(2)
#define SECTFU_DONT_COPY_PALETTE    BIT(3)

#define SECT_SINK                  0
#define SECT_OPERATIONAL           1
#define SECT_DAMAGE               2
#define SECT_CURRENT               3
#define SECT_SPECIAL               4
#define SECT_NO_RIDE               5 // moving sector - don't ride it
#define SECT_TELEPORT              6
#define SECT_DIVE_AREA             7
#define SECT_UNDERWATER            8
#define SECT_UNDERWATER2           9

#define SECT_ANGLE                16
#define SECT_HEIGHT               17
#define SECT_SPEED                18
#define SECT_FLOOR_PAN            19

#define SECT_CEILING_PAN          21

#define SECT_WALL_PAN_SPEED       23
#define SECT_WALL_PAN_ANG         24

#define SECT_DEBRIS_SEWER         25

#define SECT_TELEPORT_DEST        26
#define SECT_SO_CENTER            27

#define SECT_MATCH                28
#define SECT_LOCK_DOOR            29
#define SPRI_CLIMB_MARKER         30
#define SECT_SO_SPRITE_OBJ        31
#define SECT_SO_DONT_BOB          32

#define SECT_SO_SINK_DEST         33
#define SECT_SO_DONT_SINK         34

#define SECT_SO_DONT_SINK         34

#define SECT_RAISE_LOWER_PLAT      35
#define SECT_RAISE_LOWER_PLAT_DEST 36
#define SECT_SO_FORM_WHIRLPOOL    37
#define SECT_ACTOR_BLOCK          38
#define SECT_SO_CLIP_DIST         39

#define SECT_CEILING_STOMPER      40
#define SECT_FLOOR_STOMPER        41
#define MULTI_PLAYER_START        42

#define FIREBALL_TRAP             43
#define BOLT_TRAP                 44
#define SECT_SO_DONT_ROTATE       45

#define PARALLAX_LEVEL            46
#define SECT_DONT_COPY_PALETTE    47

#define MULTI_COOPERATIVE_START   48

#define SO_SET_SPEED              49 
#define SO_SPIN                   50 
#define SO_SPIN_REVERSE           51 
#define SO_BOB_START              52 
#define SO_BOB_SPEED              53 
#define SO_STOMP                  54 

#define SO_TURN_SPEED             55 

#define LAVA_ERUPT                56     

#define SECT_EXPLODING_CEIL_FLOOR 57                                           
#define SECT_EXPLODING_CEIL       58                                           
#define SECT_EXPLODING_FLOOR      59                                           
				  
// SPECIAL TAGS linked from SECT_SPECIAL
#define SPECIAL_DEBRIS_KILL       0
#define SPECIAL_TELEPORT_CEILING  1
#define SPECIAL_TELEPORT_FLOOR    2

#define WALLF_LOOP_DONT_SPIN            BIT(0)
#define WALLF_LOOP_REVERSE_SPIN         BIT(1)
#define WALLF_LOOP_SPIN_2X              BIT(2)
#define WALLF_LOOP_SPIN_4X              BIT(3)
#define WALLF_LOOP_OUTER                BIT(4)
#define WALLF_DONT_MOVE                 BIT(5)


typedef struct
    {
    long dist, flags;
    short depth_fract, depth; // do NOT change this, doubles as a long FIXED point number
    short   special, // for special numbers
	    stag,    // ST? tag number - for certain things it helps to know it
	    ang,     
	    height, 
	    speed, 
	    damage, 
	    floor_slope,
	    ceiling_slope,
	    number;  // usually used for matching number
    BYTE    flags2;            
    }SECT_USER, *SECT_USERp;   
    
extern SECT_USERp SectUser[MAXSECTORS];    
SECT_USERp SpawnSectUser(short sectnum);

typedef struct
    {
    unsigned int size, checksum;
    }MEM_HDR,*MEM_HDRp;
    
BOOL ValidPtr(VOID *ptr);
VOID * AllocMem(int size);
VOID * CallocMem(int size, int num);
VOID * ReAllocMem(VOID *ptr, int size);
VOID FreeMem(VOID *ptr);
    

typedef struct
    {
    SECTORp sectp;
    short sector, ang, vel;
    }FLOOR_PAN;

#define MAX_FLOOR_PAN 40   
extern FLOOR_PAN FloorPan[MAX_FLOOR_PAN];   

typedef struct
    {
    SECTORp sectp;
    short sector, ang, vel;
    }CEILING_PAN;

#define MAX_CEILING_PAN 25   
extern CEILING_PAN CeilingPan[MAX_CEILING_PAN];   

typedef struct
    {
    WALLp wallp;
    short wall, ang, vel;
    }WALL_PAN;

#define MAX_WALL_PAN 40   
extern WALL_PAN WallPan[MAX_WALL_PAN];   

#define MAXANIM 256   
typedef void ANIM_CALLBACK(ANIMp, void *);
typedef ANIM_CALLBACK *ANIM_CALLBACKp;
typedef void *ANIM_DATAp;

struct ANIMstruct
    {
    long *ptr, goal;
    long vel;
    short vel_adj;
    ANIM_CALLBACKp callback;
    ANIM_DATAp callbackdata;
    };

extern ANIM Anim[MAXANIM];   
extern short AnimCnt;


typedef struct
    {
    long x,y,z;
    short ang, tag_low, tag_high, filler;
    }TRACK_POINT, *TRACK_POINTp;

typedef struct
    {
    TRACK_POINTp TrackPoint;
    long ttflags;
    short flags;
    short NumPoints;
    }TRACK, *TRACKp;
    
// Most track type flags are in tags.h    

// Regular track flags
#define TF_TRACK_OCCUPIED BIT(0)
    
typedef struct 
    {
    BYTE FromRange,ToRange,FromColor,ToColor;
    }COLOR_MAP, *COLOR_MAPp;
    
#define MAX_TRACKS 100
    
extern TRACK Track[MAX_TRACKS];

struct SECTOR_OBJECTstruct
    {
    #define MAX_SO_SECTOR 40
    #define MAX_SO_SPRITE 60

    SECTORp sectp[MAX_SO_SECTOR];
    
    long    xmid,ymid,zmid, // midpoints of the sector object
	    xorig[MAX_SO_SECTOR*15],   // save the original x & y location of each wall so it can be
	    yorig[MAX_SO_SECTOR*15],   // refreshed 
	    vel,            // velocity
	    vel_tgt,        // target velocity
	    player_xoff,    // player x offset from the xmid
	    player_yoff,    // player y offset from the ymid
	    zorig_floor[MAX_SO_SECTOR],      // original z values for all sectors
	    zorig_ceiling[MAX_SO_SECTOR],      // original z values for all sectors
	    zdelta,         // z delta from original
	    z_tgt,          // target z delta
	    z_rate,         // rate at which z aproaches target
	    update,         // Distance from player at which you continue updating
			    // only works for single player.
	    bob_diff,       // bobbing difference for the frame
	    target_dist,    // distance to next point
	    bob_amt;        // bob amount max in z coord
	    
    short   sector[MAX_SO_SECTOR],     // hold the sector numbers of the sector object
	    sp_num[MAX_SO_SPRITE],     // hold the sprite numbers of the object
	    sectnum,        // current secnum of midpoint
	    mid_sector,     // middle sector
	    wait_tics,      // 
	    num_sectors,    // number of sectors
	    num_walls,      // number of sectors
	    track,          // the track # 0 to 20
	    point,          // the point on the track that the sector object is headed toward
	    vel_rate,       // rate at which velocity aproaches target
	    dir,            // direction traveling on the track
	    ang,            // angle facing
	ang_moving,     // angle the SO is facing
	    clipdist,       // cliping distance for operational sector objects
	    ang_tgt,        // target angle
	    ang_orig,       // original angle
	    last_ang,       // last angle before started spinning
	    old_ang,        // holding variable for the old angle
	    spin_speed,     // spin_speed
	    spin_ang,       // spin angle
	turn_speed,     // shift value determines how fast SO turns to match new angle
	    bob_sine_ndx,   // index into sine table
	    bob_speed,      // shift value for speed
	    flags;
	    
    };
    
#define MAX_SECTOR_OBJECTS 20

#define SOBJ_SPEED_UP           BIT(0)
#define SOBJ_SLOW_DOWN          BIT(1)
#define SOBJ_ZUP                BIT(2)
#define SOBJ_ZDOWN              BIT(3)
#define SOBJ_ZDIFF_MODE         BIT(4) 
#define SOBJ_MOVE_VERTICAL      BIT(5) // for sprite objects - move straight up/down
#define SOBJ_STOMP              BIT(6) 
#define SOBJ_ABSOLUTE_ANGLE     BIT(7) 
#define SOBJ_SPRITE_OBJ         BIT(8) 
#define SOBJ_DONT_ROTATE        BIT(9) 

extern SECTOR_OBJECT SectorObject[MAX_SECTOR_OBJECTS];    


typedef struct
    {
    long origx[17], origy[17];
    short sector, angopen, angclosed, angopendir, sang, anginc, wall[17];
    } SWING;

    
///////////////////////////////////////////////////////////////////////////////////////////
//
// Prototypes
//
///////////////////////////////////////////////////////////////////////////////////////////

ANIMATOR NullAnimator;

VOID SetBorder(PLAYERp pp, int);
int Distance(int x1, int y1, int x2, int y2);
short GetDeltaAngle(short, short);
int move_actor(short SpriteNum, long xchange, long ychange, long zchange);

int SetActorRotation(short SpriteNum,long,long);
int NewStateGroup(short SpriteNum, STATEp SpriteGroup[]);
VOID SectorMidPoint(short sectnum, long *xmid, long *ymid, long *zmid);
USERp SpawnUser(short SpriteNum, short id, STATEp state);

short ActorFindTrack(short SpriteNum, CHAR player_dir, long track_type, short *track_point_num, short *track_dir);

SECT_USERp GetSectUser(short sectnum);

short SoundVol(long x, long y);
short SoundAngle(long x, long  y);
void PlaySound(int num, short angle, short vol);

ANIMATOR DoActorBeginJump,DoActorJump,DoActorBeginFall,DoActorFall,DoActorDeathMove;

VOID PlayerUpdateHealth(PLAYERp pp, short value);
VOID PlayerUpdateAmmo(PLAYERp pp, short WeaponNum, short value);
VOID PlayerUpdateMana(PLAYERp pp, short value);
VOID PlayerUpdateWeapon(PLAYERp pp, short WeaponNum);
VOID PlayerUpdatePanelInfo(PLAYERp pp);
VOID RefreshInfoLine(PLAYERp pp);

VOID DoAnim(long numtics);
void AnimDelete(long *animptr);
short AnimGetGoal(long *animptr);
short AnimSet(long *animptr, long thegoal, long thevel);
//short AnimSetCallback(long *animptr, long thegoal, long thevel, ANIM_CALLBACKp call, ANIM_DATAp data);
short AnimSetCallback(short anim_ndx, ANIM_CALLBACKp call, ANIM_DATAp data);
short AnimSetVelAdj(short anim_ndx, short vel_adj);

VOID EnemyDefaults(short SpriteNum, ACTOR_ACTION_SETp action, PERSONALITYp person);

VOID getzrangepoint(long x, long y, long z, short sectnum, LONGp ceilz, LONGp ceilhit, LONGp florz, LONGp florhit);
int move_sprite(short spritenum, long xchange, long ychange, long zchange, long ceildist, long flordist, char cliptype, long numtics);
int move_missile(short spritenum, long xchange, long ychange, long zchange, long ceildist, long flordist, char cliptype, long numtics);
int DoPickTarget(SPRITEp sp, WORD max_delta_ang, BOOL skip_targets);

VOID change_sprite_stat(short, short);

///////////////////////////////////////////////////////////////////////////////////////////
//
//  Externs
//
///////////////////////////////////////////////////////////////////////////////////////////

extern int LastFrameTics;
extern char ds[];
extern int Bobbing;
extern short Skill;
extern int GodMode;

extern short neartagsector, neartagwall, neartagsprite;
extern long neartaghitdist,neartagrange;

//extern unsigned char synctics, lastsynctics;
extern BORDER_INFO BorderInfo;

extern short snum;

extern long lockspeed,totalsynctics;

#define synctics 3
#define ACTORMOVETICS (synctics<<1)
#define TICSPERMOVEMENT synctics
#define FAKETIMERHANDLER()  if (totalclock >= ototalclock + synctics) faketimerhandler()

// subtract value from clipdist on getzrange calls
#define GETZRANGE_CLIP_ADJ 8
//#define GETZRANGE_CLIP_ADJ 0

// MULTIPLAYER
// VARIABLES:  (You should extern these in your game.c)
extern short numplayers, myconnectindex;
extern short connecthead, connectpoint2[MAXPLAYERS];
extern long *lastpacket2clock;
extern char username[MAXPLAYERS][50];

///////////////////////////
//
// OLDER network additions
//
///////////////////////////

int initmultiplayers(int, int, int);
void uninitmultiplayers(void);

void sendlogon(void);
void sendlogoff(void);
// TENSW: these are unsafe to call if TEN is running. use netsendpacket
// and netgetpacket instead, which work in either case.
//void sendpacket (short otherconnectindex, char *bufptr, short bufleng);
//short getpacket (short *otherconnectindex, char *bufptr);


///////////////////////////
//
// RECENT network additions
//
///////////////////////////

extern long ototalclock,gotlastpacketclock,smoothratio;
extern BOOL ready2send;

// local copy of variables updated by faketimerhandler
extern long locselectedgun;

//FIFO queue to hold values while faketimerhandler is called from within the drawing routing
extern long movefifoplc, movefifoend;

//MULTI.OBJ sync state variables
extern char syncstate;

//GAME.C sync state variables
#define SYNC_TEST TRUE
#define NUMSYNCBYTES 16
extern BYTE syncstat[NUMSYNCBYTES];
extern long syncvalplc, othersyncvalplc;
extern long syncvalend, othersyncvalend;
extern long syncvalcnt, othersyncvalcnt;
extern BYTE syncval[MOVEFIFOSIZ][NUMSYNCBYTES], othersyncval[MOVEFIFOSIZ][NUMSYNCBYTES];

extern BOOL MoveSkip4, MoveSkip2;

#define MASTER_SWITCHING 1

extern char option[];
extern char keys[];
extern short moustat;

extern short screenpeek;

extern long dimensionmode, zoom;
extern long vel,svel,angvel;

extern SHORT StatDamageList[18];

#endif
