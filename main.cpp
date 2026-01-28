#include <iostream>
#include <array>

int main() {
    std::string command = "curl -s -H \"User-Agent: C++-App\" https://api.github.com/repos/PaskalZvezda/lab1/releases/latest";
    std::array<char, 128> buffer;
    std::string rawJson;
    FILE* pipe = _popen(command.c_str(), "r");
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        rawJson += buffer.data();
    }
    _pclose(pipe);
    std::string key = "\"tag_name\":\"";
    size_t startPos = rawJson.find(key) + key.length();
    size_t endPos =  rawJson.find("\"", startPos) - startPos;
    std::cout << "Hello, World! Version: " << rawJson.substr(startPos, endPos) << std::endl;
    std::cin>> rawJson;
    return 0;
}