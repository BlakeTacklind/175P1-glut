/* 
 * File:   pline.cpp
 * Author: btacklind
 * 
 * Created on November 10, 2015, 2:39 PM
 */

#include "pline.h"
#include "userInterface.h"

pline::pline() {
}

pline::pline(const pline& orig) {
}

pline::~pline() {
  delete [] fill;
}

pline::pline(pnt P1, pnt P2, pnt3 A, pnt3 B, pnt3 norm, screen* printTo):line(P1, P2, true) {
  
  if(getP1() == P1){
    a = A;
    b = B;
  }
  else{
    a = B;
    b = A;
  }
  
  scrn = printTo;
  normal = norm;
  numPoints = getNumPoints();
  
  Fill();
}

void pline::Fill() {
  fill = new pnt3[numPoints-2];
  pnt3 m = (b-a)/(numPoints-1);
  
  for(int i = 1; i < numPoints-1; i++){
    fill[i] = scrn->getColor(a + i*m, normal);
  }
}

void pline::draw(MakePixOff mk) {
  for(int i = 0; i < numPoints-2; i++){
    pnt x = getPoint(i);
    mk(x.x, x.y , fill[i]);
  }
}

void pline::raster(MakePixOff mk, list<pline*>& lst) {
  int xmin, xmax;
  int ymin, ymax;
  
  pnt3 norm = {0,0,0};
  
  {
    list<pline*>::iterator it = lst.begin();

    xmin = xmax = (*it)->getP1().x;
    ymin = ymax = (*it)->getP1().y;

    while(it != lst.end()){
      //find average normal to surface defined by lines
      norm = norm + (*it)->normal;
      
      //remove horizontal lines
      if((*it)->isHorizontal()){
        delete *it;
        it = lst.erase(it);
      }
      //find min and max x and y
      else{
             if((*it)->getP1().x < xmin) xmin = (*it)->getP1().x;
        else if((*it)->getP1().x > xmax) xmax = (*it)->getP1().x;

             if((*it)->getP1().y < ymin) ymin = (*it)->getP1().y;
        else if((*it)->getP1().y > ymax) ymax = (*it)->getP1().y;

             if((*it)->getP2().x < xmin) xmin = (*it)->getP2().x;
        else if((*it)->getP2().x > xmax) xmax = (*it)->getP2().x;

             if((*it)->getP2().y < ymin) ymin = (*it)->getP2().y;
        else if((*it)->getP2().y > ymax) ymax = (*it)->getP2().y;
        it++;
      }
    }
    
    norm = ~norm;
  }
  
  for(int i = ymin; i <= ymax; i++){
    int* I;
    list<holder*> vals;
    
    list<pline*>::iterator it = lst.begin();
    
    it++;
    while(it != lst.end()){

      I = (*it)->findFrontBackAtLine(i);
      
      if(I != nullptr){
        vals.push_front((*it)->getHolderFromI(I));

        delete I;
      }
      
      it++;
    }
    
    //in case of errors
    if(vals.size() < 2){
      userInterface::printError("Failed to get a possible scan line!");
      return;
    }
    
    //find min max of vals
    holder* min, max;
    {
      list<holder*>::iterator it2 = vals.begin();
      min = max = *it2;
      it2++;
      while( it2 != vals.end() ){
        //check for change max
        //change in case found a point with a larger max or same max and smaller min
        //assumes no complex points
        if(max->max.x < (*it2)->max.x || (max->max.x == (*it2)->max.x && max->min.x > (*it2)->min.x)){
          max = *it2;
        }
        
        //check for change min
        //change in case found a point with a smaller min or same min and larger max
        //assumes no complex points
        if(min->min.x > (*it2)->min.x || 
                (min->min.x == (*it2)->min.x && min->max.x < (*it2)->max.x)){
          max = *it2;
        }
        
        it2++;
      }
    }
    
    pline* l = new pline({max->min.x, i}, {min->max.x, i}, max->min.p, min->max.p, norm, (*it)->scrn);
    l->draw(mk);
    delete l;
    
    for(list<holder*>::iterator it2 = vals.begin(); it2 != vals.end(); it2++)
      delete (*it2);
  }

}

pline::holder* pline::getHolderFromI(int* I) {
  holder* h = new holder;
  
  int i = getPoint(I[0]);
  int j = getPoint(I[1]);
  
  if(i>j){
    h->max.x = i;
    h->min.x = j;
    
    h->max.p = getPnt3(I[0]);
    h->min.p = getPnt3(I[1]);
  }
  else{
    h->max.x = j;
    h->min.x = i;
    
    h->max.p = getPnt3(I[1]);
    h->min.p = getPnt3(I[0]);
  }
  
  return h;
}
