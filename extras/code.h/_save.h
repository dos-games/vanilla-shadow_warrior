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

#ifdef SAVE_EXTERN
#define SAVE_ENTRY_ROT(group) extern STATEp group[];
#define SAVE_ENTRY_NO_ROT(group) extern STATEp group[];
#define SAVE_ENTRY_PANEL(group) extern PANEL_STATE group[];
#endif

#ifdef SAVE_TABLE
#define SAVE_ENTRY_ROT(group) group,
#define SAVE_ENTRY_NO_ROT(group)
#define SAVE_ENTRY_PANEL(group)
#endif


#ifdef SAVE_TABLE2
#define SAVE_ENTRY_ROT(group)
#define SAVE_ENTRY_NO_ROT(group) group,
#define SAVE_ENTRY_PANEL(group)
#endif

#ifdef SAVE_PANEL_TABLE
#define SAVE_ENTRY_ROT(group)
#define SAVE_ENTRY_NO_ROT(group)
#define SAVE_ENTRY_PANEL(group) { group, sizeof(group) },
#endif

//
// ROTATIONAL ENTRIES
//

SAVE_ENTRY_ROT(sg_NinjaRun)
SAVE_ENTRY_ROT(sg_NinjaStand)
SAVE_ENTRY_ROT(sg_NinjaRise)
SAVE_ENTRY_ROT(sg_NinjaCrawl)
SAVE_ENTRY_ROT(sg_NinjaKneelCrawl)
SAVE_ENTRY_ROT(sg_NinjaDuck)
SAVE_ENTRY_ROT(sg_NinjaSit)
SAVE_ENTRY_ROT(sg_NinjaJump)
SAVE_ENTRY_ROT(sg_NinjaFall)
SAVE_ENTRY_ROT(sg_NinjaSwim)
SAVE_ENTRY_ROT(sg_NinjaDive)
SAVE_ENTRY_ROT(sg_NinjaClimb)
SAVE_ENTRY_ROT(sg_NinjaFly)
SAVE_ENTRY_ROT(sg_NinjaPain)
SAVE_ENTRY_ROT(sg_NinjaStar)
SAVE_ENTRY_ROT(sg_NinjaRocket)
SAVE_ENTRY_ROT(sg_NinjaUzi)
SAVE_ENTRY_ROT(sg_NinjaMirv)
SAVE_ENTRY_ROT(sg_NinjaNapalm)

SAVE_ENTRY_ROT(sg_GoroRun)
SAVE_ENTRY_ROT(sg_GoroChop)
SAVE_ENTRY_ROT(sg_GoroSpell)
SAVE_ENTRY_ROT(sg_GoroStand)
SAVE_ENTRY_ROT(sg_GoroPain)

SAVE_ENTRY_ROT(sg_SkullWait)
SAVE_ENTRY_ROT(sg_SkullRing)
SAVE_ENTRY_ROT(sg_SkullJump)
SAVE_ENTRY_ROT(sg_SkelRun)
SAVE_ENTRY_ROT(sg_SkelSlash)
SAVE_ENTRY_ROT(sg_SkelSpell)
SAVE_ENTRY_ROT(sg_SkelPain)
SAVE_ENTRY_ROT(sg_SkelTeleport)
SAVE_ENTRY_ROT(sg_SkelStand)

SAVE_ENTRY_ROT(sg_CoolieRun)
SAVE_ENTRY_ROT(sg_CoolieStand)
SAVE_ENTRY_ROT(sg_CooliePain)

SAVE_ENTRY_ROT(sg_CoolgRun)
SAVE_ENTRY_ROT(sg_CoolgStand)
SAVE_ENTRY_ROT(sg_CoolgClub)
SAVE_ENTRY_ROT(sg_CoolgAttack)

SAVE_ENTRY_ROT(sg_RipperRun)
SAVE_ENTRY_ROT(sg_RipperStand)
SAVE_ENTRY_ROT(sg_RipperSwipe)
SAVE_ENTRY_ROT(sg_RipperHeart)
SAVE_ENTRY_ROT(sg_RipperHang)
SAVE_ENTRY_ROT(sg_RipperPain)
SAVE_ENTRY_ROT(sg_RipperJump)
SAVE_ENTRY_ROT(sg_RipperFall)
SAVE_ENTRY_ROT(sg_RipperJumpAttack)
SAVE_ENTRY_ROT(sg_RipperHangJump)       
SAVE_ENTRY_ROT(sg_RipperHangFall)

SAVE_ENTRY_ROT(sg_SerpRun)
SAVE_ENTRY_ROT(sg_SerpSlash)
SAVE_ENTRY_ROT(sg_SerpSkullSpell)
SAVE_ENTRY_ROT(sg_SerpSpell)
SAVE_ENTRY_ROT(sg_SerpRapidSpell)
SAVE_ENTRY_ROT(sg_SerpStand)

