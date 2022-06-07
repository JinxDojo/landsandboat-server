-----------------------------------
--
-- Zone: Cirdas Caverns U
--
-----------------------------------
local ID = require("scripts/zones/Cirdas_Caverns_U/IDs")
-----------------------------------
local zone_object = {}

zone_object.onInitialize = function(zone)
end

zone_object.onInstanceZoneIn = function(player, instance)
    local cs = -1

    if player:getInstance() == nil then
        player:setPos(0, 0, 0, 0, 72)
        return cs
    end

    local pos = player:getPos()
    if pos.x == 0 and pos.y == 0 and pos.z == 0 then
        local entrypos = instance:getEntryPos()
        player:setPos(entrypos.x, entrypos.y, entrypos.z, entrypos.rot)
    end

    return cs
end

zone_object.onRegionEnter = function(player, region)
end

zone_object.onEventUpdate = function(player, csid, option)
end

zone_object.onEventFinish = function(player, csid, option)
    if csid == 1000 then
        player:setPos(-210.2, 40.2, -447.3, 243, xi.zone.KAMIHR_DRIFTS)
    end
end

zone_object.onInstanceLoadFailed = function()
    return xi.zone.KAMIHR_DRIFTS
end

return zone_object
