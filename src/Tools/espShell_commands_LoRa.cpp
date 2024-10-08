#include "CanGlobal.h"


bool	lorashell(espShell *sh, Stream *s, char **args)
{
  if (args[1] == 0 || (args[1] && strcmp(args[1], "enable") != 0 && strcmp(args[1], "disable") != 0))
    {
      s->println("Usage: lorashell enable|disable");
      return (false);
    }
  if (strcmp(args[1], "enable") == 0 && shellLoRa != 0)
    {
      s->println("lorashell already enabled !");
      return (false);
    }
  if (strcmp(args[1], "disable") == 0 && shellLoRa == 0)
    {
      s->println("lorashell already disabled !");
      return (false);
    }
  if (strcmp(args[1], "enable") == 0)
    shellLoRa = new espShell("RemoteSH", &LORA_SERIAL, false, true);
  else
    {
      delete shellLoRa;
      shellLoRa = 0;
    }
  return (true);
}

bool    lorasecure(espShell *sh, Stream *s, char **args)
{
  if (args[1] == 0)
    {
      s->println("Usage: lorasecure on|off");
      return (false);
    }
  if (shellLoRa == 0)
    {
      s->println("Lora Shell not initialized, please enable it first");
      return (false);
    }
  if (strcmp(args[1], "on") == 0)
    shellLoRa->secure(true);
  if (strcmp(args[1], "off") == 0)
    shellLoRa->secure(false);
  return (true);
}

void    showbuff(Stream *s, const char *title, const char *buff)
{
  s->print(title);
  for (int i = 0; i < AUTH_TOKEN_SIZE; i++)
    s->write(buff[i]);
  s->println();
}

void    showbuffhex(Stream *s, const char *title, const char *buff)
{
  s->print(title);
  for (int i = 0; i < AUTH_TOKEN_SIZE; i++)
    {
      s->print(buff[i],HEX);
      s->print(" ");
    }
  s->println();
}

bool		loraAuth(espShell *sh, Stream *s)
{
  unsigned long time;
  unsigned int	i;
  char          nounce[AUTH_TOKEN_SIZE];
  char          buff[AUTH_TOKEN_SIZE];
  authToken     remote;

  if (CanCfg.getValue("LoraRemoteKey") == "" || CanCfg.getValue("LoraCarKey") == ""
      || CanCfg.getValue("LoraRemoteKey").length() != AUTH_TOKEN_SIZE
      || CanCfg.getValue("LoraCarKey").length() != AUTH_TOKEN_SIZE)
    {
      s->println("Incorrect Lora remote/car key setup, create LoraRemoteKey/LoraCarKey with 16 bytes key first");
      return (false);
    }
  remote.carKey(CanCfg.getValue("LoraCarKey").c_str());
  remote.remoteKey(CanCfg.getValue("LoraRemoteKey").c_str());
  i = 0;
  time = millis();
  while (true)
    if (LORA_SERIAL.available())
      {
        buff[i] = ((char)LORA_SERIAL.read());
        i++;
        if (i == AUTH_TOKEN_SIZE)
          break ;
        time = millis();
      }
    else
      {
        if ((millis() - time) > 5000) // keep in mind that if command takes more than this value without writing data ... connection will be closed
	  return (false);
      }
  //showbuffhex(s, "\nReceived: ", buff);
  remote.setChallenge(buff);
  remote.decryptWithCar();
  //showbuff(s, "\nDecrypt:", remote.token());
  remote.encryptWithRemote();
  //showbuffhex(s, "\nSending: ", remote.challenge());
  LORA_SERIAL.write(remote.challenge(), AUTH_TOKEN_SIZE);
  return (true);
}

