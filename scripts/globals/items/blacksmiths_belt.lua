-----------------------------------
-- ID: 15445
-- Item: Blacksmith's Belt
-- Enchantment: Synthesis image support
-- 2Min, All Races
-----------------------------------
-- Enchantment: Synthesis image support
-- Duration: 2Min
-- Smithing Skill +3
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    local result = 0
    if (target:hasStatusEffect(xi.effect.SMITHING_IMAGERY) == true) then
        result = 237
    end
    return result
end

item_object.onItemUse = function(target)
    target:addStatusEffect(xi.effect.SMITHING_IMAGERY, 3, 0, 120)
end

item_object.onEffectGain = function(target, effect)
    target:addMod(xi.mod.SMITH, 1)
end

item_object.onEffectLose = function(target, effect)
    target:delMod(xi.mod.SMITH, 1)
end

return item_object
