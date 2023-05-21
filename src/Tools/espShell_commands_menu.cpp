#include "CanRemote.h"

typedef struct	s_menucmd
{
  const char *	name;
  bool		(*fct)(Stream *s, char **args);
}		t_menucmd;


bool		menu_load(Stream *s, char **args)
{
  String	line;
  
  if (args[2] == 0)
    {
      s->println("Usage: menu load <cfgfile>");
      return (false);
    }
  if (args[2][0] == '/')
    line = args[2];
  else
    {
      line = "/";
      line += args[2];
    }
  if (!(SPIFFS.exists(line.c_str())))
    {
      s->print("Error: File ");
      s->print(line);
      s->println(" does not exist!");
      return (false);
    }
  Menu.unloadCfg();
  Menu.cfgFile(line.c_str());
  if (Menu.loadCfg() == false)
    {
      s->println("Error loading menu");
      return (false);
    }
  RemoteGUI.refresh();
  return (true);
}

bool		menu_list(Stream *s, char **args)
{
  cfgNode	*p;

  s->print("Actual menu file: ");
  s->print(Menu.cfgFile());
  s->println(".");
  s->println();
  s->println("Menu:\tCommand:");
  s->println("(*) Active element");
  s->println();
  for (p = Menu.startNode(); p; p = p->next())
    {
      s->print(p->key());
      s->print(":\t");
      s->print('"');
      s->print(p->value());
      if (p == RemoteGUI.activeMenu())
	s->print("\t*");
      s->println('"');
    }
  return (true);
}

bool		menu_get(Stream *s, char **args)
{
  String	res;

  if (args[2] == 0)
    {
      s->println("Usage: menu get <menu>");
      return (false);
    }
  res =Menu.getValue(args[2]); 
  if (res == "")
    {
      s->print(args[2]);
      s->println(": menu not found");
      return (false);
    }
  else
    {
      s->print(args[2]);
      s->print(":\t");
      s->println(res);
    }
  return (true);
}

bool		menu_set(Stream *s, char **args)
{
  String	key;
  String	val;

  if (args[2] == 0 || (args[2] && args[3] == 0))
    {
      s->println("Usage: menu set <menu> <cmd>");
      return (false);
    }
  key = args[2];
  val = args[3];
  if (key == "")
    {
      s->println("Error: menu cannot be empty");
      return (false);
    }
  if (val == "")
    {
      s->println("Error: command cannot be empty");
      return (false);
    }
  if(Menu.setValue(key, val) == false)
    {
      s->println("Error: Could not set menu/command pair");
      return (false);
    }
  return (true);
}

bool	menu_del(Stream *s, char **args)
{
  if (args[2] == 0)
    {
      s->println("Usage: menu del <menu>");
      return (false);
    }
  if(Menu.delValue(args[2]) == false)
    {
      s->println("Error: Could not delete menu => not found?");
      return (false);
    }
  return (true);
}

bool	menu_save(Stream *s, char **args)
{
  if (Menu.saveCfg() == false)
    {
      s->println("Error: Could not save menu config");
      return (false);
    }
  s->println("Menu Configuration successfuly saved");
  return (true);
}

bool		menu_active(Stream *s, char **args)
{
  cfgNode	*p;

  s->println("Menu:\tCommand:");
  s->print(RemoteGUI.activeMenu()->key());
  s->print(":\t");
  s->print('"');
  s->print(RemoteGUI.activeMenu()->value());
  s->println('"');
  return (true);
}


t_menucmd	gl_menucmd[] =
  {
    {"load", menu_load},
    {"list", menu_list},
    {"active", menu_active},
    {"get", menu_get},
    {"set", menu_set},
    {"del", menu_del},
    {"save", menu_save},
    {0, 0}
  };


bool	menu(espShell *sh, Stream *s, char **args)
{
  if (args[1] == 0)
    {
      s->println("Usage: menu [list|save|get <menu>|set <menu> <cmd>|del <menu>| load <file>]");
      return (false);
    }
  for (int i = 0; gl_menucmd[i].name; i++)
    if (strcmp(args[1], gl_menucmd[i].name) == 0)
      return(gl_menucmd[i].fct(s, args));
  s->println("Usage: menu [list|get <menu>|set <menu> <cmd>|del <menu>]");
  return (false);
}
