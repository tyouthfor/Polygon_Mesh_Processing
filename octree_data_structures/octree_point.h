#ifndef __octree_point_h__
#define __octree_point_h__

#include<Eigen/Dense>

namespace octree
{
	typedef Eigen::Vector3d Vec3;

	/*
	* Name: octree_point_t
	* Func: Simple point class to insert into the octree. It can store other attributes.
	*/
	class octree_point_t
	{
	private:
		Vec3 pos;

	public:
		octree_point_t() {  }
		octree_point_t(const Vec3& pos) { this->pos = pos; }

		void setPosition(const Vec3& pos) { this->pos = pos; }
		const Vec3& getPosition() const { return pos; }
	};
}

#endif // !__octree_point_h__
