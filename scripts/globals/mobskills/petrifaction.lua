-----------------------------------
-- Petrifaction
-- Description: Petrifies opponent with a gaze attack
-- Type: Gaze
-- Utsusemi/Blink absorb: Ignores shadows
-- Range: Single gaze
-----------------------------------
require("scripts/globals/mobskills")
require("scripts/settings/main")
require("scripts/globals/status")
-----------------------------------
local mobskill_object = {}

mobskill_object.onMobSkillCheck = function(target, mob, skill)
    return 0
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)
    local typeEffect = xi.effect.PETRIFICATION
    skill:setMsg(xi.mobskills.mobGazeMove(mob, target, typeEffect, 1, 0, 25))
    return typeEffect
end

return mobskill_object
