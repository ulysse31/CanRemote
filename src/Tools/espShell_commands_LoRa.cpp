#include "CanRemote.h"


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
    shellLoRa = new espShell(&Serial1, false, true);
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

  if (CanRemoteCfg.getValue("LoraRemoteKey") == "" || CanRemoteCfg.getValue("LoraCarKey") == ""
      || CanRemoteCfg.getValue("LoraRemoteKey").length() != AUTH_TOKEN_SIZE
      || CanRemoteCfg.getValue("LoraCarKey").length() != AUTH_TOKEN_SIZE)
    {
      s->println("Incorrect Lora remote/car key setup, create LoraRemoteKey/LoraCarKey with 16 bytes key first");
      return (false);
    }
  remote.carKey(CanRemoteCfg.getValue("LoraCarKey").c_str());
  remote.remoteKey(CanRemoteCfg.getValue("LoraRemoteKey").c_str());
  i = 0;
  time = millis();
  while (true)
    if (Serial1.available())
      {
        buff[i] = ((char)Serial1.read());
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
  Serial1.write(remote.challenge(), AUTH_TOKEN_SIZE);
  return (true);
}

bool	lorasend(espShell *sh, Stream *s, char **args)
{
  bool		isSecure;
  unsigned long time;
  unsigned int	len;
  char		c;

  if (CanRemoteCfg.getValue("LoraSecure") == "true")
    isSecure = true;
  else
    isSecure = false;
  if (args[1] == 0)
    {
      s->println("Usage: lorasend \"text to send\"");
      return (true);
    }
  len = strlen(args[1]);
  Serial1.write("\r\n", 2);	// if asleep ... sending fake order to wake up
  delay(100);			// and wait a bit for it to get awake
  while (Serial1.available())	// flushing any remaining bytes
    c = Serial1.read();
  Serial1.write(args[1], len);
  Serial1.write("\r\n", 2);
  if (isSecure && loraAuth(sh, s) == false)
    {
      s->println("Error: Authentication Failed");
      return(false);
    }
  time = millis();
  while (Serial1.available() == 0)
    if ((millis() - time) > 1000)
      break ;
    else
      delay(10);
  c  = ((char)Serial1.read());
  if (c != 0x04)
  {
    s->print("Error: No Answer (0x");
    s->print(c, HEX);
    s->println(")");
    return (false);
  }
  time = millis();
  while (true)
    if (Serial1.available())
      {
	c = ((char)Serial1.read());
	if (c == 0x04)
	  break ;
	s->print(c);
	time = millis();
      }
    else
      {
	if ((millis() - time) > 5000) // keep in mind that if command takes more than this value without writing data ... connection will be closed
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

  if (CanRemoteCfg.getValue("LoraSecure") == "true")
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
  Serial1.write("\r\n", 2);	// if asleep ... sending fake order to wake up
  delay(100);			// and wait a bit for it to get awake
  while (Serial1.available())	// flushing any remaining bytes
    c = Serial1.read();
  Serial1.write(initcmd.c_str(), len);
  Serial1.write("\r\n", 2);
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
	  Serial1.write((uint8_t)c);
	  if (c == 0x04 || c == 0x1B)
	    break ;// end here
	  s->print(c);
	}
      delay(10);
    }
  time = millis();
  while (Serial1.available() == 0)
    if ((millis() - time) > 1000)
      break ;
    else
      delay(10);
  c  = ((char)Serial1.read());
  if (c != 0x04)
  {
    s->print("Error: No Answer (0x");
    s->print(c, HEX);
    s->println(")");
    return (true);
  }
  time = millis();
  while (true)
    if (Serial1.available())
      {
	c = ((char)Serial1.read());
	if (c == 0x04)
	  break ;
	s->print(c);
	time = millis();
      }
    else
      {
	if ((millis() - time) > 5000) // keep in mind that if command takes more than this value without writing data ... connection will be closed
	  {
	    s->println("Error: lost in transaction");
	    break ;
	  }
      }
  return (true);
}
