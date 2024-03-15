#include"trimesh.h"
#include<iostream>
#include<cassert>
#include<set>

namespace trimesh
{
	void trimesh_t::triangles2edges(number_t num_triangles, const triangle_t* triangles, std::vector<edge_t>& edges)
	{
		typedef std::set<std::pair<index_t, index_t>> edge_set_t;
		edge_set_t edge_set;

		for (int i = 0; i < num_triangles; ++i) {
			edge_set.insert(std::make_pair(std::min(triangles[i].i(), triangles[i].j()), std::max(triangles[i].i(), triangles[i].j())));
			edge_set.insert(std::make_pair(std::min(triangles[i].j(), triangles[i].k()), std::max(triangles[i].j(), triangles[i].k())));
			edge_set.insert(std::make_pair(std::min(triangles[i].k(), triangles[i].i()), std::max(triangles[i].k(), triangles[i].i())));
		}

		edges.resize(edge_set.size());

		int e = 0;
		for (auto it = edge_set.begin(); it != edge_set.end(); ++it, ++e) {
			edges[e].start() = it->first;
			edges[e].end() = it->second;
		}
	}

	void trimesh_t::clear()
	{
		m_halfedges.clear();
		m_vertex_halfedges.clear();
		m_face_halfedges.clear();
		m_edge_halfedges.clear();
		m_edge2halfedge.clear();
	}

	void trimesh_t::build(number_t num_vertices, number_t num_triangles, const trimesh::triangle_t* triangles, number_t num_edges, const trimesh::edge_t* edges)
	{
		assert(triangles);
		assert(edges);

		clear();
		m_vertex_halfedges.resize(num_vertices, -1);
		m_face_halfedges.resize(num_triangles, -1);
		m_edge_halfedges.resize(num_edges, -1);
		m_halfedges.reserve(num_edges * 2);  // Change capacity instead of size.

		// Record (vertex[i], vertex[j]) -> face index, used later.
		// The same face index per triangle.
		edge2index_t edge2fi;
		for (int i = 0; i < num_triangles; ++i) {
			const triangle_t& tri = triangles[i];
			edge2fi[std::make_pair(tri.i(), tri.j())] = i;
			edge2fi[std::make_pair(tri.j(), tri.k())] = i;
			edge2fi[std::make_pair(tri.k(), tri.i())] = i;
		}

		for (int i = 0; i < num_edges; ++i) {
			// Split edge into halfedges he0 and he1.
			// Fill "m_halfedges".
			const edge_t& edge = edges[i];

			const index_t he0_index = m_halfedges.size();
			m_halfedges.push_back(halfedge_t());
			halfedge_t& he0 = m_halfedges.back();

			const index_t he1_index = m_halfedges.size();
			m_halfedges.push_back(halfedge_t());
			halfedge_t& he1 = m_halfedges.back();

			// Fill the member variables (except for next) of he0 and he1.
			// The vertex order of the key of "edge2fi" is according to initialization.
			// While the vertex order of 'edge' is (min_vi, max_vi), according to the function "triangles2edges".
			he0.face = halfedge2face(edge2fi, edge.start(), edge.end());
			he0.vertex = edge.end();  // he0 points to the max_vi
			he0.edge = i;
			he0.opposite = he1_index;
			
			he1.face = halfedge2face(edge2fi, edge.end(), edge.start());
			he1.vertex = edge.start();  // he1 points to the min_vi
			he1.edge = i;
			he1.opposite = he0_index;

			// Fill "m_edge2halfedge", (vertex_index_i, vertex_index_j) -> halfedge_index.
			assert(m_edge2halfedge.find(std::make_pair(edge.start(), edge.end())) == m_edge2halfedge.end());
			assert(m_edge2halfedge.find(std::make_pair(edge.end(), edge.start())) == m_edge2halfedge.end());
			m_edge2halfedge[std::make_pair(edge.start(), edge.end())] = he0_index;
			m_edge2halfedge[std::make_pair(edge.end(), edge.start())] = he1_index;

			// Fill "m_vertex_halfedges"£¬each vertex stores one of outgoing halfedges.
			if (m_vertex_halfedges[he0.vertex] == -1 || he1.face == -1) {
				m_vertex_halfedges[he0.vertex] = he0.opposite;
			}
			if (m_vertex_halfedges[he1.vertex] == -1 || he0.face == -1) {
				m_vertex_halfedges[he1.vertex] = he1.opposite;
			}

			// Fill "m_face_halfedges"£¬each face stores one of halfedges.
			if (he0.face != -1 && he1.face != -1) {
				if (m_face_halfedges[he0.face] == -1) {
					m_face_halfedges[he0.face] = he0_index;
				}
				if (m_face_halfedges[he1.face] == -1) {
					m_face_halfedges[he1.face] = he1_index;
				}
			}

			// Fill "m_edge_halfedges", each edge stores he0_index.
			assert(m_edge_halfedges[i] == -1);
			m_edge_halfedges[i] = he0_index;
		}

		// Fill "next" halfedge of each halfedge.
		// Handle the boundary halfedge separately.
		std::vector<index_t> boundary_he;
		for (int i = 0; i < m_halfedges.size(); ++i) {
			halfedge_t& he = m_halfedges[i];
			if (he.face == -1) {
				boundary_he.push_back(i);
				continue;
			}
			const triangle_t& tri = triangles[he.face];
			const index_t vertex = he.vertex;
			
			index_t temp = -1;
			if (tri.i() == vertex) {
				temp = tri.j();
			}
			else if (tri.j() == vertex) {
				temp = tri.k();
			}
			else if (tri.k() == vertex) {
				temp = tri.i();
			}
			assert(temp != -1);

			he.next = m_edge2halfedge[std::make_pair(vertex, temp)];
		}

		// vertex2outgoing_boundary_halfedge_index: vertex index -> all of outgoing boundary halfedges indices.
		std::map<index_t, std::set<index_t>> vertex2outgoing_boundary_halfedge_index;
		for (auto it = boundary_he.begin(); it != boundary_he.end(); ++it) {
			halfedge_t& he = m_halfedges[*it];
			const index_t origin = m_halfedges[he.opposite].vertex;  // The index of origin vertex of boundary halfedge.
			vertex2outgoing_boundary_halfedge_index[origin].insert(*it);
		}

		// For each boundary halfedge, make its next halfedge one of the boundary halfedges
		// originating at its "vertex" (pointing at).
		for (auto it = boundary_he.begin(); it != boundary_he.end(); ++it) {
			halfedge_t& he = m_halfedges[*it];
			std::set<index_t>& outgoing = vertex2outgoing_boundary_halfedge_index[he.vertex];
			if (!outgoing.empty()) {
				auto iter = outgoing.begin();
				he.next = *iter;
				outgoing.erase(iter);
			}
		}
	}

	index_t trimesh_t::halfedge2face(const edge2index_t& edge2fi, index_t vertex_i, index_t vertex_j)
	{
		assert(!edge2fi.empty());
		// If no such edge exists, then there's no such face in the mesh.
		// The edge must be a boundary edge.
		auto it = edge2fi.find(std::make_pair(vertex_i, vertex_j));
		if (it == edge2fi.end()) { return -1; }
		return it->second;
	}

	void trimesh_t::vv_neighbors(const index_t index, std::vector<index_t>& neighbors) const
	{
		neighbors.clear();
		index_t he_index = m_vertex_halfedges[index];
		const index_t hstop = he_index;
		do {
			const halfedge_t& he = m_halfedges[he_index];
			neighbors.push_back(he.vertex);
			he_index = m_halfedges[he.opposite].next;
		} while (he_index != hstop);
	}
}