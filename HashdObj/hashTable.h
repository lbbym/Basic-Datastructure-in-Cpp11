/**
 * LBB_Linux
 * HashTable
 */

template<typename HashedObj>
class HushTable
{
public:
	explicit HashTable(int size = 101);

	bool contains(const HashObj & x)const;
	
	void makeEmpty();
	bool insert(const HashObject & x);
	bool insert(HashedObj && x);
	bool remove(const HashedObj & x);

private:
	vector<list<HashedObj>> theLists;  //链表的数组
	int currentSize;

	/**
	 * 对分离链接散列表再散列
	 */
	void rehash()
	{
		vector<list<HashedObj>> oldLists = theLists;
		
		//创建两倍打的空表
		theLists.resize(nextPrime(2 * oldLists.size()));
		for (auto & theList : theLists)
			theList.clear();

		//复制整个表
		currentSize = 0;
		for (auto & theList : oldLists)
			for (auto x : theList)
				insert(std::move(x));
	}

	size_t myhash(const HashedObj & x)const
	{
		static hash<HashedObj> hf;
		return hf(x) % theLists.size();
	}

	//函数对象模板
	template<typename Key>
	class hash
	{
	public:
		size_t operator() (const Key & k) const;
	};

	//例如：：对string的操作
	template<>
	class hash<string>
	{
	public:
		size_t operator() (const string & key)
		{
			size_t hashVal = 0;

			for (char ch : key)
				hashVal = 37 * hashVal + ch;

			return hashVal;
		}
	};

	void makeEmpty()
	{
		for (auto & theList : theLists)
			theList.clear();
	}

	bool contains(const HashedObj & x)const
	{
		auto & whichList = theLists[myhash(x)];
		return find(begin(whichList), end(whichList), x) != end(whichList);
	}

	bool remove(const HashedObj & x)
	{
		auto whichList = theLists[myhash(x)];
		auto itr = find(begin(whichList), end(whichList), x);

		if (itr == end(whichList))
			return false;

		whichList.erase(itr);
		--currentSize;
		return false;
	}


	bool insert(const HashedObj & x)
	{
		auto & whichList = theLists[myhash(x)];
		if (find(begin(whichList), end(whichList), x) != end(whichList))
			return false;
		whichList.push_back(x);

		//再散列
		if (++currentSize > theLists.size())
			rehash();

		return true;
	}
};