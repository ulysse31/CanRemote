#include <CanRemote.h>


typedef struct  s_keypresscmd
{
  unsigned int	key;
  void          (*fct)();
}               t_keypresscmd;

void		key_up()
{
  cfgNode	*p;

  if (RemoteGUI.activeMenu() == Menu.startNode())
    {
      for (p = Menu.startNode(); p && p->next(); p = p->next())
	;
      RemoteGUI.setActive(p);
      return ;
    }
  for (p = Menu.startNode(); p; p = p->next())
    if (p->next() && RemoteGUI.activeMenu() == p->next())
      RemoteGUI.setActive(p);
}

void		key_down()
{
  cfgNode	*p;

  for (p = Menu.startNode(); p; p = p->next())
    if (RemoteGUI.activeMenu() == p)
      {
	RemoteGUI.setActive(p->next());
	break ;
      }
  if (RemoteGUI.activeMenu() == 0)
    RemoteGUI.setActive(Menu.startNode());
}

void		key_left()
{

}

void		key_center()
{
  bool		ret;
  // exec here
  RemoteGUI.screen()->fillScreen(BLACK_COLOR);
  RemoteGUI.screen()->setCursor(0, 0);
  ret = shellScreen->runLine(RemoteGUI.activeMenu()->cvalue());
  //RemoteGUI.waitForKeyPress(1000);
  // show final return result
  RemoteGUI.screen()->fillScreen(BLACK_COLOR);
  // RemoteGUI.screen()->setCursor(((14 - RemoteGUI.activeMenu()->key().length()) * 6)/2, 48);
  // RemoteGUI.screen()->setTextColor(WHITE_COLOR);
  // RemoteGUI.screen()->setTextSize(1);
  // RemoteGUI.screen()->println(RemoteGUI.activeMenu()->key());
  // if (ret)
  //   {
  //     RemoteGUI.screen()->setCursor(14, 96);
  //     RemoteGUI.screen()->setTextColor(BLUE_COLOR);
  //     RemoteGUI.screen()->setTextSize(2);
  //     RemoteGUI.screen()->println("OK !!");
  //   }
  // else
  //   {
  //     RemoteGUI.screen()->setCursor(14, 96);
  //     RemoteGUI.screen()->setTextColor(RED_COLOR);
  //     RemoteGUI.screen()->setTextSize(2);
  //     RemoteGUI.screen()->println("KO !!");
  //   }
  // RemoteGUI.setUpdate();
  // delay(500);
}

void		key_right()
{
  key_center();
}

t_keypresscmd       gl_keypresscmd[] =
  {
    {KEY_UP, key_up},
    {KEY_DOWN, key_down},
    {KEY_LEFT, key_left},
    {KEY_RIGHT, key_right},
    {KEY_CENTER, key_center},
    {0, 0}
  };

void IRAM_ATTR press_up() {
  actionPin = KEY_UP;
}

void IRAM_ATTR press_down() {
  actionPin = KEY_DOWN;
}

void IRAM_ATTR press_left() {
  actionPin = KEY_LEFT;
}

void IRAM_ATTR press_right() {
  actionPin = KEY_RIGHT;
}

void IRAM_ATTR press_center() {
  actionPin = KEY_CENTER;
}

SC_GUI::SC_GUI()
{

}

SC_GUI::~SC_GUI()
{

}


void
SC_GUI::enableInterrupts()
{
  if (_interruptsEnabled)
    return ;
  attachInterrupt(KEY_UP, press_up, RISING);
  attachInterrupt(KEY_DOWN, press_down, RISING);
  attachInterrupt(KEY_LEFT, press_left, RISING);
  attachInterrupt(KEY_RIGHT, press_right, RISING);
  attachInterrupt(KEY_CENTER, press_center, RISING);
  actionPin = 0;
  _interruptsEnabled = true;
}

void
SC_GUI::disableInterrupts()
{
  if (_interruptsEnabled == false)
    return ;
  detachInterrupt(KEY_UP);
  detachInterrupt(KEY_DOWN);
  detachInterrupt(KEY_LEFT);
  detachInterrupt(KEY_RIGHT);
  detachInterrupt(KEY_CENTER);
  actionPin = 0;
  _forceUpdate = false;
  _interruptsEnabled = false;
}

bool
SC_GUI::init()
{
  pinMode(KEY_CENTER, INPUT_PULLUP);
  pinMode(KEY_UP, INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(KEY_LEFT, INPUT_PULLUP);
  pinMode(KEY_RIGHT, INPUT_PULLUP);
  SPI.begin(LUATOS_SCK, LUATOS_MISO, LUATOS_MOSI, LUATOS_SS);
  _tft = new Adafruit_ST7735(&SPI, TFT_CS, TFT_DC, TFT_RST);
  _tft->initR(INITR_MINI160x80); // Initialize ST7735R screen
  _tft->setRotation(2);
  _tft->setTextWrap(false); // Allow text to run off right edge
  _tft->fillScreen(BLACK_COLOR);
  this->enableInterrupts();
  _active = Menu.startNode();
  _forceUpdate = true;
  _lastAction = 0;
  _action = 0;
  return (true);
}

unsigned int
SC_GUI::waitForKeyPress(unsigned long timeout)
{
  unsigned int	res;
  unsigned long	time;

  this->enableInterrupts();
  time = millis();
  while (true)
    if (actionPin != 0 || (timeout && (millis() - time > timeout)))
      {
	res = actionPin;
	actionPin = 0;
	return (res);
      }
    else
      delay(10);
  this->disableInterrupts();
}

void
SC_GUI::interpreteAction()
{
  for (int i = 0; gl_keypresscmd[i].key != 0; i++)
    if (_action == gl_keypresscmd[i].key)
      {
	gl_keypresscmd[i].fct();
	break ;
      }
}

bool
SC_GUI::refresh()
{
  int		i;
  int		k;
  int		tomove;
  cfgNode       *p;

  if ((actionPin && (millis() - LastActivity > 200 || _lastAction != actionPin)) || _forceUpdate == true)
    {
      _action = actionPin;
      this->disableInterrupts();
      if (!(_forceUpdate))
	this->interpreteAction();
      _tft->fillScreen(BLACK_COLOR);
      for (p = Menu.startNode(), k = 0; p; p = p->next(), k++)
	;
      for (p = Menu.startNode(), i = 0; p; p = p->next(), i++)
	{
	  tomove = ((14 - p->key().length()) * 6)/2;
	  _tft->setCursor(tomove, ((10 - (k % 10))/2 * 16) + i*16);
	  _tft->setTextColor((p == _active ? WHITE_COLOR : DGREY_COLOR));
	  _tft->setTextSize(1);
	  _tft->print(p->key());
	}
      if (_forceUpdate)
	_forceUpdate = false;
      LastActivity = millis();
      _lastAction = _action;
      _action = 0;
      this->enableInterrupts();
    }
  actionPin = 0;
  return (true);
}
