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

///////////////////////////////////////////////////////////////////////////////
// ISTUB.C
// Copyright (c) 1997 Jim Norwood and 3D Realms Entertainment
// Include this stub file in each DLL you create for Shadow Warrior
// It is required for DLL initialization
///////////////////////////////////////////////////////////////////////////////
#include <stddef.h>

#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "def.h"
#include "weapon.h"
#include "exports.h"
#include "istub.h"


// Local DLL versions of SW data structures
short *sintablep;
spritetype *spritep;
USERp *Userp;


XREF_STDLIBp SL;
XREFp SWext;


void __export
InitStdLibExports (XREF_STDLIBp ExportList)
    {
    SL = ExportList;    // Pointer to standard library table
    }
        
void __export
InitSWExports (XREFp ExportList)
    {
    SWext = ExportList; // Assign the structure containing valid funtion
                        // pointer addresses to a local structure through
                        // which you can access functions inside SW.EXE
    }

////////////////////////////////////////////////////////////////////////////////

// SPECIAL ACCESSOR FUNCTIONS FOR MANIPULATING DATA IN SW FROM A DLL

// WATCOM does not support standard library functions under Dos4G DLL's :(
// STDLIB FUNCTIONS
int printf(const char *format, ...)
    {
    int result;
    
    va_list argptr;

    va_start (argptr,format);
    result = SL->printf (format,argptr);
    va_end (argptr);

    return(result);
    }

int sprintf( char *buf, const char *format, ... )
    {
    int result;
    va_list argptr;

    va_start (argptr,buf);
    result = SL->sprintf (buf,format,argptr);
    va_end (argptr);

    return(result);
    }

int rand( void )
{return(SL->rand());}

long int labs( long int j )
{return(SL->labs(j));}


// SW FUNCTIONS
USERp QueryUser(short SpriteNum)
{return(SWext->QueryUser(SpriteNum));}

BOOL SetUser(short SpriteNum, USERp u)
{return(SWext->SetUser(SpriteNum,u));}

SPRITEp QuerySprite(short SpriteNum)
{return(SWext->QuerySprite(SpriteNum));}

short Querysintable(short val)
{return(SWext->Querysintable(val));}

BOOL QueryRandomPrint(void)
{return(SWext->QueryRandomPrint());}

BOOL QueryPrediction(void)
{return(SWext->QueryPrediction());}

long Queryrandomseed(void)
{return(SWext->Queryrandomseed());}

unsigned long QueryMoveThingsCount(void)
{return(SWext->QueryMoveThingsCount());}

short Querytilesizx(short val)
{return(SWext->Querytilesizx(val));}

void Settilesizx(short ndx, short val)
{SWext->Settilesizx(ndx,val);}

short Querytilesizy(short val)
{return(SWext->Querytilesizy(val));}

void Settilesizy(short ndx, short val)
{SWext->Settilesizy(ndx,val);}

long Querypicanm(short ndx)
{return(SWext->Querypicanm(ndx));}

PLAYERp QueryPlayer(short ndx)
{return(SWext->QueryPlayer(ndx));}

short Queryconnecthead(void)
{return(SWext->Queryconnecthead());}

short Queryconnectpoint2(short ndx)
{return(SWext->Queryconnectpoint2(ndx));}

TRACKp QueryTrack(short ndx)
{return(SWext->QueryTrack(ndx));}

///////////////////////////////////////////////////////////////////////////////

// Local function redefinitions:    
// The below functions simply call their pointer equivalents in SW
// This is done so that static function addresses can be used in actor state
// tables inside a DLL

/////////////////////////////////////////////////////
// Local function redefinitions from AI.C inside SW
/////////////////////////////////////////////////////
void change_sprite_stat(short spritenum, short newstatnum)
{SWext->change_sprite_stat(spritenum,newstatnum);}

int NullAnimator(short SpriteNum)
{return(SWext->NullAnimator(SpriteNum));}

VOID EnemyDefaults(short SpriteNum, ACTOR_ACTION_SETp action, PERSONALITYp person)
{SWext->EnemyDefaults(SpriteNum,action,person);}

// ENGINE.C functions
int krand2(void)
{return(SWext->krand2());}

int neartag(long xs,long ys,long zs,short sectnum,short ange,short *neartagsector,short *neartagwall,short *neartagsprite,long *neartaghitdist,long neartagrange,char tagsearch)
{return(SWext->neartag(xs,ys,zs,sectnum,ange,neartagsector,neartagwall,neartagsprite,neartaghitdist,neartagrange,tagsearch));}

int getangle(long xvect,long yvect)
{return(SWext->getangle(xvect,yvect));}

int changespritesect(short spritenum,short newsectnum)
{return(SWext->changespritesect(spritenum,newsectnum));}

// PLAYER.C functions
short GetDeltaAngle(short ang1,short ang2)
{return(GetDeltaAngle(ang1,ang2));}

// SECTOR.C functions
int OperateSector(short sectnum, short player_is_operating)
{return(SWext->OperateSector(sectnum,player_is_operating));}

int OperateWall(short wallnum,short player_is_operating)
{return(SWext->OperateWall(wallnum,player_is_operating));}

// ROOMS.C functions
BOOL FAFcansee(LONG xs, LONG ys, LONG zs, SHORT sects,LONG xe, LONG ye, LONG ze, SHORT secte)
{return(SWext->FAFcansee(xs,ys,zs,sects,xe,ye,ze,secte));}

void FAFhitscan(LONG x,LONG y,LONG z,SHORT sectnum,LONG xvect,LONG yvect,LONG zvect,SHORTp hitsect,SHORTp hitwall,SHORTp hitsprite,LONGp hitx,LONGp hity,LONGp hitz,LONG clipmask)
{SWext->FAFhitscan(x,y,z,sectnum,xvect,yvect,zvect,hitsect,hitwall,hitsprite,hitx,hity,hitz,clipmask);}

