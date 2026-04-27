// 멀티 스레드: 여러작업을 동시 수행
#include <iostream>
#include <thread>

void work() {
	for (int i = 0; i <= 20; i++) {
		std::cout << "작업 스레드: " << i << std::endl;
	}
}

int main() {
	std::thread t(work);
	//work();
	for (auto i = 1; i <= 20; i++) {
		std::cout << "메인 스레드: " << i << std::endl;
	}

	t.join();

	return 0;
}