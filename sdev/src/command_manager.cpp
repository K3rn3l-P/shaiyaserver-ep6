#define WIN32_LEAN_AND_MEAN
#include <thread>
#include <chrono>
#include <windows.h>
#include <include/main.h>
#include <util/util.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <include/shaiya/include/CWorld.h>
#include <include/shaiya/include/CZone.h>
#include <include/shaiya/include/SVector.h>
#include <include/shaiya/include/CGameData.h>
#include <include/shaiya/include/UserHelper.h>

using namespace shaiya;

namespace command_manager
{
    std::string output;

    int execute(const char* input, int cmd_case) {
        std::istringstream stream(input);
        std::vector<std::string> args;

        std::string temp;
        stream >> temp;
        while (stream >> temp) {
            args.push_back(temp);
        }

        switch (cmd_case) {
        case 69: // /mmake
        {
            if (args.size() < 6) {
                output = "Usage: /mmake <mapId> <mobId> <count> <x> <y> <z>";
                return 1;
            }

            auto map = std::stoi(args.at(0));
            auto mobId = std::stoi(args.at(1));
            auto count = std::stoi(args.at(2));
            auto x = std::stof(args.at(3));
            auto y = std::stof(args.at(4));
            auto z = std::stof(args.at(5));

            SVector position = { x, y, z };
            auto zone = CWorld::GetZone(map);

            // Genera mob e ottieni objectId
            uint objectId = CZone::MobGenEx(zone, 0, mobId, count, &position);

            // Log immediato
            std::stringstream output_format;
            output_format << "Mob:" << mobId << " (ID:" << objectId << ") spawned on Map:" << map;
            output = output_format.str();

            // [MODIFICA] Timer rimozione automatica mob generati da /mmake: cambia `60` in `std::chrono::seconds(X)`
            // Rimuovi automaticamente dopo 3600 secondi
            std::thread([zone, objectId]() {
                std::this_thread::sleep_for(std::chrono::seconds(3600)); // Cambia qui la durata futura
                CZone::MobRemoveById(zone, objectId);
                }).detach();

            return 0;
        }

        case 70: // /giveitem
        {
            if (args.size() < 4) {
                output = "Usage: /giveitem <charName> <itemType> <itemId> <count>";
                return 1;
            }

            const std::string& charName = args.at(0);
            auto itemType = std::stoi(args.at(1));
            auto itemId = std::stoi(args.at(2));
            auto count = std::stoi(args.at(3));

            CUser* user = CWorld::FindUser(charName.c_str());
            if (!user) {
                output = "Error: Unable to find Character!";
                return 1;
            }

            auto info = CGameData::GetItemInfo(itemType, itemId);
            if (!info) {
                output = "Error: Item invalid!";
                return 1;
            }

            int bag = 0, slot = 0;
            if (!UserHelper::ItemCreate(user, info, count, bag, slot)) {
                output = "Error: Item creation failed.";
                return 1;
            }

            std::stringstream output_format;
            output_format << "Item:" << itemId << " (" << count << "x) sent to " << charName;
            output = output_format.str();
            return 0;
        }
        case 71: // /mera
        {
            if (args.size() < 6) {
                output = "Usage: /mera <mapId> <mobId> <count> <x> <y> <z>";
                return 1;
            }

            auto map = std::stoi(args.at(0));
            auto mobId = std::stoi(args.at(1));
            auto count = std::stoi(args.at(2));
            auto x = std::stof(args.at(3));
            auto y = std::stof(args.at(4));
            auto z = std::stof(args.at(5));

            SVector position = { x, y, z };
            int cellX = static_cast<int>(x / 100.0f);
            int cellZ = static_cast<int>(z / 100.0f);

            if (!CZone::MobRemove(CWorld::GetZone(map), mobId, count, cellX, cellZ, &position)) {
                output = "Error: Mob removal failed.";
                return 1;
            }

            std::stringstream output_format;
            output_format << "Mob:" << mobId << " removed from Map:" << map;
            output = output_format.str();
            return 0;
        }
        default:
            break;
        }

        return 1;
    }
}

// Comandi da registrare
LPCSTR cmd_mmake = "/mmake";
LPCSTR cmd_giveitem = "/giveitem";
LPCSTR cmd_mera = "/mera";

unsigned u0x4F5BE0 = 0x4F5BE0;

void __declspec(naked) naked_0x409459() {
    __asm {
        // /mmake
        push 0x0
        push - 1
        push 0x6
        push cmd_mmake
        push 69
        mov ecx, esi
        call u0x4F5BE0

        // /giveitem
        push 0x0
        push - 1
        push 0x4
        push cmd_giveitem
        push 70
        mov ecx, esi
        call u0x4F5BE0

        // /mera
        push 0x0
        push - 1
        push 0x6
        push cmd_mera
        push 71
        mov ecx, esi
        call u0x4F5BE0

        pop esi
        retn
    }
}

unsigned u0x409498 = 0x409498;
unsigned u0x40B3D4 = 0x40B3D4;

void __declspec(naked) naked_0x40948F()
{
    __asm
    {
        pushad
        sub ebx, 0x4024
        push eax
        push ebx
        call command_manager::execute
        add esp, 8

        test eax, eax
        popad
        je write_output

        cmp eax, 0x44
        ja default_case
        jmp u0x409498

        write_output :
        push command_manager::output
            push edi
            call sprintf
            add esp, 8

            default_case :
            jmp u0x40B3D4
    }
}

namespace hook {
    void command_manager() {
        util::detour((void*)0x409459, naked_0x409459, 5);
        util::detour((void*)0x40948F, naked_0x40948F, 9);
    }
}
