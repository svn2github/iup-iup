/** \file
 * \brief IupGetParam
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "iup.h"

#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_strmessage.h"
#include "iup_drvfont.h"


#define RAD2DEG  57.296f   /* radians to degrees */


/*******************************************************************************************
                    Internal Callbacks
*******************************************************************************************/

static int iParamButtonOK_CB(Ihandle* self)
{
  Ihandle* dlg = IupGetDialog(self);
  Iparamcb cb = (Iparamcb)IupGetCallback(dlg, "PARAM_CB");
  iupAttribSetStr(dlg, "STATUS", "1");
  if (cb && !cb(dlg, -1, (void*)iupAttribGet(dlg, "USER_DATA")))
    return IUP_DEFAULT;
  else
    return IUP_CLOSE;
}

static int iParamButtonCancel_CB(Ihandle* self)
{
  Ihandle* dlg = IupGetDialog(self);
  Iparamcb cb = (Iparamcb)IupGetCallback(dlg, "PARAM_CB");
  iupAttribSetStr(dlg, "STATUS", "0");
  if (cb) cb(dlg, -3, (void*)iupAttribGet(dlg, "USER_DATA"));
  return IUP_CLOSE;
}

static int iParamToggleAction_CB(Ihandle *self, int v)
{
  Ihandle* param = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_PARAM");
  Ihandle* dlg = IupGetDialog(self);
  Iparamcb cb = (Iparamcb)IupGetCallback(dlg, "PARAM_CB");
  int old_v = iupAttribGetInt(param, "VALUE");

  if (v == 1)
    iupAttribSetStr(param, "VALUE", "1");
  else
    iupAttribSetStr(param, "VALUE", "0");

  if (cb && !cb(dlg, iupAttribGetInt(param, "INDEX"), (void*)iupAttribGet(dlg, "USER_DATA"))) 
  {
    /* Undo */
    if (old_v == 1)
    {
      iupAttribSetStr(param, "VALUE", "1");
      IupSetAttribute(self, "VALUE", "1");
    }
    else
    {
      iupAttribSetStr(param, "VALUE", "0");
      IupSetAttribute(self, "VALUE", "0");
    }

    /* there is no IUP_IGNORE for IupToggle */
    return IUP_DEFAULT;
  }

  /* update the interface */
  if (v == 1)
    IupStoreAttribute(self, "TITLE", iupAttribGet(param, "_IUPGP_TRUE"));
  else
    IupStoreAttribute(self, "TITLE", iupAttribGet(param, "_IUPGP_FALSE"));

  return IUP_DEFAULT;
}

static int iParamTextAction_CB(Ihandle *self, int c, char *after)
{
  Ihandle* param = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_PARAM");
  Ihandle* dlg = IupGetDialog(self);
  Iparamcb cb = (Iparamcb)IupGetCallback(dlg, "PARAM_CB");
  Ihandle* aux = (Ihandle*)iupAttribGet(param, "AUXCONTROL");
  (void)c;
 
  iupAttribStoreStr(param, "VALUE", after);

  if (cb && !cb(dlg, iupAttribGetInt(param, "INDEX"), (void*)iupAttribGet(dlg, "USER_DATA"))) 
  {
    /* Undo */
    iupAttribStoreStr(param, "VALUE", IupGetAttribute(self, "VALUE"));
    return IUP_IGNORE;
  }

  if (aux)
  {
    if (iupStrEqual(iupAttribGet(param, "TYPE"), "COLOR"))
      IupStoreAttribute(aux, "BGCOLOR", after);
    else
      IupStoreAttribute(aux, "VALUE", after);
  }

  if (IupGetInt(self, "SPIN"))
  {
    if (iupAttribGet(self, "_IUPGP_SPINREAL"))
    {
      float min = iupAttribGetFloat(param, "MIN");
      float step = iupAttribGetFloat(self, "_IUPGP_INCSTEP");
      float val;
      if (iupStrToFloat(after, &val))
        IupSetfAttribute(self, "SPINVALUE", "%d", (int)((val-min)/step + 0.5));
    }
    else
    {
      int val;
      if (iupStrToInt(after, &val))
        IupSetfAttribute(self, "SPINVALUE", "%d", val);
    }
  }

  return IUP_DEFAULT;
}

