#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string getVersionSimple(const std::string& repo) {
    std::array<char, 128> buffer;
    std::string result;
    
    // Команда: скачиваем JSON, ищем "tag_name", вырезаем значение между кавычек
    // Используем флаг -s (silent), чтобы не спамить прогресс-баром
    std::string command = "curl -s https://api.github.com/repos/" + repo + "/releases/latest | grep -m 1 \"tag_name\" | cut -d '\"' -f 4";

    // Открываем поток для чтения вывода команды
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
    
    if (!pipe) {
        return "Ошибка при запуске curl";
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // Убираем лишний символ переноса строки в конце
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result.empty() ? "Версия не найдена" : result;
}

int main() {
    std::string repo = "nlohmann/json";
    std::string version = getVersionSimple(repo);
    
    std::cout << "Последняя версия: " << version << std::endl;
    
    return 0;
}