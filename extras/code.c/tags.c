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

#ifndef TAGS_H
#define TAGS_H

#include "jtags.h" 

//
// CONTACT Information
//
// Frank Maddin
// (512) XXX-XXXX
// CIS 75XXX,XXXX
//

// NOTE: Switches are always sprites and are usually on walls, triggers are on the floor.  
// 
// KEY:
//
//  *  - denotes this feature is in
//  *? - denotes this feature is in but is suspect
//  *X - denotes this feature is in but don't use it

/*

NOTE for tagging walls.  A red sector wall is actually two walls: An inside and an
outside wall.  Both are tagable by placing the cursor to the inside and outside
of the wall. 

In certain cases it is important to tag specifically the inside or the outside wall.
In others, it does not matter (exp: switches).  The tags list should tell you if a 
certain wall should be tagged.

Z COORDINATES AND PIXELS
------------------------

One pixel in height is equal to 256 Z coordinates.
Therefore a height of 2 pixels is a Z value of 512.

TAGGING POINTS VS WALLS
------------------------

A RED WALL is actually a single point that connects to another point.  So when you are
tagging a RED WALL you are actually tagging a single X,Y location on the map.  Sometimes
it can be difficult to tell what wall goes with what point.  When tagging a specific
point it may be necessary to check the walls x,y location to know if you have the right
wall.  Placing the player directly on the point you want to tag also helps because the
players x,y location is always shown at the bottom.


Note that "inside" walls go clockwise and "outside" walls go counter clockwise.

SECTOR/WALL TAGGING WITH SPRITES
---------------------------

    ------------------------------------------------------------------------------------
    |       TYPE                   |       DATA is in     |                            |
    ------------------------------------------------------------------------------------
    | name                   hitag | lotag  |    other    |        Description         |
    ------------------------------------------------------------------------------------ 
    SECT_SINK                  0     depth
    SECT_RESERVED              1
    SECT_DAMAGE               2     amt                   does damage to player and actors
    SECT_CURRENT               3     speed    sprite ang   
    SECT_SPECIAL               4     type                  special type in hitag (see below)
    SECT_NO_RIDE               5                           cannot ride this moving sector
    SECT_TELEPORT              6     number   position     teleport to SECT_TELEPORT_DEST -
                                                           Place sprite 8 pixels into ground
                                                           for a "GROUND" teleporter where
                                                           you must be touching the floor to
                                                           actually teleport.  Place near the
                                                           ceiling for a "CEILING" teleporter
                                                           where you must touch the ceiling
                                                           to actually teleport.
    SECT_DIVE_AREA             7     number                Diving area - pressing the CRAWL
                                                           key when here will teleport you
                                                           to a UNDERWATER area with a matching
                                                           UNIQUE "number"
    SECT_UNDERWATER            8     number                Underwater area - floating to the 
                                                           surface of this area will teleport
                                                           you to the DIVE_AREA with a matching
                                                           UNIQUE "number"
                                                           IMPORTANT: The SECT_DIVE_AREA sprite
                                                           and the SECT_UNDERWATER sprite need
                                                           to be in the same position for both
                                                           sectors (exp: centered in each room).
    SECT_UNDERWATER2           9     number                Secondary Underwater area - floating 
                                                           to the surface of this area will 
                                                           teleport you to the DIVE_AREA with 
                                                           a matching UNIQUE "number"
                                               
    NOTE: These 3 are tags
    are generic.  Tag 
    descriptions will let 
    you know when to use
    them.
    
    SECT_ANGLE                16              sprite ang
    SECT_HEIGHT               17     height   
    SECT_SPEED                18     speed                 shift value (0-6)
    
    SECT_FLOOR_PAN            19     speed    sprite ang   "
    SECT_CEILING_PAN          21     speed    sprite ang   "
    
    WALL_PAN_SPEED            23     speed                 point sprite at desired wall
    WALL_PAN_ANG              24     pan ang               point sprite at desired wall
    
    SECT_DEBRIS_SEWER         25     seconds  sprite ang   spawns sewer debris after
                                                           time
                                                           
    SECT_TELEPORT_DEST        26     number   sprite ang   set a teleport destination- 
                                              position     any angle other than the default
                                                           angle of 1536 will cause player
                                                           to face that angle when teleported.
                                                           Place sprite on ceiling to appear
                                                           near the ceiling, otherwise place
                                                           on floor to teleport to the floor.
                                                           
    SECT_SO_CENTER            27              position     Place center of SO at the sprite
                                                           x,y,z location.  Overrides norm
                                                           center.                                        
                                                           
    SECT_MATCH                28     number                Used to match multiple sectors
                                                           to one action.  "number" must
                                                           be UNIQUE and non 0.                                                                
                                                           
    SECT_LOCK_DOOR            29     key num               Lock the door.  Must have the                                                         
                                                           correct key to open it.
                                                           RED     1
                                                           BLUE    2
                                                           GREEN   3
                                                           YELLOW  4
                                                           Currently only works for rotating
                                                           and UP type doors, but I can extend 
                                                           it for others easily as needed.
                                                           
    SPRI_CLIMB_MARKER         30              pos/angle    Center this sprite on the ladder
                                                           and set the angle pointing out.                                
                                                           
    SECT_SO_SPRITE_OBJ        31                           Makes a SO become a SPRITE OBJECT.
                                                           The sprites will move but the sector
                                                           will not.  Sprites can cross sector
                                                           boundries.
                                                           
    SECT_SO_DONT_BOB          32                           Mark a bobbing SO sector so it will
                                                           not bob with the rest of the SO.

    SECT_SO_SINK_DEST         33     offset                Mark a SO sector as the destination
                                                           for sinking.so it will
                                                           not sink with other sectors.
                                                           
    SECT_SO_DONT_SINK         34                           Mark a SO sector so it will
                                                           not sink with other sectors.

    SECT_RAISE_LOWER_PLAT     35     match    angle        Set sector to lower when switch
                                                           TAG_RAISE_LOWER_PLAT_SWITCH is pressed.
                                                           Everything is connected together
                                                           with a match number that must be
                                                           unique. Must have a matching
                                                           SECT_RAISE_LOWER_PLAT_DEST set.
                                                           Can use ST1 sprite SECT_SPEED to 
                                                           set the speed.
                                                           
                                                           If the angle is set to anything other
                                                           than 1536 (default) then the pic
                                                           is copied from the PLAT_DEST.
    
    SECT_RAISE_LOWER_PLAT_DEST 36    match                 Set destination for platform to
                                                           travel to. 
                                  
    SECT_SO_FORM_WHIRLPOOL    37     depth                 Used for the boat in level 1.
                                                           Lower the whirlpool sectors
                                                           to a "depth" level.
                                                           
    SECT_BLOCK_ACTOR          38                           Tag sector to keep enemy actors
                                                           from entering.  Player can still
                                                           enter.  Can still fire through
                                                           the sector.  Don't do tons of these
                                                           per level.  Limit of 50.                                                          

    SECT_SO_CLIP_DIST         39     dist                  Sets clipping distance for          
                                                           operational SO's.
                                                                                                        
    SECT_CEILING_STOMPER      40     accel                 For speed setting place a SECT_SPEED
                                                           ST1 sprite in the sector.  Acceleration
                                                           in lotag.
                                                           
    SECT_FLOOR_STOMPER        41     accel                 For speed setting place a SECT_SPEED
                                                           ST1 sprite in the sector.  Acceleration
                                                           in lotag.
                                                           
    MULTI_PLAYER_START        42     pnum     angle        Spawn spots for multi-player games.
                                                           Valid values for player number (pnum)
                                                           are 1-15 - Note that 0 is the first  
                                                           player.  Do set the angle.
                                                           
    FIREBALL_TRAP             43     match    angle        Shoots a fireball from the location
                                                           of this sprite in the direction the  
                                                           angle is pointing when a TRIGGER     
                                                           is stepped on.  Sector trigger set 
                                                           TAG_TRIGGER_MISSILE_TRAP 
                                                           

    BOLT_TRAP                 44     match    angle        Shoots a bow bolt from the location
                                                           of this sprite in the direction the  
                                                           angle is pointing when a TRIGGER     
                                                           is stepped on.  Sector trigger set 
                                                           TAG_TRIGGER_MISSILE_TRAP 

    SECT_SO_DONT_ROTATE       45                           Makes a SO move without rotating.  
                                                           Place in the center sector.

    PARALLAX_LEVEL            46     plaxnum               Set parallax sky type 1-6. Only 
                                                           need one per level in a single 
                                                           sector.  Will effect the whole level.
                                                           
    SECT_DONT_COPY_PALETTE    47                           Normally a actor/player standing on 
                                                           a palette swapped floor takes on the
                                                           palette of the floor.  Use this ST1
                                                           tag on sectors that should act like 
                                                           this.
                                                           
    MULTI_COOPERATIVE_START   48     pnum     angle        Spawn spots for coperative multi-player 
                                                           games.  Use just like MULT_PLAYE_START.

                                                                
    The following several ST1 tags
    can be placed in the center sector
    of a SO instead of using the TRACK
    equivelents.
    
    SO_SET_SPEED              49     speed                 Same function as TRACK_SET_SPEED
    SO_SPIN                   50     speed                 Same function as TRACK_SPIN     
    SO_SPIN_REVERSE           51     speed                 Same function as TRACK_SPIN_REVERSE
    SO_BOB_START              52     amt                   Same function as TRACK_BOB_START
    SO_BOB_SPEED              53     speed                 Same function as TRACK_BOB_SPEED
    SO_STOMP                  54     speed                 Same function as TRACK_STOMP    
    
    SO_TURN_SPEED             55     speed                 Determines how fast a SO turns to 
                                                           meet the new angle.  Shift value.
                                                           Default is 4.
                                                           
    LAVA_ERUPT                56     random   ang          Spawn spots for lava eruption.
                                              pal          Random number determines how often
                                              clipdist     something is spawned. 
                                                           Angle is interval in seconds.
                                                           Pal is time to erupt.
                                                           Clipdist is for future.
                                                           
    SECT_EXPLODING_CEIL_FLOOR 57                                           
    SECT_EXPLODING_CEIL       58                                           
    SECT_EXPLODING_FLOOR      59                                           
                                                       
    SECT_SO_BOUNDING          500-600                      Bounding sprites for SO's                  
                                                           Look at Sector Object Tags Info
                                                           
    ---------------------------------------------------------------------------------                                                     
    |SPECIAL TAGS                                                                   |
    ---------------------------------------------------------------------------------                                                     
    SPECIAL_DEBRIS_KILL        0     //kills debris                                     
    RESERVED                   1                   
    RESERVED                   2                   
    
    NOTE 0-15 are used for true/false (is this sector sinkable - TRUE) settings
    along with data.  Others are used just for data.

*/


