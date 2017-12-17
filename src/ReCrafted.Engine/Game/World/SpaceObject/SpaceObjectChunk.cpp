// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectChunk.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectOctreeNode.h"
#include "SpaceObject.h"
#include "Meshing/MarchingCubes/MCMesher.h"
#include "Voxels/VoxelUtils.h"
#include "Graphics/Mesh.h"
#include "Graphics/Rendering.h"
#include "Voxels/VoxelStorage.h"

uint8_t SpaceObjectChunk::getLodBorders()
{
	/*uint8_t borders = 5;

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
	var dataSize = ChunkDataSize * ChunkDataSize * ChunkDataSize;
	m_voxelData = new sbyte[dataSize];

    var voxelStorage = spaceObject->getStorage();

	var positionOffset = Vector3::one() * 0.5f * static_cast<float>(node->get_size());
	var nodePosition = node->get_position() - positionOffset; // lower-left-back corner

	var lod = int(node->get_size() / float(SpaceObjectOctreeNode::MinimumNodeSize));
	var lod_f = static_cast<float>(lod);

	for(auto x = 0; x < ChunkDataSize; x ++)
	{
		for (auto y = 0; y < ChunkDataSize; y++)
		{
			for (auto z = 0; z < ChunkDataSize; z++)
			{
				const var index = INDEX_3D(x, y, z, ChunkDataSize);

				const var offset = Vector3(float(x), float(y), float(z));
				const var position = nodePosition + offset * lod_f;

				m_voxelData[index] = voxelStorage->getVoxel(position);
			}
		}
	}

	// get which directions this chunk must get the bordering skirts
	var borders = getLodBorders();

	// generate mesh
    mesher->generate(nodePosition, lod, borders, m_mesh, m_voxelData);
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
	if (!m_mesh || !m_mesh->isUploaded())
		return;

	auto matrix = Matrix::identity();
	Rendering::getInstance()->draw(m_mesh, &matrix);
}

void SpaceObjectChunk::dispose()
{
	SafeDeleteArray(m_voxelData);
	SafeDispose(m_mesh);
}
