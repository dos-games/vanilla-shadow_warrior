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

#define MAIN
#define QUIET
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "lists.h"

#include "def.h"
#include "save.h"
#include "savedef.h"
#include "mfile.h"
#include "symutil.h"

#if 0
//
// TO DO
//

    Look through all structures for new pointers to save off.
    Debug Debug Debug!
#endif

extern char SaveGameDescr[10][80];

VOID ScreenTileLock(void);
VOID ScreenTileUnLock(void);

int ScreenSaveSetup(PLAYERp pp);
VOID ScreenSave(FILE *fout);

int ScreenLoadSetup(void);
VOID ScreenLoad(FILE *fin);

#define PANEL_SAVE 1
#define ANIM_SAVE 1
    
extern SW_PACKET loc;
extern char LevelName[20];
extern STATE s_NotRestored[];

int 
PanelSpriteToNdx(PLAYERp pp, PANEL_SPRITEp psprite)
    {
    short ndx = 0;
    PANEL_SPRITEp psp, next;

    TRAVERSE(&pp->PanelSpriteList, psp, next)
        {
        if (psp == psprite)
            return(ndx);
            
        ndx++;
        }

    // special case for pointing to the list head
    if ((LIST)psprite == (LIST)&pp->PanelSpriteList)
        return(9999);

    return(-1);    
    }    

    
PANEL_SPRITEp
PanelNdxToSprite(PLAYERp pp, long ndx)
    {
    short count = 0;
    PANEL_SPRITEp psp, next;

    if (ndx == -1)
        return(NULL);

    if (ndx == 9999)
        return((PANEL_SPRITEp)&pp->PanelSpriteList);

    TRAVERSE(&pp->PanelSpriteList, psp, next)
        {
        if (count == ndx)
            return(psp);
            
        count++;
        }

    return(NULL);    
    }    

void SaveSymDataInfo(MFILE fil, void *ptr)    
    {
    unsigned long unrelocated_offset, offset_from_symbol;
    SYM_TABLEp st_ptr;
    char sym_name[80];
    
    if (!ptr)
        {
        offset_from_symbol = -1;
        strcpy(sym_name, "NULL");
        }
    else
        {    
        unrelocated_offset = SymDataPtrToOffset(ptr);
        st_ptr = SearchSymTableByOffset(SymTableData, SymCountData, unrelocated_offset, &offset_from_symbol);
        ASSERT(st_ptr);
        strcpy(sym_name, st_ptr->Name);
        }
    
    MWRITE(sym_name, sizeof(st_ptr->Name), 1, fil);
    MWRITE(&offset_from_symbol, sizeof(offset_from_symbol), 1, fil);
    }

void SaveSymCodeInfo(MFILE fil, void *ptr)    
    {
    unsigned long unrelocated_offset, offset_from_symbol;
    SYM_TABLEp st_ptr;
    char sym_name[80];
    
    if (!ptr)
        {
        offset_from_symbol = -1;
        strcpy(sym_name, "NULL");
        }
    else
        {    
        unrelocated_offset = SymCodePtrToOffset(ptr);
        st_ptr = SearchSymTableByOffset(SymTableCode, SymCountCode, unrelocated_offset, &offset_from_symbol);
        ASSERT(st_ptr);
        strcpy(sym_name, st_ptr->Name);
        }
    
    MWRITE(sym_name, sizeof(st_ptr->Name), 1, fil);
    MWRITE(&offset_from_symbol, sizeof(offset_from_symbol), 1, fil);
    }

void *LoadSymDataInfo(MFILE fil)    
    {
    unsigned long offset_from_symbol;
    char sym_name[80];
    SYM_TABLEp st_ptr;
    void *data_ptr;

    MREAD(&sym_name, sizeof(st_ptr->Name), 1, fil);
    MREAD(&offset_from_symbol, sizeof(offset_from_symbol), 1, fil);
    
    if (offset_from_symbol == -1)
        return(NULL);
    
    st_ptr = SearchSymTableByName(SymTableData, SymCountData, sym_name);
    ASSERT(st_ptr);
    data_ptr = SymOffsetToDataPtr(st_ptr->Offset + offset_from_symbol);
    
    return(data_ptr);
    }

void *LoadSymCodeInfo(MFILE fil)    
    {
    unsigned long offset_from_symbol;
    char sym_name[80];
    SYM_TABLEp st_ptr;
    void *code_ptr;

    MREAD(&sym_name, sizeof(st_ptr->Name), 1, fil);
    MREAD(&offset_from_symbol, sizeof(offset_from_symbol), 1, fil);
    
    if (offset_from_symbol == -1)
        return(NULL);
        
    st_ptr = SearchSymTableByName(SymTableCode, SymCountCode, sym_name);
    ASSERT(st_ptr);
    code_ptr = SymOffsetToCodePtr(st_ptr->Offset + offset_from_symbol);
    
    return(code_ptr);
    }
    
