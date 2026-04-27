#include <iostream>

using namespace std;

class Myclass {
	int n;
public:
	Myclass(int aa) : n(aa) {
		cout << n << "constructor" << endl;
	}
	~Myclass() {
		cout << n << "destructor" << endl;
	}
	Myclass(const Myclass& other) : n(other.n) {
		cout << n << "copy constructor" << endl;
	}
	void set(int aa) {
		n = aa;
		cout << "ser(" << n << ")" << endl;

	}
};
Myclass func(Myclass ob) {
	cout << "func()" << endl;
	return ob;
}
int main() {

	//Myclass* obj1 = &Myclass(10);

	//Myclass obj = Myclass(11);
	//obj.set(20);

	//const Myclass& obj1 = Myclass(30);		// 임시객체를 상수로 참조하면 가능
	////obj1.set(25);

	//Myclass &&obj2 = Myclass(40);
	//obj2.set(44);

	//cout << "bye~~" << endl;

	Myclass obj11(10);

	//Myclass obj12(func(obj11));
	//Myclass obj12(obj11);
	cout << endl;
	func(obj11);
	cout << endl;
	
	cout << "bye~~" << endl;


	return 0;
}