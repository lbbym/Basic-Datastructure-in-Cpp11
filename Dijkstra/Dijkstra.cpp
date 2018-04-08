/**
 * LBB_Linux
 * Dijkstra
 */

/**
 * Vertex结构的描述
 * 以实际的c++表示，路径通常为Vertex*型
 * 而我们描述的许多代码段
 * 或者要求解引操作符*，或者使用
 * ->操作符，而不用.操作符
 */
struct Vertex
{
	List adj;  //邻接list(表)
	bool known;
	DistType dist;	//DistType很可能是int
	Vertex path;	//很可能是vertex*
		//其他数据和成员函数视需求而定
};

/**
 * 假设到v的最短路径存在
 * 在运行dijkstra算法之后打印出最短路径
 */
void Graph::printPath(Vertex v)
{
	if (v.path != NOT_A_VERTEX)
	{
		printPath(v.path);
		cout << "to";
	}
	cout << v;
}

/**
 * 算法部分
 */
void Graph::dijkstra(Vertex s)
{
	for each Vertex v
	{
		v.dist = INFINITY;
		v.known = false;
	}

	s.dist = 0;

	while (there is an unknown distance Vertex)
	{
		Vertex v = smallest unknown distance Vertex;
		
		v.known = true;

		for each Vertex w adjacent to v
			if (!w.known)
			{
				DistType cvm = cost of edge from v to w;

				if (v.dist + cvm < w.dist)
				{
					//更新w
					decrease(w.dist to v.dist + cvm);
					w.path = v;
				}
			}
	}
}