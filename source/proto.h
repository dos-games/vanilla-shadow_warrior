//-------------------------------------------------------------------------
/*
Copyright (C) 1997, 2005 - 3D Realms Entertainment

This file is part of Shadow Warrior version 1.2

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

Original Source: 1997 - Frank Maddin and Jim Norwood
Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

#ifndef PROTO_H

#define PROTO_H

// CTW MODIFICATION WHOLE FILE
// Added return types for all the functions. This wasn't vital, but helps
// clear out a ton of warnings you get when compiling.
// CTW MODIFICATION WHOLE FILE END

//           Sets up interrupt vectors for keyboard and timer, and initializes
//           many variables for the BUILD engine.  You should call this once
//           before any other functions of the BUILD engine are used.

void uninitengine(void);
//           Restores interrupt vectors for keyboard and timer, and frees
//           buffers.  You should call this once at the end of the program
//           before quitting to dos.
//loadboard(char filename[20]);
int loadboard(char *filename, long *posx, long *posy, long *posz, short *ang, short *cursectnum);
//           Loads the given board file into memory for the BUILD engine.
//           Returns -1 if file not found.  If no extension is given, .MAP will
//           be appended to the filename.
int saveboard(char *filename, long *posx, long *posy, long *posz, short *ang, short *cursectnum);
//           Saves the given board from memory inro the specified filename.
//           Returns -1 if unable to save.  If no extension is given, .MAP will
//           be appended to the filename.
int loadpics(char filename[20]);
//           Loads the given artwork file into memory for the BUILD engine.
//           Returns -1 if file not found.  If no extension is given, .ART will
//           be appended to the filename.
//setgamemode(void);
//           This function sets the video mode to 320*200*256color graphics.
//           Since BUILD supports several different modes including mode x,
//           mode 13h, and other special modes, I don't expect you to write
//           any graphics output functions.  (Soon I have all the necessary
//           functions)  If for some reason, you use your own graphics mode,
//           you must call this function again before using the BUILD drawing
//           functions.
void draw3dscreen(void);
//           This function draws the 3D screen to the current drawing page,
//           which is not yet shown.  This way, you can overwrite some things
//           over the 3D screen such as a gun.  To view the screen, use the
//           nextpage() function.  The nextpage() function should always be
//           called sometime after each draw3dscreen() function.
void engineinput(void);
//           This function allows the engine to adjust your position depending
//           on the status of the arrow keys, and other control keys.  It
//           handles timing and clipping.
void nextpage(void);
//           After a screen is prepared, use this function to view the screen.

//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³                           OTHER ENGINE FUNCTIONS:                         ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

void overwritesprite (long x, long y, short tilenum, char shade, char orientation, char dapalnum);
//           Use this function to write sprites over the 3d view.  For example,
//           the menu system in DOOM can be done with this function.  Be sure
//           that you call this function for every single frame after the 3d
//           view is drawn or else it will be flashed on for only 1 frame.
//           If you want x and y to be the top left corner, set the orientation
//           to 0.  If you want x and y to be the middle of the sprite, set the
//           orientation to 1.  The reason I included the orienation = 1 option
//           is so that if you want a sprite centered and the size of the tile
//           changes, you don't need to recompile and guess where the new top
//           left corner is.  Oh yeah, and I forget to mention that if shade is
//           greater than 32, than overwritesprite does transluscence.  (Try it
//           out!)

void printext(long x, long y, char buffer[42], short tilenum, char invisiblecol);
//           Use this function to print text anywhere on the screen from a font
//           that you can create in EDITART.  Please see my example font in
//           TILES.ART to see how I lay out the user-defined font.  X ranges
//           from 0-319. Y ranges from 0-199.  The buffer is the string to
//           print.  Tilenum specifies which font to use.  Invisiblecol tells
//           printext what color to draw the transparent pixels.  If
//           invisiblecol is 255 then the transpararent pixels are still
//           transparent.
void printnum(long x, long y, long num, short tilenum, char invisiblecol);
//           Printnum is a function call that will print a long integer (num)
//           starting at top left corner x, y.  Please look at the documentation
//           for printext, since internally, printnum simply prepares a buffer
//           and calls the printext function.

void setvmode(long videomode);
//           If you look at the top of GAME.C, you will see something like this:
//           #pragma aux setvmode =\...  This is how you do in-line assembler in
//           WATCOM C.  All this function is doing is setting the video mode.
void showengineinfo(void);
//           Use this function after setting to text mode to view some statics
//           about the engine, such as frame rate.
void resettiming(void);
//           Resets timing, such as setting totalclock = 0.  Also resets other
//           timers.  This is for use with the showengineinfo function above.

short ksqrt(long num);  // returns (long)square root
//           A square root function optimized for integers.  Use this function
//           only if you want to.

int getangle(long xvect,long yvect);  //  returns (short)angle;
//           Use this function call to determine the angle between two points.
//           For example, if you want a monster to shoot a bullet towards you,
//           you would get the bullet's angle this way:
//  sprite[bullet].ang = getangle(posx-sprite[monst].x,posy-sprite[monst].y);

short inside(long x, long y, short sectnum);
//  Tests to see whether the overhead point (x, y) is inside sector (sectnum)
//  Returns either 0 or 1, where 1 means it is inside, and 0 means it is not.

//copytilepiece(long walnume1, long x1, long y1, long xsiz, long ysiz,
// long walnume2, long x2, long y2, char shadeoffs);
//  Copies section of tile 1 (walnume1) with top-left corner (x1,y1) and
//  rectangular size (xsiz, ysiz) to top-left corner (x2, y2) of tile 2
//  (walnume).  You can animate tiles with this function.  For example, with
//  this function, you can make a slot machine like in Ken's Labyrinth or an
//  electronic sign with text sliding from right to left.

//  Pass the starting 3D position:
//        (xstart, ystart, zstart, startsectnum)
//  Then pass the 3D angle to shoot (defined as a 3D vector):
//        (vectorx, vectory, vectorz)
//  Then set up the return values for the object hit:
//        (hitsect, hitwall, hitsprite)
//  and the exact 3D point where the ray hits:
//        (hitx, hity, hitz)
//
//  How to determine what was hit:
//      * Hitsect is always equal to the sector that was hit (always >= 0).
//
//      * If the ray hits a sprite then:
//            hitsect = thesectornumber
//            hitsprite = thespritenumber
//            hitwall = -1
//
//       * If the ray hits a wall then:
//            hitsect = thesectornumber
//            hitsprite = -1
//            hitwall = thewallnumber
//
//       * If the ray hits the ceiling of a sector then:
//            hitsect = thesectornumber
//            hitsprite = -1
//            hitwall = -1
//            vectorz < 0
//            (If vectorz < 0 then you're shooting upward which means
//                that you couldn't have hit a floor)
//
//       * If the ray hits the floor of a sector then:
//            hitsect = thesectornumber
//            hitsprite = -1
//            hitwall = -1
//            vectorz > 0
//            (If vectorz > 0 then you're shooting downard which means
//                that you couldn't have hit a ceiling)

int cansee(long x1, long y1, long z1, short sectnum1,
         long x2, long y2, long z2, short sectnum2);  // returns 0 or 1
//  This function determines whether or not two 3D points can "see" each
//  other or not.  All you do is pass it the coordinates of a 3D line defined
//  by two 3D points (with their respective sectors)  The function will return
//  a 1 if the points can see each other or a 0 if there is something blocking
//  the two points from seeing each other.  This is how I determine whether a
//  monster can see you or not. Try playing DOOM1.DAT to fully enjoy this
//  great function!

int nextsectorneighborz(short sectnum, long thez, short topbottom, short direction);
//  This function searches z-coordinates of neighboring sectors to find the
//  closest (next) ceiling starting at the given z-coordinate (thez).
//  For example, if you want to find the goal z-coordinate when opening a
//  door, you might want the door to stop at the next closest neighboring
//  ceiling z-coordinate.  You can get the z-coordinate this way:
//
//      newz = sector[nextsectorneighborz(sectnum,startz,-1,-1)].ceilingz
//
//  topbottom (3rd parameter)  -1 = search ceilings
//                                       1 = search floors
//  direction (4th parameter)  -1 = search upwards
//                                       1 = search downwards

void playposcapt(char filename[20]);
//  Plays the recorded run previosly recorded (such as POSCAPT.DAT).
//  -Not a very useful yet.
void loadpcx(char filename[20], long ptrlong);
//  Loads a .PCX image into a given region in memory.  To go directly to
//  video memory, set ptrlong = 0xa0000.  It is probably easier just to
//  put the image into TILES.ART and use the overwritesprite function.
void loadbmp(char filename[20], long ptrlong);
//  Loads a .PCX image into a given region in memory.  To go directly to
//  video memory, set ptrlong = 0xa0000.  It is probably easier just to
//  put the image into TILES.ART and use the overwritesprite function.

//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³                             SPRITE FUNCTIONS:                             ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

int insertsprite(short sectnum, short statnum);   //returns (short)spritenum;
int COVERinsertsprite(short sectnum, short statnum);   //returns (short)spritenum;
//  Whenever you insert a sprite, you must pass is the sector
//  number, and a status number (statnum).  The status number can be any
//  number from 0 to MAXSTATUS-1.  Insertsprite works like a memory
//  allocation function and returns the sprite number.

void deletesprite(short spritenum);
//  Deletes the sprite.

void changespritesect(short spritenum, short newsectnum);
//  Changes the sector of sprite (spritenum) to the
//  newsector (newsectnum).  This function may become
//  internal to the engine in the move_sprite function.  But
//  this function is necessary since all the sectors have
//  their own doubly-linked lists of sprites.

void change_sprite_stat(short spritenum, short newstatnum);
//  Changes the status of sprite (spritenum) to status
//  (newstatus).  Newstatus can be any number from 0 to MAXSTATUS-1.
//  You can use this function to put a monster on a list of active sprites
//  when it first sees you.


//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³                         SERIAL PORT FUNCTIONS:                            ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
//Notes: These functions may be rewritten to be easier to use in the future, or
//  when there is support for other methods of communcation.

void comon(void);
//  Initializes the com port if a com port is enabled in the setup program.
void comoff(void);
//  Uninitializes the com port.  You must call this before exiting your
//  program if you called comon.
void comsend(long dat, long numbytes);
//  Sends 1, 2, 3, or 4 bytes of data to the com port.
//  To send a char, do this:       comsend((long)mychar,1L);
//  To send a short int, do this:  comsend((long)myshort,2L);
//  To send a long int, do this:   comsend(mylong,4L);
void comgetchar(void);
//  Comgetchar attempts to receive 1 character.  If no characters are ready,
//  then instead of waiting until one is ready, comgetchar returns a -1, else
//  a number from 0-255 is returned.

//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³                       DIGITIZED SOUND FUNCTIONS:                          ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
//Note: If you want to write your own digitized sound driver, simply delete
//  these functions from GAME.C.

void initsb(char sndspeed, char);
//           Initializes the digitized sound routines.  You need to call this
//           only once at the beginning of the program.  The speed number is
//           determined by this equation:
//               speed# = 256 - (1,000,000 / sample_rate)
//           Currently, the sample_rate is around 11,000 Hertz. (speed# = 165)
void wsay(char filename[20], char volume);
//           Play the sound file (filename[20]) at the given volume.  Volume
//           ranges from 0 (silent) to 255 (full volume).  Here is a good
//           equation for calculating sound volume from point (x1,y1) to
//           (x2,y2):
//      wsay("blowup.wav",1023/(( ((x1-x2)-(x1-x2)+(y1-y2)*(y1-y2)) >>21)+4));

void uninitsb(void);
//           Turns the speaker off, so sounds don't continue playing while
//           your back in DOS.

//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³                            MUSIC FUNCTIONS:                               ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
//Note: If you want to write your own music driver, simply delete these
//  functions from GAME.C.  The BUILD engine uses interrupt vector 0x8 (IRQ0)
//  and that may complicate things.  If you like my music, perhaps I can send
//  you my MIDI sequencer program (It requires MPU-401, and TSENG-ET4000 SVGA,
//  but could be standardized if there's enough demand).

void loadmusic(char filename[20]);
//           Loads the given song into memory.  Be sure INSTS.DAT is in the
//           current directory.  If no extension is given, then .KSM will be
//           appended to the filename.  You should use this function only when
//           the music is off.
void musicon(void);
//           Enable the playing of music.  Use this only after loadmusic has
//           been called.
void musicoff(void);
//           Disable the playing of music.  Be sure to call this before quitting
//           to DOS.
//****************************************************************************/

