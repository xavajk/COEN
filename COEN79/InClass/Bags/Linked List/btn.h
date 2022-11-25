/* BINARY TREE NODE CLASS */
#include <cstdlib>
#include <iostream>

template <class Item>
class btn 
{
    public:
        btn(const Item& init_data = Item(), btn *init_left = NULL, btn *init_right = NULL)
        {
            data_field = init_data;
            left_field = init_left;
            right_field = init_right;
        }

        // GETTERS
        Item& data() { return data_field; }
        const Item& data() const { return data_field; }
        btn* left() { return left_field; }
        const btn* left() const { return left_field; }
        btn* right() { return right_field; }
        const btn* right() const { return right_field; }

        bool is_leaf() { return (left_field == NULL) && (right_field == NULL); }

    private:
        Item data_field;
        btn *left_field;
        btn *right_field;
};

// NON-MEMBER TOOLKIT FUNCTIONS
template <class Item>
// exam question: how many times is tree_clear called recursively?
// answer: twice for each node excluding the initial function call (1 for each pointer)
void tree_clear(btn<Item> *&root_ptr)
{
    btn<Item> *child;
    if (root_ptr != NULL) 
    {
        child = root_ptr->left();
        tree_clear(child);
        child = root_ptr->right();
        tree_clear(child);
        delete root_ptr;
        root_ptr = NULL;
    }
}

template <class Item>
btn<Item>* tree_copy(const btn<Item> *&root_ptr)
{
    if (root_ptr == NULL) return NULL;
    btn<Item> *tmp_left = tree_copy(root_ptr->left());
    btn<Item> *tmp_right = tree_copy(root_ptr->right());
    return new btn<Item> (root_ptr->data(), tmp_left, tmp_right);
}

treeClear(root){
    treeClear(root->left());
    treeClear(root->right());
    delete root;
    root = NULL;
}

template <class Item>
treeCopy(){
    btn<Item> *tmp_left = treeCopy(root->left());
    btn<Item> *tmp_right = treeCopy(root->right());
    return new btn<Item>(root->data(), tmp_left, tmp_right);
}

template <class Item>
// print tree via preorder traversal
void tree_po_print(const btn<Item> *&root_ptr)
{
    if (root_ptr != NULL)
    {
        std::cout << root_ptr->data();
        tree_po_print(root_ptr->left());
        tree_po_print(root_ptr->right());
    }
}

template <class FType, class Item>
// preorder traversal function that can perform "FType func" on "Item"
void tree_preorder(FType func, btn<Item> *&root_ptr)
{
    if (root_ptr != NULL)
    {
        func(root_ptr);
        preorder(func, root_ptr->left());
        preorder(func, root_ptr->right());
    }
}

btn<Item> *tmp;
tmp = parent->left();
parent->left() = parent->right();

int height(btn* tmp){
    int h = 0;
    if (tmp != NULL) {
        int l_height = height(tmp->left());
        int r_height = height(tmp->right());
        int max = std::max(l_height, r_height);
        h = max + 1;
    }
    return h;
}