-----------------------------------
-- Spell: BARFIRE
-----------------------------------
require("scripts/globals/spells/barspell")
require("scripts/globals/status")
-----------------------------------
local spell_object = {}

spell_object.onMagicCastingCheck = function(caster, target, spell)
    return 0
end

spell_object.onSpellCast = function(caster, target, spell)
    return applyBarspell(xi.effect.BARFIRE, caster, target, spell)
end

return spell_object
