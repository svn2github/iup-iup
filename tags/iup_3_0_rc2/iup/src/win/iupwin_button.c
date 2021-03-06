/** \file
 * \brief Button Control
 *
 * See Copyright Notice in "iup.h"
 */

#include <windows.h>
#include <commctrl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_button.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_image.h"

#include "iupwin_drv.h"
#include "iupwin_handle.h"
#include "iupwin_draw.h"


static int winButtonGetBorder(void)
{
  return 4;
}

void iupdrvButtonAddBorders(int *x, int *y)
{
  int border_size = winButtonGetBorder()*2;
  (*x) += border_size;
  (*y) += border_size;
}

/****************************************************************/

static int winButtonCalcAlignPosX(int horiz_alignment, int rect_width, int width, int xpad, int shift)
{
  int x;

  if (horiz_alignment == IUP_ALIGN_ARIGHT)
    x = rect_width - (width + 2*xpad);
  else if (horiz_alignment == IUP_ALIGN_ACENTER)
    x = (rect_width - (width + 2*xpad))/2;
  else  /* ALEFT */
    x = 0;

  x += xpad;

  if (shift)
    x++;

  return x;
}

static int winButtonCalcAlignPosY(int vert_alignment, int rect_height, int height, int ypad, int shift)
{
  int y;

  if (vert_alignment == IUP_ALIGN_ABOTTOM)
    y = rect_height - (height + 2*ypad);
  else if (vert_alignment == IUP_ALIGN_ATOP)
    y = 0;
  else  /* ACENTER */
    y = (rect_height - (height + 2*ypad))/2;

  y += ypad;

  if (shift)
    y++;

  return y;
}

static HBITMAP winButtonGetBitmap(Ihandle* ih, UINT itemState, int *shift)
{
  char *name;
  int make_inactive = 0;

  if (itemState & ODS_DISABLED)
  {
    name = iupAttribGet(ih, "IMINACTIVE");
    if (!name)
    {
      name = iupAttribGet(ih, "IMAGE");
      make_inactive = 1;
    }
  }
  else
  {
    name = iupAttribGet(ih, "IMPRESS");
    if (itemState & ODS_SELECTED && name)
    {
      if (shift && !iupAttribGetStr(ih, "IMPRESSBORDER")) 
        *shift = 0;
    }
    else
      name = iupAttribGet(ih, "IMAGE");
  }

  return iupImageGetImage(name, ih, make_inactive);
}

