
#include "graphics/Scene.h"

#include <limits>
#include <algorithm>
#include <cassert>

namespace three { namespace graphics {

  bool nodeCompare(Node const& node1, Node const& node2)
  {
    return node1.parent < node2.parent;
  }

  Scene::Scene()
  {
    const uint16_t max = std::numeric_limits<uint16_t>::max();

    for (uint32_t i = 0; i < MAX_NODES; ++i)
    {
      _indices[i].id = i;
      _indices[i].next = i + 1;
      _indices[i].index = max;
      _nodes[i].id = i;
      _nodes[i].parent = ROOT_NODE;
    }

    _freeListDequeue = 1;
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
      return index.id == node && index.index != std::numeric_limits<uint16_t>::max();
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

    void remove(Scene & scene, NodeRef nodeRef)
    {
      Scene::Index & index = scene._indices[nodeRef & INDEX_MASK];
      Node & node = scene._nodes[index.index];
      node = scene._nodes[--scene._nodeCount];
      scene._indices[node.id & INDEX_MASK].index = index.index;

      index.index = std::numeric_limits<uint16_t>::max();
      scene._indices[scene._freeListEnqueue].next = node & INDEX_MASK;
      scene._indices[scene._freeListEnqueue].next = nodeRef & INDEX_MASK;
      scene._freeListEnqueue = nodeRef & INDEX_MASK;
    }

    void link(Scene & scene, NodeRef parentRef, NodeRef childRef)
    {
      assert(has(scene, parentRef) && has(scene, childRef));

      Node & child = lookup(scene, childRef);
      child.parent = parentRef;

      // Sort nodes here!
      // Sort nodes according to parent
      std::sort(&scene._nodes[0], &scene._nodes[MAX_NODES], nodeCompare);

      // Rebuild index
      for (uint32_t i = 0; i < MAX_NODES; ++i)
      {
        Node & node = scene._nodes[i];
        Scene::Index & index = scene._indices[node.id & INDEX_MASK];

        index.id = node.id;
        index.index = i;
        index.next = (node.id & INDEX_MASK) + 1;
      }

      scene._freeListDequeue = scene._nodeCount;
    }
  }

}}