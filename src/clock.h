#ifndef __CLOCK_H_
#define __CLOCK_H_

#include <chrono>
// 时钟，用于更新位置
class Clock {
public:
	Clock(bool update) : last_time_(std::chrono::steady_clock::now()), elapsed_time_(0), update_(update) {}

	void update();

	double GetElapsedTime() const;

	void SetUpdate(bool update) { update_ = update; elapsed_time_ = std::chrono::duration<double>(0); }

private:
	std::chrono::time_point<std::chrono::steady_clock> last_time_;
	std::chrono::duration<double> elapsed_time_;
	bool update_;
};

#endif