static void winButtonDrawImageText(Ihandle* ih, HDC hDC, int rect_width, int rect_height, int border, UINT itemState)
{
  int xpad = ih->data->horiz_padding + border, 
      ypad = ih->data->vert_padding + border;
  int x, y, width, height, 
      txt_x, txt_y, txt_width, txt_height, 
      img_x, img_y, img_width, img_height, 
      bpp, shift = 0;
  HFONT hFont = (HFONT)iupwinGetHFontAttrib(ih);
  HBITMAP hBitmap;
  COLORREF fgcolor;

  char* title = iupdrvBaseGetTitleAttrib(ih);
  char* str = iupStrProcessMnemonic(title, NULL, 0);   /* remove & */
  iupdrvFontGetMultiLineStringSize(ih, str, &txt_width, &txt_height);
  if (str && str!=title) free(str);

  if (itemState & ODS_DISABLED)
    fgcolor = GetSysColor(COLOR_GRAYTEXT);
  else
    fgcolor = ih->data->fgcolor;

  hBitmap = winButtonGetBitmap(ih, itemState, NULL);
  if (!hBitmap)
    return;

  /* must use this info, since image can be a driver image loaded from resources */
  iupdrvImageGetInfo(hBitmap, &img_width, &img_height, &bpp);

  if (ih->data->img_position == IUP_IMGPOS_RIGHT ||
      ih->data->img_position == IUP_IMGPOS_LEFT)
  {
    width  = img_width + txt_width + ih->data->spacing;
    height = iupMAX(img_height, txt_height);
  }
  else
  {
    width = iupMAX(img_width, txt_width);
    height = img_height + txt_height + ih->data->spacing;
  }

  if (itemState & ODS_SELECTED && !iupwin_comctl32ver6)
    shift = 1;

  x = winButtonCalcAlignPosX(ih->data->horiz_alignment, rect_width, width, xpad, shift);
  y = winButtonCalcAlignPosY(ih->data->vert_alignment, rect_height, height, ypad, shift);

  switch(ih->data->img_position)
  {
  case IUP_IMGPOS_TOP:
    img_y = y;
    txt_y = y + img_height + ih->data->spacing;
    if (img_width > txt_width)
    {
      img_x = x;
      txt_x = x + (img_width-txt_width)/2;
    }
    else
    {
      img_x = x + (txt_width-img_width)/2;
      txt_x = x;
    }
    break;
  case IUP_IMGPOS_BOTTOM:
    img_y = y + txt_height + ih->data->spacing;
    txt_y = y;
    if (img_width > txt_width)
    {
      img_x = x;
      txt_x = x + (img_width-txt_width)/2;
    }
    else
    {
      img_x = x + (txt_width-img_width)/2;
      txt_x = x;
    }
    break;
  case IUP_IMGPOS_RIGHT:
    img_x = x + txt_width + ih->data->spacing;
    txt_x = x;
    if (img_height > txt_height)
    {
      img_y = y;
      txt_y = y + (img_height-txt_height)/2;
    }
    else
    {
      img_y = y + (txt_height-img_height)/2;
      txt_y = y;
    }
    break;
  default: /* IUP_IMGPOS_LEFT */
    img_x = x;
    txt_x = x + img_width + ih->data->spacing;
    if (img_height > txt_height)
    {
      img_y = y;
      txt_y = y + (img_height-txt_height)/2;
    }
    else
    {
      img_y = y + (txt_height-img_height)/2;
      txt_y = y;
    }
    break;
  }

  iupwinDrawBitmap(hDC, hBitmap, img_x, img_y, img_width, img_height, bpp);
  iupwinDrawText(hDC, title, txt_x, txt_y, txt_width, txt_height, hFont, fgcolor, 0);
}

static void winButtonDrawImage(Ihandle* ih, HDC hDC, int rect_width, int rect_height, int border, UINT itemState)
{
  int xpad = ih->data->horiz_padding + border, 
      ypad = ih->data->vert_padding + border;
  int x, y, width, height, bpp, shift = 0;
  HBITMAP hBitmap;

  if (itemState & ODS_SELECTED && !iupwin_comctl32ver6)
    shift = 1;

  hBitmap = winButtonGetBitmap(ih, itemState, &shift);
  if (!hBitmap)
    return;

  /* must use this info, since image can be a driver image loaded from resources */
  iupdrvImageGetInfo(hBitmap, &width, &height, &bpp);

  x = winButtonCalcAlignPosX(ih->data->horiz_alignment, rect_width, width, xpad, shift);
  y = winButtonCalcAlignPosY(ih->data->vert_alignment, rect_height, height, ypad, shift);

  iupwinDrawBitmap(hDC, hBitmap, x, y, width, height, bpp);
}

