#ifndef AUTH_H
#define AUTH_H

#include <unordered_map>
#include <string>

class Auth {
public:
    static std::unordered_map<std::string, std::string> loadUsers(const std::string& filename);
    static std::string generateSalt();
    static std::string hashPassword(const std::string& salt, const std::string& password);
};

#endif // AUTH_H
