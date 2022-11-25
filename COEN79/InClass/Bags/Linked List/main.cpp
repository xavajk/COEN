#include "node.h"

int main(int argc, char const *argv[]) {
    node* ptr = new node;
    ptr->set_data(...);
    ptr->set_link(...);
    // alternatively,
    ptr->data() = ...; // this format allows to get and set with single operation
    ptr->link() = ...;
    return 0;
}
