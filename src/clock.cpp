#include "clock.h"

void Clock::update() {
	auto now = std::chrono::steady_clock::now();
	if (update_) {
		elapsed_time_ = now - last_time_;
		last_time_ = now;
	}
}

double Clock::GetElapsedTime() const {
	return elapsed_time_.count();
}