int SaveGame(short save_num)
{
    MFILE fil;
    long i,j;
    short ndx;
    SPRITE tsp;
    SPRITEp sp;
    PLAYER tp;
    PLAYERp pp;
    SECT_USERp sectu;
    USER tu;
    USERp u;
    ANIM tanim;
    ANIMp a;
    CHAR code;
    BYTE data_code;
    SHORT data_ndx;
    PANEL_SPRITE tpanel_sprite;
    PANEL_SPRITEp psp,cur,next;
    SECTOR_OBJECTp sop;
    char game_name[80];
    long cnt = 0;

    LoadSymTable("swdata.sym", SymTableData, &SymCountData);
    LoadSymTable("swcode.sym", SymTableCode, &SymCountCode);

    sprintf(game_name,"game%d.sav",save_num);
    if ((fil = MOPEN_WRITE(game_name)) == MF_ERR)
        return(-1);

    MWRITE(SaveGameDescr[save_num],sizeof(SaveGameDescr[save_num]),1,fil);
    
    MWRITE(&Level,sizeof(Level),1,fil);
    MWRITE(&Episode,sizeof(Episode),1,fil);
    MWRITE(&Skill,sizeof(Skill),1,fil);
    
    ScreenSaveSetup(&Player[myconnectindex]);

    ScreenSave(fil);

    ScreenTileUnLock();
    
    MWRITE(&numplayers,sizeof(numplayers),1,fil);
    MWRITE(&myconnectindex,sizeof(myconnectindex),1,fil);
    MWRITE(&connecthead,sizeof(connecthead),1,fil);
    MWRITE(connectpoint2,sizeof(connectpoint2),1,fil);
    
    //save players info
    pp = &tp;
    for (i = 0; i < numplayers; i++)
        {
        memcpy(&tp, &Player[i], sizeof(PLAYER));
        
        // this does not point to global data - this is allocated link list based
        // save this inside the structure
        #if PANEL_SAVE
        pp->CurWpn = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->CurWpn);
        for (ndx = 0; ndx < MAX_WEAPONS; ndx++)
            pp->Wpn[ndx] = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->Wpn[ndx]);
        pp->Chops = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->Chops);
        for (ndx = 0; ndx < MAX_INVENTORY; ndx++)
            pp->InventorySprite[ndx] = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->InventorySprite[ndx]);
        pp->InventorySelectionBox = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->InventorySelectionBox);
        pp->MiniBarHealthBox = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->MiniBarHealthBox);
        pp->MiniBarAmmo = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->MiniBarAmmo);
        for (ndx = 0; ndx < SIZ(pp->MiniBarHealthBoxDigit); ndx++)
            pp->MiniBarHealthBoxDigit[ndx] = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->MiniBarHealthBoxDigit[ndx]);
        for (ndx = 0; ndx < SIZ(pp->MiniBarAmmoDigit); ndx++)
            pp->MiniBarAmmoDigit[ndx] = (PANEL_SPRITEp)PanelSpriteToNdx(&Player[i], pp->MiniBarAmmoDigit[ndx]);
        #endif    
        
        MWRITE(&tp, sizeof(PLAYER),1,fil);
        
        //////
     
        SaveSymDataInfo(fil, pp->remote_sprite);
        SaveSymDataInfo(fil, pp->remote.sop_control);
        SaveSymDataInfo(fil, pp->sop_remote);
        SaveSymDataInfo(fil, pp->sop);
        SaveSymDataInfo(fil, pp->hi_sectp);
        SaveSymDataInfo(fil, pp->lo_sectp);
        SaveSymDataInfo(fil, pp->hi_sp);
        SaveSymDataInfo(fil, pp->lo_sp);
        
        SaveSymDataInfo(fil, pp->last_camera_sp);
        SaveSymDataInfo(fil, pp->SpriteP);
        SaveSymDataInfo(fil, pp->UnderSpriteP);
        
        SaveSymCodeInfo(fil, pp->DoPlayerAction);
        
        SaveSymDataInfo(fil, pp->sop_control);
        SaveSymDataInfo(fil, pp->sop_riding);
        }

    #if PANEL_SAVE
    // local copy
    psp = &tpanel_sprite;
    for (i = 0; i < numplayers; i++)
        {
        int j;
        pp = &Player[i];
        ndx = 0;
        
        TRAVERSE(&pp->PanelSpriteList, cur, next)
            {
            // this is a HEADER
            MWRITE(&ndx, sizeof(ndx),1,fil);
            
            memcpy(psp, cur, sizeof(PANEL_SPRITE));
            
            // Panel Sprite - save in structure
            psp->sibling = (PANEL_SPRITEp)PanelSpriteToNdx(pp, cur->sibling);
            MWRITE(psp, sizeof(PANEL_SPRITE),1,fil);
          
            SaveSymDataInfo(fil, psp->PlayerP);
            SaveSymDataInfo(fil, psp->State);
            SaveSymDataInfo(fil, psp->RetractState);
            SaveSymDataInfo(fil, psp->PresentState);
            SaveSymDataInfo(fil, psp->ActionState);
            SaveSymDataInfo(fil, psp->RestState);
            SaveSymCodeInfo(fil, psp->PanelSpriteFunc);
            
            for (j = 0; j < SIZ(psp->over); j++)
                {
                SaveSymDataInfo(fil, psp->over[j].State);
                }
                      
            ndx++;
            }
            
        // store -1 when done for player
        ndx = -1;    
        MWRITE(&ndx, sizeof(ndx),1,fil);
        }
    #endif    
        
    MWRITE(&numsectors,sizeof(numsectors),1,fil);
    MWRITE(sector,sizeof(SECTOR), numsectors, fil);

    //Sector User information
    for (i = 0; i < numsectors; i++)
        {
        sectu = SectUser[i];
        ndx = i;
        if (sectu)         
            {
            // write header
            MWRITE(&ndx,sizeof(ndx),1,fil);
            
            MWRITE(sectu,sizeof(SECT_USER),1,fil);
            }
        else 
            {
            // write trailer
            ndx = -1;
            MWRITE(&ndx,sizeof(ndx),1,fil);
            }
        }

    MWRITE(&numwalls,sizeof(numwalls),1,fil);
    MWRITE(wall,sizeof(WALL),numwalls,fil);

    for (i = 0; i < MAXSPRITES; i++)
        {
        if (sprite[i].statnum != MAXSTATUS)         
            {
            MWRITE(&i,sizeof(i),1,fil);
            
            MWRITE(&sprite[i],sizeof(SPRITE),1,fil);
            }    
        }
    i = -1;    
    MWRITE(&i,sizeof(i),1,fil);
    
    MWRITE(headspritesect,sizeof(headspritesect),1,fil);
    MWRITE(prevspritesect,sizeof(prevspritesect),1,fil);
    MWRITE(nextspritesect,sizeof(nextspritesect),1,fil);
    MWRITE(headspritestat,sizeof(headspritestat),1,fil);
    MWRITE(prevspritestat,sizeof(prevspritestat),1,fil);
    MWRITE(nextspritestat,sizeof(nextspritestat),1,fil);
    
    //User information
    for (i = 0; i < MAXSPRITES; i++)
        {
        ndx = i;
        if (User[i])         
            {
            // write header
            MWRITE(&ndx,sizeof(ndx),1,fil);
            
            sp = &sprite[i];
            memcpy(&tu, User[i], sizeof(USER));
            u = &tu;
            
            MWRITE(u,sizeof(USER),1,fil);
            
            if (u->WallShade)
                {
                MWRITE(u->WallShade,sizeof(*u->WallShade)*u->WallCount,1,fil);
                }    
            
            if (u->rotator)
                {
                MWRITE(u->rotator,sizeof(*u->rotator),1,fil);
                if (u->rotator->origx)
                    MWRITE(u->rotator->origx,sizeof(*u->rotator->origx)*u->rotator->num_walls,1,fil);
                if (u->rotator->origy)
                    MWRITE(u->rotator->origy,sizeof(*u->rotator->origy)*u->rotator->num_walls,1,fil);
                }    
            
            SaveSymDataInfo(fil, u->WallP);
            SaveSymDataInfo(fil, u->State);
            SaveSymDataInfo(fil, u->Rot);
            SaveSymDataInfo(fil, u->StateStart);
            SaveSymDataInfo(fil, u->StateEnd);
            SaveSymDataInfo(fil, u->StateFallOverride);
            SaveSymCodeInfo(fil, u->ActorActionFunc);
            SaveSymDataInfo(fil, u->ActorActionSet);
            SaveSymDataInfo(fil, u->Personality);
            SaveSymDataInfo(fil, u->Attrib);
            SaveSymDataInfo(fil, u->sop_parent);
            SaveSymDataInfo(fil, u->hi_sectp);
            SaveSymDataInfo(fil, u->lo_sectp);
            SaveSymDataInfo(fil, u->hi_sp);
            SaveSymDataInfo(fil, u->lo_sp);
            SaveSymDataInfo(fil, u->SpriteP);
            SaveSymDataInfo(fil, u->PlayerP);
            SaveSymDataInfo(fil, u->tgt_sp);
            }
        }
    ndx = -1;
    MWRITE(&ndx,sizeof(ndx),1,fil);
    
    //
    // Sector object
    //    

    MWRITE(SectorObject, sizeof(SectorObject),1,fil);
    
    for (ndx = 0; ndx < SIZ(SectorObject); ndx++)
        {
        sop = &SectorObject[ndx];
        
        SaveSymCodeInfo(fil, sop->PreMoveAnimator);
        SaveSymCodeInfo(fil, sop->PostMoveAnimator);
        SaveSymCodeInfo(fil, sop->Animator);
        SaveSymDataInfo(fil, sop->controller);
        SaveSymDataInfo(fil, sop->sp_child);
        }
    
    
    MWRITE(SineWaveFloor, sizeof(SineWaveFloor),1,fil);
    MWRITE(SineWall, sizeof(SineWall),1,fil);
    MWRITE(SpringBoard, sizeof(SpringBoard),1,fil);
    MWRITE(Rotate, sizeof(Rotate),1,fil);
    MWRITE(DoorAutoClose, sizeof(DoorAutoClose),1,fil);
    MWRITE(&x_min_bound, sizeof(x_min_bound),1,fil);
    MWRITE(&y_min_bound, sizeof(y_min_bound),1,fil);
    MWRITE(&x_max_bound, sizeof(x_max_bound),1,fil);
    MWRITE(&y_max_bound, sizeof(y_max_bound),1,fil);


    MWRITE(Track, sizeof(Track),1,fil);
    for (i = 0; i < MAX_TRACKS; i++)
        {
        ASSERT(Track[i].TrackPoint);
        if (Track[i].NumPoints == 0)
            MWRITE(Track[i].TrackPoint, sizeof(TRACK_POINT),1,fil);
        else
            MWRITE(Track[i].TrackPoint, Track[i].NumPoints * sizeof(TRACK_POINT),1,fil);
        }
    
    MWRITE(&vel,sizeof(vel),1,fil);
    MWRITE(&svel,sizeof(svel),1,fil);
    MWRITE(&angvel,sizeof(angvel),1,fil);

    MWRITE(&loc,sizeof(loc),1,fil);
    //MWRITE(&oloc,sizeof(oloc),1,fil);
    //MWRITE(&fsync,sizeof(fsync),1,fil);

    MWRITE(LevelName,sizeof(LevelName),1,fil);
    MWRITE(&screenpeek,sizeof(screenpeek),1,fil);
    MWRITE(&totalsynctics,sizeof(totalsynctics),1,fil);

    // do all sector manipulation structures

    #if ANIM_SAVE
    ndx = 0;
    for(i = AnimCnt - 1, a = &tanim; i >= 0; i--)
        {
        // write header
        MWRITE(&ndx,sizeof(ndx),1,fil);
        
        memcpy(a,&Anim[i],sizeof(ANIM));
        MWRITE(a,sizeof(ANIM),1,fil);
        
        SaveSymDataInfo(fil, a->ptr);
        SaveSymCodeInfo(fil, a->callback);
        SaveSymDataInfo(fil, a->callbackdata);
        
        ndx++;
        }
        
    // write trailer
    ndx = -1;    
    MWRITE(&ndx,sizeof(ndx),1,fil);
    #endif

    MWRITE(&totalclock,sizeof(totalclock),1,fil);
    MWRITE(&numframes,sizeof(numframes),1,fil);
    MWRITE(&randomseed,sizeof(randomseed),1,fil);
    MWRITE(&numpalookups,sizeof(numpalookups),1,fil);

    MWRITE(&visibility,sizeof(visibility),1,fil);
    MWRITE(&parallaxtype,sizeof(parallaxtype),1,fil);
    MWRITE(&parallaxyoffs,sizeof(parallaxyoffs),1,fil);
    MWRITE(pskyoff,sizeof(pskyoff),1,fil);
    MWRITE(&pskybits,sizeof(pskybits),1,fil);
    
    MWRITE(&BorderInfo,sizeof(BorderInfo),1,fil);
    MWRITE(&MoveSkip2,sizeof(MoveSkip2),1,fil);
    MWRITE(&MoveSkip4,sizeof(MoveSkip4),1,fil);

    MCLOSE(fil);
    
    //DSPRINTF(ds, "done saving");
    MONO_PRINT(ds);
    
    return(0);
}

