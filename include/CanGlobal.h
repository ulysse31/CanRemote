#ifndef __CANREMOTE_H__
# define __CANREMOTE_H__

# include <stdint.h>
# include <Arduino.h>
# include <SPI.h>
# include <FS.h>
# include <SPIFFS.h>
# include <Wire.h>
# include <Update.h>
# include <Adafruit_GFX.h>
# include <Adafruit_SH110X.h>
# include <Adafruit_ST7735.h>
# include "mbedtls/aes.h"
# include "soc/soc.h"
# include "soc/rtc_cntl_reg.h"
# include "ESPMoticsCfg.h"
# include "authToken.h"
# include "espShell.h"
# include "result.h"
# include "xmodem_receiver.h"
# include "cppfix.h"
# include "SC_GUI.h"
# include "displaySerial.h"

# define FORMAT_SPIFFS_IF_FAILED true

# define CFGFILE "Can.cfg"
# define ALIASFILE "Aliases.cfg"
# define MENUFILE "Menu.cfg"


# define DEBUGMODE

# define HWVER1	1
# define HWVER2	2

# define SIDED_LEFT 1
# define SIDED_RIGHT 2

# define SERIAL_DEFAULT_SPEED	115200
# define LORA_SERIAL		Serial1
# define LORA_SETUP_BAUD	9600
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
# define LUATOS_SDA		GPIO_NUM_7
# define LUATOS_SCL		GPIO_NUM_6

# define E220_PWR		GPIO_NUM_12
# define E220_EN		GPIO_NUM_10
# define TFT_CS                 GPIO_NUM_7 // PyBadge/PyGamer display control pins: chip select
# define TFT_RST                GPIO_NUM_10 // Display reset
# define TFT_DC                 GPIO_NUM_6 // Display data/command select
# define TFT_BACKLIGHT          GPIO_NUM_11 // Display backlight pin

# define KEY_UP			GPIO_NUM_8
# define KEY_DOWN		GPIO_NUM_5
# define KEY_LEFT		GPIO_NUM_9
# define KEY_RIGHT		GPIO_NUM_13
# define KEY_CENTER		GPIO_NUM_4


class Canremote;


# ifdef _DECLARE_GLOBAL_CANREMOTE_
ESPMoticsCfg            CanCfg(CFGFILE, '=');
ESPMoticsCfg            Aliases(ALIASFILE, '=');
ESPMoticsCfg            Menu(MENUFILE, '=');
espShell *		shell;
espShell *		shellScreen;
espShell *		shellLoRa;
unsigned long		LastActivity;
SC_GUI			RemoteGUI;
displaySerial		SCSerial;
unsigned int		actionPin;
#  else
extern ESPMoticsCfg		CanCfg;
extern ESPMoticsCfg		Aliases;
extern ESPMoticsCfg		Menu;
extern espShell *		shell;
extern espShell *		shellScreen;
extern espShell *		shellLoRa;
extern unsigned long		LastActivity;
extern SC_GUI			RemoteGUI;
extern displaySerial		SCSerial;
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
  uint8_t		hwVersion() { return (_hwVersion); }
  uint8_t		sided() { return (_sided); }
  bool			quickAction() { return (_quickAction); }
  void			quickAction(bool action) { _quickAction = action; }
  void			loadConfig();
  void			loadServices();
  void			taskLoop();
  void			goToSleep();

private:
  bool			_spiffsstatus;
  uint8_t		_hwVersion;
  uint8_t		_sided;
  bool			_quickAction;
};

# ifdef _DECLARE_GLOBAL_CANREMOTE_
CanRemote		CanRemote;
# else
extern CanRemote	CanRemote;
# endif

#endif // __CANREMOTE__ //
