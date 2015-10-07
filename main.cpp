#include "OpenGLhandler.h"
#include "FileHandler.h"

int main(int argc, char *argv[])
{
  obj* objList = FileHandler::load("objects");
  //OpenGLhandler::init(&argc, argv);
  //OpenGLhandler::MakePix(11,10);

  return 0;
}

