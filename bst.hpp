#include <utility>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <math.h>

#define COUNT 10

////////////////////////////////////// BST //////////////////////////////////////

template <typename k_t, typename v_t, typename OP = std::less<k_t> >
class bst{

////////////////////////////////////// NODE //////////////////////////////////////

    struct node {
        std::unique_ptr<node> right;
        std::unique_ptr<node> left;
        k_t key;
        v_t value;
        node* parent;

        node(const k_t &k, const v_t &v): key{k}, value{v}, parent{nullptr} {}; // l-value
        node(const k_t &k, const v_t &v, node* n): key{k}, value{v}, parent{n} {};
        
        node(const k_t &&k, v_t &&v): key{std::move(k)}, value{std::move(v)}, parent{nullptr} {}; // r-value
        node(const k_t &&k, v_t &&v, node* n): key{std::move(k)}, value{std::move(v)}, parent{n} {};
        
        // Explicit node copy constructor
        explicit node(const std::unique_ptr<node> &p, node* parent): 
                      key{p->get_key()}, value{p->get_value()}{
            this->parent = parent;
            if(p->right)
                right = std::make_unique<node>(p->right, this);
            if(p->left)
                left = std::make_unique<node>(p->left, this);
        }

        v_t& get_value() { return value;}
        k_t& get_key() { return key;}
        node* get_left() const {return left.get();}
        node* get_right() const {return right.get();}
        node* get_parent() const {return parent;}

        void set_left(node* x) { left.reset(x); }
        void set_right(node* x) { right.reset(x); }
        void set_parent(node* x) { parent = x; }

        // release smart pointers - return raw pointer
        node* release_right() {return right.release();}
        node* release_left() {return left.release();}

    };

////////////////////////////////////// END NODE //////////////////////////////////////

    using pair_type = std::pair<const k_t, v_t>;

    OP op;
    std::unique_ptr<node> head;

    void recursion(std::vector<pair_type> keys, size_t n); // to balance bst
    void recursion_draw(node* x, int space) noexcept; // to display bst

  public:

    // constructors
    bst(): op{OP()}, head{nullptr} {};
    bst(OP comp): op{comp}, head{nullptr} {};
    bst(k_t k, v_t v): op{OP()}, head{std::make_unique<node>(k,v)} {};
    bst(k_t k, v_t v, OP comp): op{comp}, head{std::make_unique<node>(k,v)} {};

    // forward declaration
    template <typename O>
    class _iterator;

    using iterator = _iterator<pair_type>;
    using const_iterator = _iterator<const pair_type>;

    std::pair<iterator, bool> insert(const pair_type& x);
    std::pair<iterator, bool> insert(pair_type&& x);

    template< class... Types >
    std::pair<iterator,bool> emplace(Types&&... args) {
        return insert(pair_type{std::forward<Types>(args)...});
    }

    void clear() noexcept { if(head) {head.reset();} };

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept { return iterator{nullptr}; }
    const_iterator end() const noexcept { return const_iterator{nullptr}; }
    const_iterator cend() const noexcept { return const_iterator{nullptr}; }

    iterator find(const k_t& x) noexcept;
    const_iterator find(const k_t& x) const noexcept;

    void balance();

    v_t& operator[](const k_t& x){
        auto it = find(x);
        if(it != end())
            return (*it);
        else {
            auto p = insert({x,v_t{}});
            return (*p.first);
        }
    }

    v_t& operator[](k_t&& x) {
        auto it = find(x);
        if(it != end())
            return (*it);
        else {
            auto p = insert({std::move(x),v_t{}});
            return (*p.first);
        }
    }

    friend
    std::ostream& operator<<(std::ostream& os, const bst& x){
        auto it = x.cbegin();
        while (it != x.cend()) {
            os << *it << " ";
            ++it;
        }
        os << std::endl;
        return os;
    }