#if 0
void neartag(long x, long y, long z, short sectnum, short ang,  //Starting position & angle
                          short *neartagsector,   //Returns near sector if sector[].tag != 0
                          short *neartagwall,     //Returns near wall if wall[].tag != 0
                          short *neartagsprite,   //Returns near sprite if sprite[].tag != 0
                          long *neartaghitdist,   //Returns actual distance to object (scale: 1024=largest grid size)
                          long neartagrange);      //Choose maximum distance to scan (scale: 1024=largest grid size)
#else
//  If tagsearch = 1, neartag searches lotag only
//  If tagsearch = 2, neartag searches hitag only
//  If tagsearch = 3, neartag searches lotag&hitag

#define NTAG_SEARCH_LO 1
#define NTAG_SEARCH_HI 2
#define NTAG_SEARCH_LO_HI 3

void neartag (long xs, long ys, long zs, short sectnum, short ange,
  short *neartagsector, short *neartagwall, short *neartagsprite,
  long *neartaghitdist, long neartagrange, char tagsearch);
#endif  


                          
//void drawrooms(void);
void drawrooms(long posx, long posy, long posz, short ang, long horiz, short cursectnum);

void drawmasks(void);

void dragpoint(short wallnum, long newx, long newy);

void rotatepoint(long xpivot, long ypivot, long x, long y, short deltaang, long *x2, long *y2);

