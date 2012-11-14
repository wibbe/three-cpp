
#include "graphics/Scene.h"

#include <limits>

namespace three { namespace graphics {

  Scene::Scene()
  {
    _nodeCount = 0;
    for (uint32_t i = 0; i < MAX_NODES; ++i)
    {
      _indices[i].id = i;
      _indices[i].next = i + 1;
    }

    _freeListDequeue = 0;
    _freeListEnqueue = MAX_NODES - 1;
  }

  namespace scene {
    inline Node & lookup(Scene & scene, NodeRef node)
    {
      return scene._nodes[scene._indices[node & INDEX_MASK].index];
    }

    bool has(Scene & scene, NodeRef node)
    {
      Scene::Index index = scene._indices[node & INDEX_MASK];
      return index.id == node && in.index != std::numeric_limits<uint16_t>::max();
    }

    NodeRef add(Scene & scene)
    {
      Scene::Index & index = scene._indices[scene._freeListDequeue];
      scene._freeListDequeue = index.next;
      index.id += NEW_NODE_ID_ADD;
      index.index = scene._nodeCount++;
      Node & node = scene._nodes[index.index];
      node.id = index.id;
      node.parent = ROOT_NODE;

      return node.id;
    }

    void remove(Scene & scene, NodeRef node)
    {
      Scene::Index & index = scene._indices[node & INDEX_MASK];
      Node & node = scene._nodes[index.index];
      node = scene._nodes[--scene._nodeCount];
      scene._indices[node.id & INDEX_MASK].index = in.index;

      index.index = std::numeric_limits<uint16_t>::max();
      scene._indices[scene._freeListEnqueue].next = node & INDEX_MASK;
      scene._freeListEnqueue = node & INDEX_MASK;
    }

    void link(Scene & scene, NodeRef parentRef, NodeRef childRef)
    {
      assert(has(scene, parentRef) && has(scene, childRef));

      Node & child = lookup(scene, childRef);
      child.parent = parentRef;

      // Sort nodes here!
    }
  }

}}