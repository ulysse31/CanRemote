#ifndef __CANREMOTE_H__
# define __CANREMOTE_H__

# include <stdint.h>
//# include <WProgram.h>
# include <Arduino.h>
# include <SPI.h>
# include <FS.h>
# include <SPIFFS.h>
# include <Wire.h>
# include <Update.h>
# include "mbedtls/aes.h"
# include "soc/soc.h" //disable brownour problems
# include "soc/rtc_cntl_reg.h" //disable brownour problems
# include <Adafruit_ST7735.h>
# include "ESPMoticsCfg.h"
# include "authToken.h"
# include "espShell.h"
# include "result.h"
# include "xmodem_receiver.h"
# include "cppfix.h"
# include "SC_GUI.h"
# include "tftSerial.h"

# define FORMAT_SPIFFS_IF_FAILED true

# define CFGFILE "Can.cfg"
# define ALIASFILE "Aliases.cfg"
# define MENUFILE "Menu.cfg"


# define DEBUGMODE
//# define DEBUG_MODE	1

//# define SERIAL_DEFAULT_SPEED	57600
//# define SERIAL_DEFAULT_SPEED	38400
# define SERIAL_DEFAULT_SPEED	115200
//# define SERIAL_DEFAULT_SPEED	230400
//# define SERIAL_DEFAULT_SPEED	460800
# define LORA_SERIAL		Serial1
# define LORA_SETUP_BAUD	9600
/*
# define LUATOS_SCK		2
# define LUATOS_MOSI		3
# define LUATOS_MISO		10
# define LUATOS_SS		7
# define LUATOS_RX1		1
# define LUATOS_TX1		0
# define LUATOS_RX0		20
# define LUATOS_TX0		21
# define LUATOS_LED1		12
# define LUATOS_LED2		13

# define MCP_PWR		8
# define MCP_INT		12
# define MCP_CS			LUATOS_SS
# define E220_AUX		5
# define E220_M0		6
# define E220_M1		4
*/
# define LUATOS_SCK		GPIO_NUM_2
# define LUATOS_MOSI		GPIO_NUM_3
# define LUATOS_MISO		GPIO_NUM_10
# define LUATOS_SS		GPIO_NUM_7
# define LUATOS_RX1		GPIO_NUM_1
# define LUATOS_TX1		GPIO_NUM_0
# define LUATOS_RX0		GPIO_NUM_20
# define LUATOS_TX0		GPIO_NUM_21
# define LUATOS_LED1		GPIO_NUM_12
# define LUATOS_LED2		GPIO_NUM_13


# define E220_PWR		GPIO_NUM_12

# define KEY_UP			GPIO_NUM_8
# define KEY_DOWN		GPIO_NUM_5
# define KEY_LEFT		GPIO_NUM_9
# define KEY_RIGHT		GPIO_NUM_13
# define KEY_CENTER		GPIO_NUM_4

# define TFT_CS			GPIO_NUM_7 // PyBadge/PyGamer display control pins: chip select
# define TFT_RST		GPIO_NUM_10 // Display reset
# define TFT_DC			GPIO_NUM_6 // Display data/command select
# define TFT_BACKLIGHT		GPIO_NUM_11 // Display backlight pin


// 3 minutes for now
//# define ACTIVITY_TIMEOUT	20000
// for debug
# define ACTIVITY_TIMEOUT	9990000

# ifndef DEBUGMODE
#  define CMDSERIAL	Serial
# endif

#ifdef DEBUGMODE
# define DBGSERIAL	Serial
# define CMDSERIAL	DBGSERIAL
# define DEBUGINIT	DBGSERIAL.begin(9600)
# define PRINTDBG(x) do { DBGSERIAL.print("["); DBGSERIAL.print(__FILE__); DBGSERIAL.print(":"); DBGSERIAL.print(__LINE__); DBGSERIAL.print("] "); DBGSERIAL.println(x); } while (false)
#else
# define PRINTDBG(x)
#endif

# define CMDINIT	CMDSERIAL.begin(SERIAL_DEFAULT_SPEED)
# define PRINTCMD(x) do { CMDSERIAL.print("["); CMDSERIAL.print(__FILE__); CMDSERIAL.print(":"); CMDSERIAL.print(__LINE__); CMDSERIAL.print("] "); CMDSERIAL.println(x); } while (false)

class Canremote;



# ifdef _DECLARE_GLOBAL_CANREMOTE_
ESPMoticsCfg            CanRemoteCfg(CFGFILE, '=');
ESPMoticsCfg            Aliases(ALIASFILE, '=');
ESPMoticsCfg            Menu(MENUFILE, '=');
espShell *		shell;
espShell *		shellScreen;
espShell *		shellLoRa;
unsigned long		LastActivity;
SC_GUI			RemoteGUI;
tftSerial		SCSerial;
unsigned int		actionPin;
#  else
extern ESPMoticsCfg		CanRemoteCfg;
extern ESPMoticsCfg		Aliases;
extern ESPMoticsCfg		Menu;
extern espShell *		shell;
extern espShell *		shellScreen;
extern espShell *		shellLoRa;
extern unsigned long		LastActivity;
extern SC_GUI			RemoteGUI;
extern tftSerial		SCSerial;
extern unsigned int		actionPin;
# endif

class   CanRemote
{
public:
  CanRemote();
  ~CanRemote();
  String		getParam(const char *param_name, bool confirm = true);
  void			loadCfgSerial();
  bool			getYesNo(const char *Message);
  void			init();
  void			loadConfig();
  void			loadServices();
  void			taskLoop();
  void			goToSleep();

private:
  bool			_spiffsstatus;
};

# ifdef _DECLARE_GLOBAL_CANREMOTE_
CanRemote		CanRemote;
# else
extern CanRemote	CanRemote;
# endif

#endif // __CANREMOTE__ //
