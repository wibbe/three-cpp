
#pragma once

#include "graphics/Scene_types.h"
#include "math/Vector3.h"

namespace three { namespace graphics {

  #define MAX_NODES 64 * 1024
  #define INDEX_MASK 0xFFFF
  #define NEW_NODE_ID_ADD 0x10000

  struct Node
  {
    NodeRef id;
    NodeRef parent;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    AttachmentRef attachment;
  };

  class Scene
  {
  public:
    struct Index
    {
      NodeRef ID;
      uint16_t index;
      uint16_t next;
    };

  public:
    Scene();

    Node _nodes[MAX_NODES];
    Index _indices[MAX_NODES];

    uint32_t _nodeCount;
    uint16_t _freeListEnqueue;
    uint16_t _freeListDequeue;
  };

  // -- Interface --
  namespace scene {
    bool has(Scene & scene, NodeRef node);

    NodeRef add(Scene & scene);
    void remove(Scene & scene, NodeRef node);

    void link(Scene & scene, NodeRef parent, NodeRef child);
  }

}}