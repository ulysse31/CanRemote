#ifndef __TFTSERIAL_H__
# define __TFTSERIAL_H__


class		tftSerial : public Stream
{
 public:
  tftSerial();
  ~tftSerial();
  void		begin(Print *print, Stream *fallback);
  bool		tftEnable() { return ((_tft ? true : false)); }
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
  Print	*	_tft;
  Stream *	_fbserial;
};

#endif // !__TFTSERIAL_H__ //