void FAFgetzrange(LONG x,LONG y,LONG z,SHORT sectnum,LONGp hiz,LONGp ceilhit,LONGp loz,LONGp florhit,LONG clipdist,LONG clipmask)
{SWext->FAFgetzrange(x,y,z,sectnum,hiz,ceilhit,loz,florhit,clipdist,clipmask);}

void FAFgetzrangepoint(LONG x,LONG y,LONG z,SHORT sectnum,LONGp hiz,LONGp ceilhit,LONGp loz,LONGp florhit)
{SWext->FAFgetzrangepoint(x,y,z,sectnum,hiz,ceilhit,loz,florhit);}

int setsprite(short spritenum,long newx,long newy,long newz)
{return(SWext->setsprite(spritenum,newx,newy,newz));}

BOOL PicInView(short tile_num,BOOL reset)
{return(SWext->PicInView(tile_num,reset));}

// SOUNDS.C functions
void PlayerSound(int num, long *x, long *y, long *z, Voc3D_Flags flags, PLAYERp pp)
{SWext->PlayerSound(num,x,y,z,flags,pp);}

int PlaySound(int num, long *x, long *y, long *z, Voc3D_Flags flags)
{return(SWext->PlaySound(num,x,y,z,flags));}

void PlaySoundRTS(long rts_num)
{SWext->PlaySoundRTS(rts_num);}

void Set3DSoundOwner(short spritenum)
{SWext->Set3DSoundOwner(spritenum);}

void PlaySpriteSound(short spritenum, int attrib_ndx, Voc3D_Flags flags)
{SWext->PlaySpriteSound(spritenum,attrib_ndx,flags);}

// GAME.C functions
void DebugWriteString(char *string)
{SWext->DebugWriteString(string);}

int Distance(int x1,int y1,int x2,int y2)
{return(SWext->Distance(x1,y1,x2,y2));}

#if RANDOM_DEBUG
int RandomRange(int range, char *file, unsigned line)
{return(SWext->RandomRange(range, file, line));}

int krand(char *file, unsigned line)
{return(SWext->krand(file, line));}
#else
int RandomRange(int range)
{return(SWext->RandomRange(range));}

int krand(void)
{return(SWext->krand());}
#endif

int StdRandomRange(int range)
{return(SWext->StdRandomRange(range));}

// SPRITE.C functions
USERp SpawnUser(short SpriteNum, short id, STATEp state)
{return(SWext->SpawnUser(SpriteNum,id,state));}

void ChangeState(short SpriteNum,STATEp statep)
{SWext->ChangeState(SpriteNum,statep);}

int move_actor(short SpriteNum, long xchange, long ychange, long zchange)
{return(SWext->move_actor(SpriteNum,xchange,ychange,zchange));}

void NewStateGroup(short SpriteNum,STATEp StateGroup[])
{SWext->NewStateGroup(SpriteNum,StateGroup);}

void SetEnemyInactive(short SpriteNum)
{SWext->SetEnemyInactive(SpriteNum);}

int move_sprite(short spritenum,long xchange,long ychange,long zchange,long ceildist,long flordist,ULONG cliptype,long numtics)
{return(SWext->move_sprite(spritenum,xchange,ychange,zchange,ceildist,flordist,cliptype,numtics));}

BOOL DropAhead(short SpriteNum,short min_height)
{return(SWext->DropAhead(SpriteNum,min_height));}

// TRACK.C functions
int ActorFollowTrack(short SpriteNum, short locktics)
{return(SWext->ActorFollowTrack(SpriteNum, locktics));}

short ActorFindTrack(short SpriteNum,CHAR player_dir,long track_type,short *track_point_num,short *track_dir)
{return(SWext->ActorFindTrack(SpriteNum,player_dir,track_type,track_point_num,track_dir));}

// JWEAPON.C functions
int DoPhosphorus(SHORT Weapon)
{return(SWext->DoPhosphorus(Weapon));}

int DoChemBomb(SHORT Weapon)
{return(SWext->DoChemBomb(Weapon));}

int DoCaltrops(SHORT Weapon)
{return(SWext->DoCaltrops(Weapon));}

int SpawnRadiationCloud(short SpriteNum)
{return(SWext->SpawnRadiationCloud(SpriteNum));}

int DoRadiationCloud(short SpriteNum)
{return(SWext->DoRadiationCloud(SpriteNum));}

int PlayerInitChemBomb(PLAYERp pp)
{return(SWext->PlayerInitChemBomb(pp));}

int InitSpriteChemBomb(SHORT SpriteNum)
{return(SWext->InitSpriteChemBomb(SpriteNum));}

int InitChemBomb(short SpriteNum)
{return(SWext->InitChemBomb(SpriteNum));}

int PlayerInitFlashBomb(PLAYERp pp)
{return(SWext->PlayerInitFlashBomb(pp));}

int InitFlashBomb(SHORT SpriteNum)
{return(SWext->InitFlashBomb(SpriteNum));}

int SpawnFlashBombOnActor(SHORT enemy)
{return(SWext->SpawnFlashBombOnActor(enemy));}

int PlayerInitCaltrops(PLAYERp pp)
{return(SWext->PlayerInitCaltrops(pp));}

int InitCaltrops(SHORT SpriteNum)
{return(SWext->InitCaltrops(SpriteNum));}

int InitPhosphorus(SHORT SpriteNum)
{return(SWext->InitPhosphorus(SpriteNum));}

int InitBloodSpray(SHORT SpriteNum, BOOL dogib, short velocity)
{return(SWext->InitBloodSpray(SpriteNum,dogib,velocity));}

void *AllocMem(int size) 
{return(SWext->AllocMem(size));}