int draw2dscreen(long posxe, long posye, short ange, long zoome, short gride);

int qsetmode320200(void);
int qsetmode640350(void);
int qsetmode640480(void);

int setsprite(short spritenum, long newx, long newy, long newz);


int move_sprite(short spritenum, long xchange, long ychange, long zchange,
             long ceildist, long flordist,
             unsigned long cliptype, long numtics);

void movesprit(short spritenum, long xchange, long ychange, long zchange,
             long walldist, long ceildist, long flordist,
             unsigned long cliptype, long numtics);

void updatesector(long x, long y, short *sectnum);
             
void permanentwritesprite (long thex, long they, short tilenum, signed char shade,
                             long cx1, long cy1, long cx2, long cy2, char dapalnum);
                             
//rotatesprite(long sx, long sy, long z, short a, short picnum, short shade, char pal, char trans);
//rotatesprite(long sx, long sy, long z, short a, short picnum,
//signed char dashade, char dapalnum, char dastat);

void rotatesprite (long sx, long sy, long z, short a, short picnum, signed char dashade, char dapalnum, char dastat, long cx1, long cy1, long cx2, long cy2);


// CTW REMOVED
//void makepalookup(long palnum, char *remapbuf, signed char r, signed char g, signed char b, char dastat);
// CTW REMOVED END

                             
/*                             
            - Added permanentwritesprite function for status bars or other
                  sections of the screen that will not be overwritten by the
                  engine.  The format of this function is like overwritesprite
                  except that the x and y are always top left corner, no
                  orientation variable, and no translucence.
                      The 4 last parameters (cx1, cy1) - (cx2, cy2) define a
                  rectangular clipping window of where permanentwritesprite
                  can draw to.
*/             

#endif