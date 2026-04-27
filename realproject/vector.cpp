#include <iostream>
#include <vector>
using namespace std;

int main() {
	vector<int> v;
	vector<int> v1(10);
	vector<int> v2 = { 1, 2, 3, 4, 5 };

	cout << "v size : " << v.size() << endl;
	cout << "v2 size : " << v1.size() << endl;
	cout << "v3 size : " << v2.size() << endl;

	cout << "v1 벡터객체의 메모리크기 : " << sizeof(v1) << endl;
	cout << "v2 벡터객체의 메모리크기 : " << sizeof(v2) << endl;
	cout << "v 벡터객체의 메모리크기 : " << sizeof(v) << endl;

	//cout << "v[0] : " << v[0] << endl;

	for (auto i : v2) {
		cout << i << endl;			// 범위기반 for문
		//cout << v2[i] << endl;		// i는 인덱스가 아니다. 요소값들을 차례로 저장하는 변수다
		cout << endl;
	}
	
	cout << "v1의 첫번째 요소값 : " << *v2.begin() << endl;
	cout << "v2의 첫번째 요소값 : " << *(v2.end() - 1) << endl;

	return 0;
}