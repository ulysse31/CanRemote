#define _DECLARE_GLOBAL_CANREMOTE_
#include <CanGlobal.h>

void setup()
{
  shell = 0;
  shellLoRa = 0;
  actionPin = 0;
  LastActivity = millis();
  CanRemote.init();
  CanRemote.loadConfig();
  if (CanRemote.quickAction() == false)
    CanRemote.loadServices();
}

void loop()
{
  CanRemote.taskLoop();
  if (CanCfg.getValue("EnableSleep") == "true" && CanCfg.getValue("InactivityTimeout") != ""
      && (millis() - LastActivity) > (CanCfg.getValue("InactivityTimeout").toInt() * 1000) || (CanRemote.quickAction() == true))
    CanRemote.goToSleep();
}