SAVE_ENTRY_ROT(sg_HornetRun)
SAVE_ENTRY_ROT(sg_HornetStand)

SAVE_ENTRY_ROT(sg_LavaStand)
SAVE_ENTRY_ROT(sg_LavaRun)
SAVE_ENTRY_ROT(sg_LavaThrow)
SAVE_ENTRY_ROT(sg_LavaFlame)

SAVE_ENTRY_ROT(sg_Micro)
SAVE_ENTRY_ROT(sg_Meteor)
SAVE_ENTRY_ROT(sg_MirvMeteor)
SAVE_ENTRY_ROT(sg_SerpMeteor)
SAVE_ENTRY_ROT(sg_BoltThinMan)
SAVE_ENTRY_ROT(sg_Rocket)
SAVE_ENTRY_ROT(sg_BoltSeeker)
SAVE_ENTRY_ROT(sg_Spear)
SAVE_ENTRY_ROT(sg_Grenade)

SAVE_ENTRY_ROT(sg_PlayerNinjaRun)
SAVE_ENTRY_ROT(sg_PlayerNinjaStand)
SAVE_ENTRY_ROT(sg_PlayerNinjaThrow)
SAVE_ENTRY_ROT(sg_PlayerHeadFly)
SAVE_ENTRY_ROT(sg_PlayerHead)
SAVE_ENTRY_ROT(sg_PlayerHeadHurl)
SAVE_ENTRY_ROT(sg_PlayerDeath)
SAVE_ENTRY_ROT(sg_NinjaHariKari)

//
// NON-ROTATIONAL ENTRIES
//

SAVE_ENTRY_NO_ROT(sg_NinjaHariKari)
SAVE_ENTRY_NO_ROT(sg_NinjaDie)
SAVE_ENTRY_NO_ROT(sg_NinjaDead)
SAVE_ENTRY_NO_ROT(sg_NinjaDeathJump)
SAVE_ENTRY_NO_ROT(sg_NinjaDeathFall)

SAVE_ENTRY_NO_ROT(sg_GoroDie)
SAVE_ENTRY_NO_ROT(sg_GoroDead)

SAVE_ENTRY_NO_ROT(sg_SkelDie)

SAVE_ENTRY_NO_ROT(sg_CoolieDie)
SAVE_ENTRY_NO_ROT(sg_CoolieDead)

SAVE_ENTRY_NO_ROT(sg_CoolgDie)
SAVE_ENTRY_NO_ROT(sg_CoolgDead)
SAVE_ENTRY_NO_ROT(sg_CoolgBirth)

SAVE_ENTRY_NO_ROT(sg_RipperDie)
SAVE_ENTRY_NO_ROT(sg_RipperDead)
SAVE_ENTRY_NO_ROT(sg_RipperDeathJump)
SAVE_ENTRY_NO_ROT(sg_RipperDeathFall)

SAVE_ENTRY_NO_ROT(sg_SerpDie)
SAVE_ENTRY_NO_ROT(sg_SerpDead)

SAVE_ENTRY_NO_ROT(sg_HornetDie)
SAVE_ENTRY_NO_ROT(sg_HornetDead)

SAVE_ENTRY_NO_ROT(sg_LavaDie)
SAVE_ENTRY_NO_ROT(sg_LavaDead)

SAVE_ENTRY_NO_ROT(sg_SkullExplode)

//
// PANEL SPRITE STATE ENTRIES
//

