﻿/*
===========================================================================

  Copyright (c) 2010-2015 Darkstar Dev Teams

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/

===========================================================================
*/

#include "common/logging.h"

#include "../campaign_system.h"
#include "../entities/charentity.h"
#include "../entities/npcentity.h"
#include "../region.h"
#include "../zone.h"
#include "../zone_entities.h"
#include "lua_baseentity.h"
#include "lua_zone.h"
#include "../utils/mobutils.h"
#include "../mob_modifier.h"

CLuaZone::CLuaZone(CZone* PZone)
: m_pLuaZone(PZone)
{
    if (PZone == nullptr)
    {
        ShowError("CLuaZone created with nullptr instead of valid CZone*!");
    }
}


/************************************************************************
 *  Function: getLocalVar()
 *  Purpose : Returns a variable assigned locally to an entity
 *  Example : if (KingArthro:getLocalVar("[POP]King_Arthro") > 0) then
 *  Notes   :
 ************************************************************************/

auto CLuaZone::getLocalVar(const char* key)
{
    return m_pLuaZone->GetLocalVar(key);
}

/************************************************************************
 *  Function: setLocalVar()
 *  Purpose : Assigns a local variable to an entity
 *  Example : mob:setLocalVar("pop", os.time() + math.random(1200,7200));
 *  Notes   :
 ************************************************************************/

void CLuaZone::setLocalVar(const char* key, uint32 val)
{
    m_pLuaZone->SetLocalVar(key, val);
}

/************************************************************************
 *                                                                       *
 * Registering the active area in the zone                               *
 * Input data format: RegionID, x1, y1, z1, x2, y2, z2                   *
 *                                                                       *
 ************************************************************************/

void CLuaZone::registerRegion(uint32 RegionID, float x1, float y1, float z1, float x2, float y2, float z2)
{
    bool circleRegion = false;
    if (approximatelyEqual(x2, 0.0f) &&
        approximatelyEqual(y2, 0.0f) &&
        approximatelyEqual(z2, 0.0f))
    {
        circleRegion = true; // Parameters were 0, we must be a circle.
    }

    CRegion* Region = new CRegion(RegionID, circleRegion);

    // If this is a circle, parameter 3 (which would otherwise be vertical coordinate) will be the radius.
    Region->SetULCorner(x1, y1, z1);
    Region->SetLRCorner(x2, y2, z2);

    m_pLuaZone->InsertRegion(Region);
}

/************************************************************************
 *                                                                       *
 *  Setting the level limit for the zone                                 *
 *                                                                       *
 ************************************************************************/

sol::object CLuaZone::levelRestriction()
{
    return sol::lua_nil;
}

sol::table CLuaZone::getPlayers()
{
    auto table = luautils::lua.create_table();
    m_pLuaZone->ForEachChar([&table](CCharEntity* PChar) { table.add(CLuaBaseEntity(PChar)); });
    return table;
}

sol::table CLuaZone::getNPCs()
{
    auto table = luautils::lua.create_table();
    m_pLuaZone->ForEachNpc([&table](CNpcEntity* PNpc) { table.add(CLuaBaseEntity(PNpc)); });
    return table;
}

sol::table CLuaZone::getMobs()
{
    auto table = luautils::lua.create_table();
    m_pLuaZone->ForEachMob([&table](CMobEntity* PMob) { table.add(CLuaBaseEntity(PMob)); });
    return table;
}

ZONEID CLuaZone::getID()
{
    return m_pLuaZone->GetID();
}

std::string CLuaZone::getName()
{
    return reinterpret_cast<const char*>(m_pLuaZone->GetName());
}

REGION_TYPE CLuaZone::getRegionID()
{
    return m_pLuaZone->GetRegionID();
}

ZONE_TYPE CLuaZone::getType()
{
    return m_pLuaZone->GetType();
}

std::optional<CLuaBattlefield> CLuaZone::getBattlefieldByInitiator(uint32 charID)
{
    if (m_pLuaZone->m_BattlefieldHandler)
    {
        return std::optional<CLuaBattlefield>(m_pLuaZone->m_BattlefieldHandler->GetBattlefieldByInitiator(charID));
    }
    return std::nullopt;
}

bool CLuaZone::battlefieldsFull(int battlefieldId)
{
    return m_pLuaZone->m_BattlefieldHandler && m_pLuaZone->m_BattlefieldHandler->ReachedMaxCapacity(battlefieldId);
}

WEATHER CLuaZone::getWeather()
{
    return m_pLuaZone->GetWeather();
}

void CLuaZone::reloadNavmesh()
{
    m_pLuaZone->m_navMesh->reload();
}

