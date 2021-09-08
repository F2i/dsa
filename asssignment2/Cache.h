#ifndef CACHE_H
#define CACHE_H

#include "main.h"
class Node {
   public:
    int key;
    Node *left;
    Node *right;
    int height;
    Elem* cont;
    ~Node(){delete cont;}
};

class AVL
{
    public:
    Node * root;
    AVL(){
        this->root = NULL;
    }
    ~AVL(){
        this->emptyTree(this->root);
    }
    int height(Node *N);
    int max(int a, int b);
    Node *newNode(int key, Elem* cont);
    Node *rightRotate(Node *y);
    Node *leftRotate(Node *x);
    int getBalanceFactor(Node *N);
    Node *insertNode(Node *node, int key, Elem* cont);
    Node *nodeWithMimumValue(Node *node);
    Node *deleteNode(Node *root, int key);
    Node* search(Node* root, int key);
    void printTree(Node *root, string indent, bool last);
    void printPreO(Node *root);
    void printInO(Node *root);
    void emptyTree(Node *root);
};

// Circular Queue implementation in C++
class Queue {
  private:
  int front, rear;
  Elem **arr;
  int maxsize;
  
  public:
  Queue(int s) {
    front = -1;
    rear = -1;
	arr = new Elem*[s];
    maxsize = s;
  }
  ~Queue(){
      delete[] arr;
  }
  bool isFull();
  bool isEmpty();
  void enQueue(Elem * element);
  Elem * deQueue();
  void display();
  //Elem * front();
  Elem * Erear();
};

class Cache {
		int p;
	public:
        AVL * tree;
        Queue * cqueue;
		Cache(int s) {
			p = 0;
            tree = new AVL;
            cqueue = new Queue(s);
		}
		~Cache() {
            delete tree;
            delete cqueue;
		}
		Data* read(int addr);
		Elem* put(int addr, Data* cont, bool bsync);
		Elem* write(int addr, Data* cont);
		void print();
		void preOrder();
		void inOrder();	
};


#endif