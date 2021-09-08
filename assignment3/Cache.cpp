#include "main.h"
Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s) {}
Cache::~Cache(){
	//delete rp;
	//delete s_engine;
}
Data* Cache::read(int addr) {
    
    Elem * tmp = s_engine->search(addr);
    //cout << "end";
    if(tmp != NULL) 
    {
        rp->access(rp->index(addr));
        return tmp->data;
    }
    return NULL;
}
Elem* Cache::put(int addr, Data* cont) {
    Elem * toBeAdd = new Elem(addr,cont, true);
    Elem * toBeRemove = rp->insert(toBeAdd, 0);
    if(toBeRemove != NULL) s_engine->deleteNode(toBeRemove->addr);
    s_engine->insert(toBeAdd, 0);
    return toBeRemove;
}
Elem* Cache::write(int addr, Data* cont) {
    Elem * toBeWrite = s_engine->search(addr);
    if(toBeWrite != NULL){
        rp->access(rp->index(addr));
        toBeWrite->data = cont;
        toBeWrite->sync = false;
        return NULL;
    }
    else{
        Elem * toBeAdd = new Elem(addr,cont, false);
        Elem * toBeRemove = rp->insert(toBeAdd, 0);
        if(toBeRemove != NULL) s_engine->deleteNode(toBeRemove->addr);
        s_engine->insert(toBeAdd, 0);
        return toBeRemove;
    }
}
void Cache::printRP() {
    rp->print();
}
void Cache::printSE() {
    s_engine->print(rp);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////---------------------------------AVL------------------------------------------------////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Calculate height
    int AVL::height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
    }

    int AVL::max(int a, int b) {
    return (a > b) ? a : b;
    }

    // New node creation
    Node* AVL::newNode(int key, Elem* cont) {
    Node *node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->cont = cont;
    return (node);
    }

    // Rotate right
    Node* AVL::rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left),
            height(y->right)) +
            1;
    x->height = max(height(x->left),
            height(x->right)) +
            1;
    return x;
    }

    // Rotate left
    Node * AVL::leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left),
            height(x->right)) +
            1;
    y->height = max(height(y->left),
            height(y->right)) +
            1;
    return y;
    }

    // Get the balance factor of each node
    int AVL::getBalanceFactor(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) -
        height(N->right);
    }

    // Insert a node
    Node * AVL::insertNode(Node *node, int key, Elem* cont) {
    // Find the correct postion and insert the node
    if (node == NULL)
        return (newNode(key, cont));
    if (key < node->key)
        node->left = insertNode(node->left, key, cont);
    else if (key > node->key)
        node->right = insertNode(node->right, key, cont);
    else
        return node;

    // Update the balance factor of each node and
    // balance the tree
    node->height = 1 + max(height(node->left),
                height(node->right));
    int balanceFactor = getBalanceFactor(node);
    if (balanceFactor > 1) {
        if (key < node->left->key) {
        return rightRotate(node);
        } else if (key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
        }
    }
    if (balanceFactor < -1) {
        if (key > node->right->key) {
        return leftRotate(node);
        } else if (key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
        }
    }
    return node;
    }

    // Node with minimum value
    Node * AVL::nodeWithMimumValue(Node *node) {
    Node *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
    }

    // Delete a node
    Node * AVL::deleteNodex(Node *root, int key) {
    // Find the node and delete it
    if (root == NULL)
        return root;
    if (key < root->key)
        root->left = deleteNodex(root->left, key);
    else if (key > root->key)
        root->right = deleteNodex(root->right, key);
    else {
        if ((root->left == NULL) ||
        (root->right == NULL)) {
        Node *temp = root->left ? root->left : root->right;
        if (temp == NULL) {
            temp = root;
            root = NULL;
        } else {
            *root = *temp;
            //cout  << "--tag1--";
        }
        free(temp);
        } else {
        Node *temp = nodeWithMimumValue(root->right);
        root->key = temp->key;
        *(root->cont) =  *(temp->cont);
        //root->cont = new Elem(temp->cont->addr,temp->cont->data,temp->cont->sync);
        //cout  << "--tag2--";
        root->right = deleteNodex(root->right,
                    temp->key);
        }
    }

    if (root == NULL)
        return root;

    // Update the balance factor of each node and
    // balance the tree
    root->height = 1 + max(height(root->left),
                height(root->right));
    int balanceFactor = getBalanceFactor(root);
    if (balanceFactor > 1) {
        if (getBalanceFactor(root->left) >= 0) {
        return rightRotate(root);
        } else {
        root->left = leftRotate(root->left);
        return rightRotate(root);
        }
    }
    if (balanceFactor < -1) {
        if (getBalanceFactor(root->right) <= 0) {
        return leftRotate(root);
        } else {
        root->right = rightRotate(root->right);
        return leftRotate(root);
        }
    }
    return root;
    }

    //
    Node* AVL::search(Node* root, int key)
    {
        // Base Cases: root is null or key is present at root
        if (root == NULL || root->key == key)
        return root;
        
        // Key is greater than root's key
        if (root->key < key)
        return search(root->right, key);
    
        // Key is smaller than root's key
        return search(root->left, key);
    }

    // Print the tree
    void AVL::printTree(Node *root, string indent, bool last) {
    if (root != NULL) {
        cout << indent;
        if (last) {
        cout << "R----";
        indent += "   ";
        } else {
        cout << "L----";
        indent += "|  ";
        }
        cout << root->key << endl;
        printTree(root->left, indent, false);
        printTree(root->right, indent, true);
    }
    }

    void AVL::printPreO(Node *root)
    {   
      if (root == NULL)   return;
      root->cont->print();
      printPreO(root->left);
      //cout << root->key << ":";
      printPreO(root->right);
    }
     void AVL::printInO(Node *root)
    {   
      if (root == NULL)   return;
      //cout << root->key << ":";
      printInO(root->left);
      root->cont->print();
      printInO(root->right);
    }
    
