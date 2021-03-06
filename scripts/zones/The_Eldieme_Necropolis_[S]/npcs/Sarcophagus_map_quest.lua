-----------------------------------
-- Area: The Eldieme Necropolis (S)
--  NPC: Sarcophagus
-- Type: Quest NPC
-- !pos  336.594, -33.500, -56.728 175
-----------------------------------
local ID = require("scripts/zones/The_Eldieme_Necropolis_[S]/IDs")
require("scripts/globals/keyitems")
require("scripts/settings/main")
require("scripts/globals/quests")
require("scripts/globals/npc_util")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    if
        player:getQuestStatus(xi.quest.log_id.CRYSTAL_WAR, xi.quest.id.crystalWar.LOST_IN_TRANSLOCATION) == QUEST_ACCEPTED
        and not player:hasKeyItem(xi.ki.RIGHT_MAP_PIECE)
    then
        player:startEvent(5)
    else
        player:messageSpecial(ID.text.SARCOPHAGUS_SEALED)
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
    if csid == 5 then
        npcUtil.giveKeyItem(player, xi.ki.RIGHT_MAP_PIECE)
    end
end

return entity
