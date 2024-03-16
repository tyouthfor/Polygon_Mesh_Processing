#include"octree.h"

namespace octree
{
	bool octree_t::is_leaf() const
	{
		return children[0] == nullptr;
	}

	int octree_t::get_octant_containing_point(const Vec3& point) const
	{
		// Children follow a predictable pattern to make access simple.
		// Here, - means less than 'origin' in that dimension, + means greater than.
		// 
		// child:	0 1 2 3 4 5 6 7
		// x:       - - - - + + + +
		// y:       - - + + - - + +
		// z:       - + - + - + - +

		int oct = 0;
		if (point[0] >= origin[0]) { oct |= 4; }  // 000 | 100 = 100, 8 octants exclude 4 octants = 4 octants
		if (point[1] >= origin[1]) { oct |= 2; }  // 000 | 010 = 010, 4 octants exclude 2 octants = 2 octants
		if (point[2] >= origin[2]) { oct |= 1; }  // 000 | 001 = 001, 2 octants exclude 1 octants = 1 octants
		return oct;
	}

	void octree_t::insert(octree_point_t* point)
	{
		if (is_leaf()) {
			if (data == nullptr) {  // End condition.
				data = point;
				return;
			}
			else {
				// Save this data point that was here for a later re-insert.
				octree_point_t* old_point = data;
				data = nullptr;

				// Split the current node and create new empty trees for each child octant.
				for (int i = 0; i < 8; ++i) {
					Vec3 new_origin = origin;
					new_origin[0] += half_dim[0] * (i & 4 ? .5f : -.5f);  // child 4 5 6 7 will +
					new_origin[1] += half_dim[1] * (i & 2 ? .5f : -.5f);  // child 2 3 6 7 will +
					new_origin[2] += half_dim[2] * (i & 1 ? .5f : -.5f);  // child 1 3 5 7 will +
					children[i] = new octree_t(new_origin, half_dim * .5f);
				}

				// Re-insert the old point, and insert this new point.
				children[get_octant_containing_point(old_point->getPosition())]->insert(old_point);
				children[get_octant_containing_point(point->getPosition())]->insert(point);
			}
		}
		else {
			int octant = get_octant_containing_point(point->getPosition());
			children[octant]->insert(point);
		}
	}

	void octree_t::get_points_insede_box(const Vec3& box_pmin, const Vec3& box_pmax, std::vector<octree_point_t*>& results)
	{
		// Leaf node: see if the current data point is inside the bounding box.
		if (is_leaf()) {
			if (data != nullptr) {
				const Vec3& pos = data->getPosition();
				if (pos[0] >= box_pmin[0] && pos[0] <= box_pmax[0] && 
					pos[1] >= box_pmin[1] && pos[1] <= box_pmax[1] && 
					pos[2] >= box_pmin[2] && pos[2] <= box_pmax[2]) {
					results.push_back(data);
				}
			}
		}

		// Interior node: see if the child octant intersects with the bounding box.
		// Note: It's incorrect that the child octant must be inside the bounding box.
		else {
			for (int i = 0; i < 8; ++i) {
				// Compute the min/max corners of the child octant.
				const Vec3 child_pmax = children[i]->origin + children[i]->half_dim;
				const Vec3 child_pmin = children[i]->origin - children[i]->half_dim;

				if (child_pmax[0] < box_pmin[0] || child_pmax[1] < box_pmin[1] || child_pmax[2] < box_pmin[2]) { continue; }
				if (child_pmin[0] > box_pmax[0] || child_pmin[1] > box_pmax[1] || child_pmin[2] > box_pmax[2]) { continue; }
				children[i]->get_points_insede_box(box_pmin, box_pmax, results);
			}
		}
	}
}