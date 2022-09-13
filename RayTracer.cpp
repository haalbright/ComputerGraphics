#include "RayTracer.h"
//In Lieu of not having the ADS lighting component, I will prove I read the scene from a file and did the collisions correctly by using one point light and won't be including attenuation in the ray tracing.
void RayTracer::clear() const {
  for(int i = 0; i < width*height; ++i)
    m_frame[i] = glm::vec4(0.f, 0.4f, 0.f, 0.f);
}

void RayTracer::render(const Scene& _scene) const{

  //std::cout<<"-- INSIDE RAYTRACER FUNCTION"<<std::endl;
  const std::vector<Object*>& Obj=_scene.getObjects();
  float extraTau=(fmax(height,width)-width)/2.0;
  float extraSigma=(fmax(height,width)-height)/2.0;
  for(int i=0;i<width;i++){
    //std::cout<<"---- INSIDE I LOOP----"<<std::endl;
    float tau=-1.0+(2.0/float(fmax(height,width)))*(float(i)+ 0.5+extraTau);
    for(int j=0; j<height;j++){//for each pixel, generate a ray
      //std::cout<<"---- INSIDE J LOOP----"<<std::endl;

      float sigma=-1.0+(2.0/float(fmax(width,height)))*(float(j)+ 0.5+extraSigma);
      Ray genRay(glm::vec3(0.f,0.f,0.f),glm::vec3(tau,sigma,-1.0));
      Collision closest=_scene.firstIntersection(genRay);
      //std::cout<<"----- OUTSIDE OBJECT LOOP-----"<<std::endl;

      int pixelIndex=i+j*width;//might be j+i*height
      if(closest.m_type==Collision::Type::kHit){//if collision, get the color of the object it collided with
        glm::vec3 collisionLocation = closest.m_x;
        glm::vec4 collisionColor(closest.m_material->kA);
        for(int l=0;l<_scene.getLights().size();l++){
          const Light& sceneLight = *(_scene.getLights()[l]);
          Ray lightPath(sceneLight.getPos(),collisionLocation-sceneLight.getPos());
          Collision obstruction=_scene.firstIntersection(lightPath);
          float dist1=glm::length(closest.m_x-sceneLight.getPos());
          float dist2=glm::length(obstruction.m_x-sceneLight.getPos());
          float side1=glm::dot(closest.m_normal, lightPath.m_direction);
          float side2=glm::dot(closest.m_normal, genRay.m_direction);
          if(side1*side2>0 && dist2+.01>dist1){

            float angleDimming=abs(glm::dot(obstruction.m_normal,lightPath.m_direction))/(glm::length(obstruction.m_normal)*glm::length(lightPath.m_direction));
            if(angleDimming>0.01){
            collisionColor.x+=angleDimming*sceneLight.getD().x*closest.m_material->kD.x;
            collisionColor.y+=angleDimming*sceneLight.getD().y*closest.m_material->kD.y;
            collisionColor.z+=angleDimming*sceneLight.getD().z*closest.m_material->kD.z;}
          }
        }
        //std::cout<<"---------"<<pixelIndex<<std::endl;
        m_frame[pixelIndex]=collisionColor;
      }
      else{
        m_frame[pixelIndex]=glm::vec4(1.0,1.0,1.0,1.0);
      }
    }
  }
  //std::cout<<"-- OUTSIDE LOOPS--"<<std::endl;

}
