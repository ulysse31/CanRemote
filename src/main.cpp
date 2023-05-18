#define _DECLARE_GLOBAL_CANREMOTE_
#include <CanRemote.h>

void setup()
{
  shell = 0;
  shellLoRa = 0;
  actionPin = 0;
  LastActivity = millis();
  CanRemote.init();
  CanRemote.loadConfig();
  CanRemote.loadServices();
}

void loop()
{
  CanRemote.taskLoop();
  if (CanRemoteCfg.getValue("EnableSleep") == "true" && CanRemoteCfg.getValue("InactivityTimeout") != ""
      && (millis() - LastActivity) > (CanRemoteCfg.getValue("InactivityTimeout").toInt() * 1000))
    CanRemote.goToSleep();
}
