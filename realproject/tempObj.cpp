#include <iostream>

using namespace std;

class Myclass {
private:
	int a;
public:
	Myclass(int aa) : a(aa) {
		cout << a << " constructor" << endl;
	}
	~Myclass() {
		cout << a << " destructor" << endl;
	}
	Myclass(const Myclass& other) : a(other.a) {
		cout << a << " copy constructor" << endl;
	}
};

int main() {
	//Myclass obj(10);

	Myclass obj1 = Myclass(20);	// 우측값은 임시객체가 아니다 Myclass obj(20)
	Myclass(30);				// 이름없는 임시객체

	cout << "bye~~" << endl;


	return 0;
}
