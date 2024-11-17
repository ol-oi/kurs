#ifndef INTERFACE_H
#define INTERFACE_H

#include <unordered_map>
#include <string>

void printHelp();
std::unordered_map<std::string, std::string> loadUsers(const std::string& filename);
void handleClient(int clientSocket, const std::unordered_map<std::string, std::string>& users);

#endif // INTERFACE_H
