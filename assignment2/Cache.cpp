#include "main.h"

Data* Cache::read(int addr) {
    Node * tmp = tree->search(tree->root, addr);
    if(tmp != NULL) return tmp->cont->data;
    return NULL;
}

Elem* Cache::put(int addr, Data* cont, bool bsync = true) {
    if(!cqueue->isFull())
    {
        cqueue->enQueue(new Elem(addr,cont,bsync));
        tree->root = tree->insertNode(tree->root, addr, cqueue->Erear());
        return NULL;
    }
    else
    {
        Elem * tmp = cqueue->deQueue();
        tree->root = tree->deleteNode(tree->root, tmp->addr);
        cqueue->enQueue(new Elem(addr,cont,bsync));
        tree->root = tree->insertNode(tree->root, addr, cqueue->Erear());
        return tmp;
    }
}

Elem* Cache::write(int addr, Data* cont) {
    Node * tmp = tree->search(tree->root, addr);
    if(tmp != NULL) 
    {
        tmp->cont->data = cont;
        tmp->cont->sync = false;
        return NULL;
    }
    Elem * tmpE =  put(addr, cont, false);
    return tmpE;
}

void Cache::print() {
    cqueue->display();
}
void Cache::preOrder() {
    tree->printPreO(tree->root);
}
void Cache::inOrder() {
    tree->printInO(tree->root);
}



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
    Node * AVL::deleteNode(Node *root, int key) {
    // Find the node and delete it
    if (root == NULL)
        return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
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
        root->right = deleteNode(root->right,
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
    
  // Check if the queue is full
  bool Queue::isFull() {
    if (front == 0 && rear == maxsize - 1) {
      return true;
    }
    if (front == rear + 1) {
      return true;
    }
    return false;
  }
  // Check if the queue is empty
  bool Queue::isEmpty() {
    if (front == -1)
      return true;
    else
      return false;
  }
  // Adding an element
  void Queue::enQueue(Elem * element) {
    if (isFull()) {
      //cout << "Queue is full";
    } else {
      if (front == -1) front = 0;
      rear = (rear + 1) % maxsize;
      arr[rear] = element;
      //cout << endl  << "Inserted " << element->addr << endl;
    }
  }
  // Removing an element
  Elem * Queue::deQueue() {
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
      return (element);
    }
  }

  void Queue::display() {
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

Elem * Queue::Erear()
{
    if(isEmpty()) return  NULL;
    return arr[rear];
}