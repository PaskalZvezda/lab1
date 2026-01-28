#include <iostream>
#include <string>
#include <array>
#include <cstdio>

// Исправляем имена функций для Windows
#ifdef _WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #define POPEN popen
    #define PCLOSE pclose
#endif

std::string getGithubVersion(const std::string& repo) {
    std::string command = "curl -s https://api.github.com/repos/" + repo + "/releases/latest";
    std::array<char, 128> buffer;
    std::string rawJson;

    // 1. Читаем весь ответ от GitHub в строку
    FILE* pipe = POPEN(command.c_str(), "r");
    if (!pipe) return "Ошибка запуска curl";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        rawJson += buffer.data();
    }
    PCLOSE(pipe);

    // 2. Ищем "tag_name" в тексте вручную (чтобы не зависеть от grep)
    std::string key = "\"tag_name\":\"";
    size_t startPos = rawJson.find(key);
    
    if (startPos == std::string::npos) {
        return "Версия не найдена (проверьте наличие релизов)";
    }

    startPos += key.length();
    size_t endPos = rawJson.find("\"", startPos);
    
    return rawJson.substr(startPos, endPos - startPos);
}

int main() {
    // Устанавливаем кодировку консоли, чтобы не было "кракозябр"
    system("chcp 65001 > nul"); 
    
    std::string repo = "nlohmann/json";
    std::cout << "Проверка репозитория: " << repo << "..." << std::endl;
    
    std::string version = getGithubVersion(repo);
    std::cout << "Последняя версия: " << version << std::endl;

    return 0;
}