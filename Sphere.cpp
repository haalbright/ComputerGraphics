#include <iostream>//stringstream
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include "Sphere.h"
#include "GLInclude.h"
#include "Object.h"
Collision Sphere::collide(const Ray& _ray) const {
  glm::vec3 posRS=_ray.m_origin-m_center;
  float A=dot(_ray.m_direction, _ray.m_direction);
  float B=dot(posRS,2*_ray.m_direction);
  float C=dot(posRS,posRS)-(m_radius*m_radius);
  float discriminant=B*B-(4*A*C);
  if (discriminant<0){//discriminant is negative, no collision
      return Collision();
  }
  float numerator=B*-1 - sqrt(discriminant);
  bool isInsideSphere=false;
  if (numerator<0){
    numerator=B*-1 + sqrt(discriminant);
    isInsideSphere=true;
  }
  if(numerator<0){//no collision
    return Collision();
  }
  float t=numerator/(2*A);
  glm::vec3 cPosition=_ray.m_origin + _ray.m_direction*t;
  glm::vec3 cNormal=(cPosition-m_center)/m_radius;
  if(isInsideSphere){
    cNormal=-1.0*cNormal;
  }
  return Collision(cPosition, cNormal, &m_material);
}

void Sphere::printFunc(){
  std::cout<<"--Sphere Object--"<<std::endl;
  std::cout<<"Center:("<<m_center[0]<<", "<<m_center[1]<<", "<<m_center[2]<<")"<<std::endl;
  std::cout<<"Radius: "<<m_radius<<std::endl;
  std::cout<<"--Sphere Material-- "<<std::endl;
  std::cout<<"kD: {"<<m_material.kD[0]<<", "<<m_material.kD[1]<<", "<<m_material.kD[2]<<", "<<m_material.kD[3]<<"}"<<std::endl;
  std::cout<<"kA: {"<<m_material.kA[0]<<", "<<m_material.kA[1]<<", "<<m_material.kA[2]<<", "<<m_material.kA[3]<<"}"<<std::endl;
  std::cout<<"kS: {"<<m_material.kS[0]<<", "<<m_material.kS[1]<<", "<<m_material.kS[2]<<", "<<m_material.kS[3]<<"}"<<std::endl;
  std::cout<<"kA: {"<<m_material.kA[0]<<", "<<m_material.kA[1]<<", "<<m_material.kA[2]<<", "<<m_material.kA[3]<<"}"<<std::endl;
  std::cout<<"kA: {"<<m_material.kA[0]<<", "<<m_material.kA[1]<<", "<<m_material.kA[2]<<", "<<m_material.kA[3]<<"}"<<std::endl;
  std::cout<<"Rho: "<<m_material.rho<<std::endl;
  std::cout<<std::endl;
  std::cout<<std::endl;
}
