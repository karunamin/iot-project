#include <iostream>
#include <map>

int main() {
	std:: map<int, std::string> m;
	
	m.insert(std::pair<int, std::string>(40, "me"));
	m.insert(std::pair<int, std::string>(35, "show"));
	m.insert(std::make_pair(10, "doks"));

	/* pair 객체를 사용하는 경우 */
	std::map<int, std::string>::iterator iter;
	for (iter = m.begin(); iter != m.end(); iter++) {
		std::cout << " [ " << iter->first << ", " << iter->second << " ] " << " ";
		std::cout << " [ " << (*iter).first << ", " << (*iter).second << " ] " << " ";
	}
	std::cout << std::endl;

	/* operator[] */
	std::map<int, std::string>m2;
	m2[1] = "one";
	m2[2] = "two";

	for (auto& e : m2) {
		std::cout << e.first << " -> " << e.second << " ";
	}

	std::map<std::string, int>m3;
	m3.insert({ "one", 10 });
	m3.insert({ "two", 20 });

	for (auto& p : m3) {
		std::cout << "type: " << typeid(p).name() << "\n";
		std::cout << p.first << std::endl;
	}

	return 0;
}