/////////////////////////////////////////////////////
// Local function redefinitions from AI.C inside SW
/////////////////////////////////////////////////////
/*
void DebugMoveHit(short SpriteNum) 
{SWext->DebugMoveHit(SpriteNum);}

char ActorMoveHitReact(short SpriteNum) 
{return(SWext->ActorMoveHitReact(SpriteNum));}

int RandomRange(int range,char *file,unsigned int line) 
{return(SWext->RandomRange(range,file,line));}

int StdRandomRange(int range) 
{return(SWext->StdRandomRange(range));}

char ActorFlaming(short SpriteNum) 
{return(SWext->ActorFlaming(SpriteNum));}

void DoActorSetSpeed(short SpriteNum,BYTE speed) 
{SWext->DoActorSetSpeed(SpriteNum,speed);}

short ChooseActionNumber(short decision[]) 
{return(SWext->ChooseActionNumber(decision));}

int DoActorNoise(ANIMATORp Action,short SpriteNum) 
{return(SWext->DoActorNoise(Action,SpriteNum));}

int CanSeePlayer(short SpriteNum) 
{return(SWext->CanSeePlayer(SpriteNum));}

int CanHitPlayer(short SpriteNum) 
{return(SWext->CanHitPlayer(SpriteNum));}

int DoActorPickClosePlayer(short SpriteNum) 
{return(SWext->DoActorPickClosePlayer(SpriteNum));}

int CloseRangeDist(SPRITEp sp1,SPRITEp sp2) 
{return(SWext->CloseRangeDist(sp1,sp2));}

int InitActorDecide(short SpriteNum) 
{return(SWext->InitActorDecide(SpriteNum));}

int DoActorDecide(short SpriteNum) 
{return(SWext->DoActorDecide(SpriteNum));}

int InitActorAlertNoise(short SpriteNum) 
{return(SWext->InitActorAlertNoise(SpriteNum));}

int InitActorAmbientNoise(short SpriteNum) 
{return(SWext->InitActorAmbientNoise(SpriteNum));}

int InitActorAttackNoise(short SpriteNum) 
{return(SWext->InitActorAttackNoise(SpriteNum));}

int InitActorPainNoise(short SpriteNum) 
{return(SWext->InitActorPainNoise(SpriteNum));}

int InitActorDieNoise(short SpriteNum) 
{return(SWext->InitActorDieNoise(SpriteNum));}

int InitActorExtra1Noise(short SpriteNum)       
{return(SWext->InitActorExtra1Noise(SpriteNum));}

int InitActorExtra2Noise(short SpriteNum) 
{return(SWext->InitActorExtra2Noise(SpriteNum));}

int InitActorExtra3Noise(short SpriteNum) 
{return(SWext->InitActorExtra3Noise(SpriteNum));}

int InitActorExtra4Noise(short SpriteNum) 
{return(SWext->InitActorExtra4Noise(SpriteNum));}

int InitActorExtra5Noise(short SpriteNum) 
{return(SWext->InitActorExtra5Noise(SpriteNum));}

int InitActorExtra6Noise(short SpriteNum) 
{return(SWext->InitActorExtra6Noise(SpriteNum));}

int InitActorMoveCloser(short SpriteNum) 
{return(SWext->InitActorMoveCloser(SpriteNum));}

int DoActorCantMoveCloser(short SpriteNum) 
{return(SWext->DoActorCantMoveCloser(SpriteNum));}

int DoActorMoveCloser(short SpriteNum) 
{return(SWext->DoActorMoveCloser(SpriteNum));}

short FindTrackToPlayer(USERp u) 
{return(SWext->FindTrackToPlayer(u));}

short FindTrackAwayFromPlayer(USERp u) 
{return(SWext->FindTrackAwayFromPlayer(u));}

short FindWanderTrack(USERp u) 
{return(SWext->FindWanderTrack(u));}

int InitActorRunAway(short SpriteNum) 
{return(SWext->InitActorRunAway(SpriteNum));}

int InitActorRunToward(short SpriteNum) 
{return(SWext->InitActorRunToward(SpriteNum));}

int InitActorAttack(short SpriteNum) 
{return(SWext->InitActorAttack(SpriteNum));}

int DoActorAttack(short SpriteNum) 
{return(SWext->DoActorAttack(SpriteNum));}

int InitActorEvade(short SpriteNum) 
{return(SWext->InitActorEvade(SpriteNum));}

int InitActorWanderAround(short SpriteNum) 
{return(SWext->InitActorWanderAround(SpriteNum));}

int InitActorFindPlayer(short SpriteNum) 
{return(SWext->InitActorFindPlayer(SpriteNum));}

int InitActorDuck(short SpriteNum) 
{return(SWext->InitActorDuck(SpriteNum));}

int DoActorDuck(short SpriteNum) 
{return(SWext->DoActorDuck(SpriteNum));}

int DoActorMoveJump(short SpriteNum) 
{return(SWext->DoActorMoveJump(SpriteNum));}

int move_scan(short SpriteNum,short ang,long dist,long *stopx,long *stopy,long *stopz,short *stopsect) 
{return(SWext->move_scan(SpriteNum,ang,dist,stopx,stopy,stopz,stopsect));}

int FindNewAngle(short SpriteNum,char dir,long DistToMove) 
{return(SWext->FindNewAngle(SpriteNum,dir,DistToMove));}

int InitActorReposition(short SpriteNum) 
{return(SWext->InitActorReposition(SpriteNum));}

int DoActorReposition(short SpriteNum) 
{return(SWext->DoActorReposition(SpriteNum));}

int InitActorPause(short SpriteNum)
{return(SWext->InitActorPause(SpriteNum));}

int DoActorPause(short SpriteNum) 
{return(SWext->DoActorPause(SpriteNum));}
*/

/////////////////////////////////////////////////////
// Local function redefinitions from ACTOR.C inside SW
/////////////////////////////////////////////////////

