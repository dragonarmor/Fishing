-----------------------------------
-- Area: Davoi
--  NPC: Jar
-- Involved in Quest: Test my Mettle
-- Notes: Used to obtain Power Sandals
-- !pos 183, 0, -190 149
-----------------------------------
require("scripts/settings/main")
local ID = require("scripts/zones/Davoi/IDs")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    local POWER_SANDALS = 13012

    -- Give Player Power Sandals if they don't have them
    if (player:hasItem(POWER_SANDALS) == false) then
        if (player:getFreeSlotsCount() == 0) then
            player:messageSpecial(ID.text.ITEM_CANNOT_BE_OBTAINED, POWER_SANDALS)
        else
            player:addItem(POWER_SANDALS)
            player:messageSpecial(ID.text.ITEM_OBTAINED, POWER_SANDALS)
        end
    else
        player:messageSpecial(ID.text.NOTHING_OUT_OF_ORDINARY)
    end
end

entity.onEventUpdate = function(player, csid, option)
    -- printf("CSID2: %u", csid)
    -- printf("RESULT2: %u", option)
end

entity.onEventFinish = function(player, csid, option)
end

return entity