static int iParamValAction_CB(Ihandle *self)
{
  Ihandle* param = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_PARAM");
  Ihandle* text = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_TEXT");
  Ihandle* dlg = IupGetDialog(self);
  Iparamcb cb = (Iparamcb)IupGetCallback(dlg, "PARAM_CB");
  float old_value = iupAttribGetFloat(param, "VALUE");
  float val = IupGetFloat(self, "VALUE");

  char* type = iupAttribGet(param, "TYPE");
  if (iupStrEqual(type, "INTEGER"))
  {
    iupAttribSetStrf(param, "VALUE", "%d", (int)val);
  }
  else
  {
    if (iupAttribGetInt(param, "ANGLE"))
    {
      float old_angle;

      if (val == 0)
      {
        old_angle = iupAttribGetFloat(param, "VALUE");
        iupAttribSetStrf(param, "_IUPGP_OLD_ANGLE", "%g", old_angle);
      }
      else
        old_angle = iupAttribGetFloat(param, "_IUPGP_OLD_ANGLE");

      val = old_angle + val*RAD2DEG;

      if (iupAttribGetInt(param, "INTERVAL"))
      {
        float min = iupAttribGetFloat(param, "MIN");
        float max = iupAttribGetFloat(param, "MAX");
        if (val < min)
          val = min;
        if (val > max)
          val = max;
      }
      else if (iupAttribGetInt(param, "PARTIAL"))
      {
        float min = iupAttribGetFloat(param, "MIN");
        if (val < min)
          val = min;
      }
    }

    iupAttribSetStrf(param, "VALUE", "%g", val);
  }

  if (cb && !cb(dlg, iupAttribGetInt(param, "INDEX"), (void*)iupAttribGet(dlg, "USER_DATA"))) 
  {
    /* Undo */
    iupAttribSetStrf(param, "VALUE", "%g", old_value);

    if (!iupAttribGetInt(param, "ANGLE"))
      IupSetfAttribute(self, "VALUE", "%g", old_value);

    /* there is no IUP_IGNORE for IupVal */
    return IUP_DEFAULT;
  }

  type = iupAttribGet(param, "TYPE");
  if (iupStrEqual(type, "INTEGER"))
    IupSetfAttribute(text, "VALUE", "%d", (int)val);
  else
    IupSetfAttribute(text, "VALUE", "%g", val);

  if (IupGetInt(text, "SPIN"))
  {
    if (iupAttribGet(text, "_IUPGP_SPINREAL"))
    {
      float min = iupAttribGetFloat(param, "MIN");
      float step = iupAttribGetFloat(text, "_IUPGP_INCSTEP");
      float val = IupGetFloat(text, "VALUE");
      IupSetfAttribute(text, "SPINVALUE", "%d", (int)((val-min)/step + 0.5));
    }
    else
    {
      int val = IupGetInt(text, "VALUE");
      IupSetfAttribute(text, "SPINVALUE", "%d", val);
    }
  }

  return IUP_DEFAULT;
}

static int iParamListAction_CB(Ihandle *self, char *t, int i, int v)
{
  (void)t;
  if (v == 1)
  {
    Ihandle* param = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_PARAM");
    Ihandle* dlg = IupGetDialog(self);
    Iparamcb cb = (Iparamcb)IupGetCallback(dlg, "PARAM_CB");
    int old_i = iupAttribGetInt(param, "VALUE");

    iupAttribSetStrf(param, "VALUE", "%d", i-1);

    if (cb && !cb(dlg, iupAttribGetInt(param, "INDEX"), (void*)iupAttribGet(dlg, "USER_DATA"))) 
    {
      /* Undo */
      iupAttribSetStrf(param, "VALUE", "%d", old_i);
      IupSetfAttribute(self, "VALUE", "%d", old_i+1);

      /* there is no IUP_IGNORE for IupList */
      return IUP_DEFAULT;
    }
  }

  return IUP_DEFAULT;
}

static int iParamFileButton_CB(Ihandle *self)
{
  Ihandle* param   = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_PARAM");
  Ihandle* textbox = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_TEXT");

  Ihandle* dlg = IupFileDlg();

  IupSetAttributeHandle(dlg, "PARENTDIALOG", IupGetDialog(self));
  IupSetAttribute(dlg, "TITLE", iupAttribGet(param, "TITLE"));
  IupSetAttribute(dlg, "VALUE", iupAttribGet(param, "VALUE"));

  IupSetAttribute(dlg, "DIALOGTYPE", iupAttribGet(param, "_IUPGP_DIALOGTYPE"));
  IupSetAttribute(dlg, "FILTER", iupAttribGet(param, "_IUPGP_FILTER"));
  IupSetAttribute(dlg, "DIRECTORY", iupAttribGet(param, "_IUPGP_DIRECTORY"));
  IupSetAttribute(dlg, "NOCHANGEDIR", iupAttribGet(param, "_IUPGP_NOCHANGEDIR"));
  IupSetAttribute(dlg, "NOOVERWRITEPROMPT", iupAttribGet(param, "_IUPGP_NOOVERWRITEPROMPT"));

  IupPopup(dlg, IUP_CENTER, IUP_CENTER);

  if (IupGetInt(dlg, "STATUS") != -1)
  {
    IupSetAttribute(textbox, "VALUE", iupAttribGet(dlg, "VALUE"));
    iupAttribStoreStr(param, "VALUE", iupAttribGet(dlg, "VALUE"));
  } 

  IupDestroy(dlg);

  return IUP_DEFAULT;
}

static int iParamColorButton_CB(Ihandle *self, int button, int pressed)
{
  if (button==IUP_BUTTON1 && pressed)
  {
    Ihandle* param   = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_PARAM");
    Ihandle* textbox = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_TEXT");

    Ihandle* dlg = IupColorDlg();

    IupSetAttributeHandle(dlg, "PARENTDIALOG", IupGetDialog(self));
    IupSetAttribute(dlg, "TITLE", iupAttribGet(param, "TITLE"));
    IupSetAttribute(dlg, "VALUE", iupAttribGet(param, "VALUE"));

    IupPopup(dlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(dlg, "STATUS")==1)
    {
      char* value = IupGetAttribute(dlg, "VALUE");
      IupSetAttribute(textbox, "VALUE", value);
      iupAttribStoreStr(param, "VALUE", value);
      IupStoreAttribute(self, "BGCOLOR", value);
    } 

    IupDestroy(dlg);
  }

  return IUP_DEFAULT;
}

