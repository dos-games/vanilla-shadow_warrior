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

/****************************************************************************
*  CALLER.C  
*  Handles dynamic loading of OS/2 LE runtime dynamic link libraries
*
*  Copyright (c) Jim Norwood
*  All Rights Reserved
*  This file will only compile under DOS4G full version
*****************************************************************************
*/

// Standard headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// D32 API header, this is DOS4G specific
//#include <dos32lib.h>

#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "def.h"
#include "exports.h"

// Used to give debug info
#define DEBUGDLL    1

// Global flag used to notify functions of whether or not a user DLL is loaded
unsigned char DLL_Loaded = FALSE;
int DLL_Handle; // Global DLL handle
char *DLL_path; // DLL path name

// EXTERNS
extern BOOL RandomPrint;
extern unsigned long MoveThingsCount;
extern BOOL Prediction;


///PROTOTYPES/////////////////////////////////////////////////////////////////
void ExtractFileBase (char *path, char *dest);
int InitEnemyCrossbow(short SpriteNum);
VOID UpdateSinglePlayKills(short SpriteNum);
void PlaySoundRTS(long rts_num);
void (*DebugWriteString)(char *string);

//////////////////////////////////////////////////////////////////////////////
// SPECIAL DATA ACCESSOR FUNCTIONS

// User Array
USERp QueryUser(short SpriteNum)
    {
    ASSERT(User[SpriteNum]);
    return(User[SpriteNum]);
    }

BOOL SetUser(short SpriteNum, USERp u)
    {
    ASSERT(SpriteNum >= 0 && SpriteNum <= MAXSPRITES);
    ASSERT(u);
    User[SpriteNum] = u;
    return(TRUE);
    }

// Sprite Array    
SPRITEp QuerySprite(short SpriteNum)
    {
    ASSERT(SpriteNum >= 0 && SpriteNum <= MAXSPRITES);
    return(&sprite[SpriteNum]);
    }

// Sintable values    
short Querysintable(short val)
    {
    return(sintable[val]);
    }

// RandomPrint
BOOL QueryRandomPrint(void)
    {
    return(RandomPrint);
    }    
    
// Prediction
BOOL QueryPrediction(void)
    {
    return(Prediction);
    }    

// Randomseed    
long Queryrandomseed(void)
    {
    return(randomseed);
    }

unsigned long QueryMoveThingsCount(void)
    {
    return(MoveThingsCount);
    }    

long Querypicanm(short ndx)
    {
    return(Querypicanm(ndx));
    }

short Querytilesizx(short val)
    {
    return(tilesizx[val]);
    }

void Settilesizx(short ndx, short val)
    {
    tilesizx[ndx] = val;
    }

short Querytilesizy(short val)
    {
    return(tilesizy[val]);
    }

void Settilesizy(short ndx, short val)
    {
    tilesizy[ndx] = val;
    }

PLAYERp QueryPlayer(short ndx)
    {
    return(&Player[ndx]);
    }
        
short Queryconnecthead(void)
    {
    return(connecthead);
    }
    
short Queryconnectpoint2(short ndx)
    {
    return(connectpoint2[ndx]);
    }

TRACKp QueryTrack(short ndx)
    {
    return(&Track[ndx]);
    }

//////////////////////////////////////////////////////////////////////////////

// Export selected standard library functions since $%#@ing WATCOM doesn't
// support them in DLL's!
XREF_STDLIBp ELL;
void SetupExportStdLibList (void)
    {
    // Assign function addresses
    ELL->printf                 = printf;
    ELL->sprintf                = sprintf;
    ELL->rand                   = rand;
    }

