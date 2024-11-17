#ifndef AUTH_H
#define AUTH_H

#include <unordered_map>
#include <string>

std::unordered_map<std::string, std::string> loadUsers(const std::string& filename);
std::string generateSalt();
std::string hashPassword(const std::string& salt, const std::string& password);

#endif // AUTH_H