static int iParamSpinReal_CB(Ihandle *self, int pos)
{
  Ihandle* param = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_PARAM");
  Ihandle* dlg = IupGetDialog(self);
  Iparamcb cb = (Iparamcb)IupGetCallback(dlg, "PARAM_CB");
  Ihandle* text = (Ihandle*)iupAttribGet(param, "CONTROL");
  float min = iupAttribGetFloat(param, "MIN");
  float max = iupAttribGetFloat(param, "MAX");
  float val, step = iupAttribGetFloat(text, "_IUPGP_INCSTEP");

  /* here spin is always [0-spinmax] converted to [min-max] */
  val = (float)pos*step + min;
  if (val < min)
    val = min;
  if (val > max)
    val = max;

  iupAttribSetStrf(param, "VALUE", "%g", (double)val);

  if (cb) 
  {
    int ret;
    iupAttribSetStr(dlg, "SPINNING", "1");
    ret = cb(dlg, iupAttribGetInt(param, "INDEX"), (void*)iupAttribGet(dlg, "USER_DATA"));
    iupAttribSetStr(dlg, "SPINNING", NULL);
    if (!ret)
      return IUP_IGNORE;
  }

  IupSetfAttribute(text, "VALUE", "%g", (double)val);

  {
    Ihandle* aux = (Ihandle*)iupAttribGet(param, "AUXCONTROL");
    if (aux)
      IupSetfAttribute(aux, "VALUE", "%g", (double)val);
  }

  return IUP_DEFAULT;
}

static int iParamSpinInt_CB(Ihandle *self, int pos)
{
  Ihandle* param = (Ihandle*)iupAttribGetInherit(self, "_IUPGP_PARAM");
  Ihandle* dlg = IupGetDialog(self);
  Iparamcb cb = (Iparamcb)IupGetCallback(dlg, "PARAM_CB");
  Ihandle* text = (Ihandle*)iupAttribGet(param, "CONTROL");

  /* here spin is always [min-max] */

  iupAttribSetInt(param, "VALUE", pos);

  if (cb) 
  {
    int ret;
    iupAttribSetStr(dlg, "SPINNING", "1");
    ret = cb(dlg, iupAttribGetInt(param, "INDEX"), (void*)iupAttribGet(dlg, "USER_DATA"));
    iupAttribSetStr(dlg, "SPINNING", NULL);
    if (!ret)
      return IUP_IGNORE;
  }

  IupSetfAttribute(text, "VALUE", "%g", (double)pos);

  {
    Ihandle* aux = (Ihandle*)iupAttribGet(param, "AUXCONTROL");
    if (aux)
      IupSetfAttribute(aux, "VALUE", "%g", (double)pos);
  }

  return IUP_DEFAULT;
}

/*******************************************************************************************
                    Creates One Parameter Box
*******************************************************************************************/