int DoActorDie(short SpriteNum,short weapon)
{return(SWext->DoActorDie(SpriteNum,weapon));}

void DoDebrisCurrent(SPRITEp sp)
{SWext->DoDebrisCurrent(sp);}

int DoActorSectorDamage(short SpriteNum)
{return(SWext->DoActorSectorDamage(SpriteNum));}

int move_debris(short SpriteNum,long xchange,long ychange,long zchange)
{return(SWext->move_debris(SpriteNum,xchange,ychange,zchange));}

int DoActorDebris(short SpriteNum)
{return(SWext->DoActorDebris(SpriteNum));}

int DoFireFly(short SpriteNum)
{return(SWext->DoFireFly(SpriteNum));}

int DoGenerateSewerDebris(short SpriteNum)
{return(SWext->DoGenerateSewerDebris(SpriteNum));}

void KeepActorOnFloor(short SpriteNum)
{SWext->KeepActorOnFloor(SpriteNum);}

int DoActorBeginSlide(short SpriteNum,short ang,short vel,short dec)
{return(SWext->DoActorBeginSlide(SpriteNum,ang,vel,dec));}

int DoActorSlide(short SpriteNum)
{return(SWext->DoActorSlide(SpriteNum));}

int DoActorBeginJump(short SpriteNum)
{return(SWext->DoActorBeginJump(SpriteNum));}

int DoActorJump(short SpriteNum)
{return(SWext->DoActorJump(SpriteNum));}

int DoActorBeginFall(short SpriteNum)
{return(SWext->DoActorBeginFall(SpriteNum));}

int DoActorFall(short SpriteNum)
{return(SWext->DoActorFall(SpriteNum));}

int DoActorStopFall(short SpriteNum)
{return(SWext->DoActorStopFall(SpriteNum));}

int DoActorDeathMove(short SpriteNum)
{return(SWext->DoActorDeathMove(SpriteNum));}

int DoBeginJump(short SpriteNum)
{return(SWext->DoBeginJump(SpriteNum));}

int DoJump(short SpriteNum)
{return(SWext->DoJump(SpriteNum));}

int DoBeginFall(short SpriteNum)
{return(SWext->DoBeginFall(SpriteNum));}

int DoFall(short SpriteNum)
{return(SWext->DoFall(SpriteNum));}

/////////////////////////////////////////////////////
// Local function redefinitions from WEAPON.C inside SW
/////////////////////////////////////////////////////
BOOL MissileHitMatch(short Weapon,short WeaponNum,short hitsprite)
{return(SWext->MissileHitMatch(Weapon,WeaponNum,hitsprite));}

int SpawnShrapX(short SpriteNum)
{return(SWext->SpawnShrapX(SpriteNum));}

int DoLavaErupt(short SpriteNum)
{return(SWext->DoLavaErupt(SpriteNum));}

//void UserShrapSetup(SHRAPp shrap,STATEp state,long num_shrap)
//{SWext->UserShrapSetup(shrap,state,num_shrap);}

int SpawnShrap(short ParentNum,short Secondary)
{return(SWext->SpawnShrap(ParentNum,Secondary));}

int DoShrapMove(SHORT SpriteNum)
{return(SWext->DoShrapMove(SpriteNum));}

int DoVomit(short SpriteNum)
{return(SWext->DoVomit(SpriteNum));}

int DoVomitSplash(short SpriteNum)
{return(SWext->DoVomitSplash(SpriteNum));}

int DoFastShrapJumpFall(short SpriteNum)
{return(SWext->DoFastShrapJumpFall(SpriteNum));}

int DoTracerShrap(short SpriteNum)
{return(SWext->DoTracerShrap(SpriteNum));}

int DoShrapJumpFall(short SpriteNum)
{return(SWext->DoShrapJumpFall(SpriteNum));}

int DoShrapDamage(short SpriteNum)
{return(SWext->DoShrapDamage(SpriteNum));}

int SpawnBlood(short SpriteNum,short Weapon,short hitang,long hitx,long hity,long hitz)
{return(SWext->SpawnBlood(SpriteNum,Weapon,hitang,hitx,hity,hitz));}

BOOL VehicleMoveHit(short SpriteNum)
{return(SWext->VehicleMoveHit(SpriteNum));}

BOOL WeaponMoveHit(short SpriteNum)
{return(SWext->WeaponMoveHit(SpriteNum));}

int DoUziSmoke(short SpriteNum)
{return(SWext->DoUziSmoke(SpriteNum));}

int DoShotgunSmoke(short SpriteNum)
{return(SWext->DoShotgunSmoke(SpriteNum));}

int DoMineSpark(short SpriteNum)
{return(SWext->DoMineSpark(SpriteNum));}

int DoFireballFlames(short SpriteNum)
{return(SWext->DoFireballFlames(SpriteNum));}

int SetSuicide(short SpriteNum)
{return(SWext->SetSuicide(SpriteNum));}

int DoActorScale(short SpriteNum)
{return(SWext->DoActorScale(SpriteNum));}

int DoRipperGrow(short SpriteNum)
{return(SWext->DoRipperGrow(SpriteNum));}

int ActorChooseDeath(short SpriteNum,short Weapon)
{return(SWext->ActorChooseDeath(SpriteNum,Weapon));}

int ActorHealth(short SpriteNum,short amt)
{return(SWext->ActorHealth(SpriteNum,amt));}

int SopDamage(SECTOR_OBJECTp sop,short amt)
{return(SWext->SopDamage(sop,amt));}

int SopCheckKill(SECTOR_OBJECTp sop)
{return(SWext->SopCheckKill(sop));}

int ActorPain(short SpriteNum)
{return(SWext->ActorPain(SpriteNum));}

int ActorPainPlasma(short SpriteNum)
{return(SWext->ActorPainPlasma(SpriteNum));}

