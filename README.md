# Fstab helper
Program written using ftxui that help's you manage|setup fstab file.
If you remove|edit some entries from fstab, they will be stored in /etc/fstab_bak , so all disabled device values are recovered and showed on next start(if corresponding device plugged)
### Screenshot
Screenshot of a program           |  what it wrote to /etc/fstab
:-------------------------:|:-------------------------:
![screenshot_2024-02-12-003702](https://github.com/SAANN3/fstab-Helper/assets/95036865/d5c2c4d8-a943-40e6-841b-5d52687858e3) | ![screenshot_2024-02-12-003713](https://github.com/SAANN3/fstab-Helper/assets/95036865/ffbeb9a5-fbb8-480c-b041-cd4f6c176926)
## Installation
[Download](https://github.com/SAANN3/fstab-Helper/releases) a linux binary and then ```chmod +x fstabWrapper ``` it
## Usage
Because of writing into /etc/fstab , this program need to be run with root priveleges(sudo)
so run it with 
```
sudo ./fstabWrapper
```
### Or if you prefer to compile from source
run script 
```bash
git clone https://github.com/SAANN3/fstab-Helper
cd fstab-Helper
./build.sh
```
or write a little more commands
```bash
  git clone https://github.com/SAANN3/fstab-Helper
  cd fstab-Helper
  mkdir build && cd build
  cmake ..
  make
```
and program will be placed in ``` ./build/fstabWrapper```