static Ihandle* iParamCreateBox(Ihandle* param)
{
  Ihandle *box, *ctrl = NULL, *label;
  char *type;

  label = IupLabel(iupAttribGet(param, "TITLE"));

  type = iupAttribGet(param, "TYPE");
  if (iupStrEqual(type, "SEPARATOR"))
  {
    box = IupHbox(label, NULL);
    IupSetAttribute(box,"ALIGNMENT","ACENTER");
  }
  else
  {
    if (iupStrEqual(type, "STRING") && iupAttribGetInt(param, "MULTILINE"))
    {
      Ihandle* hbox1 = IupHbox(IupSetAttributes(IupFill(), "SIZE=5"), label, NULL);
      IupSetAttribute(hbox1,"ALIGNMENT","ACENTER");

      box = IupVbox(hbox1, NULL);
      IupSetAttribute(box,"ALIGNMENT","ALEFT");
    }
    else
    {
      box = IupHbox(IupSetAttributes(IupFill(), "SIZE=5"), label, NULL);
      IupSetAttribute(box,"ALIGNMENT","ACENTER");
    }
  }

  IupSetAttribute(box,"MARGIN","0x0");

  type = iupAttribGet(param, "TYPE");
  if (iupStrEqual(type, "BOOLEAN"))
  {
    int value = iupAttribGetInt(param, "VALUE");
    if (value)
    {
      ctrl = IupToggle(iupAttribGet(param, "_IUPGP_TRUE"), NULL);
      IupSetAttribute(ctrl, "VALUE", "ON");
    }
    else
    {
      ctrl = IupToggle(iupAttribGet(param, "_IUPGP_FALSE"), NULL);
      IupSetAttribute(ctrl, "VALUE", "OFF");
    }
    IupSetCallback(ctrl, "ACTION", (Icallback)iParamToggleAction_CB);

    IupAppend(box, ctrl);
    iupAttribSetStr(param, "DATA_TYPE", "1");
  }
  else if (iupStrEqual(type, "SEPARATOR"))
  {
    ctrl = IupLabel("");
    IupSetAttribute(ctrl, "SEPARATOR", "HORIZONTAL");

    IupAppend(box, ctrl);
    iupAttribSetStr(param, "DATA_TYPE", "-1");
  }
  else if (iupStrEqual(type, "LIST"))
  {
    char str[20] = "1";
    int i = 1;
    ctrl = IupList(NULL);
    IupSetCallback(ctrl, "ACTION", (Icallback)iParamListAction_CB);
    IupSetAttribute(ctrl, "DROPDOWN", "YES");
    IupSetfAttribute(ctrl, "VALUE", "%d", iupAttribGetInt(param, "VALUE")+1);

    while (*iupAttribGet(param, str) != 0)
    {
      IupStoreAttribute(ctrl, str, iupAttribGet(param, str));
      i++;
      sprintf(str, "%d", i);
    }
    IupStoreAttribute(ctrl, str, NULL);

    IupAppend(box, ctrl);
    iupAttribSetStr(param, "DATA_TYPE", "1");
  }
  else if (iupStrEqual(type, "STRING"))
  {
    if (iupAttribGetInt(param, "MULTILINE"))
    {
      Ihandle* hbox;

      ctrl = IupMultiLine(NULL);
      IupSetAttribute(ctrl, "SIZE", "100x50");
      IupSetAttribute(ctrl, "EXPAND", "YES");

      hbox = IupHbox(IupSetAttributes(IupFill(), "SIZE=5"), ctrl, NULL);
      IupSetAttribute(hbox,"ALIGNMENT","ACENTER");

      IupAppend(box, hbox);
    }
    else
    {
      ctrl = IupText(NULL);
      IupSetAttribute(ctrl, "SIZE", "100x");
      IupSetAttribute(ctrl, "EXPAND", "HORIZONTAL");
      IupAppend(box, ctrl);
    }
    IupSetCallback(ctrl, "ACTION", (Icallback)iParamTextAction_CB);
    IupStoreAttribute(ctrl, "VALUE", iupAttribGet(param, "VALUE"));

    {
      char* mask = iupAttribGet(param, "MASK");
      if (mask)
        IupStoreAttribute(ctrl, "MASK", mask);
    }

    iupAttribSetStr(param, "DATA_TYPE", "0");
    iupAttribSetStr(param, "EXPAND", "1");
  }
  else if (iupStrEqual(type, "FILE"))
  {
      Ihandle* aux;

      ctrl = IupText(NULL);
      IupSetAttribute(ctrl, "SIZE", "100x");
      IupSetAttribute(ctrl, "EXPAND", "HORIZONTAL");
      IupAppend(box, ctrl);

      IupSetCallback(ctrl, "ACTION", (Icallback)iParamTextAction_CB);
      IupStoreAttribute(ctrl, "VALUE", iupAttribGet(param, "VALUE"));

      iupAttribSetStr(param, "DATA_TYPE", "0");
      iupAttribSetStr(param, "EXPAND", "1");

      
      aux = IupButton("...", "");
      IupSetAttribute(aux, "EXPAND", "NO");

      IupSetCallback(aux, "ACTION", (Icallback)iParamFileButton_CB);
      iupAttribSetStr(aux, "_IUPGP_PARAM", (char*)param);
      iupAttribSetStr(aux, "_IUPGP_TEXT", (char*)ctrl);
      IupSetAttribute(aux, "EXPAND", "NO");

      IupAppend(box, aux); 
  }
  else if (iupStrEqual(type, "COLOR"))
  {
      Ihandle* aux;

      ctrl = IupText(NULL);
      IupSetAttribute(ctrl, "SIZE", "100x");
      IupSetAttribute(ctrl, "EXPAND", "HORIZONTAL");
      IupAppend(box, ctrl);

      IupSetCallback(ctrl, "ACTION", (Icallback)iParamTextAction_CB);
      IupSetAttribute(ctrl, "MASK", "(/d|/d/d|1/d/d|2(0|1|2|3|4)/d|25(0|1|2|3|4|5)) (/d|/d/d|1/d/d|2(0|1|2|3|4)/d|25(0|1|2|3|4|5)) (/d|/d/d|1/d/d|2(0|1|2|3|4)/d|25(0|1|2|3|4|5)) (/d|/d/d|1/d/d|2(0|1|2|3|4)/d|25(0|1|2|3|4|5))");
      IupStoreAttribute(ctrl, "VALUE", iupAttribGet(param, "VALUE"));

      iupAttribSetStr(param, "DATA_TYPE", "0");
      iupAttribSetStr(param, "EXPAND", "1");

      
      aux = IupCanvas(NULL);
      IupSetAttribute(aux, "SIZE", "20x10");
      IupSetAttribute(aux, "EXPAND", "NO");
      IupStoreAttribute(aux, "BGCOLOR", iupAttribGet(param, "VALUE"));

      IupSetCallback(aux, "BUTTON_CB", (Icallback)iParamColorButton_CB);
      iupAttribSetStr(param, "AUXCONTROL", (char*)aux);
      iupAttribSetStr(aux, "_IUPGP_PARAM", (char*)param);
      iupAttribSetStr(aux, "_IUPGP_TEXT", (char*)ctrl);
      IupSetAttribute(aux, "EXPAND", "NO");

      IupAppend(box, aux); 
  }
  else /* INTEGER, REAL */
  {
    ctrl = IupText(NULL);
    IupSetCallback(ctrl, "ACTION", (Icallback)iParamTextAction_CB);
    IupStoreAttribute(ctrl, "VALUE", iupAttribGet(param, "VALUE"));

    type = iupAttribGet(param, "TYPE");
    if (iupStrEqual(type, "REAL"))
    {
      if (iupAttribGetInt(param, "INTERVAL"))
      {
        float min = iupAttribGetFloat(param, "MIN");
        float max = iupAttribGetFloat(param, "MAX");
        float step = iupAttribGetFloat(param, "STEP");
        float val = iupAttribGetFloat(param, "VALUE");
        if (step == 0) step = (max-min)/20.0f;
        IupSetfAttribute(ctrl, "MASKFLOAT", "%g:%g", (double)min, (double)max);
                             
        /* here spin is always [0-spinmax] converted to [min-max] */

        IupSetAttribute(ctrl, "SPIN", "YES");   /* spin only for intervals */
        IupSetAttribute(ctrl, "SPINAUTO", "NO");
        IupAppend(box, ctrl);
        IupSetCallback(ctrl, "SPIN_CB", (Icallback)iParamSpinReal_CB);
        /* SPINMIN=0 and SPININC=1 */
        IupSetfAttribute(ctrl, "SPINMAX", "%d", (int)((max-min)/step + 0.5));
        IupSetfAttribute(ctrl, "SPINVALUE", "%d", (int)((val-min)/step + 0.5));

        iupAttribSetStrf(ctrl, "_IUPGP_INCSTEP", "%g", step);
        iupAttribSetStr(ctrl, "_IUPGP_SPINREAL", "1");
      }
      else if (iupAttribGetInt(param, "PARTIAL"))
      {
        float min = iupAttribGetFloat(param, "MIN");
        if (min == 0)
          IupSetAttribute(ctrl, "MASK", IUP_MASK_UFLOAT);
        else
          IupSetfAttribute(ctrl, "MASKFLOAT", "%g:%g", (double)min, (double)1.0e10);
        IupAppend(box, ctrl);
      }
      else
      {
        IupSetAttribute(ctrl, "MASK", IUP_MASK_FLOAT);
        IupAppend(box, ctrl);
      }

      iupAttribSetStr(param, "DATA_TYPE", "2");
      IupSetAttribute(ctrl, "SIZE", "50x");
    }
    else /* INTEGER*/
    {
      int val = iupAttribGetInt(param, "VALUE");
      IupSetAttribute(ctrl, "SPIN", "YES");   /* spin always */
      IupSetAttribute(ctrl, "SPINAUTO", "NO");  /* manually update spin so the callback can also updated it */
      IupAppend(box, ctrl);
      IupSetCallback(ctrl, "SPIN_CB", (Icallback)iParamSpinInt_CB);
      iupAttribSetStr(ctrl, "_IUPGP_INCSTEP", "1");
      IupSetfAttribute(ctrl, "SPINVALUE", "%d", val);

      /* here spin is always [min-max] */

      if (iupAttribGetInt(param, "INTERVAL"))
      {
        int min = iupAttribGetInt(param, "MIN");
        int max = iupAttribGetInt(param, "MAX");
        int step = iupAttribGetInt(param, "STEP");
        if (step)
        {
          iupAttribSetStrf(ctrl, "_IUPGP_INCSTEP", "%d", step);
          IupSetfAttribute(ctrl, "SPININC", "%d", step);
        }
        IupSetfAttribute(ctrl, "SPINMAX", "%d", max);
        IupSetfAttribute(ctrl, "SPINMIN", "%d", min);
      }
      else if (iupAttribGetInt(param, "PARTIAL"))
      {
        int min = iupAttribGetInt(param, "MIN");
        if (min == 0)
          IupSetAttribute(ctrl, "MASK", IUP_MASK_UINT);
        else
          IupSetfAttribute(ctrl, "MASKINT", "%d:2147483647", min);
        IupSetfAttribute(ctrl, "SPINMIN", "%d", min);
        IupSetAttribute(ctrl, "SPINMAX", "2147483647");
      }
      else                             
      {
        IupSetAttribute(ctrl, "SPINMIN", "-2147483647");
        IupSetAttribute(ctrl, "SPINMAX", "2147483647");
        IupSetAttribute(ctrl, "MASK", IUP_MASK_INT);
      }

      iupAttribSetStr(param, "DATA_TYPE", "1");
      IupSetAttribute(ctrl, "SIZE", "50x");
    }

    if (iupAttribGetInt(param, "INTERVAL") || iupAttribGetInt(param, "ANGLE"))
    {
      Ihandle* aux;

      if (iupAttribGetInt(param, "ANGLE"))
      {
        aux = IupCreatep("dial", "HORIZONTAL", NULL);
        if (aux)
        {
          IupSetfAttribute(aux, "VALUE", "%g", (double)(iupAttribGetFloat(param, "VALUE")/RAD2DEG));
          IupSetAttribute(aux, "SIZE", "50x10");
        }
      }
      else
      {
        char* step;
        aux = IupVal("HORIZONTAL");
        IupStoreAttribute(aux, "MIN", iupAttribGet(param, "MIN"));
        IupStoreAttribute(aux, "MAX", iupAttribGet(param, "MAX"));
        IupStoreAttribute(aux, "VALUE", iupAttribGet(param, "VALUE"));
        IupSetAttribute(aux, "EXPAND", "HORIZONTAL");
        iupAttribSetStr(param, "AUXCONTROL", (char*)aux);
        iupAttribSetStr(param, "EXPAND", "1");
        step = iupAttribGet(param, "STEP");
        if (step)
          IupSetfAttribute(aux, "STEP", "%g", iupAttribGetFloat(param, "STEP")/(iupAttribGetFloat(param, "MAX")-iupAttribGetFloat(param, "MIN")));
        else if (iupStrEqual(type, "INTEGER"))
          IupSetfAttribute(aux, "STEP", "%g", 1.0/(iupAttribGetFloat(param, "MAX")-iupAttribGetFloat(param, "MIN")));
      }

      if (aux)
      {
        IupSetCallback(aux, "VALUECHANGED_CB", (Icallback)iParamValAction_CB);
        iupAttribSetStr(aux, "_IUPGP_PARAM", (char*)param);
        iupAttribSetStr(aux, "_IUPGP_TEXT", (char*)ctrl);

        IupAppend(box, aux);
      }
    }
  }

  if (ctrl) IupStoreAttribute(ctrl, "TIP", iupAttribGet(param, "TIP"));
  iupAttribSetStr(box, "_IUPGP_PARAM", (char*)param);
  iupAttribSetStr(param, "CONTROL", (char*)ctrl);
  iupAttribSetStr(param, "LABEL", (char*)label);
  return box;
}

