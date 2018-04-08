template<typename HashedObj>
class HashTable
{
public:
	explict HashTable(int size = 101):array(nextPrime(size))
	{
		makeEmpty();
	}
	

	bool cintains(const HashedObj & x)const
	{
		return isActive(findPos(x));
	}

	viod makeEmpty()
	{
		currentSize = 0;
		for (auto & entry : array)
			entry.info = EMPTY;
	}
	bool insert(const HashedObj & x)
	{
		//将x作为active插入
		int currentPos = findPos(x);
		if (isActive(currentPos))
			return false;

		array[currentPos].element = x;
		array[currentSize].info = ACTIVE;

		//再散列
		if (++currentSize > array.size() / 2)
			rehash();

		return true;
	}
	bool insert(HashedObj && x);
	bool remove(const HashedObj & x)
	{
		int currentPos = findPos(x);
		if (!isActive(x))
			return false;

		array[currentPos].info = DELETED;
		return true;
	}

	enum EntryType{ACTIVE, EMPTY, DELETED};

private:
	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry(const HushedObj & e = HashedObj{}, EntryType i = EMPTY)
			:element{ e }, info{ i } {}
		HashEntry(HushedObj && e, EntryType i = EMPTY)
			:element{ std::move(e) }, info{ i } {}
	};

	vector<HashEntry> array;
	int currentSize;

	bool isActive(int currentPos)const 
	{
		return array[currentPos].info == ACTIVE;
	}

	int findPos(const HashedObj & x)const
	{
		int offset = 1;
		int currentPos = myhash(x);

		while (array[currentPos].info != EMPTY &&
			   array[currentPos].element != x)
		{
			//平方消解函数，计算第i次探查
			currentPos += offset;
			offset += 2;
			if (currentPos >= array.size())
				currentPos -= array.size();
		}

		return currentPos;
	}
	void rehash();
	size_t myhash(const HashedObj & x)const;
};

/**
 * 对平方探测散列表的再散列
 */
template<typename HashedObj>
inline void HashTable<HashedObj>::rehash()
{
	vector<HashEntry> oldArray = array;

	//创建新的两倍大小的空表
	array.resize(nextPrime(2 * oldArray.size()));
	for (auto & entry : array)
		entry.info = EMPTY;

	//复制整个表
	currentSize = 0;
	for (auto & entry : oldArray)
		if (entry.info == ACTIVE)
			insert(std::move(entry.element));
}