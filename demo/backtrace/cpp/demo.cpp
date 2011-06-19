/*
 * This demo starts up and intentionally crashes itself to prove the backtrace stuff works
 */

#include <libsdlgame/libsdlgame.h>

class Object {
private:
        int *x;
public:
        int crashme(void);
};

int Object::crashme(void)
{
        this->x = 0;
        return *(this->x);
}

int main(int argc, char *argv[])
{
  RegisterStacktrace();

  // We don't initialize anything here because we would lose the memory
  Object x;
  x.crashme();
  return 0;
}
