#include <igl/marching_cubes.h>
#include <igl/signed_distance.h>
#include <igl/read_triangle_mesh.h>
#include <igl/voxel_grid.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>
#include <iostream>

int main(int argc, char* argv[])
{
    // 1. Read 3D model data (.obj)
    Eigen::MatrixXi F;
    Eigen::MatrixXd V;
    igl::read_triangle_mesh(TUTORIAL_SHARED_PATH "/armadillo.obj", V, F);
    std::cout << "Creating grid...\n";

    // Number of vertices on the largest side
    const int s = 100;

    // 2. Create grid
    // res: The number of grids' vertices in 3 dimensions
    // GV[i]: The position of grids' vertex[i]
    Eigen::RowVector3i res;
    Eigen::MatrixXd GV;
    igl::voxel_grid(V, 0, s, 1, GV, res);

    // 3. Compute SDF
    // S: Signed distances
    // B: Map values in S to symbols (-1, 0, 1) --> aliasing artifacts
    std::cout << "Computing distances...\n";
    Eigen::VectorXd S, B;
    Eigen::VectorXi I;
    Eigen::MatrixXd C, N;
    igl::signed_distance(GV, V, F, igl::SIGNED_DISTANCE_TYPE_PSEUDONORMAL, S, I, C, N);
    B = S;
    std::for_each(B.data(), B.data() + B.size(), [](double& b) {b = (b > 0 ? 1 : (b < 0 ? -1 : 0)); });

    // 4. Fill the SV and SF by marching cubes algorithm
    // SV[i]: The position of triangle meshes' vertex[i]
    // SF[i]: The index of vertices of i-th triangle mesh
    std::cout << "Marching cubes...\n";
    Eigen::MatrixXd SV, BV;
    Eigen::MatrixXi SF, BF;
    igl::marching_cubes(S, GV, res(0), res(1), res(2), 0, SV, SF);
    igl::marching_cubes(B, GV, res(0), res(1), res(2), 0, BV, BF);

    // 5. Display the result
    std::cout << R"(Usage:
        '1'  Show original mesh.
        '2'  Show marching cubes contour of signed distance.
        '3'  Show marching cubes contour of indicator function.
    )";
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(SV, SF);
    viewer.callback_key_down = [&](igl::opengl::glfw::Viewer& viewer, unsigned char key, int mod) -> bool
    {
        switch (key) {
        case '1':
            viewer.data().clear();
            viewer.data().set_mesh(V, F);
            break;
        case '2':
            viewer.data().clear();
            viewer.data().set_mesh(SV, SF);
            break;
        case '3':
            viewer.data().clear();
            viewer.data().set_mesh(BV, BF);
            break;
        default:
            return false;
        }
        viewer.data().set_face_based(true);
        return true;
    };
    viewer.launch();
}