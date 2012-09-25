

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
      const char * sky[6] = {
        "assets/sky/teide_posx.jpg",
        "assets/sky/teide_negx.jpg",
        "assets/sky/teide_posy.jpg",
        "assets/sky/teide_negy.jpg",
        "assets/sky/teide_posz.jpg",
        "assets/sky/teide_negz.jpg",
      };

      setTitle("Camaro Example");

      scene = new Scene();

      camera = new PerspectiveCamera(70, 1024.0 / 768.0, 1, 1000);
      camera->position.z = -17;
      camera->position.y = 8;
      camera->lookAt(Vector3(0, 1, 0));
      scene->add(camera);

      skyMap = ImageUtils::loadCubeTexture(sky, RGBFormat);

      camaro = createCar();
      scene->add(camaro);

      renderer = new GLRenderer();
      renderer->setClearColor(Color(0.7, 0.7, 0.7));
    }

    void resize(int width, int height)
    {
      renderer->setSize(width, height);
      camera->aspect = width / (float)height;
      camera->updateProjectionMatrix();
    }

    bool update(double dt)
    {
      camaro->rotation.y -= dt * 0.3;
      camaro->matrixWorldNeedsUpdate = true;

      return !isKeyDown(Key::Esc);
    }

    void paint()
    {
      renderer->render(scene, camera);
    }

    Object * createCar()
    {
      Mesh * body = CTMLoader::loadMesh("assets/models/camaro/body.ctm");
      Mesh * black = CTMLoader::loadMesh("assets/models/camaro/black.ctm");
      Mesh * black2 = CTMLoader::loadMesh("assets/models/camaro/black2.ctm");
      Mesh * crome = CTMLoader::loadMesh("assets/models/camaro/crome.ctm");
      Mesh * intake = CTMLoader::loadMesh("assets/models/camaro/intake.ctm");
      Mesh * interior = CTMLoader::loadMesh("assets/models/camaro/interior.ctm");
      Mesh * tire = CTMLoader::loadMesh("assets/models/camaro/tire.ctm");
      Mesh * tireRim = CTMLoader::loadMesh("assets/models/camaro/tire_rim.ctm");
      Mesh * window = CTMLoader::loadMesh("assets/models/camaro/window.ctm");
      assert(body && black && black2 && crome && intake && interior && tire && tireRim && window && "Could not Camaro model");

      MeshBasicMaterial * bodyMaterial = new MeshBasicMaterial();
      bodyMaterial->useEnvMap = true;
      bodyMaterial->gammaCorrection = true;
      bodyMaterial->diffuse = Color(1, 0.5, 0.0);
      bodyMaterial->envMap = skyMap;
      bodyMaterial->reflectivity = 0;
      bodyMaterial->combine = 0;

      MeshBasicMaterial * cromeMaterial = new MeshBasicMaterial();
      cromeMaterial->gammaCorrection = true;
      cromeMaterial->useEnvMap = true;
      cromeMaterial->envMap = skyMap;
      cromeMaterial->reflectivity = 0.5;

      MeshBasicMaterial * windowMaterial = new MeshBasicMaterial();
      windowMaterial->gammaCorrection = true;
      windowMaterial->transparent = true;
      windowMaterial->opacity = 0.8;
      windowMaterial->diffuse = Color(0.3, 0.3, 0.4);
      windowMaterial->useEnvMap = true;
      windowMaterial->useEnvMap = skyMap;
      windowMaterial->reflectivity = 1;

      MeshBasicMaterial * tireMaterial = new MeshBasicMaterial();
      tireMaterial->diffuse = Color(0.1, 0.1, 0.1);

      MeshBasicMaterial * blackMaterial = new MeshBasicMaterial();
      blackMaterial->diffuse = Color(0.0, 0.0, 0.0);

      body->material = bodyMaterial;
      black->material = blackMaterial;
      black2->material = blackMaterial;
      crome->material = cromeMaterial;
      intake->material = bodyMaterial;
      interior->material = blackMaterial;
      tire->material = tireMaterial;
      tireRim->material = cromeMaterial;
      window->material = windowMaterial;

      Object * car = new Object();
      car->add(body);
      car->add(black);
      car->add(black2);
      car->add(crome);
      car->add(intake);
      car->add(interior);
      car->add(tire);
      car->add(tireRim);
      car->add(window);

      return car;
    }

  private:
    Renderer * renderer;
    Scene * scene;
    PerspectiveCamera * camera;
    Object * camaro;
    Texture * skyMap;
};


int main(int argc, char * argv[])
{
  CamaroDemo demo;
  demo.run();
  return 0;
}
