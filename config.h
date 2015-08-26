/* See LICENSE file for copyright and license details. */
#include "gaplessgrid.c"

/* appearance */
#define NUMCOLORS 8
static const char colors[NUMCOLORS][MAXCOLORS][9] = {
    // border     fg         bg
    { "#ababab", "#7d7d7d", "#020202" },  // 01 - normal
    { "#FF0000", "#20b2e7", "#020202" },  // 02 - selected
    { "#B3354C", "#B3354C", "#020202" },  // 03 - urgent
    { "#FFA500", "#FFA500", "#020202" },  // 14 - orange
    { "#20b2e7", "#20b2e7", "#020202" },  // 0F - White
    { "#608040", "#608040", "#020202" },  // 06 - green
    { "#B1D354", "#B1D354", "#020202" },  // 0F - light green
    { "#55b521", "#55b521", "#020202" },  // 0F - strong green
//    { "#00FF00", "#20b2e7", "#00FF00" },  // 04 - black
//    { "#20b2e7", "#20b2e7", "#020202" },  // 0F - White
//    { "#FF0000", "#FF0000", "#020202" },  // 05 - red
//    { "#877C43", "#877C43", "#020202" },  // 07 - yellow
//    { "#1C678C", "#1C678C", "#020202" },  // 08 - blue
//    { "#E300FF", "#E300FF", "#020202" },  // 09 - magenta
//    { "#000000", "#000000", "#000000" },  // unusable
//    { "#337373", "#337373", "#020202" },  // 0B - cyan
//    { "#808080", "#808080", "#020202" },  // 0C - light gray
//    { "#4C4C4C", "#4C4C4C", "#020202" },  // 0D - gray
//    { "#FFEE00", "#FFEE00", "#020202" },  // 0E - yellow2
//    { "#20b2e7", "#20b2e7", "#020202" },  // 0F - White
//    { "#BF9F5F", "#BF9F5F", "#020202" },  // 10 - light yellow
//    { "#3995BF", "#3995BF", "#020202" },  // 11 - light blue
//    { "#A64286", "#A64286", "#020202" },  // 12 - light magenta
//    { "#6C98A6", "#6C98A6", "#020202" },  // 13 - light cyan
//    { "#FFA500", "#FFA500", "#020202" },  // 14 - orange
//
//    { "#0300ff", "#0300ff", "#802635" },  // 15 - warning
};

static const char *fonts[] = {
    "ohsnap.icons-medium-r-normal-*-14-*-*-*-*-*-*-*",
    //"Sans:size=10.5",
    //"VL Gothic:size=10.5",
    //"WenQuanYi Micro Hei:size=10.5",
};
static const char dmenufont[] = "-*-ohsnap.icons-medium-r-*-*-14-*-*-*-*-*-*-*";
//static const char normbordercolor[] = colors[0][0];
//static const char normbgcolor[]     = colors[0][2];
//static const char normfgcolor[]     = colors[0][1];
//static const char selbordercolor[]  = colors[1][0];
//static const char selbgcolor[]      = colors[1][2];
//static const char selfgcolor[]      = colors[1][1];
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { "ÿ", "ý", "ü", "þ", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            True,        -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
    { "TTT",      htile },
    { "###",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", colors[0][2], "-nf", colors[0][1], "-sb", colors[1][2], "-sf", colors[1][1], NULL };
static const char *termcmd[]  = { "urxvt", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_v,      setlayout,      {.v = &layouts[3]} },
    { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4] } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

