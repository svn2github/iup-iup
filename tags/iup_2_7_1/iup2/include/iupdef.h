/** \file
 * \brief Callbacks, Attributes and Attribute Values definitions.    
 * Avoid using these definitions. Use the strings instead.
 *
 * See Copyright Notice in iup.h
 */
 
#ifndef __IUPDEF_H 
#define __IUPDEF_H

#define IUP_RUN            "RUN"
#define IUP_ENGLISH        "ENGLISH"
#define IUP_PORTUGUESE     "PORTUGUESE"
#define IUP_SBH            "SBH"
#define IUP_SBV            "SBV"

/************************************************************************/
/*                            Acoes                                     */
/************************************************************************/
#define IUP_DEFAULT_ACTION "DEFAULT_ACTION"
#define IUP_IDLE_ACTION    "IDLE_ACTION"

#define IUP_ACTION         "ACTION"
#define IUP_GETFOCUS_CB    "GETFOCUS_CB"
#define IUP_KILLFOCUS_CB   "KILLFOCUS_CB"
#define IUP_K_ANY          "K_ANY"
#define IUP_KEYPRESS_CB    "KEYPRESS_CB"
#define IUP_HELP_CB        "HELP_CB"

#define IUP_SCROLL_CB      "SCROLL_CB"
#define IUP_RESIZE_CB      "RESIZE_CB"
#define IUP_MOTION_CB      "MOTION_CB"
#define IUP_BUTTON_CB      "BUTTON_CB"
#define IUP_ENTERWINDOW_CB "ENTERWINDOW_CB"
#define IUP_LEAVEWINDOW_CB "LEAVEWINDOW_CB"
#define IUP_WHEEL_CB       "WHEEL_CB"

#define IUP_MASK_CB        "MASK_CB"
#define IUP_OPEN_CB        "OPEN_CB"
#define IUP_HIGHLIGHT_CB   "HIGHLIGHT_CB"
#define IUP_MENUCLOSE_CB   "MENUCLOSE_CB"

#define IUP_MAP_CB         "MAP_CB"
#define IUP_CLOSE_CB       "CLOSE_CB"
#define IUP_SHOW_CB        "SHOW_CB"

#define IUP_DROPFILES_CB   "DROPFILES_CB"
#define IUP_WOM_CB         "WOM_CB"

/************************************************************************/
/*                            Atributos                                 */
/************************************************************************/

#define IUP_DIRECTION      "DIRECTION"
#define IUP_ACTIVE         "ACTIVE"
#define IUP_BGCOLOR        "BGCOLOR"
#define IUP_FRAMECOLOR     "FRAMECOLOR"
#define IUP_FGCOLOR        "FGCOLOR"
#define IUP_COLOR          "COLOR"
#define IUP_WID            "WID"
#define IUP_SIZE           "SIZE"
#define IUP_RASTERSIZE     "RASTERSIZE"
#define IUP_TITLE          "TITLE"
#define IUP_VALUE          "VALUE"
#define IUP_VISIBLE        "VISIBLE"
#define IUP_FONT           "FONT"
#define IUP_TIP            "TIP"
#define IUP_EXPAND         "EXPAND"
#define IUP_SEPARATOR      "SEPARATOR"

#define IUP_HOTSPOT        "HOTSPOT"
#define IUP_HEIGHT         "HEIGHT"
#define IUP_WIDTH          "WIDTH"

#define IUP_KEY            "KEY"

#define IUP_MULTIPLE       "MULTIPLE"
#define IUP_DROPDOWN       "DROPDOWN"
#define IUP_VISIBLE_ITEMS  "VISIBLE_ITEMS"

#define IUP_MARGIN         "MARGIN"
#define IUP_GAP            "GAP"
#define IUP_ALIGNMENT      "ALIGNMENT"

#define IUP_IMAGE          "IMAGE"
#define IUP_IMINACTIVE     "IMINACTIVE"
#define IUP_IMPRESS        "IMPRESS"
#define IUP_SAVEUNDER      "SAVEUNDER"

#define IUP_NC             "NC"
#define IUP_MASK           "MASK"

#define IUP_APPEND         "APPEND"
#define IUP_BORDER         "BORDER"

