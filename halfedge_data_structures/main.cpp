#include<iostream>
#include"trimesh.h"

int main()
{
	trimesh::trimesh_t mesh;

	// Triangles
	// The order of index is important.
	// 
	//       1
	//      /|\
	//     / | \
	//    /  |  \
	//   0   |   3
	//    \  |  /
	//     \ | /
	//      \|/
	//       2
	//
	std::vector<trimesh::triangle_t> triangles;
	triangles.resize(2);
	const int num_vertices = 4;

	triangles[0].i() = 0;
	triangles[0].j() = 1;
	triangles[0].k() = 2;

	triangles[1].i() = 2;
	triangles[1].j() = 1;
	triangles[1].k() = 3;

	// Edges
	std::vector<trimesh::edge_t> edges;
	mesh.triangles2edges(triangles.size(), &triangles[0], edges);

	// Triangle mesh
	mesh.build(num_vertices, triangles.size(), &triangles[0], edges.size(), &edges[0]);

	// Traverse all the one-ring neighbors of each vertex.
	std::vector<trimesh::index_t> neighbors;
	for (int i = 0; i < num_vertices; ++i) {
		mesh.vv_neighbors(i, neighbors);
		std::cout << "Neighbors of vertex " << i << ": ";
		for (int j = 0; j < neighbors.size(); ++j) {
			std::cout << neighbors[j] << ' ';
		}
		std::cout << '\n';
	}

	return 0;
}