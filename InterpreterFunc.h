

#ifndef INTERPRETERFUNC_H
#define INTERPRETERFUNC_H

class InterpreterFunc{
public:
  InterpreterFunc(){};

  virtual char* operator()(char**) = 0;
};

#endif


