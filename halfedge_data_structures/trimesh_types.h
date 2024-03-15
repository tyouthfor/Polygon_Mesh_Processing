#ifndef __trimesh_types_h__
#define __trimesh_types_h__

// 1. #ifndef #define �Ǿ����ͷ�ļ�������Header Guard�����ƣ���ֹͬһͷ�ļ�����ΰ�����������ظ��������

// 2. ��������� namespace �¿��Ա���ȫ��������ͻ��ͬʱ������ģ�黯��������ߴ����ά���ԺͿɶ���

// 3. ���ض����Ա������ query ����
// (1). index_t& start() {} : return �ǳ������� + �ǳ����������ԶԷ��ص����Ա���������޸�
// (2). const index_t& start() {} : return �������� + �����������ܶԷ��ص����Ա���������޸�
	// ���� const index_t & start() {} : �������޷����ؽ��������������ֵĺ������������س�������ǳ���������ʹ����������ͬ��
	// ���� index_t& start() const {} : ����ȫ����Ϊ��������֤���޸����Ա���������ǳ������������޸Ĳ���
	// �������������������볣����Ҫһ��ʹ��
// �����������������ѡ�������һ����������ߴ��밲ȫ�ԺͿ�ά����

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
