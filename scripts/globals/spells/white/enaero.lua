-----------------------------------
-- Spell: Enaero
-----------------------------------
require("scripts/globals/status")
require("scripts/globals/magic")
-----------------------------------
local spell_object = {}

spell_object.onMagicCastingCheck = function(caster, target, spell)
    return 0
end

spell_object.onSpellCast = function(caster, target, spell)
    local effect = xi.effect.ENAERO
    doEnspell(caster, target, spell, effect)
    return effect
end

return spell_object
