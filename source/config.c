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

#include "condef.h"
#ifdef SW_3DFX
//#define __3DFX_ACC__
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <dos.h>
#include "settings.h"
#include "types.h"
#include "develop.h"
#include "scriplib.h"
#include "file_lib.h"
#include "gamedefs.h"
#include "keyboard.h"
#include "util_lib.h"
#include "function.h"
#include "control.h"
#include "fx_man.h"
#include "sounds.h"
#include "config.h"
#include "sndcards.h"
#include "common.h"

// we load this in to get default button and key assignments
// as well as setting up function mappings

#include "_functio.h"
#include "_config.h"

extern void ReadGameSetup(int32 scripthandle);
extern void WriteGameSetup(int32 scripthandle);

//
// Comm variables
//

char CommPlayerName[32];
int32 NumberPlayers,CommPort,PortSpeed,IrqNumber,UartAddress;

//
// Sound variables
//
int32 FXDevice;
int32 MusicDevice;
int32 FXVolume;
int32 MusicVolume;
fx_blaster_config BlasterConfig;
int32 NumVoices;
int32 NumChannels;
int32 NumBits;
int32 MixRate;
int32 MidiPort;

int32 ControllerType;

//
// Screen variables
//

int32 ScreenMode;
int32 ScreenWidth;
int32 ScreenHeight;

char  RTSName[MAXRTSNAMELENGTH];
//static char setupfilename[64]={SETUPFILENAME};
char setupfilename[64]={SETUPFILENAME};
static int32 scripthandle;

char  MouseAnalogAxes[MAXMOUSEAXES][MAXFUNCTIONLENGTH];
char  JoystickAnalogAxes[MAXJOYAXES][MAXFUNCTIONLENGTH];

int32 EnableRudder;

char  MouseDigitalAxes[MAXMOUSEAXES][2][MAXFUNCTIONLENGTH];
char  GamePadDigitalAxes[MAXGAMEPADAXES][2][MAXFUNCTIONLENGTH];
char  JoystickDigitalAxes[MAXJOYAXES][2][MAXFUNCTIONLENGTH];

/*
===================
=
= CONFIG_GetSetupFilename
=
===================
*/
#define MAXSETUPFILES 20
void CONFIG_GetSetupFilename( void )
   {
   struct find_t fblock;
   char extension[10];
   char * src;
   char * filenames[MAXSETUPFILES];
   int32 numfiles;
   int32 i;
   extern char ds[];

   i = CheckParm (SETUPNAMEPARM);
   if (i!=0)
      {
      numfiles = 0;
      strcpy(setupfilename,_argv[i+1]);
      //printf("Using Setup file: %s\n",setupfilename);
      return;
      }
   
   strcpy(setupfilename,SETUPFILENAME);

   // determine extension
   src = setupfilename + strlen(setupfilename) - 1;

   while (*src != '.')
      {
      src--;
      }
   strcpy (&extension[1],src);
   extension[0] = '*';
   
   numfiles=0;
   if (_dos_findfirst(extension,0,&fblock)==0)
      {
      do
         {
         filenames[numfiles]=SafeMalloc(128);
         strcpy(filenames[numfiles],fblock.name);
         numfiles++;
         if (numfiles == MAXSETUPFILES)
            break;
         }
      while(!_dos_findnext(&fblock));
      }
      
   if (numfiles>1)
      {
      int32 time;
      int32 oldtime;
      int32 count;

      printf("\nMultiple Configuration Files Encountered\n");
      printf("========================================\n\n");
      printf("Please choose a configuration file from the following list by pressing its\n");
      printf("corresponding letter:\n\n");
      for (i=0;i<numfiles;i++)
         {
         if (strcmpi(filenames[i],SETUPFILENAME))
            {
            printf("%c. %s\n",'a'+(char)i,filenames[i]);
            }
         else
            {
            printf("%c. %s <DEFAULT>\n",'a'+(char)i,filenames[i]);
            }
         }
      printf("\n");
      printf("(%s will be used if no selection is made within 10 seconds.)\n\n",SETUPFILENAME);
      KB_FlushKeyboardQueue();
      KB_ClearKeysDown();
      count = 9;
      oldtime = clock();
      time=clock()+(10*CLOCKS_PER_SEC);
      while (clock()<time)
         {
         if (clock()>oldtime)
            {
            printf("%ld seconds left. \r",count);
            fflush(stdout);
            oldtime = clock()+CLOCKS_PER_SEC;
            count--;
            }
         if (KB_KeyWaiting())
            {
            int32 ch = KB_Getch();
            ch -='a';
            if (ch>=0 && ch<numfiles)
               {
               strcpy (setupfilename, filenames[ch]);
               break;
               }
            }
         }
      printf("\n\n");
      }
      
   if (numfiles==1)
      strcpy (setupfilename, filenames[0]);
   else
   if (numfiles==0)
        {
        #ifdef SW_3DFX
        printf("\nCFG file not found.  Run SET3DFX.EXE.\n",setupfilename);
        #else
        printf("\nCFG file not found.  Run SETUP.EXE.\n",setupfilename);
        #endif
        exit(0);
        }
      
   //printf("Using Setup file: %s\n",setupfilename);
   i=clock()+(3*CLOCKS_PER_SEC/4);
   while (clock()<i)
      {
      ;
      }
   for (i=0;i<numfiles;i++)
      {
      SafeFree(filenames[i]);
      }
   }

