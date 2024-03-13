# Marching Cubes Algorithm

### 算法目标

根据 grid 以及 SDF（隐式），用 triangle mesh（参数）重建出三维物体的表面。要渲染 triangle mesh，就需要知道每个 triangle 的顶点位置（对应传入 marching_cubes 函数的 SV 变量）以及顶点索引（对应传入 marching_cubes 函数的 SF 变量）。

### 算法思想

已知 grid 每个顶点的位置，根据 grid 每个顶点的 SDF 值线性插值出 triangle mesh 的顶点位置，而顶点索引可根据查表得到。

### 算法流程

1. marching_cubes 函数
* 遍历每个 grid。对每一个 cube，求出 8 个顶点的索引和 SDF 值，然后调用 march_cube 函数，重建该 cube 中的 triangle mesh。

2. march_cube 函数
* 遍历 cube 的 8 个顶点，根据其 SDF 值的正负判断该顶点与物体的内外关系，得到顶点编码 c_flags。
* 根据 c_flags 查询 aiCubeEdgeFlags 可得 triangle mesh 与 cube 的哪些边相交，即边编码 e_flags。
* 遍历 cube 的 12 条边，如果该边与 triangle mesh 相交，则线性插值出相交点的位置，记录在 V(n) 中。同时，创建相交点的索引，记录在 edge_vertices(e) 中，表示索引为 e 的边上 triangle mesh 的顶点索引，在下一步中会被使用。
* 又根据 c_flags 查询 a2fConnectionTable 可得该 cube 中有多少个 triangle mesh 以及每个 triangle mesh 的三个顶点所在的 cube 的边的索引。根据该边索引查询 edge_vertices 即可得 triangle mesh 的三个顶点的索引，记录在 F(m) 中。