void AVL::emptyTree(Node* root)
{
    if(root)
    {
        emptyTree(root->left);
        emptyTree(root->right);
        delete root;
    }
}

		void AVL::insert(Elem * ekey, int i ){
            root = insertNode(root, ekey->addr, ekey);
            //cout << "in cay insert" << endl;
            //printTree(root, "", true);
        }

		void AVL::deleteNode(int key){
            root = deleteNodex(root, key);
            //cout << "in cay delete" << endl;
            //printTree(root, "", true);
        }

		void AVL::print(ReplacementPolicy* q) {
			cout << "Print AVL in inorder:\n";
			printInO(root);
			cout << "Print AVL in preorder:\n";
			printPreO(root);
            //printTree(root, " ", true);
        }

		Elem* AVL::search(int key){
            Node * tmp = search(root, key);
            if(tmp != NULL) return tmp->cont;
            return NULL;
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////---------------------------------FIFO------------------------------------------------////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Check if the queue is full
//   bool FIFO::isFull() {
//     if (front == 0 && rear == maxsize - 1) {
//       return true;
//     }
//     if (front == rear + 1) {
//       return true;
//     }
//     return false;
//   }
//   // Check if the queue is empty
//   bool FIFO::isEmpty() {
//     if (front == -1)
//       return true;
//     else
//       return false;
//   }
  // Adding an element
  void FIFO::enQueue(Elem * element) {
    if (isFull()) {
      //cout << "Queue is full";
    } else {
      if (front == -1) front = 0;
      rear = (rear + 1) % maxsize;
      arr[rear] = element;
      //cout << endl  << "Inserted " << element->addr << endl;
        count++;
    }
  }
  // Removing an element
  Elem * FIFO::deQueue() {
    Elem * element;
    if (isEmpty()) {
      //cout << "Queue is empty" << endl;
      return NULL;
    } else {
      element = arr[front];
      if (front == rear) {
        front = -1;
        rear = -1;
      }
      // Q has only one element,
      // so we reset the queue after deleting it.
      else {
        front = (front + 1) % maxsize;
      }
      count--;
      return (element);
    }
  }

  void FIFO::display() {
    // Function to display status of Circular Queue
    int i;
    if (isEmpty()) {
        //do no thing
    } else {
      for (i = front; i != rear; i = (i + 1) % maxsize)
        arr[i]->print();
      arr[i]->print();
    }
  }

Elem * FIFO::Erear()
{
    if(isEmpty()) return  NULL;
    return arr[rear];
}

	Elem* FIFO::insert(Elem* e,int idx){
        Elem * tmp = NULL;
        if(isFull()) tmp = deQueue();
        enQueue(e);
        return tmp;
    }

	void FIFO::access(int idx) {}

	int FIFO::remove(){return 0;}

	void FIFO::print(){
        display();
    }

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////---------------------------------MRU------------------------------------------------////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Elem* MRU::insert(Elem* e,int idx){
        Elem *tmp = NULL;
		if(isFull()){
            int removeId = remove();
			tmp = arr[removeId];
			access(removeId);
			arr[removeId] = e;
		}
		else{
            access(count);
			arr[count] = e;
			count++;
		}
		return tmp;
    }

	void MRU::access(int idx){
        last_active++;
       time[idx] = last_active;
    }

	int MRU::remove(){
		for(int i = 0; i < MAXSIZE; i++) if(time[i] == last_active) return i;
		return 0;
    }

	void MRU::print(){
        if(count == 0) return;
        int tmp0 = count;
        int tmp1 = last_active;
        while(tmp0 > 0){
            for(int i = 0; i < count; i++){
                if(time[i] == tmp1)
                {
                    arr[i]->print();
                    tmp0--;
                    break;
                }
            }
            tmp1--;
        }
    }

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////---------------------------------LFU------------------------------------------------////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LFU::reheapDown(int index)
	{
        int ifreq = freq[index];
        Elem * iarr = arr[index];
        int i = index;
        int u = 0;
        int k;
        Elem *_k;
        bool check = false;
        while(2*i + 1 < count){
            u = 2*i + 1;
            k = freq[u];
            _k = arr[u];
            if(2*i + 2 < count) {
                if(k > freq[u+1])  k = freq[++u];
                _k = arr[u];
            }
            if(ifreq < k){
                freq[i] = ifreq;
                arr[i] = iarr;
                check = true;
            // cout << "debug here end" <<  << arr[i]->addr << ":" << freq[i];
                break;
            }
            else{
                freq[i] = k;
                arr[i] = _k;
                i = u;
            // cout << "debug here end2" << arr[i]->addr << ":" << freq[i];
            }
        }
        if(!check){
            freq[i] = ifreq;
            arr[i] = iarr;
        } 
            // cout << "debug finish here end" <<  << arr[index]->addr << ":" << freq[index];          
	}

	void LFU::reheapUp(int index){
        int ifreq = freq[index];
        Elem * iarr = arr[index];
        int i = index;
        int k;
        Elem *_k;
        int u = (i%2 == 0)? (i/2 - 1) : i/2; 
        bool check = false;
        while(u >= 0){
            k = freq[u];
            _k = arr[u];
            if(ifreq >= k){
                freq[i] = ifreq;
                arr[i] = iarr;
                check = true;
            // cout << "debug here end" <<  << arr[i]->addr << ":" << freq[i];
                break;
            }
            else{
                freq[i] = k;
                arr[i] = _k;
                i = u;
            // cout << "debug here end" <<  << arr[i]->addr << ":" << freq[i];
            }
            u = (i%2 == 0)? (i/2 - 1) : i/2;
        }

        if(!check){
            freq[i] = ifreq;
            arr[i] = iarr;
        }
            // cout << "debug finish here end" <<  << arr[index]->addr << ":" << freq[index];
}

	Elem *LFU::insert(Elem* e, int idx){
		Elem *tmp = NULL;
		if(isFull()){
			tmp = arr[0];
			arr[0] = arr[count - 1];
			freq[0] = freq[count - 1];
            count--;
			reheapDown(0);
            count++;
            access(count - 1);
            arr[count - 1] = e;
			freq[count - 1] = 1;
			reheapUp(count - 1);
		}
		else{
            
            access(count);
            arr[count] = e;
			count++;
			reheapUp(count - 1);
		}
		return tmp;
	}
	void LFU::access(int idx){
		freq[idx]++;
        last_active++;
		time[idx] = last_active;
        if(forHeap){
            reheapDown(idx);
            forHeap = false;
        }
	}

	int LFU::remove(){
		return 0;
	}

	void LFU::print(){
		for(int i = 0; i < count; i++)	arr[i]->print();
	}



    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////---------------------------------DBHasing------------------------------------------------////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DBHashing::insert(Elem *ekey, int i){
	i = 0;
	while(i < size){
		int idx = (h1(ekey->addr) + i*h2(ekey->addr)) % size;
		if(status[idx] != NON_EMPTY){
			hashtable[idx] = ekey;
			status[idx] = NON_EMPTY;
			return ;
		}
		else i++;
	}
}
void DBHashing::deleteNode(int key){
	int i = 0;
	while(i < size){
		int idx = (h1(key) + i*h2(key)) % size;
		if(hashtable[idx]->addr == key){
			status[idx] = DELETED;
			return ;
		}
		else i++;
	}
}

void DBHashing::print(ReplacementPolicy* q){
	cout << "Prime memory:" << endl;
	for(int i = 0; i < size; i++){
		if(this->status[i] == NON_EMPTY) hashtable[i]->print();
	}
}
Elem * DBHashing::search(int key) {
	int i = 0;
	while(i < size){
		int idx = (h1(key) + i*h2(key)) % size;
        if(status[idx] == NONE_FILLED) return NULL;
        else if(status[idx] == NON_EMPTY) {    
            if(hashtable[idx]->addr == key) return hashtable[idx]; 
        }
		i++;
	}
    return NULL;
    //cout << "here2" << endl;
}