#define IUP_CARET          "CARET"
#define IUP_SELECTION      "SELECTION"
#define IUP_SELECTEDTEXT   "SELECTEDTEXT"
#define IUP_INSERT         "INSERT"

#define IUP_CONID          "CONID"
#define IUP_CURSOR         "CURSOR"

#define IUP_ICON           "ICON"
#define IUP_MENUBOX        "MENUBOX"
#define IUP_MINBOX         "MINBOX"
#define IUP_MAXBOX         "MAXBOX"
#define IUP_RESIZE         "RESIZE"
#define IUP_MENU           "MENU"
#define IUP_STARTFOCUS     "STARTFOCUS"
#define IUP_PARENTDIALOG   "PARENTDIALOG"
#define IUP_SHRINK         "SHRINK"
#define IUP_DEFAULTENTER   "DEFAULTENTER"
#define IUP_DEFAULTESC     "DEFAULTESC"
#define IUP_X              "X"
#define IUP_Y              "Y"
#define IUP_TOOLBOX        "TOOLBOX"
#define IUP_CONTROL        "CONTROL"
#define IUP_READONLY       "READONLY"

#define IUP_SCROLLBAR      "SCROLLBAR"
#define IUP_POSY           "POSY"
#define IUP_POSX           "POSX"
#define IUP_DX             "DX"
#define IUP_DY             "DY"
#define IUP_XMAX           "XMAX"
#define IUP_XMIN           "XMIN"
#define IUP_YMAX           "YMAX"
#define IUP_YMIN           "YMIN"

#define IUP_RED            "255 0 0"
#define IUP_GREEN          "0 255 0"
#define IUP_BLUE           "0 0 255"

#define IUP_MIN            "MIN"
#define IUP_MAX            "MAX"

#define IUP_TIME           "TIME"
#define IUP_DRAG           "DRAG"
#define IUP_DROP           "DROP"
#define IUP_REPAINT        "REPAINT"
#define IUP_TOPMOST        "TOPMOST"
#define IUP_CLIPCHILDREN   "CLIPCHILDREN"

#define IUP_DIALOGTYPE     "DIALOGTYPE"
#define IUP_FILE           "FILE"
#define IUP_MULTIPLEFILES  "MULTIPLEFILES"
#define IUP_FILTER         "FILTER"
#define IUP_FILTERUSED     "FILTERUSED"
#define IUP_FILTERINFO     "FILTERINFO"
#define IUP_EXTFILTER      "EXTFILTER"
#define IUP_DIRECTORY      "DIRECTORY"
#define IUP_ALLOWNEW       "ALLOWNEW"
#define IUP_NOOVERWRITEPROMPT "NOOVERWRITEPROMPT"
#define IUP_NOCHANGEDIR    "NOCHANGEDIR"
#define IUP_FILEEXIST      "FILEEXIST"
#define IUP_STATUS         "STATUS"

#define IUP_LOCKLOOP       "LOCKLOOP"
#define IUP_SYSTEM         "SYSTEM"
#define IUP_DRIVER         "DRIVER"
#define IUP_SCREENSIZE     "SCREENSIZE"
#define IUP_SYSTEMLANGUAGE "SYSTEMLANGUAGE"
#define IUP_COMPUTERNAME   "COMPUTERNAME"
#define IUP_USERNAME       "USERNAME"

/************************************************************************/
/*                   Valores para o filedlg                             */
/************************************************************************/
#define IUP_OPEN "OPEN"
#define IUP_SAVE "SAVE"
#define IUP_DIR  "DIR"

/************************************************************************/
/*                   Valores para scrollbar                             */
/************************************************************************/
#define IUP_HORIZONTAL     "HORIZONTAL"
#define IUP_VERTICAL       "VERTICAL"

/************************************************************************/
/*                       Valores dos Atributos                          */
/************************************************************************/
#define IUP_YES       "YES"
#define IUP_NO        "NO"
#define IUP_ON        "ON"
#define IUP_OFF       "OFF"

#define IUP_ACENTER   "ACENTER"
#define IUP_ALEFT     "ALEFT"
#define IUP_ARIGHT    "ARIGHT"
#define IUP_ATOP      "ATOP"
#define IUP_ABOTTOM   "ABOTTOM"

