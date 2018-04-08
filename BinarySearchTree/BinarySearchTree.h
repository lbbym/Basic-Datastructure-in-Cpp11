/*
**LBB_Linux
**Chongqing Univer
**2017/9/20
*/
template<typename Comparable>
class BinarySearchTree
{
public:
	BinarySearchTree();
	BinarySearchTree(const BinarySearchTree & rhs);
	BinarySearchTree()(BinarySearchTree && rhs);
	~BinarySearchTree();

	const Comparable & findMin() const;
	const Comparable & findMax() const;
	bool contains(const Comparable & x) const;
	bool isEmpty() const;
	void printTree(ostream & out = cout)const;

	void makeEmpty();
	void insert(const Comparable & x);
	void insert(Comparable && x);
	void remove(const Comparable & x);

	BinarySearchTree & operator=(const BinarySearchTree & rhs);
	BinarySearchTree & operator=(BinarySearchTree && rhs);

private:
	struct BinaryNode
	{
		Comparable element;
		BinaryNode *left;
		BinaryNode *right;

		BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt)
			:element{ theElement }, left{ lt }, right{ rt } {}

		BinaryNode(Comparable && theElement, BinaryNode *lt, BinaryNode *rt)
			:element{ std::move(theElement) }, left(lt), right{ rt } {}
	};

	BinaryNode *root;

	void insert(const Comparable & x, BinaryNode * & t);
	void insert(Comparable && x, BinaryNode * & t);
	void remove(const Comparable & x, BinaryNode * & t);
	BinaryNode * findMin(BinaryNode *t)const;
	BinaryNode * findMax(BinaryNode *t)const;
	bool contains(const Comparable & x, BinaryNode * t)const;
	void makeEmpty(BinaryNode * & t);
	void printTree(BinaryNode * t, ostream & out)const;
	BinaryNode * clone(BinaryNode *t)const;
};


/*
**拷贝构造函数
*/
template<typename Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const BinarySearchTree & rhs):root{nullptr}
{
	root = clone(rhs.root);
}

/*
**二叉查找树析构函数
*/
template<typename Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree()
{
	makeEmpty();
}

//如果在数中找到x，返回true
template<typename Comparable>
bool BinarySearchTree<Comparable>::contains(const Comparable & x) const
{
	return contains(x, root);
}

template<typename Comparable>
void BinarySearchTree<Comparable>::makeEmpty()
{
	return makeEmpty();
}

//将x插入到树中，忽略重复元
template<typename Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x)
{
	insert(x, root);
}

//将x从树中删除，如果没有x，什么也不做
template<typename Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x)
{
	remove(x, root);
}

/*
**向子树插入元素的内部方法
**x是要插入的项
**t为该子树的根节点
*置子树的新根
*/
template<typename Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x, BinaryNode *& t)
{
	if (t == nullptr)
		t = new BinaryNode{ x,nullptr,nullptr };
	else if (x < t->element)
		insert(x, t->left);
	else if (t->element < x)
		insert(x, t->right);
	else
		;//重复元:什么也不做
}

/*
**向子树插入元素的内部方法
**x是通过移动实现插入的项
**t为该子树的根
*/
template<typename Comparable>
void BinarySearchTree<Comparable>::insert(Comparable && x, BinaryNode * & t)
{
	if (t == nullptr)
		t = new BinaryNode{ std::move(x),nullptr,nullptr };
	else if (std::move(x) < t->element)
		insert(std::move(x), t->left);
	else if (t->element < std::move(x))
		insert(std::move(x), t->right);
	else
		;//重复元:什么也不做
}

/*
**从一颗子树删除一项的内部方法
**参数x是被删除的项
**参数t为该子树的根节点
**置该子树的新根
*/
template<typename Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x, BinaryNode * & t)
{
	if (t == nullptr)
		return;//没找到，什么也不做
	if (x < t->element)
		remove(x, t->left);
	else if (t->element < x)
		remove(x, t->right);
	else if (t->left != nullptr && t->right != nullptr)//有两个儿子
	{
		t->element = findMin(t->right)->element;
		remove(t->element, t->right);
	}
	else 
	{
		BinaryNode *oldNode = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete oldNode;
	}
}

/*
**找出树t中的最小项的内部方法
**返回包含最小项的节点
*/
template<typename Comparable>
BinaryNode * BinarySearchTree<Comparable>::findMin(BinaryNode * t) const
{
	if(t==nullptr)
		return nullptr;
	if (t->left == nullptr)
		return t;
	return findMin(t->left);
}

/*
**找出树t中最大项的内部方法
**返回包含最大项的节点
*/
template<typename Comparable>
BinaryNode * BinarySearchTree<Comparable>::findMax(BinaryNode * t) const
{
	if (t != nullptr)
		while (t->right != nullptr)
			t = t->right;
	return t;
}

/*
**测试一项是否在子树上的内部方法
**x是要查找的项
**t是作为该子树的根节点
*/
template<typename Comparable>
bool BinarySearchTree<Comparable>::contains(const Comparable & x, BinaryNode * t) const
{
	if (t == nullptr)
		return false;
	else if (x < t->element)
		return contains(x, t->left);
	else if (t->element < x)
		return contains(x, t->right);
	else
		return true;
}

/*
**使子树为空的内部方法
*/
template<typename Comparable>
void BinarySearchTree<Comparable>::makeEmpty(BinaryNode *& t)
{
	if (t != nullptr)
	{
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
	t = nullptr;
}


/**
 * 克隆子树的内部方法
 */
template<typename Comparable>
BinaryNode * BinarySearchTree<Comparable>::clone(BinaryNode * t) const
{
	if (t == nullptr)
		return nullptr;
	else
		return new BinaryNode{ t->element,clone(t->left),clone(t->right) };
}
