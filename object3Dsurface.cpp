/* 
 * File:   object3Dsurface.cpp
 * Author: btacklind
 * 
 * Created on November 5, 2015, 3:20 PM
 */

#include "object3Dsurface.h"
#include <string>
#include "userInterface.h"
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

char* object3Dsurface::storedFileName;

unsigned int object3Dsurface::nObjects;
object3Dsurface** object3Dsurface::objectList;

bool checkForSame(list<unsigned int> vals){
  for(list<unsigned int>::iterator it = vals.begin(); it != vals.end(); it++){
    list<unsigned int>::iterator it2 = it;
    for(++it2; it2 != vals.end(); it2++){
      if(*it2 == *it) return true;
    }
  }
  
  return false;
}

template <class E>
E* getArrFromList(list<E> l){
  E* arr = new E[l.size()];
  
  typename list<E>::iterator itr = l.begin();
  for(int i = 0; itr != l.end(); itr++, i++)
    arr[i] = *itr;
  
  return arr;
}

object3Dsurface::object3Dsurface(int npnt, pnt3* Points, pnt3* pNormals, int nedge, edge* Edges, int nsurf, surface** surfs) {
  nPoints = npnt;
  points = Points;
  pntNorms = pNormals;
  nEdges = nedge;
  edges = Edges;
  nSurface = nsurf;
  surfaces = surfs;
  
  for(int i = 0; i < nSurface; i++)
    surfaces[i]->setParent(this);
  
}


object3Dsurface::object3Dsurface() {
}

object3Dsurface::object3Dsurface(const object3Dsurface& orig) {
}

object3Dsurface::~object3Dsurface() {
}


bool object3Dsurface::load(const char* filename){
  storedFileName = const_cast<char*>(filename);
  

  ifstream file(filename, ios::in);
  

  if(file.is_open()){
    list<object3Dsurface*> tempList;
    string line;
    nObjects = 0;
    
    /*
     * Get the first line of the file
     * It should be number of objects.
     * If bad input close file and spit out error.
     */
    if(getline(file, line)){
      int num = atoi(line.c_str());
      
      if (num < 0){
        userInterface::printError("negative objects?!?!");
        return close(file, tempList, false);
      }
      else if (num == 0){
        userInterface::printError("bad input or zero objects"); 
        return close(file, tempList, false);
      }
      
      nObjects = num;
    }
    else{
      userInterface::printError("Empty file!");
      return close(file, tempList, false);
    }
    
    /*
     * iterate through the number of objects
     */
    
    for(int i = 0; i < nObjects; i++){
      
      /*
       * Get the number of points
       */
      int num;
      if(getline(file, line)){
        num = atoi(line.c_str());

        if (num < 0){
          userInterface::printError("negative points?!?!");
          return close(file, tempList, false);
        }
        else if (num == 0){
          userInterface::printError("bad input or zero points");  
          return close(file, tempList, false);
        }
      }
      else{
        userInterface::printError("Missing Object definition");
        return close(file, tempList, false);
      }
      
      /*
       * Iterate through the number of points
       */
      pnt3* points = new pnt3[num];
      for (int j = 0; j < num; j++){
        if(getline(file, line)){
          int del = line.find_first_of(' ');
          int del2 = line.substr(del+1).find_first_of(' ');
          
          if (del < 1 || del2 < 1){
            userInterface::printError("Bad point delimiter");
            return close(file, tempList, false);
          }
          
          float x = atof(line.substr(0, del).c_str());
          float y = atof(line.substr(del+1, del2).c_str());
          float z = atof(line.substr(del+1+del2+1).c_str());
          
          points[j].x = x;
          points[j].y = y;
          points[j].z = z;
        }
        else{
          userInterface::printError("Missing Point definition");
          return close(file, tempList, false);
        }
      }
      
      /*
       * Get the number of edges
       */
      int num2;
      if(getline(file, line)){
        num2 = atoi(line.c_str());

        if (num < 0){
          userInterface::printError("Negative edges?!?!");
          return close(file, tempList, false);
        }
        else if (num == 0){
          userInterface::printError("Bad input or zero points");
          return close(file, tempList, false);
        }
      }
      else{
        userInterface::printError("Missing Object edge definition");
        return close(file, tempList, false);
      }
      
      /*
       * Iterate trough the number of edges
       */
      edge* e = new edge[num2];
      for(int j = 0; j < num2; j++){
        if(getline(file, line)){
          int del = line.find_first_of(' ');
          if (del < 1){
            userInterface::printError("Bad edge delimiter");
            return close(file, tempList, false);
          }
          
          int x = atoi(line.substr(0, del).c_str());
          int y = atoi(line.substr(del+1).c_str());
          
          if(x >= num || y >= num){
            userInterface::printError("Point in edge definition does not exist");
            return close(file, tempList, false);
          }
          if (x == y){
            userInterface::printError("Edge can't connect to itself");
            return close(file, tempList, false);
          }
          
          e[j].p1 = x;
          e[j].p2 = y;
        }
        else{
          userInterface::printError("Missing Edge definition");
          return close(file, tempList, false);
        }
      }
      
      /*
       * Get the number of surfaces
       */
      int num3;
      if(getline(file, line)){
        num3 = atoi(line.c_str());

        if (num < 0){
          userInterface::printError("Negative surfaces?!?!");
          return close(file, tempList, false);
        }
        else if (num == 0){
          userInterface::printError("Bad input or zero surfaces");  
          return close(file, tempList, false);
        }
      }
      else{
        userInterface::printError("Missing Object surface definition");
        return close(file, tempList, false);
      }
      
      pnt3* pNorms = new pnt3[num];
      for (int j = 0; j < num; j++){
        pNorms[j].x = zeroVector.x;
        pNorms[j].y = zeroVector.y;
        pNorms[j].z = zeroVector.z;
      }
      
      pnt3* eNorms = new pnt3[num2];
      for (int j = 0; j < num2; j++){
        eNorms[j].x = zeroVector.x;
        eNorms[j].y = zeroVector.y;
        eNorms[j].z = zeroVector.z;
      }
      
      /*
       * Iterate trough the number of surfaces
       */
      surface** s = new surface*[num3];
      for(int j = 0; j < num3; j++){
        list<unsigned int> vals;
        pnt3 p;
        
        /*
         * Get points that form edges
         */
        if(getline(file, line)){
          int del;
          while((del = line.find_first_of(' ')) > 0){
            int pnt = atoi(line.substr(0, del).c_str());
            
            if(pnt >= num || pnt < 0){
              userInterface::printError("Point in edge definition does not exist");
              return close(file, tempList, false);
            }
          
            vals.push_back(pnt);
            line = line.substr(del+1);
          }
         
          int pnt = atoi(line.substr(del+1).c_str());
          
          if(pnt >= num || pnt < 0){
            userInterface::printError("Point in edge definition does not exist");
            return close(file, tempList, false);
          }
          
          vals.push_back(pnt);
          
          if(vals.size() < 3){
            userInterface::printError("Surface must have 3 or more points");
            return close(file, tempList, false);
          }
          
          if (checkForSame(vals)){
            userInterface::printError("Surface can't have two of the same point");
            return close(file, tempList, false);
          }
          
        }
        else{
          userInterface::printError("Missing Surface definition");
          return close(file, tempList, false);
        }
        
        /*
         * get normal for surface
         */
        if(getline(file, line)){
          int del = line.find_first_of(' ');
          int del2 = line.substr(del+1).find_first_of(' ');
          
          if (del < 1 || del2 < 1){
            userInterface::printError("Bad normal delimiter");
            return close(file, tempList, false);
          }
          
          p.x = atof(line.substr(0, del).c_str());
          p.y = atof(line.substr(del+1, del2).c_str());
          p.z = atof(line.substr(del+1+del2+1).c_str());
          
          if(p == zeroVector){
            userInterface::printError("Surface normal can't be zero vector");
            return close(file, tempList, false);
          }
          
          p = ~p;
        }
        else{
          userInterface::printError("Missing Edge definition");
          return close(file, tempList, false);
        }
        
        //convert pnt values into point locations
        int k = 0;
        for(list<unsigned int>::iterator it = vals.begin(); it != vals.end(); it++){
          pNorms[*it] = pNorms[*it] + p;
        }
        
        s[j] = new surface(num3, getArrFromList(vals), p);
      }

      //Build Object
      tempList.push_back(new object3Dsurface(num, points, pNorms, num2, e, num3, s));
    }
    
    return close(file, tempList, true);
  }
  userInterface::printError("Unable to open file");
  
  return false;
}

