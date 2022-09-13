#ifndef _SPHERE_H_
#define _SPHERE_H_

// GL
#include "GLInclude.h"

// Engine
#include "Collision.h"
#include "Material.h"
#include "Object.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief Sphere
////////////////////////////////////////////////////////////////////////////////
class Sphere : public Object {
  public:
    Sphere(const glm::vec3 _c,
           float _r, Material _m) :
      m_center{_c}, m_radius{_r}, m_material{_m} {}

    Collision collide(const Ray& _ray) const override;
    void printFunc()override;
    glm::vec3 getCenter(){return m_center;}
    float getRadius(){return m_radius;}
    Material getMaterial(){return m_material;}
  private:
    glm::vec3  m_center; ///< Center
    float      m_radius; ///< Radius
    Material m_material; ///< Material
};

#endif
