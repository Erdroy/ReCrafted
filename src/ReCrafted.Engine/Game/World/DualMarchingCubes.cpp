// ReCrafted © 2016-2017 Always Too Late

#include "DualMarchingCubes.h"
#include "Core/Math/FastNoiseSIMD/FastNoiseSIMD.h"
#include "Platform/Platform.h"
#include "MCTables.h"

FastNoiseSIMD* noise_terrain = nullptr;
float* noise_terrain_set = nullptr;

Vector3 GetIntersection(Vector3 p1, Vector3 p2, float d1, float d2)
{
	return p1 + (p2 - p1) * (1.0f / (d2 - d1)) * -d1;
}

class Vertex
{
public:
	int index = 0;
	Array<int> edges = {};
	Vector3 position = {};
};

class Edge
{
public:
	int index = 0;
	Vector3 a = {};
	Vector3 b = {};
	float valueA = 0.0f;
	float valueB = 0.0f;
	Array<Vertex> vertices = {};
	bool flipped = false;
	Vector3 intersection = {};
	bool hasPoint = false;

	Edge() { }

	Vector3 Intersection()
	{
		if (hasPoint)
			return intersection;

		intersection = GetIntersection(a, b, valueA, valueB);
		hasPoint = true;
		return intersection;
	}
};

class Cell
{
public:
	int vertexCount = 0;
	Vertex vertices[3];
	Edge edges[12];

	Cell()
	{
		for (auto i = 0; i < 12; i++)
		{
			edges[i] = {};
			edges[i].index = i;
		}
	}
};

float terrain_data[16][16][16];
Cell cells[17][17][17];

float Sample(const Vector3& position)
{
	auto x = static_cast<int>(position.x);
	auto z = static_cast<int>(position.z);

	auto index = x * 17 + z;
	auto noise = noise_terrain_set[index];

	auto hillLevel = (1.0f - noise) * 4.0;
	auto baseLevel = 2.0f + hillLevel;

	if (position.y < baseLevel)
		return -1.0f;

	if (position.y - 2 < baseLevel)
		return -1.0f;

	return 1.0f;
}

void generateData()
{
	for (auto y = 0; y < 16; y++)
	{
		for (auto x = 0; x < 16; x++)
		{
			for (auto z = 0; z < 16; z++)
			{
				auto pos = Vector3(x, y, z);
				terrain_data[x][y][z] = Sample(pos);
			}
		}
	}
}

Edge generateEdge(int x, int y, int z, int i)
{
	auto p = Vector3(x, y, z);
	auto a = p + CornerDeltas[EdgePairs[i][0]];
	auto b = p + CornerDeltas[EdgePairs[i][1]];

	auto edge = Edge();
	edge.index = i;
	edge.a = a;
	edge.b = b;
	edge.valueA = Sample(a);
	edge.valueB = Sample(b);

	return edge;
}

void generateCells()
{
	for (auto y = 0; y < 17; y++)
	{
		for (auto x = 0; x < 17; x++)
		{
			for (auto z = 0; z < 17; z++)
			{
				cells[x][y][z] = {};
				cells[x][y][z].edges[0] = generateEdge(x, y, z, 0);
				cells[x][y][z].edges[3] = generateEdge(x, y, z, 3);
				cells[x][y][z].edges[8] = generateEdge(x, y, z, 8);
			}
		}
	}
}

void generateEdges()
{
	for (auto y = 0; y < 16 - 1; y++)
	{
		for (auto x = 0; x < 16 - 1; x++)
		{
			for (auto z = 0; z < 16 - 1; z++)
			{
				cells[x][y][z].edges[1] = cells[x + 1][y][z].edges[3];
				cells[x][y][z].edges[2] = cells[x][y][z + 1].edges[0];

				cells[x][y][z].edges[4] = cells[x][y + 1][z].edges[0];
				cells[x][y][z].edges[5] = cells[x + 1][y + 1][z].edges[3];
				cells[x][y][z].edges[6] = cells[x][y + 1][z + 1].edges[0];
				cells[x][y][z].edges[7] = cells[x][y + 1][z].edges[3];

				cells[x][y][z].edges[9] = cells[x + 1][y][z].edges[8];
				cells[x][y][z].edges[10] = cells[x + 1][y][z + 1].edges[8];
				cells[x][y][z].edges[11] = cells[x][y][z + 1].edges[8];
			}
		}
	}
}

