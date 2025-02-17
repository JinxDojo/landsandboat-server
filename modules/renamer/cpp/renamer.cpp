﻿#include "../src/map/utils/moduleutils.h"
#include "../src/map/zone.h"

extern uint8 PacketSize[512];
extern std::function<void(map_session_data_t* const, CCharEntity* const, CBasicPacket)> PacketParser[512];

class RenamerModule : public CPPModule
{
    static void SendListPacket(CCharEntity* PChar, std::string const& data)
    {
        if (data.empty())
        {
            // Nothing to do, bail out
            return;
        }

        auto* customPacket = new CBasicPacket();
        customPacket->setType(0x1FF);
        customPacket->setSize(0x100);
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            customPacket->ref<uint8>(0x04 + i) = data[i];
        }
        PChar->pushPacket(customPacket);
    }

    static void Handle0x01Packet(map_session_data_t* const, CCharEntity* const PChar, CBasicPacket)
    {
        ShowInfo(fmt::format("{} requested renamer list for {}", PChar->GetName(), PChar->loc.zone->GetName()));

        auto zoneId = PChar->getZone();
        auto zoneTable = luautils::lua["xi"]["renamerTable"].get<sol::table>()[zoneId].get<sol::table>();

        std::string dataString;
        for (auto [key, value] : zoneTable)
        {
            auto entryTable = value.as<sol::table>();

            // convert entityId to targid
            auto entityId = entryTable[1].get<uint32>();
            auto targid = entityId - 0x1000000 - (zoneId << 12);
            auto entityName = entryTable[2].get<std::string>();
            auto packedString = fmt::format("{},{}.", targid, entityName);

            // If the dataString gets too large, send a packet with what we've
            // already prepared so we don't exceed the target size of 0x100.
            if (0x04 + dataString.size() + packedString.size() > 0x100)
            {
                SendListPacket(PChar, dataString);
                dataString.clear();
            }
            else
            {
                dataString += packedString;
            }
        }

        SendListPacket(PChar, dataString);
    }

    void OnInit() override
    {
        TracyZoneScoped;

        ShowInfo("Renamer: Loading ./modules/renamer/lua/list.lua");

        auto result = lua.safe_script_file("./modules/renamer/lua/list.lua", &sol::script_pass_on_error);
        if (!result.valid())
        {
            sol::error err = result;
            ShowError("Load error: %s", err.what());
            return;
        }

        if (!result.return_count())
        {
            ShowError("No returned renamer list object");
            return;
        }

        // NOTE: If we don't attach the result to something global, it will not be
        //     : correclty captured by the packet handling lambda.
        lua[sol::create_if_nil]["xi"]["renamerTable"] = result;

        // Add a custom packet handler to the PacketParser array for id 0x01
        PacketParser[0x01] = Handle0x01Packet;
    }
};

REGISTER_CPP_MODULE(RenamerModule);
