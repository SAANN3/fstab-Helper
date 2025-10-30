#include "DeviceClass.h"
#include <filesystem>
#include <iostream>

DeviceClass::DeviceClass(std::string _UUID, std::string _label,std::string _device,std::string _type) : UUID(_UUID), label(_label), device(_device), type(_type)
{

}

void DeviceClass::setDump(std::string dump)
{
    if(!dump.empty()){
        this->dump = std::stoi(dump);
    }
    
}

void DeviceClass::setFsck(std::string fsck)
{
    if(!fsck.empty()){
        this->fsck = std::stoi(fsck);
    }
   
}

void DeviceClass::setIsEnabled(bool enabled)
{
    this->isEnabled = enabled;
}

void DeviceClass::setDir(std::string dir)
{

    if(dir.empty()){
        if(type == "swap"){
            this->dir = "none";
        }
        return;
    }
    if(!std::filesystem::exists(dir) && type != "swap"){
         std::filesystem::create_directories(dir);
    }
    
    this->dir = dir;
}

void DeviceClass::setOptions(std::string options)
{
    if(!options.empty()){
        this->options = options;
    }
}

DeviceClass::~DeviceClass()
{

}
std::string DeviceClass::getType()
{
    return this->type;
}
std::string DeviceClass::getDevice()
{
    return this->device;
}
std::string DeviceClass::getUUID()
{
    return this->UUID;
}
std::string DeviceClass::getLabel()
{
    return this->label;
}
std::string DeviceClass::getDir()
{
    return this->dir;
}
std::string DeviceClass::getOptions()
{
    return this->options;
}
std::string DeviceClass::getDump()
{
    return std::to_string(this->dump);
}
bool DeviceClass::getIsEnabled()
{
    return this->isEnabled;
}
std::string DeviceClass::getFsck()
{
    return std::to_string(this->fsck);
}
void DeviceClass::











write(std::ofstream &file,int force)
{
    
    if(!(isEnabled || (force && !dir.empty()))){
        return;
    }
    file << "UUID="<< UUID << " " ;
    file << dir << " " ;
    file << type << " " ;
    file << options << " "; 
    file << dump << " "; 
    file << fsck << std::endl;

}