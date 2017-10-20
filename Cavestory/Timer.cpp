#include "Timer.h"

// static
std::set<Timer*> Timer::timers_;

Timer::Timer(int expirationTime) :
	currentTime_(expirationTime + 1),
	expirationTime_(expirationTime) 
{
	timers_.insert(this);
}

Timer::~Timer()
{
	timers_.erase(this);
}

void Timer::reset()
{
	currentTime_ = 0;
}

bool Timer::active() const 
{ 
	return currentTime_ < expirationTime_; 
}

bool Timer::expired() const 
{ 
	return !active(); 
}

// Assumes the user knows if this is active or not
int Timer::currentTime() const 
{ 
	return currentTime_; 
}
// static
void Timer::updateAll(int elapsedTime)
{
	for (std::set<Timer*>::iterator it = timers_.begin();
		it != timers_.end();
		++it)
	{
		(*it)->update(elapsedTime);
	}
}

void Timer::update(int elapsedTime)
{
	if (active())
	{
		currentTime_ += elapsedTime;
	}
}