//////////////////////////////////////////////////////////////////////////////////////////
//
// MISC TAGS
//
//////////////////////////////////////////////////////////////////////////////////////////

//* Finds the nearest TAG_DOOR_TO_TRIGGER and opens it.  Currently door must be a UP opening type.
//* Use sprite tagging SECT_MATCH to correlate one trigger to multiple doors.
#define TAG_OPEN_DOOR_TRIGGER   1
//* High tag is speed of door - 0 defaults to 128
#define TAG_DOOR_TO_TRIGGER     2

//* Stepping on this finds actors closeby that are on a track at a point defined as
//* TRACK_ACTOR_WAIT_FOR_TRIGGER and activates them.
//* High tag = distance to look for actors
#define TAG_TRIGGER_ACTORS      3

//* For doors that move up, tag a sector next to the door with this to let the 
//* door know where to stop at.
#define TAG_DOOR_STOP           4

//* Finds the nearest TAG_DOOR_TO_TRIGGER and opens it.  Currently door must be an UP opening type.
//* High tag is SECT_MATCH to correlate one trigger to multiple doors.
#define TAG_OPEN_DOOR_SWITCH    5

//* For ladders attached to white walls.
#define TAG_LADDER              30

//X* Small sector spikes that move up and down at random intervals.
#define TAG_SPIKE               40

