#include <stdio.h>
#include "main.h"
#include "Cache.h"
#include "Cache.cpp"

int h1(int k) {
	return k + 1;
}
int h2(int k) {
	return 2 * k + 1;
}
int h3(int k) {
	return 3 * k;
}
int h4(int k) {
	return 3 * k + 5;
}
Data* getData(string s) {
	stringstream ss;
	ss << s;
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
    return NULL;	
}
void simulate(string filename)
{
  	ifstream ifs;
  	ifs.open(filename, std::ifstream::in);
  	string s;
	SearchEngine* sr;
	ReplacementPolicy* rp;
	Cache* c;
  	while (getline(ifs, s))
  	{
    	stringstream ss(s);
    	string code,tmp;
    	ss >> code;
    	int addr;

    	switch (code[0]) {
			case 'M': //MAXSIZE
				ss >> addr;
				MAXSIZE = addr;
				//cout << "M" << endl;
				break;
			case 'S': // Search Engine
				ss >> tmp;
				if (tmp[0] == 'A')
					sr = new AVL();
				else {
					int (*hash1)(int);
					int (*hash2)(int);
					hash1 = tmp[1]=='1'?h1:tmp[1]=='2'?h2:tmp[1]=='3'?h3:h4;
					hash2 = tmp[2]=='1'?h1:tmp[2]=='2'?h2:tmp[2]=='3'?h3:h4;
					ss >> addr;//size
					sr = new DBHashing(hash1,hash2,addr);
				}
				//cout << "S" << endl;
				break;
			case 'T':// ReplacementPolicy
				ss >> addr;
				if (addr==1) rp = new LRU();
				else if (addr==2) rp = new LFU();
				else if (addr==3) rp = new FIFO();
				else rp = new MRU();
				c = new Cache(sr,rp);
				//cout << "T" << endl;
				break;
    		case 'R': //read
			    ss >> addr; 
    			Data* res;
				//cout << "R0" << endl;
				res = c->read(addr);
    			if (res == NULL) {
    				ss >> tmp;
    				c->put(addr,getData(tmp));
    			} else {
					cout << res->getValue() << endl;
				}
				//cout << "R" << endl;
    			break;
        	case 'U': //put
                ss >> addr;
                ss >> tmp;
                c->put(addr,getData(tmp));
				//cout << "U" << endl;
                break;
    		case 'W': //write
                ss >> addr;
    			ss >> tmp;
    			c->write(addr,getData(tmp));
				//cout << "W" << endl;
    			break;
    		case 'P': // print
    			cout << "Print replacement buffer\n";
    			c->printRP();
				//cout << "P" << endl;
    			break;
    		case 'E': //
    			cout << "Print search buffer\n";
    			c->printSE();
				//cout << "E" << endl;
    			break;
    	}
  	}
	delete c;
}

void test()
{
	 AVL * tree = new AVL;
   Elem* buhbuh = new  Elem(1,new Int(33),true);
//   buhbuh->addr = 6969;
  tree->root = tree->insertNode(tree->root, 33, buhbuh);
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
  tree->root = tree->deleteNodex(tree->root, 13);
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
	MAXSIZE = 15;
   FIFO q;
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
	if (argc < 2)
    	return 1;
  	const char *fileName = argv[1];
    simulate(string(fileName));
	return 0;
}

