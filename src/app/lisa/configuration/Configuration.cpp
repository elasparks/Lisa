#include "Configuration.h"

Configuration::Configuration(const std::string &filePath) {
    try {
        loadConfiguration(filePath);
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << std::endl;
        restoreDefaultConfiguration();
        saveConfiguration(filePath);
    }
    if (!serverPort || !poolSize || !dbHost || !dbPort || !dbName || !dbUser || !dbPassword) {
        std::cerr << "Server configuration file: missing json fields!" << std::endl;
        restoreDefaultConfiguration();
        saveConfiguration(filePath);
    }
}

void Configuration::loadConfiguration(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open() || file.bad() || file.fail())
        throw std::runtime_error("Can't find server configuration file!");
    nlohmann::json json;
    try {
        file >> json;
        file.close();
    } catch (...) {
        throw std::runtime_error("Server configuration file: JSON is corrupted!");
    }

    if (json.contains("serverPort") && json["serverPort"].is_number_unsigned())
        this->serverPort = std::make_shared<uint16_t>(json["serverPort"]);

    if (json.contains("poolSize") && json["poolSize"].is_number_unsigned())
        this->poolSize = std::make_shared<uint16_t>(json["poolSize"]);

    if (json.contains("dbHost") && json["dbHost"].is_string())
        this->dbHost = std::make_shared<std::string>(json["dbHost"]);

    if (json.contains("dbPort") && json["dbPort"].is_number_unsigned())
        this->dbPort = std::make_shared<uint16_t>(json["dbPort"]);

    if (json.contains("dbName") && json["dbName"].is_string())
        this->dbName = std::make_shared<std::string>(json["dbName"]);

    if (json.contains("dbUser") && json["dbUser"].is_string())
        this->dbUser = std::make_shared<std::string>(json["dbUser"]);

    if (json.contains("dbPassword") && json["dbPassword"].is_string())
        this->dbPassword = std::make_shared<std::string>(json["dbPassword"]);
}

void Configuration::restoreDefaultConfiguration() {
    std::cerr << "Restoring the default server configuration..." << std::endl;

    if (!this->serverPort)
        this->serverPort = std::make_shared<uint16_t>(1221);

    if (!this->poolSize)
        this->poolSize = std::make_shared<uint16_t>(10);

    if (!this->dbHost)
        this->dbHost = std::make_shared<std::string>("localhost");

    if (!this->dbPort)
        this->dbPort = std::make_shared<uint16_t>(5432);

    if (!this->dbName)
        this->dbName = std::make_shared<std::string>("lisa");

    if (!this->dbUser)
        this->dbUser = std::make_shared<std::string>("lisa");

    if (!this->dbPassword)
        this->dbPassword = std::make_shared<std::string>("1221");
}

void Configuration::saveConfiguration(const std::string &filePath) const {
    std::ofstream file(filePath);
    if (!file.is_open() || file.bad() || file.fail()) {
        std::cerr << "Can't save server configuration file!" << std::endl;
        return;
    }
    file << this->toJson().dump(4);
    file.close();
}

nlohmann::json Configuration::toJson() const {
    return {
            {"serverPort", *this->serverPort},
            {"poolSize",   *this->poolSize},
            {"dbHost",     *this->dbHost},
            {"dbPort",     *this->dbPort},
            {"dbName",     *this->dbName},
            {"dbUser",     *this->dbUser},
            {"dbPassword", *this->dbPassword}
    };
}

const uint16_t &Configuration::getServerPort() const {
    return *serverPort;
}

const uint16_t &Configuration::getPoolSize() const {
    return *poolSize;
}

const std::string &Configuration::getDbHost() const {
    return *dbHost;
}

const uint16_t &Configuration::getDbPort() const {
    return *dbPort;
}

const std::string &Configuration::getDbName() const {
    return *dbName;
}

const std::string &Configuration::getDbUser() const {
    return *dbUser;
}

const std::string &Configuration::getDbPassword() const {
    return *dbPassword;
}
