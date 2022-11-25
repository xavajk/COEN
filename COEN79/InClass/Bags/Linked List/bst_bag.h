#include "btn.h"

class IntSmartPointer {
    public:
        int operator*(int* ptr) {
            return *ptr;
        }

        int* operator->(int* ptr) {
            return ptr;
        }

        int* operator++(int) {
            return ptr++;
        }

        int* operator++() {
            return ++ptr;
        }

        int* operator--(int) {
            return ptr--;
        }

        int* operator--() {
            return --ptr;
        }

    private:
        int* p;
};

template <class Item>
class bst_bag {
    public:
        bst_bag() { root = NULL; }
        bst_bag(const bst_bag &src) { tree_copy(root); } 
        ~bst_bag() { tree_clear(root); }
        
        // *** exam question
        size_t count(const Item &target) {
            btn<Item> *cursor = root;
            size_t cnt = 0;

            while (cursor != NULL) {
                if (target == cursor->data()) ++cnt;
                if (target <= cursor->data()) cursor = cursor->left();
                else cursor = cursor->right();
            }
            return cnt;
        }

        

        // *** exam question
        void insert (const Item &entry) {
            if (root == NULL) {
                root = new btn<Item>(entry);
                return;
            }
            else {
                btn<Item> *cursor = root;
                while (cursor != NULL) {
                    if (entry <= cursor->data()) {
                        if (cursor->left() == NULL) {
                            cursor->left() = new btn<Item>(entry);
                            return;
                        }
                        cursor = cursor->left();
                    }
                    else {
                        if (cursor->right() == NULL) {
                            cursor->right() = new btn<Item>(entry);
                            return;
                        }
                        cursor = cursor->right();
                    }
                }
            }
        }

        // *** exam question
        bool eraseOne (const Item &target) {
            return bstRemove(root, target);
        }

        bool eraseAll (const Item &target) {

        }

        // *** exam question
        bool bstRemove (btn<Item> *root, const Item &target) {
            // check if list is empty
            if (root == NULL) return false;

            // search for target recursively
            if (target < root->data()) return bstRemove(root->left(), target);
            if (target > root->data()) return bstRemove(root->right(), target);

            // target == root->data()
            // left subtree is NULL
            if (root->left == NULL) {
                btn<Item> *tmp = root;
                root = root->right();
                delete tmp;
                return true;
            }
            // else replace target with largest element of left subtree 
            else {
                bstRemoveMax(root->left(), root->data());
                return true;
            }

        // *** exam question
        void bstRemoveMax(btn<Item> *&root, Item &target) {
            if (root->right() == NULL) {
                btn<Item> *tmp = root;
                target = root->data();
                root = root->left();
                delete tmp;
            }
            else bstRemoveMax(root->right(), target);
        }

        void insertAll(btn<Item> *root) {
            if (root != NULL) {
                insert(root->data());
                insertAll(root->left());
                insertAll(root->right());
            }
        }

        // must be careful for self-assignment and infinite loop
        void operator+=(const bst_bag &src) {
            if (root == src.root) {
                bst_bag<Item> copy = src;
                insertAll(copy.root);
            } else {
                insertAll(src.root);
            }
        }

        // *** know imlementation of right rotation
        void rightRotate(btn<Item> *&root) {
            btn<Item> *tmp = root->left();
            root->left() = tmp->right();
            tmp->right() = root;
            root = tmp;
        }

        void leftRotate(btn<Item> *&root) {
            btn<Item> *tmp = root->right();
            root->right() = tmp->left();
            tmp->left() = root;
            root = tmp;
        }
    
    private:
        btn<Item> *root;
};

binary_tree_node<Item>* left_rotation (binary_tree_node<Item>*& parent) 
binary_tree_node<Item>* right_rotation (binary_tree_node<Item>*& parent) 
binary_tree_node<Item>* left_right_rotation (binary_tree_node<Item>*& parent) 
binary_tree_node<Item>* right_left_rotation (binary_tree_node<Item>*& parent) 

template <class Item>  
binary_tree_node<Item>* balance(binary_tree_node <Item>*& temp) 
{
    int temp_bf = diff(temp);
    if (height(temp) <= 2) return temp;
    if (temp_bf > -2 && temp_bf < 2) {
        balance(temp->left());
        balance(temp->right());
    }

    else 
    {
        // temp balance factor >= 2
        if (temp_bf >= 2)
        {
            if (diff(temp->left()->right() == nullptr)) right_rotation(temp);
            else left_right_rotation(temp);
        }

        // temp balance factor <= -2
        if (temp_bf <= -2) {
            if (diff(temp->right()->left() == nullptr)) left_rotation(temp);
            else right_left_rotation(temp);
        }
    }
    return temp;
}

template <class Item>  
void flip(binary_tree_node<Item>* root_ptr) {
    if (root_ptr->left() == NULL && root_ptr->right() == NULL) return;
    else 
    {
        binary_tree_node<Item>* temp = root_ptr->left();
        root_ptr->left() = root_ptr->right();
        root_ptr->right() = temp;
        flip(root_ptr->left());
        flip(root_prt->right());
    }
}