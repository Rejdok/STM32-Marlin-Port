#a port of [Marlin 3D-printer firmware](https://github.com/MarlinFirmware/Marlin) to STM32 (stm32f4xx and stm32f1xx).

##supported:
* stm32f407
* stm32f103 (maple mini)


##requirements:
first you should install SW4STM32 (System Workbench For STM32) free software. the IDE/toolchain to open and compile this project.


##how to setup your own mk pins:
* (stm32f103. maple mini)
* * edit `Src/stm32f1xx/pin_map.c`,
* * change the ports/pins in the `pinMap` table
![maple mini (stm32f103) port/pin map](doku/maple_mini/Maple_Mini_Pinout.svg)


##how to upload firmware to your STM32:
* you may upload compiled firmware (.bin file) using SW4STM32.
* you may use console uploaders from the `tools\` folder


##note:
* to install STM32 windows driver run `tools/drivers/win/install_drivers.bat (thanks to [Roger Clark](https://github.com/rogerclarkmelbourne).)
* to upload by serial (ex. maple-mini. stm32f103) do from the root of this repo 
```
tools/linux/serial_upload stm32f103/MarlinPort.bin /dev/ttyUSB0
```
* resulting usb-com port in linuxes may be (ubuntu) /dev/ttyACM<number>