//Shoots a fireball from a ST1 FIREBALL_TRAP (or BOLT) sprite with a matching number.
//High tag = match number to match with ST1 sprite 
#define TAG_TRIGGER_MISSILE_TRAP 43

//////////////////////////////////////////////////////////////////////////////////////////
//
// DOOR TAGS
//
//////////////////////////////////////////////////////////////////////////////////////////

//* High tag for z moving doors is always the speed unless otherwise noted - default
//* 200

//* Standard Door tags - currently they close after about 4 secs
//* To lock these types of doors place a SECT_LOCK_DOOR ST1 in this sector
#define TAG_DOOR_UP             100
//* Standard Door tags - currently they close after about 4 secs
#define TAG_DOOR_DOWN           101

//* Normally looks for a neighboring sector's next lowest ceiling.  If you want 
//* to stop at another ceiling that is connected to the door sector, tag with 
//* this tag.
#define TAG_DOOR_UP_DEST        102

//* Same as TAG_DOOR_UP except it will not close again.
#define TAG_DOOR_UP_DONT_CLOSE  103

//* Flip switch, closest door opens, you go through door, door closes after some time.
//* Can only go through is one way.
#define TAG_ONE_WAY_DOOR_SWITCH 109
#define TAG_ONE_WAY_DOOR        110

//* Red sector rotating doors.  They can have up to 15 points per door and
//* rotate about any point you choose.  Don't have to be rectangular, can be any shape.  Can
//* use masked walls with them.

//* Red sector rotating door - Master sector that has doors inside of it
//* To lock these types of doors place a SECT_LOCK_DOOR ST1 in this sector
#define TAG_DOOR_ROTATE         112

//* Red sector rotating door - Tag the actual door with this. Rotate POSITIVE or NEGATIVE 90 degrees
#define TAG_DOOR_ROTATE_POS     113
#define TAG_DOOR_ROTATE_NEG     114

//* Wall pivot point - MUST tag the ouside of the wall, NOT the inside!! - red sector method
//  Remember - a wall is just an x,y location
#define TAG_WALL_ROTATE_PIVOT 2

//* Tag for sliding door (Star Trek/Wolf type)
#define TAG_DOOR_SLIDING        115

//* Tag for exiting current level and starting up another one.
//* High Tag - Destination level number - 
//* 0 - Title screen
//* 1 - e1l1
//* 2 - e1l2
//* ...
#define TAG_LEVEL_EXIT_SWITCH 116

//////////////////////////////////////////////////////////////////////////////////////////
//
// ELEVATOR TAGS
//
//////////////////////////////////////////////////////////////////////////////////////////

//* High tag for elevator is always the speed unless otherwise noted - default
//* 200

//* Automatic Elevators - go up and down by themselves, you don't have to be next to them
#define TAG_ELEVATOR_AUTO       202

//* Makes TAG_ELEVATOR turn on and act like TAG_ELEVATOR_AUTO.
#define TAG_ELEVATOR_SWITCH     203
#define TAG_ELEVATOR_AUTO_STOPPED  204  // turned on by TAG_ELEVATOR_SWITCH

//* Player stands on the elevator and it moves to its opposite position.  Player gets off and it 
//* returns to original posion.
//* High tag sets the amount of time that is stays up or down
#define TAG_ELEVATOR_TRIGGER_UP    205
#define TAG_ELEVATOR_TRIGGER_DOWN    206

//* Operate on 205 & 206 to bring them down without stepping on them
#define TAG_ELEVATOR_TRIGGER_UP_SWITCH      207
#define TAG_ELEVATOR_TRIGGER_DOWN_SWITCH    208

// Given: elevator starts out on the floor.  Player flips the switch and the elevator goes up.  
// It returns to its original position after a period of time. 
#define TAG_ONE_WAY_ELEVATOR_SWITCH     209
#define TAG_ONE_WAY_ELEVATOR            210

//* To raise or lower platforms.  Manipulated platforms to move toward the
//* destination sectors Z coord.  Use ST1 sprites SECT_RAISE_LOWER_PLAT and 
//* SECT_RAISE_LOWER_PLAT_DEST to set up the actual sectors you want to raise/lower
//* High tag is the number to MATCH the ST1 match number.
//* Currently can only use each once per level, but this can be changed.
#define TAG_RAISE_LOWER_PLAT_SWITCH   213
//* Same as switch but tag a sector as a trigger.
#define TAG_RAISE_LOWER_PLAT_TRIGGER  213

//* Step on the sector, press the space bar, and it shoves you high into the air.
//* High tag has height of jump
#define TAG_SPRING_BOARD        240

//* Switch to make stairs from the ceiling or floor.  START is the first sector.  Other sectors are 
//* tagged with UP/DOWN.
//* High tag for switch is maximum distance for finding the next stair.
#define TAG_STAIRS_DOWN_SWITCH  241
#define TAG_STAIRS_DOWN_START   242
//* High tag = delta Z in pixels from the last stair.
#define TAG_STAIRS_DOWN         243
#define TAG_STAIRS_UP_SWITCH    244
#define TAG_STAIRS_UP_START     245
#define TAG_STAIRS_UP           246

//? Just like real life elevators, you call them with a switch and they come down and wait for
// you. Use's high tags to match a switch with an elevator.
// 
#define TAG_ELEVATOR_UP_SWITCH  250
#define TAG_ELEVATOR_UP         251