int LoadGameFullHeader(short save_num, char *descr, short *episode, short *level, short *skill)
{
    MFILE fil;
    char game_name[80];
    short tile;

    sprintf(game_name,"game%d.sav",save_num);
    if ((fil = MOPEN_READ(game_name)) == MF_ERR)
        return(-1);
    
    MREAD(descr, sizeof(SaveGameDescr[0]), 1,fil);
    
    MREAD(level,sizeof(*level),1,fil);
    MREAD(episode,sizeof(*episode),1,fil);
    MREAD(skill,sizeof(*skill),1,fil);
    
    tile = ScreenLoadSetup();
    ScreenLoad(fil);
    
    fclose(fil);
    
    return(tile);
}    

void LoadGameDescr(short save_num, char *descr)
{
    MFILE fil;
    char game_name[80];
    short tile;
    
    sprintf(game_name,"game%d.sav",save_num);
    if ((fil = MOPEN_READ(game_name)) == MF_ERR)
        return;
    
    MREAD(descr, sizeof(SaveGameDescr[0]),1,fil);
    
    fclose(fil);
}    


int LoadGame(short save_num)
{
    MFILE fil;
    long i,j;
    short ndx,SpriteNum,sectnum;
    PLAYERp pp;
    SPRITEp sp;
    USERp u;
    SECTOR_OBJECTp sop;
    SECT_USERp sectu;
    CHAR code;
    ANIMp a;
    BYTE data_code;
    SHORT data_ndx;
    PANEL_SPRITEp psp,next,cur;
    PANEL_SPRITE tpanel_sprite;
    char game_name[80];

    long RotNdx;
    long StateStartNdx;
    long StateNdx;
    long StateEndNdx;

    TerminateLevel();

    LoadSymTable("swdata.sym", SymTableData, &SymCountData);
    LoadSymTable("swcode.sym", SymTableCode, &SymCountCode);

    sprintf(game_name,"game%d.sav",save_num);
    if ((fil = MOPEN_READ(game_name)) == MF_ERR)
        return(-1);
    
    MREAD(SaveGameDescr[save_num], sizeof(SaveGameDescr[save_num]),1,fil);
    
    MREAD(&Level,sizeof(Level),1,fil);
    MREAD(&Episode,sizeof(Episode),1,fil);
    MREAD(&Skill,sizeof(Skill),1,fil);
    
    ScreenLoadSetup();
    ScreenLoad(fil);
    ScreenTileUnLock();

    MREAD(&numplayers, sizeof(numplayers),1,fil);
    MREAD(&myconnectindex,sizeof(myconnectindex),1,fil);
    MREAD(&connecthead,sizeof(connecthead),1,fil);
    MREAD(connectpoint2,sizeof(connectpoint2),1,fil);
    
    //save players
    //MREAD(Player,sizeof(PLAYER), numplayers,fil);
    
    //save players info
    for (i = 0; i < numplayers; i++)
        {
        pp = &Player[i];
        
        MREAD(pp, sizeof(*pp), 1, fil);

        pp->remote_sprite = LoadSymDataInfo(fil);
        pp->remote.sop_control = LoadSymDataInfo(fil);
        pp->sop_remote = LoadSymDataInfo(fil);
        pp->sop = LoadSymDataInfo(fil);
        
        pp->hi_sectp = LoadSymDataInfo(fil);
        pp->lo_sectp = LoadSymDataInfo(fil);
        
        pp->hi_sp = LoadSymDataInfo(fil);
        pp->lo_sp = LoadSymDataInfo(fil);
        
        pp->last_camera_sp = LoadSymDataInfo(fil);
        pp->SpriteP = LoadSymDataInfo(fil);
        pp->UnderSpriteP = LoadSymDataInfo(fil);
        pp->DoPlayerAction = LoadSymCodeInfo(fil);
        pp->sop_control = LoadSymDataInfo(fil);
        pp->sop_riding = LoadSymDataInfo(fil);
        }
    
    
    #if PANEL_SAVE
    for (i = 0; i < numplayers; i++)
        {
        int j;
        pp = &Player[i];
        
        INITLIST(&pp->PanelSpriteList);

        while (TRUE)
            {
            MREAD(&ndx, sizeof(ndx),1,fil);
        
            if (ndx == -1)
                break;
            
            psp = CallocMem(sizeof(PANEL_SPRITE), 1);
            ASSERT(psp);

            MREAD(psp, sizeof(PANEL_SPRITE),1,fil);
            INSERT_TAIL(&pp->PanelSpriteList,psp);
            
            psp->PlayerP = LoadSymDataInfo(fil);
            psp->State = LoadSymDataInfo(fil);
            psp->RetractState = LoadSymDataInfo(fil);
            psp->PresentState = LoadSymDataInfo(fil);
            psp->ActionState = LoadSymDataInfo(fil);
            psp->RestState = LoadSymDataInfo(fil);
            psp->PanelSpriteFunc = LoadSymCodeInfo(fil);

            for (j = 0; j < SIZ(psp->over); j++)
                {
                psp->over[j].State = LoadSymDataInfo(fil);
                }

            }
        }
    #endif    

    MREAD(&numsectors,sizeof(numsectors),1,fil);
    MREAD(sector,sizeof(SECTOR),numsectors,fil);

    //Sector User information
    for (i = 0; i < numsectors; i++)
        {
        MREAD(&sectnum,sizeof(sectnum),1,fil);
        if (sectnum != -1)         
            {
            SectUser[sectnum] = sectu = (SECT_USERp)CallocMem(sizeof(SECT_USER), 1);
            MREAD(sectu,sizeof(SECT_USER),1,fil);
            }
        }
        
    MREAD(&numwalls,sizeof(numwalls),1,fil);
    MREAD(wall,sizeof(WALL),numwalls,fil);

    //Store all sprites to preserve indeces
    MREAD(&i, sizeof(i),1,fil);
    while(i != -1)
        {
        MREAD(&sprite[i], sizeof(SPRITE),1,fil);
        MREAD(&i, sizeof(i),1,fil);
        }
        
    MREAD(headspritesect,sizeof(headspritesect),1,fil);
    MREAD(prevspritesect,sizeof(prevspritesect),1,fil);
    MREAD(nextspritesect,sizeof(nextspritesect),1,fil);
    MREAD(headspritestat,sizeof(headspritestat),1,fil);
    MREAD(prevspritestat,sizeof(prevspritestat),1,fil);
    MREAD(nextspritestat,sizeof(nextspritestat),1,fil);

    //User information
    memset(User, NULL, sizeof(User));
    
    MREAD(&SpriteNum, sizeof(SpriteNum),1,fil);
    while(SpriteNum != -1)
        {
        sp = &sprite[SpriteNum];
        User[SpriteNum] = u = (USERp)CallocMem(sizeof(USER), 1);
        MREAD(u,sizeof(USER),1,fil);

        if (u->WallShade)
            {
            u->WallShade = CallocMem(u->WallCount * sizeof(*u->WallShade), 1);
            MREAD(u->WallShade,sizeof(*u->WallShade)*u->WallCount,1,fil);
            }    
        
        if (u->rotator)
            {
            u->WallShade = CallocMem(sizeof(*u->rotator), 1);
            MREAD(u->rotator,sizeof(*u->rotator),1,fil);
            
            if (u->rotator->origx)
                {
                u->rotator->origx = CallocMem(u->rotator->num_walls * sizeof(*u->rotator->origx), 1);
                MREAD(u->rotator->origx,sizeof(*u->rotator->origx)*u->rotator->num_walls,1,fil);
                }
            if (u->rotator->origy)
                {
                u->rotator->origy = CallocMem(u->rotator->num_walls * sizeof(*u->rotator->origy), 1);
                MREAD(u->rotator->origy,sizeof(*u->rotator->origy)*u->rotator->num_walls,1,fil);
                }
            }    
        
        u->WallP = LoadSymDataInfo(fil);
        u->State = LoadSymDataInfo(fil);
        u->Rot = LoadSymDataInfo(fil);
        u->StateStart = LoadSymDataInfo(fil);
        u->StateEnd = LoadSymDataInfo(fil);
        u->StateFallOverride = LoadSymDataInfo(fil);
        u->ActorActionFunc = LoadSymCodeInfo(fil);
        u->ActorActionSet = LoadSymDataInfo(fil);
        u->Personality = LoadSymDataInfo(fil);
        u->Attrib = LoadSymDataInfo(fil);
        u->sop_parent = LoadSymDataInfo(fil);
        u->hi_sectp = LoadSymDataInfo(fil);
        u->lo_sectp = LoadSymDataInfo(fil);
        u->hi_sp = LoadSymDataInfo(fil);
        u->lo_sp = LoadSymDataInfo(fil);
        u->SpriteP = LoadSymDataInfo(fil);
        u->PlayerP = LoadSymDataInfo(fil);
        u->tgt_sp = LoadSymDataInfo(fil);
        
        MREAD(&SpriteNum,sizeof(SpriteNum),1,fil);
        }
        
    MREAD(SectorObject, sizeof(SectorObject),1,fil);
    
    for (ndx = 0; ndx < SIZ(SectorObject); ndx++)
        {
        sop = &SectorObject[ndx];
        
        sop->PreMoveAnimator = LoadSymCodeInfo(fil);
        sop->PostMoveAnimator = LoadSymCodeInfo(fil);
        sop->Animator = LoadSymCodeInfo(fil);
        sop->controller = LoadSymDataInfo(fil);
        sop->sp_child = LoadSymDataInfo(fil);
        }
    
    MREAD(SineWaveFloor, sizeof(SineWaveFloor),1,fil);
    MREAD(SineWall, sizeof(SineWall),1,fil);
    MREAD(SpringBoard, sizeof(SpringBoard),1,fil);
    MREAD(Rotate, sizeof(Rotate),1,fil);
    MREAD(DoorAutoClose, sizeof(DoorAutoClose),1,fil);
    MREAD(&x_min_bound, sizeof(x_min_bound),1,fil);
    MREAD(&y_min_bound, sizeof(y_min_bound),1,fil);
    MREAD(&x_max_bound, sizeof(x_max_bound),1,fil);
    MREAD(&y_max_bound, sizeof(y_max_bound),1,fil);
    
    MREAD(Track, sizeof(Track),1,fil);
    for (i = 0; i < MAX_TRACKS; i++)
        {
        if (Track[i].NumPoints == 0)
            {
            Track[i].TrackPoint = (TRACK_POINTp)CallocMem(sizeof(TRACK_POINT), 1);
            MREAD(Track[i].TrackPoint, sizeof(TRACK_POINT),1,fil);
            }
        else    
            {
            Track[i].TrackPoint = (TRACK_POINTp)CallocMem(Track[i].NumPoints * sizeof(TRACK_POINT), 1);
            MREAD(Track[i].TrackPoint, Track[i].NumPoints * sizeof(TRACK_POINT),1,fil);
            }
        }
        
    MREAD(&vel,sizeof(vel),1,fil);
    MREAD(&svel,sizeof(svel),1,fil);
    MREAD(&angvel,sizeof(angvel),1,fil);

    MREAD(&loc,sizeof(loc),1,fil);

    MREAD(LevelName,sizeof(LevelName),1,fil);
    MREAD(&screenpeek,sizeof(screenpeek),1,fil);
    MREAD(&totalsynctics,sizeof(totalsynctics),1,fil);  // same as kens lockclock

    // do all sector manipulation structures

    #if ANIM_SAVE
    for(i = MAXANIM - 1; i >= 0; i--)
        {
        a = &Anim[i];
        
        AnimCnt++;
        MREAD(&ndx,sizeof(ndx),1,fil);
        
        if (ndx == -1)
            break;
        
        MREAD(a,sizeof(ANIM),1,fil);
        
        a->ptr = LoadSymDataInfo(fil);
        a->callback = LoadSymCodeInfo(fil);
        a->callbackdata = LoadSymDataInfo(fil);
        }
    #endif    

    MREAD(&totalclock,sizeof(totalclock),1,fil);
    MREAD(&numframes,sizeof(numframes),1,fil);
    MREAD(&randomseed,sizeof(randomseed),1,fil);
    MREAD(&numpalookups,sizeof(numpalookups),1,fil);

    MREAD(&visibility,sizeof(visibility),1,fil);
    MREAD(&parallaxtype,sizeof(parallaxtype),1,fil);
    MREAD(&parallaxyoffs,sizeof(parallaxyoffs),1,fil);
    MREAD(pskyoff,sizeof(pskyoff),1,fil);
    MREAD(&pskybits,sizeof(pskybits),1,fil);

    MREAD(&BorderInfo,sizeof(BorderInfo),1,fil);
    MREAD(&MoveSkip2,sizeof(MoveSkip2),1,fil);
    MREAD(&MoveSkip4,sizeof(MoveSkip4),1,fil);
    
    MCLOSE(fil);
    
    //!!IMPORTANT - this POST stuff will not work here now becaus it does actual reads
    
    //
    // POST processing of info MREAD in
    //
    
    #if PANEL_SAVE
    for (i = 0; i < numplayers; i++)
        {
        pp = &Player[i];
        TRAVERSE(&pp->PanelSpriteList, psp, next)
            {
            // dont need to set Next and Prev this was done
            // when sprites were inserted
            
            // sibling is the only PanelSprite (malloced ptr) in the PanelSprite struct
            psp->sibling = PanelNdxToSprite(pp, (long)psp->sibling);
            }
        }
    #endif


    // what is this for? don't remember
    totalclock = totalsynctics;
    ototalclock = totalsynctics;
    
    // this is ok - just duplicating sector list with pointers
    for (sop = SectorObject; sop < &SectorObject[SIZ(SectorObject)]; sop++)
        {
        for (i = 0; i < sop->num_sectors; i++)
            sop->sectp[i] = &sector[sop->sector[i]];
        }
    
    //!!Again this will not work here
    //restore players info
    for (i = 0; i < numplayers; i++)
        {
        #if PANEL_SAVE
        pp->CurWpn = PanelNdxToSprite(pp, (long)pp->CurWpn);

        for (ndx = 0; ndx < MAX_WEAPONS; ndx++)
            pp->Wpn[ndx] = PanelNdxToSprite(pp, (long)pp->Wpn[ndx]);

        for (ndx = 0; ndx < MAX_INVENTORY; ndx++)
            pp->InventorySprite[ndx] = PanelNdxToSprite(pp, (long)pp->InventorySprite[ndx]);

        pp->Chops = PanelNdxToSprite(pp, (long)pp->Chops);
        pp->InventorySelectionBox = PanelNdxToSprite(pp, (long)pp->InventorySelectionBox);
        pp->MiniBarHealthBox = PanelNdxToSprite(pp, (long)pp->MiniBarHealthBox);
        pp->MiniBarAmmo = PanelNdxToSprite(pp, (long)pp->MiniBarAmmo);

        for (ndx = 0; ndx < SIZ(pp->MiniBarHealthBoxDigit); ndx++)
            pp->MiniBarHealthBoxDigit[ndx] = PanelNdxToSprite(pp, (long)pp->MiniBarHealthBoxDigit[ndx]);

        for (ndx = 0; ndx < SIZ(pp->MiniBarAmmoDigit); ndx++)
            pp->MiniBarAmmoDigit[ndx] = PanelNdxToSprite(pp, (long)pp->MiniBarAmmoDigit[ndx]);

        #endif
        }

    InitNetVars();
    InitTimingVars();
    
    sprintf(ds, "done loading");
    MONO_PRINT(ds);
    
    return(0);
}

