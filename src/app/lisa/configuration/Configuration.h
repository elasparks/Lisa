#ifndef LISA_CONFIGURATION_H
#define LISA_CONFIGURATION_H

#include <cstdint>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class Configuration {
private:
    std::shared_ptr<uint16_t> serverPort;
    std::shared_ptr<uint16_t> poolSize;
    std::shared_ptr<std::string> dbHost;
    std::shared_ptr<uint16_t> dbPort;
    std::shared_ptr<std::string> dbName;
    std::shared_ptr<std::string> dbUser;
    std::shared_ptr<std::string> dbPassword;

    void loadConfiguration(const std::string& filePath);

    void restoreDefaultConfiguration();

    void saveConfiguration(const std::string& filePath) const;

public:
    explicit Configuration(const std::string& filePath);

    [[nodiscard]] nlohmann::json toJson() const;

    [[nodiscard]] const uint16_t &getServerPort() const;

    [[nodiscard]] const uint16_t &getPoolSize() const;

    [[nodiscard]] const std::string &getDbHost() const;

    [[nodiscard]] const uint16_t &getDbPort() const;

    [[nodiscard]] const std::string &getDbName() const;

    [[nodiscard]] const std::string &getDbUser() const;

    [[nodiscard]] const std::string &getDbPassword() const;
};


#endif
