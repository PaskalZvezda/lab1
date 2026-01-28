#include <iostream>
#include <string>
#include <array>
#include <cstdio>

#ifdef _WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #define POPEN popen
    #define PCLOSE pclose
#endif

std::string getGithubVersion(const std::string& repo) {
    // Добавили заголовок User-Agent, это критично для GitHub API
    std::string command = "curl -s -H \"User-Agent: C++-App\" https://api.github.com/repos/" + repo + "/releases/latest";
    std::array<char, 128> buffer;
    std::string rawJson;

    FILE* pipe = POPEN(command.c_str(), "r");
    if (!pipe) return "Ошибка: не удалось запустить curl";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        rawJson += buffer.data();
    }
    PCLOSE(pipe);

    // Если GitHub вернул ошибку (например, 403), мы увидим это в rawJson
    if (rawJson.empty()) return "Ошибка: пустой ответ от сервера";

    // Ищем тег. В JSON это выглядит так: "tag_name":"1.0"
    std::string key = "\"tag_name\":\"";
    size_t startPos = rawJson.find(key);
    
    if (startPos == std::string::npos) {
        return "Версия не найдена. Ответ сервера: " + rawJson.substr(0, 50) + "...";
    }

    startPos += key.length();
    size_t endPos = rawJson.find("\"", startPos);
    
    return rawJson.substr(startPos, endPos - startPos);
}

int main() {
    system("chcp 65001 > nul"); 
    
    // Вставьте сюда свой путь: "PaskalZvezda/lab1"
    std::string repo = "PaskalZvezda/lab1"; 
    
    std::cout << "Проверка репозитория: " << repo << std::endl;
    
    std::string version = getGithubVersion(repo);
    std::cout << "Результат: " << version << std::endl;

    return 0;
}