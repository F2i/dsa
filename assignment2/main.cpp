#include <stdio.h>
#include "main.h"
#include "Cache.h"
#include "Cache.cpp"

Data* getData(string s) {
	stringstream ss(s);
	int idata;
    float fdata;
    if (ss >> idata) 
		return new Int(idata);
    else if (ss >> fdata)
    	return new Float(fdata);
    else if (s.compare("true") || s.compare("false"))
    	return new Bool(s.compare("true"));
    else {
		s.resize(s.size()-1);
		return new Address(stoi(s));
	}
    	
}
void simulate(string filename,Cache* c)
{
  ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  string s;
  while (getline(ifs, s))
  {
    stringstream ss(s);
    string code,tmp;
    ss >> code;
    int addr;
    switch (code[0]) {
    	case 'R': //read
			    ss >> addr; 
    			Data* res;
				res = c->read(addr);
    			if (res == NULL) {
    				ss >> tmp;
    				c->put(addr,getData(tmp));
    			} else {
					cout << res->getValue() << endl;
				}
    			break;
        case 'U': //put
                ss >> addr;
                ss >> tmp;
                c->put(addr,getData(tmp));
                break;
    	case 'W': //write
                ss >> addr;
    			ss >> tmp;
    			c->write(addr,getData(tmp));
    			break;
    	case 'P': // print
    			cout << "Print queue\n";
    			c->print();
    			break;
    	case 'E': // preorder
    			cout << "Print AVL in preorder\n";
    			c->preOrder();
    			break;
    	case 'I': // inorder
    			cout << "Print AVL in inorder\n";
    			c->inOrder();
    			break;
    }
  }
}

void test()
{
	 AVL * tree = new AVL;
//   Elem* buhbuh = new Elem;
//   buhbuh->addr = 6969;
  tree->root = tree->insertNode(tree->root, 33, new Elem(1,new Int(33),true));
  tree->root = tree->insertNode(tree->root, 13, new Elem(2,new Int(13),true));
  tree->root = tree->insertNode(tree->root, 53, new Elem(3,new Int(53),true));
  tree->root = tree->insertNode(tree->root, 9, new Elem(4 ,new Int(9),true));
  tree->root = tree->insertNode(tree->root, 21, new Elem(5,new Int(21),true));
  tree->root = tree->insertNode(tree->root, 61, new Elem(6,new Int(61),true));
  tree->root = tree->insertNode(tree->root, 8, new Elem(7,new Int(8),true));
  tree->root = tree->insertNode(tree->root, 11, new Elem(8,new Int(11),true));
  tree->printTree(tree->root, "", true);
  cout << "---------" << endl;
  tree->printInO(tree->root);
  cout << "---------" << endl;
  tree->printPreO(tree->root);
  tree->root = tree->deleteNode(tree->root, 13);
  cout << "After deleting " << endl;
  tree->printTree(tree->root, "", true);
  Node* meme = tree->search(tree->root,13);
  if(meme == NULL) cout << "deo co" << endl;
  else  cout<< ":" << meme->cont->addr << endl;
  cout << "---------" << endl;
  tree->printInO(tree->root);
  cout << "---------" << endl;
  tree->printPreO(tree->root);
}

void test2()
{
   Queue q(5);
  // Fails because front = -1
  q.deQueue();

   q.enQueue(new Elem(1,new Int(33),true));
  q.enQueue(new Elem(1,new Int(71),true));
  q.enQueue(new Elem(1,new Int(-33),true));
  q.enQueue(new Elem(1,new Int(-13),true));
  q.enQueue(new Elem(1,new Int(-9933),true));

  // Fails to enqueue because front == 0 && rear == SIZE - 1
  q.enQueue(new Elem(1,new Int(-133),true));

  q.display();

  Elem * elem = q.deQueue();

  if (elem != NULL)
    cout << endl
       << "Deleted Element is " << elem->data->getValue();

  q.enQueue(new Elem(1,new Int(7),true));

  q.display();

  q.deQueue();

  q.display();
  // Fails to enqueue because front == rear + 1
  q.enQueue(new Elem(1,new Int(-101),true));
   q.display();
}

int main(int argc, char* argv[]) {
	
	//test();
  
  test2();return 0;
	if (argc < 2)
    	return 1;

	Cache* c = new Cache(MAXSIZE); 
  	const char *fileName = argv[1];
    simulate(string(fileName),c);
  	delete c;
	return 0;
}