static void winButtonDrawText(Ihandle* ih, HDC hDC, int rect_width, int rect_height, int border, UINT itemState)
{
  int xpad = ih->data->horiz_padding + border, 
      ypad = ih->data->vert_padding + border;
  int x, y, width, height, shift = 0;
  COLORREF fgcolor;

  char* title = iupdrvBaseGetTitleAttrib(ih);
  if (title)
  {
    HFONT hFont = (HFONT)iupwinGetHFontAttrib(ih);
    char* str = iupStrProcessMnemonic(title, NULL, 0);   /* remove & */
    iupdrvFontGetMultiLineStringSize(ih, str, &width, &height);
    if (str && str!=title) free(str);

    if (itemState & ODS_DISABLED)
      fgcolor = GetSysColor(COLOR_GRAYTEXT);
    else
      fgcolor = ih->data->fgcolor;

    if (itemState & ODS_SELECTED && !iupwin_comctl32ver6)
      shift = 1;

    x = winButtonCalcAlignPosX(ih->data->horiz_alignment, rect_width, width, xpad, shift);
    y = winButtonCalcAlignPosY(ih->data->vert_alignment, rect_height, height, ypad, shift);

    iupwinDrawText(hDC, title, x, y, width, height, hFont, fgcolor, 0);
  }
  else
  {
    /* fill with the background color if defined at the element */
    char* bgcolor = iupAttribGet(ih, "BGCOLOR");
    if (bgcolor)
    {
      RECT rect;
      unsigned char r=0, g=0, b=0;
      iupStrToRGB(bgcolor, &r, &g, &b);
      SetDCBrushColor(hDC, RGB(r,g,b));
      rect.left = xpad;
      rect.top = ypad;
      rect.right = rect_width - xpad;
      rect.bottom = rect_height - ypad;
      FillRect(hDC, &rect, (HBRUSH)GetStockObject(DC_BRUSH));
    }
  }
}

static void winButtonDrawItem(Ihandle* ih, DRAWITEMSTRUCT *drawitem)
{ 
  iupwinBitmapDC bmpDC;
  int border, draw_border;
  int width = drawitem->rcItem.right - drawitem->rcItem.left;
  int height = drawitem->rcItem.bottom - drawitem->rcItem.top;

  HDC hDC = iupwinDrawCreateBitmapDC(&bmpDC, drawitem->hDC, width, height);

  iupwinDrawParentBackground(ih, hDC, &drawitem->rcItem);

  if ((drawitem->itemState & ODS_FOCUS) && !(drawitem->itemState & ODS_HOTLIGHT))
    drawitem->itemState |= ODS_DEFAULT;

  border = winButtonGetBorder();

  if (ih->data->type & IUP_BUTTON_IMAGE && iupAttribGet(ih, "IMPRESS") && !iupAttribGetStr(ih, "IMPRESSBORDER"))
    draw_border = 0;
  else
  {
    if (iupStrBoolean(iupAttribGetStr(ih, "FLAT")))
    {
      if (drawitem->itemState & ODS_HOTLIGHT || iupAttribGet(ih, "_IUPWINBUT_ENTERWIN"))
        draw_border = 1;
      else
        draw_border = 0;
    }
    else
      draw_border = 1;
  }

  if (draw_border)
    iupwinDrawButtonBorder(ih->handle, hDC, &drawitem->rcItem, drawitem->itemState);

  if (ih->data->type == IUP_BUTTON_IMAGE)
    winButtonDrawImage(ih, hDC, width, height, border, drawitem->itemState);
  else if (ih->data->type == IUP_BUTTON_TEXT)
    winButtonDrawText(ih, hDC, width, height, border, drawitem->itemState);
  else  /* both */
    winButtonDrawImageText(ih, hDC, width, height, border, drawitem->itemState);

  if (drawitem->itemState & ODS_FOCUS)
  {
    border--;
    iupdrvDrawFocusRect(ih, hDC, border, border, width-2*border, height-2*border);
  }

  iupwinDrawDestroyBitmapDC(&bmpDC);
}


/***********************************************************************************************/


static int winButtonSetImageAttrib(Ihandle* ih, const char* value)
{
  (void)value;
  if (ih->data->type != IUP_BUTTON_TEXT)
  {
    iupdrvDisplayUpdate(ih);
    return 1;
  }
  else
    return 0;
}

static int winButtonSetImInactiveAttrib(Ihandle* ih, const char* value)
{
  (void)value;
  if (ih->data->type != IUP_BUTTON_TEXT)
  {
    iupdrvDisplayUpdate(ih);
    return 1;
  }
  else
    return 0;
}

