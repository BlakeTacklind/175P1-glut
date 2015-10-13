#include "OpenGLhandler.h"
#include "obj.h"
#include <iostream>

int main(int argc, char *argv[])
{
  //load objects from default file
  obj::load("objects");
  
  //initilize glut!
  OpenGLhandler::init(&argc, argv);
  return 0;
}

