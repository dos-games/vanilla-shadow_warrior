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

#ifndef AI_H

#define AI_H

// Call functions based on a random range value
typedef struct 
    {
    short range;
    ANIMATORp action;
    } DECISION, *DECISIONp;    

// Personality structure
struct PERSONALITYstruct
    {
    DECISIONp Battle;
    DECISIONp Offense;
    DECISIONp Broadcast;
    DECISIONp Surprised;
    DECISIONp Evasive;
    DECISIONp LostTarget;
    DECISIONp CloseRange;
    DECISIONp TouchTarget;
    };
    
enum ActorStates { SLOW_SPEED, NORM_SPEED, MID_SPEED, FAST_SPEED, MAX_SPEED};

#define MAXATTRIBSNDS   11
typedef enum {
    attr_ambient, attr_alert, attr_attack, attr_pain, attr_die, 
    attr_extra1, attr_extra2, attr_extra3,attr_extra4,attr_extra5,
    attr_extra6
} ATTRIB_SNDS;    

struct ATTRIBUTEstruct
    {
    short Speed[MAX_SPEED];
    CHAR TicAdjust[MAX_SPEED];
    BYTE MaxWeapons;
    ATTRIB_SNDS Sounds[MAXATTRIBSNDS];
    };
    
extern ATTRIBUTE DefaultAttrib;    
    
// AI.C functions
void __export DebugMoveHit(short SpriteNum);
char __export ActorMoveHitReact(short SpriteNum);
char __export ActorFlaming(short SpriteNum);
void __export DoActorSetSpeed(short SpriteNum,BYTE speed);
short __export ChooseActionNumber(short decision[]);
int __export DoActorNoise(ANIMATORp Action,short SpriteNum);
int __export CanSeePlayer(short SpriteNum);
int __export CanHitPlayer(short SpriteNum);
int __export DoActorPickClosePlayer(short SpriteNum);
int __export CloseRangeDist(SPRITEp sp1,SPRITEp sp2);
int __export InitActorDecide(short SpriteNum);
int __export DoActorDecide(short SpriteNum);
int __export InitActorAlertNoise(short SpriteNum);
int __export InitActorAmbientNoise(short SpriteNum);
int __export InitActorAttackNoise(short SpriteNum);
int __export InitActorPainNoise(short SpriteNum);
int __export InitActorDieNoise(short SpriteNum);
int __export InitActorExtra1Noise(short SpriteNum);
int __export InitActorExtra2Noise(short SpriteNum);
int __export InitActorExtra3Noise(short SpriteNum);
int __export InitActorExtra4Noise(short SpriteNum);
int __export InitActorExtra5Noise(short SpriteNum);
int __export InitActorExtra6Noise(short SpriteNum);
int __export InitActorMoveCloser(short SpriteNum);
int __export DoActorCantMoveCloser(short SpriteNum);
int __export DoActorMoveCloser(short SpriteNum);
short __export FindTrackToPlayer(USERp u);
short __export FindTrackAwayFromPlayer(USERp u);
short __export FindWanderTrack(USERp u);
int __export InitActorRunAway(short SpriteNum);
int __export InitActorRunToward(short SpriteNum);
int __export InitActorAttack(short SpriteNum);
int __export DoActorAttack(short SpriteNum);
int __export InitActorEvade(short SpriteNum);
int __export InitActorWanderAround(short SpriteNum);
int __export InitActorFindPlayer(short SpriteNum);
int __export InitActorDuck(short SpriteNum);
int __export DoActorDuck(short SpriteNum);
int __export DoActorMoveJump(short SpriteNum);
int __export move_scan(short SpriteNum,short ang,long dist,long *stopx,long *stopy,long *stopz,short *stopsect);
int __export FindNewAngle(short SpriteNum,char dir,long DistToMove);
int __export InitActorReposition(short SpriteNum);
int __export DoActorReposition(short SpriteNum);
int __export InitActorPause(short SpriteNum);
int __export DoActorPause(short SpriteNum);

/*    
ANIMATOR
InitActorDecide,
InitActorMoveCloser,
InitActorAttack,
InitActorRunAway,
InitActorEvade,
InitActorWanderAround,
InitActorFindPlayer,
InitActorReposition,
InitActorPause,
InitActorDuck,
InitActorAmbientNoise,
InitActorAlertNoise,
InitActorAttackNoise,
InitActorPainNoise,
InitActorDieNoise,
InitActorExtra1Noise,
InitActorExtra2Noise,
InitActorExtra3Noise,
InitActorExtra4Noise,
InitActorExtra5Noise,
InitActorExtra6Noise;

ANIMATOR
DoActorDecide,
DoActorMoveCloser,
DoActorAttack,
DoActorRunAway,
DoActorWanderAround,
DoActorReposition,
DoActorPause,
DoActorDuck,
DoActorAmbientNoise,
DoActorAlertNoise,
DoActorAttackNoise,
DoActorPainNoise,
DoActorDieNoise,
DoActorExtra1Noise,
DoActorExtra2Noise,
DoActorExtra3Noise,
DoActorExtra4Noise,
DoActorExtra5Noise,
DoActorExtra6Noise;
*/

VOID DoActorSetSpeed(short SpriteNum, BYTE speed);

#endif