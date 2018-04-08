/**
 * 快速排序算法（驱动程序）
 */
template<typename Comparable>
void quicksort(vector<Comparable> & a)
{
	quicksort(a, 0, a.size() - 1);
}

/**
 * 执行三数中值分割
 * 返回left， center， right三项的中值
 * 将它们排序并隐匿枢纽元
 */
template<typename Comparable>
const Comparable & meidan3(vector<Comparable> & a, int left, int right)
{
	int center = (left + right) / 2;

	if (a[center] < a[left])
		std::swap(a[left], a[center]);
	if (a[right] < a[left])
		std::swap(a[left], a[right]);
	if (a[right] < a[center])
		std::swap(a[center], a[right]);

	//讲枢纽元置于right-1处
	std::swap(a[center], a[right - 1]);
	return a[right - 1];
}

/**
 * 进行递归调用的内部快速排序方法
 * 使用三数中值分割法，以及截止范围是10的截至技术
 * a是Comparable项的数组
 * left为子数组最左元素的下标
 * right为子数组最右元素的下标
 */
template<typename Comparable>
void quicksort(vector<Comparable> & a, int left, int right)
{
	if (left + 10 <= right)
	{
		const Comparable & pivot = median3(a, left, right);

		//开始分割
		int i = left, j = right - 1;
		for (;;)
		{
			while (a[++i] < pivot) {}
			while (pivot < a[--j]) {}
			if (i < j)
				std::swap(a[i], a[j]);
			else
				break;
		}

		std::swap(a[i], a[right - 1]); //恢复枢纽元

		quicksort(a, left, i - 1); //将小于等于枢纽元的元素排序
		quicksort(a, i + 1, right); //将大于等于枢纽元的元素排序
	}
	else //对子数组进行插入排序
		insertionSort(a, left, right);
}