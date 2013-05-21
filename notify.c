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
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <math.h>
#include <glib-2.0/glib.h>
#include <unistd.h>
#include <libnotify/notify.h>

#define VERSION 0.4

int LUA_API luaopen_notify (lua_State *);

static int
newnotify (lua_State * L)
{
  NotifyNotification *notification = NULL;
  const char *summary, *body, *icon;
  GError *error = NULL;

  if (!notify_init ("icon-summary-body")) {
    g_error_free (error);
    return luaL_error (L, "icon-summary-body");
  }

  summary = luaL_checkstring (L, 1);
  body = luaL_checkstring (L, 2);
  icon = (lua_gettop (L) > 2)
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
  NotifyNotification **note_pointer = (NotifyNotification **)lua_newuserdata(L, sizeof(NotifyNotification *));
  *note_pointer = notification;
  luaL_getmetatable(L, "lnotify.notification");
  lua_setmetatable(L, -2);

  /* lua_pushlightuserdata (L, notification); */

  return 1;
}


static int
show (lua_State * L)
{
  NotifyNotification **notify = (NotifyNotification **)luaL_checkudata(L, 1, "lnotify.notification");

  if (notify)
    notify_notification_show (*notify, NULL);
  else
    luaL_error (L, "NULL");

  return 0;
}

static int
set_urgency (lua_State * L)
{
  NotifyUrgency level = 0;
  int l = 0;
  NotifyNotification **notify = (NotifyNotification **)luaL_checkudata(L, 1, "lnotify.notification");

  if (!lua_isnumber (L, 2)) {
    return lua_error (L);
  }

  l = luaL_checknumber (L, 2);

  if (l <= 0 || l > 3)		/*check if the number is lower or higher than the allowed numbers. if the number is not valid, returns 0 */
    return 0;

  switch (l) {
  case 1:
    level = NOTIFY_URGENCY_LOW;
    break;
  case 2:
    level = NOTIFY_URGENCY_NORMAL;
    break;
  case 3:
    level = NOTIFY_URGENCY_CRITICAL;
    break;
  }
  notify_notification_set_urgency (*notify, level);
  return 0;
}

int LUA_API
luaopen_notify (lua_State * L)
{
  const luaL_Reg driver[] = {
    {"new", newnotify},
    {NULL, NULL},
  };

  const luaL_Reg notify_object[] = {
    {"show", show},
    {"set_urgency", set_urgency},
    {NULL, NULL},
  };

  luaL_newmetatable(L, "lnotify.notification");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, notify_object, 0);

  luaL_newlib(L, driver);

  return 1;
}
