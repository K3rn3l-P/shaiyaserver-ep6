#pragma once
#include <string>

void write_log(const std::string& message);
bool validate_username(const char* username);
bool validate_password(const char* password);
bool safe_sql_input(const char* str);