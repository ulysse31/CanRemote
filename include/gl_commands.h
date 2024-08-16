#ifndef __GL_COMMANDS_H__
# define __GL_COMMANDS_H__

t_cmdfunc       gl_commands[] = {
  { "cat", "show file content", cat },
  { "ls", "list files", ls },
  { "rm", "delete file", rm },
  { "cp", "copy file", cp },
  { "mv", "rename file", cp },
  { "ed", "a line-per-line basic text editor", ed },
  { "md5sum", "calculates md5 of a file", md5sum },
  { "xmreceive", "receive file from tty using XMODEM protocol", xmreceive },
  { "fwupdate", "firmware update command", fwupdate },
  { "interactive", "enable/disable interactive shell parameters (echo, verbose...)", interactive },
  { "cfg", "main config parameters editor", cfg },
  { "alias", "alias command / shortcuts", alias },
  { "menu", "Graphical menu command listing", menu },
  { "pin", "pin input/output control", pin },
  { "sleep", "sleep status/management command", cmd_sleep },
  { "battery", "show battery status", battery },
  { "free", "show free memory", cmd_free },
  { "echo", "echo text", echo },
  { "waitforkey", "waits for a keypress", waitforkey },
  { "simulatekey", "simulates keypress", simulatekey },
  { "serial", "serial redirection tool", serial },
  { "exec", "execute commands from file", exec },
  { "delay", "sleeps for a specified number of milliseconds", cmd_delay },
  { "lorasend", "send command via lora", lorasend },
  { "lorasecure", "enable/disable auth in lorashell", lorasecure },
  { "lorashell", "enable/disable lorashell", lorashell },
  { "restart", "restart the device", restart },
  {0, 0, 0}
};

#endif // !__GL_COMMANDS_H__ //
