
#include <iostream>

using namespace std;


template<typename T>	// 클래스 템플릿
class A {
	T num;
public:
	A(T data) : num(data) { }
};
template<typename T>	// 함수 템플릿
T Add(T a, T b) {
	return a + b;
}
template<typename T>
void swwp(T& a, T& b) {
	T temp;
	temp = a;
	a = b;
	b = temp;
}

int main() {
	char a = 'a', b = 'b';

	cout << "호출전 a : " << a << ",호출전 b : " << b << endl;
	swwp<char>(a, b);
	cout << "호출후 a : " << a << ",호출후 b : " << b << endl;

	int res = Add<double>(10.1, 20.1);

	cout << res << endl;

	A<int> aa(10);
	A<double> d(10.1);

	return 0;
}
