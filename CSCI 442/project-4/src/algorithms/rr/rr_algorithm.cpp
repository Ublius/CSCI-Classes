#include "algorithms/rr/rr_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

/*
    Here is where you should define the logic for the round robin algorithm.
*/

RRScheduler::RRScheduler(int slice) {
  if (slice == -1) {
    this->time_slice = 3;
  } else {
    this->time_slice = slice;
  }
}

std::shared_ptr<SchedulingDecision> RRScheduler::get_next_thread() {
  if (rr_queue.empty()) {
    auto decision = std::make_shared<SchedulingDecision>();
    decision->explanation = "No threads available for scheduling.";
    return decision;
  }

  auto next_thread = rr_queue.front();
  rr_queue.pop();

  auto decision = std::make_shared<SchedulingDecision>();
  decision->thread = next_thread;
  decision->time_slice = this->time_slice;
  decision->explanation = "Selected from " + std::to_string(rr_queue.size() + 1) +
                            " threads. Will run for at most " + std::to_string(this->time_slice) + " ticks.";

  return decision;
}

void RRScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
  rr_queue.push(thread);
}

size_t RRScheduler::size() const {
  return rr_queue.size();
}