int ActorStdMissile(short SpriteNum,short Weapon)
{return(SWext->ActorStdMissile(SpriteNum,Weapon));}

int ActorDamageSlide(short SpriteNum,short damage,short ang)
{return(SWext->ActorDamageSlide(SpriteNum,damage,ang));}

int PlayerDamageSlide(PLAYERp pp,short damage,short ang)
{return(SWext->PlayerDamageSlide(pp,damage,ang));}

int GetDamage(short SpriteNum,short Weapon,short DamageNdx)
{return(SWext->GetDamage(SpriteNum,Weapon,DamageNdx));}

int RadiusGetDamage(short SpriteNum,short Weapon,long max_damage)
{return(SWext->RadiusGetDamage(SpriteNum,Weapon,max_damage));}

int PlayerCheckDeath(PLAYERp pp,short Weapon)
{return(SWext->PlayerCheckDeath(pp,Weapon));}

int StarBlood(short SpriteNum,short Weapon)
{return(SWext->StarBlood(SpriteNum,Weapon));}

int DoDamage(short SpriteNum,short Weapon)
{return(SWext->DoDamage(SpriteNum,Weapon));}

int DoDamageTest(short Weapon)
{return(SWext->DoDamageTest(Weapon));}

int DoHitscanDamage(short Weapon,short hitsprite)
{return(SWext->DoHitscanDamage(Weapon,hitsprite));}

int DoFlamesDamageTest(short Weapon)
{return(SWext->DoFlamesDamageTest(Weapon));}

short PrevWall(short wall_num)
{return(SWext->PrevWall(wall_num));}

void TraverseBreakableWalls(short start_sect,long x,long y,long z,short ang,long radius)
{SWext->TraverseBreakableWalls(start_sect,x,y,z,ang,radius);}

int DoExpDamageTest(short Weapon)
{return(SWext->DoExpDamageTest(Weapon));}

int DoMineExpMine(short Weapon)
{return(SWext->DoMineExpMine(Weapon));}

int DoStar(SHORT Weapon)
{return(SWext->DoStar(Weapon));}

int DoPlasmaDone(SHORT Weapon)
{return(SWext->DoPlasmaDone(Weapon));}

int MissileSeek(SHORT Weapon,SHORT delay_tics,SHORT aware_range,SHORT dang_shift,SHORT turn_limit,SHORT z_limit)
{return(SWext->MissileSeek(Weapon,delay_tics,aware_range,dang_shift,turn_limit,z_limit));}

int ComboMissileSeek(SHORT Weapon,SHORT delay_tics,SHORT aware_range,SHORT dang_shift,SHORT turn_limit,SHORT z_limit)
{return(SWext->ComboMissileSeek(Weapon,delay_tics,aware_range,dang_shift,turn_limit,z_limit));}

int VectorMissileSeek(SHORT Weapon,SHORT delay_tics,SHORT aware_range1,SHORT aware_range2)
{return(SWext->VectorMissileSeek(Weapon,delay_tics,aware_range1,aware_range2));}

int DoBlurExtend(SHORT Weapon,SHORT interval,SHORT blur_num)
{return(SWext->DoBlurExtend(Weapon,interval,blur_num));}

int InitPlasmaFountain(SPRITEp wp,SPRITEp sp)
{return(SWext->InitPlasmaFountain(wp,sp));}

int DoPlasmaFountain(SHORT Weapon)
{return(SWext->DoPlasmaFountain(Weapon));}

int DoPlasma(SHORT Weapon)
{return(SWext->DoPlasma(Weapon));}

int DoCoolgFire(SHORT Weapon)
{return(SWext->DoCoolgFire(Weapon));}

int DoEelFire(short Weapon)
{return(SWext->DoEelFire(Weapon));}

void ScaleSpriteVector(short SpriteNum,long scale)
{SWext->ScaleSpriteVector(SpriteNum,scale);}

void WallBounce(short SpriteNum,short ang)
{SWext->WallBounce(SpriteNum,ang);}

BOOL SlopeBounce(short SpriteNum,BOOL *hitwall)
{return(SWext->SlopeBounce(SpriteNum,hitwall));}

int DoGrenade(SHORT Weapon)
{return(SWext->DoGrenade(Weapon));}

int DoVulcanBoulder(SHORT Weapon)
{return(SWext->DoVulcanBoulder(Weapon));}

int DoMineRangeTest(short Weapon,short range)
{return(SWext->DoMineRangeTest(Weapon,range));}

int DoMineStuck(SHORT Weapon)
{return(SWext->DoMineStuck(Weapon));}

int SetMineStuck(SHORT Weapon)
{return(SWext->SetMineStuck(Weapon));}

int DoMine(SHORT Weapon)
{return(SWext->DoMine(Weapon));}

int DoPuff(short SpriteNum)
{return(SWext->DoPuff(SpriteNum));}

int DoRailPuff(short SpriteNum)
{return(SWext->DoRailPuff(SpriteNum));}

int DoBoltThinMan(SHORT Weapon)
{return(SWext->DoBoltThinMan(Weapon));}

int DoTracer(SHORT Weapon)
{return(SWext->DoTracer(Weapon));}

int DoTankShell(SHORT Weapon)
{return(SWext->DoTankShell(Weapon));}

int DoTracerStart(SHORT Weapon)
{return(SWext->DoTracerStart(Weapon));}

int DoLaser(SHORT Weapon)
{return(SWext->DoLaser(Weapon));}

int DoRail(SHORT Weapon)
{return(SWext->DoRail(Weapon));}

int DoRailStart(SHORT Weapon)
{return(SWext->DoRailStart(Weapon));}

int DoRocket(SHORT Weapon)
{return(SWext->DoRocket(Weapon));}

int DoMicroMini(SHORT Weapon)
{return(SWext->DoMicroMini(Weapon));}