// Export all the happy little Shadow Warrior functions.    
XREFp EL;
void SetupExportList (void)
    {
    EL = AllocMem(sizeof(XREF));

    // Accessors
    EL->QueryUser              = QueryUser;
    EL->SetUser                = SetUser;
    EL->QuerySprite            = QuerySprite;
    EL->Querysintable          = Querysintable;
    EL->QueryRandomPrint       = QueryRandomPrint;
    EL->QueryPrediction        = QueryPrediction;
    EL->Queryrandomseed        = Queryrandomseed;
    EL->QueryMoveThingsCount   = QueryMoveThingsCount;
    EL->Querytilesizx          = Querytilesizx;
    EL->Settilesizx            = Settilesizx;
    EL->Querytilesizy          = Querytilesizy;
    EL->Settilesizy            = Settilesizy;
    EL->Querypicanm            = Querypicanm;
    EL->QueryPlayer            = QueryPlayer;
    EL->Queryconnecthead       = Queryconnecthead;
    EL->Queryconnectpoint2     = Queryconnectpoint2;
    EL->QueryTrack             = QueryTrack;
    
    // Misc
    EL->change_sprite_stat     = change_sprite_stat;
    EL->NullAnimator           = NullAnimator;
    EL->EnemyDefaults          = EnemyDefaults;
    EL->AllocMem               = AllocMem;
    
    // ENGINE.C functions
    EL->krand2                 = krand2;
    EL->neartag                = neartag;
    EL->getangle               = getangle;
    EL->changespritesect       = changespritesect;
    
    // PLAYER.C functions
    EL->GetDeltaAngle          = GetDeltaAngle;
    
    // SECTOR.C functions
    EL->OperateSector          = OperateSector;
    EL->OperateWall            = OperateWall;

    // ROOMS.C functions
    EL->FAFcansee              = FAFcansee;
    EL->FAFhitscan             = FAFhitscan;
    EL->FAFgetzrange           = FAFgetzrange;
    EL->FAFgetzrangepoint      = FAFgetzrangepoint;
    EL->setsprite              = setsprite;
    EL->PicInView              = PicInView;
    
    // SOUNDS.C functions
    EL->PlayerSound            = PlayerSound;
    EL->PlaySound              = PlaySound;
    EL->PlaySoundRTS           = PlaySoundRTS;
    EL->Set3DSoundOwner        = Set3DSoundOwner;
    EL->PlaySpriteSound        = PlaySpriteSound;
    
    // GAME.C functions
    EL->DebugWriteString       = DebugWriteString;
    EL->Distance               = Distance;
    EL->RandomRange            = RandomRange;
    EL->krand                  = krand;
    EL->StdRandomRange         = StdRandomRange;
    
    // SPRITE.C functions
    EL->SpawnUser              = SpawnUser;
    EL->ChangeState            = ChangeState;
    EL->move_actor             = move_actor;
    EL->NewStateGroup          = NewStateGroup;
    EL->SetEnemyInactive       = SetEnemyInactive;
    EL->move_sprite            = move_sprite;
    EL->DropAhead              = DropAhead;
    
    // TRACK.C functions
    EL->ActorFollowTrack       = ActorFollowTrack;
    EL->ActorFindTrack         = ActorFindTrack;
    
    // JWEAPON.C functions
    EL->DoPhosphorus           = DoPhosphorus;
    EL->DoChemBomb             = DoChemBomb;
    EL->DoCaltrops             = DoCaltrops;
    EL->SpawnRadiationCloud    = SpawnRadiationCloud;
    EL->DoRadiationCloud       = DoRadiationCloud;
    EL->PlayerInitChemBomb     = PlayerInitChemBomb;
    EL->InitSpriteChemBomb     = InitSpriteChemBomb;
    EL->InitChemBomb           = InitChemBomb;
    EL->PlayerInitFlashBomb    = PlayerInitFlashBomb;
    EL->InitFlashBomb          = InitFlashBomb;
    EL->SpawnFlashBombOnActor  = SpawnFlashBombOnActor;
    EL->PlayerInitCaltrops     = PlayerInitCaltrops;
    EL->InitCaltrops           = InitCaltrops;
    EL->InitPhosphorus         = InitPhosphorus;
    EL->InitBloodSpray         = InitBloodSpray;
    // ai.c functions
    /*
    EL->DebugMoveHit           = DebugMoveHit;
    EL->ActorMoveHitReact      = ActorMoveHitReact;
    EL->RandomRange            = RandomRange;
    EL->StdRandomRange         = StdRandomRange;
    EL->ActorFlaming           = ActorFlaming;
    EL->DoActorSetSpeed        = DoActorSetSpeed;
    EL->ChooseActionNumber     = ChooseActionNumber;
    EL->DoActorNoise           = DoActorNoise;
    EL->CanSeePlayer           = CanSeePlayer;
    EL->CanHitPlayer           = CanHitPlayer;
    EL->DoActorPickClosePlayer = DoActorPickClosePlayer;
    EL->CloseRangeDist         = CloseRangeDist;
    EL->InitActorDecide        = InitActorDecide;
    EL->DoActorDecide          = DoActorDecide;
    EL->InitActorAlertNoise    = InitActorAlertNoise;
    EL->InitActorAmbientNoise  = InitActorAmbientNoise;
    EL->InitActorAttackNoise   = InitActorAttackNoise;
    EL->InitActorPainNoise     = InitActorPainNoise;
    EL->InitActorDieNoise      = InitActorDieNoise;
    EL->InitActorExtra1Noise   = InitActorExtra1Noise;
    EL->InitActorExtra2Noise   = InitActorExtra2Noise;
    EL->InitActorExtra3Noise   = InitActorExtra3Noise;
    EL->InitActorExtra4Noise   = InitActorExtra4Noise;
    EL->InitActorExtra5Noise   = InitActorExtra5Noise;
    EL->InitActorExtra6Noise   = InitActorExtra6Noise;
    EL->InitActorMoveCloser    = InitActorMoveCloser;
    EL->DoActorCantMoveCloser  = DoActorCantMoveCloser;
    EL->DoActorMoveCloser      = DoActorMoveCloser;
    EL->FindTrackToPlayer      = FindTrackToPlayer;
    EL->FindTrackAwayFromPlayer= FindTrackAwayFromPlayer;
    EL->FindWanderTrack        = FindWanderTrack;
    EL->InitActorRunAway       = InitActorRunAway;
    EL->InitActorRunToward     = InitActorRunToward;
    EL->InitActorAttack        = InitActorAttack;
    EL->DoActorAttack          = DoActorAttack;
    EL->InitActorEvade         = InitActorEvade;
    EL->InitActorWanderAround  = InitActorWanderAround;
    EL->InitActorFindPlayer    = InitActorFindPlayer;
    EL->InitActorDuck          = InitActorDuck;
    EL->DoActorDuck            = DoActorDuck;
    EL->DoActorMoveJump        = DoActorMoveJump;
    EL->move_scan              = move_scan;
    EL->FindNewAngle           = FindNewAngle;
    EL->InitActorReposition    = InitActorReposition;
    EL->DoActorReposition      = DoActorReposition;
    EL->InitActorPause         = InitActorPause;
    EL->DoActorPause           = DoActorPause;
    */
    
    // actor.c functions
    EL->DoActorDie             = DoActorDie;
    EL->DoDebrisCurrent        = DoDebrisCurrent;
    EL->DoActorSectorDamage    = DoActorSectorDamage;
    EL->move_debris            = move_debris;
    EL->DoActorDebris          = DoActorDebris;
    EL->DoFireFly              = DoFireFly;
    EL->DoGenerateSewerDebris  = DoGenerateSewerDebris;
    EL->KeepActorOnFloor       = KeepActorOnFloor;
    EL->DoActorBeginSlide      = DoActorBeginSlide;
    EL->DoActorSlide           = DoActorSlide;
    EL->DoActorBeginJump       = DoActorBeginJump;
    EL->DoActorJump            = DoActorJump;
    EL->DoActorBeginFall       = DoActorBeginFall;
    EL->DoActorFall            = DoActorFall;
    EL->DoActorStopFall        = DoActorStopFall;
    EL->DoActorDeathMove       = DoActorDeathMove;
    EL->DoBeginJump            = DoBeginJump;
    EL->DoJump                 = DoJump;
    EL->DoBeginFall            = DoBeginFall;
    EL->DoFall                 = DoFall;
    // weapon.c functions      
    EL->MissileHitMatch         = MissileHitMatch;
    EL->SpawnShrapX             = SpawnShrapX;
    EL->DoLavaErupt             = DoLavaErupt;
    EL->SpawnShrap              = SpawnShrap;
    EL->DoShrapMove             = DoShrapMove;
    EL->DoVomit                 = DoVomit;
    EL->DoVomitSplash           = DoVomitSplash;
    EL->DoFastShrapJumpFall     = DoFastShrapJumpFall;
    EL->DoTracerShrap           = DoTracerShrap;
    EL->DoShrapJumpFall         = DoShrapJumpFall;
    EL->DoShrapDamage           = DoShrapDamage;
    EL->SpawnBlood              = SpawnBlood;
    EL->VehicleMoveHit          = VehicleMoveHit;
    EL->WeaponMoveHit           = WeaponMoveHit;
    EL->DoUziSmoke              = DoUziSmoke;
    EL->DoShotgunSmoke          = DoShotgunSmoke;
    EL->DoMineSpark             = DoMineSpark;
    EL->DoFireballFlames        = DoFireballFlames;
    EL->SetSuicide              = SetSuicide;
    EL->DoActorScale            = DoActorScale;
    EL->DoRipperGrow            = DoRipperGrow;
    EL->ActorChooseDeath        = ActorChooseDeath;
    EL->ActorHealth             = ActorHealth;
    EL->SopDamage               = SopDamage;
    EL->SopCheckKill            = SopCheckKill;
    EL->ActorPain               = ActorPain;
    EL->ActorPainPlasma         = ActorPainPlasma;
    EL->ActorStdMissile         = ActorStdMissile;
    EL->ActorDamageSlide        = ActorDamageSlide;
    EL->PlayerDamageSlide       = PlayerDamageSlide;
    EL->GetDamage               = GetDamage;
    EL->RadiusGetDamage         = RadiusGetDamage;
    EL->PlayerCheckDeath        = PlayerCheckDeath;
    EL->StarBlood               = StarBlood;
    EL->DoDamage                = DoDamage;
    EL->DoDamageTest            = DoDamageTest;
    EL->DoHitscanDamage         = DoHitscanDamage;
    EL->DoFlamesDamageTest      = DoFlamesDamageTest;
    EL->PrevWall                = PrevWall;
    EL->TraverseBreakableWalls  = TraverseBreakableWalls;
    EL->DoExpDamageTest         = DoExpDamageTest;
    EL->DoMineExpMine           = DoMineExpMine;
    EL->DoStar                  = DoStar;
    EL->DoPlasmaDone            = DoPlasmaDone;
    EL->MissileSeek             = MissileSeek;
    EL->ComboMissileSeek        = ComboMissileSeek;
    EL->VectorMissileSeek       = VectorMissileSeek;
    EL->DoBlurExtend            = DoBlurExtend;
    EL->InitPlasmaFountain      = InitPlasmaFountain;
    EL->DoPlasmaFountain        = DoPlasmaFountain;
    EL->DoPlasma                = DoPlasma;
    EL->DoCoolgFire             = DoCoolgFire;
    EL->DoEelFire               = DoEelFire;
    EL->ScaleSpriteVector       = ScaleSpriteVector;
    EL->WallBounce              = WallBounce;
    EL->SlopeBounce             = SlopeBounce;
    EL->DoGrenade               = DoGrenade;
    EL->DoVulcanBoulder         = DoVulcanBoulder;
    EL->DoMineRangeTest         = DoMineRangeTest;
    EL->DoMineStuck             = DoMineStuck;
    EL->SetMineStuck            = SetMineStuck;
    EL->DoMine                  = DoMine;
    EL->DoPuff                  = DoPuff;
    EL->DoRailPuff              = DoRailPuff;
    EL->DoBoltThinMan           = DoBoltThinMan;
    EL->DoTracer                = DoTracer;
    EL->DoTankShell             = DoTankShell;
    EL->DoTracerStart           = DoTracerStart;
    EL->DoLaser                 = DoLaser;
    EL->DoLaserStart            = DoLaserStart;
    EL->DoRail                  = DoRail;
    EL->DoRailStart             = DoRailStart;
    EL->DoRocket                = DoRocket;
    EL->DoMicroMini             = DoMicroMini;
    EL->SpawnExtraMicroMini     = SpawnExtraMicroMini;
    EL->DoMicro                 = DoMicro;
    EL->DoUziBullet             = DoUziBullet;
    EL->DoBoltSeeker            = DoBoltSeeker;
    EL->DoBoltShrapnel          = DoBoltShrapnel;
    EL->DoBoltFatMan            = DoBoltFatMan;
    EL->DoElectro               = DoElectro;
    EL->DoLavaBoulder           = DoLavaBoulder;
    EL->DoSpear                 = DoSpear;
    EL->SpawnCoolieExp          = SpawnCoolieExp;
    EL->SpawnBasicExp           = SpawnBasicExp;
    EL->SpawnFireballFlames     = SpawnFireballFlames;
    EL->SpawnFireballExp        = SpawnFireballExp;
    EL->SpawnGoroFireballExp    = SpawnGoroFireballExp;
    EL->SpawnBoltExp            = SpawnBoltExp;
    EL->SpawnTankShellExp       = SpawnTankShellExp;
    EL->SpawnNuclearSecondaryExp= SpawnNuclearSecondaryExp;
    EL->SpawnNuclearExp         = SpawnNuclearExp;
    EL->SpawnTracerExp          = SpawnTracerExp;
    EL->SpawnMicroExp           = SpawnMicroExp;
    EL->AddSpriteToSectorObject = AddSpriteToSectorObject;
    EL->SpawnBigGunFlames       = SpawnBigGunFlames;
    EL->SpawnGrenadeSecondaryExp= SpawnGrenadeSecondaryExp;
    EL->SpawnGrenadeSmallExp    = SpawnGrenadeSmallExp;
    EL->SpawnGrenadeExp         = SpawnGrenadeExp;
    EL->SpawnExpZadjust         = SpawnExpZadjust;
    EL->SpawnMineExp            = SpawnMineExp;
    EL->InitMineShrap           = InitMineShrap;
    EL->DoMineExp               = DoMineExp;
    EL->DoSectorExp             = DoSectorExp;
    EL->SpawnSectorExp          = SpawnSectorExp;
    EL->SpawnLargeExp           = SpawnLargeExp;
    EL->SpawnMeteorExp          = SpawnMeteorExp;
    EL->DoFireball              = DoFireball;
    EL->DoFindGround            = DoFindGround;
    EL->DoFindGroundPoint       = DoFindGroundPoint;
    EL->DoNapalm                = DoNapalm;
    EL->DoMeteor                = DoMeteor;
    EL->DoSerpMeteor            = DoSerpMeteor;
    EL->DoMirvMissile           = DoMirvMissile;
    EL->DoMirv                  = DoMirv;
    EL->MissileSetPos           = MissileSetPos;
    EL->DoRing                  = DoRing;
    EL->InitSpellRing           = InitSpellRing;
    EL->DoSerpRing              = DoSerpRing;
    EL->InitLavaFlame           = InitLavaFlame;
    EL->InitLavaThrow           = InitLavaThrow;
    EL->InitVulcanBoulder       = InitVulcanBoulder;
    EL->InitSerpRing            = InitSerpRing;
    EL->InitSpellNapalm         = InitSpellNapalm;
    EL->InitEnemyNapalm         = InitEnemyNapalm;
    EL->InitSpellMirv           = InitSpellMirv;
    EL->InitEnemyMirv           = InitEnemyMirv;
    EL->InitSwordAttack         = InitSwordAttack;
    EL->WeaponAutoAim           = WeaponAutoAim;
    EL->AimHitscanToTarget      = AimHitscanToTarget;
    EL->WeaponAutoAimHitscan    = WeaponAutoAimHitscan;
    EL->WeaponHitscanShootFeet  = WeaponHitscanShootFeet;
    EL->InitStar                = InitStar;
    EL->InitHeartAttack         = InitHeartAttack;
    EL->InitShotgun             = InitShotgun;
    EL->InitLaser               = InitLaser;
    EL->InitRail                = InitRail;
    EL->InitRocket              = InitRocket;
    EL->InitNuke                = InitNuke;
    EL->InitMicro               = InitMicro;
    EL->InitRipperSlash         = InitRipperSlash;
    EL->InitSerpSlash           = InitSerpSlash;
    EL->WallSpriteInsideSprite  = WallSpriteInsideSprite;
    EL->DoBladeDamage           = DoBladeDamage;
    EL->InitCoolgBash           = InitCoolgBash;
    EL->InitSkelSlash           = InitSkelSlash;
    EL->InitGoroChop            = InitGoroChop;
    EL->InitHornetSting         = InitHornetSting;
    EL->InitSerpSpell           = InitSerpSpell;
    EL->InitEnemyRocket         = InitEnemyRocket;
    EL->InitEnemyStar           = InitEnemyStar;
    EL->InitSkelSpell           = InitSkelSpell;
    EL->InitCoolgFire           = InitCoolgFire;
    EL->DoCoolgDrip             = DoCoolgDrip;
    EL->InitCoolgDrip           = InitCoolgDrip;
    EL->InitEelFire             = InitEelFire;
    EL->InitFireballTrap        = InitFireballTrap;
    EL->InitBoltTrap            = InitBoltTrap;
    EL->InitSpearTrap           = InitSpearTrap;
    EL->DoSuicide               = DoSuicide;
    EL->DoDefaultStat           = DoDefaultStat;
    EL->InitTracerUzi           = InitTracerUzi;
    EL->InitTracerTurret        = InitTracerTurret;
    EL->InitTracerAutoTurret    = InitTracerAutoTurret;
    EL->BulletHitSprite         = BulletHitSprite;
    EL->SpawnWallHole           = SpawnWallHole;
    EL->HitscanSpriteAdjust     = HitscanSpriteAdjust;
    EL->InitUzi                 = InitUzi;
    EL->InitTankShell           = InitTankShell;
    EL->InitTurretRocket        = InitTurretRocket;
    EL->InitTurretFireball      = InitTurretFireball;
    EL->InitTurretRail          = InitTurretRail;
    EL->InitTurretLaser         = InitTurretLaser;
    EL->InitSobjMachineGun      = InitSobjMachineGun;
    EL->InitSobjGun             = InitSobjGun;
    EL->SpawnBoatSparks         = SpawnBoatSparks;
    EL->SpawnSwordSparks        = SpawnSwordSparks;
    EL->SpawnTurretSparks       = SpawnTurretSparks;
    EL->SpawnShotgunSparks      = SpawnShotgunSparks;
    EL->InitTurretMgun          = InitTurretMgun;
    EL->InitEnemyUzi            = InitEnemyUzi;
    EL->InitGrenade             = InitGrenade;
    EL->InitSpriteGrenade       = InitSpriteGrenade;
    EL->InitMine                = InitMine;
    EL->HelpMissileLateral      = HelpMissileLateral;
    EL->InitFireball            = InitFireball;
    EL->InitEnemyFireball       = InitEnemyFireball;
    EL->SpriteWarpToUnderwater  = SpriteWarpToUnderwater;
    EL->SpriteWarpToSurface     = SpriteWarpToSurface;
    EL->SpawnSplash             = SpawnSplash;
    EL->SpawnSplashXY           = SpawnSplashXY;
    EL->SpawnUnderSplash        = SpawnUnderSplash;
    EL->MissileHitDiveArea      = MissileHitDiveArea;
    EL->SpawnBubble             = SpawnBubble;
    EL->DoVehicleSmoke          = DoVehicleSmoke;
    EL->SpawnVehicleSmoke       = SpawnVehicleSmoke;
    EL->DoBubble                = DoBubble;
    EL->QueueReset              = QueueReset;
    EL->TestDontStick           = TestDontStick;
    EL->TestDontStickSector     = TestDontStickSector;
    EL->QueueStar               = QueueStar;
    EL->QueueHole               = QueueHole;
    EL->QueueFloorBlood         = QueueFloorBlood;
    EL->QueueFootPrint          = QueueFootPrint;
    EL->QueueWallBlood          = QueueWallBlood;
    EL->DoFloorBlood            = DoFloorBlood;
    EL->DoWallBlood             = DoWallBlood;
    EL->SpawnShell              = SpawnShell;
    EL->DoShrapVelocity         = DoShrapVelocity;
    EL->ShrapKillSprite         = ShrapKillSprite;
    EL->DoItemFly               = DoItemFly;
    EL->InitEnemyCrossbow       = InitEnemyCrossbow;
    EL->UpdateSinglePlayKills   = UpdateSinglePlayKills;
    }                          

