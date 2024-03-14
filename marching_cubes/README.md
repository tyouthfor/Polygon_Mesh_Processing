# Marching Cubes Algorithm

### 算法目标

根据 SDF（隐式），用 triangle mesh（显式）重建出三维物体的表面。要渲染 triangle mesh，就需要知道每个 triangle 的顶点位置（对应传入 marching_cubes 函数的 SV 变量）以及顶点索引（对应传入 marching_cubes 函数的 SF 变量）。

### 算法思想

将三维空间分割成一个个 cube，遍历处理每个 cube。根据 cube 每个顶点与物体的内外关系提前计算出 256 种情况的哈希表（marching_cubes_tables.h），再根据 cube 每个顶点的 SDF 值线性插值出 triangle mesh 的顶点位置，而顶点索引可查表得到。

### 算法流程

1. marching_cubes 函数
* 遍历每个 cube。对每个 cube，求出 8 个顶点的索引和 SDF 值，然后调用 march_cube 函数，重建该 cube 中的 triangle mesh。

2. march_cube 函数
* 遍历 cube 的 8 个顶点，根据其 SDF 值的正负判断该顶点与物体的内外关系，得到顶点编码 c_flags。
* 根据 c_flags 查询 aiCubeEdgeFlags 可得 triangle mesh 与 cube 的哪些边相交，即边编码 e_flags。
* 遍历 cube 的 12 条边，如果该边与 triangle mesh 相交，则线性插值出相交点的位置，记录在 V(n) 中。同时，创建相交点的索引，记录在 edge_vertices(e) 中，表示索引为 e 的边上 triangle mesh 的顶点索引，在下一步中会被使用。
* 又根据 c_flags 查询 a2fConnectionTable 可得该 cube 中有多少个 triangle mesh 以及每个 triangle mesh 的三个顶点所在的 cube 的边的索引。根据该边索引查询 edge_vertices 即可得 triangle mesh 的三个顶点的索引，记录在 F(m) 中。

### 算法改进

由于每个 cube 内最多 5 个 triangle mesh，采样率被限制，因此在一些精细表面（例如交界处的 sharp edges 和 corners）无法重建出细节。一种方法是以牺牲时间和空间为代价增加分辨率；令一种方法是在精细表面增加采样点，由此得到了 **Extended Marching Cubes**，它通过计算 SDF 的梯度来获得边缘信息，梯度大的地方多采样一些。

Marching Cubes 的另一个问题是，随着采样精度的增加，triangle mesh 数也随之增长地很快。一方面可以在提取之后做 simplification，另一方面可以直接在 Marching Cubes 上进行改进，通过一个自适应的**八叉树**，得到了 Real-time exploration of regular volume data by adaptive reconstruction of isosurfaces。

还有一个同样基于八叉树，但是与 Marching Cubes 走不同路线的隐式转显式的方法 **Dual Contouring**，它使用的顶点位于 cube 的内部而不是边上，可以提高 triangle mesh 的质量和拓扑结构。

————参考自 https://zhuanlan.zhihu.com/p/613392327

<br/>

要使表面光滑，最简单的算法是 **Laplacian Smoothing**。但 Marching Cubes 更常出现的问题是网格连接关系比较差，此时需要用到 **Remeshing** 算法，可以使用 mmgtools。曲面细分，可以使用 libgil 里的 **Subdivision**。

————参考自 https://www.zhihu.com/question/334472307