/*
===================
=
= CONFIG_FunctionNameToNum
=
===================
*/

int32 CONFIG_FunctionNameToNum( char * func )
   {
   int32 i;

   for (i=0;i<NUMGAMEFUNCTIONS;i++)
      {
      if (!stricmp(func,gamefunctions[i]))
         {
         return i;
         }
      }
   return -1;
   }

/*
===================
=
= CONFIG_FunctionNumToName
=
===================
*/

char * CONFIG_FunctionNumToName( int32 func )
   {
   if (func < NUMGAMEFUNCTIONS)
      {
      return gamefunctions[func];
      }
   else
      {
      return NULL;
      }
   }

/*
===================
=
= CONFIG_AnalogNameToNum
=
===================
*/
int32 CONFIG_AnalogNameToNum( char * func )
   {
   if (!stricmp(func,"analog_turning"))
      {
      return analog_turning;
      }
   if (!stricmp(func,"analog_strafing"))
      {
      return analog_strafing;
      }
   if (!stricmp(func,"analog_moving"))
      {
      return analog_moving;
      }
   if (!stricmp(func,"analog_lookingupanddown"))
      {
      return analog_lookingupanddown;
      }
   return -1;
   }

/*
===================
=
= CONFIG_ReadKeys
=
===================
*/

void CONFIG_ReadKeys( int32 scripthandle )
   {
   int32 i;
   int32 numkeyentries;
   int32 function;
   char keyname1[80];
   char keyname2[80];
   kb_scancode key1,key2;

   numkeyentries = SCRIPT_NumberEntries( scripthandle,"KeyDefinitions" );

   for (i=0;i<numkeyentries;i++)
      {
      function = CONFIG_FunctionNameToNum(SCRIPT_Entry(scripthandle,"KeyDefinitions", i ));
      if (function != -1)
         {
         memset(keyname1,0,sizeof(keyname1));
         memset(keyname2,0,sizeof(keyname2));
         SCRIPT_GetDoubleString
            (
            scripthandle,
            "KeyDefinitions",
            SCRIPT_Entry( scripthandle, "KeyDefinitions", i ),
            keyname1,
            keyname2
            );
         key1 = 0;
         key2 = 0;
         if (keyname1[0])
            {
            key1 = (byte) KB_StringToScanCode( keyname1 );
            }
         if (keyname2[0])
            {
            key2 = (byte) KB_StringToScanCode( keyname2 );
            }
         CONTROL_MapKey( function, key1, key2 );
         }
      }
   }

// new Duke stuff
/*
===================
=
= CONFIG_SetupMouse
=
===================
*/

// CTW MODIFICATION ENTIRE FILE
// Converted all "false" and "true" to "FALSE" and "TRUE"
// CTW MODIFICATION ENTIRE FILE END