Array<Vector3> generatePolys()
{
	Array<Vector3> vertices = {};

	for (auto y = 0; y < 16 - 1; y++)
	{
		for (auto x = 0; x < 16 - 1; x++)
		{
			for (auto z = 0; z < 16 - 1; z++)
			{
				auto cubeIndex = 0;
				for (auto i = 0; i < 8; i++)
				{
					auto p = Vector3(x, y, z) + CornerDeltas[i];
					if (Sample(p) < 0)
						cubeIndex |= 1 << i;
				}

				if (cubeIndex == 0 || cubeIndex == 255)
					continue;

				auto vIndex = 0;
				cells[x][y][z].vertexCount = VerticesNumberTable[cubeIndex];
				cells[x][y][z].vertices[0] = Vertex();

				for (auto i = 0; i < 16; i++)
				{
					if (EdgesTable[cubeIndex][i] == -2)
						break;

					if (EdgesTable[cubeIndex][i] == -1)
					{
						vIndex++;
						if (vIndex < cells[x][y][z].vertexCount)
						{
							cells[x][y][z].vertices[vIndex] = Vertex();
						}
						continue;
					}

					auto edge = EdgesTable[cubeIndex][i];

					cells[x][y][z].vertices[vIndex].edges.add(edge);
					cells[x][y][z].edges[edge].vertices.add(cells[x][y][z].vertices[vIndex]);
				}

				for(auto i = 0; i < cells[x][y][z].vertexCount; i ++)
				{
					auto tx = &cells[x][y][z].vertices[i];

					// TODO
					//if (v == null) //for edges 241/243, which were originally marked as having 2 vertices...?
					//	continue;

					auto point = Vector3();
					
					for(auto j = 0; j < tx->edges.count(); j ++)
					{
						auto edge = tx->edges[j];
						auto e = &cells[x][y][z].edges[edge];
						point += e->Intersection();
					}

					point *= 1.0f / tx->edges.count();

					tx->position = point;
					tx->index = vertices.count();

					vertices.add(point);
				}
			}
		}
	}

	return vertices;
}

Array<int> generateIndices()
{
	Array<int> indices = {};

	for (auto y = 0; y < 16; y++)
	{
		for (auto x = 0; x < 16; x++)
		{
			for (auto z = 0; z < 16; z++)
			{
				//if (Cells[x, y, z] == null)
				//	return {};

				int edgeIndexes[] = { 0, 3, 8 };

				for (auto i = 0; i < 3; i++)
				{
					auto e = cells[x][y][z].edges[edgeIndexes[i]];
					auto flipped = e.valueA < 0;

					if (e.vertices.count() > 3)
					{
						if (flipped)
						{
							indices.add(e.vertices[1].index);
							indices.add(e.vertices[0].index);
							indices.add(e.vertices[2].index);

							indices.add(e.vertices[2].index);
							indices.add(e.vertices[3].index);
							indices.add(e.vertices[1].index);
						}
						else
						{
							indices.add(e.vertices[2].index);
							indices.add(e.vertices[0].index);
							indices.add(e.vertices[1].index);

							indices.add(e.vertices[1].index);
							indices.add(e.vertices[3].index);
							indices.add(e.vertices[2].index);
						}
					}
					else if (e.vertices.count() == 3)
					{
						if (flipped)
						{
							indices.add(e.vertices[2].index);
							indices.add(e.vertices[1].index);
							indices.add(e.vertices[0].index);
						}
						else
						{
							indices.add(e.vertices[0].index);
							indices.add(e.vertices[1].index);
							indices.add(e.vertices[2].index);
						}
					}
				}
			}
		}
	}

	return indices;
}

void DMC::generate()
{
	FastNoiseSIMD::SetSIMDLevel(-1);

	noise_terrain = FastNoiseSIMD::NewFastNoiseSIMD(100);
	noise_terrain->SetNoiseType(FastNoiseSIMD::SimplexFractal);
	noise_terrain->SetFrequency(0.008f);
	noise_terrain->SetFractalType(FastNoiseSIMD::FBM);
	noise_terrain->SetFractalOctaves(4);
	noise_terrain->SetFractalLacunarity(2.0f);
	noise_terrain->SetFractalGain(0.5f);

	noise_terrain_set = noise_terrain->GetSimplexFractalSet(0, 0, 0, 16, 1, 16);

	auto start = Platform::getMiliseconds();

	generateData();
	generateCells();
	generateEdges();
	auto vertices = generatePolys();
	auto indices = generateIndices();
	auto end = Platform::getMiliseconds();

	auto diff = end - start;

	noise_terrain->FreeNoiseSet(noise_terrain_set);
}

void DMC::draw()
{
}
