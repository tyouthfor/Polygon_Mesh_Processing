#ifndef __trimesh_types_h__
#define __trimesh_types_h__

// 1. #ifndef #define 是经典的头文件保护（Header Guard）机制，防止同一头文件被多次包含而引起的重复定义错误

// 2. 将代码放在 namespace 下可以避免全局命名冲突，同时有助于模块化开发，提高代码可维护性和可读性

// 3. 重载对类成员变量的 query 函数
// (1). index_t& start() {} : return 非常量引用 + 非常函数，可以对返回的类成员变量进行修改
// (2). const index_t& start() {} : return 常量引用 + 常函数，不能对返回的类成员变量进行修改
	// 不能 const index_t & start() {} : 编译器无法重载仅按返回类型区分的函数，但能重载常函数与非常函数（即使返回类型相同）
	// 不能 index_t& start() const {} : 不安全，因为常函数保证不修改类成员变量，但非常量引用允许修改操作
	// 综上所述，常量引用与常函数要一起使用
// 编译器会根据上下文选择调用哪一个函数，提高代码安全性和可维护性

namespace trimesh
{
	typedef long index_t;

	/*
	* Name: edge_t
	* Func: Edge class represented by indices of two vertices.
	*/
	class edge_t
	{
	public:
		edge_t() { vertex[0] = vertex[1] = -1; }

		index_t& start() { return vertex[0]; }
		const index_t& start() const { return vertex[0]; }

		index_t& end() { return vertex[1]; }
		const index_t& end() const { return vertex[1]; }

	private:
		index_t vertex[2];
	};

	/*
	* Name: triangle_t
	* Func: Triangle class represented by indices of three vertices.
	*/
	class triangle_t
	{
	public:
		triangle_t() { vertex[0] = vertex[1] = vertex[2] = -1; }

		index_t& i() { return vertex[0]; }
		const index_t& i() const { return vertex[0]; }

		index_t& j() { return vertex[1]; }
		const index_t& j() const { return vertex[1]; }

		index_t& k() { return vertex[2]; }
		const index_t& k() const { return vertex[2]; }

	private:
		index_t vertex[3];
	};
}

#endif // !__trimesh_types_h__
