marching cubes algorithm

### �㷨Ŀ��

���� grid �Լ� SDF���� triangle mesh �ؽ�����ά����ı��档Ҫ��Ⱦ triangle mesh������Ҫ֪��ÿ�� triangle �Ķ���λ�ã���Ӧ���� marching_cubes ������ SV �������Լ�������������Ӧ���� marching_cubes ������ SF ��������

### �㷨˼��

��֪ grid ÿ�������λ�ã����� grid ÿ������� SDF ֵ���Բ�ֵ�� triangle mesh �Ķ���λ�ã������������ɸ��ݲ��õ���

### �㷨����

1. marching_cubes ����
* ����ÿ�� grid����ÿһ�� cube����� 8 ������������� SDF ֵ��Ȼ����� march_cube �������ؽ��� cube �е� triangle mesh��

2. march_cube ����
* ���� cube �� 8 �����㣬������ SDF ֵ�������жϸö���������������ϵ���õ�������� c_flags��
* ���� c_flags ��ѯ aiCubeEdgeFlags �ɵ� triangle mesh �� cube ����Щ���ཻ�����߱��� e_flags��
* ���� cube �� 12 ���ߣ�����ñ��� triangle mesh �ཻ�������Բ�ֵ���ཻ���λ�ã���¼�� V(n) �С�ͬʱ�������ཻ�����������¼�� edge_vertices(e) �У���ʾ����Ϊ e �ı��� triangle mesh �Ķ�������������һ���лᱻʹ�á�
* �ָ��� c_flags ��ѯ a2fConnectionTable �ɵø� cube ���ж��ٸ� triangle mesh �Լ�ÿ�� triangle mesh �������������ڵ� cube �ıߵ����������ݸñ�������ѯ edge_vertices ���ɵ� triangle mesh �������������������¼�� F(m) �С