#define TAG_ELEVATOR_DOWN_SWITCH  252
#define TAG_ELEVATOR_DOWN         253

//* Raise or Lower water with the TAG_SPRITE_WATER_VALVE
//* High tag = height to raise or lower water in pixels.  Add 10000 for a negative number.
//* Exp: -256 would be 10256.
#define TAG_WATER_GATE          254
//* Same as WATER_GATE except that all sectors tagged with 255 are effected.
//* Note that this can only be used once per level.
#define TAG_WATER_GATE_ALL      255

//* Sine wave floor effect - max 5 per level
//*
//* Tag the first sector with 400, the next with 401, and so on up to 419.
//* The WAVE will actually flow from the opposite end.  The WAVE direction is what is 
//* refered to when BEGINNING and END are referred to below.
//*
//* 1st Sector High Tag = the range in pixels the floor will undulate in the Z direction
//* 2rd Sector High Tag = range decrement - takes the range from the 1st sector high tag and
//*                       adjusts all the rest by this decrement - makes the WAVE start off 
//*                       small and get LARGER toward the end - THIS IS IN Z COORDINATES 
//*                       *NOT* PIXELS
//* 3nd Sector High Tag = speed of the motion - a shift value - default to 3 
//* 4th Sector High Tag = a distance from one peak (top of curve) to the next - default is the
//*                       number of sectors in the SINE WAVE (max of 20)
//* Last Sector High Tag= special tag to make a realistic ocean wave - modifies the range 
//*                       toward the END (greatest range is usually here) of the WAVE
//*                       so that it dissapates quickly. Without this it would just continue
//*                       to get bigger toward the end of the WAVE.

#define TAG_SINE_WAVE_FLOOR     400
#define TAG_SINE_WAVE_CEILING   420
#define TAG_SINE_WAVE_BOTH      440


//////////////////////////////////////////////////////////////////////////////////////////
//
// SECTOR OBJECT TAGS
//
//////////////////////////////////////////////////////////////////////////////////////////
// Sector Objects (SOs) are groups of sectors that can be moved around on a track.
// All SOs must have at a minimum:
//      1. Bounding sprites (upper left and lower right) that contains all sectors to be moved.
//      2. A sector marked as the center of the SO.
// 
// Tags 500-600 are set reserved for SOs.  There is a maximum of 20 SOs per level.  
//
// Tags 500-504 correspond to object 1, 505-509 to object 2, 510-514 to object 3, etc ...
// 
// Bounding sprite tags are set up similar.  
//      Upper left tag of object 1 is 500, lower right is 501
//      Upper left tag of object 2 is 505, lower right is 506
//      Etc...
//////////////////////////////////////////////////////////////////////////////////////////

// Not really used yet.
#define TAG_OBJECT              500
// The center of this sector will be the center of the SO
// IMPORTANT: High Tag must be set to the track number
#define TAG_OBJECT_CENTER       501
// Velocity of object in high tag
#define TAG_OBJECT_VELOCITY     502
// Not really used yet.
//#define TAG_OBJECT              503
// Bounding sector for sector object
//#define TAG_OBJECT_BOUNDS       504


//////////////////////////////////////////////////////////////////////////////////////////
//
// TRACK TAGS FOR SECTOR OBJECTS
//
//////////////////////////////////////////////////////////////////////////////////////////
// Tracks allow SOs and sprites to follow a path designated by the map-maker.
// All Tracks must have at a minimum:
//      1. A TRACK_START.
//      2. A TRACK_END.
//
// Tracks are set up by the program by connecting sprites placed by the map-maker in the BUILD 
// editor.  The track sprites start at location 1900 and end at 1999 so there are 100 available
// tracks.  Tracks automatically loop back to the beginning when the TRACK_END sprite is
// detected.  Track tags listed below can be set to modify the behavior of objects moving along 
// them.
//
// Look at the tracks in example maps to see how they are placed.
//
// NOTE: Track direction is dependent on the next closest sprite to the TRACK_START sprite.
// As noted below, certain track tags are dependant on the direction the object is traveling on
// on the track.
//
// For placing actor sprite on the tracks:
//      1. Put a sprite down near the point you want it to start from.
//      2. Adjust the angle to the left to make the sprite move in the "reverse" direction
//         and to the right to make the sprite move in the "forward" direction.
//      3. For the pic, place the first pic of the actor found in the editart file.
//
// Every track can have a type set in the high tag of TRACK_START.
//////////////////////////////////////////////////////////////////////////////////////////

    
//* Mark first track sprite with this
//* High tag = Type of track (defined next)
#define TRACK_START             700 

    //
    // TRACK TYPES - only valid for sprites only NOT for SOs
    //
    // Used to attach a name to a track purpose.  The actors will "look around" for
    // tracks to know what actions are available to them.
    //
    
    // All tracks are assumed to be SEGMENTS (non-circular, generally leading in a certain
    // direction) unless otherwise noted.  SEGMENTS are generally kept short with a few
    // exceptions.

    // Follow a circular route available for sprites to hop on and off.
    // Generally covers a larger area than TT_WANDER.
    #define TT_ROUTE            1
        
    // Jump up only
    #define TT_JUMP_UP          2
    // Jump down only
    #define TT_JUMP_DOWN        3
    // Jump up/down track
    #define TT_JUMP_BOTH        4
    // Ladder track.  Currently only good for going up.  Must jump down.
    #define TT_LADDER           5
    // Stair track.  Hard for sprites to maneuver narrow stairs well without tracks.
    #define TT_STAIRS           6
    
    // Traverse a complex route - generally thought of to move you from point A to point B
    // in complex map situations.
    #define TT_TRAVERSE         7
    
    // Duck behind cover and attack
    #define TT_DUCK_N_SHOOT     8
    // Hide behind cover and attack
    #define TT_HIDE_N_SHOOT     9
    
    // Exit tracks to exit a room/area.  Probably should lead to a door alot of the time.
    #define TT_EXIT             10

    // Wander track. Wander about general area until you come upon a player.
    #define TT_WANDER           11
    
    // Scan for other tracks.  Generally a VERY short track put these in places where 
    // other tracks can be seen easily.  Good "vantage points".
    #define TT_SCAN             12
    
    // Super Jump 
    #define TT_SUPER_JUMP_UP    13
    

