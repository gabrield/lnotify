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

n1 = notify.new("Lua libnotify bind (level 1-> LOW)", "This is a teste :D", path)
notify.set_urgency(n1, 1)
notify.show(n1);
sleep(2)

path = os.getenv("PWD") .. "/document-print.svg"
n2 = notify.new("Bye (level 3-> CRITICAL)", "See you...", path)
notify.set_urgency(n2, 3)
notify.show(n2)
sleep(2)


path = os.getenv("PWD") .. "/system-shutdown.svg"
n3 = notify.new("Cheers (level 2-> NORMAL)", "BRB", path)
notify.set_urgency(n3, 2)
notify.show(n3)
