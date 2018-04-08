/**
 * 归并排序算法（驱动程序）
 */
template <typename Comparable>
void mergeSort(vector<Comparable> & a)
{
	vector(Comparable) tmpArray(a.size());

	mergeSort(a, tmpArray, 0, a.size() - 1);
}

/**
 * 进行递归调用的内部方法
 * a为Comparable项的数组
 * tmpArray为放置归并结果的数组
 * left为子数组最左元素的下标
 * right为子数组最右元素的下标
 */
template<typename Comparable>
void mergeSort(vector<Comparable> & a,
	vector<Comparable> & tmpArray, int left, int right)
{
	if (left < right)
	{
		int center = (left + right) / 2;
		mergeSort(a, tmpArray, left, center);
		mergeSort(a, tmpArray, center + 1, right);
		merge(a, tmpArray, left, center + 1, right);
	}
}

/**
 * 合并子数组已排序两半部分的内部方法
 * a为Comparable项的数组
 * tmpArray为放置归并结果的数组
 * leftPos为子数组最左元素的下标
 * rightPos为后半部分起点的下标
 * rightEnd为子数组最右元素的下标
 */
template<typename Comparable>
void merge(vector<Comparable> & a, vector<Comparable> & tmpArray,
	int leftPos, int rightPos, int rightEnd)
{
	int leftEnd = rightPos - 1;
	int tmpPos = leftPos;
	int numElements = rightEnd - leftPos + 1;

	//主循环
	while (leftPos <= leftEnd && rightPos <= rightEnd)
		if (a[leftPos] <= a[rightPos])
			tmpArray[tmpPos++] = std::move(a[leftPos++]);
		else
			tmpArray[tmpPos++] = std::move(a[rightPos++]);

	while (leftPos <= leftEnd) //复制前半部分的剩余元素
		tmpArray[tmpPos++] = std::move(a[leftPos++]);

	while (rightPos <= rightEnd)  //复制后半部分的剩余元素
		tmpArray[tmpPos++] = std::move(a[rightPos++]);

	//将tmpArray复制回a
	for (int i = 0; i < numElements; ++i, --rightEnd)
		a[rightEnd] = std::move(tmpArray[rightEnd]);
}