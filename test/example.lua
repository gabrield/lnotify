-- #!/usr/bin/env lua


--[[
Simple script binding libnotify to Lua, quite easy ;)
It is still simple and there is no much resources, but usable.
--]]

require("notify")

-- sleep(n) ->  little trick
function sleep(length)
    local start = os.clock()
   
    while os.clock() - start < length do end
end

path = os.getenv("PWD") .. "/application-zip.svg"
notify.set_urgency(1)
notify.message("Lua libnotify bind (level 1-> LOW)", "This is a teste :D", path);
sleep(2)


notify.set_urgency(3)
path = os.getenv("PWD") .. "/document-print.svg"
notify.message("Bye (level 3-> CRITICAL)", "See you...", path);
sleep(2)


notify.set_urgency(2)
path = os.getenv("PWD") .. "/system-shutdown.svg"
notify.message("Cheers (level 2-> NORMAL)", "BRB", path);

