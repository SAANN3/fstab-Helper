#include "Ui.h"
#include "ftxui-grid-container/grid-container.hpp"
#include <ftxui/dom/table.hpp>  
#include "Controller.h"
Ui::Ui(/* args */)
{
    UIquit = Button("Quit",[&]{this->quit();}) ;
    UIwrite = Button("Write",[&]{this->write();});
}

void Ui::start()
{
  UIglobalComponent.push_back({UIwrite,UIquit});
  auto grid =  GridContainer(UIglobalComponent);
  auto render = Renderer(grid,[&]{
    Component warning = Renderer([warningText = warningText]{return text(warningText)|border|center;});
    warning = Maybe(warning,&warningShow);
    std::vector<Elements> renders;
    renders.push_back({
        text("Enabled"),
        text("Label") | flex,
        text("Mount Directory") | flex,
        text("File system type") | flex,
        text("options") ,
        text("dump"),
        text("Filesystem checks"),

    });
    for(int i = 0;i < UIglobalComponent.size()-1;i++){
      Elements tmp =
            {deviceUI[i].UIenabled->Render(),
            deviceUI[i].UIlabel->Render(),
            deviceUI[i].UIdir->Render(),
            deviceUI[i].UItype->Render(),
            deviceUI[i].UIoptions->Render(),
            deviceUI[i].UIdump->Render(),
            deviceUI[i].UIfsck->Render()};
      renders.push_back(tmp);
    }
    auto table = Table({renders});
    table.SelectAll().Border(LIGHT);
    table.SelectRow(0).SeparatorVertical(LIGHT);
    table.SelectRow(0).Border(DOUBLE);
    table.SelectColumn(3).DecorateCells(center);
    table.SelectColumns(5,6).DecorateCells(center);
    return vbox({table.Render(),warning->Render(),hbox({UIwrite->Render()|flex,UIquit->Render()|flex})});
});

  screen.Loop(render);
}
void Ui::setDeviceUI(std::vector<DeviceEntry> input)
{
    this->deviceUI = input;
    for(int i = 0;i<deviceUI.size();i++) {
        deviceUI[i].UIenabled = Checkbox("[]",&deviceUI[i].isEnabled);
        deviceUI[i].UIdir = Input(&deviceUI[i].dir, deviceUI[i].dir);
        deviceUI[i].UIoptions = Input(&deviceUI[i].options,deviceUI[i].options);
        deviceUI[i].UIdump = Input(&deviceUI[i].dump,deviceUI[i].dump);
        deviceUI[i].UIfsck = Input(&deviceUI[i].fsck,deviceUI[i].fsck);
        if(deviceUI[i].label.empty()){
          deviceUI[i].UIlabel = Renderer([label = deviceUI[i].device]{return text(label);});
        } else{
          deviceUI[i].UIlabel = Renderer([label = deviceUI[i].label]{return text(label);});
        }
        deviceUI[i].UIdir |= CatchEvent([&](Event event) {
          return event.character() == "\n";
        });
        deviceUI[i].UIoptions |= CatchEvent([&](Event event) {
          return event.character() == "\n";
        });
        deviceUI[i].UIdump |= CatchEvent([&](Event event) {
          return event.character() == "\n";
        });
        deviceUI[i].UIfsck |= CatchEvent([&](Event event) {
          return event.character() == "\n";
        });
        deviceUI[i].UItype = Renderer([type = deviceUI[i].type]{return text(type);});
        deviceUI[i].componentUI =
            { deviceUI[i].UIenabled,
            deviceUI[i].UIdir,
            deviceUI[i].UIlabel,
            deviceUI[i].UItype,
            deviceUI[i].UIoptions,
            deviceUI[i].UIdump,
            deviceUI[i].UIfsck,};
        UIglobalComponent.push_back(deviceUI[i].componentUI);
        
    }
    
    
}
void Ui::write()
{
  for(auto &i : deviceUI){
      if(i.dir.empty() && i.isEnabled){
        warningShow = true;
        warningText = "Mount directory for " + (i.label.empty()?i.device:i.label) + " is empty,can't write";
        return;
      }
      if(!i.dir.empty() && i.dir[0]!='/' && i.type != "swap"){
        warningShow = true;
        warningText = "Mount directory for " + (i.label.empty()?i.device:i.label) + " starts not with /,can't write";
        return;
      }
      if(!(i.fsck == "0" || i.fsck == "1" || i.fsck == "2")){
        warningShow = true;
        warningText = "Filesystem checks for " +(i.label.empty()?i.device:i.label) + " not in range of [0,1,2] ,can't write";
        return;
      }
      if(i.options.empty()){
        warningShow = true;
        warningText = "Options for " + (i.label.empty()?i.device:i.label) + " is empty,can't write";
        return;
      }
      if(i.dump.empty()){
        warningShow = true;
        warningText = "Dump for " + (i.label.empty()?i.device:i.label) + " is empty,can't write";
        return;
      }
    
    
    warningShow = false;
  } 
  controller->write();
  warningShow = true;
  warningText = "Wrote";
  
  
}
void Ui::quit()
{
  screen.ExitLoopClosure();
  screen.Exit();
}
std::vector<Ui::DeviceEntry> Ui::getDeviceUI()
{
    return this->deviceUI;
}