//* Mark last sprite with this
#define TRACK_END               701
//* Set the target speed and actual speed at this point
//* Valid values for target speed are 2 to 128
#define TRACK_SET_SPEED         702
//* Stop for (seconds) in high tag
#define TRACK_STOP              703
//* Reverse the direction
#define TRACK_REVERSE           704    

// Note that the next two tags have the opposite effect when traveling the REVERSE direction

//* Sets up a target speed by a delta (amt) in high tag
//* velocity = (current target velocity) + (speed up amount in high tag)
//* Valid values for target speed are 2 to 128
#define TRACK_SPEED_UP          705    

//* Sets up a target speed by a delta (amt) in high tag
//* velocity = (current target velocity) - (speed up amount in high tag)
//* Valid values for target speed are 2 to 128
#define TRACK_SLOW_DOWN         706    

//* The rate at which the velocity approaches the target speed.
//* This is defined as a shift value (for processing speed) and defaults to 6.  The valid range
//* is 1 to 12.
#define TRACK_VEL_RATE          707   


//* Moves the floor of the object by a delta PIXEL position (amt) in high tag
#define TRACK_ZUP               709
//* Moves the floor of the object by a delta PIXEL position (amt) in high tag
#define TRACK_ZDOWN             710
//* Sets the rate at which the ZUP/ZDOWN moves. Defaults to 256.
#define TRACK_ZRATE             711
//* Special mode where the object looks at the zcoord of the next track point and moves
//* to achieve the destination.  In high tag is a negative delta Z from the placement of 
//* the track sprites z value.
#define TRACK_ZDIFF_MODE        712

//* Object spins as it goes along its track.  Spin speed in high tag.
//* For now, once you start it spinning you cannot stop it.
#define TRACK_SPIN              715

//* Object stops spining.  Angle to finish at in high tag.
#define TRACK_SPIN_STOP         716

//* Bobbing for SO's. 
//* High tag = Bob amt in pixels.
#define TRACK_BOB_START         717
//* High tag = Bob speed - shift amt pixels.
#define TRACK_BOB_SPEED         718
// Bobbing for SO's. 
#define TRACK_BOB_STOP          719
//* Start object spinning in the opposite direction.
#define TRACK_SPIN_REVERSE      720

//* Start object stomping from ceiling
#define TRACK_STOMP             721

//* Start object sinking to ST1 SECT_SO_SINK_DEST
//* For boats
//* High tag = speed of sinking
#define TRACK_SO_SINK           723

//* For boats - lower whirlpool sector - tagged with ST1 SECT_SO_FORM_WHIRLPOOL
#define TRACK_SO_FORM_WHIRLPOOL   724

//* Move Sprite Objects (not sector objects) straight up or down in the z 
//* direction to the next points z height then continues along the track
//* High tag = speed of movement - default is 256
#define TRACK_MOVE_VERTICAL    725

///////////////
//
// TRACK TAGS FOR SPRITES ONLY
//
///////////////

//* Set the target speed and actual speed at this point
// Valid values for target speed are 2 to 128
#define TRACK_ACTOR_SET_SPEED         750
//* Stop for (seconds) in high tag
#define TRACK_ACTOR_STOP              751
//* Reverse the direction
#define TRACK_ACTOR_REVERSE           752    

// Note that the next two tags have the opposite effect when traveling the REVERSE direction

//* Sets up a target speed by a delta (amt) in high tag
// velocity = (current target velocity) + (speed up amount in high tag)
// Valid values for target speed are 2 to 128
#define TRACK_ACTOR_SPEED_UP          753    

//* Sets up a target speed by a delta (amt) in high tag
// velocity = (current target velocity) - (speed up amount in high tag)
// Valid values for target speed are 2 to 128
#define TRACK_ACTOR_SLOW_DOWN         754    

//* The rate at which the velocity approaches the target speed.
// This is defined as a shift value (for processing speed) and defaults to 6.  The valid range
// is 1 to 12.
#define TRACK_ACTOR_VEL_RATE          755   

//* Special mode where the object looks at the zcoord of the next track point and moves
// to achieve the destination.  Should not be set when jumping/climbing etc.
#define TRACK_ACTOR_ZDIFF_MODE        759



//Note: All actions are preformed only if they exist for the character.

// High tag = seconds
#define TRACK_ACTOR_STAND           770
//* High tag = height value (default 384)
#define TRACK_ACTOR_JUMP            771
//* Toggle crawl state
#define TRACK_ACTOR_CRAWL           772
// Toggle swim state
#define TRACK_ACTOR_SWIM            773
// Toggle fly spell
#define TRACK_ACTOR_FLY             774
//* High tag = seconds
#define TRACK_ACTOR_SIT             776
// High tag = seconds
#define TRACK_ACTOR_DEATH1          777
//* High tag = seconds
#define TRACK_ACTOR_DEATH2          778
// Air Death!
// High tag = seconds
#define TRACK_ACTOR_DEATH_JUMP      779

//* Close range attacks - in order of least powerful to most
//* High tag = seconds
#define TRACK_ACTOR_CLOSE_ATTACK1   780
#define TRACK_ACTOR_CLOSE_ATTACK2   781
//* Long range attacks - in order of least powerful to most
//* High tag = seconds
#define TRACK_ACTOR_ATTACK1         782
#define TRACK_ACTOR_ATTACK2         783
#define TRACK_ACTOR_ATTACK3         784
#define TRACK_ACTOR_ATTACK4         785
#define TRACK_ACTOR_ATTACK5         786
#define TRACK_ACTOR_ATTACK6         787

