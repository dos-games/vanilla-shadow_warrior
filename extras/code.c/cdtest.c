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

     /*
         SIMULATE.C - Shows how to issue a real-mode interrupt
         from protected mode using DPMI call 300h.  Any buffers
         to be passed to DOS must be allocated in DOS memory
         This can be done with DPMI call 100h.  This program
         will call DOS int 21, function 39h, "Create
         Directory".

         Compile & Link: wcl386 /l=dos4g simulate
     */

     #include <i86.h>
     #include <dos.h>
     #include <stdio.h>
     #include <string.h>

#define ADDRESS_MODE_HSG        0
#define ADDRESS_MODE_RED_BOOK   1

#define STATUS_ERROR_BIT        0x8000
#define STATUS_BUSY_BIT         0x0200
#define STATUS_DONE_BIT         0x0100
#define STATUS_ERROR_MASK       0x00ff

#define ERROR_WRITE_PROTECT     0
#define ERROR_UNKNOWN_UNIT      1
#define ERROR_DRIVE_NOT_READY   2
#define ERROR_UNKNOWN_COMMAND   3
#define ERROR_CRC_ERROR         4
#define ERROR_BAD_REQUEST_LEN   5
#define ERROR_SEEK_ERROR        6
#define ERROR_UNKNOWN_MEDIA     7
#define ERROR_SECTOR_NOT_FOUND  8
#define ERROR_OUT_OF_PAPER      9
#define ERROR_WRITE_FAULT       10
#define ERROR_READ_FAULT        11
#define ERROR_GENERAL_FAILURE   12
#define ERROR_RESERVED_13       13
#define ERROR_RESERVED_14       14
#define ERROR_BAD_DISK_CHANGE   15

#define COMMAND_READ            3
#define COMMAND_WRITE           12
#define COMMAND_PLAY_AUDIO      132
#define COMMAND_STOP_AUDIO      133
#define COMMAND_RESUME_AUDIO    136

#define READ_REQUEST_AUDIO_CHANNEL_INFO         4
#define READ_REQUEST_DEVICE_STATUS              6
#define READ_REQUEST_MEDIA_CHANGE               9
#define READ_REQUEST_AUDIO_DISK_INFO            10
#define READ_REQUEST_AUDIO_TRACK_INFO           11
#define READ_REQUEST_AUDIO_STATUS               15

#define WRITE_REQUEST_EJECT                     0
#define WRITE_REQUEST_RESET                     2
#define WRITE_REQUEST_AUDIO_CHANNEL_INFO        3

#define STATUS_DOOR_OPEN                        0x00000001
#define STATUS_DOOR_UNLOCKED                    0x00000002
#define STATUS_RAW_SUPPORT                      0x00000004
#define STATUS_READ_WRITE                       0x00000008
#define STATUS_AUDIO_SUPPORT                    0x00000010
#define STATUS_INTERLEAVE_SUPPORT               0x00000020
#define STATUS_BIT_6_RESERVED                   0x00000040
#define STATUS_PREFETCH_SUPPORT                 0x00000080
#define STATUS_AUDIO_MANIPLUATION_SUPPORT       0x00000100
#define STATUS_RED_BOOK_ADDRESS_SUPPORT         0x00000200

#define MEDIA_NOT_CHANGED       1
#define MEDIA_STATUS_UNKNOWN    0
#define MEDIA_CHANGED          -1

#define AUDIO_CONTROL_MASK              0xd0
#define AUDIO_CONTROL_DATA_TRACK        0x40
#define AUDIO_CONTROL_AUDIO_2_TRACK     0x00
#define AUDIO_CONTROL_AUDIO_2P_TRACK    0x10
#define AUDIO_CONTROL_AUDIO_4_TRACK     0x80
#define AUDIO_CONTROL_AUDIO_4P_TRACK    0x90

#define AUDIO_STATUS_PAUSED             0x0001

#pragma pack(1)

struct playAudioRequest
    {
    char addressingMode;
    int startLocation;
    int sectors;
    };

struct readRequest
    {
    char mediaDescriptor;
    short bufferOffset;
    short bufferSegment;
    short length;
    short startSector;
    int volumeID;
    };

struct writeRequest
    {
    char mediaDescriptor;
    short bufferOffset;
    short bufferSegment;
    short length;
    short startSector;
    int volumeID;
    };

struct cd_request
    {
    char headerLength;
    char unit;
    char command;
    short status;
    char reserved[8];
    union
        {
        struct playAudioRequest playAudio;
        struct readRequest read;
        struct writeRequest write;
        } x;
    };