    // copy semantic
    bst(const bst &b) : op{b.op} {
        if (b.head)
            head.reset(new node(b.head, nullptr));
    }
    
    bst& operator=(const bst& b) {
        op = b.op;
        head.reset();
        auto tmp = b;
        *this = std::move(tmp);
        return *this;
    }

    // move semantic
    bst(bst&& b) noexcept = default; // move constr 
    bst& operator=(bst&& b) noexcept = default; //move assignment

    void erase(const k_t& x);

    void draw() {
        if(head.get() != nullptr)
            recursion_draw(head.get(), 0);
    }

};

////////////////////////////////////// END BST //////////////////////////////////////

////////////////////////////////////// ITERATOR //////////////////////////////////////

template <typename k_t, typename v_t, typename OP>
template <typename O>
class bst<k_t,v_t,OP>::_iterator{
    using node = typename bst<k_t,v_t,OP>::node;
    node* current; 

    // private functions
    node* next() noexcept;

  public:
    using value_type = v_t;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    _iterator() noexcept: current{nullptr} {};
    explicit _iterator(node *p) noexcept : current{p} {}

    reference operator*() const noexcept { return current->get_value(); }
    pointer operator->() const { return &**this; }

    // pre-increment
    _iterator &operator++() noexcept {
        current = next();
        return *this;
    }

    friend
    bool operator==(const _iterator &a, const _iterator &b) {
        return a.current == b.current;
    }

    friend
    bool operator!=(const _iterator &a, const _iterator &b) { 
        return !(a == b); 
    }

    node* get_current() const {return current;}
    void set_current(node* x) { current = x;}
};

template <typename k_t, typename v_t, typename OP>
template <typename O>
typename bst<k_t,v_t,OP>::node* bst<k_t,v_t,OP>::_iterator<O>::next() noexcept {
    if(current->get_right() != nullptr) {
        current = current->get_right();
        while(current->get_left() != nullptr)
            current = current->get_left();
    } else {
        if(current->get_parent() == nullptr) // head with no right children
            return nullptr;
        while(current->get_parent()->get_left() != current){
                current = current->get_parent();
                if(current->get_parent() == nullptr)
                    return nullptr;
        }
        current = current->get_parent();
    }
    return current;
}

////////////////////////////////////// END ITERATOR //////////////////////////////////////

////////////////////////////////////// BST FUNCTIONS //////////////////////////////////////

template <typename k_t, typename v_t, typename OP>
typename bst<k_t,v_t,OP>::iterator bst<k_t,v_t,OP>::begin() noexcept {

    if(head == nullptr)
        return iterator(nullptr);
    
    auto it = iterator(head.get());
    while( it.get_current()->get_left() != nullptr)
        it.set_current(it.get_current()->get_left());
    
    return it;
}

template <typename k_t, typename v_t, typename OP>
typename bst<k_t,v_t,OP>::const_iterator bst<k_t,v_t,OP>::begin() const noexcept {

    if(head == nullptr)
        return const_iterator(nullptr);

    auto it = const_iterator(head.get());
    while( it.get_current()->get_left() != nullptr)
        it.set_current(it.get_current()->get_left());
    
    return it;
}

template <typename k_t, typename v_t, typename OP>
typename bst<k_t,v_t,OP>::const_iterator bst<k_t,v_t,OP>::cbegin() const noexcept{

    if(head == nullptr)
        return const_iterator(nullptr);

    auto it = const_iterator(head.get());
    while( it.get_current()->get_left() != nullptr)
        it.set_current(it.get_current()->get_left());
    
    return it;
}

template <typename k_t, typename v_t, typename OP>
typename bst<k_t,v_t,OP>::iterator bst<k_t,v_t,OP>::find(const k_t& x) noexcept{
    
    auto it = iterator(head.get());
    while(it.get_current() != nullptr ){
        auto node = it.get_current();
        if(op(node->get_key(),x))
            it.set_current(node->get_right()); 
        else if(op(x,node->get_key()))
            it.set_current(node->get_left()); 
        else
            return(iterator(it)); 
    }
    return end();
}

