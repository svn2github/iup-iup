------------------------------------------------------------------------------
-- Scintilla class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "scintilla",
  parent = iup.WIDGET,
  creation = "",
  callback = {
  savepoint_cb = "n",
--  valuechanged_cb = "",
--  caret_cb = "nnn", 
  marginclick_cb = "nns",
  hotspotclick_cb = "nnns",
--  button_cb = "nnnns",
--  motion_cb = "nns",
  action = "nnns",
  },
  include = "iup_scintilla.h",
  extracode = [[ 
int iup_scintillalua_open(lua_State* L)
{
  if (iuplua_opencall_internal(L))
    IupScintillaOpen();
    
  iuplua_get_env(L);
  iupscintillalua_open(L);
  return 0;
}

/* obligatory to use require"iuplua_scintilla" */
int luaopen_iuplua_scintilla(lua_State* L)
{
  return iup_scintillalua_open(L);
}

]]
}

function ctrl.createElement(class, param)
  return iup.Scintilla()
end

iup.RegisterWidget(ctrl)
iup.SetClass(ctrl, "iup widget")
