#include "obj.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <math.h>

#define PI 3.14159265

using namespace std;

unsigned int obj::nObjects;
obj* obj::objectList;
char* obj::storedFileName;
    

obj::obj(unsigned int numPoints, pnt* points){
  nPoints = numPoints;
  pointList = points;
}

void obj::load(char* filename){
  storedFileName = filename;
  
  ifstream file(filename, ios::in);
  
  if(file.is_open()){
    string line;
    
    /*
     * Get the first line of the file
     * It should be number of objects.
     * If bad input close file and spit out error.
     */
    if(getline(file, line)){
      int num = atoi(line.c_str());
      
      if (num < 0){
        cout << "negative objects?!?!\n";
        file.close();
        return;
      }
      else if (num == 0){
        cout << "bad input or zero objects\n";  
        file.close();
        return;
      }
      
      nObjects = num;
      objectList = (obj*)malloc(sizeof(obj) * nObjects);
    }
    else{
      cout << "Empty file!\n";
      file.close();
      return;
    }
    
    /*
     * iterate through the number of objects
     */
    
    for(int i = 0; i < nObjects; i++){
      int num;
      if(getline(file, line)){
        num = atoi(line.c_str());

        if (num < 0){
          cout << "negative points?!?!\n";
          file.close();
          return;
        }
        else if (num == 0){
          cout << "bad input or zero points\n";  
          file.close();
          return;
        }
      }
      else{
        cout << "Missing Object definition\n";
        file.close();
        return;
      }
      
      /*
       * Iterate through the number of points
       */
      pnt* points = (pnt*)malloc(sizeof(pnt) * num);
      for (int j = 0; j < num; j++){
        if(getline(file, line)){
          int del = line.find_first_of(' ');
          if (del < 1){
            cout << "Bad point delimiter\n";
            file.close();
            return;
          }
          
          int x = atoi(line.substr(0, del).c_str());
          int y = atoi(line.substr(del+1).c_str());
          points[j].x = x;
          points[j].y = y;
          
          
        }
        else{
          cout << "Missing Point definition\n";
          file.close();
          return;
        }
      }
      
      //Build Object
      objectList[i] = obj(num, points);
    }
    
    file.close();
  }
  else cout << "Unable to open file\n";
  
  return;
}

string obj::getString(){
  string str = "NumPoints: ";
  char* temp;
  itoa(nPoints, temp, 10);
  str.append(temp);
  str.append("\n");
  
  return str;
};

string obj::getTotalString(){
  string str = "Number of Objects: ";
  char* temp;
  itoa(nObjects, temp, 10);
  str.append(temp);
  str.append("\n");
  
  return str;
}

obj::~obj(){
//  free(pointList);
}

void obj::save(char* filename){
  ofstream file(filename);
  if (file.is_open()){
    file << nObjects;
    for (int i = 0; i < nObjects; i++){
      file << "\n" << objectList[i].nPoints;
      for (int j = 0 ; j < objectList[i].nPoints; j++){
        file << "\n" << objectList[i].pointList[j].x << " " << objectList[i].pointList[j].y;
      }
    }
    
    file.close();
  }
  else cout << "failed to open save file";
}

/*
 * gets the calculated centroid. average of all points
 */
obj::pnt obj::getCentroid(){
  float x = 0;
  float y = 0;
  for (int i = 0; i < nPoints; i++){
    x += pointList[i].x;
    y += pointList[i].y;
  }
  
  x /= nPoints;
  y /= nPoints;
  
  pnt c;
  c.x = (int)(x+.5);
  c.y = (int)(y+.5);
  return c;
}

/*
 * move the object by x and y
 */
void obj::translation(int x, int y){
  for (int i = 0; i < nPoints; i++){
    pointList[i].x += x;
    pointList[i].y += y;
  }
}

/*
 * scales object from the centroid
 */
void obj::scale(float a, float b){
  pnt c = getCentroid();
  
  //const for centroid scaling. also +.5 for rounding
  const double px = c.x *(1-a) + .5, py = c.y * (1 - b) + .5;
  
  //iterate through each point updating their locations for scale
  for(int i = 0; i < nPoints; i++){
    pointList[i].x = (int)(pointList[i].x * a + px);
    pointList[i].y = (int)(pointList[i].y * b + py);
  }
}

/*
 * rotates object around the centroid
 */
void obj::rotation(float alpha){
  pnt c = getCentroid();
  
  //const for sin and cosin calculations
  const double radians = alpha*PI/180;
  const double sa = sin(radians), ca = cos(radians);
  //const for centroid rotation. also +.5 for rounding
  const double px = c.y*sa - c.x*ca + c.x+.5, py = c.y - c.x*sa - ca*c.y +.5;
  
  for(int i = 0; i < nPoints; i++){
    int x = (int)(pointList[i].x*ca - pointList[i].y*sa + px);
    pointList[i].y = (int)(pointList[i].x*sa + pointList[i].y*ca + py);
    pointList[i].x = x;
  }
}
