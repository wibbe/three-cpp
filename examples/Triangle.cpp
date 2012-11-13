
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
#include "base/Code.h"

#include <iostream>

using namespace three;

class TriangleDemo : public Window
{
  public:
    TriangleDemo()
      : Window(1024, 768)
    {
      setTitle("Triangle Example");

      scene = new Scene();

      camera = new PerspectiveCamera();
      camera->position.y = 1;
      camera->position.z = 2;
      //camera->rotation.x = -0.4;
      camera->lookAt(Vector3(0, 0, 0));
      scene->add(camera);

      Geometry * geometry = new Geometry();
      geometry->vertices.push_back(Vector3(0, 0.5, 0));
      geometry->vertices.push_back(Vector3(-0.5, 0, 0));
      geometry->vertices.push_back(Vector3(0.5, 0, 0));
      geometry->vertices.push_back(Vector3(0, 0.5, 0));
      geometry->vertices.push_back(Vector3(0.5, 0, 0));
      geometry->vertices.push_back(Vector3(-0.5, 0, 0));
      geometry->faces.push_back(Face(0, 1, 2));
      geometry->faces.push_back(Face(3, 4, 5));
      //geometry->faceCount = 2;

      MeshBasicMaterial * material = new MeshBasicMaterial();
      //material->map = ImageUtils::loadTexture("assets/crate.png");
      material->diffuse = Color(1, 0.8, 0.4);
      material->useTextureMap = false;

      mesh = new Mesh(geometry, material);
      //mesh->position.y = 0.4;
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
      mesh->rotation.y += dt * 2.0;
      mesh->matrixWorldNeedsUpdate = true;

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
};


int main(int argc, char * argv[])
{
  TriangleDemo demo;
  demo.run();
  return 0;
}
