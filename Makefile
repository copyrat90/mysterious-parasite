#---------------------------------------------------------------------------------------------------------------------
# TARGET is the name of the output.
# BUILD is the directory where object files & intermediate files will be placed.
# LIBBUTANO is the main directory of butano library (https://github.com/GValiente/butano).
# PYTHON is the path to the python interpreter.
# SOURCES is a list of directories containing source code.
# INCLUDES is a list of directories containing extra header files.
# DATA is a list of directories containing binary data.
# GRAPHICS is a list of directories containing files to be processed by grit.
# AUDIO is a list of directories containing files to be processed by mmutil.
# DMGAUDIO is a list of directories containing files to be processed by mod2gbt and s3m2gbt.
# ROMTITLE is a uppercase ASCII, max 12 characters text string containing the output ROM title.
# ROMCODE is a uppercase ASCII, max 4 characters text string containing the output ROM code.
# USERFLAGS is a list of additional compiler flags:
#     Pass -flto to enable link-time optimization.
#     Pass -O0 to improve debugging.
# USERASFLAGS is a list of additional assembler flags.
# USERLDFLAGS is a list of additional linker flags:
#     Pass -flto=auto -save-temps to enable parallel link-time optimization.
# USERLIBDIRS is a list of additional directories containing libraries.
#     Each libraries directory must contains include and lib subdirectories.
# USERLIBS is a list of additional libraries to link with the project.
# USERBUILD is a list of additional directories to remove when cleaning the project.
# EXTTOOL is an optional command executed before processing audio, graphics and code files.
#
# All directories are specified relative to the project directory where the makefile is found.
#---------------------------------------------------------------------------------------------------------------------
TARGET      :=  $(notdir $(CURDIR))
BUILD       :=  build
LIBBUTANO   :=  ../butano
PYTHON      :=  python
SOURCES     :=  src src/scene src/cmd src/game src/game/mob src/game/mob/ai src/game/item src/debug iso_bn/src
INCLUDES    :=  include iso_bn/include
DATA        :=
GRAPHICS    :=  graphics $(BUILD)/fonts
AUDIO       :=  audio
DMGAUDIO    :=  dmg_audio
ROMTITLE    :=  MYSTERY PARA
ROMCODE     :=  2MPE

### Debug flags ###
USERFLAGS   :=  -DMP_DEBUG -DBN_CFG_BGS_MAX_ITEMS=8 -DBN_CFG_SPRITES_MAX_ITEMS=256 -DBN_CFG_PROFILER_ENABLED=true
### Release flags ###
# USERFLAGS   :=  -DBN_CFG_BGS_MAX_ITEMS=8 -DBN_CFG_SPRITES_MAX_ITEMS=256 -DBN_CFG_LOG_ENABLED=false

USERASFLAGS := 
USERLDFLAGS :=  
USERLIBDIRS :=  
USERLIBS    :=  
USERBUILD   :=  

### gba-free-fonts font generator ###
FONTS       :=  fonts/fonts/galmuri9
TEXTS       :=  include/texts.hpp
EXTTOOL     :=  $(PYTHON) fonts/tools/butano/butano_fonts_tool.py --build=$(BUILD) --fonts="$(FONTS)" --texts="$(TEXTS)"

#---------------------------------------------------------------------------------------------------------------------
# Export absolute butano path:
#---------------------------------------------------------------------------------------------------------------------
ifndef LIBBUTANOABS
	export LIBBUTANOABS	:=	$(realpath $(LIBBUTANO))
endif

#---------------------------------------------------------------------------------------------------------------------
# Include main makefile:
#---------------------------------------------------------------------------------------------------------------------
include $(LIBBUTANOABS)/butano.mak