#define IUP_NORTH     "NORTH"
#define IUP_SOUTH     "SOUTH"
#define IUP_WEST      "WEST"
#define IUP_EAST      "EAST"
#define IUP_NE        "NE"
#define IUP_SE        "SE"
#define IUP_NW        "NW"
#define IUP_SW        "SW"

#define IUP_FULLSCREEN "FULLSCREEN"
#define IUP_FULL      "FULL"
#define IUP_HALF      "HALF"
#define IUP_THIRD     "THIRD"
#define IUP_QUARTER   "QUARTER"
#define IUP_EIGHTH    "EIGHTH"

#define IUP_ARROW     "ARROW"
#define IUP_BUSY      "BUSY"
#define IUP_RESIZE_N  "RESIZE_N"
#define IUP_RESIZE_S  "RESIZE_S"
#define IUP_RESIZE_E  "RESIZE_E"
#define IUP_RESIZE_W  "RESIZE_W"
#define IUP_RESIZE_NE "RESIZE_NE"
#define IUP_RESIZE_NW "RESIZE_NW"
#define IUP_RESIZE_SE "RESIZE_SE"
#define IUP_RESIZE_SW "RESIZE_SW"
#define IUP_MOVE      "MOVE"
#define IUP_HAND      "HAND"
#define IUP_NONE      "NONE"
#define IUP_IUP       "IUP"
#define IUP_CROSS     "CROSS"
#define IUP_PEN       "PEN"
#define IUP_TEXT      "TEXT"
#define IUP_RESIZE_C  "RESIZE_C"
#define IUP_OPENHAND  "OPENHAND"

#define IUP_HELVETICA_NORMAL_8   "HELVETICA_NORMAL_8"
#define IUP_HELVETICA_ITALIC_8   "HELVETICA_ITALIC_8"
#define IUP_HELVETICA_BOLD_8     "HELVETICA_BOLD_8"
#define IUP_HELVETICA_NORMAL_10  "HELVETICA_NORMAL_10"
#define IUP_HELVETICA_ITALIC_10  "HELVETICA_ITALIC_10"
#define IUP_HELVETICA_BOLD_10    "HELVETICA_BOLD_10"
#define IUP_HELVETICA_NORMAL_12  "HELVETICA_NORMAL_12"
#define IUP_HELVETICA_ITALIC_12  "HELVETICA_ITALIC_12"
#define IUP_HELVETICA_BOLD_12    "HELVETICA_BOLD_12"
#define IUP_HELVETICA_NORMAL_14  "HELVETICA_NORMAL_14"
#define IUP_HELVETICA_ITALIC_14  "HELVETICA_ITALIC_14"
#define IUP_HELVETICA_BOLD_14    "HELVETICA_BOLD_14"
#define IUP_COURIER_NORMAL_8     "COURIER_NORMAL_8"
#define IUP_COURIER_ITALIC_8     "COURIER_ITALIC_8"
#define IUP_COURIER_BOLD_8       "COURIER_BOLD_8"
#define IUP_COURIER_NORMAL_10    "COURIER_NORMAL_10"
#define IUP_COURIER_ITALIC_10    "COURIER_ITALIC_10"
#define IUP_COURIER_BOLD_10      "COURIER_BOLD_10"
#define IUP_COURIER_NORMAL_12    "COURIER_NORMAL_12"
#define IUP_COURIER_ITALIC_12    "COURIER_ITALIC_12"
#define IUP_COURIER_BOLD_12      "COURIER_BOLD_12"
#define IUP_COURIER_NORMAL_14    "COURIER_NORMAL_14"
#define IUP_COURIER_ITALIC_14    "COURIER_ITALIC_14"
#define IUP_COURIER_BOLD_14      "COURIER_BOLD_14"
#define IUP_TIMES_NORMAL_8       "TIMES_NORMAL_8"
#define IUP_TIMES_ITALIC_8       "TIMES_ITALIC_8"
#define IUP_TIMES_BOLD_8         "TIMES_BOLD_8"
#define IUP_TIMES_NORMAL_10      "TIMES_NORMAL_10"
#define IUP_TIMES_ITALIC_10      "TIMES_ITALIC_10"
#define IUP_TIMES_BOLD_10        "TIMES_BOLD_10"
#define IUP_TIMES_NORMAL_12      "TIMES_NORMAL_12"
#define IUP_TIMES_ITALIC_12      "TIMES_ITALIC_12"
#define IUP_TIMES_BOLD_12        "TIMES_BOLD_12"
#define IUP_TIMES_NORMAL_14      "TIMES_NORMAL_14"
#define IUP_TIMES_ITALIC_14      "TIMES_ITALIC_14"
#define IUP_TIMES_BOLD_14        "TIMES_BOLD_14"

