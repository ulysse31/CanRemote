#include "CanRemote.h"

tftSerial::tftSerial()
{
  _tft = 0;
  _fbserial = 0;
}

tftSerial::~tftSerial()
{

}

void
tftSerial::begin(Print *print, Stream *fallback)
{
  _tft = print;
  _fbserial = fallback;
}

size_t
tftSerial::print(const char c)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(c));
}

size_t
tftSerial::print(const String &s)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(s));
}

size_t
tftSerial::print(const char c[])
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(c));
}

size_t
tftSerial::print(unsigned char b, int base)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(b, base));
}

size_t
tftSerial::print(int num, int base)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(num, base));
}

size_t
tftSerial::print(unsigned int num, int base)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(num, base));
}

size_t
tftSerial::print(long num, int base)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(num, base));
}

size_t
tftSerial::print(unsigned long num, int base)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(num, base));
}

size_t tftSerial::print(long long num, int base)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(num, base));
}

size_t
tftSerial::print(unsigned long long num, int base)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(num, base));
}

size_t
tftSerial::print(double num, int digits)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->print(num, digits));
}

size_t
tftSerial::println(void)
{
    return print("\r\n");
}


size_t
tftSerial::println(const String &s)
{
    size_t n = print(s);
    n += println();
    return (n);
}

size_t
tftSerial::println(const char c[])
{
    size_t n = print(c);
    n += println();
    return (n);
}

size_t
tftSerial::println(char c)
{
    size_t n = print(c);
    n += println();
    return (n);
}

size_t
tftSerial::println(unsigned char b, int base)
{
    size_t n = print(b, base);
    n += println();
    return (n);
}

size_t
tftSerial::println(int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
tftSerial::println(unsigned int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
tftSerial::println(long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
tftSerial::println(unsigned long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t tftSerial::println(long long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
tftSerial::println(unsigned long long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
tftSerial::println(double num, int digits)
{
    size_t n = print(num, digits);
    n += println();
    return (n);
}


//
// Compats from here
//

size_t
tftSerial::write(uint8_t c)
{
  if (tftEnable() == 0)
    return (0);
  return (_tft->write(c));
}

int
tftSerial::available()
{
  if (fallbackEnable() == 0)
    return (0);
  return (_fbserial->available());
}

int
tftSerial::read()
{
  if (fallbackEnable() == 0)
    return (0);
  return (_fbserial->read());
}

int
tftSerial::peek()
{
  if (fallbackEnable() == 0)
    return (0);
  return (_fbserial->peek());
}

