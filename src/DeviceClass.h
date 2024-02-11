#ifndef DEVICECLASS_H
#define DEVICECLASS_H
#include <string>
#include <fstream>
class DeviceClass
{
    
private:
    
    
    const std::string type;
    const std::string device;
    const std::string UUID;
    const std::string label;
    std::string dir = "";
    std::string options = "defaults";
    int dump = 0;
    int fsck = 0;

    bool isEnabled = false;
public:
    void setDir(std::string dir);
    void setOptions(std::string options);
    void setDump(std::string dump);
    void setFsck(std::string fsck);
    void setIsEnabled(bool enabled);
    std::string getType();
    std::string getDevice();
    std::string getUUID();
    std::string getLabel();
    std::string getDir();
    std::string getOptions();
    std::string getDump();
    std::string getFsck();
    bool getIsEnabled();
    void write(std::ofstream &file,int force = 0);

    DeviceClass(std::string _UUID,std::string _label,std::string _device,std::string _type);
    ~DeviceClass();
};
#endif