//* High tag = seconds
#define TRACK_ACTOR_LOOK            790
//* High tag = seconds to pause
//* Point of the sprite angle in the direction of the operatable sector/wall/switch
//* Actor presses the space bar to operate sector/wall/switch. 
#define TRACK_ACTOR_OPERATE         791
// High tag = height to go up before jumping to next point
// Sprite angle must be facing the ladder
#define TRACK_ACTOR_CLIMB_LADDER    792
//* Set up a default jump value - for use before climbing ladders etc
#define TRACK_ACTOR_SET_JUMP        793

//* Specail Action - depends on each actor
//* High tag = seconds
#define TRACK_ACTOR_SPECIAL1        795
#define TRACK_ACTOR_SPECIAL2        796

//* Jump if moving forward on track
#define TRACK_ACTOR_JUMP_IF_FORWARD 797
//* Jump if moving backward on track
#define TRACK_ACTOR_JUMP_IF_REVERSE 798

//* Wait for player to come into range before moving from this point.
//* High tag = Distance from player at which NPC can start moving.
#define TRACK_ACTOR_WAIT_FOR_PLAYER 799

//* Wait for trigger to be tripped before moving from this point.
//* Use TAG_TRIGGER_ACTORS defined above is used to trigger the actor.
#define TRACK_ACTOR_WAIT_FOR_TRIGGER 800

//* Quick TAGS - Used internally by ME ONLY
#define TRACK_ACTOR_QUICK_JUMP       801
#define TRACK_ACTOR_QUICK_JUMP_DOWN  802
#define TRACK_ACTOR_QUICK_SUPER_JUMP 803
#define TRACK_ACTOR_QUICK_SCAN       804
#define TRACK_ACTOR_QUICK_EXIT       805
#define TRACK_ACTOR_QUICK_LADDER     806

//////////////////////////////////////////////////////////////////////////////////////////
//
// SPRITE TAGS
//
//////////////////////////////////////////////////////////////////////////////////////////

// 100-199 are reserved for placing actors on tracks
#define TAG_ACTOR_TRACK_BEGIN       30000
#define TAG_ACTOR_TRACK_END         30099

//* When "operated" moves grating in the direction the sprite is pointing
//* High tag = distance to move the grate - 1024 is a good distance
#define TAG_SPRITE_GRATING          200

//* Switch to rotate a SO 180 degrees when pressed.  Operates on the nearest TAG_WATER_GATE
//* as defined above.
//* High tag = Sector Object number to rotate.
#define TAG_SPRITE_ROTATE_WATER_VALVE   201
//* Same as above except operates for TAG_WATER_GATE_ALL as defined above.
#define TAG_SPRITE_ROTATE_WATER_VALVE2  202

#define TAG_SPRITE_EXPLODE_SECTOR       257
//////////////////////////////////////////////////////////////////////////////////////////
//
// LIGHTING TAGS
//
//////////////////////////////////////////////////////////////////////////////////////////

//*X Stobing effect - took this out, use TAG_LIGHT_TORCH
//#define TAG_LIGHT_STROBE        300

//* Fade effect.  Fades in and out smoothly.  
//* Ceiling is minimum darkness.
//* Floor is maximum darkness.
//* High byte is speed of fading.  The lower the number the faster.  Default is 3.
#define TAG_LIGHT_FADE          301
//* Diffuse fade light across multiple sectors (up to 10).
//* Place these types of sectors around TAG_LIGHT_FADE's.  The program will automatically
//* attach these to the TAG_LIGHT_FADE's.
//* High byte = amt of diffusion 1,2,3,4 valid values - only need to tag one of the high bytes
#define TAG_LIGHT_FADE_DIFFUSE  304

//* Torch flicker effect - random shade changes
//* Ceiling is minimum darkness.
#define TAG_LIGHT_TORCH         302
//* Same as previous except floor is not changed.
#define TAG_LIGHT_TORCH_NO_FLOOR 303

//* Programmer use
#define TAG_SPRITE_DEBUG        999

//////////////////////////////////////////////////////////////////////////////////////////
//
// HIGH TAGS
//
//////////////////////////////////////////////////////////////////////////////////////////


// Set aside 1000-1099 for high tag coordinating bits


//////////////////////////////////////////////////////////////////////////////////////////
//
// WALL TAGS
//
//////////////////////////////////////////////////////////////////////////////////////////

//* Wall tag for sliding door placed on the end of the door
#define TAG_WALL_SLIDING_DOOR 10

//* Opens the closest door - no matter the type
#define TAG_WALL_SWITCH 199

//* Sine wave wall effect - max 5 per level
//*
//* EXP - Tag the first wall with 300, last with 302.
//*       Use point2 to see which direction the wall goes.
//*
//* 1st Sector High Tag = range
//* 2nd Sector High Tag = speed
//* 3th Sector High Tag = a distance from one peak (top of curve) to the next

//* Sine Wave wall in y direction
#define TAG_WALL_SINE_Y_BEGIN 300
//* Sine Wave wall in x direction
#define TAG_WALL_SINE_X_BEGIN 301
#define TAG_WALL_SINE_Y_END 302
#define TAG_WALL_SINE_X_END 303

//* Switch to rotate a SO 90 degrees when pressed.
//* High tag = SO number to rotate.
#define TAG_ROTATE_SO_SWITCH 304

//* Climbable wall that has a top.
#define TAG_WALL_CLIMB 305
  
