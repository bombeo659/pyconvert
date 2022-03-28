#ifndef CACHE_H
#define CACHE_H

#include "main.h"
//-------------------------------------------------//
class Node
{
	public:
	Elem* element;
	Node* left;
	Node* right;
	int height;
    Node(int a, Data* d, bool s){
		element = new Elem(a, d, s);
		left = NULL;
		right = NULL;
		height = 1;
	}
};
enum STATUS_TYPE {NIL, NON_EMPTY, DELETED};
typedef pair<int, Elem*> val;
//------------------------------------------------//
class ReplacementPolicy {
	protected:
	int count;
	Elem** arr;
	
	public:
	virtual ~ReplacementPolicy(){delete [] this->arr;}
	virtual void insert(Elem* e) = 0;
	virtual Elem* remove() = 0;
	virtual void search(int addr) = 0;
	virtual void replace(int addr, Data* cont) = 0;
	virtual void print() = 0;

	bool isFull() {return count == MAXSIZE;}
};

class FIFO : public ReplacementPolicy {
	public:
	FIFO(){count = 0; arr = new Elem*[MAXSIZE];}
	~FIFO(){delete [] this->arr;}

	void insert(Elem* e);
	Elem* remove();
	void search(int addr);
	void replace(int addr, Data* cont);
	void print();
};

class MRU : public ReplacementPolicy {
	public:
	MRU(){this->count = 0; this->arr = new Elem*[MAXSIZE];}
	~MRU(){delete [] this->arr;}

	void insert(Elem* e);
	virtual Elem* remove();
	virtual void search(int addr);
	void print();
	void replace(int addr, Data* cont);
};
class LRU: public MRU {
	public:
	LRU(){this->count = 0; this->arr = new Elem*[MAXSIZE];}
	~LRU(){delete [] this->arr;}

	Elem* remove() override;
};

class LFU: public ReplacementPolicy {
	private:
	val* list;
	protected:
	void reHeapUp(int position);
	void reHeapDown(int position);
	public:
	LFU(){
		this->count = 0;
		this->list = new val[MAXSIZE];
		this->arr = new Elem*[MAXSIZE];
		for(int i = 0; i < this->count; i++){
			list[i].first = 0;
			list[i].second = NULL;
		}
	}
	~LFU(){delete [] this->arr; delete [] this->list;}

	void insert(Elem* e);
	Elem* remove();
	void search(int addr);
	void print();
	void replace(int addr, Data* cont);
};

//-------------------------------------------------------//
class SearchEngine {
	public:
	virtual ~SearchEngine(){}
	virtual Elem* search(int key) = 0; // -1 if not found
	virtual void insert(Elem* e) = 0;
	virtual	void deleteNode(int key) = 0;
	virtual void print() = 0;
	virtual void replace(int addr, Data* cont) = 0;
};

class DBHashing: public SearchEngine {
	private:
		int (*h1)(int);
		int (*h2)(int);
		STATUS_TYPE* status;
		Elem** hashTable;
		int size;
	public:
	DBHashing(int (*h1)(int),int (*h2)(int),int s){
		this->h1 = h1;
		this->h2 = h2;
		this->hashTable = new Elem*[s];
		this->status = new STATUS_TYPE[s];
		this->size = s;
		for (int i = 0; i < s; i++)
            this->status[i] = NIL;
	}
	~DBHashing(){
		delete [] status;
		delete [] hashTable;
	}
	int hp(int key, int i){
		return (h1(key) + i*h2(key))%(this->size);
	}
	void insert(Elem* e);
	void deleteNode(int key);
	void print();
	Elem* search(int key);
	void replace(int addr, Data* cont);
};

class AVL : public SearchEngine {
	Node* root;
    protected:
        Node* rightRotate(Node *node);
        Node* leftRotate(Node *node);
        Node* insertRec(Node* node, Elem* e);
        Node* deleteNodeRec(Node* node, int key);
		Node* searchRec(Node* node, int element);
		void replaceRec(Node* node, int element, Data* cont);
		void printPreOrderRec(Node* node);
		void printInOrderRec(Node* node);
	public:
		AVL(){this->root = NULL;}
        ~AVL(){DestroyRecursive(this->root);}
		
		void insert(Elem* e);
        void deleteNode(int element);
		Elem* search(int element);
		void replace(int addr, Data* cont);
		void print();

		void DestroyRecursive(Node* node){
			if (node){
				DestroyRecursive(node->left);
				DestroyRecursive(node->right);
				delete node;
			}
		}	
};

#endif