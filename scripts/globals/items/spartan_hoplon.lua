-----------------------------------
-- ID: 15170
-- Item: Spartan Hoplon
-- Item Effect: Phalanx
-----------------------------------
require("scripts/globals/status")
require("scripts/globals/msg")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return 0
end

item_object.onItemUse = function(target)
    if (target:addStatusEffect(xi.effect.PHALANX, 10, 0, 180)) then -- Retail potency unknown, 10 is a guess. (someone 1000 needles test this thing!)
        target:messageBasic(xi.msg.basic.GAINS_EFFECT_OF_STATUS, xi.effect.PHALANX)
    else
        target:messageBasic(xi.msg.basic.NO_EFFECT)
    end
end

return item_object
