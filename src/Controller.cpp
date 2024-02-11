#include "Controller.h"
#include <filesystem>
#include <iostream>
#include <blkid/blkid.h>
#include <fstream>
const std::string  uuidPath = "/dev/disk/by-uuid/";
const std::string  labelPath = "/dev/disk/by-label";
const std::string  dirPath = "/proc/mounts";
const std::string fstabPath = "/etc/fstab";
const std::string backFstabPath = "/etc/fstab_bak";
std::vector<std::string> Controller::listFiles(std::string path)
{
    std::vector<std::string> files;
    for (const auto & entry : std::filesystem::directory_iterator(path))
        files.push_back(entry.path());
    return files;
}
std::vector<std::string> Controller::getType(std::vector<std::string> devices)
{
    for(int i = 0; i < devices.size();i++){
        blkid_probe pr = blkid_new_probe_from_filename(devices[i].c_str());
        if (!pr) {
            std::cout << "Can't open " + devices[i] + " aborting..." << std::endl;
            exit(-1);
        }
        blkid_do_probe(pr);
        const char *fsType;
        blkid_probe_lookup_value(pr,"TYPE",&fsType,NULL);
        devices[i] = fsType;
        blkid_free_probe(pr);
    }
    return devices;
}
std::vector<std::string> Controller::extractDevice(std::vector<std::string> input)
{
    for(int i = 0;i<input.size();i++){
        input[i]  =  std::filesystem::read_symlink(input[i]);
        input[i]  = input[i].substr(input[i].find_last_of("/"));
        input[i] = "/dev" + input[i];
    }
    return input;
}
std::map<std::string, std::string> Controller::getDevices(std::vector<std::string> input)
{
    
    std::vector<std::string> devices = extractDevice(input);
    for(auto &val:input){
        val = val.substr(val.find_last_of("/")+1);
    }
    return getDevices(devices,input);

}

std::map<std::string,std::string> Controller::getDevices(std::vector<std::string> devices, std::vector<std::string> input)
{
    std::map<std::string,std::string> map;
    for(int i = 0; i<input.size();i++)
    {
        map.insert(std::make_pair(devices[i],input[i]));
    }
    return map;
}

std::map<std::string,Controller::InfoContainer> Controller::readFstab(std::string path)
{
    std::map<std::string,InfoContainer> output;
    std::fstream file(path);
    std::string line;
    while(getline(file,line)){
        std::vector<std::string> separator;
        if(line.empty()){continue;}
        if(line[0] == '#' ){continue;}
        if(line.find("UUID=")==line.npos){continue;}
        int pos1 = 5;
        int pos2 = 5;
        for(int i = 5;i<=line.size();i++){
            if(line[i] == ' ' || line[i]=='\t' || i == (line.size())){ 
                pos2 = i-pos1;
                std::string tmp = line.substr(pos1,pos2);
                pos1 = i+1;
                if(tmp.empty()||tmp[0]=='\t'){continue;}
                separator.push_back(tmp);

            }

        }
        InfoContainer container;
        container.mountDir = separator[1];
        container.options = separator[3];
        container.dump = separator[4];
        container.fsck = separator[5];
        container.enabled = 1;
        output.insert(std::make_pair(separator[0],container));
        
  }
  file.close();
  return output;
}
std::vector<Ui::DeviceEntry> Controller::getTUIlook()
{
    
    std::vector<Ui::DeviceEntry> output;
    for(int i = 0;i < devicesClasses.size();i++){
        Ui::DeviceEntry deviceEntry;
        deviceEntry.type = devicesClasses[i]->getType();
        deviceEntry.device = devicesClasses[i]->getDevice();
        deviceEntry.UUID = devicesClasses[i]->getUUID();
        deviceEntry.label = devicesClasses[i]->getLabel();
        deviceEntry.dir = devicesClasses[i]->getDir();
        deviceEntry.options = devicesClasses[i]->getOptions();
        deviceEntry.dump = devicesClasses[i]->getDump();
        deviceEntry.fsck = devicesClasses[i]->getFsck();
        deviceEntry.isEnabled = devicesClasses[i]->getIsEnabled();
        output.push_back(deviceEntry);
    }
    return output;
}
Controller::Controller()
{
    
    std::map<std::string,std::string> allLabels = getDevices(listFiles(labelPath));
    std::map<std::string,std::string> allUUIDs = getDevices(listFiles(uuidPath));
    std::vector<std::string> devices; for(auto const& i:allUUIDs){devices.push_back(i.first);}
    std::map<std::string,std::string> allTypes = getDevices(devices,getType(devices));
    std::map<std::string,Controller::InfoContainer> fstabData;
    for(int i;i<devices.size();i++){
        devicesClasses.push_back(new DeviceClass(allUUIDs[devices[i]],allLabels[devices[i]],devices[i],allTypes[devices[i]]));
    }
    if(std::filesystem::exists(backFstabPath)){
        fstabData = readFstab(backFstabPath);
        for(int i = 0;i<fstabData.size();i++){
            devicesClasses[i]->setDir(fstabData[allUUIDs[devices[i]]].mountDir);
            devicesClasses[i]->setOptions(fstabData[allUUIDs[devices[i]]].options);
            devicesClasses[i]->setFsck(fstabData[allUUIDs[devices[i]]].fsck);
            devicesClasses[i]->setDump(fstabData[allUUIDs[devices[i]]].dump);
            devicesClasses[i]->setIsEnabled(fstabData[allUUIDs[devices[i]]].enabled);
        }
    }
    fstabData = readFstab(fstabPath);
    for(int i = 0;i<devices.size();i++){
        devicesClasses[i]->setDir(fstabData[allUUIDs[devices[i]]].mountDir);
        devicesClasses[i]->setOptions(fstabData[allUUIDs[devices[i]]].options);
        devicesClasses[i]->setFsck(fstabData[allUUIDs[devices[i]]].fsck);
        devicesClasses[i]->setDump(fstabData[allUUIDs[devices[i]]].dump);
        devicesClasses[i]->setIsEnabled(fstabData[allUUIDs[devices[i]]].enabled);
    }
    
    TUI = new Ui();
    TUI->controller = this;
    TUI->setDeviceUI(getTUIlook());
    TUI->start();
}
void Controller::write()
{
    
    std::ofstream file(fstabPath);
    std::ofstream fileBak(backFstabPath);
    updateDevices(TUI->getDeviceUI());
    for(int i = 0;i<devicesClasses.size();i++){
        devicesClasses[i]->write(file);
        devicesClasses[i]->write(fileBak,1);
    }
    fileBak.close();
    file.close();
}
void Controller::updateDevices(std::vector<Ui::DeviceEntry> input)
{
    for(int i = 0;i<input.size();i++){
        devicesClasses[i]->setDir(input[i].dir);
        devicesClasses[i]->setDump(input[i].dump);
        devicesClasses[i]->setFsck(input[i].fsck);
        devicesClasses[i]->setIsEnabled(input[i].isEnabled);
        devicesClasses[i]->setOptions(input[i].options);
    }
}