struct audioChannelInfo_s
    {
    char code;
    char channel0input;
    char channel0volume;
    char channel1input;
    char channel1volume;
    char channel2input;
    char channel2volume;
    char channel3input;
    char channel3volume;
    };

struct deviceStatus_s
    {
    char code;
    int status;
    };

struct mediaChange_s
    {
    char code;
    char status;
    };

struct audioDiskInfo_s
    {
    char code;
    char lowTrack;
    char highTrack;
    int leadOutStart;
    };

struct audioTrackInfo_s
    {
    char code;
    char track;
    int start;
    char control;
    };

struct audioStatus_s
    {
    char code;
    short status;
    int PRstartLocation;
    int PRendLocation;
    };

struct reset_s
    {
    char code;
    };

union readInfo_u
    {
    struct audioChannelInfo_s audioChannelInfo;
    struct deviceStatus_s deviceStatus;
    struct mediaChange_s mediaChange;
    struct audioDiskInfo_s audioDiskInfo;
    struct audioTrackInfo_s audioTrackInfo;
    struct audioStatus_s audioStatus;
    struct reset_s reset;
    };

#pragma pack()

#define MAXIMUM_TRACKS                  32

typedef struct
    {
    int start;
    int length;
    BOOL isData;
    } track_info;

typedef struct
    {
    BOOL valid;
    int leadOutAddress;
    track_info track[MAXIMUM_TRACKS];
    BYTE lowTrack;
    BYTE highTrack;
    } cd_info;

static struct cd_request far *cdRequest;
static union  readInfo_u far *readInfo;
static cd_info cd;

static BOOL playing = false;
static BOOL wasPlaying = false;
static BOOL mediaCheck = false;
static BOOL initialized = false;
static BOOL enabled = true;
static BOOL playLooping = false;
static short cdRequestSegment;
static short cdRequestOffset;
static short readInfoSegment;
static short readInfoOffset;
static BYTE remap[256];
static BYTE cdrom;
static BYTE playTrack;
static BYTE cdvolume;

     static struct rminfo {
         long EDI;
         long ESI;
         long EBP;
         long reserved_by_system;
         long EBX;
         long EDX;
         long ECX;
         long EAX;
         short flags;
         short ES,DS,FS,GS,IP,CS,SP,SS;

     } RMI;

     void main()
     {
         union REGS regs;
         struct SREGS sregs;
         int interrupt_no=0x31;
         short selector;
         short segment;
         char far *str;

         /* DPMI call 100h allocates DOS memory */
         memset(&sregs,0,sizeof(sregs));
         regs.w.ax=0x0100;
         regs.w.bx=0x0004;
         int386x( interrupt_no, &regs, &regs, &sregs);

         segment=regs.w.ax;
         selector=regs.w.dx;

         /* Move string to DOS real-mode memory */
         str=MK_FP(selector,0);

         cdRequest = (struct cd_request far *)memory;
    //   cdRequestSegment = FP_SEG(memory);
         cdRequestSegment = segment;
         cdRequestOffset = FP_OFF(memory);

         readInfo = (union readInfo_u far *)(memory + sizeof(struct cd_request));
         readInfoSegment = cdRequestSegment;
         readInfoOffset = FP_OFF(memory + sizeof(struct cd_request));

         /* Set up real-mode call structure */

         cdRequest->headerLength = 13;
         cdRequest->unit = 0;
         cdRequest->command = COMMAND_WRITE;
         cdRequest->status = 0;

         cdRequest->x.write.mediaDescriptor = 0;
         cdRequest->x.write.bufferOffset = readInfoOffset;
         cdRequest->x.write.bufferSegment = readInfoSegment;
         cdRequest->x.write.length = sizeof(struct reset_s);
         cdRequest->x.write.startSector = 0;
         cdRequest->x.write.volumeID = 0;

         readInfo->reset.code = WRITE_REQUEST_EJECT;

         _fmemcpy( str, (char far *)cdRequest, sizeof(struct cd_request));

         /* Set up real-mode call structure */
         memset(&RMI,0,sizeof(RMI));
         RMI.EAX=0x1510; 
         RMI.ECX=3;
         RMI.ES = segment;
         RMI.EBX = 0;

         //Use DPMI call 300h to issue the DOS interrupt
         regs.w.ax = 0x0300;
         regs.h.bl = 0x2f;
         regs.h.bh = 0;
         regs.w.cx = 0;
         sregs.es   = FP_SEG(&RMI);
         regs.x.edi = FP_OFF(&RMI);
         int386x( interrupt_no, &regs, &regs, &sregs );
     }