/*******************************************************************************************
                    Creates the Dialog and Normalize Sizes
*******************************************************************************************/

static Ihandle* IupParamDlgP(Ihandle** params)
{
  Ihandle *dlg, *button_ok, *button_cancel, 
          *dlg_box, *button_box, *param_box;
  int i, lbl_width, p, expand;

  button_ok = IupButton("OK", NULL);
  IupSetAttribute(button_ok, "PADDING", "20x0");
  IupSetCallback(button_ok, "ACTION", (Icallback)iParamButtonOK_CB);

  button_cancel = IupButton(iupStrMessageGet("IUP_CANCEL"), NULL);
  IupSetAttribute(button_cancel, "PADDING", "20x0");
  IupSetCallback(button_cancel, "ACTION", (Icallback)iParamButtonCancel_CB);
  
  param_box = IupVbox(NULL);

  i = 0; expand = 0;
  while (params[i] != NULL)
  {
    IupAppend(param_box, iParamCreateBox(params[i]));

    if (IupGetInt(params[i], "EXPAND"))
      expand = 1;

    i++;
  }

  button_box = IupHbox(
    IupFill(),
    button_ok,
    button_cancel,
    NULL);
  IupSetAttribute(button_box,"MARGIN","0x0");
  IupSetAttribute(button_box, "NORMALIZESIZE", "HORIZONTAL");

  dlg_box = IupVbox(
    IupFrame(param_box),
    button_box,
    NULL);
  IupSetAttribute(dlg_box, "MARGIN", "10x10");
  IupSetAttribute(dlg_box, "GAP", "5");

  dlg = IupDialog(dlg_box);

  IupSetAttribute(dlg, "MINBOX", "NO");
  IupSetAttribute(dlg, "MAXBOX", "NO");
  if (!expand) 
  {
    IupSetAttribute(dlg, "RESIZE", "NO");
    IupSetAttribute(dlg, "DIALOGFRAME", "YES");
    IupSetAttribute(dlg,"DIALOGHINT","YES");
  }
  IupSetAttributeHandle(dlg, "DEFAULTENTER", button_ok);
  IupSetAttributeHandle(dlg, "DEFAULTESC", button_cancel);
  IupSetAttribute(dlg, "TITLE", "ParamDlg");
  IupSetAttribute(dlg, "PARENTDIALOG", IupGetGlobal("PARENTDIALOG"));
  IupSetAttribute(dlg, "ICON", IupGetGlobal("ICON"));
  iupAttribSetStr(dlg, "OK", (char*)button_ok);
  iupAttribSetStr(dlg, "CANCEL", (char*)button_cancel);

  IupMap(dlg);

  /* get the largest label size and set INDEX */
  i = 0; lbl_width = 0, p = 0;
  while (params[i] != NULL)
  {
    int w;

    char* type = iupAttribGet(params[i], "TYPE");
    if (!iupStrEqual(type, "SEPARATOR"))
    {
      char str[20];
      sprintf(str, "PARAM%d", p);
      IupSetAttribute(dlg, str, (char*)params[i]);
      iupAttribSetStrf(params[i], "INDEX", "%d", p);
      p++;
    }

    w = IupGetInt((Ihandle*)iupAttribGet(params[i], "LABEL"), "SIZE");
    if (w > lbl_width)
      lbl_width = w;

    i++;
  }

  i = 0;
  while (params[i] != NULL)
  {
    char* type = iupAttribGet(params[i], "TYPE");
    if (!iupStrEqual(type, "SEPARATOR"))
    {
      if (iupStrEqual(type, "LIST"))
      {
        /* set a minimum size for lists */
        Ihandle* ctrl = (Ihandle*)iupAttribGet(params[i], "CONTROL");
        if (IupGetInt(ctrl, "SIZE") < 50)
          IupSetAttribute(ctrl, "SIZE", "50x");
      }
      else if (iupStrEqual(type, "BOOLEAN"))
      {
        /* reserve enough space for boolean strings */
        Ihandle* ctrl = (Ihandle*)iupAttribGet(params[i], "CONTROL");
        int wf = iupdrvFontGetStringWidth(ctrl, iupAttribGet(params[i], "_IUPGP_FALSE"));
        int wt = iupdrvFontGetStringWidth(ctrl, iupAttribGet(params[i], "_IUPGP_TRUE"));
        int w = IupGetInt(ctrl, "SIZE");
        int v = IupGetInt(ctrl, "VALUE");
        if (v) /* True */
        {
          int box = w - wt;
          wf += box;
          if (wf > w)
            IupSetfAttribute(ctrl, "SIZE", "%dx", wf+8);
        }
        else
        {
          int box = w - wf;
          wt += box;
          if (wt > w)
            IupSetfAttribute(ctrl, "SIZE", "%dx", wt+8);
        }
      }

      IupSetfAttribute((Ihandle*)iupAttribGet(params[i], "LABEL"), "SIZE", "%dx", lbl_width);
    }

    i++;
  }

  IupSetAttribute(dlg, "SIZE", NULL);

  return dlg;
}