static int winButtonSetImPressAttrib(Ihandle* ih, const char* value)
{
  (void)value;
  if (ih->data->type != IUP_BUTTON_TEXT)
  {
    iupdrvDisplayUpdate(ih);
    return 1;
  }
  else
    return 0;
}

static int winButtonSetActiveAttrib(Ihandle* ih, const char* value)
{
  /* redraw IMINACTIVE image if any */
  if (ih->data->type != IUP_BUTTON_TEXT)
    iupdrvDisplayUpdate(ih);

  return iupBaseSetActiveAttrib(ih, value);
}

static int winButtonSetAlignmentAttrib(Ihandle* ih, const char* value)
{
  char value1[30]="", value2[30]="";

  iupStrToStrStr(value, value1, value2, ':');

  if (iupStrEqualNoCase(value1, "ARIGHT"))
    ih->data->horiz_alignment = IUP_ALIGN_ARIGHT;
  else if (iupStrEqualNoCase(value1, "ALEFT"))
    ih->data->horiz_alignment = IUP_ALIGN_ALEFT;
  else /* "ACENTER" */
    ih->data->horiz_alignment = IUP_ALIGN_ACENTER;

  if (iupStrEqualNoCase(value2, "ABOTTOM"))
    ih->data->vert_alignment = IUP_ALIGN_ABOTTOM;
  else if (iupStrEqualNoCase(value2, "ATOP"))
    ih->data->vert_alignment = IUP_ALIGN_ATOP;
  else /* "ACENTER" */
    ih->data->vert_alignment = IUP_ALIGN_ACENTER;

  iupdrvDisplayRedraw(ih);

  return 1;
}

static char* winButtonGetAlignmentAttrib(Ihandle *ih)
{
  char* horiz_align2str[3] = {"ALEFT", "ACENTER", "ARIGHT"};
  char* vert_align2str[3] = {"ATOP", "ACENTER", "ABOTTOM"};
  char *str = iupStrGetMemory(50);
  sprintf(str, "%s:%s", horiz_align2str[ih->data->horiz_alignment], vert_align2str[ih->data->vert_alignment]);
  return str;
}

static int winButtonSetPaddingAttrib(Ihandle* ih, const char* value)
{
  iupStrToIntInt(value, &ih->data->horiz_padding, &ih->data->vert_padding, 'x');
  if (ih->handle)
    iupdrvDisplayRedraw(ih);
  return 0;
}

static int winButtonSetBgColorAttrib(Ihandle* ih, const char* value)
{
  /* update internal image cache for controls that have the IMAGE attribute */
  if (ih->data->type != IUP_BUTTON_TEXT)
  {
    iupAttribSetStr(ih, "BGCOLOR", value);
    iupImageUpdateParent(ih);
    iupdrvDisplayRedraw(ih);
  }
  return 1;
}

static char* winButtonGetBgColorAttrib(Ihandle* ih)
{
  /* the most important use of this is to provide
     the correct background for images */
  if (iupwin_comctl32ver6 && !iupAttribGet(ih, "IMPRESS"))
  {
    COLORREF cr;
    if (iupwinDrawGetThemeButtonBgColor(ih->handle, &cr))
    {
      char* str = iupStrGetMemory(20);
      sprintf(str, "%d %d %d", (int)GetRValue(cr), (int)GetGValue(cr), (int)GetBValue(cr));
      return str;
    }
  }

  if (iupAttribGet(ih, "IMPRESS"))
    return iupBaseNativeParentGetBgColorAttrib(ih);
  else
    return NULL;
//    return IupGetGlobal("DLGBGCOLOR");
}

static int winButtonSetFgColorAttrib(Ihandle* ih, const char* value)
{
  unsigned char r, g, b;
  if (iupStrToRGB(value, &r, &g, &b))
  {
    ih->data->fgcolor = RGB(r,g,b);
    iupdrvDisplayRedraw(ih);
  }
  return 1;
}

/****************************************************************************************/

