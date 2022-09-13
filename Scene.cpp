#include <iostream>//stringstream
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Collision.h"
//#include "Ray.h"
//#include "Light.h"
#include "Object.h"
#include <stdlib.h>
//#include "GLInclude.h"
//#include "Camera.h"
//#include "Sphere.h"
#include "Scene.h"
#include "Plane.h"

Collision Scene::firstIntersection (const Ray& _ray)const{
  Collision closest;
  for (int k=0;k<mObjects.size();k++){//for every object in the scene....
    Collision collision(mObjects[k]->collide(_ray));//cast the ray
    if(collision.m_type==Collision::Type::kHit){//if there is a collision, check if it's the only one
      if(closest.m_type==Collision::Type::kMiss){
        closest=collision;
      }
      else{//multiple collisions, must find closest
        float dist1=glm::dot(_ray.m_direction,closest.m_x);
        float dist2=glm::dot(_ray.m_direction, collision.m_x);
        if(dist2<dist1){
          closest=collision;
        }
      }
    }
  }
  return closest;
}

float sTof(const std::string& str){
      std::stringstream convert1(str);
      float f;
      convert1>>f;
      return f;
  }

  std::vector<std::string> token(std::string str){
    std::vector<std::string> splitStr;
    int found;
    while(str.find(" ")!=-1){
      found=str.find(" ");
      splitStr.push_back(str.substr(0,found));
      str=str.substr(found+1);
    }
    splitStr.push_back(str);
    return splitStr;
  }

  Material readMaterial(std::ifstream* inFile){
    std::string line;
    Material aMaterial;
    while(getline(*inFile,line)){
      if(line=="~M"){
        break;
      }
      std::vector<std::string> parsed=token(line);
      if(parsed[0]=="kD"){
        aMaterial.kD=glm::vec4(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]), sTof(parsed[4]));
      }
      else if(parsed[0]=="kA"){
        aMaterial.kA=glm::vec4(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]), sTof(parsed[4]));
      }
      else if(parsed[0]=="kS"){
        aMaterial.kS=glm::vec4(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]), sTof(parsed[4]));
      }
      else if(parsed[0]=="hasTexture"){
        if (parsed[1]=="false"){
          aMaterial.hasTexture=false;
        }
        else if(parsed[1]=="true") {
          aMaterial.hasTexture=true;
        }
        else {
          std::cout<<"ERROR: Material can't have hasTexture="<<parsed[1]<<std::endl;
        }
      }
      else if(parsed[0]=="pS"){
        aMaterial.rho=sTof(parsed[1]);
      }
      else {
        std::cout<<"Error: Unknown material attribute: "<<line<<std::endl;
      }
    }
    return aMaterial;
  }


void Scene::readFromFile (const std::string& filename){
  std::string line;
  std::ifstream inFile;
  inFile.open(filename);
  if (inFile.is_open()){
    while(getline(inFile,line)){
      if(line=="Sphere"){
        glm::vec3 vCenter;
        float vRadius;
        Material m;
        while(getline(inFile,line)){
          if(line=="~Sphere"){
            break;
          }
          std::vector<std::string> parsed=token(line);
          if(parsed[0]=="Center"){
            vCenter=glm::vec3(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]));
          }
          else if (parsed[0]=="Radius"){
            vRadius=sTof(parsed[1]);
          }
          else if(parsed[0]=="M"){
            m=readMaterial(&inFile);
          }
          else{
            std::cout<<"ERROR: Unknown Sphere attribute: "<<line<<std::endl;
          }
        }
        mObjects.push_back(new Sphere(vCenter, vRadius, m));
      }
      else if(line=="Plane"){
        glm::vec3 vPosition,vNormal;
        Material m;
        while(getline(inFile,line)){
          if(line=="~Plane"){
            break;
          }
          std::vector<std::string> parsed=token(line);
          if(parsed[0]=="P"){
            vPosition=glm::vec3(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]));
          }
          else if (parsed[0]=="N"){
            vNormal=glm::vec3(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]));
          }
          else if(parsed[0]=="M"){
            m=readMaterial(&inFile);
          }
          else{
            std::cout<<"ERROR: Unknown Plane attribute: "<<line<<std::endl;
          }
        }
        mObjects.push_back(new Plane(vPosition, vNormal, m));
      }

      else if(line=="Light"){
        glm::vec4 vA, vD, vS;
        glm::vec3 vPosition;
        int type;
        while(getline(inFile,line)){
          if(line=="~Light"){
            break;
          }
          std::vector<std::string> parsed=token(line);
          if(parsed[0]=="Position"){
            vPosition=glm::vec3(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]));
          }
          else if (parsed[0]=="iA"){
            vA=glm::vec4(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]), sTof(parsed[4]));
          }
          else if (parsed[0]=="iD"){
            vD=glm::vec4(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]), sTof(parsed[4]));
          }
          else if (parsed[0]=="iS"){
            vS=glm::vec4(sTof(parsed[1]),sTof(parsed[2]),sTof(parsed[3]), sTof(parsed[4]));
          }
          else if (parsed[0]=="Type"){
            type=sTof(parsed[1]);
          }
          else{
            std::cout<<"ERROR: Unknown Light attribute: "<<line<<std::endl;
          }
        }
        mLights.push_back(new Light(vA,vD,vS,type,vPosition));
      }
      else{
        std::cout<<"ERROR: Unknown Object Type: "<<line<<std::endl;
      }
    }
    inFile.close();
  }
  else{
    std::cout<<"File can't be opened"<<std::endl;
  }
}

void Scene::printFunc(){
  for(int i=0;i<mObjects.size();i++){
    mObjects[i]->printFunc();
  }
  for(int k=0;k<mLights.size();k++){
    mLights[k]->printFunc();
  }
}