template <typename k_t, typename v_t, typename OP>
typename bst<k_t,v_t,OP>::const_iterator bst<k_t,v_t,OP>::find(const k_t& x) const noexcept{
    
    auto it = const_iterator(head.get());
    while(it.get_current() != nullptr ){
        auto node = it.get_current();
        if(op(node->get_key(),x))
            it.set_current(node->get_right()); 
        else if(op(x,node->get_key()))
            it.set_current(node->get_left()); 
        else
            return(const_iterator(it)); 
    }
    return cend();
}

template <typename k_t, typename v_t, typename OP>
std::pair<typename bst<k_t,v_t,OP>::iterator,bool> bst<k_t,v_t,OP>::insert(const pair_type& x){

    if (head == nullptr){
        head = std::make_unique<node>(x.first, x.second);
        return(std::make_pair(iterator(head.get()),true));
    }
    
    node* new_node = nullptr;
    auto tmp = head.get();
    
    while (tmp != nullptr){
        new_node = tmp;
        if (op(x.first,tmp->get_key()))
            tmp = tmp->get_left();
        else if (op(tmp->get_key(),x.first))
            tmp = tmp->get_right();
        else
             return(std::make_pair(iterator(tmp),false)); // if the key already exist
    }
    tmp = new node(x.first, x.second, new_node);
    if (op(x.first,new_node->get_key()))
        new_node->set_left(tmp); 
    else
        new_node->set_right(tmp);
    return(std::make_pair(iterator(tmp),true));
}

template <typename k_t, typename v_t, typename OP>
std::pair<typename bst<k_t,v_t,OP>::iterator,bool> bst<k_t,v_t,OP>::insert(pair_type&& x){

    if (head == nullptr){
        head = std::make_unique<node>(std::move(x.first), std::move(x.second));
        return(std::make_pair(iterator(head.get()),true));
    }
    
    node* new_node = nullptr;
    auto tmp = head.get();
    
    while (tmp != nullptr){
        new_node = tmp;
        if (op(x.first,tmp->get_key()))
            tmp = tmp->get_left();
        else if (op(tmp->get_key(),x.first))
            tmp = tmp->get_right();
        else
             return(std::make_pair(iterator(tmp),false)); // if the key already exist
    }
    tmp = new node(std::move(x.first), std::move(x.second), new_node);
    if (op(x.first,new_node->get_key()))
        new_node->set_left(tmp); 
    else
        new_node->set_right(tmp);
    return(std::make_pair(iterator(tmp),true));
}

template <typename k_t, typename v_t, typename OP>
void bst<k_t,v_t,OP>::balance() {

    //copying ordered values in a vector 
    std::vector<pair_type> vec;
    auto it = this->cbegin();
    while(it != cend()){
        vec.push_back(std::make_pair(it.get_current()->get_key(), it.get_current()->get_value()));
        ++it;
    }
    clear(); //clear the bst
    recursion(vec, vec.size()); //build the balanced bst
}

template <typename k_t, typename v_t, typename OP>
void bst<k_t,v_t,OP>::recursion(std::vector<pair_type> vec, size_t n) {
    if(n==1) {
        insert({vec[0].first,vec[0].second});
    } else if(n==2) {
        insert({vec[0].first,vec[0].second});
        insert({vec[1].first,vec[1].second});
    } else{
        int middle = (n % 2 == 0) ? (n/2)-1 : floor(n/2);
        insert({vec[middle].first,vec[middle].second});

        //create 2 sub-vectors
        auto first_left = vec.begin();
        auto last_left = vec.begin() + middle;
        auto first_right = vec.begin() + middle + 1;
        auto last_right = vec.end();
        std::vector<pair_type> left_values(first_left,last_left);
        std::vector<pair_type> right_values(first_right,last_right);
        recursion(left_values, left_values.size());
        recursion(right_values, right_values.size());
    }
}