//////////////////////////////////////////////////////////////////////////////
#if 0

// Load a DLL 
//
// This program builds two ways.  /D_USE_IMPLIB_ causes it to use 
// an implib to load the DLL in the usual manner, at program load-time.
// If _USE_IMPLIB_ is not specified, then the dynamic loading and
// calling of the DLL comes o play.
//	 
// Returns a handle to the loaded DLL is successful, otherwise it returns -1     
// char DLLpathname[PATH_MAX+64];		
 DLL_Load(char *DLLpathname)
    {
    #ifndef _USE_IMPLIB_	 // Use D32LoadModule() and D32QueryProcAddr()
    char DLLname[64];
    char modName[9];
    TSF32 tsfregs;
     procHandle;
    
    // Make sure a proper DLL file name is constructed
    ExtractFileBase (DLLpathname, DLLname);
    strcat(DLLname,".DLL");
    
	//
	// Load the DLL dynamically...
	//
	procHandle = D32LoadModule(DLLname,"\x00\r" ,&tsfregs);

	if (procHandle) 
   	    {
        // report the path and filename of the dll loaded
	    D32QueryModuleFileName(procHandle,DLLpathname,PATH_MAX+64);
        prf("D32QueryModuleFileName: %s\n",DLLpathname);
        
		// report the dll's module name
        D32QueryModuleName(procHandle,modName,9);
        prf("D32QueryModuleName: %s\n",modName);
      
		// report the handle of the loaded dll module
        prf("D32QueryModuleHandle: %d\n",D32QueryModuleHandle(modName));

        prf("DLL was successfully loaded.\n");
        return(procHandle);
        
        } else
    		prf("D32LoadModule: failed to load module %s\n",DLLname);
    
    return(-1);
        
    #endif
    }

