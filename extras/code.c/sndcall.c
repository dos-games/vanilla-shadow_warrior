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

SoundShutdown();
MusicShutdown();
gs.SoundVolume = FX_GetVolume();
FX_SetCallBack(SoundCallBack);
MUSIC_PlaySong(SongPtr, loopflag);
FX_StopAllSounds();
MUSIC_StopSong();
voice = FX_PlayLoopedVOC(vp->data, start, 65536, pitch, 255 - sound_dist, 255 - sound_dist, 255 - sound_dist, priority, num);
voice = FX_PlayVOC3D(vp->data, pitch, angle, sound_dist, priority, num);
FX_SetReverseStereo(!gs.FlipStereo);
status = FX_Init(FXDevice, NumVoices, NumChannels, NumBits, MixRate);
FX_SetVolume(FXVolume);
status = FX_SetCallBack(SoundCallBack);
Error(FX_ErrorString(FX_Error));
status = FX_Shutdown();
status = MUSIC_Init(MusicDevice, MidiPort);
MUSIC_SetVolume(MusicVolume);
status = MUSIC_Shutdown();
FX_SoundActive(vp->handle)
FX_StopSound(vp->handle);       
FX_SoundsPlaying();
FX_Pan3D(p->handle, angle, dist);
FX_SetPitch(p->handle, pitch);

