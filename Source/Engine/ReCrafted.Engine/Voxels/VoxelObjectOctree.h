// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class VoxelObjectOctree
{
    DELETE_COPY_MOVE(VoxelObjectOctree)

public:
    class Node
    {
        DELETE_COPY_MOVE(Node)

    public:
        Node();
        ~Node();
        
    };

private:
    Node** m_rootNodes = nullptr;

public:
    VoxelObjectOctree();
    ~VoxelObjectOctree();

};
