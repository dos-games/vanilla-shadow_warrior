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

#include <dos.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <assert.h>
#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>

#define DPMI_INT        0x31


struct meminfo {
    unsigned LargestBlockAvail;
    unsigned MaxUnlockedPage;
    unsigned LargestLockablePage;
    unsigned LinAddrSpace;

    unsigned NumFreePagesAvail;
    unsigned NumPhysicalPagesFree;
    unsigned TotalPhysicalPages;
    unsigned FreeLinAddrSpace;
    unsigned SizeOfPageFile;
    unsigned Reserved[3];
} MemInfo;


typedef struct
    {
    char *TrackPoint;
    long ttflags;
    short flags;
    short NumPoints;
    }TRACK, *TRACKp;

void *
CallocMem(int size, int num)
    {
        char *bp;
//    return (calloc(size, num));
    bp = calloc(size,num);
//      memset(bp,0,size*num);
        assert(bp != NULL);
        return(bp);
    }

void
FreeMem(void *ptr)
    {
    free(ptr);
    }

void showmem()
    {
    union REGS regs;
    struct SREGS sregs;
    size_t stacksize;

    regs.x.eax = 0x00000500;
    memset( &sregs, 0, sizeof(sregs) );

    sregs.es = FP_SEG( &MemInfo );
    regs.x.edi = FP_OFF( &MemInfo );


    int386x( DPMI_INT, &regs, &regs, &sregs );
    printf( "Largest available block (in bytes): %lu\n",
            MemInfo.LargestBlockAvail );
    #if 0
    printf( "Maximum unlocked page allocation: %lu\n",
            MemInfo.MaxUnlockedPage );
    printf( "Pages that can be allocated and locked: "

            "%lu\n", MemInfo.LargestLockablePage );
    printf( "Total linear address space including "
            "allocated pages: %lu\n",
            MemInfo.LinAddrSpace );
    printf( "Number of free pages available: %lu\n",
             MemInfo.NumFreePagesAvail );
    printf( "Number of physical pages not in use: %lu\n",
             MemInfo.NumPhysicalPagesFree );

    printf( "Total physical pages managed by host: %lu\n",
             MemInfo.TotalPhysicalPages );
    printf( "Free linear address space (pages): %lu\n",
             MemInfo.FreeLinAddrSpace );
    printf( "Size of paging/file partition (pages): %lu\n",
             MemInfo.SizeOfPageFile );
    #endif
    }


//#define MAXARRAY 1000
#define MAXARRAY 200
#pragma on (check_stack);

main()
{
    union REGS regs;
    struct SREGS sregs;
    size_t stacksize;

//    TRACK Track[MAXARRAY];
    long i;
//  char *teststr[MAXARRAY];
    void *ReserveMem;
    short num = -3;

       num -= -3; 
    printf("-3 - -3 = %d", num);
    getch();
    exit(0);
    
    printf("FREE HEAP AT START:\n");
    showmem();

    while(1)
        {
            i = (long)(rand()*1024);
            if (i > 29000000L) i=29000000L;
            printf("MALLOCING: %ld\n",i);
            ReserveMem = (void *)malloc(i);

            printf("FREE AFTER MALLOC:\n");
            showmem();

            if(ReserveMem != NULL)
                {
                free(ReserveMem);
                printf("FREE:\n");
                showmem();
                } else
                {
                    printf("Out of memory!!!\n");
                    exit(0);
                }
        }
    
    getch();
    printf("Done.\n");
}