VOID
ScreenSave(MFILE fout)
    {
    long num;
    num = MWRITE((void*)waloff[SAVE_SCREEN_TILE], SAVE_SCREEN_XSIZE * SAVE_SCREEN_YSIZE, 1, fout);
    ASSERT(num == 1);
    }
    
VOID
ScreenLoad(MFILE fin)
    {
    long num;
    setviewtotile(SAVE_SCREEN_TILE, SAVE_SCREEN_YSIZE, SAVE_SCREEN_XSIZE);
    num = MREAD((void*)waloff[SAVE_SCREEN_TILE], SAVE_SCREEN_XSIZE * SAVE_SCREEN_YSIZE, 1, fin);
    //num = fread((void*)waloff[SAVE_SCREEN_TILE], SAVE_SCREEN_XSIZE * SAVE_SCREEN_YSIZE, 1, fin);
    
    setviewback();
    //ASSERT(num == 1);
    }

#if 0    
#if DEBUG    
// can be called every iteration to catch holes    
VOID SaveRestoreValidate(VOID)
    {
    PLAYERp pp;
    SPRITEp sp;
    USERp u;
    int i,ndx;
    ANIMp a;
    PANEL_SPRITEp cur, next;

    for (i = 0; i < numplayers; i++)
        {
        pp = &Player[i];
        
        // test player action func
        ASSERT(PlayerFuncPtrToNdx(pp->DoPlayerAction) != -1);
        ASSERT(PanelSpriteToNdx(&Player[i], pp->CurWpn) != -1);
        
        for (ndx = 0; ndx < MAX_WEAPONS; ndx++)
            {
            if (pp->Wpn[ndx])
                {
                ASSERT(PanelSpriteToNdx(&Player[i], pp->Wpn[ndx]) != -1);
                }
            }
        for (ndx = 0; ndx < MAX_INVENTORY; ndx++)
            {
            if (pp->InventorySprite[ndx])
                {
                ASSERT(PanelSpriteToNdx(&Player[i], pp->InventorySprite[ndx]) != -1);
                }
            }
        
        TRAVERSE(&pp->PanelSpriteList, cur, next)
            {
            if (cur->sibling)
                ASSERT(PanelSpriteToNdx(pp, cur->sibling) != -1);
        
            if (cur->PanelSpriteFunc)
                ASSERT(PanelSpriteFuncPtrToNdx(cur->PanelSpriteFunc) != -1);
            
            if (cur->State)    
                {
                // test all states
                ASSERT(PanelStatePtrToNdx(cur->State) != -1);
                if (cur->RetractState)    
                    ASSERT(PanelStatePtrToNdx(cur->RetractState) != -1);
                if (cur->PresentState)    
                    ASSERT(PanelStatePtrToNdx(cur->PresentState) != -1);
                if (cur->ActionState)    
                    ASSERT(PanelStatePtrToNdx(cur->ActionState) != -1);
                if (cur->RestState)    
                    ASSERT(PanelStatePtrToNdx(cur->RestState) != -1);
                }
            }
        }
            
    for(i = AnimCnt - 1, a = Anim; i >= 0; i--)
        {
        if (a->callback)
            {
            ASSERT(AnimFuncPtrToNdx(a->callback) != -1);
            }
        }

    for (i = 0; i < MAXSPRITES; i++)
        {
        sp = &sprite[i];
        u = User[i];
        
        if (u)
            {
            if (u->State)
                {
                if (u->Rot && u->RotNum)    
                    {
                    ASSERT(RotPtrToNdx(u->Rot) != -1);
                    if (u->StateFallOverride)
                        ASSERT(RotPtrToNdx(u->StateFallOverride));
                    }
                else
                    {
                    ASSERT(StatePtrToNdx(u->StateStart) != -1);
                    if (u->StateEnd)
                        ASSERT(StatePtrToNdx(u->StateEnd) != -1);
                    }    
                }
            else
            if (u->ActorActionFunc)    
                {
                ASSERT(FuncPtrToNdx(u->ActorActionFunc) != -1);
                }    
            }    
        }
    }
#endif    
#endif
