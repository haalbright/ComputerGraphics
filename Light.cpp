#include <iostream>//stringstream
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Sphere.h"
#include "GLInclude.h"
#include "Light.h"
#include "Object.h"

void Light::printFunc()const{
  std::cout<<"--Light Object--"<<std::endl;
  std::cout<<"Position:("<<position[0]<<", "<<position[1]<<", "<<position[2]<<")"<<std::endl;
  std::cout<<"iD: {"<<iD[0]<<", "<<iD[1]<<", "<<iD[2]<<", "<<iD[3]<<"}"<<std::endl;
  std::cout<<"iA: {"<<iA[0]<<", "<<iA[1]<<", "<<iA[2]<<", "<<iA[3]<<"}"<<std::endl;
  std::cout<<"iS: {"<<iS[0]<<", "<<iS[1]<<", "<<iS[2]<<", "<<iS[3]<<"}"<<std::endl;
  std::cout<<"Light Type: "<<type<<std::endl;
  std::cout<<std::endl;
  std::cout<<std::endl;
}
