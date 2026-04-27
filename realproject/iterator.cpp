/*
	iterator(반복자) : 컨테이너 내부의 데이터에 접근할 수 있는 포인터
	vector 
*/
#include <iostream>
#include <vector>


int main()
{
	std::vector<int> v = { 10, 20, 30, 40, 50 };

	std::vector<int>::iterator iter;	// 반복자 선언
	for (iter = v.begin(); iter != v.end(); iter++) {
		std::cout << *iter << " ";
	}
	std::cout << std::endl;

	for (auto it = v.begin(); it != v.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::vector<int>::const_reverse_iterator rit;	// 역ㅂ아향 반복자 선언
	for (rit = v.rbegin(); rit != v.rend(); rit++) {
		std::cout << *rit << " ";
	}
	std::cout << std::endl;
	
	for(auto riter=v.rbegin(); rit != v.rend(); rit++) {
		std::cout << *rit << " ";

	return 0;
}