#include "OpenGLhandler.h"
#include "obj.h"
#include "userInterface.h"
#include <iostream>

int main(int argc, char *argv[])
{
  //start UI
  
  userInterface::init();
  //load objects from default file
  obj::load("objects");
  
  //initilize glut!
  OpenGLhandler::init(&argc, argv);
  return 0;
}