void CONFIG_SetupMouse( int32 scripthandle )
   {
   int32 i;
   char str[80];
   char temp[80];
   int32 function, scale;

   for (i=0;i<MAXMOUSEBUTTONS;i++)
      {
      sprintf(str,"MouseButton%ld",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString( scripthandle,"Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapButton( function, i, FALSE );
      sprintf(str,"MouseButtonClicked%ld",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString( scripthandle,"Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapButton( function, i, TRUE );
      }
   // map over the axes
   for (i=0;i<MAXMOUSEAXES;i++)
      {
      sprintf(str,"MouseAnalogAxes%ld",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString(scripthandle, "Controls", str,temp);
      function = CONFIG_AnalogNameToNum(temp);
      if (function != -1)
         {
         CONTROL_MapAnalogAxis(i,function);
         }
      sprintf(str,"MouseDigitalAxes%ld_0",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString(scripthandle, "Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapDigitalAxis( i, function, 0 );
      sprintf(str,"MouseDigitalAxes%ld_1",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString(scripthandle, "Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapDigitalAxis( i, function, 1 );
      sprintf(str,"MouseAnalogScale%ld",i);
      SCRIPT_GetNumber(scripthandle, "Controls", str,&scale);
      CONTROL_SetAnalogAxisScale( i, scale );
      }

   // 0 to 65536  
   SCRIPT_GetNumber( scripthandle, "Controls","MouseSensitivity",&function);
   gs.MouseSpeed = function;
   CONTROL_SetMouseSensitivity(function<<2);
   }

/*
===================
=
= CONFIG_SetupGamePad
=
===================
*/

void CONFIG_SetupGamePad( int32 scripthandle )
   {
   int32 i;
   char str[80];
   char temp[80];
   int32 function;


   for (i=0;i<MAXJOYBUTTONS;i++)
      {
      sprintf(str,"JoystickButton%ld",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString( scripthandle,"Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapButton( function, i, FALSE );
      sprintf(str,"JoystickButtonClicked%ld",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString( scripthandle,"Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapButton( function, i, TRUE );
      }
   // map over the axes
   for (i=0;i<MAXGAMEPADAXES;i++)
      {
      sprintf(str,"GamePadDigitalAxes%ld_0",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString(scripthandle, "Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapDigitalAxis( i, function, 0 );
      sprintf(str,"GamePadDigitalAxes%ld_1",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString(scripthandle, "Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapDigitalAxis( i, function, 1 );
      }
   SCRIPT_GetNumber( scripthandle, "Controls","JoystickPort",&function);
   CONTROL_JoystickPort = function;
   }

/*
===================
=
= CONFIG_SetupJoystick
=
===================
*/

// new stuff from Duke
void CONFIG_SetupJoystick( int32 scripthandle )
   {
   int32 i;
   char str[80];
   char temp[80];
   int32 function, scale;

   for (i=0;i<MAXJOYBUTTONS;i++)
      {
      sprintf(str,"JoystickButton%ld",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString( scripthandle,"Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapButton( function, i, FALSE );
      sprintf(str,"JoystickButtonClicked%ld",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString( scripthandle,"Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapButton( function, i, TRUE );
      }
   // map over the axes
   for (i=0;i<MAXJOYAXES;i++)
      {
      sprintf(str,"JoystickAnalogAxes%ld",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString(scripthandle, "Controls", str,temp);
      function = CONFIG_AnalogNameToNum(temp);
      if (function != -1)
         {
         CONTROL_MapAnalogAxis(i,function);
         }
      sprintf(str,"JoystickDigitalAxes%ld_0",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString(scripthandle, "Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapDigitalAxis( i, function, 0 );
      sprintf(str,"JoystickDigitalAxes%ld_1",i);
      memset(temp,0,sizeof(temp));
      SCRIPT_GetString(scripthandle, "Controls", str,temp);
      function = CONFIG_FunctionNameToNum(temp);
      if (function != -1)
         CONTROL_MapDigitalAxis( i, function, 1 );
      sprintf(str,"JoystickAnalogScale%ld",i);
      SCRIPT_GetNumber(scripthandle, "Controls", str,&scale);
      CONTROL_SetAnalogAxisScale( i, scale*3 );
      }
   // read in JoystickPort
   SCRIPT_GetNumber( scripthandle, "Controls","JoystickPort",&function);
   CONTROL_JoystickPort = function;
   // read in rudder state
   SCRIPT_GetNumber( scripthandle, "Controls","EnableRudder",&CONTROL_RudderEnabled);
   }

/*
===================
=
= CONFIG_ReadSetup
=
===================
*/

void CONFIG_ReadSetup( void )
   {
   int32 dummy;
   int32 scripthandle;
   extern char palette[256*3];
   extern char palette_data[256][3];
   char ret;
   extern char ds[];
   extern char PlayerNameArg[32];

   CONTROL_ClearAssignments();
   if (!SafeFileExists(setupfilename))
      {
      Error("ReadSetup: %s does not exist\n"
#ifdef SW_3DFX      
            "           Please run SET3DFX.EXE\n",setupfilename);
#else
            "           Please run SETUP.EXE\n",setupfilename);
#endif            
      }
   scripthandle = SCRIPT_Load(setupfilename);
   ret = SCRIPT_GetNumber( scripthandle, "Screen Setup", "ScreenMode",&ScreenMode);
   
   SCRIPT_GetNumber( scripthandle, "Screen Setup", "ScreenWidth",&ScreenWidth);
   SCRIPT_GetNumber( scripthandle, "Screen Setup", "ScreenHeight",&ScreenHeight);

   SCRIPT_GetNumber( scripthandle, "Sound Setup", "FXDevice",&FXDevice);
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "MusicDevice",&MusicDevice);
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "FXVolume",&FXVolume);
   gs.SoundVolume = FXVolume;
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "MusicVolume",&MusicVolume);
   gs.MusicVolume = MusicVolume;

   SCRIPT_GetNumber( scripthandle, "Sound Setup", "NumVoices",&NumVoices);
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "NumChannels",&NumChannels);
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "NumBits",&NumBits);
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "MixRate",&MixRate);
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "MidiPort",&MidiPort);
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "BlasterAddress",&dummy);
   BlasterConfig.Address = dummy;
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "BlasterType",&dummy);
   BlasterConfig.Type = dummy;
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "BlasterInterrupt",&dummy);
   BlasterConfig.Interrupt = dummy;
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "BlasterDma8",&dummy);
   BlasterConfig.Dma8 = dummy;
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "BlasterDma16",&dummy);
   BlasterConfig.Dma16 = dummy;
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "BlasterEmu",&dummy);
   BlasterConfig.Emu = dummy;
   SCRIPT_GetNumber( scripthandle, "Sound Setup", "ReverseStereo",&dummy);
   gs.FlipStereo = dummy;
   if (gs.FlipStereo)
       gs.FlipStereo = 1;

   SCRIPT_GetNumber( scripthandle, "Controls","ControllerType",&ControllerType);
   SCRIPT_GetString( scripthandle, "Comm Setup", "RTSName",RTSName);
   
    SCRIPT_GetNumber( scripthandle, "Comm Setup","ComPort",&CommPort);
    SCRIPT_GetNumber( scripthandle, "Comm Setup","NumberPlayers",&NumberPlayers);
    SCRIPT_GetNumber( scripthandle, "Comm Setup","PortSpeed",&PortSpeed);
    SCRIPT_GetNumber( scripthandle, "Comm Setup","IrqNumber",&IrqNumber);
    SCRIPT_GetNumber( scripthandle, "Comm Setup","UartAddress",&UartAddress);

    SCRIPT_GetString( scripthandle, "Comm Setup","PlayerName",CommPlayerName);
    
    if (PlayerNameArg[0] != '\0')
        {
        strcpy(CommPlayerName, PlayerNameArg);
        }
    
    ReadGameSetup(scripthandle);
    
   CONTROL_ClearAssignments();

   CONFIG_ReadKeys(scripthandle);

   switch (ControllerType)
      {
      case controltype_keyboardandmouse:
         CONFIG_SetupMouse(scripthandle);
         break;
      case controltype_keyboardandjoystick:
      case controltype_keyboardandflightstick:
      case controltype_keyboardandthrustmaster:
         CONFIG_SetupJoystick(scripthandle);
         break;
      case controltype_keyboardandgamepad:
         CONFIG_SetupGamePad(scripthandle);
         break;
      // tried this because this is what it looked like todds was doing   
      default:   
         CONFIG_SetupMouse(scripthandle);
         //CONFIG_SetupJoystick(scripthandle);
         break;
      }
   }

/*
===================
=
= CONFIG_WriteSetup
=
===================
*/

void CONFIG_WriteSetup( void )
   {
   int32 dummy;

   switch (ControllerType)
      {
      case controltype_keyboardandmouse:
         //dummy = CONTROL_GetMouseSensitivity();
         //dummy >>= 1;
         //SCRIPT_PutNumber( scripthandle, "Controls","MouseSensitivity",dummy,FALSE,FALSE);
         SCRIPT_PutNumber( scripthandle, "Controls","MouseSensitivity",gs.MouseSpeed,FALSE,FALSE);
         break;
      }
      
   SCRIPT_PutNumber( scripthandle, "Sound Setup", "FXVolume",gs.SoundVolume,FALSE,FALSE);
   SCRIPT_PutNumber( scripthandle, "Sound Setup", "MusicVolume",gs.MusicVolume,FALSE,FALSE);
   dummy = gs.FlipStereo;
   SCRIPT_PutNumber( scripthandle, "Sound Setup", "ReverseStereo",dummy,FALSE,FALSE);
   
   SCRIPT_PutNumber( scripthandle, "Screen Setup", "ScreenWidth", ScreenWidth,FALSE,FALSE);
   SCRIPT_PutNumber( scripthandle, "Screen Setup", "ScreenHeight",ScreenHeight,FALSE,FALSE);
   SCRIPT_PutNumber( scripthandle, "Screen Setup", "ScreenMode",ScreenMode,FALSE,FALSE);
   
   WriteGameSetup(scripthandle);
   
   SCRIPT_Save (scripthandle, setupfilename);
   SCRIPT_Free (scripthandle);
   }

