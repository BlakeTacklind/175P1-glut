
#include "gline.h"
#include "line.h"
#include "types.h"
#include "userInterface.h"

gline::gline(cpnt a, cpnt b):line(dg(a), dg(b), true){
  p1 = a;
  p2 = b;

  numPoints = getNumPoints();

  Fill();
}

gline::~gline() {
  delete [] fill;
}


void gline::Fill(){
  pnt3 m = (p2.c - p1.c) / (numPoints - 1);

  fill = new pnt3[numPoints];

  fill[0] = p1.c;
  fill[numPoints - 1] = p2.c;

  for(int i = 1; i < numPoints - 1; i++){
    fill[i] = p1.c + (i*m);
  }
}

void gline::draw(MakePixOff mk){
  for(int i = 0; i < numPoints; i++){
    pnt p = getPoint(i);
    mk(p.x, p.y, fill[i]);
  }
}

void gline::raster(MakePixOff mk, list<gline*>& lst){
  int xmin, xmax;
  int ymin, ymax;
  
  {
    list<gline*>::iterator it = lst.begin();

    xmin = xmax = (*it)->p1.x;
    ymin = ymax = (*it)->p1.y;

    while(it != lst.end()){
      //remove horizontal lines
      if((*it)->isHorizontal()){
        delete *it;
        it = lst.erase(it);
      }
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
        it++;
      }
    }
  }

  //scan along y
  for(int i = ymin; i <= ymax; i++){
    //find all points along horizontal

    cpnt min, max;

    list<gline*>::iterator it = lst.begin();

    int* I;
    
    do{
      I = (*it)->findFrontBackAtLine(i);
    } while (I == nullptr && (++it) != lst.end());


    if(it == lst.end()){
      userInterface::printError("Failed to find a line along horizontal!");
      return;
    } 

    min = max = (*it)->getPoint(I[0]);

    cpnt temp = (*it)->getPoint(I[1]);

    if(temp.x > max.x) max = temp;
    if(temp.x < min.x) min = temp;

    delete I;
    it++;
    
    while(it != lst.end()){

      I = (*it)->findFrontBackAtLine(i);
      if(I==nullptr){
        it++;
        continue;
      }

      temp = (*it)->getCPoint(I[0]);

      if(temp.x > max.x) max = temp;
      if(temp.x < min.x) min = temp;

      temp = (*it)->getCPoint(I[1]);

      if(temp.x > max.x) max = temp;
      if(temp.x < min.x) min = temp;

      delete I;
      it++;
    }

    //create cline from inside to inside and draw it 
    gline(min, max).draw(mk);
  }

}
