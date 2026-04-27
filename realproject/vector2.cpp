#include <iostream>
#include <vector>

using namespace std;

int main() {
	vector<int>	v1;

	//cout << "v[i], v.size(), v.capacity()" << endl;
	//for (auto i = 0; i < 32; i++) {
	//	v.push_back(i + 1);
	//	cout << " [ " << v[i] << ", " << v.size() << ", " << v.capacity() << " ] " << endl;
	//}

	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	v1.push_back(5);

	v1.insert(v1.begin(), 10);
	v1.insert(find(v1.begin(), v1.end(), 3), 30);	// 원소3앞에 30넣기
	v1.pop_back();

	for (auto i : v1)
	{
		cout << "i; " << i << endl;
	}


	return 0;
}

/*
	v.push_back(7): 마지막 원소 뒤에 원소 7을 추가한다.
	v.insert(i, val): i번째 위치에 원소 vlal을 넣는다.
	v.pop_back(): 마지막 원소를 제거한다.
	v.size(): 벡터에 저장된 원소 개수를 리턴한다.
	v.capacity(): 벡터에 저장 가능한 공간의 크기를 리턴한다
*/