template <typename k_t, typename v_t, typename OP>
void bst<k_t,v_t,OP>::erase(const k_t& x){

    iterator p = find(x);

    if (p != end()){ // if key exists
        auto current = p.get_current();
        auto left = current->get_left();
        auto right = current->get_right();
        auto parent = current->get_parent();
           
        if(left && right) { // node with two children
            ++p;
            auto _current = p.get_current();
            auto _parent = _current->get_parent();
            auto _right = _current->get_right();
            auto tmp = current->release_left();
            _current->set_left(tmp);
            tmp->set_parent(_current);

            if(_parent == current) {
                auto tmp_next = _parent->release_right();
                if(head.get() == current) { 
                    auto del_root = head.release();
                    head.reset(tmp_next);
                    delete del_root;
                } else if(parent->get_left() == current) {
                    auto tmp_current = parent->release_left();
                    parent->set_left(tmp_next);
                    delete tmp_current;
                } else {
                    auto tmp_current = parent->release_right();
                    parent->set_right(tmp_next);
                    delete tmp_current;
                }
                tmp_next->set_parent(parent);
            } else {
                auto tmp_next = _parent->release_left();
                if(_right) {
                    auto tmp_right = _current->release_right();
                    _parent->set_left(tmp_right);
                    tmp_right->set_parent(_parent);
                }

                auto tmp_parent = current->release_right();
                tmp_next->set_right(tmp_parent);
                tmp_parent->set_parent(tmp_next);
                
                if(head.get() == current) {
                    auto del_root = head.release();
                    head.reset(tmp_next);
                    delete del_root;
                } else if (parent->get_left() == current) {
                    auto tmp_current = parent->release_left();
                    parent->set_left(tmp_next);
                    delete tmp_current;
                } else {
                    auto tmp_current = parent->release_right();
                    parent->set_right(tmp_next);
                    delete tmp_current;    
                }
                tmp_next->set_parent(parent);  
            }
        } else if(!left && !right) { // leaf node
             
            if(head.get() == current) {
                auto del_root = head.release();
                head.reset(nullptr);
                delete del_root;
            }
            else if(parent->get_left() == current)
                parent->set_left(nullptr);
            else
                parent->set_right(nullptr);
        } else if(left && !right) { // node with only left child
            
            left->set_parent(parent);
            if(head.get() == current) {
                auto new_root = current->release_left();
                auto del_root = head.release();
                head.reset(new_root);
                delete del_root;
            } else if(parent->get_left() == current) {
                auto tmp = parent->release_left();
                tmp->release_left();
                parent->set_left(left);  
                delete tmp;
            } else {
                auto tmp = parent->release_right();
                tmp->release_left();
                parent->set_right(left);
                delete tmp;
            }      
        } else { // node with only right child 
            right->set_parent(parent);
            if(head.get() == current) {
                auto new_root = current->release_right();
                auto del_root = head.release();
                head.reset(new_root);
                delete del_root;
            } else if(parent->get_left() == current) {
                auto tmp = parent->release_left();
                tmp->release_right();
                parent->set_left(right);
                delete tmp;
            } else {
                auto tmp = parent->release_right();
                parent->set_right(right);
                tmp->release_right(); 
                delete tmp;
            }
        }
    }
}

template <typename k_t, typename v_t, typename OP>
void bst<k_t,v_t,OP>::recursion_draw(node* x, int space) noexcept {

    if (x == nullptr) 
        return;
    
    // Increase distance between levels 
    space += COUNT;

    recursion_draw(x->get_right(), space); 

    std::cout << std::endl; 
    for (int i = COUNT; i < space; i++) 
        std::cout << " ";
    std::cout << x->get_value() << "\n"; 

    recursion_draw(x->get_left(), space); 
    
}

////////////////////////////////////// END BST FUNCTIONS //////////////////////////////////////