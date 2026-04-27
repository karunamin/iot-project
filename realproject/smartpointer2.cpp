/*
	스마트 포인터: 특별한 클래스타입으로, 포인터처럼 동작하지만 스스로 메모리를 관리한다.(RAII 디자인 패턴)
	객체가 더 이상 필요하지 않을 때 (스코프를 벗어나면) 자동으로 삭제된다.
	unique_ptr: 소유권을 독점적으로 관리하는 스마트 포인터
*/

#include <iostream>

class Myclass {
public:
	Myclass() {
		std::cout << "Myclass() 호출: 객체 생성" << std::endl;
	}
	~Myclass() {
		std::cout << "~Myclass() 호출: 객체 소멸" << std::endl;
	}
};

int main() {
	std::unique_ptr<Myclass> ptr(new Myclass());		// 동적으로 생성된 Myclass객체를 unique_ptr 포인터가 가리킨다

	std::unique_ptr<Myclass> ptr1 = std::make_unique<Myclass>();	// C++14: new 사용을 자제한다

	auto ptr2 = std::make_unique<Myclass>();

	// std::unique_ptr<Myclass> ptr3 = ptr2; 식으로 복사는 안된다. 소유권을 독점한다
	std::unique_ptr<Myclass> ptr4 = std::move(ptr2);		// 복사는 허용하지 않으나 소유권 이전은 가능하다

	return 0;
}