#include "VM.h"
#include "VM.cpp"
using namespace std;

void test(string filename) {
    VM *vm = new VM();
    try {
        vm->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete vm;
}

int main(int argc, char **argv)
{
//  if (argc < 2)
//    return 1;
//  test(argv[1]);
//std::ios_base::sync_with_stdio(false);
  test("prog1.txt");
  cout << endl;
  test("prog2.txt");
  cout << endl;
  test("prog3.txt");
  cout << endl;
  test("prog4.txt");
  cout << endl;
  test("prog5.txt");
  cout << endl;
  test("prog6.txt");
  cout << endl;
  test("prog7.txt");
  cout << endl;
  test("prog8.txt");
  cout << endl;
  test("prog9.txt");
  cout << endl;
  test("prog10.txt");
  cout << endl;
  test("buhbuh.txt");
  cout << 1781.1234567 <<endl;
  //system("pause");
  return 0;
}