/************************************************************************/
/*                           Teclas                                     */
/************************************************************************/
#define IUP_K_exclam            "K_exclam"
#define IUP_K_quotedbl          "K_quotedbl"
#define IUP_K_numbersign        "K_numbersign"
#define IUP_K_dollar            "K_dollar"
#define IUP_K_percent           "K_percent"
#define IUP_K_ampersand         "K_ampersand"
#define IUP_K_quoteright        "K_quoteright"
#define IUP_K_parentleft        "K_parentleft"
#define IUP_K_parentright       "K_parentright"
#define IUP_K_asterisk          "K_asterisk"
#define IUP_K_plus              "K_plus"
#define IUP_K_comma             "K_comma"
#define IUP_K_minus             "K_minus"
#define IUP_K_period            "K_period"
#define IUP_K_slash             "K_slash"
#define IUP_K_0                 "K_0"
#define IUP_K_1                 "K_1"
#define IUP_K_2                 "K_2"
#define IUP_K_3                 "K_3"
#define IUP_K_4                 "K_4"
#define IUP_K_5                 "K_5"
#define IUP_K_6                 "K_6"
#define IUP_K_7                 "K_7"
#define IUP_K_8                 "K_8"
#define IUP_K_9                 "K_9"
#define IUP_K_colon             "K_colon"
#define IUP_K_semicolon         "K_semicolon "
#define IUP_K_less              "K_less"
#define IUP_K_equal             "K_equal"
#define IUP_K_greater           "K_greater"
#define IUP_K_question          "K_question"
#define IUP_K_at                "K_at"
#define IUP_K_A                 "K_A"
#define IUP_K_B                 "K_B"
#define IUP_K_C                 "K_C"
#define IUP_K_D                 "K_D"
#define IUP_K_E                 "K_E"
#define IUP_K_F                 "K_F"
#define IUP_K_G                 "K_G"
#define IUP_K_H                 "K_H"
#define IUP_K_I                 "K_I"
#define IUP_K_J                 "K_J"
#define IUP_K_K                 "K_K"
#define IUP_K_L                 "K_L"
#define IUP_K_M                 "K_M"
#define IUP_K_N                 "K_N"
#define IUP_K_O                 "K_O"
#define IUP_K_P                 "K_P"
#define IUP_K_Q                 "K_Q"
#define IUP_K_R                 "K_R"
#define IUP_K_S                 "K_S"
#define IUP_K_T                 "K_T"
#define IUP_K_U                 "K_U"
#define IUP_K_V                 "K_V"
#define IUP_K_W                 "K_W"
#define IUP_K_X                 "K_X"
#define IUP_K_Y                 "K_Y"
#define IUP_K_Z                 "K_Z"
#define IUP_K_bracketleft       "K_bracketleft"
#define IUP_K_backslash         "K_backslash"
#define IUP_K_bracketright      "K_bracketright"
#define IUP_K_circum            "K_circum"
#define IUP_K_underscore        "K_underscore"
#define IUP_K_quoteleft         "K_quoteleft"
#define IUP_K_a                 "K_a"
#define IUP_K_b                 "K_b"
#define IUP_K_c                 "K_c"
#define IUP_K_d                 "K_d"
#define IUP_K_e                 "K_e"
#define IUP_K_f                 "K_f"
#define IUP_K_g                 "K_g"
#define IUP_K_h                 "K_h"
#define IUP_K_i                 "K_i"
#define IUP_K_j                 "K_j"
#define IUP_K_k                 "K_k"
#define IUP_K_l                 "K_l"
#define IUP_K_m                 "K_m"
#define IUP_K_n                 "K_n"
#define IUP_K_o                 "K_o"
#define IUP_K_p                 "K_p"
#define IUP_K_q                 "K_q"
#define IUP_K_r                 "K_r"
#define IUP_K_s                 "K_s"
#define IUP_K_t                 "K_t"
#define IUP_K_u                 "K_u"
#define IUP_K_v                 "K_v"
#define IUP_K_w                 "K_w"
#define IUP_K_x                 "K_x"
#define IUP_K_y                 "K_y"
#define IUP_K_z                 "K_z"
#define IUP_K_braceleft         "K_braceleft"
#define IUP_K_bar               "K_bar"
#define IUP_K_braceright        "K_braceright"
#define IUP_K_tilde             "K_tilde"

