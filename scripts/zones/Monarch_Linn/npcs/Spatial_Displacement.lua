-----------------------------------
-- Area: Monarch_Linn
--  NPC: Spatial Displacement
-- !pos -35 -1 -539 31
-----------------------------------
local ID = require("scripts/zones/Monarch_Linn/IDs")
require("scripts/globals/bcnm")
require("scripts/globals/quests")
require("scripts/globals/missions")
require("scripts/globals/zone")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
    xi.bcnm.onTrade(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    local X = player:getXPos()

    if X > 12.934 and X < 24.934 then
        if player:getPreviousZone() == xi.zone.RIVERNE_SITE_A01 then
            player:startEvent(11) -- To Riv Site A
        elseif player:getPreviousZone() == xi.zone.RIVERNE_SITE_B01 then
            player:startEvent(10) -- To Riv Site B
        end
    -- TODO: Do we really want to only check X values here?
    elseif ((X > -524.521 and X < -512.521) or (X > 75.524 and X < 87.524) or (X > 675.271 and X < 687.271)) then
        player:startEvent(32003)  -- leave the battlefield
    elseif X > -25.684 and X < -13.684 then -- post-battlefield exit
        player:startEvent(7)
    elseif not xi.bcnm.onTrigger(player, npc) then
        player:messageSpecial(ID.text.GLOWING_MIST) -- needs confirmation
    end
end

entity.onEventUpdate = function(player, csid, option, extras)
    xi.bcnm.onEventUpdate(player, csid, option, extras)
end

entity.onEventFinish = function(player, csid, option)
    if csid == 11 and option == 1 then
        player:setPos(-508.582, -8.471, -387.670, 92, 30) -- To Riv Site A (Retail confirmed)
    elseif csid == 10 and option == 1 then
        player:setPos(-533.690, -20.5, 503.656, 224, 29) -- To Riv Site B (Retail confirmed)
    elseif csid == 7 and option == 1 then
        player:setPos(-538.526, -29.5, 359.219, 255, 25) -- back to Misareaux Coast (Retail confirmed)
    else
        xi.bcnm.onEventFinish(player, csid, option)
    end
end

return entity
