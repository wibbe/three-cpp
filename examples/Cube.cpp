
#include "Window.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Camera.h"
#include "PerspectiveCamera.h"
#include "DefaultMaterials.h"
#include "Texture.h"
#include "ImageUtils.h"
#include "CubeGeometry.h"
#include "opengl/GLRenderer.h"
#include "Code.h"

#include <iostream>

using namespace three;

class CubeDemo : public Window
{
  public:
    CubeDemo()
      : Window(1024, 768)
    {
      setTitle("Cube Example");

      scene = new Scene();

      camera = new PerspectiveCamera(70, 1024.0 / 768.0, 1, 1000);
      camera->position.z = 5;
      scene->add(camera);

      Geometry * geometry = new CubeGeometry(2, 2, 2);
      MeshBasicMaterial * material = new MeshBasicMaterial();
      material->map = ImageUtils::loadTexture("assets/crate.png");
      material->diffuse = Color(1, 1, 1, 1);

      mesh = new Mesh(geometry, material);
      scene->add(mesh);

      renderer = new GLRenderer();
      renderer->setSize(1024, 768);
    }

    bool update(double dt)
    {
      mesh->rotation.x += dt * 0.005;
      mesh->rotation.y += dt * 0.01;

      return !isKeyDown(Key::Esc);
    }
    
    void paint()
    {
      renderer->render(scene, camera);
    }

  private:
    Renderer * renderer;
    Scene * scene;
    Camera * camera;
    Mesh * mesh;
};


int main(int argc, char * argv[])
{
  CubeDemo demo;
  demo.run();
  return 0;
}
