###########################################################################
#
#
#############################################################################

#version = production
version = debug
compiler_type = 386
#compiler_type =

!ifeq compiler_type 386
stacksize = 30k
!else
stacksize = 20k
!endif

model = l

# Jim's stuff
!include jimstuff.mif

.OPTIMIZE
#.SILENT

makefile          = makefile
prg_dir           = .
obj_dir           = .\obj

final_dir         = .\
sw_dir            = .
lib_dir           = $(sw_dir)

header_dir        = $(prg_dir);$(sw_dir);$(sw_dir)\setup\mact;$(sw_dir)\setup
tasm_include_dir  = /i$(prg_dir) /i$(sw_dir)

input_file        = $[*
output_dir        = $(obj_dir)\$^&

.h   :              $(header_dir)
.c   :              $(header_dir)
.asm :              $(header_dir)
.obj :              $(obj_dir)
.lib :              $(lib_dir)

#############################################################################
#
# Production options
#
#############################################################################

#tasm_production     = /t /mx /zi /p /w2
#wcc386_production   = /zq /wx /d1 /oneatx /zp4 /5r /fpi87 /fp3
#wcc_production      = /m$(model) /5 /zq /wx /d1 /oneatx /zp4 /fpi87 /fp3
#wlink_production    =

tasm_production     = /t /mx /zi /p /w2
wcc386_production   = /w4 /5r /omarxnet
wlink_production    =

#############################################################################
#
# Debugging options
#
#############################################################################

tasm_debug          = /t /mx /zi /p /w2
#wcc386_debug       = /zq /wx /d2
#wcc386_debug        = /w4 /5r /d2 /of+
wcc386_debug        = /w4 /5r /d2
wlink_debug         =


#############################################################################
#
# Compiler setup
#
#############################################################################

asm_pre_options     = $(tasm_$(version)) /i$(tasm_include_dir)
asm_post_options    = $(output_dir)

cc_pre_options      = $(wcc$(compiler_type)_$(version)) /i=$(header_dir)
cc_post_options     = /fo=$(output_dir)

link_options        = $(wlink_$(version))

asm                 = tasm
cc                  = wcc$(compiler_type)
link                = wlink
lib                 = wlib

.asm.obj :
   $(asm) $(asm_pre_options) $(input_file) $(asm_post_options)

.c.obj : .AUTODEPEND
   $(cc) $(cc_pre_options) $(input_file) $(cc_post_options)


#############################################################################
#
# Library files
#
#############################################################################

!ifeq compiler_type 386
lib_files = $(lib_dir)\setup\mact\mact$(compiler_type).lib $(lib_dir)\audio_wf.lib .\mca3r.lib .\mca3s.lib
#lib_files = $(lib_dir)\setup\mact\mact$(compiler_type).lib $(lib_dir)\audio_wf.lib  
!else
lib_files = $(lib_dir)\mact$(compiler_type).lib
!endif


#############################################################################
#
# Make setup
#
#############################################################################

sw_object_files = mdastr.obj &
       game.obj &
       save.obj &
       panel.obj &
       weapon.obj &
       player.obj &
       cheats.obj &
       config.obj &
       sounds.obj &
       setup.obj &
	   skull.obj &
	   kbd.obj &
	       draw.obj &
	   skel.obj &
	   serp.obj &
	       goro.obj &
	       ripper.obj &
	       ninja.obj &
	       coolie.obj &
	       coolg.obj &
	       hornet.obj &
	       sector.obj &
	       sprite.obj &
	       ai.obj &
	       usrhooks.obj &
	       net.obj &
	   lava.obj &
track.obj &
		   timer.obj &
		   colormap.obj &
	       cache1d.obj &
		   mmulti.obj &
	       a.obj &
		   engine.obj &
	   actor.obj &
		   $(jim_object_files)
	
$(final_dir)sw.exe : sw.exe
#   move sw.exe $(final_dir)sw.exe

sw.exe : $(sw_object_files) $(lib_files) $(makefile)
   @set exe_name=$^&
   @%make exelink

#############################################################################
#
# Object file dependencies
#
#############################################################################


#############################################################################
#
# Link the EXE
#
#############################################################################

exelink : .SYMBOLIC
   @%create temp.lnk
   @%append temp.lnk option verbose
   @%append temp.lnk option quiet
   @%append temp.lnk option map
   @%append temp.lnk option stack=$(stacksize)
#   @%append temp.lnk option CASEEXACT
   @%append temp.lnk name $(%exe_name)
   @%append temp.lnk debug all
   @for %i in ($(lib_files)) do @%append temp.lnk library %i
   for %i in ($($(%exe_name)_object_files)) do @%append temp.lnk file $(obj_dir)\%i
   $(link) $(link_options) @temp.lnk
   @del temp.lnk


