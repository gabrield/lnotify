/*
===============================================================================

Copyright (C) 2010-2011 Gabriel Duarte (gabrield@devio.us)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

===============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <math.h>
#include <glib.h>
#include <unistd.h>
#include <libnotify/notify.h>

#define VERSION 0.3

int LUA_API luaopen_notify (lua_State *);

static int
newnotify (lua_State * L)
{
  NotifyNotification *notification = NULL;
  const char *summary, *body, *icon;
  GError *error = NULL;
  int top;

  if (!notify_is_initted() && !notify_init ("icon-summary-body")) {
    g_error_free (error);
    return luaL_error (L, "icon-summary-body");
  }

  top = lua_gettop(L);
  summary = luaL_checkstring (L, 1);

  body = (top > 1)
    ? luaL_checkstring (L, 2)
    : NULL;
  icon = (top > 2)
    ? luaL_checkstring (L, 3)
    : NULL;

  if (notification == (NotifyNotification *) 0) {
    notification = notify_notification_new (
      summary,
      body,
      icon
#if NOTIFY_CHECK_VERSION(0, 7, 0)
#else
      , NULL
#endif
    );
  }

  lua_pushlightuserdata (L, notification);

  return 1;
}

static int
setappname (lua_State * L)
{
  const char *name;
  NotifyNotification *notify;

  if (lua_gettop(L) < 2) {
    /* if there are less than two args, assume we're trying to set the default app name */
    name = luaL_checkstring(L, 1);
    if (notify_is_initted())
      notify_set_app_name(name);
    else
      notify_init(name);
  }
  else {
    notify = (NotifyNotification *) lua_touserdata(L, 1);
    if (!notify) luaL_argerror(L, 1, "Unknown type");
    name = luaL_checkstring(L, 2);
    notify_notification_set_app_name(notify, name);
  }

  return 0;
}

static int
show (lua_State * L)
{
  NotifyNotification *notify = (NotifyNotification *) lua_touserdata (L, 1);
  if (!notify) luaL_argerror(L, 1, "Unknown type");
  notify_notification_show (notify, NULL);
  return 0;
}

static int
set_urgency (lua_State * L)
{
  int l = 0;
  NotifyUrgency level = 0;
  NotifyNotification *notify = (NotifyNotification *) lua_touserdata (L, 1);
  if (!notify) luaL_argerror(L, 1, "Unknown type");

  if (lua_isnumber(L, 2)) {
      l = lua_tointeger(L, 2);
  }
  else {
      const char *lstr = luaL_checkstring(L, 2);
      if      (strcmp(lstr, "low") == 0)      l = 1;
      else if (strcmp(lstr, "normal") == 0)   l = 2;
      else if (strcmp(lstr, "critical") == 0) l = 3;
      else luaL_argerror(L, 2, lstr);
  }

  switch(l) {
    case 1:
      level = NOTIFY_URGENCY_LOW;
      break;
    case 2:
      level = NOTIFY_URGENCY_NORMAL;
      break;
    case 3:
      level = NOTIFY_URGENCY_CRITICAL;
      break;
    default:
      luaL_argerror(L, 2, lua_tolstring(L, 2, NULL)); /* coerce int to string */
  }

  notify_notification_set_urgency (notify, level);
  return 0;
}

int LUA_API
luaopen_notify (lua_State * L)
{
  const luaL_Reg driver[] = {
    {"show", show},
    {"set_urgency", set_urgency},
    {"set_appname", setappname},
    {"new", newnotify},
    {NULL, NULL},
  };

#if LUA_VERSION_NUM == 502
  luaL_newlib(L, driver);
#else
  luaL_register(L, "notify", driver);
#endif

  lua_pushliteral (L, "VERSION");
  lua_pushnumber (L, VERSION);
  lua_settable (L, -3);

  return 1;
}
