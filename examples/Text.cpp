
#include "base/Window.h"
#include "base/Scene.h"
#include "base/Mesh.h"
#include "base/Geometry.h"
#include "base/OrthographicCamera.h"
#include "util/GeometryHelper.h"
#include "material/DefaultMaterials.h"
#include "base/Texture.h"
#include "base/Font.h"
#include "loader/FontLoader.h"
#include "opengl/GLRenderer.h"
#include "base/Code.h"
#include <iostream>

using namespace three;

void addTextString(float x, float y, const char * text, Color const& color, Font * font, Material * material, Scene * scene)
{
  Geometry * geom = new Geometry();
  font->buildTextGeometry(text, Vector2(0, 0), geom, color);

  Mesh * mesh = new Mesh(geom, material);
  mesh->position.x = x;
  mesh->position.y = y;
  scene->add(mesh);
}

class TextDemo : public Window
{
  public:
    TextDemo()
      : Window(1024, 768)
    {
      setTitle("Text Demo");

      scene = new Scene();

      camera = new OrthographicCamera(0, 1024, 0, 768, 1, 1000);
      camera->position.z = 1;
      camera->position.y = 0;
      camera->position.x = 0;
      scene->add(camera);

      Font * font = FontLoader::loadFont("assets/fonts/UbuntuLight.ttf", 22.0, 256, 256);

      fontMaterial = new MeshBasicMaterial();
      fontMaterial->map = font->texture;
      fontMaterial->diffuse = Color(1, 1, 1);
      fontMaterial->useTextureMap = true;
      fontMaterial->gammaCorrection = true;
      fontMaterial->transparent = true;

      addTextString(10, 32, "Hello World", Color(1, 0, 1), font, fontMaterial, scene);

      renderer = new GLRenderer();
      renderer->setClearColor(Color("#85B3B7"));
    }

    void resize(int width, int height)
    {
      renderer->setSize(width, height);
      camera->right = width;
      camera->bottom = height;
      camera->updateProjectionMatrix();

      paint();
    }

    bool update(double dt)
    {
      return !isKeyDown(Key::Esc);
    }

    void paint()
    {
      renderer->render(scene, camera);
    }

  private:
    Renderer * renderer;
    Scene * scene;
    OrthographicCamera * camera;

    MeshBasicMaterial * fontMaterial;
};


int main(int argc, char * argv[])
{
  TextDemo demo;
  demo.run();
  return 0;
}
