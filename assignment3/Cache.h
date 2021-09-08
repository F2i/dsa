#ifndef CACHE_H
#define CACHE_H

#include "main.h"


class ReplacementPolicy {
	protected:
	int count;
	Elem** arr;
	
	public:
	virtual Elem* insert(Elem* e,int idx) = 0;//insert e into arr[idx] if idx != -1 else into the position by replacement policy
	virtual void access(int idx) = 0;//idx is index in the cache of the accessed element 
	virtual int remove() = 0; 
	virtual void print() = 0;
	bool forHeap = false;
	
	bool isFull() {return count == MAXSIZE;}
	bool isEmpty() {return count == 0;}
	Elem* getValue(int idx){return arr[idx];}
	void replace(int idx,Elem* e) {
		delete arr[idx];
		access(idx);
		arr[idx] = e;
	}

	int index(int xaddr){
		forHeap = true;
		for(int i = 0; i < MAXSIZE ; i++)	{
			if(arr[i]->addr == xaddr) return i;
		}
		return 0;
	}
};

class SearchEngine {
	public:
	virtual Elem* search(int key) = 0; // -1 if not found
	virtual void insert(Elem * ekey,int idx) = 0;
	virtual	void deleteNode(int key) = 0;
	virtual void print(ReplacementPolicy* r) = 0;
};

class FIFO : public ReplacementPolicy {
	public:
	// FIFO(){count = 0; arr = new Elem*[MAXSIZE];}
	// ~FIFO(){}
	Elem* insert(Elem* e,int idx);
	void access(int idx);
	int remove();
	void print();

	//-----------------------------------------------------------custom
	int front, rear;
	int maxsize;
	FIFO() {
		//cout << "create FIFO" << endl;
	count = 0;
    front = -1;
    rear = -1;
	maxsize = MAXSIZE;
	//cout << maxsize;
	arr = new Elem*[MAXSIZE];
  }
	~FIFO(){
		delete[] arr;
	}
	//bool isFull();
	//bool isEmpty();
	void enQueue(Elem * element);
	Elem * deQueue();
	void display();
	//Elem * front();
	Elem * Erear();
};

class MRU : public ReplacementPolicy {
	public:
	int last_active;
	int * time;
	MRU(){
		count = 0;
		last_active = 0;
		arr = new Elem*[MAXSIZE]; 
		time = new int[MAXSIZE];
		for(int i = 0; i < MAXSIZE; i++) time[i] = 0;
		//cout << "MRU mo phac" << endl;
	}
	~MRU(){
		delete[] arr;
		delete[] time;
	}
	Elem* insert(Elem* e,int idx);
	void access(int idx);
	int remove();
	void print();
};

class LRU: public MRU {
	public:
	int remove() override {
		int minId = 0;
		int min = time[0];
		for(int i = 1; i < count; i++){
			if(time[i] < min)
			{
				min = time[i];
				minId = i;
			}
		}
		return minId;
	}
};

class LFU: public ReplacementPolicy {
	public:
	int * freq;
	int * time;
	int last_active;
	LFU(){
		count = 0; 
		arr = new Elem*[MAXSIZE]; 
		freq = new int[MAXSIZE];
		time = new int[MAXSIZE];
		last_active = 0;
		for(int i=  0; i < MAXSIZE; i++) {
            freq[i] = 0;
            time[i] = 0;
            }
	}
	~LFU(){
		delete[] arr;
		delete[] freq;
		delete[] time;
	}
	Elem* insert(Elem* e,int idx);
	void access(int idx);
	int remove();
	void print();
	void reheapUp(int index);
	void reheapDown(int index);
};


class Node {
   public:
    int key;
    Node *left;
    Node *right;
    int height;
    Elem* cont;
    ~Node(){delete cont;}
};


class AVL : public SearchEngine {
	public:
		// AVL() {}
		// ~AVL() {}
		void insert(Elem * ekey,int i);
		void deleteNode(int key);
		void print(ReplacementPolicy* q);
		Elem* search(int key);
		
		//-----------------------------------------------------------custom
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
		Node *deleteNodex(Node *root, int key);
		Node* search(Node* root, int key);
		void printTree(Node *root, string indent, bool last);
		void printPreO(Node *root);
		void printInO(Node *root);
		void emptyTree(Node *root);
};


enum STATUS_TYPE {NONE_FILLED, NON_EMPTY, DELETED};
class DBHashing: public SearchEngine {
	public:
		int (*h1)(int);
		int (*h2)(int);
		int size;
		STATUS_TYPE * status;
		Elem **hashtable;

		DBHashing(int (*h1)(int), int (*h2)(int), int s){
			this->h1 = h1;
			this->h2 = h2;
			this->size = s;
			status = new STATUS_TYPE[size];
			hashtable = new Elem*[size];
			for(int i = 0; i < s; i++){
				hashtable[i] = NULL;
				status[i] = NONE_FILLED;
			}
		}
		~DBHashing(){
			delete[] status;
			delete[] hashtable;
		}

		void insert(Elem *key, int i);
		void deleteNode(int key);
		void print(ReplacementPolicy* q);
		Elem * search(int key);
		
};


#endif