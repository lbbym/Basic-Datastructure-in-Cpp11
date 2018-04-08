#ifndef CUCKOO_HASH_TABLE_H
#define CUCKOO_HASH_TABLE_H

/**
 * LBB_CC
 * 2017/11/10
 */

#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include "UniformRandom.h"


/**
 * 为杜鹃散列生成泛型HashFamily接口
 */
template<typename AnyType>
class CucckooHashFamily
{
public:
	size_t hash(const AnyType & x, int which)const;
	int getNumberofFuntions();
	void generateNewFunctions();
};

template <int count>
class StringHashFamily
{
public:
	StringHashFamily() :MULTIPLIERS(count)
	{
		generateNewFunctions();
	}

	int getNumberofFunctions()const
	{
		return count;
	}

	void generateNewFunctions()
	{
		for (auto & mult : MULTIPLIERS)
			mult = r.nextInt();
	}

	size_t hash(const string & x, int which)const
	{
		const int multiplier = MULTIPLIERS[which];
		size_t hashVal = 0;

		for (auto ch : x)
			hashVal = multiplier*hashVal + ch;

		return hashVal;
	}
private:
	vector<int> MULTIPLIERS;
	UniformRandom r;
};

int nextPrime(int n);

#define MAX_LOAD 0.40

//#define MAX_LOAD 0.40 //g++ 4.6 does not support static const double

//CuckooHashing Hash table class
//
//CONSTRUCTION: an approximate initial size or default of 101
//
//**************PUBLIC OPERATIONS****************
//bool insert(x)		-->Insert x
//bool remove(x)		-->Remove x
//bool contains(x)		-->Return true if x is present
//void makeEmpty()		-->Remobe all items
//int  hashCode(string str)	-->Global method to hash strings

/**
 * 杜鹃散列的类接口
 */

template <typename AnyType, typename HashFamily>
class HashTable
{
public:
	
	explicit HashTable(int size = 101) :array(nextPrime(size))
	{
		numHashFunctions = hashFunctions.getNumberofFunctions();
		rehashes = 0;
		makeEmpty();
	}

	bool contains(const AnyType & x)const
	{
		return findPos(x) != -1;
	}

	void makeEmpty()
	{
		currentSize = 0;
		for (auto & entry : array)
			entry.isActive = false;
	}

	/**
	 * 从散列表中删除x
	 * 若项x找到且被删除则返回true
	 */
	bool remove(const AnyType & x)
	{
		int currentPos = findPos(x);
		if (!isActive(currentPos))
			return false;
		
		array[currentPos].isActive = false;
		--currentSize;
		return true;
	}

	/**
	 * 杜鹃散列中共有的插入例程
	 */
	bool insert(const AnyType & x)
	{
		if (contains(x))
			return false;

		if (currentSize >= array.size() * MAX_LOAD)
			expand();

		return insertHelper(x);
	}

	bool insert(AnyType && x)
	{
		if (contains(x))
			return false;

		if (currentSize >= array.size()*MAX_LOAD)
			expand();

		return insertHelper1(std::move(x));
	}

	int size()const
	{
		return currentSize;
	}

	int capacity()const
	{
		return array.size();
	}

private:

	struct HashEntry
	{
		AnyType element;
		bool isActive;

		HashEntry(const AnyType & e = AnyType(), bool a = false)
			:element{ e }, isActive{ a } {}
		HashEntry(AnyType && e, bool a = false)
			:element{ std::move(e) }, isActive{ a } {}
	};

	vector<HashEntry> array;
	int currentSize;
	int numHashFunctions;
	int rehashes;
	UniformRandom r;
	HashFamily hashFunctions;

	//static const double MAX_LOAD = 0.40;
	static const int ALLOWED_REHASHDS = 5;

	bool insertHelper1(const AnyType & xx)
	{
		const int COUNT_LIMIT = 100;
		AnyType x = xx;

		while (true)
		{
			int lastPos = -1;
			int pos;

			for (int count = 0; count < COUNT_LIMIT; ++count)
			{
				for (int i = 0; i < numHashFunctions; ++i)
				{
					pos = myhash(x, i);

					if (!isActive(pos))
					{
						array[pos] = std::move(HashEntry{ std::move(x),true });
						++currentSize;
						return true;
					}
				}

				//None of the spots are available. Kick out random one
				int i = 0;
				do
				{
					pos = myhash(x, r.nextInt(numHashFunctions));
				} while (pos == lastPos && i++ < 5);

				lastPos = pos;
				std::swap(x, array[pos].element);
			}

			if (++rehashes > ALLOWED_REHASHDS)
			{
				expand();	//Make the table bigger
				rehashes = 0;//重置rehashes的计数
			}
			else
				rehash(); //表大小相同，散列函数都是新的
		}
	}

	bool insertHelper1(AnyType && x)
	{
		const int COUNT_LIMIT = 100;

		while (true)
		{
			int lastPos = -1;
			int pos;

			for (int count = 0; count < COUNT_LIMIT; ++count)
			{
				for (int i = 0; i < numHashFunctions; ++i)
				{
					pos = myhash(x, i);

					if (!isActive(pos))
					{
						array[pos] = std::move(HashEntry{ std::move(x),true });
						++currentSize;
						return true;
					}
				}

				//None of the spots are available. Kick random one
				int i = 0;
				do
				{
					pos = myhash(x, r.nextInt(numHashFunctions));
				} while (pos == lastPos && i++ < 5);

				lastPos = pos;
				std::swap(x, array[pos].element);
			}

			if (++rehashes > ALLOWED_REHASHDS)
			{
				expand();
				rehashes = 0;
			}
			else
				rehash();
		}
	}

	/**
	 * 查找所有散列函数的位置
	 * 返回查找终止的位置，若找不到则返回-1
	 */
	int findPos(const AnyType & x)const
	{
		for (int i = 0; i < numHashFunctions; ++i)
		{
			int pos = myhash(x, 1);

			if (isActive(pos) && array[pos].element == x)
				return pos;
		}

		return -1;
	}

	bool isActive(int currentPos)const
	{
		return currentPos != -1 && array[currentPos].isActive;
	}

	void expand()
	{
		rehash(static_cast<int>(array.size() / MAX_LOAD));
	}

	void rehash()
	{
		hashFunctions.generateNewFunctions();
		rehash(array.size());
	}

	void rehash(int newSize)
	{
		vector<HashEntry> oldArray = array;

		//Create new double-sized, empty table
		array.resize(nextPrime(newSize));
		for (auto & entry : array)
			entry.isActive = false;

		//Copy table over
		currentSize = 0;
		for (auto & entry : oldArray)
			if (entry.isActive)
				insert(std::move(entry.element));
	}

	size_t myhash(const AnyType & x, int which)const
	{
		return hashFunctions.hash(x, which) % array.size();
	}
};

#endif // !CUCKOO_HASH_TABLE_H