int SpawnExtraMicroMini(SHORT Weapon)
{return(SWext->SpawnExtraMicroMini(Weapon));}

int DoMicro(SHORT Weapon)
{return(SWext->DoMicro(Weapon));}

int DoUziBullet(SHORT Weapon)
{return(SWext->DoUziBullet(Weapon));}

int DoBoltSeeker(SHORT Weapon)
{return(SWext->DoBoltSeeker(Weapon));}

int DoBoltShrapnel(SHORT Weapon)
{return(SWext->DoBoltShrapnel(Weapon));}

int DoBoltFatMan(SHORT Weapon)
{return(SWext->DoBoltFatMan(Weapon));}

int DoElectro(SHORT Weapon)
{return(SWext->DoElectro(Weapon));}

int DoLavaBoulder(SHORT Weapon)
{return(SWext->DoLavaBoulder(Weapon));}

int DoSpear(SHORT Weapon)
{return(SWext->DoSpear(Weapon));}

int SpawnCoolieExp(short SpriteNum)
{return(SWext->SpawnCoolieExp(SpriteNum));}

int SpawnBasicExp(SHORT Weapon)
{return(SWext->SpawnBasicExp(Weapon));}

int SpawnFireballFlames(SHORT SpriteNum,SHORT enemy)
{return(SWext->SpawnFireballFlames(SpriteNum,enemy));}

int SpawnFireballExp(SHORT Weapon)
{return(SWext->SpawnFireballExp(Weapon));}

int SpawnGoroFireballExp(SHORT Weapon)
{return(SWext->SpawnGoroFireballExp(Weapon));}

int SpawnBoltExp(SHORT Weapon)
{return(SWext->SpawnBoltExp(Weapon));}

int SpawnTankShellExp(SHORT Weapon)
{return(SWext->SpawnTankShellExp(Weapon));}

int SpawnNuclearSecondaryExp(SHORT Weapon,short ang)
{return(SWext->SpawnNuclearSecondaryExp(Weapon,ang));}

int SpawnNuclearExp(SHORT Weapon)
{return(SWext->SpawnNuclearExp(Weapon));}

int SpawnTracerExp(SHORT Weapon)
{return(SWext->SpawnTracerExp(Weapon));}

int SpawnMicroExp(SHORT Weapon)
{return(SWext->SpawnMicroExp(Weapon));}

int AddSpriteToSectorObject(short SpriteNum,SECTOR_OBJECTp sop)
{return(SWext->AddSpriteToSectorObject(SpriteNum,sop));}

int SpawnBigGunFlames(SHORT Weapon,SHORT Operator,SECTOR_OBJECTp sop)
{return(SWext->SpawnBigGunFlames(Weapon,Operator,sop));}

int SpawnGrenadeSecondaryExp(SHORT Weapon,short ang)
{return(SWext->SpawnGrenadeSecondaryExp(Weapon,ang));}

int SpawnGrenadeSmallExp(SHORT Weapon)
{return(SWext->SpawnGrenadeSmallExp(Weapon));}

int SpawnGrenadeExp(SHORT Weapon)
{return(SWext->SpawnGrenadeExp(Weapon));}

void SpawnExpZadjust(short Weapon,SPRITEp exp,long upper_zsize,long lower_zsize)
{SWext->SpawnExpZadjust(Weapon,exp,upper_zsize,lower_zsize);}

int SpawnMineExp(SHORT Weapon)
{return(SWext->SpawnMineExp(Weapon));}

int InitMineShrap(short SpriteNum)
{return(SWext->InitMineShrap(SpriteNum));}

int DoMineExp(short SpriteNum)
{return(SWext->DoMineExp(SpriteNum));}

int DoSectorExp(SHORT SpriteNum)
{return(SWext->DoSectorExp(SpriteNum));}

int SpawnSectorExp(SHORT Weapon)
{return(SWext->SpawnSectorExp(Weapon));}

int SpawnLargeExp(SHORT Weapon)
{return(SWext->SpawnLargeExp(Weapon));}

int SpawnMeteorExp(SHORT Weapon)
{return(SWext->SpawnMeteorExp(Weapon));}

int DoFireball(SHORT Weapon)
{return(SWext->DoFireball(Weapon));}

int DoFindGround(SHORT SpriteNum)
{return(SWext->DoFindGround(SpriteNum));}

int DoFindGroundPoint(SHORT SpriteNum)
{return(SWext->DoFindGroundPoint(SpriteNum));}

int DoNapalm(SHORT Weapon)
{return(SWext->DoNapalm(Weapon));}

int DoMeteor(SHORT Weapon)
{return(SWext->DoMeteor(Weapon));}

int DoSerpMeteor(SHORT Weapon)
{return(SWext->DoSerpMeteor(Weapon));}

int DoMirvMissile(SHORT Weapon)
{return(SWext->DoMirvMissile(Weapon));}

int DoMirv(SHORT Weapon)
{return(SWext->DoMirv(Weapon));}

BOOL MissileSetPos(short Weapon,ANIMATORp DoWeapon,long dist)
{return(SWext->MissileSetPos(Weapon,DoWeapon,dist));}

int DoRing(SHORT Weapon)
{return(SWext->DoRing(Weapon));}

void InitSpellRing(PLAYERp pp)
{SWext->InitSpellRing(pp);}

int DoSerpRing(SHORT Weapon)
{return(SWext->DoSerpRing(Weapon));}

int InitLavaFlame(short SpriteNum)
{return(SWext->InitLavaFlame(SpriteNum));}

int InitLavaThrow(short SpriteNum)
{return(SWext->InitLavaThrow(SpriteNum));}

int InitVulcanBoulder(short SpriteNum)
{return(SWext->InitVulcanBoulder(SpriteNum));}

int InitSerpRing(short SpriteNum)
{return(SWext->InitSerpRing(SpriteNum));}

