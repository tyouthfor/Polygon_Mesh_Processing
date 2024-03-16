# Octree

### Reference
* https://github.com/brandonpelfrey/SimpleOctree/tree/master

八叉树的 C++ 实现，一种描述三维空间的树状数据结构，每个结点表示一个 cube；每个节点有八个子结点，表示该 cube 的八个**象限(Octant)**。每个叶结点存储一个三维空间中的点，其他结点不存储（分割方式不唯一）。

![image](https://github.com/tyouthfor/Polygon_Mesh_Processing/blob/main/image/octree.png)

相当于可变分辨率的体素网格。
<br/>

### Application

八叉树能加速空间查询，例如将空间中的点视为物体，给定一个物体 A 的包围盒，粗略判断哪些物体可能与 A 碰撞。如果根节点代表的 cube 与 A 不相交，则根节点的子树直接剪枝，大大减小了搜索空间。当点均匀分布在空间内时，时间复杂度从 $O(n)$ 降为 $O(\log n)$ 。

当然除了管理视为点的物体外，也能管理占体积的物体。

————参考自 [游戏场景管理的八叉树算法是怎样的？ - 知乎 (zhihu.com)](https://www.zhihu.com/question/25111128/answer/30129131) 。
