/** \file
 * \brief Scintilla control: Overtype
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#undef SCI_NAMESPACE
#include <Scintilla.h>

#include "iup.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"

#include "iupsci.h"


/***** OVERTYPE *****/

static char* iScintillaGetOvertypeAttrib(Ihandle *ih)
{
  return iupStrReturnBoolean(iupScintillaSendMessage(ih, SCI_GETOVERTYPE, 0, 0));
}

static int iScintillaSetOvertypeAttrib(Ihandle *ih, const char *value)
{
  if (iupStrBoolean(value))
    iupScintillaSendMessage(ih, SCI_SETOVERTYPE, 1, 0);
  else
    iupScintillaSendMessage(ih, SCI_SETOVERTYPE, 0, 0);

  return 0;
}

void iupScintillaRegisterOvertype(Iclass* ic)
{
  iupClassRegisterAttribute(ic, "OVERWRITE", iScintillaGetOvertypeAttrib, iScintillaSetOvertypeAttrib, NULL, NULL, IUPAF_NO_INHERIT);
}
