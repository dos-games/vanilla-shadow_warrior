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

#include <malloc.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "types.h"
#include "sndcards.h"
#include "fx_man.h"
#include "music.h"
#include "util_lib.h"
#include "gamedefs.h"
#include "config.h"

#undef MAXPLAYERS
#include "build.h"
#include "keys.h"

#include "panel.h"
#include "game.h"

kopen4load(char *filename, char searchfirst);
kread(long handle, void *buffer, long leng);
klseek(long handle, long offset, long whence);
kfilelength(long handle);
kclose(long handle);

void SoundCallBack(unsigned long num);
char *LoadMidi(char *);

#define NUM_SAMPLES 10

char *BitNames[2] =
    {
    "8-bit", "16-bit"
    };

char *ChannelNames[2] =
    {
    "Mono", "Stereo"
    };

char *VoiceNames[8] =
    {
    "1", "2", "3", "4", "5", "6", "7", "8"
    };

char *SoundCardNames[NumSoundCards + 1] =
    {
    "None",
    "SoundBlaster",
    "Pro AudioSpectrum",
    "Sound Man 16",
    "Adlib",
    "General Midi",
    "Sound Canvas",
    "Awe 32",
    "WaveBlaster",
    "Ensoniq SoundScape",
    "Gravis UltraSound",
    "Disney Sound Source",
    "Tandy Sound Source",
    "PC speaker"
    };

int MusicOn = TRUE;
int FxOn = TRUE;

int music;
int soundfx;
int num_voices;

int NumSounds = 0;

int angle;
int distance;
int voice;

int volume;
int fxvolume;

int loopflag;

char *SongPtr = NULL;
char *SongName = NULL;

//
// Standard VOC format information - generally don't need this
//

typedef struct
    {
    BYTE filler[0x1a];
    BYTE type;
    WORD length;
    BYTE filler2;
    BYTE freq;
    BYTE pack;
    BYTE data[1];
    } *VOC_HDRp;

//
// Table that describes the voc file and how it will be played
// Can be easily extended, but you may need to change digi.h
//

//struct STATEstruct;
//typedef struct VOCstruct VOC_INFO, *VOC_INFOp;

typedef struct VOCstruct
    {
    char  *name;                 // name of voc file on disk
    BYTEp data;                 // pointer to voc data
    SHORT pitch_lo;             // lo pitch value
    SHORT pitch_hi;             // hi pitch value
    BYTE priority;              // priority at which vocs are played
                                // I think
    BYTE lock;                  // locking byte for caching
    BYTE playing;               // number of this type of sound currently playing
---------- Conflict 1 - Base File: \dev\sw\code\sounds.c(file)
---------- File 1: \dev\sw\back74\sounds.c(file)
    SHORT voc_num;               // pointer to a different structure
---------- File 2: \dev\sw\merge\sounds.c(file)
    SHORT voc_num;              // Backward reference to parent sound
---------- End Conflict
    } VOC_INFO, *VOC_INFOp;

//    
// Includes digi.h to build the table
//
    
#define DIGI_TABLE
extern VOC_INFO voc[];
static VOC_INFO voc[] =
    {
#include "digi.h"
    {NULL, NULL}
    };
#undef  DIGI_TABLE

#define MAXSONGS        10              // This is the max songs per episode

typedef enum
    {
    et_episode1, et_episode2, et_episode3, et_martialcommbat
    } Episode_Type;

Episode_Type episode_num = et_episode1;     // Set to default

char *e1_songs[] = {
    "intro12.mid",
    "yokoha02.mid",
    "nippon34.mid",
    "kotoki12.mid",
    "hoshia02.mid",
    "execut11.mid",
    "sanai.mid",
    "kotex.mid",
    "noki41.mid"};

char *e2_songs[] = {
    NULL};
char *e3_songs[] = {
    NULL};
char *commbat_songs[] = {
    NULL};

char **song_queue = NULL;
BOOL OpenSound(VOC_INFOp vp, long *handle, long *length);
int ReadSound(long handle, VOC_INFOp vp, long length);


/*
===================
=
= My stuff
=
===================
*/

//
// Routine called when a sound is finished playing
//

void 
SoundCallBack(unsigned long num)
    {
    VOC_INFOp vp = &voc[num];

    // Update counter
    vp->playing--;
    vp->lock--;
    
    #if 0
    if (looping)
        restart sound
    else
        unlock
    #endif
    }


VOID 
UnInitSound(VOID)
    {
    SoundShutdown();
    MusicShutdown();
    }

