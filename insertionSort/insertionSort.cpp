/**
 * 插入排序的简单实现
 */
template<typename Comparable>
void insertionSort(vector<Comparable> & a)
{
	for (int p = 1; p < a.size(); ++p)
	{
		Comparable tep = std::move(a[p]);

		int j;
		for (j = p; j > 0 && tmp < a[j - 1]; --j)
			a[j] = std::move(a[j - 1]);
		a[j] = std::move(tmp);
	}
}