//* For SO's that rotate - tag ONE wall of the loop to make loop NOT rotate with the rest of the
//* SO.  Does not matter if you tag the inside or outside wall.
//* Exp - drill bit
#define TAG_WALL_LOOP_DONT_SPIN 500                                             
//* Reverse spin for this wall loop and sector.  Tag same as DONT_SPIN.
//* Exp - pit with teath
#define TAG_WALL_LOOP_REVERSE_SPIN 501                                             
//* Spin twice as fast as SO.  Tag same as DONT_SPIN.
//* Exp - whirlpool
#define TAG_WALL_LOOP_SPIN_2X 502                                             
//* Spin 4X as fast as SO.  Tag same as DONT_SPIN.
//* Exp - whirlpool
#define TAG_WALL_LOOP_SPIN_4X 503                                             
//* Tag the outer loop of a SO with this.
#define TAG_WALL_LOOP_OUTER 504                                             
//* Just tag one wall so it does not move.  My attempt to fix sector splitting for the 
//* SO outer loop.  Not working real well.
#define TAG_WALL_DONT_MOVE 505                                             

//* Just tag one wall of closed loop teleporter.  
//* High byte is speed - shift value 0-6
#define TAG_WALL_LOOP_TELEPORTER_PAN 506


#endif

/*******************************************************************************

/////////////////////////////////////////////////////////////////////////////////
//
// Skill Notes from history.txt
//
/////////////////////////////////////////////////////////////////////////////////

    SKILL & AGRESSIVNESS

        Can set skill level on the command line with -s#
        
        Press F8 on sprite to get a menu to set these values.
        
            Skill - Valid values 0-4
            
                Enemy Actors
                
                    0 Fewest Enemys
                    1
                    2 Default
                    3
                    4 Most Enemys
                    
                Ammo/Weapon Sprites
                
                    0 Most A/W
                    1
                    2 Default
                    3
                    4 Fewest A/W
                 
            Aggressiveness - Valid values 0-3

                    0 Random - Default
                    1 Least Agressive
                    2
                    3 Most Agressive
                    
/////////////////////////////////////////////////////////////////////////////////
//
// Track Notes from history.txt
//
/////////////////////////////////////////////////////////////////////////////////

QUICK TRACK (tm) FEATURES    

    Quick Tracks (tm) save Mappers time in placing certain types of tracks.
    They let you place a single sprite for a track instead of 3 or 4.  See
    ACTOR.MAP for examples.

    Quick Jump -  Place "QJ" sprite (located next to ST1) and set
    TRACK_ACTOR_QUICK_JUMP in lotag, jump height in high tag, and turn angle to
    dirction of jump.  NOTE that jump height is not required and generally
    should not be used unless the actors are not doing what they are supposed
    to.  The program will figure the jump height.

    Quick Jump Down - Place "QJD" sprite similar to "QJ".

    Quick Super Jump -  "QSJ" sprite.  Only used by creatures that can jump
    really high (panther, ripper).

    Quick Scan -  Just like the scan track only a single sprite.
        
    NO they're not REALLY TradeMarked!  I was just goof'in on ya!

********************************************************************************
Added a track type so Actors can lie in wait for a player to come within a
certain distance, then continue on the track.  Can create all sorts of "hard
wired" or "canned" situations where ninja's pop out at you when you get close.

    Enter a room and ninja's hiding behind boxes, do super leaps over (or on top
    of) boxes to attack you.
    
    Ninjas hiding in the shadows in low crawl ways.
    
    Ninjas crouching on ledge passages that jump down to attack you.
    
    Ninjas crouching on bridges.
    
    Super leap from "under" the water to attack.
    
Another track type.  Same as above except that they are set in motion by a
trigger.  This way a bunch of ninja's can act as if they are working together
and attack at once.

********************************************************************************

As I implement Actor Code and such I will sometimes define (very generally) in
LOGIC sections what is going on in the code to let you guys know what is
happening logic wise.  Read it! :)

NOTE: Look at tags.h for Track Type (TT_name) descriptions.

LOGIC: RUN WAY

    A ninja runs away when his health is low and on a random number.

    Ninja looks for track types near by in this order.
    
    1. TT_EXIT
    2. TT_LADDER
    3. TT_TRAVERSE
    4. TT_STAIRS
    5. TT_JUMP_UP
    6. TT_JUMP_DOWN
    7. TT_SCAN
    
    If he finds one of these that leads AWAY from the player, he will take it.
                                
    When a ninja gets to the end of a track he will then search again for for a
    track leading away from the player.  Theoretically you would be able to
    chase a fleeing ninja ALL THE WAY ACROSS A MAP if tracks were set up
    nicely.
    
LOGIC: FIND PLAYER

    A ninja can try to find the player via a track if 
        A) Ninja is moving toward player and is blocked by something.
        B) Ninja looses sight of player.

    If Player is above, looks for track types nearby in this order.
        1. TT_LADDER
        2. TT_STAIRS
        3. TT_JUMP_UP
        4. TT_TRAVERSE
        5. TT_SCAN
        6. TT_EXIT
    
    If Player is below, looks for track types nearby in this order.
        1. TT_JUMP_DOWN
        2. TT_TRAVERSE
        3. TT_SCAN
        4. TT_EXIT

    If Player is at same level, looks for track types nearby in this order.
        1. TT_TRAVERSE
        2. TT_EXIT
        3. TT_SCAN
        
    If he finds one of these that leads CLOSER to the player, he will take it.
    
LOGIC: WANDER

    When loosing sight of the player, a ninja can then choose to wander.
          
    Looks for track types nearby in this order.
    
        1. TT_DUCK_N_SHOOT
        2. TT_HIDE_N_SHOOT
        3. TT_WANDER
        4. TT_JUMP_DOWN
        5. TT_JUMP_UP
        6. TT_TRAVERSE
        7. TT_STAIRS
        8. TT_LADDER
        9. TT_EXIT
    
    
IMPORTANT NOTE: Currently when searching for track types, actors stop looking
after they find the first satisfactory track.  This is NOT optimal.  Should
search through all types for the best case.  This will be fixed in the future.

*******************************************************************************/