VOID 
InitFX(VOID)
    {
    VOC_INFOp vp;

    
    ExternalSoundMod();


    // Select which cards to use
    SoundStartup();

    if (!FxOn)
        return;

    // Get the current volume of the music and sound fx
    fxvolume = FX_GetVolume();

    for (vp = voc; vp->name; vp++)
        {
        vp->playing = 0;
        }

    // Set up our fx callback so we can display the sounds that are playing
    FX_SetCallBack(SoundCallBack);
    }

VOID 
InitMusic(VOID)
    {

    // Select which cards to use
    MusicStartup();

    if (!MusicOn)
        return;

    volume = MUSIC_GetVolume();

    switch (episode_num)
        {
    case et_episode1:
        song_queue = e1_songs;
        break;
    case et_episode2:
        song_queue = e2_songs;
        break;
    case et_episode3:
        song_queue = e3_songs;
        break;
    case et_martialcommbat:
        song_queue = commbat_songs;
        break;
    default:
        ASSERT(song_queue != NULL);
        break;
        }
    }

VOID ExternalSoundMod(VOID)
    {
    FILE * fin;
    VOC_INFOp vp;
    char name[40];
    long pri;
    long pitch_lo, pitch_hi;
    long ret;
    
    fin = fopen("swextern.snd", "r"); 
    
    if (!fin)
        return;
    
    while(TRUE)
        {
        ret = fscanf(fin,"%s %d %d",name,&pitch_lo,&pitch_hi);
        
        if (ret == EOF)
            break;
        
        for (vp = voc; vp < &voc[SIZ(voc)]; vp++)
            {
            if (!vp->name)
                continue;
                
            if (stricmp(name, vp->name) == NULL)
                {
                //vp->priority = pri;
                vp->pitch_lo = pitch_lo;
                vp->pitch_hi = pitch_hi;
                }
            }
        }
    
    fclose(fin);
    }    

VOID 
InitSound(VOID)
    {
    InitFX();
    InitMusic();
    }

extern short Level;
VOID 
PlaySong(VOID)
    {
    if (!MusicOn)
        return;

    SongPtr = LoadMidi(song_queue[Level]);

    // Set loop to continually play song
    loopflag = MUSIC_LoopSong;

    MUSIC_PlaySong(SongPtr, loopflag);
    }

VOID 
StopFX(VOID)
    {
    if (!FxOn)
        return;

    // Stop the sound fx engine.
    FX_StopAllSounds();
    }

VOID 
StopSong(VOID)
    {
    if (!MusicOn)
        return;

    // Stop the music
    MUSIC_StopSong();
    FreeMem(SongPtr);
    SongPtr = NULL;
    }

VOID 
StopSound(VOID)
    {
    StopFX();
    StopSong();
    }

//
// Returns a volume range between 0 and 255
// Crappy routine that needs to be re-written - each sound may need a distance
// setting so that things like explosions carry a long way and quiter sounds dont
// 0 dist is max vol - 255 dist is max vol
//    

short 
SoundDist(long x, long y)
    {
    long sqrdist;

    extern short screenpeek;

    sqrdist = SQ(Player[screenpeek].posx - x);
    sqrdist += SQ(Player[screenpeek].posy - y);

    if (sqrdist < 2097152)
        return (0);
    else
        return (255 - (1023 / ((sqrdist >> 21) + 4)));
    }

//
// Angle calcuations - may need to be checked to make sure they are right
//    

short 
SoundAngle(long x, long y)
    {
    extern short screenpeek;

    short angle, delta_angle;

    angle = getangle(x - Player[screenpeek].posx, y - Player[screenpeek].posy);

    delta_angle = GetDeltaAngle(angle, Player[screenpeek].pang);

    // convert a delta_angle to a real angle if negative
    if (delta_angle < 0)
        delta_angle = NORM_ANGLE((1024 + delta_angle) + 1024);

    // convert 2048 degree angle to 32 degree angle
    return (delta_angle >> 6);
    }

//
// Play a sound
//    

void 
PlaySound(int num, short angle, short sound_dist)
    {
    VOC_INFOp vp = &voc[num];
    int pitch;

    if (!FxOn)
        return;
    
    // if no data we need to cache it in
    if (!vp->data)    
        {
        long handle;
        long length;
        
        if (!OpenSound(vp, &handle, &length))
            return;
            
        vp->lock = CACHE_LOCK_START;
        allocache(&vp->data, length, &vp->lock);
        ReadSound(handle, vp, length);
        }
    else
    // if data is not locked
    if (vp->lock <= CACHE_UNLOCK_MAX)    
        {
        vp->lock = CACHE_LOCK_START;
        }    
    else
    // if data is already locked
        {
        vp->lock++;
        }    
    
    //sprintf(ds, "ang %d, vol %d", angle, vol);
    //MONO_PRINT(ds);

    if (sound_dist < 5)
        angle = 0;

    // Check for pitch bending
    ASSERT(vp->pitch_lo <= vp->pitch_hi);
    if (vp->pitch_hi == vp->pitch_lo)
        pitch = vp->pitch_lo;
    else
    if (vp->pitch_hi != vp->pitch_lo)
        pitch = vp->pitch_lo + (RANDOM_RANGE(vp->pitch_hi-vp->pitch_lo));
        
    // Request playback
    voice = FX_PlayVOC3D(vp->data, pitch, angle, sound_dist, vp->priority, num);

    // If sound played, update our counter
    if (voice > FX_Ok)
        {
        vp->playing++;
        }
    }

