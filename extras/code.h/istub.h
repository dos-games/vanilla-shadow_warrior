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

// Exported function prototypes
// These local caller equivalents are defined in istub.c
// Copyright(c)1997 Jim Norwood and 3D Realms Inc.

// Special Accessor Functions
USERp QueryUser(short SpriteNum);
BOOL SetUser(short SpriteNum, USERp u);
SPRITEp QuerySprite(short SpriteNum);
short Querysintable(short val);
BOOL QueryRandomPrint(void);
BOOL QueryPrediction(void);
long Queryrandomseed(void);
unsigned long QueryMoveThingsCount(void);
short Querytilesizx(short val);
void Settilesizx(short ndx, short val);
short Querytilesizy(short val);
void Settilesizy(short ndx, short val);
long Querypicanm(short ndx);
PLAYERp QueryPlayer(short ndx);
short Queryconnecthead(void);
short Queryconnectpoint2(short ndx);
TRACKp QueryTrack(short ndx);

// Special Accessor MACRO redefinitions
#define QTRAVERSE_CONNECT(i)   for (i = Queryconnecthead(); i != -1; i = Queryconnectpoint2(i))
#define QNORM_SPRITE(val) ((val) & (SIZ(QuerySprite(0)) - 1))
#define QSPRITEp_SIZE_X(sp)   (((sp)->xrepeat == 64) ?                         \
                    Querytilesizx((sp)->picnum) :                   \
                    (((sp)->xrepeat * Querytilesizx((sp)->picnum)) >> 6) \
                )  
                      
#define QSPRITEp_SIZE_Y(sp)   (((sp)->yrepeat == 64) ?                          \
                    Querytilesizy((sp)->picnum) :                    \
                    (((sp)->yrepeat * Querytilesizy((sp)->picnum)) >> 6) \
                )       
                    
#define QSPRITEp_SIZE_Z(sp)   (((sp)->yrepeat == 64) ?                          \
                    Z(Querytilesizy((sp)->picnum)) :                 \
                    (((sp)->yrepeat * Querytilesizy((sp)->picnum)) << 2) \
                )      

// x & y offset of tile
#define QTILE_XOFF(picnum) ((CHAR)TEST(Querypicanm((picnum)) >> 8, 0xFF))
#define QTILE_YOFF(picnum) ((CHAR)TEST(Querypicanm((picnum)) >> 16, 0xFF))

// x & y offset of current sprite tile
#define QSPRITEp_XOFF(sp) ((CHAR)TEST(Querypicanm((sp)->picnum) >> 8, 0xFF))
#define QSPRITEp_YOFF(sp) ((CHAR)TEST(Querypicanm((sp)->picnum) >> 16, 0xFF))

// Z size of top (TOS) and bottom (BOS) part of sprite
#define QSPRITEp_SIZE_TOS(sp) (DIV2(QSPRITEp_SIZE_Z(sp)) + Z(QSPRITEp_YOFF(sp)))
#define QSPRITEp_SIZE_BOS(sp) (DIV2(QSPRITEp_SIZE_Z(sp)) - Z(QSPRITEp_YOFF(sp)))

// acual Z for TOS and BOS - handles both WYSIWYG and old style
#define QSPRITEp_TOS(sp) (TEST((sp)->cstat, CSTAT_SPRITE_YCENTER) ? \
            ((sp)->z - QSPRITEp_SIZE_TOS(sp)) :         \
            ((sp)->z - QSPRITEp_SIZE_Z(sp)))

#define QSPRITEp_BOS(sp) (TEST((sp)->cstat, CSTAT_SPRITE_YCENTER) ? \
            ((sp)->z + QSPRITEp_SIZE_BOS(sp)) :         \
            (sp)->z)
            
// mid and upper/lower sprite caluculations            
#define QSPRITEp_MID(sp) (DIV2(QSPRITEp_TOS(sp) + QSPRITEp_BOS(sp)))
#define QSPRITEp_UPPER(sp) (QSPRITEp_TOS(sp) + DIV4(QSPRITEp_SIZE_Z(sp)))
#define QSPRITEp_LOWER(sp) (QSPRITEp_BOS(sp) - DIV4(QSPRITEp_SIZE_Z(sp)))

////////////////////////////////////////////////////////////////////////////////

// STDLIB FUNCTIONS
int printf(const char *format, ...);
int sprintf( char *buf, const char *format, ... );
int rand( void );

// SW FUNCTIONS
void *AllocMem(int);

