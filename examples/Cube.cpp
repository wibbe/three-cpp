
#include "base/Window.h"
#include "base/Scene.h"
#include "base/Camera.h"
#include "base/Mesh.h"
#include "base/Camera.h"
#include "base/PerspectiveCamera.h"
#include "material/DefaultMaterials.h"
#include "base/Texture.h"
#include "base/Font.h"
#include "loader/ImageUtils.h"
#include "loader/FontLoader.h"
#include "base/CubeGeometry.h"
#include "opengl/GLRenderer.h"
#include "base/Code.h"

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
      camera->position.z = -5;
      camera->position.y = 2;
      camera->position.x = 1;
      camera->lookAt(Vector3(0, 0, 0));
      scene->add(camera);

      Font * font = FontLoader::loadFont("assets/fonts/UbuntuLight.ttf", 32.0, 256, 256);

      Geometry * geometry = new CubeGeometry(2, 2, 2);

      colorMaterial = new MeshBasicMaterial();
      colorMaterial->diffuse = Color("#4680AA");
      colorMaterial->gammaCorrection = true;

      textureMaterial = new MeshBasicMaterial();
      //textureMaterial->map = ImageUtils::loadTexture("assets/textures/crate.png");
      textureMaterial->map = font->texture;
      textureMaterial->diffuse = Color(1, 1, 1);
      textureMaterial->useTextureMap = true;
      textureMaterial->gammaCorrection = true;
      textureMaterial->transparent = true;

      mesh = new Mesh(geometry, textureMaterial);
      scene->add(mesh);

      renderer = new GLRenderer();
      renderer->setClearColor(Color("#85B3B7"));
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

      if (isKeyDown('1'))
        mesh->material = textureMaterial;
      else if (isKeyDown('2'))
        mesh->material = colorMaterial;

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
    Mesh * mesh;

    MeshBasicMaterial * colorMaterial;
    MeshBasicMaterial * textureMaterial;
};


int main(int argc, char * argv[])
{
  CubeDemo demo;
  demo.run();
  return 0;
}