void InitSpellNapalm(PLAYERp pp)
{SWext->InitSpellNapalm(pp);}

int InitEnemyNapalm(short SpriteNum)
{return(SWext->InitEnemyNapalm(SpriteNum));}

int InitSpellMirv(PLAYERp pp)
{return(SWext->InitSpellMirv(pp));}

int InitEnemyMirv(short SpriteNum)
{return(SWext->InitEnemyMirv(SpriteNum));}

int InitSwordAttack(PLAYERp pp)
{return(SWext->InitSwordAttack(pp));}

int WeaponAutoAim(SPRITEp sp,short Missile,short ang,BOOL test)
{return(SWext->WeaponAutoAim(sp,Missile,ang,test));}

int AimHitscanToTarget(SPRITEp sp,long *z,short *ang,long z_ratio)
{return(SWext->AimHitscanToTarget(sp,z,ang,z_ratio));}

int WeaponAutoAimHitscan(SPRITEp sp,long *z,short *ang,BOOL test)
{return(SWext->WeaponAutoAimHitscan(sp,z,ang,test));}

void WeaponHitscanShootFeet(SPRITEp sp,SPRITEp hp,long *zvect)
{SWext->WeaponHitscanShootFeet(sp,hp,zvect);}

int InitStar(PLAYERp pp)
{return(SWext->InitStar(pp));}

int InitHeartAttack(PLAYERp pp)
{return(SWext->InitHeartAttack(pp));}

int InitShotgun(PLAYERp pp)
{return(SWext->InitShotgun(pp));}

int InitLaser(PLAYERp pp)
{return(SWext->InitLaser(pp));}

int InitRail(PLAYERp pp)
{return(SWext->InitRail(pp));}

int InitRocket(PLAYERp pp)
{return(SWext->InitRocket(pp));}

int InitNuke(PLAYERp pp)
{return(SWext->InitNuke(pp));}

int InitMicro(PLAYERp pp)
{return(SWext->InitMicro(pp));}

int InitRipperSlash(short SpriteNum)
{return(SWext->InitRipperSlash(SpriteNum));}

int InitSerpSlash(short SpriteNum)
{return(SWext->InitSerpSlash(SpriteNum));}

BOOL WallSpriteInsideSprite(SPRITEp wsp,SPRITEp sp)
{return(SWext->WallSpriteInsideSprite(wsp,sp));}

int DoBladeDamage(short SpriteNum)
{return(SWext->DoBladeDamage(SpriteNum));}

int InitCoolgBash(short SpriteNum)
{return(SWext->InitCoolgBash(SpriteNum));}

int InitSkelSlash(short SpriteNum)
{return(SWext->InitSkelSlash(SpriteNum));}

int InitGoroChop(short SpriteNum)
{return(SWext->InitGoroChop(SpriteNum));}

int InitHornetSting(short SpriteNum)
{return(SWext->InitHornetSting(SpriteNum));}

int InitSerpSpell(short SpriteNum)
{return(SWext->InitSerpSpell(SpriteNum));}

int InitEnemyRocket(short SpriteNum)
{return(SWext->InitEnemyRocket(SpriteNum));}

int InitEnemyStar(short SpriteNum)
{return(SWext->InitEnemyStar(SpriteNum));}

int InitSkelSpell(short SpriteNum)
{return(SWext->InitSkelSpell(SpriteNum));}

int InitCoolgFire(short SpriteNum)
{return(SWext->InitCoolgFire(SpriteNum));}

int DoCoolgDrip(short SpriteNum)
{return(SWext->DoCoolgDrip(SpriteNum));}

int InitCoolgDrip(short SpriteNum)
{return(SWext->InitCoolgDrip(SpriteNum));}

int InitEelFire(short SpriteNum)
{return(SWext->InitEelFire(SpriteNum));}

int InitFireballTrap(short SpriteNum)
{return(SWext->InitFireballTrap(SpriteNum));}

int InitBoltTrap(short SpriteNum)
{return(SWext->InitBoltTrap(SpriteNum));}

int InitSpearTrap(short SpriteNum)
{return(SWext->InitSpearTrap(SpriteNum));}

int DoSuicide(short SpriteNum)
{return(SWext->DoSuicide(SpriteNum));}

int DoDefaultStat(short SpriteNum)
{return(SWext->DoDefaultStat(SpriteNum));}

int InitTracerUzi(PLAYERp pp)
{return(SWext->InitTracerUzi(pp));}

int InitTracerTurret(short SpriteNum,short Operator,long horiz)
{return(SWext->InitTracerTurret(SpriteNum,Operator,horiz));}

int InitTracerAutoTurret(short SpriteNum,short Operator,long xchange,long ychange,long zchange)
{return(SWext->InitTracerAutoTurret(SpriteNum,Operator,xchange,ychange,zchange));}

int BulletHitSprite(SPRITEp sp,short hitsprite,short hitsect,short hitwall,long hitx,long hity,long hitz,short ID)
{return(SWext->BulletHitSprite(sp,hitsprite,hitsect,hitwall,hitx,hity,hitz,ID));}

int SpawnWallHole(short hitsect,short hitwall,long hitx,long hity,long hitz)
{return(SWext->SpawnWallHole(hitsect,hitwall,hitx,hity,hitz));}

BOOL HitscanSpriteAdjust(short SpriteNum,short hitwall)
{return(SWext->HitscanSpriteAdjust(SpriteNum,hitwall));}

int InitUzi(PLAYERp pp)
{return(SWext->InitUzi(pp));}

int InitTankShell(short SpriteNum,PLAYERp pp)
{return(SWext->InitTankShell(SpriteNum,pp));}

int InitTurretRocket(short SpriteNum,PLAYERp pp)
{return(SWext->InitTurretRocket(SpriteNum,pp));}

