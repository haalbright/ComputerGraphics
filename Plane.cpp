#include <iostream>//stringstream
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Sphere.h"
#include "GLInclude.h"
#include "Plane.h"
#include "Object.h"
Collision Plane::collide(const Ray& _ray) const {
  glm::vec3 posP=position-_ray.m_origin;
  float numerator=dot(posP, normal);
  float denominator=dot(_ray.m_direction,normal);
  if(denominator!=0 && numerator/denominator>0){
    float t=numerator/denominator;
    glm::vec3 cPosition=_ray.at(t);
    //std::cout<<"Collision with plane found at: "<<cPosition[0]<<", "<<cPosition[1]<<", "<<cPosition[2]<<std::endl;
    return Collision(cPosition,normal,&m);
  }
  //std::cout<<"No collision found"<<std::endl;
  return Collision();
}

void Plane::printFunc(){
  std::cout<<"--Plane Object--"<<std::endl;
  std::cout<<"Position: ("<<position[0]<<", "<<position[1]<<", "<<position[2]<<")"<<std::endl;
  std::cout<<"Normal: ("<<normal[0]<<", "<<normal[1]<<", "<<normal[2]<<")"<<std::endl;
  std::cout<<"--Plane Material-- "<<std::endl;
  std::cout<<"kD: {"<<m.kD[0]<<", "<<m.kD[1]<<", "<<m.kD[2]<<", "<<m.kD[3]<<"}"<<std::endl;
  std::cout<<"kA: {"<<m.kA[0]<<", "<<m.kA[1]<<", "<<m.kA[2]<<", "<<m.kA[3]<<"}"<<std::endl;
  std::cout<<"kS: {"<<m.kS[0]<<", "<<m.kS[1]<<", "<<m.kS[2]<<", "<<m.kS[3]<<"}"<<std::endl;
  std::cout<<"kA: {"<<m.kA[0]<<", "<<m.kA[1]<<", "<<m.kA[2]<<", "<<m.kA[3]<<"}"<<std::endl;
  std::cout<<"kA: {"<<m.kA[0]<<", "<<m.kA[1]<<", "<<m.kA[2]<<", "<<m.kA[3]<<"}"<<std::endl;
  std::cout<<"Rho: "<<m.rho<<std::endl;
  std::cout<<std::endl;
  std::cout<<std::endl;
}
