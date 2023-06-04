#ifndef __DISPLAYSERIAL_H__
# define __DISPLAYSERIAL_H__


class		displaySerial : public Stream
{
 public:
  displaySerial();
  ~displaySerial();
  void		begin(Print *print, Stream *fallback);
  bool		displayEnable() { return ((_display ? true : false)); }
  bool		fallbackEnable() { return ((_fbserial ? true : false)); }

  size_t	print(const String &);
  size_t	print(const char[]);
  size_t	print(char);
  size_t	print(unsigned char, int = DEC);
  size_t	print(int, int = DEC);
  size_t	print(unsigned int, int = DEC);
  size_t	print(long, int = DEC);
  size_t	print(unsigned long, int = DEC);
  size_t	print(long long, int = DEC);
  size_t	print(unsigned long long, int = DEC);
  size_t	print(double, int = 2);
  size_t	println(const String &s);
  size_t	println(const char[]);
  size_t	println(char);
  size_t	println(unsigned char, int = DEC);
  size_t	println(int, int = DEC);
  size_t	println(unsigned int, int = DEC);
  size_t	println(long, int = DEC);
  size_t	println(unsigned long, int = DEC);
  size_t	println(long long, int = DEC);
  size_t	println(unsigned long long, int = DEC);
  size_t	println(double, int = 2);
  size_t	println(void);

  size_t	write(uint8_t c);
  int		available();
  int		read();
  int		peek();
  
 private:
  Print	*	_display;
  Stream *	_fbserial;
};

#endif // !__DISPLAYSERIAL_H__ //

