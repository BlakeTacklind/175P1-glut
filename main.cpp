#include "OpenGLhandler.h"
#include "obj.h"
#include <iostream>
#include "userInterface.h"

int main(int argc, char *argv[])
{
  obj::load("objects");
  
  OpenGLhandler::init(&argc, argv);
  
  //obj::freeAll();
  return 0;
}

