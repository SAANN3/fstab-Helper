#ifndef UI_H
#define UI_H
#include <memory> 
#include "ftxui/component/captured_mouse.hpp" 
#include "ftxui/component/component.hpp"     
#include "ftxui/component/component_base.hpp"  
#include "ftxui/component/component_options.hpp"  
#include "ftxui/component/screen_interactive.hpp"  
#include "ftxui/dom/elements.hpp" 
#include "ftxui/util/ref.hpp"  
#include <string>
#include <vector>

class Controller;
using namespace ftxui;
class Ui
{

public:
    Ui();
    
    struct DeviceEntry{
        std::string type = "";
        std::string device = "";
        std::string UUID = "";
        std::string label = "";
        std::string dir = "";
        std::string options = "";
        std::string dump = "";
        std::string fsck = "";
        bool isEnabled = false;
        Component UIdir ;
        Component UIoptions;
        Component UIdump ;
        Component UIfsck;
        Component UItype; 
        Component UIlabel;
        Component UIenabled;
        Components componentUI;
        

    };
    
    Controller *controller = nullptr;
    void setDeviceUI(std::vector<DeviceEntry> input);
    std::vector<DeviceEntry> getDeviceUI();
    void start();
private:
    bool warningShow = false;
    std::string warningText = " ";
    Component UIwrite;
    Component UIquit;
    std::vector<DeviceEntry> deviceUI;
    std::vector<Components> UIglobalComponent;
    ScreenInteractive screen = ScreenInteractive::Fullscreen();
    void quit();
    void write();
    
};
#endif