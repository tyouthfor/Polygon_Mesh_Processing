#ifndef __trimesh_h__
#define __trimesh_h__

#include"trimesh_types.h"
#include<vector>
#include<map>
#include<set>

namespace trimesh
{
	/*
	* Name: trimesh_t
	* Func: Triangle mesh class.
	* @Varia m_halfedges[i]: The halfedge of index i.
	* @Varia m_vertex_halfedges[i]: The index of one of outgoing halfedges related to vertex[i].
	* @Varia m_face_halfedges[i]: The index of one of halfedges related to face[i].
	* @Varia m_edge_halfedges[i]: The index of one of halfedges related to edge[i].
	* @Varia m_edge2halfedge: The map that (vertex_i, vertex_j) -> halfedge_k.
	*/
	class trimesh_t
	{
	private:
		typedef long index_t;
		typedef const unsigned long number_t;
		typedef std::map<std::pair<index_t, index_t>, index_t> edge2index_t;

		struct halfedge_t;
		std::vector<halfedge_t> m_halfedges;
		std::vector<index_t> m_vertex_halfedges;
		std::vector<index_t> m_face_halfedges;
		std::vector<index_t> m_edge_halfedges;
		edge2index_t m_edge2halfedge;

		/*
		* Name: halfedge_t
		* Func: Halfedge class.
		* @Varia vertex: The index of vertex pointed by halfedge.
		* @Varia face: The index of face related to halfedge. -1 if halfedge is a boundary halfedge.
		* @Varia edge: The index of edge related to halfedge.
		* @Varia next: The index of next halfedge.
		* The next halfedge of an interior halfedge is also an interior halfedge in the same face.
		* The next halfedge of a boundary halfedge is still a boundary halfedge in another face.
		* @Varia opposite: The index of opposite halfedge.
		*/
		struct halfedge_t
		{
			index_t vertex, face, edge, next, opposite;
			halfedge_t() { vertex = face = edge = next = opposite = -1; }
		};

	public:
		/*
		* Name: triangles2edges
		* Func: Build the edges vector from the given triangles vector.
		*/
		void triangles2edges(number_t num_triangles, const triangle_t* triangles, std::vector<edge_t>& edges);

		/*
		* Name: clear
		* Func: Clear all the member variables in the triangle mesh. Used at the beginning of building.
		*/
		void clear();

		/*
		* Name: build
		* Func: Build the halfedge data structures from the given triangles and edges.
		*/
		void build(number_t num_vertices, number_t num_triangles, const trimesh::triangle_t* triangles, number_t num_edges, const trimesh::edge_t* edges);

		/*
		* Name: halfedge2face
		* Func: Return the index of face related to halfedge (vertex_i, vertex_j), according to map "e2i".
		*/
		index_t halfedge2face(const edge2index_t& e2i, index_t vertex_i, index_t vertex_j);

		/*
		* Name: vv_neighbors
		* Func: Record the indices of one-ring neighbors of vertex[index] into neighbors vector.
		*/
		void vv_neighbors(const index_t index, std::vector<index_t>& neighbors) const;
	};
}

#endif // !__trimesh_h__
