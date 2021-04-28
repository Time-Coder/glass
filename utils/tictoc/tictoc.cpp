#include <tictoc.hpp>
#include <thread>
#include <iostream>

using namespace std;

std::chrono::time_point<std::chrono::system_clock> Timer::common_tic_time = std::chrono::system_clock::now();

Timer::Timer() : tic_time(std::chrono::system_clock::now()) {}

void Timer::tic()
{
	tic_time = std::chrono::system_clock::now();
}

float Timer::toc()
{
	return (std::chrono::system_clock::now() - tic_time).count()*1E-9;
}

void tic()
{
	Timer::common_tic_time = std::chrono::system_clock::now();
}

float toc()
{
	return (std::chrono::system_clock::now() - Timer::common_tic_time).count()*1E-9;
}

// float time()
// {
// 	return std::chrono::system_clock::now().count()*1E-9;
// }

void sleep(float seconds)
{
	std::this_thread::sleep_for(std::chrono::microseconds((unsigned long long)(seconds*1E6)));
}

void Scheduler::schedule(void (*func)(), double after = 0.0)
{
	if(th.joinable())
	{
		th.join();
	}
	if(after <= 0)
	{
		func();
		return;
	}
	_func = func;
	_after = after;
	cancled = false;
	timer.tic();
	th = thread(&Scheduler::wait_and_run, this);
}

void Scheduler::cancle()
{
	cancled = true;
	if(th.joinable())
	{
		th.join();
	}
}

void Scheduler::wait_and_run()
{
	while(timer.toc() <= _after)
	{
		if(cancled)
		{
			return;
		}
	}

	if(!cancled)
	{
		_func();
	}
}

Scheduler::~Scheduler()
{
	if(th.joinable())
	{
		th.join();
	}
}