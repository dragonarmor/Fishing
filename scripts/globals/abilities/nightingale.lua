-----------------------------------
-- Ability: Nightingale
-- Game Description: Halves the casting and recast times of songs
-- Obtained: Bard Level 75 Group 2 Meriting
-- Recast Time: 0:10:00
-- Duration: 0:01:00
-----------------------------------
require("scripts/settings/main")
require("scripts/globals/status")
-----------------------------------
local ability_object = {}

ability_object.onAbilityCheck = function(player, target, ability)
    return 0, 0
end

ability_object.onUseAbility = function(player, target, ability)
    player:addStatusEffect(xi.effect.NIGHTINGALE, 0, 0, 60)
end

return ability_object
