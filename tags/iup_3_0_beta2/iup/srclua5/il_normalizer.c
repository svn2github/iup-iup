/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int Normalizer(lua_State *L)
{
  Ihandle *ih = IupNormalizer(NULL);
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupnormalizerlua_open(lua_State * L)
{
  iuplua_register(L, Normalizer, "Normalizer");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/normalizer_be64.loh"
#else
#include "loh/normalizer_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/normalizer_le64w.loh"
#else
#include "loh/normalizer_le64.loh"
#endif
#else
#include "loh/normalizer.loh"
#endif
#endif
#else
  iuplua_dofile(L, "normalizer.lua");
#endif

  return 0;
}

