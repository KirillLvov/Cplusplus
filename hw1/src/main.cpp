#include <iostream>
#include "process.h"

int main() {
	
	Process process("cat");
	char buf20[20];
	char buf17[17];
	
	std::cout << "Try to writeExact('The first message', 17)" << std::endl;
	try {
		process.writeExact("The first message", 17);
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << "Try to readExact(buf17, 17)" << std::endl;
	try {
		process.readExact(buf17, 17);
		std::cout << buf17 << std::endl;
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << "Try to write('The second message', 17)" << std::endl;
	try {
		process.write("The second message", 17);
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << "Try to read(buf20, 20)" << std::endl;
	try {
		process.read(buf20, 20);
		std::cout << buf20 << std::endl;
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << "Try to closeStdin()" << std::endl;
	try {
		process.closeStdin();
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << "Try to write('The third message', 17)" << std::endl;
	try {
		process.write("The third message", 17);
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << "Try to close()" << std::endl;
	try {
		process.close();
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << "Try to close() again" << std::endl;
	try {
		process.close();
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
	
	std::cout << "Try to read(buf20, 20)" << std::endl;
	try {
		process.read(buf20, 20);
		std::cout << buf20 << std::endl;
		std::cout << "OK" << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << err.what() << std::endl;
	}
}
