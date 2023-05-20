# CanRemote
LoRa based remote control for sending messages to a CAN-Bus enabled car, equiped with a LoRa receiver.  
This project uses the following parts :

* A LuatOS ESP32-C3 board (for controller)
* A LuatOS Air101-LCD (for dev version only ! will change in the future)
* A IP5306 power module
* A E220-400T22D LoRa Module
* A USB-C female connector (power goes to IP5306, data to ESP32-C3)
 
## This Code is for now for the DEV board version
 
 
schematics for this DEV version remote are available here :
 
https://oshwlab.com/ulysse31/canremotebrick

This DEV version do NOT uses any PCB *on its own*, all parts are actually soldered with wires and tapped together

A final version is being built, it will NOT use the Air101-LCD, and this for power-consumption reason.  
The Air101-LCD is a tft, which, consumes already a lot more than an OLED screen, but even worse : in combination with the LuatOS ESP32-C3 board (fixed as a shield)  
IT IS IMPOSSIBLE TO POWER OFF THE BACKLIGHT OF THE TFT => which is of course a total no-go ...  

So the final version will use a 4 pin I2C OLED screen.  

PCB + Schematics for the final version is available here:  

https://oshwlab.com/ulysse31/canremotebrickv2_copy

Remote Control that goes in par with CanCarControl project:  

https://github.com/ulysse31/CanCarControl