static int winButtonProc(Ihandle* ih, UINT msg, WPARAM wp, LPARAM lp, LRESULT *result)
{
  if (ih->data->type != IUP_BUTTON_TEXT)
  {
    /* redraw IMPRESS image if any */
    if ((msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) && iupAttribGet(ih, "IMPRESS"))
      iupdrvDisplayRedraw(ih);
  }

  switch (msg)
  {
  case WM_XBUTTONDBLCLK:
  case WM_LBUTTONDBLCLK:
  case WM_MBUTTONDBLCLK:
  case WM_RBUTTONDBLCLK:
  case WM_XBUTTONDOWN:
  case WM_LBUTTONDOWN:
  case WM_MBUTTONDOWN:
  case WM_RBUTTONDOWN:
    {
      iupwinButtonDown(ih, msg, wp, lp);
      break;
    }
  case WM_XBUTTONUP:
  case WM_LBUTTONUP:
  case WM_MBUTTONUP:
  case WM_RBUTTONUP:
    {
      iupwinButtonUp(ih, msg, wp, lp);
      break;
    }
  case WM_MOUSELEAVE:
    if (!iupwin_comctl32ver6)
    {
      iupAttribSetStr(ih, "_IUPWINBUT_ENTERWIN", NULL);
      iupdrvDisplayRedraw(ih);
    }
    break;
  case WM_MOUSEMOVE:
    if (!iupwin_comctl32ver6)
    {
      if (!iupAttribGet(ih, "_IUPWINBUT_ENTERWIN"))
      {
        iupAttribSetStr(ih, "_IUPWINBUT_ENTERWIN", "1");
        iupdrvDisplayRedraw(ih);
      }
    }
    break;
  case WM_SETFOCUS:
    if (!iupAttribGetInt(ih, "FOCUSONCLICK") && wp && iupAttribGet(ih, "_IUPWIN_ENTERWIN"))
    {
      Ihandle* previous = iupwinHandleGet((void*)wp);
      if (previous)
      {
        SetFocus(previous->handle);
        *result = 0;
        return 1;
      }
    }
    break;
  }

  return iupwinBaseProc(ih, msg, wp, lp, result);
}

static int winButtonWmNotify(Ihandle* ih, NMHDR* msg_info, int *result)
{
  if (msg_info->code == NM_CUSTOMDRAW)
  {
    NMCUSTOMDRAW *customdraw = (NMCUSTOMDRAW*)msg_info;

    if (customdraw->dwDrawStage==CDDS_PREERASE)
    {
      DRAWITEMSTRUCT drawitem;
      drawitem.itemState = 0;

      if (customdraw->uItemState & CDIS_DISABLED)
        drawitem.itemState |= ODS_DISABLED;
      else if (customdraw->uItemState & CDIS_SELECTED)
        drawitem.itemState |= ODS_SELECTED;
      else if (customdraw->uItemState & CDIS_HOT)
        drawitem.itemState |= ODS_HOTLIGHT;
      else if (customdraw->uItemState & CDIS_DEFAULT)
        drawitem.itemState |= ODS_DEFAULT;

      if (customdraw->uItemState & CDIS_FOCUS)
        drawitem.itemState |= ODS_FOCUS;

      drawitem.hDC = customdraw->hdc;
      drawitem.rcItem = customdraw->rc;

      winButtonDrawItem(ih, (void*)&drawitem);  /* Simulate a WM_DRAWITEM */

      *result = CDRF_SKIPDEFAULT;
      return 1;
    }
  }

  return 0; /* result not used */
}

static int winButtonWmCommand(Ihandle* ih, WPARAM wp, LPARAM lp)
{
  (void)lp;

  switch (HIWORD(wp))
  {
  case BN_DOUBLECLICKED:
  case BN_CLICKED:
    {
      Icallback cb = IupGetCallback(ih, "ACTION");
      if (cb && cb(ih) == IUP_CLOSE)
        IupExitLoop();
    }
  }

  return 0; /* not used */
}