SAVE_ENTRY_PANEL(ps_PanelEnvironSuit)
SAVE_ENTRY_PANEL(ps_PanelCloak)
SAVE_ENTRY_PANEL(ps_PanelMedkit)
SAVE_ENTRY_PANEL(ps_PanelRepairKit)
SAVE_ENTRY_PANEL(ps_PanelSelectionBox)
SAVE_ENTRY_PANEL(ps_PanelNightVision)
SAVE_ENTRY_PANEL(ps_PanelChemBomb)
SAVE_ENTRY_PANEL(ps_PanelFlashBomb)
SAVE_ENTRY_PANEL(ps_PanelCaltrops)
SAVE_ENTRY_PANEL(ps_PresentSword)
SAVE_ENTRY_PANEL(ps_SwordRest)
SAVE_ENTRY_PANEL(ps_SwordHide)
SAVE_ENTRY_PANEL(ps_SwordSwing)
SAVE_ENTRY_PANEL(ps_RetractSword)
SAVE_ENTRY_PANEL(ps_PresentStar)
SAVE_ENTRY_PANEL(ps_StarHide)
SAVE_ENTRY_PANEL(ps_ThrowStar)
SAVE_ENTRY_PANEL(ps_RetractStar)
SAVE_ENTRY_PANEL(ps_FireUzi)
SAVE_ENTRY_PANEL(ps_UziNull)
SAVE_ENTRY_PANEL(ps_UziHide)
SAVE_ENTRY_PANEL(ps_PresentUzi)
SAVE_ENTRY_PANEL(ps_PresentUziReload)
SAVE_ENTRY_PANEL(ps_RetractUzi)
SAVE_ENTRY_PANEL(ps_FireUzi2)
SAVE_ENTRY_PANEL(ps_PresentUzi2)
SAVE_ENTRY_PANEL(ps_Uzi2Hide)
SAVE_ENTRY_PANEL(ps_RetractUzi2)
SAVE_ENTRY_PANEL(ps_Uzi2Suicide)
SAVE_ENTRY_PANEL(ps_Uzi2Null)
SAVE_ENTRY_PANEL(ps_UziEject)
SAVE_ENTRY_PANEL(ps_UziClip)
SAVE_ENTRY_PANEL(ps_UziReload)
SAVE_ENTRY_PANEL(ps_UziDoneReload)
SAVE_ENTRY_PANEL(ps_UziShell)
SAVE_ENTRY_PANEL(ps_Uzi2Shell)
#if 0
SAVE_ENTRY_PANEL(ps_PresentRocket)
SAVE_ENTRY_PANEL(ps_RocketRest)
SAVE_ENTRY_PANEL(ps_RocketHide)
SAVE_ENTRY_PANEL(ps_RocketFire)
SAVE_ENTRY_PANEL(ps_RocketReload)
SAVE_ENTRY_PANEL(ps_RetractRocket)
#endif
SAVE_ENTRY_PANEL(ps_ShotgunFire)

SAVE_ENTRY_PANEL(ps_PresentHothead)
SAVE_ENTRY_PANEL(ps_HotheadRest)
SAVE_ENTRY_PANEL(ps_HotheadHide)
SAVE_ENTRY_PANEL(ps_HotheadAttack)

SAVE_ENTRY_PANEL(ps_RetractHothead)
SAVE_ENTRY_PANEL(ps_PresentMicro)
SAVE_ENTRY_PANEL(ps_MicroRest)
SAVE_ENTRY_PANEL(ps_MicroHide)
SAVE_ENTRY_PANEL(ps_MicroFire)
//SAVE_ENTRY_PANEL(ps_MicroReload)
SAVE_ENTRY_PANEL(ps_RetractMicro)
#if 0
SAVE_ENTRY_PANEL(ps_PresentElectro)
SAVE_ENTRY_PANEL(ps_ElectroRest)
SAVE_ENTRY_PANEL(ps_ElectroHide)
SAVE_ENTRY_PANEL(ps_ElectroFire)
SAVE_ENTRY_PANEL(ps_ElectroRecoil)
SAVE_ENTRY_PANEL(ps_RetractElectro)
SAVE_ENTRY_PANEL(ps_PresentNapalm)
SAVE_ENTRY_PANEL(ps_NapalmRest)
SAVE_ENTRY_PANEL(ps_NapalmHide)
SAVE_ENTRY_PANEL(ps_NapalmFire)
SAVE_ENTRY_PANEL(ps_NapalmRecoil)
SAVE_ENTRY_PANEL(ps_RetractNapalm)
SAVE_ENTRY_PANEL(ps_PresentRing)
SAVE_ENTRY_PANEL(ps_RingRest)
SAVE_ENTRY_PANEL(ps_RingHide)
SAVE_ENTRY_PANEL(ps_RingFire)
SAVE_ENTRY_PANEL(ps_RingRecoil)
SAVE_ENTRY_PANEL(ps_RetractRing)
#endif
SAVE_ENTRY_PANEL(ps_PresentHeart)
SAVE_ENTRY_PANEL(ps_HeartRest)
SAVE_ENTRY_PANEL(ps_HeartHide)
SAVE_ENTRY_PANEL(ps_HeartAttack)
SAVE_ENTRY_PANEL(ps_HeartBlood)
SAVE_ENTRY_PANEL(ps_RetractHeart)
SAVE_ENTRY_PANEL(ps_PresentGrenade)
SAVE_ENTRY_PANEL(ps_GrenadeRest)
SAVE_ENTRY_PANEL(ps_GrenadeHide)
SAVE_ENTRY_PANEL(ps_GrenadeFire)
SAVE_ENTRY_PANEL(ps_GrenadeRecoil)
SAVE_ENTRY_PANEL(ps_RetractGrenade)
SAVE_ENTRY_PANEL(ps_PresentMine)
SAVE_ENTRY_PANEL(ps_MineRest)
SAVE_ENTRY_PANEL(ps_MineHide)
SAVE_ENTRY_PANEL(ps_MineThrow)
SAVE_ENTRY_PANEL(ps_RetractMine)
SAVE_ENTRY_PANEL(ps_OnFire)

#undef SAVE_ENTRY_ROT
#undef SAVE_ENTRY_NO_ROT
#undef SAVE_ENTRY_PANEL
