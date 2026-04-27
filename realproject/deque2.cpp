#include <iostream>
#include <deque>
//#include <string>

using namespace std;

int main() {
	
	deque<string> dq;	// dq 객체 생성

	dq.push_front("tiger");
	dq.push_front("lion");
	dq.push_back("cats");

	deque<string>::iterator it;
	for (it = dq.begin(); it != dq.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	deque<string>::const_iterator constiter = dq.begin();
	constiter += 2;						// const int* pa = &pa;
	dq.insert(constiter, 2, "INSTERT");
	for (constiter = dq.begin(); constiter != dq.end(); constiter++) {
		cout << *constiter << " ";
	}

	constiter = dq.end();
	constiter--;		// 마지막 원소
	constiter--;		// 마지막 앞 원소
	dq.erase(constiter);
	for (constiter = dq.begin(); constiter != dq.end(); constiter++) {
		cout << *constiter << " ";
	}
	cout << endl;
	
	for (auto element : dq) {
		cout << element << " ";
	}
	cout << endl;
	
	for (auto& element : dq) {
		cout << element << " ";
	}
	cout << endl;





	//int a = 10;
	//int b = 20;
	//const int* pa = &a;		// 데이터를 상수-포인터 간접참조로 데이터를 변경하지 마라
	//int* const pb = &b;		// 포인터 변수 상수-포인터에 다른 변수에 주소로 변경하지 마라
	//
	//*pa = 100;
	//pa = &b;

	//const int c = 10;
	//const int* pc = &c;


	return 0;

}