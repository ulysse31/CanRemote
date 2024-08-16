#include "CanGlobal.h"

void	flushScreen()
{
  if (CanRemote.hwVersion() == HWVER2)
    ((Adafruit_SH1107*)RemoteGUI.screen())->display();
}

CanRemote::CanRemote()
{

}

CanRemote::~CanRemote()
{

}

void
CanRemote::init()
{
  adc1_config_width(ADC_WIDTH_BIT_12);
  //  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
  //  adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED)
    _quickAction = false;
  else
    _quickAction = true;
  _spiffsstatus = SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
  CanCfg.loadCfg();
  Aliases.loadCfg();
  Menu.loadCfg();
  if (CanCfg.getValue("QuickAction") != "true")
    _quickAction = false;
  if (CanCfg.getValue("HWVersion") == "V2")
    _hwVersion = HWVER2;
  else
    _hwVersion = HWVER1;
  if (CanCfg.getValue("Sided") == "Left")
    _sided = SIDED_LEFT;
  else
    _sided = SIDED_RIGHT;
  pinMode(E220_PWR, OUTPUT);
  digitalWrite(E220_PWR, HIGH);
  if (_hwVersion == HWVER2)
    {
      pinMode(E220_EN, OUTPUT);
      digitalWrite(E220_EN, LOW);
    }
  Serial1.begin(LORA_SETUP_BAUD, SERIAL_8N1, LUATOS_RX1, LUATOS_TX1);
  Serial.begin(SERIAL_DEFAULT_SPEED);
  Serial.println("####################### CanRemote INIT #######################");
  RemoteGUI.init();
  shell = new espShell("RemoteSH", &Serial);
  SCSerial.begin(RemoteGUI.screen(), &Serial);
  SCSerial.flushFunc(&flushScreen);
  shellScreen = new espShell("RemoteSH", &SCSerial);
  //shellLoRa = new espShell("RemoteSH", &Serial1, false, true);
}

bool
CanRemote::getYesNo(const char *Message)
{
  char c;

  Serial.print(Message);
  Serial.print(" (Y/N): ");
  while (true)
    {
      if (Serial.available() > 0)
      {
        c = Serial.read();
        if (c == 'y' || c == 'Y')
          return (true);
        if (c == 'n' || c == 'N')
          return (false);
      }
      else
        delay(200);
    }
  return (false);
}

String
CanRemote::getParam(const char *param_name, bool confirm)
{
  char    c = 0;
  String  result;
  String  strconf;
  bool    inLoop = true;

  Serial.print(param_name);
  while (inLoop)
  {
    result = "";
    while (c != '\r' && c != '\n')
    {
      if (Serial.available() > 0)
      {
        c = Serial.read();
        Serial.print(c);
        result += c;
      }
      else
        delay (100);
    }
    result[result.length() - 1] = '\0';
    Serial.print("\r\n");
    strconf = result;
    strconf = strconf + ", confirm ?";
    (getYesNo(strconf.c_str()) ? inLoop = false : inLoop = true);
  }
  result[result.length() - 1] = '\0';
  return (result);
}

void
CanRemote::loadCfgSerial()
{
  Serial.println("Interactive Config menu setting launched:");
  Serial.print("\r\n");

  Serial.setTimeout(400000000);
  String host = getParam("\r\nEnter host name: ", true);
  String ssid = getParam("\r\nEnter SSID: ", true);
  String passphrase = getParam("\r\nEnter Passphrase: ", true);

  CanCfg.setValue("host", host.c_str());
  CanCfg.setValue("ssid", ssid.c_str());
  CanCfg.setValue("passphrase", passphrase.c_str());
  Serial.setTimeout(1000);
  Serial.println("All done! Saving ...");
  if(CanCfg.saveCfg() == true)
    Serial.println("Configuration Successfuly Saved!\n");
  else
    Serial.println("Failed to Save Configuration!\n");
}



void
CanRemote::loadConfig()
{
  RemoteGUI.setActive(Menu.startNode());
}

void
CanRemote::loadServices()
{
  Serial.println("Now dropping shells...");
  shell->init();
  //shellLoRa->init();
}

void
CanRemote::taskLoop()
{
  if (_quickAction == false)
    {
      shell->checkCmdLine();
      if (shellLoRa)
	shellLoRa->checkCmdLine();
    }
  RemoteGUI.refresh();
  delay(10); // main refresh tick for shell / screen ...
}


void
CanRemote::goToSleep()
{
   if (esp_deep_sleep_enable_gpio_wakeup( 1 << KEY_CENTER, ESP_GPIO_WAKEUP_GPIO_LOW) != ESP_OK)
     {
       Serial.println("\nError: Could not enable KEY_CENTER wakeup pin");
       LastActivity = millis();
       return ;
     }
   RemoteGUI.clearScreen();
   RemoteGUI.enableSleep(true);
   Serial.println("\n****************\n* Inactivity Timeout\n*  Going to Sleep\n****************\n");
   esp_deep_sleep_start();
}
