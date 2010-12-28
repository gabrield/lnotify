/*
===============================================================================

Copyright (C) 2010 Gabriel Duarte (gabrield@impa.br)

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
#include <lauxlib.h>
#include <math.h>
#include <glib.h>
#include <unistd.h>
#include <libnotify/notify.h>

#ifndef VERSION
#define VERSION "0.2-6"
#endif

int LUA_API luaopen_notify(lua_State *);

NotifyUrgency level = 0;


static int set_urgency(lua_State *L)
{
    int l = 0;
    if(!lua_isnumber(L, 1))
    {
        return lua_error(L);
    }
    
    l  = luaL_checknumber(L, 1);
    
    if(l <= 0 || l > 3) /*check if the number is lower or higher than the allowed numbers. if the number is not valid, returns 0*/
        return 0;

    switch(l)
    {
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
    return 0;
}

static int notify(lua_State *L)
{
    const char *summary, *body, *icon;
    static NotifyNotification *notification;
    GError *error = NULL;

    if(!notify_init("icon-summary-body"))
    {
        g_error_free (error);
        return luaL_error(L, "icon-summary-body");
    }
   
    if(lua_gettop(L) > 2 )
        icon = luaL_checkstring(L, 3);
    else
        icon = NULL;

 
    summary = luaL_checkstring(L, 1);
    body = luaL_checkstring(L, 2);
            
    /* Now, we always get a new notification, no update */
    if(notification == (NotifyNotification*)0)
    {
        /*notify_notification_close(notification, NULL);*/
        notification = notify_notification_new(summary, body, icon, NULL);
        notify_notification_set_urgency(notification, level);
        /*printf("NEW\n");*/
    }
    else
    {
/*      notify_notification_close(notification, NULL);
        notification = notify_notification_new(summary, body, icon, NULL);
        notify_notification_set_urgency(notification, level);
        printf("UPDATE\n");*/
        notify_notification_update(notification, summary, body, icon);
    }

    error = NULL;
    notify_notification_show(notification, &error);

    return 0 ;
}

int LUA_API luaopen_notify(lua_State *L)
{
    const luaL_Reg driver[] = 
    {
        {"message", notify},
        {"set_urgency", set_urgency},
        {NULL, NULL},
    };
    
    luaL_openlib (L, "notify", driver, 0);
    lua_pushliteral(L, "version");
    lua_pushliteral(L, VERSION);
    lua_settable(L, -3);
    
    return 1;
}