std::optional<CLuaBaseEntity> CLuaZone::insertDynamicEntity(sol::table table)
{
    auto& lua = luautils::lua;

    CBaseEntity* PEntity = nullptr;
    if (table.get_or<uint8>("objtype", TYPE_NPC) == TYPE_NPC)
    {
        PEntity = new CNpcEntity();
        PEntity->name = "DefaultName";
    }
    else
    {
        auto groupId = table.get_or<uint32>("groupId", 0);
        auto groupZoneId = table.get_or<uint32>("groupZoneId", 0);

        PEntity = mobutils::InstantiateDynamicMob(groupId, groupZoneId, m_pLuaZone->GetID());
    }

    // NOTE: Mob allegiance is the default for NPCs
    PEntity->allegiance = static_cast<ALLEGIANCE_TYPE>(table.get_or<uint8>("allegiance", ALLEGIANCE_TYPE::MOB));

    uint16 ZoneID = m_pLuaZone->GetID();

    // TODO: Wrap this entity in a unique_ptr that will free this dynamic targ ID
    //       on despawn/destruction
    // TODO: The tracking of these IDs is pretty bad also, fix that in zone_entities
    PEntity->targid = m_pLuaZone->GetZoneEntities()->GetNewDynamicTargID();
    if (PEntity->targid >= 0x900)
    {
        ShowError("CLuaZone::insertDynamicEntity : targid is high (03hX), update packets will be ignored", PEntity->targid);
    }

    PEntity->id = 0x1000000 + (ZoneID << 12) + PEntity->targid;

    PEntity->loc.zone       = m_pLuaZone;
    PEntity->loc.p.rotation = table.get_or<uint8>("rotation", 0);
    PEntity->loc.p.x        = table.get_or<float>("x", 0.01);
    PEntity->loc.p.y        = table.get_or<float>("y", 0.01);
    PEntity->loc.p.z        = table.get_or<float>("z", 0.01);
    PEntity->loc.p.moving   = 0;

    auto name = table.get_or<std::string>("name", "");
    if (name.empty())
    {
        ShowWarning("Trying to spawn dynamic entity without a name! (%s - %s)",
            PEntity->name.c_str(), (const char*)m_pLuaZone->GetName());

        // If the name hasn't been provided, use "DefaultName" for NPCs, and whatever comes from the mob_pool for Mobs
        name = PEntity->name;
    }

    auto lookupName = "DE_" + name;

    PEntity->name = lookupName;
    PEntity->packetName = name;

    auto typeKey = (PEntity->objtype == TYPE_NPC) ? "npcs" : "mobs";
    auto cacheEntry = lua[sol::create_if_nil]["xi"]["zones"][(const char*)m_pLuaZone->GetName()][typeKey][lookupName];

    if (auto* PNpc = dynamic_cast<CNpcEntity*>(PEntity))
    {
        PNpc->namevis       = table.get_or<uint8>("namevis", 0);
        PNpc->status        = STATUS_TYPE::NORMAL;
        PNpc->m_flags       = 0;
        PNpc->name_prefix   = 32;

        // TODO: Does this even work?
        PNpc->widescan      = table.get_or<uint8>("widescan", 1);

        auto onTrade = table["onTrade"].get_or<sol::function>(sol::lua_nil);
        if (onTrade.valid())
        {
            cacheEntry["onTrade"] = onTrade;
        }

        auto onTrigger = table["onTrigger"].get_or<sol::function>(sol::lua_nil);
        if (onTrigger.valid())
        {
            PNpc->m_triggerable = true;
            cacheEntry["onTrigger"] = onTrigger;
        }

        m_pLuaZone->InsertNPC(PNpc);
    }
    else if (auto* PMob = dynamic_cast<CMobEntity*>(PEntity))
    {
        auto onMobDeath = table["onMobDeath"].get_or<sol::function>(sol::lua_nil);
        if (onMobDeath.valid())
        {
            cacheEntry["onMobDeath"] = onMobDeath;
        }
        else
        {
            cacheEntry["onMobDeath"] = [](){}; // Empty func
        }

        m_pLuaZone->InsertMOB(PMob);
    }

    if (table["look"].get_type() == sol::type::number)
    {
        PEntity->SetModelId(table.get<uint16>("look"));
    }
    else if (table["look"].get_type() == sol::type::string)
    {
        auto look = stringToLook(table.get<std::string>("look"));
        std::memcpy(&PEntity->look, &look, sizeof(PEntity->look));
    }

    PEntity->updatemask |= UPDATE_ALL_MOB;

    return CLuaBaseEntity(PEntity);
}

/************************************************************************
 *  Function: SetSoloBattleMusic(253)
 *  Purpose : Set Solo Battle music for zone
 ************************************************************************/

void CLuaZone::setSoloBattleMusic(uint8 musicId)
{
    m_pLuaZone->SetSoloBattleMusic(musicId);
}

auto CLuaZone::getSoloBattleMusic()
{
    return m_pLuaZone->GetSoloBattleMusic();
}

