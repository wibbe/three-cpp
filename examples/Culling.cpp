
#include "base/Window.h"
#include "base/Scene.h"
#include "base/Mesh.h"
#include "base/PerspectiveCamera.h"
#include "base/OrthographicCamera.h"
#include "base/UIPanel.h"
#include "base/Texture.h"
#include "base/Font.h"
#include "base/CubeGeometry.h"
#include "base/PlaneGeometry.h"
#include "opengl/GLRenderer.h"
#include "loader/ImageUtils.h"
#include "loader/FontLoader.h"
#include "material/DefaultMaterials.h"
#include "util/GeometryHelper.h"
#include "extras/PoissonSet.h"

#include <iostream>
#include <stdlib.h>

using namespace three;

Mesh * createTree(Material * material)
{
  Mesh  * mesh = new Mesh(new Geometry(), material);

  float height = 1.5f + (rand() / (float)RAND_MAX) * 1.0f;

  // Steam
  Matrix4 transform;
  transform.setPosition(Vector3(0, height * 0.5, 0));

  addBox(mesh->geometry, transform, Vector3(0.15, height, 0.15), Color("#6D3926"));
  return mesh;
}

Mesh * createGround(Material * material)
{
  Mesh  * mesh = new Mesh(new Geometry(), material);
  addPlane(mesh->geometry, 1000, 1000, 20, 20, Color("#61AE61"));

  return mesh;
}

void createTrees(float density, float width, float height, Scene * scene, Material * material)
{
  std::vector<Vector2> positions;
  generatePoissonSet(positions, width, height, density, 3294);

  Object * root = new Object();
  root->position.x = width * -0.5;
  root->position.z = height * -0.5;

  for (std::vector<Vector2>::iterator it = positions.begin(), end = positions.end(); it != end; ++it)
  {
    Vector2 pos = *it;

    Mesh * mesh = createTree(material);
    mesh->position.x = pos.x;
    mesh->position.z = pos.y;
    root->add(mesh);
  }

  scene->add(root);
}

class CullingDemo : public Window
{
  public:
    CullingDemo()
      : Window(1.0 / 60.0, 1024, 768)
    {
      setTitle("Culling Demo");

      MeshBasicMaterial * colorMaterial = new MeshBasicMaterial();
      colorMaterial->diffuse = Color("#FFF");
      colorMaterial->gammaCorrection = true;
      colorMaterial->useVertexColor = true;

      scene = new Scene();

      camera = new PerspectiveCamera(70, 1024.0 / 768.0, 1, 1000);
      camera->position.z = -5;
      camera->position.y = 2;
      camera->position.x = 1;
      camera->lookAt(Vector3(0, 0, 0));
      scene->add(camera);

      Geometry * geometry = new CubeGeometry(2, 2, 2);

      scene->add(createGround(colorMaterial));

      createTrees(2.2, 300, 300, scene, colorMaterial);

      // -- Create UI --
      uiScene = new Scene();
      uiCamera = new OrthographicCamera(0, 1024, 0, 768, 1, 100);
      uiScene->add(uiCamera);

      ui = new UIPanel(FontLoader::loadFont("assets/fonts/UbuntuLight.ttf", 14.0, 256, 256));
      ui->position = Vector3(5, 5, -1);
      ui->size.y = 145;
      uiScene->add(ui);

      renderer = new GLRenderer();
      renderer->setClearColor(Color("#85B3B7"));
      renderer->autoClear = false;
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

    bool update(double dt)
    {
      ui->begin(getMousePosition(), isMouseDown(Mouse::Left), getMouseScoll());

      ui->value("Batches: %d", renderer->stats.batchCount);
      ui->value("Polygons: %d", renderer->stats.polygonCount);
      ui->value("Texture changes: %d", renderer->stats.textureChanges);
      ui->value("Shader changes: %d", renderer->stats.shaderChanges);
      ui->value("Render Target changes: %d", renderer->stats.renderTargetChanges);

      ui->separatorLine();

      bool quit = ui->button("Quit");

      ui->end();

      return !(isKeyDown(Key::Esc) || quit);
    }

    void paint()
    {
      renderer->stats.reset();
      renderer->render(scene, camera, 0, true);
      renderer->render(uiScene, uiCamera);
    }

  private:
    Renderer * renderer;
    Scene * scene;
    PerspectiveCamera * camera;

    OrthographicCamera * uiCamera;
    Scene * uiScene;
    UIPanel * ui;
};


int main(int argc, char * argv[])
{
  CullingDemo demo;
  demo.run();
  return 0;
}
