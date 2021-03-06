PROJNAME = iup
LIBNAME  = iuplua_pplot3
OPT = YES

USE_LUA  = Yes
USE_CDLUA = Yes

LOHDIR = loh
SRCLUA =  luapplot.lua
SRC    =  luapplot.c

DEFINES = IUPLUA_USELOH
INCLUDES = ../include
LDIR = ../lib/$(TEC_UNAME)  
LIBS = iup iuplua3 iup_pplot iupcd

ifeq ($(findstring Win, $(TEC_SYSNAME)), )
  USE_MOTIF = Yes
endif
