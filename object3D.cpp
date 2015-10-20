/* 
 * File:   object3D.cpp
 * Author: btacklind
 * 
 * Created on October 20, 2015, 12:05 PM
 */

#include "object3D.h"
#include <string>
#include <fstream>
#include "userInterface.h"
#include <list>

char* object3D::storedFileName;
unsigned int object3D::nObjects;

object3D::object3D(int npnt, pnt3* Points, int nedge, edge* Edges){
  nPoints = npnt;
  points = Points;
  nEdge = nedge;
  edges = Edges;
}

object3D::object3D() {
  nPoints = 0;
  nEdge = 0;
}

object3D::object3D(const object3D& orig) {
}

object3D::~object3D() {
  delete [] points;
  delete [] edges;
}

template <class E>
E* getArrFromList(list<E> l){
  E* arr = new E[l.size()];
  
  typename list<E>::iterator itr = l.begin();
  for(int i = 0; itr != l.end(); itr++, i++)
    arr[i] = *itr;
  
  return arr;
}

bool object3D::load(char* filename){
  storedFileName = filename;
  
  ifstream file(filename, ios::in);
  
  if(file.is_open()){
    list<object3D*> tempList;
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
        file.close();
        return false;
      }
      else if (num == 0){
        userInterface::printError("bad input or zero objects");  
        file.close();
        return false;
      }
      
      nObjects = num;
    }
    else{
      userInterface::printError("Empty file!");
      file.close();
      objectList = getArrFromList(tempList);
      nObjects = tempList.size();
      return false;
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
          file.close();
          objectList = getArrFromList(tempList);
          nObjects = tempList.size();
          return false;
        }
        else if (num == 0){
          userInterface::printError("bad input or zero points");  
          file.close();
          objectList = getArrFromList(tempList);
          nObjects = tempList.size();
          return false;
        }
      }
      else{
        userInterface::printError("Missing Object definition");
        file.close();
        objectList = getArrFromList(tempList);
        nObjects = tempList.size();
        return false;
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
            file.close();
            objectList = getArrFromList(tempList);
            nObjects = tempList.size();
            return false;
          }
          
          float x = atof(line.substr(0, del).c_str());
          float y = atof(line.substr(del+1, del2).c_str());
          float z = atof(line.substr(del2+1).c_str());
          
          points[j].x = x;
          points[j].y = y;
          points[j].z = z;
        }
        else{
          userInterface::printError("Missing Point definition");
          file.close();
          objectList = getArrFromList(tempList);
          nObjects = tempList.size();
          return false;
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
          file.close();
          objectList = getArrFromList(tempList);
          nObjects = tempList.size();
          return false;
        }
        else if (num == 0){
          userInterface::printError("Bad input or zero points");  
          file.close();
          objectList = getArrFromList(tempList);
          nObjects = tempList.size();
          return false;
        }
      }
      else{
        userInterface::printError("Missing Object edge definition");
        file.close();
        objectList = getArrFromList(tempList);
        nObjects = tempList.size();
        return false;
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
            file.close();
            objectList = getArrFromList(tempList);
            nObjects = tempList.size();
            return false;
          }
          
          int x = atoi(line.substr(0, del).c_str());
          int y = atoi(line.substr(del+1).c_str());
          e[j].p1 = x;
          e[j].p2 = y;
        }
        else{
          userInterface::printError("Missing Edge definition");
          file.close();
          objectList = getArrFromList(tempList);
          nObjects = tempList.size();
          return false;
        }
      }
      
      //Build Object
      tempList.push_front(new object3D(num, points, num2, e));
    }
    
    objectList = getArrFromList(tempList);
    nObjects = tempList.size();
    
    file.close();
    return true;
  }
  userInterface::printError("Unable to open file");
  
  return false;
}

/*
 * save objects to file
 */
bool object3D::save(char* filename){
  storedFileName = filename;
  
  ofstream file(filename);
  if (file.is_open()){
    file << nObjects;
    for (int i = 0; i < nObjects; i++){
      file << "\n" << objectList[i]->nPoints;
      for (int j = 0 ; j < objectList[i]->nPoints; j++)
        file << "\n" << objectList[i]->points[j].x << " " << objectList[i]->points[j].y << " " << objectList[i]->points[j].z;
      
      for (int j = 0 ; j < objectList[i]->nEdge; j++)
        file << "\n" << objectList[i]->edges[j].p1 << " " << objectList[i]->edges[j].p2;
    }
    
    file.close();
    return true;
  }
  userInterface::printError("failed to open save file");
  return false;
}


