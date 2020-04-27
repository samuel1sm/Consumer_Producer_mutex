#include <mutex> 
#pragma once

using namespace std;
class Semaphore
{
public:
	Semaphore(int permits);
	Semaphore(int permits, bool fair);


private:
	mutex mtx;
	int permits;
	int permits_max;
	int fair;
};



