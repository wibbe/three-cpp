

#include "base/Window.h"
#include "base/Scene.h"
#include "base/Mesh.h"
#include "base/PerspectiveCamera.h"
#include "base/OrthographicCamera.h"
#include "material/DefaultMaterials.h"
#include "base/Texture.h"
#include "loader/ImageUtils.h"
#include "base/CubeGeometry.h"
#include "base/PlaneGeometry.h"
#include "opengl/GLRenderer.h"
#include "base/Code.h"
#include "loader/CTMLoader.h"
#include "base/MathUtils.h"
#include "loader/FontLoader.h"
#include "base/UIPanel.h"

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
      camera->position.z = -18;
      camera->position.x = 0;
      camera->position.y = 9;
      camera->lookAt(Vector3(0, 1, 0));

      pivot = new Object();
      pivot->add(camera);

      scene->add(pivot);

      skyMap = ImageUtils::loadCubeTexture(sky, RGBFormat);

      camaro = createCar();
      scene->add(camaro);

      MeshBasicMaterial * groundMaterial = new MeshBasicMaterial();
      groundMaterial->diffuse = Color("#636363");
      groundMaterial->useEnvMap = true;
      groundMaterial->envMap = skyMap;
      groundMaterial->reflectivity = 0.2;

      Mesh * ground = new Mesh(new PlaneGeometry(30, 30, 4, 4), groundMaterial);
      scene->add(ground);

      // UI

      uiScene = new Scene();

      uiCamera = new OrthographicCamera(0, 1024, 0, 768, 1, 1000);
      uiScene->add(uiCamera);

      panel = new UIPanel(FontLoader::loadFont("assets/fonts/UbuntuMono.ttf", 14, 256, 256));
      panel->position = Vector3(5, 5, -1);
      panel->size.y = 200;
      uiScene->add(panel);

      renderer = new GLRenderer();
      renderer->setClearColor(Color("#587CEC"));
      renderer->autoClear = false;

      drag = false;
      mouseX = 0;
      mouseY = 0;
      angle = 0.0f;
      angleSpeed = 0.0f;
    }

    void resize(int width, int height)
    {
      renderer->setSize(width, height);
      camera->aspect = width / (float)height;
      camera->updateProjectionMatrix();

      uiCamera->right = width;
      uiCamera->bottom = height;
      uiCamera->updateProjectionMatrix();
    }

    void mousePressed(int button)
    {
      drag = true;
    }

    void mouseReleased(int button)
    {
      drag = false;
    }

    void mouseMoved(int x, int y)
    {
      int diffX = x - mouseX;
      int diffY = y - mouseY;
      mouseX = x;
      mouseY = y;

      if (drag && !hitPanel)
        angle -= diffX * 0.003f;
    }

    bool update(double dt)
    {
      //camaro->rotation.y = angle;
      //camaro->matrixWorldNeedsUpdate = true;
      pivot->rotation.y = angle;
      pivot->matrixWorldNeedsUpdate = true;

      if (drag && !hitPanel)
      {
        angleSpeed = max(0.0f, angleSpeed - (float)dt * 1.0f);
      }
      else
      {
        angleSpeed = min(0.2f, angleSpeed + (float)dt * 0.2f);
      }

      angle += angleSpeed * dt;

      // Build ui
      hitPanel = panel->begin(getMousePosition(), isMouseDown(Mouse::Left), getMouseScoll());
      panel->label("Camaro Demo");
      panel->separatorLine();

      if (panel->checkbox("Reflection", bodyMaterial->reflectivity > 0.1, true))
      {
        bodyMaterial->reflectivity = bodyMaterial->reflectivity > 0.1 ? 0.0 : 0.2;
        bodyMaterial->needsUpdate = true;
      }

      panel->separatorLine();
      panel->label("Body Color");
      panel->indent();

      panel->slider("Red", &bodyMaterial->diffuse.r, 0.0f, 1.0f, 0.01f, true);
      panel->slider("Green", &bodyMaterial->diffuse.g, 0.0f, 1.0f, 0.01f, true);
      panel->slider("Blue", &bodyMaterial->diffuse.b, 0.0f, 1.0f, 0.01f, true);

      panel->unindent();

      panel->separatorLine();
      bool quit = panel->button("Quit");
      panel->end();

      return !(isKeyDown(Key::Esc) || quit);
    }

    void paint()
    {
      renderer->render(scene, camera, 0, true);
      renderer->render(uiScene, uiCamera);
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

      bodyMaterial = new MeshBasicMaterial();
      bodyMaterial->name = "body";
      bodyMaterial->useEnvMap = true;
      bodyMaterial->diffuse = Color("#CE6618");
      bodyMaterial->envMap = skyMap;
      bodyMaterial->reflectivity = 0.2;
      bodyMaterial->needsUpdate = true;

      MeshBasicMaterial * cromeMaterial = new MeshBasicMaterial();
      cromeMaterial->useEnvMap = true;
      cromeMaterial->envMap = skyMap;
      cromeMaterial->reflectivity = 0.9;

      MeshBasicMaterial * windowMaterial = new MeshBasicMaterial();
      windowMaterial->gammaCorrection = true;
      windowMaterial->transparent = true;
      windowMaterial->opacity = 0.5;
      windowMaterial->diffuse = Color(0.3, 0.3, 0.4);
      windowMaterial->useEnvMap = true;
      windowMaterial->useEnvMap = skyMap;
      windowMaterial->reflectivity = 0.8;

      MeshBasicMaterial * tireMaterial = new MeshBasicMaterial();
      tireMaterial->diffuse = Color("#1A1A1A");

      MeshBasicMaterial * blackMaterial = new MeshBasicMaterial();
      blackMaterial->diffuse = Color("#000");

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
    Scene * uiScene;
    UIPanel * panel;
    PerspectiveCamera * camera;
    OrthographicCamera * uiCamera;
    Object * camaro;
    Object * pivot;
    Texture * skyMap;

    MeshBasicMaterial * bodyMaterial;

    bool hitPanel;
    bool drag;
    int mouseX, mouseY;
    float angle, angleSpeed;
};


int main(int argc, char * argv[])
{
  CamaroDemo demo;
  demo.run();
  return 0;
}
