#ifndef ZZ_TIMER
#define ZZ_TIMER

#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class Timer {
public:
	void Start() {
		start_tp_ = chrono::steady_clock::now();
	}
	void End(const char* pNameIn) {
		std::cout << pNameIn << " " << chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start_tp_).count() << std::endl;;
	}
	chrono::steady_clock::time_point start_tp_;
};
static Timer timer;

#endif