BOOL    
OpenSound(VOC_INFOp vp, long *handle, long *length)
    {
    *handle = kopen4load(vp->name, 0);

    if (*handle == -1)
        {
        return(FALSE);
        }
        
    *length = kfilelength(*handle);
    
    return(TRUE);
    }

int
ReadSound(long handle, VOC_INFOp vp, long length)
    {
    if (kread(handle, vp->data, length) != length)
        {
        TerminateGame();
        printf("Error reading file '%s'.\n", vp->name);
        exit(0);
        }

    kclose(handle);
    }
    
char *
LoadMidi(char *filename)
    {
    long handle;
    long size;
    char *MidiPtr;

    if ((handle = kopen4load(filename, 0)) == -1)
        {
        TerminateGame();
        printf("Cannot open '%s' for read.\n", filename);
        exit(1);
        }

    size = kfilelength(handle);    

    MidiPtr = (char *) AllocMem(size);
    if (MidiPtr == NULL)
        {
        TerminateGame();
        printf("Out of memory while reading '%s'.\n", filename);
        exit(1);
        }

    if (kread(handle, MidiPtr, size) != size)
        {
        TerminateGame();
        printf("Unexpected end of file while reading '%s'.\n", filename);
        exit(1);
        }

    kclose(handle);

    return (MidiPtr);
    }


/*
===================
=
= SoundStartup
=
===================
*/

void 
SoundStartup(void)
    {
    int32 status;

    // if they chose None lets return
    if (FXDevice == NumSoundCards)
        {
        FxOn = FALSE;
        return;
        }

    FxOn = TRUE;

    // Do special Sound Blaster, AWE32 stuff
    if (
        (FXDevice == SoundBlaster) ||
        (FXDevice == Awe32)
        )
        {
        int MaxVoices;
        int MaxBits;
        int MaxChannels;

        status = FX_SetupSoundBlaster
            (
            BlasterConfig, &MaxVoices, &MaxBits, &MaxChannels
            );
        }
    else
        {
        status = FX_Ok;
        }

    if (status == FX_Ok)
        {
        status = FX_Init(FXDevice, NumVoices, NumChannels, NumBits, MixRate);
        if (status == FX_Ok)
            {
            FX_SetVolume(FXVolume);
            if (ReverseStereo == 1)
                {
                FX_SetReverseStereo(!FX_GetReverseStereo());
                }
            }
        }
    if (status != FX_Ok)
        {
        Error(FX_ErrorString(FX_Error));
        }

    status = FX_SetCallBack(SoundCallBack);

    if (status != FX_Ok)
        {
        Error(FX_ErrorString(FX_Error));
        }

    }

/*
===================
=
= SoundShutdown
=
===================
*/

void 
SoundShutdown(void)
    {
    int32 status;

    // if they chose None lets return
    if (FXDevice == NumSoundCards)
        {
        return;
        }

    status = FX_Shutdown();
    if (status != FX_Ok)
        {
        Error(FX_ErrorString(FX_Error));
        }
    }

/*
===================
=
= MusicStartup
=
===================
*/

void 
MusicStartup(void)
    {
    int32 status;

    // if they chose None lets return
    if (MusicDevice == NumSoundCards)
        {
        MusicOn = FALSE;
        return;
        }

    MusicOn = TRUE;

    // satisfy AWE32 and WAVEBLASTER stuff
    BlasterConfig.Midi = MidiPort;

    status = MUSIC_Init(MusicDevice, MidiPort);
    if (status == MUSIC_Ok)
        {
        MUSIC_SetVolume(MusicVolume);
        }
    else
        {
        Error(MUSIC_ErrorString(MUSIC_ErrorCode));
        }

    }

/*
===================
=
= MusicShutdown
=
===================
*/

void 
MusicShutdown(void)
    {
    int32 status;

    // if they chose None lets return
    if (MusicDevice == NumSoundCards)
        return;

    status = MUSIC_Shutdown();
    if (status != MUSIC_Ok)
        {
        Error(MUSIC_ErrorString(MUSIC_ErrorCode));
        }
    }