/************************************************************************
 *  Function: SetPartyBattleMusic(253)
 *  Purpose : Set Party Battle music for zone
 ************************************************************************/

void CLuaZone::setPartyBattleMusic(uint8 musicId)
{
    m_pLuaZone->SetPartyBattleMusic(musicId);
}

auto CLuaZone::getPartyBattleMusic()
{
    return m_pLuaZone->GetPartyBattleMusic();
}

/************************************************************************
 *  Function: SetBackgroundMusicDay(253)
 *  Purpose : Set Background Day music for zone
 ************************************************************************/

void CLuaZone::setBackgroundMusicDay(uint8 musicId)
{
    m_pLuaZone->SetBackgroundMusicDay(musicId);
}

auto CLuaZone::getBackgroundMusicDay()
{
    return m_pLuaZone->GetBackgroundMusicDay();
}

/************************************************************************
 *  Function: SetBackgroundMusicNight(253)
 *  Purpose : Set Background Night music for zone
 ************************************************************************/

void CLuaZone::setBackgroundMusicNight(uint8 musicId)
{
    m_pLuaZone->SetBackgroundMusicNight(musicId);
}

auto CLuaZone::getBackgroundMusicNight()
{
    return m_pLuaZone->GetBackgroundMusicNight();
}

sol::table CLuaZone::queryEntitiesByName(std::string const& name)
{
    TracyZoneScoped;

    auto table = luautils::lua.create_table();

    // TODO: Make work for instances
    // TODO: Replace with a constant-time lookup
    m_pLuaZone->ForEachNpc([&](CNpcEntity* PNpc)
    {
        if (std::string((const char*)PNpc->GetName()) == name)
        {
            table.add(CLuaBaseEntity(PNpc));
        }
    });

    m_pLuaZone->ForEachMob([&](CMobEntity* PMob)
    {
        if (std::string((const char*)PMob->GetName()) == name)
        {
            table.add(CLuaBaseEntity(PMob));
        }
    });

    if (table.empty())
    {
        ShowWarning("Query for entity name: %s in zone: %s returned no results", name, m_pLuaZone->GetName());
    }

    return table;
}

//======================================================//

void CLuaZone::Register()
{
    SOL_USERTYPE("CZone", CLuaZone);

    SOL_REGISTER("getLocalVar", CLuaZone::getLocalVar);
    SOL_REGISTER("setLocalVar", CLuaZone::setLocalVar);

    SOL_REGISTER("registerRegion", CLuaZone::registerRegion);
    SOL_REGISTER("levelRestriction", CLuaZone::levelRestriction);
    SOL_REGISTER("getPlayers", CLuaZone::getPlayers);
    SOL_REGISTER("getNPCs", CLuaZone::getNPCs);
    SOL_REGISTER("getMobs", CLuaZone::getMobs);
    SOL_REGISTER("getID", CLuaZone::getID);
    SOL_REGISTER("getName", CLuaZone::getName);
    SOL_REGISTER("getRegionID", CLuaZone::getRegionID);
    SOL_REGISTER("getType", CLuaZone::getType);
    SOL_REGISTER("getBattlefieldByInitiator", CLuaZone::getBattlefieldByInitiator);
    SOL_REGISTER("battlefieldsFull", CLuaZone::battlefieldsFull);
    SOL_REGISTER("getWeather", CLuaZone::getWeather);
    SOL_REGISTER("reloadNavmesh", CLuaZone::reloadNavmesh);
    SOL_REGISTER("insertDynamicEntity", CLuaZone::insertDynamicEntity);

    SOL_REGISTER("getSoloBattleMusic", CLuaZone::getSoloBattleMusic);
    SOL_REGISTER("getPartyBattleMusic", CLuaZone::getPartyBattleMusic);
    SOL_REGISTER("getBackgroundMusicDay", CLuaZone::getBackgroundMusicDay);
    SOL_REGISTER("getBackgroundMusicNight", CLuaZone::getBackgroundMusicNight);
    SOL_REGISTER("setSoloBattleMusic", CLuaZone::setSoloBattleMusic);
    SOL_REGISTER("setPartyBattleMusic", CLuaZone::setPartyBattleMusic);
    SOL_REGISTER("setBackgroundMusicDay", CLuaZone::setBackgroundMusicDay);
    SOL_REGISTER("setBackgroundMusicNight", CLuaZone::setBackgroundMusicNight);

    SOL_REGISTER("queryEntitiesByName", CLuaZone::queryEntitiesByName);
}

std::ostream& operator<<(std::ostream& os, const CLuaZone& zone)
{
    std::string id = zone.m_pLuaZone ? std::to_string(zone.m_pLuaZone->GetID()) : "nullptr";
    return os << "CLuaZone(" << id << ")";
}

//======================================================//
