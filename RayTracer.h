#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

// GL
#include "GLInclude.h"

// Engine
#include "Renderer.h"
#include "Collision.h"
#include "Scene.h"
////////////////////////////////////////////////////////////////////////////////
/// @brief Ray tracer
////////////////////////////////////////////////////////////////////////////////
class RayTracer : public Renderer {
  public:
    RayTracer(){
      width=1;
      height=1;
      m_frame=std::make_unique<glm::vec4[]>(width*height);
      std::cout<<"RayTracer constructed"<<std::endl;
    }
    void clear() const override;
    void render(const Scene& _scene) const override;
    std::unique_ptr<glm::vec4[]>& getFrame() {return m_frame;}
    void resize(int w, int h){
      width=w;
      height=h;
      m_frame.reset();
      m_frame=std::make_unique<glm::vec4[]>(w*h);
    }
  private:
    std::unique_ptr<glm::vec4[]> m_frame; ///< Framebuffer
    int width;
    int height;
};

#endif