void change_sprite_stat(short spritenum, short newstatnum);
int NullAnimator(short SpriteNum);

VOID EnemyDefaults(short SpriteNum, ACTOR_ACTION_SETp action, PERSONALITYp person);

// ENGINE.C functions
int krand2(void);
int neartag(long xs,long ys,long zs,short sectnum,short ange,short *neartagsector,short *neartagwall,short *neartagsprite,long *neartaghitdist,long neartagrange,char tagsearch);
int getangle(long xvect,long yvect);
int changespritesect(short spritenum,short newsectnum);

// PLAYER.C functions
short GetDeltaAngle(short ang1,short ang2);

// SECTOR.C functions
int OperateSector(short sectnum, short player_is_operating);
int OperateWall(short wallnum,short player_is_operating);

// ROOMS.C functions
BOOL FAFcansee(LONG xs, LONG ys, LONG zs, SHORT sects,LONG xe, LONG ye, LONG ze, SHORT secte);
void FAFhitscan(LONG x,LONG y,LONG z,SHORT sectnum,LONG xvect,LONG yvect,LONG zvect,SHORTp hitsect,SHORTp hitwall,SHORTp hitsprite,LONGp hitx,LONGp hity,LONGp hitz,LONG clipmask);
void FAFgetzrange(LONG x,LONG y,LONG z,SHORT sectnum,LONGp hiz,LONGp ceilhit,LONGp loz,LONGp florhit,LONG clipdist,LONG clipmask);
void FAFgetzrangepoint(LONG x,LONG y,LONG z,SHORT sectnum,LONGp hiz,LONGp ceilhit,LONGp loz,LONGp florhit);
int setsprite(short spritenum,long newx,long newy,long newz);
BOOL PicInView(short tile_num,BOOL reset);

// SOUNDS.C functions
void PlayerSound(int num, long *x, long *y, long *z, Voc3D_Flags flags, PLAYERp pp);
int PlaySound(int num, long *x, long *y, long *z, Voc3D_Flags flags);
void PlaySoundRTS(long rts_num);
void Set3DSoundOwner(short spritenum);
void PlaySpriteSound(short spritenum, int attrib_ndx, Voc3D_Flags flags);

// GAME.C functions
void DebugWriteString(char *string);
int Distance(int x1,int y1,int x2,int y2);
#if RANDOM_DEBUG
int RandomRange(int, char*, unsigned);
int krand(char *file, unsigned line);
#else
int RandomRange(int range);
int krand(void);
#endif
int StdRandomRange(int range);

// SPRITE.C functions
USERp SpawnUser(short SpriteNum, short id, STATEp state);
void ChangeState(short SpriteNum,STATEp statep);
int move_actor(short SpriteNum, long xchange, long ychange, long zchange);
void NewStateGroup(short SpriteNum,STATEp StateGroup[]);
void SetEnemyInactive(short SpriteNum);
int move_sprite(short spritenum,long xchange,long ychange,long zchange,long ceildist,long flordist,ULONG cliptype,long numtics);
BOOL DropAhead(short SpriteNum,short min_height);

// TRACK.C functions
int ActorFollowTrack(short SpriteNum, short locktics);
short ActorFindTrack(short SpriteNum,CHAR player_dir,long track_type,short *track_point_num,short *track_dir);

// JWEAPON.C functions
int DoPhosphorus(SHORT Weapon);
int DoChemBomb(SHORT Weapon);
int DoCaltrops(SHORT Weapon);
int SpawnRadiationCloud(short SpriteNum);
int DoRadiationCloud(short SpriteNum);
int PlayerInitChemBomb(PLAYERp pp);
int InitSpriteChemBomb(SHORT SpriteNum);
int InitChemBomb(short SpriteNum);
int PlayerInitFlashBomb(PLAYERp pp);
int InitFlashBomb(SHORT SpriteNum);
int SpawnFlashBombOnActor(SHORT enemy);
int PlayerInitCaltrops(PLAYERp pp);
int InitCaltrops(SHORT SpriteNum);
int InitPhosphorus(SHORT SpriteNum);
int InitBloodSpray(SHORT SpriteNum, BOOL dogib, short velocity);

