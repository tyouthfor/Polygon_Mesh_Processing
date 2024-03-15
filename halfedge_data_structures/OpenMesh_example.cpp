#include"OpenMesh_example.h"
#include<iostream>
#include<OpenMesh/Core/IO/MeshIO.hh>
#include<OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;

void OpenMesh_example()
{
    MyMesh mesh;
    OpenMesh::IO::read_mesh(mesh, "output.off");

    // Traverse all the vertices.
    for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++) {
        OpenMesh::VertexHandle ver_H = *v_it;
        std::cout << "Vertex " << ver_H << ": " << mesh.point(ver_H) << ' ' << mesh.valence(ver_H) << '\n';

        // Traverse all the incident edges of the vertex.
        std::cout << "Edges: ";
        for (auto e_it = mesh.ve_begin(ver_H); e_it != mesh.ve_end(ver_H); e_it++) {
            auto edge_H = *e_it;
            std::cout << edge_H << ' ';
        }
        std::cout << '\n';

        // Traverse all the outgoing halfedges of the vertex
        std::cout << "Outgoing Halfedges: ";
        for (auto h_it = mesh.voh_begin(ver_H); h_it != mesh.voh_end(ver_H); h_it++) {
            auto he_H = *h_it;
            std::cout << he_H << ' ';
        }
        std::cout << '\n';

        // Traverse all the incoming halfedges of the vertex
        std::cout << "Incoming Halfedges: ";
        for (auto h_it = mesh.vih_begin(ver_H); h_it != mesh.vih_end(ver_H); h_it++) {
            auto he_H = *h_it;
            std::cout << he_H << ' ';
        }
        std::cout << '\n';

        // Traverse all the incident faces of the vertex
        std::cout << "Faces: ";
        for (auto f_it = mesh.vf_begin(ver_H); f_it != mesh.vf_end(ver_H); f_it++) {
            auto face_H = *f_it;
            std::cout << face_H << ' ';
        }
        std::cout << '\n';

        // Traverse all the one-ring neighbors of the vertex
        std::cout << "One-Ring Neighbors: ";
        for (auto vv_it = mesh.vv_iter(ver_H); vv_it.is_valid(); vv_it++) {
            std::cout << *vv_it << ' ';
        }
        std::cout << '\n';

        std::cout << '\n';
    }
}