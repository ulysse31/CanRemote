#include "CanGlobal.h"

void	*xmalloc(size_t size)
{
  void	*ptr;

  if ((ptr = malloc(size)) == 0)
    {
      Serial.println("Could Not Allocate Memory, exiting...");
      exit (1);
    }
  return (ptr);
}
