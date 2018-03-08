// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectOctree.h"
#include "SpaceObject.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Math/Math.h"
#include "Graphics/Renderer/Renderer.h"

void SpaceObjectOctree::init()
{
	// calculate diameter
	var settings = spaceObject->getSettings();
	var radius = settings.maxSurfaceHeight;
	var objectSize = Math::roundUpToPow2(static_cast<int>(radius * 2)); // diameter rounded up to power of 2

    // calculate bounds size
    var size = Vector3::one() * static_cast<float>(objectSize);

    // build bounds
    m_bounds = BoundingBox(settings.position, size);

    // calculate length of root nodes on single axis
    var rootNodesLength = Math::pow(2, settings.rootOctreeDepth);

    // calculate the count of root nodes
    m_rootNodesCount = Math::pow(rootNodesLength, 3);

    // calculate the node size
    var rootNodeSize = objectSize / rootNodesLength;

    // calculate the center offset
    var rootNodeOffset = static_cast<float>(rootNodesLength / 2 * rootNodeSize) - rootNodeSize * 0.5f;
    var rootNodePositionOffset = Vector3(rootNodeOffset, rootNodeOffset, rootNodeOffset);

    // create root nodes
    m_rootNodes = new SpaceObjectOctreeNode*[m_rootNodesCount];
    var nodeId = 0;
    for(var x = 0; x < rootNodesLength; x ++)
    {
        for (var y = 0; y < rootNodesLength; y++)
        {
            for (var z = 0; z < rootNodesLength; z++)
            {
                // create root node
                m_rootNodes[nodeId] = new SpaceObjectOctreeNode();
                var node = m_rootNodes[nodeId];

                // calculate node position
                var position = Vector3(
                    static_cast<float>(x * rootNodeSize), 
                    static_cast<float>(y * rootNodeSize), 
                    static_cast<float>(z * rootNodeSize));
                position -= rootNodePositionOffset;

                // set position and size
                node->set_position(position);
                node->set_size(rootNodeSize);

                // set owner, parent and root
                node->owner = this;
                node->parent = nullptr;
                node->root = node;
                node->m_isRoot = true;

                // populate node
                node->populate();

                nodeId++;
            }
        }
    }
}

void SpaceObjectOctree::update()
{
	Profiler::beginProfile("SpaceObjectOctree::update");
	// do we need fixed update rate here?

    // update root nodes
    for(var i = 0; i < m_rootNodesCount; i ++)
	    m_rootNodes[i]->update();

	Profiler::endProfile();
}

void SpaceObjectOctree::draw()
{
    Profiler::beginProfile("SpaceObjectOctree::draw");

    Renderer::getInstance()->setStage(RenderStage::Default);

    for (var i = 0; i < m_rootNodesCount; i++)
        m_rootNodes[i]->draw();
    Profiler::endProfile();
}

void SpaceObjectOctree::updateViews(Array<Vector3>& views)
{
	Profiler::beginProfile("SpaceObjectOctree::updateViews");
    for (var i = 0; i < m_rootNodesCount; i++)
        m_rootNodes[i]->updateViews(views);
	Profiler::endProfile();
}

void SpaceObjectOctree::dispose()
{
    for (var i = 0; i < m_rootNodesCount; i++)
    {
        SafeDispose(m_rootNodes[i]);
        SafeDelete(m_rootNodes[i]);
    }

    delete [] m_rootNodes;
}
