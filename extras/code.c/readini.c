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

// FILE: readini.cpp
// PURPOSE:  implementation of portable INI readin routines

#if WINDOWS
#ifdef __WATCOMC__

#ifdef WIN32
#include <windows.h>

#else
#include <wcdefs.h>
#endif		// WIN32

#else
#include <windows.h>
#endif
#else
#define TRUE 1
#define FALSE 0
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "readini.h"

char *strtoupper(char *str)
{
   unsigned int i;
   char *pt = str;
   for(i=0;i<strlen(str);i++,pt++)
          *pt = toupper(*pt);
   return(str);
}


BOOL FindSection(FILE *fh,char *sectionName)
{
         char instr[128],*ptr,*ptr2,*nameuc,*taguc=NULL;
        nameuc = strdup(sectionName);
        (void)strtoupper(nameuc);
        fseek(fh,0,SEEK_SET); // start from top of file
        do {
          fgets(instr,128,fh);
          ptr = instr;
          while ((*ptr!=NULL)&&(*ptr!='[')) ptr++;
          if (*ptr!=NULL) {
                  ptr2 = ptr;
                 while ((*ptr2!=NULL)&&(*ptr2!=']')) ptr2++;
                 ptr++; // advance past [
                 *ptr2 = NULL; // eliminate ]
          }
          if (taguc!=NULL) free(taguc);
          taguc = strdup(ptr);
          (void)strtoupper(taguc);
        } while ((strcmp(taguc,nameuc)!=0)&&(!feof(fh)));
        if (strcmp(taguc,nameuc)==0){
                free(taguc);
                return TRUE;
        } else {
                if (taguc!=NULL) free(taguc);
                return FALSE;
   }
}

unsigned int GetOption(FILE *fh, char *key,char *optStr, long strSz)
{
        char instr[128],inKey[80],*ptr;
        long pos;
        char *keyuc;
        int i;

        keyuc = strdup(key);
        (void)strtoupper(keyuc);

        pos = ftell(fh); // store start of section

        do {
          fgets(instr,128,fh);
          if (instr[0] != ';'){ // not a comment
             if (instr[strlen(instr)-1] == '\n') //trim the trailing \n
                 instr[strlen(instr)-1] = NULL;
             for (i=0;instr[i]==' ';i++);
             ptr = inKey;
             for (;(instr[i]!=' ')&&(instr[i]!='=');i++)
                     *ptr++ = instr[i];
             *ptr = NULL;
             (void)strtoupper(inKey);
          }   
        } while ((inKey[0]!='[') && // reached new section
                         (strcmp(inKey,keyuc)!=0) && // found key
                         (!feof(fh))); // end of file
         fseek(fh,pos,SEEK_SET); // return to start of section
         if (strcmp(inKey,keyuc)==0){
                ptr = instr;
          while ((*ptr != NULL)&&(*ptr != '=')) ptr++;
          if (*ptr == '=') ptr++;
          strncpy(optStr,ptr,strSz);
                return strlen(optStr);
         } else return 0;
}