/*******************************************************************************************
                    Parameter String Parsing
*******************************************************************************************/

static char* iParamGetNextStrItem(char* line, char sep, int *count)
{
  int i = 0;

  while (line[i] != '\n' && line[i] != 0)
  {
    if (line[i] == sep)
    {
      line[i] = 0;
      *count = i+1;
      return line;
    }

    i++;
  }

  /* last item may not have the separator */
  line[i] = 0;
  *count = i;
  return line;
}

static void iParamSetBoolNames(char* extra, Ihandle* param)
{
  char *falsestr = NULL, *truestr = NULL;
  int count;

  if (extra)
  {
    falsestr = iParamGetNextStrItem(extra, ',', &count);  extra += count;
    truestr = iParamGetNextStrItem(extra, ',', &count);
  }

  if (falsestr && truestr)
  {
    iupAttribStoreStr(param, "_IUPGP_TRUE", truestr);
    iupAttribStoreStr(param, "_IUPGP_FALSE", falsestr);
  }
  else
  {
/*    iupAttribStoreStr(param, "_IUPGP_TRUE", iupStrMessageGet("IUP_TRUE"));     */
/*    iupAttribStoreStr(param, "_IUPGP_FALSE", iupStrMessageGet("IUP_FALSE"));   */
    iupAttribStoreStr(param, "_IUPGP_TRUE", "");
    iupAttribStoreStr(param, "_IUPGP_FALSE", "");
  }
}

