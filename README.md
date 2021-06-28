# Advanced Programming Exam

This is an implementation of a templated Binary Search Tree (BST) in C++. The header file contains such implementation, while the source file `main.cc` contains a set of tests that covers the possible cases of the bst functions and the Makefile compiles it creating an executable named bst.

##### Binary Search Tree
In the implementation there are two templated classes: one for the tree and one for the iterator. The former is templated on the key, on the value and on the comparison operator, it has a pointer to the head of the tree and a compare operator (`std::less` by default).
```c++
template <typename k_t, typename v_t, typename OP = std::less<k_t> >
class bst{
    using pair_type = std::pair<const k_t, v_t>;
    OP op;
    std::unique_ptr<node> head;
}
```

##### Node
Node is a struct contained in the class bst which makes use of the templated key and value. In addition there are two smart pointers to the childern and a raw pointer to the parent. A raw pointer is used because every node is a child of some other node (except from the head), therefore it is not possible to use unique ones.
```c++
struct node {
    std::unique_ptr<node> right;
    std::unique_ptr<node> left;
    k_t key;
    v_t value;
    node* parent;
}
```

##### Iterator
This second class allows to iterate through the bst and its declaration is inside the bst class. The iterator is templated on the pair given by key and value and its only member is a pointer to the current node.
```c++
class bst{
    template <typename O>
    class _iterator;
    using iterator = _iterator<pair_type>;
    using const_iterator = _iterator<const pair_type>;
}

template <typename k_t, typename v_t, typename OP>
template <typename O>
class bst<k_t,v_t,OP>::_iterator{
    using node = typename bst<k_t,v_t,OP>::node;
    node* current; 
}
```

### Functions

##### Insert

```c++
std::pair<iterator, bool> insert(const pair_type& x);
std::pair<iterator, bool> insert(pair_type&& x);
```
Inserts a new node and returns a pair of an iterator (pointing to the node) and a bool. The bool is true if a new node has been allocated, false otherwise (i.e., the key was already present in the tree). 

##### Emplace

```c++
template< class... Types >
std::pair<iterator,bool> emplace(Types&&... args);
```
Inserts a new element into the container constructed in-place with the given args, by invoking the insert() function with an argument of pair type.

##### Clear

```c++
void clear();
```
Clears the content of the tree by `reset` the head pointer.

##### Begin

```c++
iterator begin();
const_iterator begin() const;
const_iterator cbegin() const;
```

Returns an iterator pointing to the left-most node of the head, or to nullpointer if the tree is empty.

##### End

```c++
iterator end();
const_iterator end() const;
const_iterator cend() const;
```

Returns an iterator to one-past the last element.

##### Find

```c++
iterator find(const key_type& x);
const_iterator find(const key_type& x) const;
```
Finds a given key. If the key is present, it returns an iterator to the proper node, `end()` otherwise.

##### Balance

```c++
void balance();
```

This function balances the tree by copying keys and values of the nodes into a vector from the beginning of the bst to the end. Then, clears the tree and builds it again by recursively dividing the vector into two and inserting the middle values of the sub-vectors in the tree. 

##### Subscripting operator

```c++
value_type& operator[](const key_type& x);
value_type& operator[](key_type&& x);
```

Returns a reference to the value that is mapped to a key equivalent to `x` by calling find() function, and performing an insertion with a default value if such key does not already exist.

##### Put-to operator

```c++
friend
std::ostream& operator<<(std::ostream& os, const bst& x);
```
Returns a reference to ostream object that contains all values of the nodes from begin() to end().

##### Copy and move

The copy semantics perform a deep-copy: for copy constructor reset the head of the left side tree with the head of the right side tree, and for copy assignment, clear the tree and then move the head of the left side tree.
The move constructor and assignment are set as `default`.

##### Erase

```c++
void erase(const key_type& x);
```

Removes the node with the corresponding key (if it exists) and then, if needed, moves the nodes connected to the deleted one to maintain the bst structure.
