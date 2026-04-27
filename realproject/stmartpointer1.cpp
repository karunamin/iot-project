#include <iostream>
using namespace std;

class Myclass {
public:
	Myclass() {
		cout << "Myclass 객체 생성" << endl;
	}
};
int main() {
	int* p = new int;
	*p = 10;
	cout << "*p: " << *p << endl;

	delete p;

	//int* p2 = new int(20);

	int* arr = new int[5];
	arr[0] = 10;
	cout << "arr[0]: " << arr[0] << endl;

	delete[] arr;

	Myclass* obj = new Myclass();

	delete obj;

	unique_ptr<char[]> name = make_unique<char[]>(10);
	strcpy(name.get(), "홍길동");
	cout << name.get() << endl;

return 0;
}