#ifndef _SCENE_H_
#define _SCENE_H_
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "Ray.h"
#include "Light.h"
#include "GLInclude.h"
#include "Camera.h"
#include "Sphere.h"


class Scene {
  public:
    Scene(): mCam(new Camera()){};
    //~Scene();
    void readFromFile (const std::string& filename);
    Collision firstIntersection(const Ray& _ray)const;
    glm::vec3 parseVec3(const std::string& vecLine);
    void printFunc();
    const std::vector<Object*>& getObjects() const {return mObjects;}
    const std::vector<Light*>& getLights()const {return mLights;}
    const Camera* getCam() {return mCam;}
  private:
    std::vector<Object*> mObjects;
    std::vector<Light*> mLights;
    Camera* mCam;
};

#endif
