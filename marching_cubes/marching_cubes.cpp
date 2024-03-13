// This file is part of libigl, a simple c++ geometry processing library.
// 
// Copyright (C) 2021 Alec Jacobson <alecjacobson@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#include "marching_cubes.h"
#include "march_cube.h"

// Adapted from public domain code at
// http://paulbourke.net/geometry/polygonise/marchingsource.cpp

#include <unordered_map>
#include <iostream>
#include <cstdint>

template <typename DerivedS, typename DerivedGV, typename DerivedV, typename DerivedF>
IGL_INLINE void igl::marching_cubes(
    const Eigen::MatrixBase<DerivedS>& S,
    const Eigen::MatrixBase<DerivedGV>& GV,
    const unsigned nx,
    const unsigned ny,
    const unsigned nz,
    const typename DerivedS::Scalar isovalue,
    Eigen::PlainObjectBase<DerivedV>& V,
    Eigen::PlainObjectBase<DerivedF>& F)
{
    typedef typename DerivedS::Scalar Scalar;
    typedef unsigned Index;

    // use same order as a2fVertexOffset
    const unsigned ioffset[8] = { 0, 1, 1 + nx, nx, nx * ny, 1 + nx * ny, 1 + nx + nx * ny, nx + nx * ny };

    std::unordered_map<std::int64_t, int> E2V;  // E2V: current edge (GV_i, GV_j) to vertex (V_k) map
    V.resize(std::pow(nx * ny * nz, 2. / 3.), 3);
    F.resize(std::pow(nx * ny * nz, 2. / 3.), 3);
    Index n = 0;  // n: Current number of mesh vertices (i.e., occupied rows in V)
    Index m = 0;  // m: Current number of mesh triangles (i.e., occupied rows in F)

    const auto xyz2i = [&nx, &ny](const int& x, const int& y, const int& z) -> unsigned
    {
        return x + nx * (y + ny * z);
    };

    const auto cube = [&GV, &S, &V, &n, &F, &m, &isovalue, &E2V, &xyz2i, &ioffset](const int x, const int y, const int z)
    {
        // Get the index of current cube
        const unsigned i = xyz2i(x, y, z);

        // Get the index and SDF value of cube's 8 vertices
        // cI(c): the index of c-th vertex in this cube
        // cS(c): the SDF value of c-th vertex in this cube
        Eigen::Matrix<Index, 8, 1> cI;
        Eigen::Matrix<Scalar, 8, 1> cS;
        for (int c = 0; c < 8; c++) {
            const unsigned ic = i + ioffset[c];
            cI(c) = ic;
            cS(c) = S(ic);
        }

        march_cube(GV, cS, cI, isovalue, V, n, F, m, E2V);
    };

    // March over all cubes (loop order chosen to match memory)
    for (int z = 0; z < nz - 1; z++) {
        for (int y = 0; y < ny - 1; y++) {
            for (int x = 0; x < nx - 1; x++) {
                cube(x, y, z);
            }
        }
    }
    V.conservativeResize(n, 3);
    F.conservativeResize(m, 3);
}