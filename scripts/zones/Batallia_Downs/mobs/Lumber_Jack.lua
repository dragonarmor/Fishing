-----------------------------------
-- Area: Batallia Downs (105)
--   NM: Lumber Jack
-----------------------------------
mixins = {require("scripts/mixins/job_special")}
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(xi.mobMod.ADD_EFFECT, 1)
    mob:setMobMod(xi.mobMod.IDLE_DESPAWN, 600)
end

entity.onAdditionalEffect = function(mob, target, damage)
    if mob:hasStatusEffect(xi.effect.ENSTONE) then
        return 0, 0, 0
    else
        return xi.mob.onAddEffect(mob, target, damage, xi.mob.ae.STUN)
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    -- Set Weeping Willow's respawn time (21-24 hours)
    GetMobByID(mob:getID() -6):setRespawnTime(math.random(75600, 86400))
end

return entity
