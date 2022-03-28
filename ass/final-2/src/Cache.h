#ifndef CACHE_H
#define CACHE_H

#include "main.h"

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
class AVL{
    Node* root;
    protected:
        Node* rightRotate(Node *node);
        Node* leftRotate(Node *node);
        Node* insertRec(Node* node, int addr, Data* cont, bool sync);
        Node* deleteNodeRec(Node* node, int key);
		Node* searchRec(Node* node, int element);
		Node* searchandreplaceRec(Node* node, int element, Data* cont);
		void printPreOrderRec(Node* node);
		void printInOrderRec(Node* node);
    public:
        AVL(){this->root = NULL;}
        ~AVL(){
			DestroyRecursive(this->root);
			this->root = NULL;
		}
        void insertNode(int addr, Data* cont, bool sync){
            this->root = insertRec(this->root, addr, cont, sync);
        }
        void deleteNode(int element){
            this->root = deleteNodeRec(this->root, element);
        }
		Elem* searchNode(int element){
			Node* tmp = searchRec(root, element);
			if(tmp == NULL)
				return NULL;
			else
				return tmp->element;
		}
		Elem* searchandreplaceNode(int element, Data* cont){
			Node* tmp = searchandreplaceRec(root, element, cont);
			if(tmp == NULL)
				return NULL;
			else
				return tmp->element;
		}
		void printPreOrder(){
			printPreOrderRec(root);
		}
		void printInOrder(){
			printInOrderRec(root);
		}
		void DestroyRecursive(Node* node){
			if (node){
				DestroyRecursive(node->left);
				DestroyRecursive(node->right);
				delete node;
			}
		}
};
class Cache {
		AVL tree;
		queue<int> q;
	public:
		Cache(int size){}
		~Cache() {
			tree.~AVL();
			while(!q.empty())
				q.pop();
		}
		bool fullQueue();
		Data* read(int addr);
		Elem* put(int addr, Data* cont);
		Elem* write(int addr, Data* cont);
		void print();
		void preOrder();
		void inOrder();	
};
#endif