#define IUP_K_cA       "K_cA"
#define IUP_K_cB       "K_cB"
#define IUP_K_cC       "K_cC"
#define IUP_K_cD       "K_cD"
#define IUP_K_cE       "K_cE"
#define IUP_K_cF       "K_cF"
#define IUP_K_cG       "K_cG"
#define IUP_K_cJ       "K_cJ"
#define IUP_K_cK       "K_cK"
#define IUP_K_cL       "K_cL"
#define IUP_K_cN       "K_cN"
#define IUP_K_cO       "K_cO"
#define IUP_K_cP       "K_cP"
#define IUP_K_cQ       "K_cQ"
#define IUP_K_cR       "K_cR"
#define IUP_K_cS       "K_cS"
#define IUP_K_cT       "K_cT"
#define IUP_K_cU       "K_cU"
#define IUP_K_cV       "K_cV"
#define IUP_K_cW       "K_cW"
#define IUP_K_cX       "K_cX"
#define IUP_K_cY       "K_cY"
#define IUP_K_cZ       "K_cZ"
#define IUP_K_mA       "K_mA"
#define IUP_K_mB       "K_mB"
#define IUP_K_mC       "K_mC"
#define IUP_K_mD       "K_mD"
#define IUP_K_mE       "K_mE"
#define IUP_K_mF       "K_mF"
#define IUP_K_mG       "K_mG"
#define IUP_K_mH       "K_mH"
#define IUP_K_mI       "K_mI"
#define IUP_K_mJ       "K_mJ"
#define IUP_K_mK       "K_mK"
#define IUP_K_mL       "K_mL"
#define IUP_K_mM       "K_mM"
#define IUP_K_mN       "K_mN"
#define IUP_K_mO       "K_mO"
#define IUP_K_mP       "K_mP"
#define IUP_K_mQ       "K_mQ"
#define IUP_K_mR       "K_mR"
#define IUP_K_mS       "K_mS"
#define IUP_K_mT       "K_mT"
#define IUP_K_mU       "K_mU"
#define IUP_K_mV       "K_mV"
#define IUP_K_mW       "K_mW"
#define IUP_K_mX       "K_mX"
#define IUP_K_mY       "K_mY"
#define IUP_K_mZ       "K_mZ"
#define IUP_K_BS       "K_BS"
#define IUP_K_TAB      "K_TAB"
#define IUP_K_CR       "K_CR"
#define IUP_K_SP       "K_SP"
#define IUP_K_ESC      "K_ESC"
#define IUP_K_sCR      "K_sCR"
#define IUP_K_sTAB     "K_sTAB"
#define IUP_K_cTAB     "K_cTAB"
#define IUP_K_mTAB     "K_mTAB"
#define IUP_K_HOME     "K_HOME"
#define IUP_K_UP       "K_UP"
#define IUP_K_PGUP     "K_PGUP"
#define IUP_K_LEFT     "K_LEFT"
#define IUP_K_RIGHT    "K_RIGHT"
#define IUP_K_END      "K_END"
#define IUP_K_DOWN     "K_DOWN"
#define IUP_K_PGDN     "K_PGDN"
#define IUP_K_MIDDLE   "K_MIDDLE"
#define IUP_K_INS      "K_INS"
#define IUP_K_DEL      "K_DEL"
#define IUP_K_sHOME    "K_sHOME"
#define IUP_K_sUP      "K_sUP"
#define IUP_K_sPGUP    "K_sPGUP"
#define IUP_K_sLEFT    "K_sLEFT"
#define IUP_K_sRIGHT   "K_sRIGHT"
#define IUP_K_sEND     "K_sEND"
#define IUP_K_sDOWN    "K_sDOWN"
#define IUP_K_sPGDN    "K_sPGDN"
#define IUP_K_cHOME    "K_cHOME"
#define IUP_K_cPGUP    "K_cPGUP"
#define IUP_K_cLEFT    "K_cLEFT"
#define IUP_K_cRIGHT   "K_cRIGHT"
#define IUP_K_cEND     "K_cEND"
#define IUP_K_cPGDN    "K_cPGDN"
#define IUP_K_cUP      "K_cUP"
#define IUP_K_cDOWN    "K_cDOWN"
#define IUP_K_cMIDDLE  "K_cMIDDLE"
#define IUP_K_cINS     "K_cINS"
#define IUP_K_cDEL     "K_cDEL"
#define IUP_K_mHOME    "K_mHOME"
#define IUP_K_mPGUP    "K_mPGUP"
#define IUP_K_mLEFT    "K_mLEFT"
#define IUP_K_mRIGHT   "K_mRIGHT"
#define IUP_K_mEND     "K_mEND"
#define IUP_K_mPGDN    "K_mPGDN"
#define IUP_K_mUP      "K_mUP"
#define IUP_K_mDOWN    "K_mDOWN"
#define IUP_K_mINS     "K_mINS"
#define IUP_K_mDEL     "K_mDEL"
#define IUP_K_F1       "K_F1"
#define IUP_K_F2       "K_F2"
#define IUP_K_F3       "K_F3"
#define IUP_K_F4       "K_F4"
#define IUP_K_F5       "K_F5"
#define IUP_K_F6       "K_F6"
#define IUP_K_F7       "K_F7"
#define IUP_K_F8       "K_F8"
#define IUP_K_F9       "K_F9"
#define IUP_K_F10      "K_F10"
#define IUP_K_F11      "K_F11"
#define IUP_K_F12      "K_F12"
#define IUP_K_sF1      "K_sF1"
#define IUP_K_sF2      "K_sF2"
#define IUP_K_sF3      "K_sF3"
#define IUP_K_sF4      "K_sF4"
#define IUP_K_sF5      "K_sF5"
#define IUP_K_sF6      "K_sF6"
#define IUP_K_sF7      "K_sF7"
#define IUP_K_sF8      "K_sF8"
#define IUP_K_sF9      "K_sF9"
#define IUP_K_sF10     "K_sF10"
#define IUP_K_sF11     "K_sF11"
#define IUP_K_sF12     "K_sF12"
#define IUP_K_cF1      "K_cF1"
#define IUP_K_cF2      "K_cF2"
#define IUP_K_cF3      "K_cF3"
#define IUP_K_cF4      "K_cF4"
#define IUP_K_cF5      "K_cF5"
#define IUP_K_cF6      "K_cF6"
#define IUP_K_cF7      "K_cF7"
#define IUP_K_cF8      "K_cF8"
#define IUP_K_cF9      "K_cF9"
#define IUP_K_cF10     "K_cF10"
#define IUP_K_cF11     "K_cF11"
#define IUP_K_cF12     "K_cF12"
#define IUP_K_mF1      "K_mF1"
#define IUP_K_mF2      "K_mF2"
#define IUP_K_mF3      "K_mF3"
#define IUP_K_mF4      "K_mF4"
#define IUP_K_mF5      "K_mF5"
#define IUP_K_mF6      "K_mF6"
#define IUP_K_mF7      "K_mF7"
#define IUP_K_mF8      "K_mF8"
#define IUP_K_mF9      "K_mF9"
#define IUP_K_mF10     "K_mF10"
#define IUP_K_m1       "K_m1"
#define IUP_K_m2       "K_m2"
#define IUP_K_m3       "K_m3"
#define IUP_K_m4       "K_m4"
#define IUP_K_m5       "K_m5"
#define IUP_K_m6       "K_m6"
#define IUP_K_m7       "K_m7"
#define IUP_K_m8       "K_m8"
#define IUP_K_m9       "K_m9"
#define IUP_K_m0       "K_m0"

#endif
