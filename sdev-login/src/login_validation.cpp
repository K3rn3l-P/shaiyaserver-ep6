#include <regex>
#include <fstream>
#include <ctime>
#include <string>
#include <cctype>
#include <windows.h>

// Log automatico nella stessa directory della DLL
void write_log(const std::string& message) {
    char path[MAX_PATH];
    HMODULE hm = NULL;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCSTR>(&write_log), &hm)) {
        GetModuleFileNameA(hm, path, sizeof(path));
        std::string log_path = path;
        log_path = log_path.substr(0, log_path.find_last_of("\\/")) + "\\login_validation.log";
        std::ofstream log(log_path, std::ios::app);
        if (log.is_open()) {
            std::time_t now = std::time(nullptr);
            char timebuf[32];
            ctime_s(timebuf, sizeof(timebuf), &now);
            log << timebuf << ": " << message << std::endl;
        }
    }
}

// Username: 4-18 caratteri, solo lettere/numeri/underscore
bool validate_username(const char* username) {
    std::regex re("^[A-Za-z0-9_]{4,18}$");
    return std::regex_match(username, re);
}

// Password: 8-20 caratteri, almeno 1 maiuscola, 1 minuscola, 1 numero, 1 speciale
bool validate_password(const char* password) {
    std::string pw(password);
    if (pw.length() < 8 || pw.length() > 20) return false;
    bool has_upper = false, has_lower = false, has_digit = false, has_special = false;
    for (char c : pw) {
        if (std::isupper((unsigned char)c)) has_upper = true;
        else if (std::islower((unsigned char)c)) has_lower = true;
        else if (std::isdigit((unsigned char)c)) has_digit = true;
        else if (strchr("@!#$%&*?_", c)) has_special = true;
        else return false; // carattere non consentito
    }
    return has_upper && has_lower && has_digit && has_special;
}

// Protezione SQL injection: vieta caratteri pericolosi
bool safe_sql_input(const char* str) {
    return str && !strpbrk(str, "'\";--\\/");
}