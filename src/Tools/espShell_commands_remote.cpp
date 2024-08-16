#include "CanGlobal.h"

typedef struct		s_voltmatrix
{
  float			volts;
  uint8_t		percent;
}			t_voltmatrix;

t_voltmatrix	gl_voltmatrix[] = {
  {4.15,  100},
  {4.02, 95},
  {4.00, 90},
  {3.98, 85},
  {3.95, 80},
  {3.91, 75},
  {3.87, 70},
  {3.85, 65},
  {3.83, 60},
  {3.82, 55},
  {3.80, 50},
  {3.79, 45},
  {3.77, 40},
  {3.75, 35},
  {3.73, 30},
  {3.71, 25},
  {3.60, 20},
  {3.41, 15},
  {3.33, 10},
  {3.27, 5},
  {3.16, 0},
  {0, 0}
};


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

bool	battery(espShell *sh, Stream *s, char **args)
{
  uint16_t	adcval;
  uint8_t	i;
  String	factor;
  float		voltage;
  uint8_t	percentage;

  for (i = 0; i < BAT_ADC_MAXSAMPLES; i++, delay(2))
    adcval += adc1_get_raw((adc1_channel_t)(BAT_ADC_PIN));
  adcval /= BAT_ADC_MAXSAMPLES;
  s->print("ADC: ");
  s->println(adcval);
  factor = CanCfg.getValue("ADCFactor");
  voltage = ((float)(adcval)) * factor.toFloat();
  s->print(voltage);
  s->println("V");

  for (i = 0, percentage = gl_voltmatrix[i].percent; gl_voltmatrix[i].volts != 0; i++, percentage = gl_voltmatrix[i-1].percent)
    if (gl_voltmatrix[i].volts <= voltage)
      break ;
  s->print(percentage);
  s->println("%");
  return (true);
}
