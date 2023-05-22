# CanRemote DEV VERSION
LoRa based remote control for sending messages to a CAN-Bus enabled car, equiped with a LoRa receiver.  
This project uses the same approach as the CanCarControl: a command line shell and config based file setup.  
The goal of this project is to play with the car, with a "DIY"/"Homemade" remote (other repo) in order to do stuff such as:  
  
* Locking/unlocking the car
* Get the GPS infos
* Enable/disable the warnings
* Opening closing the windows
* Get the windows and door status
* ...  

A GUI was added using a screen and a wheel mp3 button type (click, up, down button).  
The GUI interface elements is generated from config files dedicated to this purpose.  
This project uses the following parts :

* A LuatOS ESP32-C3 board (for controller)
* A LuatOS Air101-LCD (for dev version only ! will change in the future)
* A IP5306 power module
* A E220-400T22D LoRa Module
* A USB-C female connector (power goes to IP5306, data to ESP32-C3)
* A multi-directional wheel type button (eg: K1-1502SA-01 or other)
 
## This Code is for now for the DEV board version
This development version is not the "final" version, it was just a first step to validate the concept before producing the final remote version: you know what it feels to create a circuit and launch its production then spot a forgot/inconsistent/erroneous part in it ? well ... it's like "don't dev on prod" citation for physical stuff ^^'  
 
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


## How it works
As said earlier, this project is re-using the same code base as the CanCarControl project, since its aim is to communicate with it.  
So it uses the same shell and config class, as well as a similar (but not exactly the same) command listing:  
  | Command | Category | Description |
  | ------- | ------ | ----------------- |
  | cat | fs | show file content |
  | ls | fs | list files |
  | rm | fs | delete a file |
  | cp | fs | copy a file |
  | mv | fs | rename a file |
  | ed | fs | basic "per line file editor" (NO TERMCAPS/CURSOR ACTION, ONLY typing and BACKSPACE if needed) |
  | md5sum | sys | calculates the md5 of a file (useful for OTA updates) |
  | xmreceive | sys | a command to receive a file via xmodem protocol over the serial |
  | fwupdate | sys | proceed to OTA with the given file |
  | cfg | sys | command to manage main configuration parameters |
  | alias | sys | alias command / shortcuts management tool |
  | pin | sys | gpio pin management => allows to set a pin mode, read and/or write HIGH / LOW |
  | sleep | sys | command shortcut to manage sleep configuration parameters => after setting them you still must do a "cfg save" to keep settings at reboot |
  | free | sys | shows free memory (useful for hunting memory leaks) |
  | serial | sys | serial management tool => allow to connect from a serial to another serial, at a given speed ... |
  | exec | sys | execute commands from a given file |
  | delay | sys | waits x ms then ends ... |
  | restart | sys | restart the device |
  | menu | GUI | Graphical menu command management |
  | waitforkey | GUI | waits for a keypress, with optionally a timeout value in ms |
  | simulatekey | GUI | command line that simulates a keypress |
  | interactive | LoRa | enable/disable shell "interactiveness" (echo, verbose) => the lora serial uses a non-interactive shell ... |
  | lorasend | LoRa | sends a command to distant lora shell |
  | lorasecure | LoRa | enable / disables authentication system on local listening lora shell |
  
## System config
A basic configuration file named /etc/Can.cfg on the SPIFFS contains all the common variable settings.  
PLEASE REMEMBER THAT SPIFFS DO NOT SUPPORT DIRECTORIES...  so it is one file named /etc/Can.cfg yes ...  
Here is a list of parameters used actually:
  
  |    Variable Name    |   Value Type   |                        Description                        |
  | ------------------- | -------------- | --------------------------------------------------------- |
  | LoraSecure          |  true / false  | Boolean string to disable/enable auth in local lora shell |
  | LoraRemoteKey       | 16 char string | AES-128 bit string used as key by the remote device       |
  | LoraCarKey          | 16 char string | AES-128 bit string used as key by CanCarControl device    |
  | EnableSleep         |  true / false  | Boolean to enable / disable sleep feature                 |
  | InactivityTimeout   |  num. string   | number of seconds of inactivity before going to sleep     |
  
## How it (concretely) works
this "mini-system" uses variables and shell commands to do the needed actions, once you have on the receiver (CanCarControl) side setup a given action that you want to have it on the remote, you'll to :
 * check that it works via command line by running the command with lorasend  
 * add an element in the corresponding menu config file to call the command  
 * Test it ^^'  
  
Example: you added a "lock" command that locks the car in the CanCarControl device, you now want to call it on the remote:
 * Create a lock.sh file (with ed command), containing the command need to run:  
   >lorasend lock  
   >waitforkey 1000  

  
  waitforkey command will create a delay that allows to see output of the lorasend on screen.  
  
Then, either:  
 * edit the file /etc/Menu.cfg (base menu config called at boot), and add the line :  
  
    >lock=exec lock.sh  
  
Or:  
 * type the commands:  
    >menu set lock "exec lock.sh"  
    >menu save  
  
if you restart, the device, the item "lock" will appear on screen, and clicking on it will execute the command ...  
Here is a sample of a /etc/Menu.cfg:  
  
>lock=exec lock.sh  
>unlock=exec unlock.sh  
>open boot=exec openboot.sh  
>warning=exec warning.sh  
>ask GPS=exec askGPS.sh  
>Settings=menu load /etc/Settings.cfg  
>exit=sleep now  
  
As seen in the above sample, you can have sub menus by calling the command "menu load 'filename'" that will load the content of the given file as items to use by the GUI.  
Just remember to add inside the new GUI config file an item like:  
  
>Back=menu load /etc/Menu.cfg  
  
This will allow to come back to the initial menu ...  