int InitTurretFireball(short SpriteNum,PLAYERp pp)
{return(SWext->InitTurretFireball(SpriteNum,pp));}

int InitTurretRail(short SpriteNum,PLAYERp pp)
{return(SWext->InitTurretRail(SpriteNum,pp));}

int InitTurretLaser(short SpriteNum,PLAYERp pp)
{return(SWext->InitTurretLaser(SpriteNum,pp));}

int InitSobjMachineGun(short SpriteNum,PLAYERp pp)
{return(SWext->InitSobjMachineGun(SpriteNum,pp));}

int InitSobjGun(PLAYERp pp)
{return(SWext->InitSobjGun(pp));}

int SpawnBoatSparks(PLAYERp pp,short hitsect,short hitwall,long hitx,long hity,long hitz,short hitang)
{return(SWext->SpawnBoatSparks(pp,hitsect,hitwall,hitx,hity,hitz,hitang));}

int SpawnSwordSparks(PLAYERp pp,short hitsect,short hitwall,long hitx,long hity,long hitz,short hitang)
{return(SWext->SpawnSwordSparks(pp,hitsect,hitwall,hitx,hity,hitz,hitang));}

int SpawnTurretSparks(SPRITEp sp,short hitsect,short hitwall,long hitx,long hity,long hitz,short hitang)
{return(SWext->SpawnTurretSparks(sp,hitsect,hitwall,hitx,hity,hitz,hitang));}

int SpawnShotgunSparks(PLAYERp pp,short hitsect,short hitwall,long hitx,long hity,long hitz,short hitang)
{return(SWext->SpawnShotgunSparks(pp,hitsect,hitwall,hitx,hity,hitz,hitang));}

int InitTurretMgun(SECTOR_OBJECTp sop)
{return(SWext->InitTurretMgun(sop));}

int InitEnemyUzi(short SpriteNum)
{return(SWext->InitEnemyUzi(SpriteNum));}

int InitGrenade(PLAYERp pp)
{return(SWext->InitGrenade(pp));}

int InitSpriteGrenade(short SpriteNum)
{return(SWext->InitSpriteGrenade(SpriteNum));}

int InitMine(PLAYERp pp)
{return(SWext->InitMine(pp));}

int HelpMissileLateral(SHORT Weapon,long dist)
{return(SWext->HelpMissileLateral(Weapon,dist));}

int InitFireball(PLAYERp pp)
{return(SWext->InitFireball(pp));}

int InitEnemyFireball(short SpriteNum)
{return(SWext->InitEnemyFireball(SpriteNum));}

BOOL SpriteWarpToUnderwater(SPRITEp sp)
{return(SWext->SpriteWarpToUnderwater(sp));}

BOOL SpriteWarpToSurface(SPRITEp sp)
{return(SWext->SpriteWarpToSurface(sp));}

int SpawnSplash(short SpriteNum)
{return(SWext->SpawnSplash(SpriteNum));}

int SpawnSplashXY(long hitx,long hity,long hitz,short sectnum)
{return(SWext->SpawnSplashXY(hitx,hity,hitz,sectnum));}

int SpawnUnderSplash(short SpriteNum)
{return(SWext->SpawnUnderSplash(SpriteNum));}

BOOL MissileHitDiveArea(short SpriteNum)
{return(SWext->MissileHitDiveArea(SpriteNum));}

int SpawnBubble(short SpriteNum)
{return(SWext->SpawnBubble(SpriteNum));}

int DoVehicleSmoke(short SpriteNum)
{return(SWext->DoVehicleSmoke(SpriteNum));}

int SpawnVehicleSmoke(short SpriteNum)
{return(SWext->SpawnVehicleSmoke(SpriteNum));}

int DoBubble(short SpriteNum)
{return(SWext->DoBubble(SpriteNum));}

void QueueReset(void )
{SWext->QueueReset();}

BOOL TestDontStick(short SpriteNum,short hitsect,short hitwall,long hitz)
{return(SWext->TestDontStick(SpriteNum,hitsect,hitwall,hitz));}

BOOL TestDontStickSector(short hitsect)
{return(SWext->TestDontStickSector(hitsect));}

int QueueStar(short SpriteNum)
{return(SWext->QueueStar(SpriteNum));}

int QueueHole(short ang,short hitsect,short hitwall,long hitx,long hity,long hitz)
{return(SWext->QueueHole(ang,hitsect,hitwall,hitx,hity,hitz));}

int QueueFloorBlood(short hitsprite)
{return(SWext->QueueFloorBlood(hitsprite));}

int QueueFootPrint(short hitsprite)
{return(SWext->QueueFootPrint(hitsprite));}

int QueueWallBlood(short hitsprite,short ang)
{return(SWext->QueueWallBlood(hitsprite,ang));}

int DoFloorBlood(short SpriteNum)
{return(SWext->DoFloorBlood(SpriteNum));}

int DoWallBlood(short SpriteNum)
{return(SWext->DoWallBlood(SpriteNum));}

int SpawnShell(short SpriteNum,short ShellNum)
{return(SWext->SpawnShell(SpriteNum,ShellNum));}

int DoShrapVelocity(SHORT SpriteNum)
{return(SWext->DoShrapVelocity(SpriteNum));}

int ShrapKillSprite(short SpriteNum)
{return(SWext->ShrapKillSprite(SpriteNum));}

//BOOL CheckBreakToughness(BREAK_INFOp break_info,short ID)
//{return(SWext->CheckBreakToughness(break_info,ID));}

int DoItemFly(SHORT SpriteNum)
{return(SWext->DoItemFly(SpriteNum));}

int InitEnemyCrossbow(short SpriteNum)
{return(SWext->InitEnemyCrossbow(SpriteNum));}

VOID UpdateSinglePlayKills(short SpriteNum)
{SWext->UpdateSinglePlayKills(SpriteNum);}