bool object3Dsurface::close(ifstream& f, list<object3Dsurface*>& tList, bool ret){
  objectList = getArrFromList(tList);
  nObjects = tList.size();
  
  f.close();
  return ret;
}

/*
 * save objects to file
 */
bool object3Dsurface::save(const char* filename){
  storedFileName = const_cast<char*>(filename);
  
  ofstream file(filename);
  if (file.is_open()){
    file << nObjects;
    for (int i = 0; i < nObjects; i++){
      file << "\n" << objectList[i]->nPoints;
      for (int j = 0 ; j < objectList[i]->nPoints; j++)
        file << "\n" << objectList[i]->points[j].x << " " << objectList[i]->points[j].y << " " << objectList[i]->points[j].z;
      
      file << "\n" << objectList[i]->nEdges;
      for (int j = 0 ; j < objectList[i]->nEdges; j++)
        file << "\n" << objectList[i]->edges[j].p1 << " " << objectList[i]->edges[j].p2;
      
      file << "\n" << objectList[i]->nSurface;
      for (int j = 0; j < objectList[i]->nSurface; j++){
        
        for (int k = 0; k < objectList[i]->nSurface; k++){
          file << "\n" << objectList[i]->surfaces[k]->getNormalString();
          file << "\n" << objectList[i]->surfaces[k]->getPntString();
        }
      }
      
    }
    
    file.close();
    return true;
  }
  userInterface::printError("failed to open save file");
  return false;
}
/*
object3Dsurface::object3Dsurface(int npnt, pnt3* Points, int nedge, edge* Edges, int nsurf, surface** surfs) {
  nPoints = npnt;
  points = Points;
  nEdges = nedge;
  edges = Edges;
  nSurface = nsurf;
  surfaces = surfs;
  
  
}*/

