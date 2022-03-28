#include "main.h"
//-----------------avl---------------------------//
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
Node* replaceNode(Node* node){
	Node* current = node->right;
    while(current->left != NULL){
		current = current->left;
    }
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
Node* AVL::insertRec(Node* node, Elem* e){
	if(node == NULL) // node is null
		return(new Node(e->addr, e->data, e->sync));

	if(e->addr < node->element->addr)
		node->left = insertRec(node->left, e);
	else if(e->addr > node->element->addr)
		node->right = insertRec(node->right, e);
	else // Equal keys not allowed
		return node;

	node->height = 1 + max(height(node->left), height(node->right)); // update height

	int balance = getBalance(node);
	if(balance > 1 && e->addr < node->left->element->addr)// Left Left Case
		return rightRotate(node);
    if(balance < -1 && e->addr > node->right->element->addr)// Right Right Case
		return leftRotate(node);
    if(balance > 1 && e->addr > node->left->element->addr){// Left Right Case
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}
	if(balance < -1 && e->addr < node->right->element->addr){// Right Left Case
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
void AVL::replaceRec(Node* node, int element, Data* cont){
    if(node == NULL)
        return;

    if(element > node->element->addr)
        return replaceRec(node->right, element, cont);
    else if(element < node->element->addr)
        return replaceRec(node->left, element, cont);
    else{
        node->element->data = cont;
        node->element->sync = false;
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
void AVL::insert(Elem* e){
    this->root = insertRec(this->root, e);
}
void AVL::deleteNode(int element){
    this->root = deleteNodeRec(this->root, element);
}
Elem* AVL::search(int element){
    Node* tmp = searchRec(root, element);
    if(tmp == NULL)
        return NULL;
    else
        return tmp->element;
}
void AVL::print(){
    cout << "Print AVL in inorder:\n";
    printInOrderRec(this->root);
    cout << "Print AVL in preorder:\n";
    printPreOrderRec(this->root);
}
void AVL::replace(int addr, Data* cont){
    this->replaceRec(this->root,addr,cont);
}
//-----------------------------------------------------//
//---------------------DBHash--------------------------//
void DBHashing::insert(Elem* e){
    int i = 0;
    while(i < this->size){
        int slot = hp(e->addr, i);
        if(this->status[slot] == NIL || this->status[slot] == DELETED){
            this->hashTable[slot] = e;
            this->status[slot] = NON_EMPTY;
            break;
        } else i++;
    }
}
void DBHashing::deleteNode(int key){
    int i = 0;
    int j;
    do{
        j = hp(key, i);
        if(this->status[j] == NON_EMPTY && this->hashTable[j]->addr == key){
            this->status[j] = DELETED;
            break;
        }
        i++;
    } while (this->status[j] != NIL && i < this->size);
}
Elem* DBHashing::search(int key) {
    int i = 0;
    int j;
    do{
        j = hp(key, i);
        if(this->status[j] == NON_EMPTY && this->hashTable[j]->addr == key)
            return hashTable[j];
        i++;
    } while (this->status[j] != NIL && i < this->size);

    return NULL;
}
void DBHashing::replace(int addr, Data* cont){
    int i = 0;
    int j;
    do{
        j = hp(addr, i);
        if(this->status[j] == NON_EMPTY && this->hashTable[j]->addr == addr){
            hashTable[j]->data = cont;
            hashTable[j]->sync = false;
            break;
        }
        i++;
    } while (this->status[j] != NIL && i < this->size);
}
void DBHashing::print(){
    cout << "Prime memory:\n";
    for(int i = 0; i < this->size; i++)
        if(this->status[i] == NON_EMPTY)
            this->hashTable[i]->print();
}
//-----------------------------------------------------//
//---------------------FIFO---------------------------//
void FIFO::insert(Elem* e){
    arr[this->count] = e;
    this->count++;
}
Elem* FIFO::remove(){
    Elem* valRemoved = arr[0];
    for(int i = 1; i < this->count; i++)
        arr[i-1] = this->arr[i];
    this->count--;
    return valRemoved;
}
void FIFO::search(int addr){
    return;
}
void FIFO::replace(int addr, Data* cont){
    for(int i = 0; i < this->count; i++){
        if(arr[i]->addr == addr){
            arr[i]->data = cont;
            arr[i]->sync = false;
            break;
        }
    }
}
void FIFO::print(){
    for(int i = 0; i < this->count; i++)
        this->arr[i]->print();
}
//-----------------------------------------------------//
//-----------------------MRU-------------------------//
void MRU::insert(Elem* e){
    this->count ++;
    for(int i = count - 1; i > 0; i--){
        this->arr[i] = this->arr[i-1];
    }
    this->arr[0] = e;
}
Elem* MRU::remove(){
    Elem* valRemoved = arr[0];
    for(int i = 1; i < this->count; i++)
        arr[i-1] = this->arr[i];
    this->count--;
    return valRemoved;
}
void MRU::search(int addr){
    for(int i = 0; i < this->count; i++){
        if(arr[i]->addr == addr){
            Elem* tmp = arr[i];
            for(int j = i+1; j < this->count; j++)
                arr[j-1] = arr[j];
            this->count--;
            this->insert(tmp);
            break;
        }
    }
}
void MRU::print(){
    for(int i = 0; i < this->count; i++){
        this->arr[i]->print();
    }
}
void MRU::replace(int addr, Data* cont){
     for(int i = 0; i < this->count; i++){
        if(arr[i]->addr == addr){
            arr[i]->data = cont;
            arr[i]->sync = false;
            break;
        }
    }
}
//-----------------------------------------------------//
//---------------------LRU---------------------------//
Elem* LRU::remove(){
    this->count--;
    return this->arr[count];
}
//-----------------------------------------------------//
//---------------------LFU---------------------------//
void LFU::reHeapDown(int position){
    int lIndex = position*2 + 1;
    int rIndex = position*2 + 2;
    int childIndex;
    if(lIndex < this->count){
        if(rIndex < this->count && this->list[rIndex].first < this->list[lIndex].first){
            childIndex = rIndex;
        } else{
            childIndex = lIndex;
        }
        if(this->list[position].first >= this->list[childIndex].first){
            val tmp = this->list[childIndex];
            this->list[childIndex] = this->list[position];
            this->list[position] = tmp;

            this->reHeapDown(childIndex);
        }
    }
    else return;
}
void LFU::reHeapUp(int position){
    if(position > 0){
        int pIndex = (position-1)/2;
        if(this->list[position].first < this->list[pIndex].first){
    
            val tmp = this->list[pIndex];
            this->list[pIndex] = this->list[position];
            this->list[position] = tmp;
    
            this->reHeapUp(pIndex);
        }
    }
    else return;
}
void LFU::insert(Elem* e){
    this->count++;
    list[this->count-1].first = 1;
    list[this->count-1].second = e;
    this->reHeapUp(this->count-1);
}
Elem* LFU::remove(){
    Elem* tmp = list[0].second;
    list[0] = list[this->count-1];
    this->count--;
    reHeapDown(0);
    return tmp;
} 
void LFU::search(int addr){
    for(int i = 0; i < this->count; i++){
        if(list[i].second->addr == addr){
            list[i].first++;
            this->reHeapDown(i);
            break;
        }
    }
}
void LFU::print(){
    for(int i = 0; i < this->count; i++){
        list[i].second->print();
    }
}
void LFU::replace(int addr, Data* cont){
    for(int i = 0; i < this->count; i++){
        if(list[i].second->addr == addr){
            list[i].second->data = cont;
            list[i].second->sync = false;
            break;
        }
    }
}
//-----------------------------------------------------//

//----------------------------Cache--------------------------//
Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s){}
Cache::~Cache(){
	delete rp;
	delete s_engine;
}
Data* Cache::read(int addr) {
    Elem* tmp = this->s_engine->search(addr);
    if(tmp == NULL)
        return NULL;
    else{
        this->rp->search(addr);
        return tmp->data;
    }
}
Elem* Cache::put(int addr, Data* cont) {
    Elem* e = new Elem(addr, cont, true);
    if(!rp->isFull()){
        this->rp->insert(e);
        this->s_engine->insert(e);
        return NULL;
    }
    else{
        Elem* tmp = this->rp->remove();
        this->s_engine->deleteNode(tmp->addr);
        this->s_engine->insert(e);
        this->rp->insert(e);
        return tmp;
    }
}
Elem* Cache::write(int addr, Data* cont) {
    Elem* tmp = this->s_engine->search(addr);
    this->rp->search(addr);
    if(tmp != NULL){
        this->rp->replace(addr, cont);
        this->s_engine->replace(addr, cont);
        return tmp;
    }
    else{
        Elem* e = new Elem(addr, cont, false);
        if(!rp->isFull()){
            this->rp->insert(e);
            this->s_engine->insert(e);
            return NULL;
        }
        else{
            Elem* e_out = this->rp->remove();
            this->s_engine->deleteNode(e_out->addr);
            this->s_engine->insert(e);
            this->rp->insert(e);
            return e_out;
        }
    }
}
void Cache::printRP() {
    this->rp->print();
}
void Cache::printSE() {
    this->s_engine->print();
}
//-------------------------------------end-------------------------------//
