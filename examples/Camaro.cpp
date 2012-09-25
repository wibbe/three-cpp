

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
#include "CTMLoader.h"

#include <iostream>
#include <cassert>

using namespace three;

class CamaroDemo : public Window
{
  public:
    CamaroDemo()
      : Window(1024, 768)
    {
      setTitle("Camaro Example");

      scene = new Scene();

      camera = new PerspectiveCamera(70, 1024.0 / 768.0, 1, 1000);
      camera->position.z = -17;
      camera->position.y = 8;
      camera->lookAt(Vector3(0, 1, 0));
      scene->add(camera);

      camero = CTMLoader::loadMesh("assets/Camaro.ctm");
      assert(camero && "Could not load asserts/Camaro.ctm");

      MeshBasicMaterial * material = new MeshBasicMaterial();
      material->diffuse = Color(1, 0.8, 0.4);
      material->useTextureMap = false;

      camero->material = material;
      scene->add(camero);

      renderer = new GLRenderer();
    }

    void resize(int width, int height)
    {
      renderer->setSize(width, height);
      camera->aspect = width / (float)height;
      camera->updateProjectionMatrix();
    }

    bool update(double dt)
    {
      camero->rotation.y -= dt * 0.3;
      camero->matrixWorldNeedsUpdate = true;

      return !isKeyDown(Key::Esc);
    }

    void paint()
    {
      renderer->render(scene, camera);
    }

  private:
    Renderer * renderer;
    Scene * scene;
    PerspectiveCamera * camera;
    Mesh * camero;
};


int main(int argc, char * argv[])
{
  CamaroDemo demo;
  demo.run();
  return 0;
}