// ACTOR.C functions
int DoActorDie(short SpriteNum,short weapon);
void DoDebrisCurrent(SPRITEp sp);
int DoActorSectorDamage(short SpriteNum);
int move_debris(short SpriteNum,long xchange,long ychange,long zchange);
int DoActorDebris(short SpriteNum);
int DoFireFly(short SpriteNum);
int DoGenerateSewerDebris(short SpriteNum);
void KeepActorOnFloor(short SpriteNum);
int DoActorBeginSlide(short SpriteNum,short ang,short vel,short dec);
int DoActorSlide(short SpriteNum);
int DoActorBeginJump(short SpriteNum);
int DoActorJump(short SpriteNum);
int DoActorBeginFall(short SpriteNum);
int DoActorFall(short SpriteNum);
int DoActorStopFall(short SpriteNum);
int DoActorDeathMove(short SpriteNum);
int DoBeginJump(short SpriteNum);
int DoJump(short SpriteNum);
int DoBeginFall(short SpriteNum);
int DoFall(short SpriteNum);

// WEAPON.C functions
BOOL MissileHitMatch(short Weapon,short WeaponNum,short hitsprite);
int SpawnShrapX(short SpriteNum);
int DoLavaErupt(short SpriteNum);
//void UserShrapSetup(SHRAPp shrap,STATEp state,long num_shrap);
int SpawnShrap(short ParentNum,short Secondary);
int DoShrapMove(SHORT SpriteNum);
int DoVomit(short SpriteNum);
int DoVomitSplash(short SpriteNum);
int DoFastShrapJumpFall(short SpriteNum);
int DoTracerShrap(short SpriteNum);
int DoShrapJumpFall(short SpriteNum);
int DoShrapDamage(short SpriteNum);
int SpawnBlood(short SpriteNum,short Weapon,short hitang,long hitx,long hity,long hitz);
BOOL VehicleMoveHit(short SpriteNum);
BOOL WeaponMoveHit(short SpriteNum);
int DoUziSmoke(short SpriteNum);
int DoShotgunSmoke(short SpriteNum);
int DoMineSpark(short SpriteNum);
int DoFireballFlames(short SpriteNum);
int SetSuicide(short SpriteNum);
int DoActorScale(short SpriteNum);
int DoRipperGrow(short SpriteNum);
int ActorChooseDeath(short SpriteNum,short Weapon);
int ActorHealth(short SpriteNum,short amt);
int SopDamage(SECTOR_OBJECTp sop,short amt);
int SopCheckKill(SECTOR_OBJECTp sop);
int ActorPain(short SpriteNum);
int ActorPainPlasma(short SpriteNum);
int ActorStdMissile(short SpriteNum,short Weapon);
int ActorDamageSlide(short SpriteNum,short damage,short ang);
int PlayerDamageSlide(PLAYERp pp,short damage,short ang);
int GetDamage(short SpriteNum,short Weapon,short DamageNdx);
int RadiusGetDamage(short SpriteNum,short Weapon,long max_damage);
int PlayerCheckDeath(PLAYERp pp,short Weapon);
int StarBlood(short SpriteNum,short Weapon);
int DoDamage(short SpriteNum,short Weapon);
int DoDamageTest(short Weapon);
int DoHitscanDamage(short Weapon,short hitsprite);
int DoFlamesDamageTest(short Weapon);
short PrevWall(short wall_num);
void TraverseBreakableWalls(short start_sect,long x,long y,long z,short ang,long radius);
int DoExpDamageTest(short Weapon);
int DoMineExpMine(short Weapon);
int DoStar(SHORT Weapon);
int DoPlasmaDone(SHORT Weapon);
int MissileSeek(SHORT Weapon,SHORT delay_tics,SHORT aware_range,SHORT dang_shift,SHORT turn_limit,SHORT z_limit);
int ComboMissileSeek(SHORT Weapon,SHORT delay_tics,SHORT aware_range,SHORT dang_shift,SHORT turn_limit,SHORT z_limit);
int VectorMissileSeek(SHORT Weapon,SHORT delay_tics,SHORT aware_range1,SHORT aware_range2);
int DoBlurExtend(SHORT Weapon,SHORT interval,SHORT blur_num);
int InitPlasmaFountain(SPRITEp wp,SPRITEp sp);
int DoPlasmaFountain(SHORT Weapon);
int DoPlasma(SHORT Weapon);
int DoCoolgFire(SHORT Weapon);
int DoEelFire(short Weapon);
void ScaleSpriteVector(short SpriteNum,long scale);
void WallBounce(short SpriteNum,short ang);
BOOL SlopeBounce(short SpriteNum,BOOL *hitwall);
int DoGrenade(SHORT Weapon);
int DoVulcanBoulder(SHORT Weapon);
int DoMineRangeTest(short Weapon,short range);
int DoMineStuck(SHORT Weapon);
int SetMineStuck(SHORT Weapon);
int DoMine(SHORT Weapon);
int DoPuff(short SpriteNum);
int DoRailPuff(short SpriteNum);
int DoBoltThinMan(SHORT Weapon);
int DoTracer(SHORT Weapon);
int DoTankShell(SHORT Weapon);
int DoTracerStart(SHORT Weapon);
int DoLaser(SHORT Weapon);
int DoLaserStart(SHORT Weapon);
int DoRail(SHORT Weapon);
int DoRailStart(SHORT Weapon);
int DoRocket(SHORT Weapon);
int DoMicroMini(SHORT Weapon);
int SpawnExtraMicroMini(SHORT Weapon);
int DoMicro(SHORT Weapon);
int DoUziBullet(SHORT Weapon);
int DoBoltSeeker(SHORT Weapon);
int DoBoltShrapnel(SHORT Weapon);
int DoBoltFatMan(SHORT Weapon);
int DoElectro(SHORT Weapon);
int DoLavaBoulder(SHORT Weapon);
int DoSpear(SHORT Weapon);
int SpawnCoolieExp(short SpriteNum);
int SpawnBasicExp(SHORT Weapon);
int SpawnFireballFlames(SHORT SpriteNum,SHORT enemy);
int SpawnFireballExp(SHORT Weapon);
int SpawnGoroFireballExp(SHORT Weapon);
int SpawnBoltExp(SHORT Weapon);
int SpawnTankShellExp(SHORT Weapon);
int SpawnNuclearSecondaryExp(SHORT Weapon,short ang);
int SpawnNuclearExp(SHORT Weapon);
int SpawnTracerExp(SHORT Weapon);
int SpawnMicroExp(SHORT Weapon);
int AddSpriteToSectorObject(short SpriteNum,SECTOR_OBJECTp sop);
int SpawnBigGunFlames(SHORT Weapon,SHORT Operator,SECTOR_OBJECTp sop);
int SpawnGrenadeSecondaryExp(SHORT Weapon,short ang);
int SpawnGrenadeSmallExp(SHORT Weapon);
int SpawnGrenadeExp(SHORT Weapon);
void SpawnExpZadjust(short Weapon,SPRITEp exp,long upper_zsize,long lower_zsize);
int SpawnMineExp(SHORT Weapon);
int InitMineShrap(short SpriteNum);
int DoMineExp(short SpriteNum);
int DoSectorExp(SHORT SpriteNum);
int SpawnSectorExp(SHORT Weapon);
int SpawnLargeExp(SHORT Weapon);
int SpawnMeteorExp(SHORT Weapon);
int DoFireball(SHORT Weapon);
int DoFindGround(SHORT SpriteNum);
int DoFindGroundPoint(SHORT SpriteNum);
int DoNapalm(SHORT Weapon);
int DoMeteor(SHORT Weapon);
int DoSerpMeteor(SHORT Weapon);
int DoMirvMissile(SHORT Weapon);
int DoMirv(SHORT Weapon);
BOOL MissileSetPos(short Weapon,ANIMATORp DoWeapon,long dist);
int DoRing(SHORT Weapon);
void InitSpellRing(PLAYERp pp);
int DoSerpRing(SHORT Weapon);
int InitLavaFlame(short SpriteNum);
int InitLavaThrow(short SpriteNum);
int InitVulcanBoulder(short SpriteNum);
int InitSerpRing(short SpriteNum);
void InitSpellNapalm(PLAYERp pp);
int InitEnemyNapalm(short SpriteNum);
int InitSpellMirv(PLAYERp pp);
int InitEnemyMirv(short SpriteNum);
int InitSwordAttack(PLAYERp pp);
int WeaponAutoAim(SPRITEp sp,short Missile,short ang,BOOL test);
int AimHitscanToTarget(SPRITEp sp,long *z,short *ang,long z_ratio);
int WeaponAutoAimHitscan(SPRITEp sp,long *z,short *ang,BOOL test);
void WeaponHitscanShootFeet(SPRITEp sp,SPRITEp hp,long *zvect);
int InitStar(PLAYERp pp);
int InitHeartAttack(PLAYERp pp);
int InitShotgun(PLAYERp pp);
int InitLaser(PLAYERp pp);
int InitRail(PLAYERp pp);
int InitRocket(PLAYERp pp);
int InitNuke(PLAYERp pp);
int InitMicro(PLAYERp pp);
int InitRipperSlash(short SpriteNum);
int InitSerpSlash(short SpriteNum);
BOOL WallSpriteInsideSprite(SPRITEp wsp,SPRITEp sp);
int DoBladeDamage(short SpriteNum);
int InitCoolgBash(short SpriteNum);
int InitSkelSlash(short SpriteNum);
int InitGoroChop(short SpriteNum);
int InitHornetSting(short SpriteNum);
int InitSerpSpell(short SpriteNum);
int InitEnemyRocket(short SpriteNum);
int InitEnemyStar(short SpriteNum);
int InitSkelSpell(short SpriteNum);
int InitCoolgFire(short SpriteNum);
int DoCoolgDrip(short SpriteNum);
int InitCoolgDrip(short SpriteNum);
int InitEelFire(short SpriteNum);
int InitFireballTrap(short SpriteNum);
int InitBoltTrap(short SpriteNum);
int InitSpearTrap(short SpriteNum);
int DoSuicide(short SpriteNum);
int DoDefaultStat(short SpriteNum);
int InitTracerUzi(PLAYERp pp);
int InitTracerTurret(short SpriteNum,short Operator,long horiz);
int InitTracerAutoTurret(short SpriteNum,short Operator,long xchange,long ychange,long zchange);
int BulletHitSprite(SPRITEp sp,short hitsprite,short hitsect,short hitwall,long hitx,long hity,long hitz,short ID);
int SpawnWallHole(short hitsect,short hitwall,long hitx,long hity,long hitz);
BOOL HitscanSpriteAdjust(short SpriteNum,short hitwall);
int InitUzi(PLAYERp pp);
int InitTankShell(short SpriteNum,PLAYERp pp);
int InitTurretRocket(short SpriteNum,PLAYERp pp);
int InitTurretFireball(short SpriteNum,PLAYERp pp);
int InitTurretRail(short SpriteNum,PLAYERp pp);
int InitTurretLaser(short SpriteNum,PLAYERp pp);
int InitSobjMachineGun(short SpriteNum,PLAYERp pp);
int InitSobjGun(PLAYERp pp);
int SpawnBoatSparks(PLAYERp pp,short hitsect,short hitwall,long hitx,long hity,long hitz,short hitang);
int SpawnSwordSparks(PLAYERp pp,short hitsect,short hitwall,long hitx,long hity,long hitz,short hitang);
int SpawnTurretSparks(SPRITEp sp,short hitsect,short hitwall,long hitx,long hity,long hitz,short hitang);
int SpawnShotgunSparks(PLAYERp pp,short hitsect,short hitwall,long hitx,long hity,long hitz,short hitang);
int InitTurretMgun(SECTOR_OBJECTp sop);
int InitEnemyUzi(short SpriteNum);
int InitGrenade(PLAYERp pp);
int InitSpriteGrenade(short SpriteNum);
int InitMine(PLAYERp pp);
int HelpMissileLateral(SHORT Weapon,long dist);
int InitFireball(PLAYERp pp);
int InitEnemyFireball(short SpriteNum);
BOOL SpriteWarpToUnderwater(SPRITEp sp);
BOOL SpriteWarpToSurface(SPRITEp sp);
int SpawnSplash(short SpriteNum);
int SpawnSplashXY(long hitx,long hity,long hitz,short);
int SpawnUnderSplash(short SpriteNum);
BOOL MissileHitDiveArea(short SpriteNum);
int SpawnBubble(short SpriteNum);
int DoVehicleSmoke(short SpriteNum);
int SpawnVehicleSmoke(short SpriteNum);
int DoBubble(short SpriteNum);
void QueueReset(void );
BOOL TestDontStick(short SpriteNum,short hitsect,short hitwall,long hitz);
BOOL TestDontStickSector(short hitsect);
int QueueStar(short SpriteNum);
int QueueHole(short ang,short hitsect,short hitwall,long hitx,long hity,long hitz);
int QueueFloorBlood(short hitsprite);
int QueueFootPrint(short hitsprite);
int QueueWallBlood(short hitsprite,short ang);
int DoFloorBlood(short SpriteNum);
int DoWallBlood(short SpriteNum);
int SpawnShell(short SpriteNum,short ShellNum);
int DoShrapVelocity(SHORT SpriteNum);
int ShrapKillSprite(short SpriteNum);
//BOOL CheckBreakToughness(BREAK_INFOp break_info,short ID);
int DoItemFly(SHORT SpriteNum);
int InitEnemyCrossbow(short SpriteNum);
VOID UpdateSinglePlayKills(short SpriteNum);