static int winButtonMapMethod(Ihandle* ih)
{
  char* value;
  DWORD dwStyle = WS_CHILD | 
                  BS_NOTIFY; /* necessary because of the base messages */

  if (!ih->parent)
    return IUP_ERROR;

 /* Buttons with the BS_PUSHBUTTON style do NOT use the returned brush in WM_CTLCOLORBTN. 
    Buttons with these styles are always drawn with the default system colors.
    So FGCOLOR and BGCOLOR do NOT work.
    The BS_FLAT style does NOT completely remove the borders. With XP styles is ignored. So FLAT do NOT work.
    BCM_SETTEXTMARGIN is not working. 
    Buttons with images and with XP styles do NOT draw the focus feedback.
    Can NOT remove the borders when using IMPRESS.
    So IUP will draw its own button,
    but uses the Windows functions to draw text and images in native format. */
  if (iupwin_comctl32ver6)
    dwStyle |= BS_PUSHBUTTON; /* it will be an ownerdraw because we use NM_CUSTOMDRAW */
  else
    dwStyle |= BS_OWNERDRAW;

  value = iupAttribGet(ih, "IMAGE");
  if (value)
  {
    ih->data->type = IUP_BUTTON_IMAGE;

    value = iupAttribGet(ih, "TITLE");
    if (value)
      ih->data->type |= IUP_BUTTON_TEXT;
  }
  else
    ih->data->type = IUP_BUTTON_TEXT;

  if (iupStrBoolean(iupAttribGetStr(ih, "CANFOCUS")))
    dwStyle |= WS_TABSTOP;

  if (!iupwinCreateWindowEx(ih, "BUTTON", 0, dwStyle))
    return IUP_ERROR;

  /* Process WM_COMMAND */
  IupSetCallback(ih, "_IUPWIN_COMMAND_CB", (Icallback)winButtonWmCommand);

  /* Process BUTTON_CB */
  IupSetCallback(ih, "_IUPWIN_CTRLPROC_CB", (Icallback)winButtonProc);

  if (iupwin_comctl32ver6)
    IupSetCallback(ih, "_IUPWIN_NOTIFY_CB", (Icallback)winButtonWmNotify);  /* Process WM_NOTIFY */
  else
    IupSetCallback(ih, "_IUPWIN_DRAWITEM_CB", (Icallback)winButtonDrawItem);  /* Process WM_DRAWITEM */

  return IUP_NOERROR;
}

void iupdrvButtonInitClass(Iclass* ic)
{
  /* Driver Dependent Class functions */
  ic->Map = winButtonMapMethod;

  /* Driver Dependent Attribute functions */

  /* Overwrite Visual */
  iupClassRegisterAttribute(ic, "ACTIVE", iupBaseGetActiveAttrib, winButtonSetActiveAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);

  /* Visual */
  iupClassRegisterAttribute(ic, "BGCOLOR", winButtonGetBgColorAttrib, winButtonSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGBGCOLOR", IUPAF_DEFAULT);

  /* Special */
  iupClassRegisterAttribute(ic, "FGCOLOR", NULL, winButtonSetFgColorAttrib, "DLGFGCOLOR", NULL, IUPAF_NOT_MAPPED);  /* force the new default value */  
  iupClassRegisterAttribute(ic, "TITLE", iupdrvBaseGetTitleAttrib, iupdrvBaseSetTitleAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  /* IupButton only */
  iupClassRegisterAttribute(ic, "ALIGNMENT", winButtonGetAlignmentAttrib, winButtonSetAlignmentAttrib, "ACENTER:ACENTER", NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMAGE", NULL, winButtonSetImageAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMINACTIVE", NULL, winButtonSetImInactiveAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMPRESS", NULL, winButtonSetImPressAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "FOCUSONCLICK", NULL, NULL, "YES", NULL, IUPAF_NO_INHERIT);

  iupClassRegisterAttribute(ic, "PADDING", iupButtonGetPaddingAttrib, winButtonSetPaddingAttrib, IUPAF_SAMEASSYSTEM, "0x0", IUPAF_NOT_MAPPED);
}