bool	lorasend(espShell *sh, Stream *s, char **args)
{
  bool		isSecure;
  unsigned long time;
  unsigned int	len;
  char		c;

  if (CanCfg.getValue("LoraSecure") == "true")
    isSecure = true;
  else
    isSecure = false;
  if (args[1] == 0)
    {
      s->println("Usage: lorasend \"text to send\"");
      return (true);
    }
  len = strlen(args[1]);
  LORA_SERIAL.write("\r\n", 2);	// if asleep ... sending fake order to wake up
  delay(100);			// and wait a bit for it to get awake
  while (LORA_SERIAL.available())	// flushing any remaining bytes
    c = LORA_SERIAL.read();
  LORA_SERIAL.write(args[1], len);
  LORA_SERIAL.write("\r\n", 2);
  if (isSecure && loraAuth(sh, s) == false)
    {
      s->println("Error: Authentication Failed");
      return(false);
    }
  time = millis();
  while (LORA_SERIAL.available() == 0)
    if ((millis() - time) > 1000)
      break ;
    else
      delay(10);
  c  = ((char)LORA_SERIAL.read());
  if (c != 0x04)
    {
      s->print("Error: No Answer (0x");
      s->print(c, HEX);
      s->println(")");
      return (false);
    }
  time = millis();
  while (true)
    if (LORA_SERIAL.available())
      {
	c = ((char)LORA_SERIAL.read());
	if (c == 0x04)
	  break ;
	s->print(c);
	time = millis();
      }
    else
      {
	if ((millis() - time) > 10000) // keep in mind that if command takes more than this value without writing data ... connection will be closed
	  {
	    s->println("Error: lost in transaction");
	    return (false);
	  }
      }
  return (true);
}

bool	loraed(espShell *sh, Stream *s, char **args)
{
  bool		isSecure;
  unsigned long time;
  unsigned int	len;
  String	initcmd;
  char		c;

  if (CanCfg.getValue("LoraSecure") == "true")
    isSecure = true;
  else
    isSecure = false;
  if (args[1] == 0)
    {
      s->println("Usage: loraed \"filename\"");
      return (true);
    }
  initcmd = String("ed ");
  initcmd += args[1];
  len = strlen(initcmd.c_str());
  LORA_SERIAL.write("\r\n", 2);	// if asleep ... sending fake order to wake up
  delay(100);			// and wait a bit for it to get awake
  while (LORA_SERIAL.available())	// flushing any remaining bytes
    c = LORA_SERIAL.read();
  LORA_SERIAL.write(initcmd.c_str(), len);
  LORA_SERIAL.write("\r\n", 2);
  if (isSecure && loraAuth(sh, s) == false)
    {
      s->println("Error: Authentication Failed");
      return(false);
    }
  // we are auth and distant ed is listening
  // we now need to redirect serial input to lora
  // and wait for Ctrl+D (or Esc)
  while (s->available())  // clear remaining bytes in serial buffer
    c = ((char)s->read());
  s->print("Adding Edited lines remotely inside ");
  s->println(args[1]);
  s->println("Once ended, type <Ctrl+D> or Esc");
  s->println("Process will end after a period of inactivity of 5 mins");
  while (true)
    {
      if (s->available())
	{
	  c = ((char)s->read());
	  LORA_SERIAL.write((uint8_t)c);
	  if (c == 0x04 || c == 0x1B)
	    break ;// end here
	  s->print(c);
	}
      delay(10);
    }
  time = millis();
  while (LORA_SERIAL.available() == 0)
    if ((millis() - time) > 1000)
      break ;
    else
      delay(10);
  c  = ((char)LORA_SERIAL.read());
  if (c != 0x04)
    {
      s->print("Error: No Answer (0x");
      s->print(c, HEX);
      s->println(")");
      return (true);
    }
  time = millis();
  while (true)
    if (LORA_SERIAL.available())
      {
	c = ((char)LORA_SERIAL.read());
	if (c == 0x04)
	  break ;
	s->print(c);
	time = millis();
      }
    else
      {
	if ((millis() - time) > 10000) // keep in mind that if command takes more than this value without writing data ... connection will be closed
	  {
	    s->println("Error: lost in transaction");
	    break ;
	  }
      }
  return (true);
}
