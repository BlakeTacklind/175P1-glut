#include "obj.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

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
  
//  cout << objectList[0].getString();
  cout << getTotalString();
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
  
  cout << objectList[0].getString();
  
  for(int i = 0; i < nObjects; i++)
    cout << i;
    //    cout << objectList[i].getString();
  
  return str;
}
