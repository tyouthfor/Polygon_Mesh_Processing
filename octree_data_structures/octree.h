#ifndef __octree_h__
#define __octree_h__

#include"octree_point.h"
#include<cstddef>
#include<vector>

namespace octree
{
	/*
	* Name: octree_t
	* Func: octree/octree node class.
	* @Varia origin: The pyhsical center of the node.
	* @Varia half_dim: Half the width/height/depth of the node (cube).
	* @Varia children: Pointers to child octants.
	* @Varia data: Data to be stored at the node.
	*/
	class octree_t
	{
	private:
		Vec3 origin;
		Vec3 half_dim;
		octree_t* children[8];
		octree_point_t* data;

	public:
		octree_t(const Vec3& origin, const Vec3& half_dim)
		{
			this->origin = origin;
			this->half_dim = half_dim;
			for (int i = 0; i < 8; ++i) {
				children[i] = nullptr;
			}
			data = nullptr;
		}

		~octree_t()
		{
			for (int i = 0; i < 8; ++i) {
				delete children[i];
			}
		}

		/*
		* Name: is_leaf
		* Func: return true if current node is a leaf node.
		*/
		bool is_leaf() const;

		/*
		* Name: get_octant_containing_point
		* Func: Determine which octant of the node would contain the point.
		*/
		int get_octant_containing_point(const Vec3& point) const;

		/*
		* Name: insert
		* Func: Insert a point into one of leaf nodes of the octree (may split current node).
		*/
		void insert(octree_point_t* point);

		/*
		* Name: get_points_inside_box
		* Func: Query the octree for points within a bounding box defined by min/max point.
		*/
		void get_points_insede_box(const Vec3& box_pmin, const Vec3& box_pmax, std::vector<octree_point_t*>& results);
	};
}

#endif // !__octree_h__
