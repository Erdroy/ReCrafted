// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectChunk.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectOctreeNode.h"
#include "SpaceObject.h"
#include "Meshing/MarchingCubes/MCMesher.h"
#include "Graphics/Mesh.h"
#include "Graphics/Rendering.h"
#include "Voxels/VoxelStorage.h"
#include "Voxels/VoxelClipmap.h"

uint8_t SpaceObjectChunk::getLodBorders()
{
	/*uint8_t borders = 0u;

	if (node->hasNeighLowerLoD(NodeDirection::Front))
		borders |= BORDER_FRONT;

	if (node->hasNeighLowerLoD(NodeDirection::Back))
		borders |= BORDER_BACK;

	if (node->hasNeighLowerLoD(NodeDirection::Left))
		borders |= BORDER_LEFT;

	if (node->hasNeighLowerLoD(NodeDirection::Right))
		borders |= BORDER_RIGHT;

	if (node->hasNeighLowerLoD(NodeDirection::Top))
		borders |= BORDER_TOP;

	if (node->hasNeighLowerLoD(NodeDirection::Back))
		borders |= BORDER_BOTTOM;*/

    // TODO: cull not needed skirts

	//return borders;
	return 0xFF;
}

void SpaceObjectChunk::init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject)
{
	this->spaceObject = spaceObject;
	this->node = node;

	m_chunkNormal = spaceObject->get_position() - node->get_position();
	m_chunkNormal.normalize();
}

void SpaceObjectChunk::generate(IVoxelMesher* mesher)
{
	// WARNING: this function is called on WORKER THREAD!

	m_mesh = Mesh::createMesh();

	// generate voxel data

	cvar positionOffset = Vector3::one() * 0.5f * static_cast<float>(node->get_size());
	cvar lod = int(node->get_size() / float(SpaceObjectOctreeNode::MinimumNodeSize));

    var nodePosition = node->get_position() - positionOffset; // lower-left-back corner

    // TODO: VoxelCache class (should store chunks for some time or all the time if chunk is still being used)
    // TODO: check if this is cached (use start and lod) (and not modified by this time) and get if present
    // TODO: if not - generate and modify

    // get voxel chunk
    voxelData = spaceObject->getStorage()->getVoxelChunk(nodePosition, lod);

    // modify chunk data
    cvar clipmap = spaceObject->getClipmap();

    if(clipmap->chunkModified(nodePosition, lod))
        clipmap->chunkModify(nodePosition, lod, voxelData);

    if(voxelData == nullptr)
    {
        m_hasVoxels = false;
        return;
    }

    m_hasVoxels = true;

	// get which directions this chunk must get the bordering skirts
	cvar borders = getLodBorders();

	// generate mesh
    mesher->generate(nodePosition, lod, borders, m_mesh, voxelData);
}

void SpaceObjectChunk::upload()
{
	// upload changes
	if(m_mesh && m_mesh->canUpload())
		m_mesh->upload();
}

void SpaceObjectChunk::update()
{
	
}

void SpaceObjectChunk::draw()
{
	if (!m_mesh || !m_mesh->isUploaded() || !m_hasVoxels)
		return;

	auto matrix = Matrix::identity();
	Rendering::getInstance()->draw(m_mesh, &matrix);
}

void SpaceObjectChunk::dispose()
{
    // TODO: remove when cache will be done!
    SafeDeleteArrayNN(voxelData);

	SafeDispose(m_mesh);
}
