lnotify is a simple library binding libnotify to Lua, quite easy to use, supporting emergency levels.
Pop up notifications from Lua to your desktop now is a piece of cake. 

To compile lnotify, you need o have installed the develpment files of libnotify and gtk+-2.0 . After that, just type "make" to compile, optionally setting the `LUA_INC` environment variable to compile against a specific Lua version:

<pre>
$ LUA_INC=lua5.2; make
gcc -O2 -Wall -Wmissing-prototypes -Wmissing-declarations -ansi -pedantic -fPIC -I/usr/include/lua5.2  `pkg-config --cflags --libs gtk+-2.0 libnotify`    -c -o notify.o notify.c
gcc -o notify.so -shared  notify.o -O2 -Wall -Wmissing-prototypes -Wmissing-declarations -ansi -pedantic -fPIC -I/usr/include/lua5.2  `pkg-config --cflags --libs gtk+-2.0 libnotify`
$
</pre>


To use lnotify is easy, just require it on Lua, like this:

$ lua
Lua 5.1.4  Copyright (C) 1994-2008 Lua.org, PUC-Rio
> require("notify")
> n1 = notify.new("Lua libnotify bind (level 1-> LOW)", "This is a test :D")
> notify.set_urgency(n1, 1)
> notify.set_appname(n1, "My Lua App")
> notify.show(n1);


Look at test/ folder to see an example using emergency levels, icons, etc..

To install lnotify, just copy notify.so to your Lua library path, usually /usr/local/lib/lua/5.x/ , like:

$ sudo cp notify.so /usr/local/lib/lua/5.1/


Now, you can call lnotify from any Lua script, enjoy!
