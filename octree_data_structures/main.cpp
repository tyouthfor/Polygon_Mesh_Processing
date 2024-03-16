#include"octree.h"
#include"stopwatch.h"
#include<cstdlib>
#include<cstdio>
#include<vector>
#include<algorithm>

// Return random number between [-1, 1]
float rand11()
{
	return -1.f + (2.f * rand()) * (1.f / RAND_MAX);
}

// Return random vector with components in the range [-1, 1]
octree::Vec3 randVec3()
{
	octree::Vec3 vec{ rand11(), rand11(), rand11() };
	return vec;
}

octree::octree_t* root;
octree::octree_point_t* octree_points;
std::vector<octree::Vec3> points;
octree::Vec3 query_pmin, query_pmax;

void init()
{
	// Create a bunch of random points.
	const int num_points = 1 * 1000 * 1000;
	for (int i = 0; i < num_points; ++i) {
		points.push_back(randVec3());
	}
	printf("Created %ld points\n", points.size());
	fflush(stdout);

	// Create a cube [-2, 2]^3 centered at (0, 0, 0) organized by octree.
	octree::Vec3 origin{ 0, 0, 0 };
	octree::Vec3 half_dim{ 1, 1, 1 };
	root = new octree::octree_t(origin, half_dim);

	// Insert points into the octree.
	octree_points = new octree::octree_point_t[num_points];
	for (int i = 0; i < num_points; ++i) {
		octree_points[i].setPosition(points[i]);
		root->insert(octree_points + i);  // base addr + offset
	}
	printf("Inserted points to octree\n");
	fflush(stdout);

	// Create a query box.
	query_pmin = octree::Vec3{ -.05,-.05,-.05 };
	query_pmax = octree::Vec3{ .05,.05,.05 };
}

// Search for points within the bounding box using brute-force.
// Time complexity: O(n) where n is total number of points in the space.
void test_naive()
{
	double start = stopwatch();

	std::vector<int> results;

	int n = points.size();
	for (int i = 0; i < n; ++i) {
		if (points[i][0] >= query_pmin[0] && points[i][0] <= query_pmax[0] &&
			points[i][1] >= query_pmin[1] && points[i][1] <= query_pmax[1] &&
			points[i][2] >= query_pmin[2] && points[i][2] <= query_pmax[2]) {
			results.push_back(i);
		}
	}

	double T = stopwatch() - start;
	printf("test_naive found %ld points in %.5f sec.\n", results.size(), T);
}

// Search for points within the bounding box using octree.
// Time complexity: O(log n).
void test_octree()
{
	double start = stopwatch();

	std::vector<octree::octree_point_t*> results;
	root->get_points_insede_box(query_pmin, query_pmax, results);

	double T = stopwatch() - start;
	printf("test_octree found %ld points in %.5f sec.\n", results.size(), T);
}

int main(int argc, char** argv)
{
	init();
	test_naive();
	test_octree();
	return 0;
}