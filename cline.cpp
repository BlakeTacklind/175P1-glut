
#include "cline.h"
#include "line.h"
#include "types.h"

cline::cline(cpnt a, cpnt b){
  p1 = a;
  p2 = b;

  l = new line(dg(a), dg(b), true);

  numPoints = l->getNumPoints();

  FillG();
}

cline::~cline() {
  delete l;
  delete [] fill;
}


void cline::FillG(){
  pnt3 m = (p2.c-p1.c) / numPoints;

  fill = new pnt3[numPoints];

  fill[0] = p1.c;
  fill[numPoints - 1] = p2.c;

  for(int i = 1; i < numPoints-1; i++){
    fill[i] = p1.c + (i*m);
  }
}

void cline::FillP(){

}

void cline::draw(MakePixOff mk){
  for(int i = 0; i < numPoints; i++){
    pnt p = l->getPoint(i);
    mk(p.x, p.y, fill[i]);
  }
}

void cline::raster(MakePixOff mk, list<cline*>& lst){
  int xmin, xmax;
  int ymin, ymax;
  
  {
    list<cline*>::iterator it = lst.begin();

    xmin = xmax = (*it)->p1.x;
    ymin = ymax = (*it)->p1.y;

    while(it != lst.end()){
      //remove horizontal lines
      if((*it)->l->isHorizontal())
        it = lst.erase(it);
      //find min and max x and y
      else{
             if((*it)->p1.x < xmin) xmin = (*it)->p1.x;
        else if((*it)->p1.x > xmax) xmax = (*it)->p1.x;

             if((*it)->p1.y < ymin) ymin = (*it)->p1.y;
        else if((*it)->p1.y > ymax) ymax = (*it)->p1.y;

             if((*it)->p2.x < xmin) xmin = (*it)->p2.x;
        else if((*it)->p2.x > xmax) xmax = (*it)->p2.x;

             if((*it)->p2.y < ymin) ymin = (*it)->p2.y;
        else if((*it)->p2.y > ymax) ymax = (*it)->p2.y;
      }
    }
  }


  //scan along y
  for(int i = ymin; i <= ymax; i++){
    //find all points along horizontal

    cpnt min, max;

    //lst.front()->

    for(list<cline*>::iterator it = lst.begin(); it != lst.end(); it++){

    }

    //create cline from inside to inside and draw it 


  }


}

cline::twoInt cline::findFrontBackAtLine(int y){

}
