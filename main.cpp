#include "OpenGLhandler.h"
#include "obj.h"
#include <iostream>
#include "userInterface.h"

int main(int argc, char *argv[])
{
  obj::load("objects");
  //cout << obj::getObject(0).getPoints()[0].y;
  //obj::getObject(2).rotation(30.0);
  //obj::getObject(2).rotation(-30.0);
  //userInterface::init();
  //obj::save("objects2");
  cout<<"test1\n";
  OpenGLhandler::init(&argc, argv);
  
  //obj::freeAll();
  return 0;
}