/////////////////////////////////////////////////////////////////////////////////
//
// History of changes - not kept up with religously
//
/////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************
HISTORY

10/12/94
--------

Add description of walls vs. points a top of file.

SINK tags now have hi-tags working.

Deleted swinging door tags

10/28/94
--------

Add description of Z coordinates VS pixels at the top of file.

Updated Sine Wave Floor tags.

Sector object tags had some wrong info. Re-read.

11/3/94
--------

Added the following for sine wave walls. Not documented yet.
    TAG_WALL_SINE_Y_BEGIN 300
    TAG_WALL_SINE_X_BEGIN 301
    TAG_WALL_SINE_Y_END 302
    TAG_WALL_SINE_X_END 303
    
Added TAG_ROTATE_SO_SWITCH 304.

Documented sine wave walls.

11/6/94
-------

Tagging conflicts for sinking water were occuring for things like the whirlpool
and waves.  The following explains how sector information can be extended with
sprites.  This works because a sprite KNOWS what sector it is in.  I just delete
the sprites after I get the info out of them.

    1. Sinking items such as water do not need to be tagged anymore.  Use
    sprites as follows.
    2. To set the depth place a sprite  on the sector to be sinkable and set
    its lotag to the depth you want.  Put picnum number 2307 (has the text ST1
    for a pic) on the sprite.
    3. Hitag is the type of data and the lotag is usually the actual data.
    See the chart below.
    4. I plan on using this technique lots in the future for setting extra tags.
    5. Look in actor.map for examples.
   
    ----------------------------------------------------
    |       TYPE                    |   DATA is in     |
    ----------------------------------------------------
    | name                   hitag  | lotag  | other   |
    ---------------------------------------------------- 
    SECT_SINK                  0      depth
    SECT_RESERVED              1
    SECT_DAMAGE               2      amt    
    SECT_CURRENT               3      speed    angle of sprite   
    SECT_ANGLE                16               angle of sprite
    SECT_HEIGHT               17      height   
    SECT_SPEED                18      speed
    
    NOTE 0-15 are used for true/false (is this sector sinkable - TRUE) settings
    along with data.  Others are used just for data.
    
I've placed this table at the top of the file. I will maintain it.    

11/13/94
-------

With every new uplad check new info in table at top.

NEW TAGS

#define TAG_WALL_DONT_SPIN 500            
#define TRACK_SPIN_REVERSE      720

11/13/94
-------

NEW TAGS

Changed  TAG_WALL_DONT_SPIN 500   

Changed the way panning works.  Look at chart above.

Changed hitag of 
#define TAG_STAIRS_DOWN_SWITCH  241
#define TAG_STAIRS_DOWN_START   242

Added
#define TAG_WALL_LOOP_REVERSE_SPIN 501                                             
#define TAG_WALL_LOOP_SPIN_2X 502                                             
#define TAG_WALL_LOOP_SPIN_4X 503         

IMPORTANT:

Changed the way SO's are bounded.  A bounding sector is no longer needed.  I use bounding
sprites (upper left and lower right) and tag the outer SO loop.  See examples.                                     

Using bounding sectors would slow things down under certain situations and make other 
thing impossible to do (like rotating interlocking gears).

Note do not spit the outer sector if at all possible.  Call me if you need to do this.

11/13/94
--------

Sine Wave numbers have changed.

12/05/94
--------

Added the following documentation to the Actor Track Section

// For placing actor sprite on the tracks:
//      1. Put a sprite down near the point you want it to start from.
//      2. Adjust the angle to the left to make the sprite move in the "reverse" direction
//         and to the right to make the sprite move in the "forward" direction.
//      3. For the pic, place the first pic of the actor found in the editart file.

12/10/94
--------

Added some actor track tags and Track Types (TT_) and documentation.

1/3/95
--------

#define TRACK_ACTOR_WAIT_FOR_PLAYER 799
#define TRACK_ACTOR_WAIT_FOR_TRIGGER 800
#define TAG_TRIGGER_ACTORS       3

1/12/95
---------

#define TAG_WATER_GATE          254
#define TAG_WATER_GATE_ALL      255

#define TAG_SPRITE_GRATING          200
#define TAG_SPRITE_ROTATE_WATER_VALVE   201
#define TAG_SPRITE_ROTATE_WATER_VALVE2  202


    SECT_MATCH                28      number                Used to match multiple sectors
                                                            to one action.  "number" must
                                                            be non 0.                                                                

#define TAG_OPEN_DOOR_TRIGGER             1
#define TAG_DOOR_TO_TRIGGER        2


1/20/95
---------

    Updated incorrect info for sine wave documentation.

    SECT_DIVE_AREA 
    SECT_UNDERWATER
    SECT_UNDERWATER
    SECT_LOCK_DOOR                                                 

                    
// Got rid of all these due to new easier to use ST1 sprite tag SECT_LOCK_DOOR
#define TAG_DOOR_KEY          104
#define TAG_DOOR_KEY          105
#define TAG_DOOR_KEY          106
#define TAG_DOOR_KEY          108
#define TAG_DOOR_ROTATE_KEY      116 
#define TAG_DOOR_ROTATE_KEY      117 
#define TAG_DOOR_ROTATE_KEY      118 
#define TAG_DOOR_ROTATE_KEY      119 

// updated documentaion to include locking of doors
#define TAG_DOOR_UP             100
#define TAG_DOOR_ROTATE         112

// updated documentation of sine waves had some tag numbers incorrect.                   

1/30/95
---------

CHANGED
#define TAG_ACTOR_TRACK_BEGIN       30000
#define TAG_ACTOR_TRACK_END         30099
#define TAG_WALL_SWITCH 199

NEW
    SPRI_CLIMB_MARKER         30              pos/angle    Center this sprite on the ladder
                                                           and set the angle pointing out.                                


****************************************************************************************/

