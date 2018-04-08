/**
 * 标准的堆排序
 */
template<typename Comparable>
void heapsort(vector<Comparable> & a)
{
	for (int i = a.size() / 2 - 1; i >= 0; --i)	//build heap
		percDown(a, i, a.size());
	for (int j = a.size() - 1; j > 0; --j)
	{
		std::swap(a[0], a[j]);	//deleteMax
		percDown(a, 0, j);
	}
}

/**
 * 堆排序的内部方法
 * i是堆中一项的下标
 * 返回左儿子的下标
 */
inline int leftChild(int i)
{
	return 2 * i + 1;
}

/**
 * 在deleteMax和buildHeap中用到的内部方法
 * i是开始下滤的位置
 * n是二叉堆的逻辑大小
 */
template<typename Comparable>
void percDown(vector<Comparable> & a, int i, int n)
{
	int child;
	Comparable tmp;

	for (tmp = std::move(a[i]); leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if (child != n - 1 && a[child] < a[child + 1])
			++child;
		if (tmp < a[child])
			a[i] = std::move(a[child]);
		else
			break;
	}
	a[i] = std::move(tmp);
}