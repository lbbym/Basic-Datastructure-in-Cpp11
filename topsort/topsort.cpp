/**
 * LBB_Linux
 * 实施拓扑排序的伪代码
 * O(|E|+|V|)
 */
void Graph::topsort()
{
	Queue<Vertex> q;
	int counter = 0;

	q.makeEmpty();
	for each Vertex v
		if (v.indegree == 0)
			q.enqueue(v);

	while (!q.isEmpty())
	{
		Vertex v = q.dequeue();
		v.topNum = ++counter; //分配下一个拓扑编码

		for each Vertex w adjacent to v
			if (--w.indegree == 0)
				q.enqueue(w);
	}

	if (counter != NUM_VERTICES) //不是无圈图
		throw CycleFoundException{};
}