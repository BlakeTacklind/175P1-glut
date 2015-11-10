
#include "cline.h"
#include "line.h"
#include "types.h"
#include "userInterface.h"

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
  
  //cout << "test 0" <<endl;

  {
    list<cline*>::iterator it = lst.begin();

    xmin = xmax = (*it)->p1.x;
    ymin = ymax = (*it)->p1.y;

    while(it != lst.end()){
      //remove horizontal lines
      if((*it)->l->isHorizontal()){
        cout<<"delete hor"<<endl;
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

  //cout << "test 1" <<endl;


  //scan along y
  for(int i = ymin; i <= ymax; i++){
    //find all points along horizontal

    //cout << i << " number\n";
    cpnt min, max;

    list<cline*>::iterator it = lst.begin();

    int* I;
    
    do{
      //cout<<"Silly\n";
      I = (*it)->findFrontBackAtLine(i);
      if(I == nullptr) cout<<"Found no item!\n";
    } while (I == nullptr && (++it) != lst.end());

    //cout<<"ness\n";

    if(it == lst.end()){
      userInterface::printError("Failed to find point!");
      continue;
    } 

    min = max = (*it)->getPoint(I[0]);

    cpnt temp = (*it)->getPoint(I[1]);

    if(temp.x > max.x) max = temp;
    if(temp.x < min.x) min = temp;

    delete I;
    it++;
    
    while(it != lst.end()){

      I = (*it)->findFrontBackAtLine(i);
      
      temp = (*it)->getPoint(I[0]);

      if(temp.x > max.x) max = temp;
      if(temp.x < min.x) min = temp;

      temp = (*it)->getPoint(I[1]);

      if(temp.x > max.x) max = temp;
      if(temp.x < min.x) min = temp;

      delete I;
      it++;
    }

    //create cline from inside to inside and draw it 
    cline(min, max).draw(mk);
    //cout<<"draw line "<<y<<endl;
  }

  
  //cout << "test e" <<endl;

}

int* cline::findFrontBackAtLine(int y){
  if(p1.y > y && p2.y > y) return nullptr;
  if(p1.y < y && p2.y < y) return nullptr;

  //cout<<"test 0"<<endl;
  //cout<<l->getP1().y<<" "<<l->getP2().y<<" "<<y<<" "<<numPoints<<endl;

  int* I = new int[2];
  bool first = true;

  for(int i = 0; i < numPoints; i++){
    if(first){
      if(l->getPoint(i).y == y){
        I[0] = i;
        first = false;
        //cout<<"test 4\n";
        if(i == numPoints-1){
          I[1] = i;
          return I;
        }

      }
    }
    else{
      //cout<<"test 5\n";
      if(i == numPoints - 1){
        I[1] = i;
        return I;
      }

      if(l->getPoint(i).y != y){
        I[1] = i-1;
        return I;
      }
    }
  }

  delete I;

  return nullptr;
}
