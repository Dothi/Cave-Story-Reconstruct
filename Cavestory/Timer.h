#ifndef TIMER_H_
#define TIMER_H_

#include <set>

struct Timer
{
	Timer(int expirationTime);
	~Timer();

	// ---------non copyable------------------------
	Timer(const Timer&) = delete;
	const Timer& operator=(const Timer&) = delete;
	// ---------------------------------------------

	void reset();
	bool active() const;
	bool expired() const;
	// Assumes the user knows if this is active or not
	int currentTime() const;

	static void updateAll(int elapsedTime);

private:
	void update(int elapsedTime);

	int currentTime_;
	const int expirationTime_;

	static std::set<Timer*> timers_;
};

#endif
