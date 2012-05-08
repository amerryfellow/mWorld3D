#include <GL/glfw.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>

#include "main.h"

using namespace std;

int main() {
	cout << "Starting Graphics Thread" << endl;
	std::thread t1(graphics);
	
	cout << "Starting Physics Thread" << endl;
	std::thread t2(physics);
	
	cout << "Thread ID 1 " << t1.get_id() << endl;
	cout << "Thread ID 2 " << t2.get_id() << endl;
	t1.join();
	t2.join();
	
	return 0;
}
