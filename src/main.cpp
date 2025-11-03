
#include "DeviceClass.h"
#include "Controller.h"
#include <unistd.h>
void checkRoot(){
  if(getuid()){
    std::cout << "Because of writing into /etc/fstab this program can't be runned without root" << std::endl;
    exit(1);
  }
  
}
int main() {
  checkRoot();
  Controller *controller  = new Controller();
  return 0;
}