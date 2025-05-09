#pragma once

#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <string>

namespace shaiya {
    struct SVector;
}

namespace command_manager {
    // Messaggio di output generico per comandi
    extern std::string output;

    /**
     * Esegue un comando di console.
     * @param input  La stringa completa di input del comando (incluso nome comando).
     * @param cmd_case  L'identificatore numerico del comando.
     * @return 0 se il comando è stato eseguito con successo, 1 altrimenti.
     */
    int execute(const char* input, int cmd_case);
}

#endif // COMMAND_MANAGER_H
