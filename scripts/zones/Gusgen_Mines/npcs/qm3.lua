-----------------------------------
-- Area: Gusgen Mines
--  NPC: qm3 (???)
-- Involved In Quest: Healing the Land
-- !pos -168 1 311 196
-----------------------------------
require("scripts/settings/main")
require("scripts/globals/keyitems")
require("scripts/globals/quests")
local ID = require("scripts/zones/Gusgen_Mines/IDs")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    local HealingTheLand = player:getQuestStatus(xi.quest.log_id.SANDORIA, xi.quest.id.sandoria.HEALING_THE_LAND)

    if (HealingTheLand == QUEST_ACCEPTED and player:hasKeyItem(xi.ki.SEAL_OF_BANISHING) == true) then
        player:delKeyItem(xi.ki.SEAL_OF_BANISHING)
        player:messageSpecial(ID.text.FOUND_LOCATION_SEAL, xi.ki.SEAL_OF_BANISHING)
    elseif (HealingTheLand == QUEST_ACCEPTED and player:hasKeyItem(xi.ki.SEAL_OF_BANISHING) == false) then
        player:messageSpecial(ID.text.IS_ON_THIS_SEAL, xi.ki.SEAL_OF_BANISHING)
    else
        player:messageSpecial(ID.text.LETTERS_IS_WRITTEN_HERE)
    end
end

entity.onEventUpdate = function(player, csid, option)
    -- printf("CSID2: %u", csid)
    -- printf("RESULT2: %u", option)
end

entity.onEventFinish = function(player, csid, option)
end

return entity
