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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
    {
    char GlobName[64];
    long GlobOffset;
    }GLOB_TABLE, *GLOB_TABLEp;

    
GLOB_TABLE GlobTable[6000];    
extern int _nullarea;
extern int __begtext;
    
void * GlobDataPtrToOffset(char *ptr)
    {
    return((void *)(ptr - ((char *)(&_nullarea))));
    }

void * GlobFuncPtrToOffset(char *ptr)
    {
    return((void *)(ptr - ((char *)(&__begtext)) + 3));
    }
    
char * NextLine(char *ptr)    
    {
    char *new_ptr;
    
    //new_ptr = strtok(ptr,"\n");
    new_ptr = strchr(ptr,'\n');

    // only increment if non 0
    if (new_ptr)
        new_ptr++;

    return(new_ptr);
    }

long ParseMapFile(char *base_ptr)
    {
    char *line_ptr;
    long ndx = 0;
    char tmp[256];
    
    line_ptr = base_ptr;
    
    do    
        {
        if (memcmp(line_ptr, "0003:", 5) == 0)
            {
            sscanf(line_ptr, "%*5s %[0123456789abcdefABCDEF\+\*] %s", tmp, GlobTable[ndx].GlobName);
            sscanf(tmp,"%x", &GlobTable[ndx].GlobOffset);

            ndx++;
            }
        }
    while ((line_ptr = NextLine(line_ptr)) != NULL);
    
    return(ndx);
    }    

int LongCompare(long *key, long *base)
    {    
    return(*key - *base);
    }                          
                              
GLOB_TABLEp SearchGlobTable(int count, unsigned long key_glob_offset)
    {    
    short i;
    
    #if 0
    GLOB_TABLEp gptr;
    
    gptr = bsearch( &key_glob_offset, GlobTable,
               count,
               sizeof(GlobTable[0]),
               LongCompare);
        
    if (gptr)      
        return(gptr - GlobTable);
    #endif    
    
    for (i = 0; i < count; i++)
        {
        if (GlobTable[i].GlobOffset == key_glob_offset)
            return(&GlobTable[i]);
        else
        if (GlobTable[i].GlobOffset > key_glob_offset)
            return(&GlobTable[i-1]);
        }    
    
    return(NULL);
        
    }                          
    

long int filesize( FILE *fp )
  {
    long int save_pos, size_of_file;

    save_pos = ftell( fp );
    fseek( fp, 0L, SEEK_END );
    size_of_file = ftell( fp );
    fseek( fp, save_pos, SEEK_SET );
    return( size_of_file );
  }
    
char * LoadMapFile(char *name)
    {
    FILE *fin;
    long bytes_read;
    long size;
    static char GlobBuffer[1<<19];
    
    fin = fopen(name, "rb");
    
    if (!fin)
        return(NULL);

    size = filesize(fin);

    bytes_read = fread(GlobBuffer, size, 1, fin);

    GlobBuffer[size] = '\0';

    fclose(fin);

    return(GlobBuffer);
    }
    
void MapFileControl(void)
    {
    char *base_ptr;
    int count = 0;
    unsigned long offset;
    extern char InputMode;
    extern long totalsynctics;
    GLOB_TABLEp gt_ptr;
    
    base_ptr = LoadMapFile("sw.map");
    
    if (base_ptr)
        {
        count = ParseMapFile(base_ptr);
        
        offset = GlobDataPtrToOffset(&InputMode);
        gt_ptr = SearchGlobTable(count, offset);

        offset = GlobDataPtrToOffset(((char*)(&totalsynctics))+1);
        gt_ptr = SearchGlobTable(count, offset);
        
        offset = GlobFuncPtrToOffset((char*)LoadMapFile);
        printf("LoadMapFile offset %ld", offset);     
        }
    }    

#if 1
void GlobTest(void)    
    {
    char glob_offset[128];
    char glob_name[128];
    //char trash[128];
    char line_ptr[128] = "0003:0001048c  _CheatInputMode";
    //char line_ptr[128] = "0003:000074ec+ _gm_on";
    long val;

    /*
    sscanf(line_ptr, "%*5s %[^\+\* ] %*1s %s", glob_offset, glob_name);
    printf("%s %s\n", glob_offset, glob_name);
    
    sscanf(line_ptr, "%*5s %[^\+\* ] %*1[\+\* ] %s", glob_offset, glob_name);
    printf("%s %s\n", glob_offset, glob_name);

    sscanf(line_ptr, "%*5s %[^\+\* ] %*[^_] %s", glob_offset, glob_name);
    printf("%s %s\n", glob_offset, glob_name);
 
    sscanf(line_ptr, "%*5s %s %s", glob_offset, glob_name);
    sscanf(glob_offset,"%x",&val);
    printf("%s %s %x\n", glob_offset, glob_name, val);
    
    sscanf(line_ptr, "%*5s %x %s", &val, glob_name);
    printf("%s %x\n", glob_name, val);

    sscanf(line_ptr, "%*5s %x %*[\+\*] %s", &val, glob_name);
    printf("%s %x\n", glob_name, val);

    sscanf(line_ptr, "%*5s %[0123456789abcdefABCDEF\+\*] %s", glob_offset, glob_name);
    sscanf(glob_offset,"%x",&val);
    printf("%s %s %x\n", glob_offset, glob_name, val);

    exit(0);
    */
}    
#endif