static void iParamSetInterval(char* extra, Ihandle* param)
{
  char *min, *max, *step;
  int count;

  if (!extra)
    return;

  min = iParamGetNextStrItem(extra, ',', &count);  extra += count;
  max = iParamGetNextStrItem(extra, ',', &count);  extra += count;
  step = iParamGetNextStrItem(extra, ',', &count);  

  if (max[0])
  {
    iupAttribSetStr(param, "INTERVAL", "1");
    iupAttribStoreStr(param, "MIN", min);
    iupAttribStoreStr(param, "MAX", max);
    if (step[0])
      iupAttribStoreStr(param, "STEP", step);
  }
  else
  {
    iupAttribSetStr(param, "PARTIAL", "1");
    iupAttribStoreStr(param, "MIN", min);
  }
}

static void iParamSetFileOptions(char* extra, Ihandle* param)
{
  char *type, *filter, *directory, *nochangedir, *nooverwriteprompt;
  int count;

  if (!extra)
    return;

  type = iParamGetNextStrItem(extra, '|', &count);  extra += count;
  filter = iParamGetNextStrItem(extra, '|', &count);  extra += count;
  directory = iParamGetNextStrItem(extra, '|', &count);  extra += count;
  nochangedir = iParamGetNextStrItem(extra, '|', &count);  extra += count;
  nooverwriteprompt = iParamGetNextStrItem(extra, '|', &count);  extra += count;

  iupAttribStoreStr(param, "_IUPGP_DIALOGTYPE", type);
  iupAttribStoreStr(param, "_IUPGP_FILTER", filter);
  iupAttribStoreStr(param, "_IUPGP_DIRECTORY", directory);
  iupAttribStoreStr(param, "_IUPGP_NOCHANGEDIR", nochangedir);
  iupAttribStoreStr(param, "_IUPGP_NOOVERWRITEPROMPT", nooverwriteprompt);
}

static void iParamSetListItems(char* extra, Ihandle* param)
{
  int d = 1, count;
  char str[20], *item;

  if (!extra)
    return;

  item = iParamGetNextStrItem(extra, '|', &count);  extra += count;
  while (*item)
  {
    sprintf(str, "%d", d);
    iupAttribStoreStr(param, str, item);

    item = iParamGetNextStrItem(extra, '|', &count);  extra += count;
    d++;
  }

  sprintf(str, "%d", d);
  iupAttribSetStr(param, str, "");
}

static char* iParamGetStrExtra(char* line, char start, char end, int *count)
{
  int i = 0, end_pos = -1;

  if (*line != start)
  {
    *count = 0;
    return NULL;
  }
  line++; /* skip start */

  while (line[i] != '\n' && line[i] != 0)
  {
    if (line[i] == end)
      end_pos = i;

    i++;
  }

  if (end_pos != -1)
  {
    line[end_pos] = 0;
    *count = 1+end_pos+1;
    return line;
  }
  else
  {
    *count = 0;
    return NULL;
  }
}

static int iParamCopyStrLine(char* line, const char* format)
{
  int i = 0;
  while (format[i] != '\n' && format[i] != 0)
  {
    line[i] = format[i];
    i++;
    if (i > 4094)   /* to avoid being bigger than the local array */
      break;
  }
  line[i] = '\n';
  line[i+1] = 0;
  return i+1; 
}

char iupGetParamType(const char* format, int *line_size)
{
  char* type = strchr(format, '%');
  char* line_end = strchr(format, '\n');
  if (line_end)
    *line_size = line_end-format+1;
  if (type)
    return *(type+1);
  else
    return 0;
}

static Ihandle *IupParamf(const char* format, int *line_size)
{
  Ihandle* param;
  char line[4096];
  char* line_ptr = &line[0], *title, type, *tip, *extra, *mask;
  int count;

  *line_size = iParamCopyStrLine(line, format);

  title = iParamGetNextStrItem(line_ptr, '%', &count);  line_ptr += count;
  param = IupUser();
  iupAttribStoreStr(param, "TITLE", title);
  
  type = *line_ptr;
  line_ptr++;

  switch(type)
  {
  case 'b':
    iupAttribSetStr(param, "TYPE", "BOOLEAN");
    iupAttribSetStr(param, "DATA_TYPE", "1");  /* integer */
    extra = iParamGetStrExtra(line_ptr, '[', ']', &count);  line_ptr += count;
    iParamSetBoolNames(extra, param);
    break;
  case 'l':
    iupAttribSetStr(param, "TYPE", "LIST");
    iupAttribSetStr(param, "DATA_TYPE", "1");  /* integer */
    extra = iParamGetStrExtra(line_ptr, '|', '|', &count);  line_ptr += count;
    iParamSetListItems(extra, param);
    break;
  case 'a':
    iupAttribSetStr(param, "TYPE", "REAL");
    iupAttribSetStr(param, "DATA_TYPE", "2");  /* real */
    iupAttribSetStr(param, "ANGLE", "1");
    extra = iParamGetStrExtra(line_ptr, '[', ']', &count);  line_ptr += count;
    iParamSetInterval(extra, param);
    break;
  case 'm':
    iupAttribSetStr(param, "MULTILINE", "1");
    /* continue */
  case 's':
    iupAttribSetStr(param, "TYPE", "STRING");
    iupAttribSetStr(param, "DATA_TYPE", "0");  /* string */
    mask = iParamGetNextStrItem(line_ptr, '{', &count);  
    if (*mask) 
      iupAttribStoreStr(param, "MASK", mask);
    line_ptr += count-1; /* ignore the fake separator */
    line_ptr[0] = '{';   /* restore possible separator */
    break;
  case 'i':
    iupAttribSetStr(param, "TYPE", "INTEGER");
    iupAttribSetStr(param, "DATA_TYPE", "1");  /* integer */
    extra = iParamGetStrExtra(line_ptr, '[', ']', &count);  line_ptr += count;
    iParamSetInterval(extra, param);
    break;
  case 'r':
    iupAttribSetStr(param, "TYPE", "REAL");
    iupAttribSetStr(param, "DATA_TYPE", "2");  /* real */
    extra = iParamGetStrExtra(line_ptr, '[', ']', &count);  line_ptr += count;
    iParamSetInterval(extra, param);
    break;
  case 'f':
    iupAttribSetStr(param, "TYPE", "FILE");
    iupAttribSetStr(param, "DATA_TYPE", "0");  /* string */
    extra = iParamGetStrExtra(line_ptr, '[', ']', &count);  line_ptr += count;
    iParamSetFileOptions(extra, param);
    break;
  case 'c':
    iupAttribSetStr(param, "TYPE", "COLOR");
    iupAttribSetStr(param, "DATA_TYPE", "0");  /* string */
    break;
  case 't':
    iupAttribSetStr(param, "TYPE", "SEPARATOR");
    iupAttribSetStr(param, "DATA_TYPE", "-1"); /* NONE */
    break;
  default:
    return NULL;
  }

  tip = iParamGetStrExtra(line_ptr, '{', '}', &count);
  if (tip)
    iupAttribStoreStr(param, "TIP", tip);

  return param;
}

