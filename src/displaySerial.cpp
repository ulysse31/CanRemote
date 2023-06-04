#include "CanGlobal.h"

displaySerial::displaySerial()
{
  _display = 0;
  _fbserial = 0;
}

displaySerial::~displaySerial()
{

}

void
displaySerial::begin(Print *print, Stream *fallback)
{
  _display = print;
  _fbserial = fallback;
}

size_t
displaySerial::print(const char c)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(c));
}

size_t
displaySerial::print(const String &s)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(s));
}

size_t
displaySerial::print(const char c[])
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(c));
}

size_t
displaySerial::print(unsigned char b, int base)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(b, base));
}

size_t
displaySerial::print(int num, int base)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(num, base));
}

size_t
displaySerial::print(unsigned int num, int base)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(num, base));
}

size_t
displaySerial::print(long num, int base)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(num, base));
}

size_t
displaySerial::print(unsigned long num, int base)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(num, base));
}

size_t displaySerial::print(long long num, int base)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(num, base));
}

size_t
displaySerial::print(unsigned long long num, int base)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(num, base));
}

size_t
displaySerial::print(double num, int digits)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->print(num, digits));
}

size_t
displaySerial::println(void)
{
    return print("\r\n");
}


size_t
displaySerial::println(const String &s)
{
    size_t n = print(s);
    n += println();
    return (n);
}

size_t
displaySerial::println(const char c[])
{
    size_t n = print(c);
    n += println();
    return (n);
}

size_t
displaySerial::println(char c)
{
    size_t n = print(c);
    n += println();
    return (n);
}

size_t
displaySerial::println(unsigned char b, int base)
{
    size_t n = print(b, base);
    n += println();
    return (n);
}

size_t
displaySerial::println(int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
displaySerial::println(unsigned int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
displaySerial::println(long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
displaySerial::println(unsigned long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t displaySerial::println(long long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
displaySerial::println(unsigned long long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return (n);
}

size_t
displaySerial::println(double num, int digits)
{
    size_t n = print(num, digits);
    n += println();
    return (n);
}

//
// Compats from here
//

size_t
displaySerial::write(uint8_t c)
{
  if (displayEnable() == 0)
    return (0);
  return (_display->write(c));
}

int
displaySerial::available()
{
  if (fallbackEnable() == 0)
    return (0);
  return (_fbserial->available());
}

int
displaySerial::read()
{
  if (fallbackEnable() == 0)
    return (0);
  return (_fbserial->read());
}

int
displaySerial::peek()
{
  if (fallbackEnable() == 0)
    return (0);
  return (_fbserial->peek());
}

