#ifndef INTERFACE_H
#define INTERFACE_H

#include <unordered_map>
#include <string>

class Interface {
public:
    static void printHelp();
    static std::unordered_map<std::string, std::string> loadUsers(const std::string& filename);
    static void handleClient(int clientSocket, const std::unordered_map<std::string, std::string>& users);
};

#endif // INTERFACE_H
