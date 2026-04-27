#include <iostream>
#include <deque>

int main() {

	std::deque<int> dq;

	for (int i = 0; i <= 5; i++) {
		dq.push_back(i * 10);
	}
	std::cout << std::endl;

	std::deque<int>::iterator it;
	for (it = dq.begin(); it != dq.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	dq.push_front(1);	// 앞쪽에 삽입
	dq.push_front(2);
	dq.push_back(100);	// 뒤쪽에 삽입
	dq.push_back(200);

	for (it = dq.begin(); it != dq.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;


	return 0;
}