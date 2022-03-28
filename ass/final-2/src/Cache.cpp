#include "main.h"

int max(int a, int b){
	return (a > b) ? a : b;
}
int height(Node *node){
	if(node == NULL) return 0;
	return node->height;
}
int getBalance(Node *node){
	if(node == NULL) return 0;
	return height(node->left) - height(node->right);
}
Node * replaceNode(Node* node){
	Node* current = node->right;
	while(current->right != NULL)
		current = current->left;
	return current;
}

Node* AVL::rightRotate(Node *node){
	Node* leftStree = node->left;
	Node* tmp = leftStree->right;
    //rotating
    node->left = tmp;
	leftStree->right = node;
    //update height
	node->height = max(height(node->left), height(node->right)) + 1;
	leftStree->height = max(height(leftStree->left), height(leftStree->right)) + 1;

	return leftStree;
}
Node* AVL::leftRotate(Node *node){
	Node *rightStree = node->right;
	Node *tmp = rightStree->left;
    //rotating
	node->right = tmp;
	rightStree->left = node;
    //update height
	node->height = max(height(node->left), height(node->right)) + 1;
	rightStree->height = max(height(rightStree->left), height(rightStree->right)) + 1;

	return rightStree;
}

Node* AVL::insertRec(Node* node, int addr, Data* cont, bool sync){
	if(node == NULL) // node is null
		return(new Node(addr, cont, sync));

	if(addr < node->element->addr)
		node->left = insertRec(node->left, addr, cont, sync);
	else if(addr > node->element->addr)
		node->right = insertRec(node->right, addr, cont, sync);
	else // Equal keys not allowed
		return node;

	node->height = 1 + max(height(node->left), height(node->right)); // update height

	int balance = getBalance(node);
	if(balance > 1 && addr < node->left->element->addr)// Left Left Case
		return rightRotate(node);
    if(balance < -1 && addr > node->right->element->addr)// Right Right Case
		return leftRotate(node);
    if(balance > 1 && addr > node->left->element->addr){// Left Right Case
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}
	if(balance < -1 && addr < node->right->element->addr){// Right Left Case
	    node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

Node* AVL::deleteNodeRec(Node* node, int key){
    if(node == NULL)
        return node;

    if(key < node->element->addr)
        node->left = deleteNodeRec(node->left, key);
    else if(key > node->element->addr)
        node->right = deleteNodeRec(node->right, key);
    else{
        if((node->left == NULL) || (node->right == NULL)){//one or no child
            Node *temp = node->left ? node->left : node->right;
            if (temp == NULL){//no child
                temp = node;
                node = NULL;
            } else // One child case
                *node = *temp; // Copy the contents of the non-empty child
            delete temp;
        } else{ // node with two children
            Node* temp = replaceNode(node);
            node->element = temp->element;
            node->right = deleteNodeRec(node->right, temp->element->addr);
        }
    }
    if (node == NULL) return node; // delete in leaf
    //update height
    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->left) >= 0)// Left Left Case
        return rightRotate(node);
    if (balance > 1 && getBalance(node->left) < 0){ // Left Right Case
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0)// Right Right Case
        return leftRotate(node);
    if (balance < -1 && getBalance(node->right) > 0){ // Right Left Case
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* AVL::searchRec(Node* node, int element){
    if(node == NULL)
        return NULL;

    if(element > node->element->addr)
        return searchRec(node->right, element);
    else if(element < node->element->addr)
        return searchRec(node->left, element);
    else
        return node;
}
Node* AVL::searchandreplaceRec(Node* node, int element, Data* cont){
    if(node == NULL)
        return NULL;

    if(element > node->element->addr)
        return searchandreplaceRec(node->right, element, cont);
    else if(element < node->element->addr)
        return searchandreplaceRec(node->left, element, cont);
    else{
        node->element->data = cont;
        node->element->sync = false;
        return node;
    }
}

void AVL::printPreOrderRec(Node* node){
    if(node != NULL) 
    { 
        node->element->print();
        printPreOrderRec(node->left); 
        printPreOrderRec(node->right); 
    } 
}
void AVL::printInOrderRec(Node* node){
    if(node != NULL) 
    { 
        printInOrderRec(node->left);
        node->element->print();
        printInOrderRec(node->right); 
    } 
}

bool Cache::fullQueue(){
    if((int)this->q.size() == MAXSIZE)
        return true;
    else
        return false;
}
Data* Cache::read(int addr) {
    Elem* tmp = this->tree.searchNode(addr);
    if(tmp == NULL)
        return NULL;
    else
        return tmp->data;
}
Elem* Cache::put(int addr, Data* cont) {
	if(!fullQueue()){
        this->q.push(addr);
        this->tree.insertNode(addr, cont, true);
        return NULL;
    }
    else{
        int tmp = this->q.front();
        this->q.pop();
        Elem* elem = this->tree.searchNode(tmp);
        this->tree.deleteNode(tmp);
        this->q.push(addr);
        this->tree.insertNode(addr, cont, true);
        return elem;
    }
}
Elem* Cache::write(int addr, Data* cont) {
    Elem* tmp = this->tree.searchandreplaceNode(addr, cont);
    if(tmp == NULL){
        if(!fullQueue()){
            this->q.push(addr);
            this->tree.insertNode(addr, cont, false);
            return NULL;
        }
        else{
            int tmp = this->q.front();
            this->q.pop();
            Elem* elem = this->tree.searchNode(tmp);
            this->tree.deleteNode(tmp);
            this->q.push(addr);
            this->tree.insertNode(addr, cont, false);
            return elem;
        }
    }
    return tmp;
}
void Cache::print() {
    int size = this->q.size();
	for(int i = 0; i < size; i++){
        int tmp = this->q.front();
        this->q.pop();
        this->q.push(tmp);
        this->tree.searchNode(tmp)->print();
    }
}
void Cache::preOrder() {
	this->tree.printPreOrder();
}
void Cache::inOrder() {
    this->tree.printInOrder();
}
