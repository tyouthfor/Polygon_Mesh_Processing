// This file is part of libigl, a simple c++ geometry processing library.
// 
// Copyright (C) 2021 Alec Jacobson <alecjacobson@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#include "march_cube.h"
#include "marching_cubes_tables.h"
#include <cstdint>

template <
    typename DerivedGV,
    typename Scalar,
    typename Index,
    typename DerivedV,
    typename DerivedF>
IGL_INLINE void igl::march_cube(
    const DerivedGV& GV,
    const Eigen::Matrix<Scalar, 8, 1>& cS,
    const Eigen::Matrix<Index, 8, 1>& cI,
    const Scalar& isovalue,
    Eigen::PlainObjectBase<DerivedV>& V,
    Index& n,
    Eigen::PlainObjectBase<DerivedF>& F,
    Index& m,
    std::unordered_map<std::int64_t, int>& E2V)
{
    // Input: The index (i, j) of (starting vertex, ending vertex) and interpolation coefficient t
    // Output: The index of triangle meshes' vertex on this edge
    const auto ij2vertex = [&E2V, &V, &n, &GV](const Index& i, const Index& j, const Scalar& t) -> Index
    {
        const auto ij2key = [](std::int32_t i, std::int32_t j)
        {
            if (i > j) {
                std::swap(i, j);
            }
            std::int64_t ret = 0;
            ret |= i;
            ret |= static_cast<std::int64_t>(j) << 32;
            return ret;
        };

        // GV[i]: The position of grids' vertex[i]
        // V[i]: The position of triangle meshes' vertex[i]
        // n: Current number of mesh vertices (i.e., occupied rows in V)
        // E2V: Current edge (GV_i, GV_j) to vertex (V_k) map
        // The goal of this function is to fill V (here) and F (later).
        // Remember V is empty and n equals zero at first.
        const auto key = ij2key(i, j);
        const auto it = E2V.find(key);
        int v = -1;
        if (it == E2V.end()) {
            if (n == V.rows()) {
                V.conservativeResize(V.rows() * 2 + 1, V.cols());
            }
            V.row(n) = GV.row(i) + t * (GV.row(j) - GV.row(i));  // Linear interpolation
            v = n;
            E2V[key] = v;
            n++;
        }
        else {
            v = it->second;
        }
        return v;
    };

    // 1. Find which vertices of the cube are in the object, according to the postive/negative SDF value
    int c_flags = 0;  // c_flags: encoding of vertices
    for (int c = 0; c < 8; c++) {
        if (cS(c) > isovalue) {
            c_flags |= 1 << c;
        }
    }

    // 2. Find which edges of the cube intersect the surface(triangle mesh)
    int e_flags = aiCubeEdgeFlags[c_flags];  // e_flags: encoding of edges

    // If the cube is entirely inside/outside of the surface, then there will be no intersections
    if (e_flags == 0) {
        return;
    }

    // 3. Find the point of intersection of the surface with each edge
    // edge_vertices[e]: The index of triangle meshes' vertex in e-th edge, -1 if not has
    Eigen::Matrix<Index, 12, 1> edge_vertices;
    for (int e = 0; e < 12; e++) {  // 12 edges per cube
#ifndef NDEBUG
        edge_vertices[e] = -1;
#endif
        // 3.1 If there is an intersection on this edge
        if (e_flags & (1 << e)) {
            // 3.2 Find the position and index of triangle meshes' vertex on this edge
            const Scalar& a = cS(a2eConnection[e][0]);  // a: SDF value of starting point of e-th edge
            const Scalar& b = cS(a2eConnection[e][1]);  // b: SDF value of ending point of e-th edge
            const Scalar delta = b - a;
            Scalar t = (delta == 0) ? 0.5 : (isovalue - a) / delta;  // t: Linear interpolation coefficient

            edge_vertices[e] = ij2vertex(cI(a2eConnection[e][0]), cI(a2eConnection[e][1]), t);
            assert(edge_vertices[e] >= 0);
            assert(edge_vertices[e] < n);
        }
    }

    // 4. Record triangle meshes into F
    // F[m]: The index of vertices of m-th triangle mesh
    for (int f = 0; f < 5; f++) {  // There can be up to 5 triangle meshes per cube
        if (a2fConnectionTable[c_flags][3 * f] < 0) {
            break;
        }
        if (m == F.rows()) {
            F.conservativeResize(F.rows() * 2 + 1, F.cols());
        }
        assert(edge_vertices[a2fConnectionTable[c_flags][3 * f + 0]] >= 0);
        assert(edge_vertices[a2fConnectionTable[c_flags][3 * f + 1]] >= 0);
        assert(edge_vertices[a2fConnectionTable[c_flags][3 * f + 2]] >= 0);

        // m: Current number of mesh triangles (i.e., occupied rows in F)
        F.row(m) <<
            edge_vertices[a2fConnectionTable[c_flags][3 * f + 0]],
            edge_vertices[a2fConnectionTable[c_flags][3 * f + 1]],
            edge_vertices[a2fConnectionTable[c_flags][3 * f + 2]];
        m++;
    }
}