#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <map>
#include "DeviceClass.h"
#include "Ui.h"
#include <vector>

class Controller
{
    
private:
    struct InfoContainer{
        std::string mountDir;
        std::string options;
        std::string dump;
        std::string fsck;
        bool enabled = false;
    };
    std::map<std::string,std::string> getDevices(std::vector<std::string> input);
    std::map<std::string,std::string> getDevices(std::vector<std::string> devices, std::vector<std::string> input);
    std::vector<DeviceClass*> devicesClasses;
    std::vector<std::string> listFiles(std::string path);
    std::vector<std::string> getType(std::vector<std::string> devices);
    std::vector<std::string> extractDevice(std::vector<std::string> input);
    std::map<std::string,InfoContainer> readFstab(std::string path);
    std::vector<Ui::DeviceEntry> getTUIlook();
    void updateDevices(std::vector<Ui::DeviceEntry> input);
    Ui* TUI;
public:
    void write();
    Controller();
    
};

#endif