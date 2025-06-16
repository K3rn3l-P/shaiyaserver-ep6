#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <util/util.h>
#include <shaiya/include/network/dbAgent/incoming/0400.h>
#include <shaiya/include/network/dbAgent/outgoing/0400.h>
#include "include/main.h"
#include "include/shaiya/include/CUser.h"
#include "include/shaiya/include/SConnection.h"
#include "include/shaiya/include/SDatabase.h"
#include "include/shaiya/include/SDatabasePool.h"
#include <regex>
#include <algorithm>
#include <cctype>
using namespace shaiya;

// Definizione fuori dal namespace
bool validate_character_name(const char* name) {
    if (!name) return false;
    std::regex re("^[A-Za-z0-9_]{3,13}$");
    if (!std::regex_match(name, re))
        return false;
    static const char* blacklist[] = { "admin", "gm", "mod", "shaiya", "support" };
    std::string lower(name);
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c); });
    for (const auto& bad : blacklist)
        if (lower.find(bad) != std::string::npos)
            return false;
    if (std::regex_search(name, std::regex(R"((.)\1{2,})")))
        return false;
    return true;
}

namespace user_character
{
    bool is_name_available(char* name)
    {
        if (!validate_character_name(name))
            return false;

        auto db = SDatabasePool::AllocDB();
        if (!db)
            return false;

        SDatabase::Prepare(db);

        std::string query("SELECT CharName FROM [PS_GameData].[dbo].[Chars] WHERE CharName=? AND Del=0;");
        if (SDatabase::PrepareSql(db, query.c_str()))
        {
            SDatabasePool::FreeDB(db);
            return false;
        }

        if (FAILED(SQLBindParameter(db->stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, std::strlen(name), 0, name, 19, nullptr)))
        {
            SDatabasePool::FreeDB(db);
            return false;
        }

        if (SDatabase::ExecuteSql(db))
        {
            SDatabasePool::FreeDB(db);
            return false;
        }

        long rowCount = SQL_NULL_DATA;
        if (FAILED(SQLRowCount(db->stmt, &rowCount)))
        {
            SDatabasePool::FreeDB(db);
            return false;
        }

        SDatabasePool::FreeDB(db);
        return !rowCount;
    }

    /// <summary>
    /// Handles incoming 0x40D packets.
    /// </summary>
    void handler_0x40D(CUser* user, DBAgentCharNameAvailableIncoming* incoming)
    {
        incoming->name[incoming->name.size() - 1] = '\0';
        bool available = is_name_available(incoming->name.data());

        if (!user->connection)
            return;

        DBAgentCharNameAvailableOutgoing outgoing{};
        outgoing.billingId = user->billingId;
        outgoing.available = available;
        SConnection::Send(user->connection, &outgoing, sizeof(DBAgentCharNameAvailableOutgoing));
    }
}

unsigned u0x4061D9 = 0x4061D9;
void __declspec(naked) naked_0x4061D3()
{
    __asm
    {
        // original
        add edx,-0x402
        cmp edx,0xB
        je case_0x40D
        jmp u0x4061D9

        case_0x40D:
        pushad

        push eax // packet
        push ecx // user
        call user_character::handler_0x40D
        add esp,0x8

        popad

        mov al,0x1
        retn
    }
}

void hook::user_character()
{
    // CUser::PacketUserChar switch
    util::detour((void*)0x4061D3, naked_0x4061D3, 6);
}
