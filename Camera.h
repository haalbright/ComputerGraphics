#ifndef _CAMERA_H_
#define _CAMERA_H_

// GL
#include "GLInclude.h"

// Engine
#include "Collision.h"
#include "Material.h"
#include "Object.h"
////////////////////////////////////////////////////////////////////////////////
/// @brief Camera
////////////////////////////////////////////////////////////////////////////////

class Camera{
public:
  float aRatio=16.0/9.0;
  Camera():mPerspective(glm::perspective(glm::radians(45.0f), (float)aRatio, 0.1f, 1000.0f)), mEye(glm::vec3(0.0f,0.0f,0.0f)){};
  const glm::vec3& eye() {return mEye;}
private:
  glm::vec3 mEye;
  glm::mat4 mPerspective;
};

#endif
