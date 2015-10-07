//#include "OpenGLhandler.h"
#include "obj.h"
#include <iostream>

int main(int argc, char *argv[])
{
  obj::load("objects");
  //cout << obj::getObject(0).getPoints()[0].y;
  
  obj::save("objects2");
  //OpenGLhandler::init(&argc, argv);
  //OpenGLhandler::MakePix(11,10);

  
  return 0;
}

