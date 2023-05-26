#include "CanGlobal.h"

bool	simulatekey(espShell *sh, Stream *s, char **args)
{
  if (args[1] == 0)
    {
      s->println("Usage: simulatekey <up|down|right|center>");
      return (true);
    }
  RemoteGUI.setLastAction(0);
  if (strcmp(args[1], "up") == 0)
    return ((actionPin = KEY_UP));
  if (strcmp(args[1], "down") == 0)
    return ((actionPin = KEY_DOWN));
  if (strcmp(args[1], "right") == 0)
    return ((actionPin = KEY_RIGHT));
  if (strcmp(args[1], "center") == 0)
    return ((actionPin = KEY_CENTER));
  s->println("Error: Unknown key");
  return (false);
}

bool	waitforkey(espShell *sh, Stream *s, char **args)
{
  unsigned int  res;
  unsigned long time;
  unsigned long timeout;

  timeout = (args[1] ? strtoul(args[1],0, 10) : 0);
  actionPin = 0;
  time = millis();
  RemoteGUI.enableInterrupts();
  while (true)
    if (actionPin != 0 || (timeout && (millis() - time > timeout)))
      {
        res = actionPin;
        actionPin = 0;
        return (true);
      }
    else
      delay(10);
  RemoteGUI.disableInterrupts();
  return (true);
}
