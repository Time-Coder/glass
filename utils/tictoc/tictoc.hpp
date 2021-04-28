#ifndef __TICTOC_HPP__
#define __TICTOC_HPP__

#include <chrono>
#include <thread>

struct Timer
{
	static std::chrono::time_point<std::chrono::system_clock> common_tic_time;

private:
	std::chrono::time_point<std::chrono::system_clock> tic_time;

public:
	Timer();
	void tic();
	float toc();
};

class Scheduler
{
	void (*_func)() = NULL;
	bool cancled = true;
	Timer timer;
	double _after = 0.0;
	std::thread th;

public:
	~Scheduler();
	void schedule(void (*func)(), double after);
	void cancle();
	void wait_and_run();
};

void tic();
float toc();
float time();
void sleep(float seconds);

#endif