/*******************************************************************************************
                    Exported Functions
*******************************************************************************************/

int iupGetParamCount(const char *format, int *param_extra)
{
  int param_count = 0, sep = 0;
  const char* s = format;

  *param_extra = 0;
  while(*s)
  {
    if (*s == '%' && *(s+1) == 't')  /* do not count separator lines */
    {
      sep = 1;
      (*param_extra)++;
    }

    if (*s == '\n')
    {
      if (sep)
        sep = 0;
      else
        param_count++;
    }

    s++;
  }

  return param_count;
}

static void iParamDestroyAll(Ihandle **params)
{
  int i = 0;
  while (params[i] != NULL)
  {
    IupDestroy(params[i]);
    i++;
  }
}

int IupGetParamv(const char* title, Iparamcb action, void* user_data, const char* format, int param_count, int param_extra, void** param_data)
{
  Ihandle *dlg, *params[50];
  int i, line_size, p;

  assert(title && format);
  if (!title || !format)
    return 0;

  for (i = 0, p = 0; i < param_count+param_extra; i++)
  {
    int data_type;

    params[i] = IupParamf(format, &line_size);
    assert(params[i]);
    if (!params[i])
      return 0;

    data_type = IupGetInt(params[i], "DATA_TYPE");
    if (data_type == 1)
    {
      int *data_int = (int*)(param_data[p]);
      if (!data_int) return 0;
      iupAttribSetStrf(params[i], "VALUE", "%d", *data_int);
      p++;
    }
    else if (data_type == 2)
    {
      float *data_float = (float*)(param_data[p]);
      if (!data_float) return 0;
      iupAttribSetStrf(params[i], "VALUE", "%g", *data_float);
      p++;
    }
    else if (data_type == 0)
    {
      char *data_str = (char*)(param_data[p]);
      if (!data_str) return 0;
      iupAttribStoreStr(params[i], "VALUE", data_str);
      p++;
    }

    format += line_size; 
  }
  params[i] = NULL;

  dlg = IupParamDlgP(params);
  IupSetAttribute(dlg, "TITLE", (char*)title);
  IupSetCallback(dlg, "PARAM_CB", (Icallback)action);
  iupAttribSetStr(dlg, "USER_DATA", (char*)user_data);

  if (action) 
    action(dlg, -2, user_data);

  IupPopup(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

  if (!IupGetInt(dlg, "STATUS"))
  {
    iParamDestroyAll(params);
    IupDestroy(dlg);
    return 0;
  }
  else
  {
    for (i = 0, p = 0; i < param_count; i++)
    {
      Ihandle* param;
      int data_type;
      char str[20];

      sprintf(str, "PARAM%d", i);
      param = (Ihandle*)iupAttribGet(dlg, str);

      data_type = iupAttribGetInt(param, "DATA_TYPE");
      if (data_type == 1)
      {
        int *data_int = (int*)(param_data[i]);
        *data_int = iupAttribGetInt(param, "VALUE");
        p++;
      }
      else if (data_type == 2)
      {
        float *data_float = (float*)(param_data[i]);
        *data_float = iupAttribGetFloat(param, "VALUE");
        p++;
      }
      else 
      {
        char *data_str = (char*)(param_data[i]);
        strcpy(data_str, iupAttribGet(param, "VALUE"));
        p++;
      }
    }

    iParamDestroyAll(params);
    IupDestroy(dlg);
    return 1;
  }
}

int IupGetParam(const char* title, Iparamcb action, void* user_data, const char* format,...)
{
  int param_count, param_extra, i;
  void* param_data[50];
  va_list arg;

  param_count = iupGetParamCount(format, &param_extra);

  va_start(arg, format);
  for (i = 0; i < param_count; i++)
  {
    param_data[i] = (void*)(va_arg(arg, void*));
  }
  va_end(arg);

  return IupGetParamv(title, action, user_data, format, param_count, param_extra, param_data);
}

