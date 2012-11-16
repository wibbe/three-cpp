
#include "base/Window.h"
#include "base/Scene.h"
#include "base/Camera.h"
#include "base/Mesh.h"
#include "base/Camera.h"
#include "base/PerspectiveCamera.h"
#include "material/DefaultMaterials.h"
#include "base/Texture.h"
#include "loader/ImageUtils.h"
#include "base/CubeGeometry.h"
#include "opengl/GLRenderer.h"
#include "base/RenderTarget.h"

#include <iostream>

using namespace three;

class RenderToTexture : public Window
{
  public:
    RenderToTexture()
      : Window(512, 512)
    {
      setTitle("Cube Example");

      scene = new Scene();

      target = new RenderTarget(512, 512);

      camera = new PerspectiveCamera(70, 512.0 / 512.0, 1, 1000);
      camera->position.z = -3;
      camera->position.y = 2;
      camera->lookAt(Vector3(0, 0, 0));
      scene->add(camera);

      Geometry * geometry = new CubeGeometry(2, 2, 2);

      colorMaterial = new MeshBasicMaterial();
      colorMaterial->map = ImageUtils::loadTexture("assets/textures/crate.png");
      colorMaterial->diffuse = Color("#A81515");
      colorMaterial->gammaCorrection = true;
      colorMaterial->useTextureMap = true;

      textureMaterial = new MeshBasicMaterial();
      textureMaterial->map = target->colorTexture;
      textureMaterial->diffuse = Color(1, 1, 1);
      textureMaterial->useTextureMap = true;
      textureMaterial->gammaCorrection = true;

      mesh = new Mesh(geometry, colorMaterial);
      scene->add(mesh);

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
      mesh->rotation.x += dt * 0.05;
      mesh->rotation.y += dt * 0.1;
      mesh->matrixWorldNeedsUpdate = true;

      return !isKeyDown(Key::Esc);
    }

    void paint()
    {
      mesh->material = colorMaterial;
      renderer->setClearColor(Color("#005300"));
      renderer->render(scene, camera, target);

      mesh->material = textureMaterial;
      renderer->setClearColor(Color("#BEE7FF"));
      renderer->render(scene, camera);
    }

  private:
    Renderer * renderer;
    Scene * scene;
    PerspectiveCamera * camera;
    Mesh * mesh;
    RenderTarget * target;
    MeshBasicMaterial * colorMaterial;
    MeshBasicMaterial * textureMaterial;
};


int main(int argc, char * argv[])
{
  RenderToTexture demo;
  demo.run();
  return 0;
}
