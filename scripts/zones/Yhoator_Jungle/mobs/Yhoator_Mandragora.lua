-----------------------------------
-- Area: Yhoator Jungle
--  Mob: Yhoator Mandragora
-----------------------------------
require("scripts/globals/regimes")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    xi.regime.checkRegime(player, mob, 130, 1, xi.regime.type.FIELDS)
end

return entity