// Frees a dll    
// Returns TRUE if DLL was successfully released
BOOL DLL_Unload( procHandle)
    {    
    #ifndef _USE_IMPLIB_	 // Function exists only if runtime DLL's are used
	// unload the DLL and exit gracefully
	if (!D32UnloadModule(procHandle))
        {
		prf("Couldn't unload %d.\n", procHandle);
        return(TRUE);
        }
    return(FALSE);    
    #endif
    }
    
// Executes and function contained in a specified DLL    
// Returns TRUE on success
BOOL DLL_ExecFunc( procHandle, char *fName)
    {
    #ifndef _USE_IMPLIB_	 // Function exists only if runtime DLL's are used
	 retcode;
    DWORD (*fp)();
    

	if (procHandle) // use the module via the handle returned by D32LoadModule()
		{
		// If building with the -#r switch enabled, for register based passing, 
        // "function name is padded with '_' character"
		strcat(fName, "_");

		// Find the DLL entry po by name...
        // Note that the ordinals are ignored when a function name is passed in
	    fp = D32QueryProcAddr(()procHandle,(DWORD) 0,(char *)fName,(DWORD *)&fp);
        
		if (fp) 
          	{
            #if DEBUGDLL
			prf("D32QueryProcAddr(%d,0,%s) returned %#X\n",procHandle,fName,fp);
            #endif

            // call the function in the .dll
			retcode = (*fp) ();

            #if DEBUGDLL
			prf("%s, got back %d.\n", fName,retcode);	
            #endif

			fflush(stdout);		
            return(TRUE);
			} 
            #if DEBUGDLL    
            else 
           		prf("Couldn't find %d.%s\n",procHandle,fName);
            #endif    
       }

       return(FALSE);
    #endif    
    }

#endif        