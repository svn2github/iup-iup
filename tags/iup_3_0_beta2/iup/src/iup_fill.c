/** \file
 * \brief Fill Control.
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>
#include <stdlib.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_stdcontrols.h"
#include "iup_layout.h"


enum {IUP_FILL_NONE, IUP_FILL_HORIZ, IUP_FILL_VERT};

struct _IcontrolData 
{
  int dir;
};

static int iFillGetDir(Ihandle* ih)
{
  if (!ih->parent)
    return IUP_FILL_NONE;

  if (ih->data->dir != IUP_FILL_NONE)
    return ih->data->dir;

  /* Its parent must be an IupHbox or an IupVbox. */
  if (ih->parent->iclass->nativetype == IUP_TYPEVOID)
  {
    if (iupStrEqual(ih->parent->iclass->name, "hbox"))
      ih->data->dir = IUP_FILL_HORIZ;
    else if (iupStrEqual(ih->parent->iclass->name, "vbox"))
      ih->data->dir = IUP_FILL_VERT;
  }

  return ih->data->dir;
}

static int iFillMapMethod(Ihandle* ih)
{
  ih->handle = (InativeHandle*)-1; /* fake value just to indicate that it is already mapped */
  iFillGetDir(ih);
  return IUP_NOERROR;
}

static void iFillUnMapMethod(Ihandle* ih)
{
  ih->data->dir = IUP_FILL_NONE;
}

static int iFillSetRasterSizeAttrib(Ihandle* ih, const char* value)
{
  if (!value)
  {
    ih->userwidth = 0;
    ih->userheight = 0;
  }
  else
  {
    int s = 0;
    if (iFillGetDir(ih) == IUP_FILL_NONE)  /* if Fill is not yet a child of a Vbox or Hbox */
    {
      iupAttribSetStr(ih, "SIZE", NULL);
      return 1;
    }

    iupStrToInt(value, &s);
    if (s > 0) 
    {
      if (iFillGetDir(ih) == IUP_FILL_HORIZ)
      {
        ih->userwidth = s;  /* inside HBOX */
        ih->userheight = 0;
      }
      else
      {
        ih->userheight = s; /* inside VBOX */
        ih->userwidth = 0;
      }
    }
  }
  iupAttribSetStr(ih, "SIZE", NULL);
  return 0;
}

static int iFillSetSizeAttrib(Ihandle* ih, const char* value)
{
  if (!value)
  {
    ih->userwidth = 0;
    ih->userheight = 0;
  }
  else
  {
    int s = 0;
    if (iFillGetDir(ih) == IUP_FILL_NONE) /* if Fill is not yet a child of a Vbox or Hbox */
    {
      iupAttribSetStr(ih, "RASTERSIZE", NULL);
      return 1;
    }

    iupStrToInt(value, &s);
    if (s > 0) 
    {
      int charwidth, charheight;
      iupdrvFontGetCharSize(ih, &charwidth, &charheight);
      if (iFillGetDir(ih) == IUP_FILL_HORIZ)
      {
        ih->userwidth = iupWIDTH2RASTER(s, charwidth);  /* inside HBOX */
        ih->userheight = 0;
      }
      else
      {
        ih->userheight = iupHEIGHT2RASTER(s, charheight); /* inside VBOX */
        ih->userwidth = 0;
      }
    }
  }
  iupAttribSetStr(ih, "RASTERSIZE", NULL);
  return 1;
}

static char* iFillGetExpandAttrib(Ihandle* ih)
{
  if (iFillGetDir(ih) == IUP_FILL_NONE) /* if Fill is not yet a child of a Vbox or Hbox */
    return "NO";

  /* if size is not defined, then expansion on that direction is permited */
  if (iFillGetDir(ih) == IUP_FILL_HORIZ)
  {
    if (ih->userwidth <= 0)
      return "HORIZONTAL";
  }
  else
  {
    if (ih->userheight <= 0)
      return "VERTICAL";
  }

  return "NO";
}

static void iFillUpdateSize(Ihandle* ih)
{
  char* value = iupAttribGetStr(ih, "SIZE");
  if (value) 
  { 
    iFillSetSizeAttrib(ih, value);
    iupAttribSetStr(ih, "SIZE", NULL);
  }
  value = iupAttribGetStr(ih, "RASTERSIZE");
  if (value) 
  { 
    iFillSetRasterSizeAttrib(ih, value);
    iupAttribSetStr(ih, "RASTERSIZE", NULL);
  }
}

static void iFillComputeNaturalSizeMethod(Ihandle* ih)
{
  iFillUpdateSize(ih);

  /* EXPAND is ignored for FILL */
  ih->expand = IUP_EXPAND_NONE;

  /* always initialize the natural size using the user size */
  ih->naturalwidth = ih->userwidth;
  ih->naturalheight = ih->userheight;

  if (iFillGetDir(ih) == IUP_FILL_NONE) /* if Fill is not a child of a Vbox or Hbox */
    return;

  /* if size is not defined, then expansion on that direction is permited */

  if (iFillGetDir(ih) == IUP_FILL_HORIZ)
  {
    if (ih->naturalwidth <= 0)
      ih->expand = IUP_EXPAND_W0;
  }
  else
  {
    if (ih->naturalheight <= 0)
      ih->expand = IUP_EXPAND_H0;
  }
}

static int iFillCreateMethod(Ihandle* ih, void** params)
{
  (void)params;
  ih->data = iupALLOCCTRLDATA();
  return IUP_NOERROR;
}

/******************************************************************************/

Ihandle* IupFill(void)
{
  return IupCreate("fill");
}

Iclass* iupFillGetClass(void)
{
  Iclass* ic = iupClassNew(NULL);

  ic->name = "fill";
  ic->format = NULL;  /* no parameters */
  ic->nativetype = IUP_TYPEVOID;
  ic->childtype = IUP_CHILDNONE;
  ic->is_interactive = 0;

  /* Class functions */
  ic->Create = iFillCreateMethod;
  ic->Map = iFillMapMethod;
  ic->UnMap = iFillUnMapMethod;
  ic->ComputeNaturalSize = iFillComputeNaturalSizeMethod;

  ic->SetCurrentSize = iupBaseSetCurrentSizeMethod;
  ic->SetPosition = iupBaseSetPositionMethod;

  /* Common */
  iupBaseRegisterCommonAttrib(ic);

  /* Overwrite Common */
  iupClassRegisterAttribute(ic, "SIZE", iupBaseGetSizeAttrib, iFillSetSizeAttrib, NULL, IUP_NOT_MAPPED, IUP_NO_INHERIT);
  iupClassRegisterAttribute(ic, "RASTERSIZE", iupBaseGetRasterSizeAttrib, iFillSetRasterSizeAttrib, NULL, IUP_NOT_MAPPED, IUP_NO_INHERIT);

  /* Base */
  iupClassRegisterAttribute(ic, "EXPAND", iFillGetExpandAttrib, NULL, NULL, IUP_NOT_MAPPED, IUP_NO_INHERIT);

  return ic;
}
