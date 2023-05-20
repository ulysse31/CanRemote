#ifndef __SC_GUI_H__
# define __SC_GUI_H__


// Endianness Fun :p
# define RED_COLOR	ST77XX_BLUE
# define BLUE_COLOR	ST77XX_RED
# define BLACK_COLOR	ST77XX_BLACK
# define WHITE_COLOR	ST77XX_WHITE
# define DGREY_COLOR	0x7BEF

class			SC_GUI
{
 public:
  SC_GUI();
  ~SC_GUI();
  bool			init();
  bool			refresh();
  void			setUpdate(bool update = true) { _forceUpdate = update; }
  void			interpreteAction();
  unsigned int		waitForKeyPress(unsigned long timeout = 0);
  cfgNode *		activeMenu() { return (_active); }
  void			setActive(cfgNode *p) { _active = p; }
  Adafruit_ST7735	*screen() { return (_tft); }
private:
  Adafruit_ST7735	*_tft;
  cfgNode		*_active;
  bool			_forceUpdate;
  unsigned int		_lastAction;
};

#endif // !__SC_GUI_H__ //
