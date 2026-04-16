#include "algorithms/priority/priority_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>


/*
    Here is where you should define the logic for the priority algorithm.
*/

PRIORITYScheduler::PRIORITYScheduler(int slice) {
  if (slice != -1) {
    throw("PRIORITY must have a timeslice of -1");
  }
}

std::shared_ptr<SchedulingDecision> PRIORITYScheduler::get_next_thread() {
  if (priority_queue.empty()) {
    auto decision = std::make_shared<SchedulingDecision>();
    decision->explanation = "No threads available for scheduling.";
    return decision;
  }

  int before_counts[4] = {0, 0, 0, 0};
  Stable_Priority_Queue<std::shared_ptr<Thread>> temp_queue = priority_queue;
  while (!temp_queue.empty()) {
      auto thread = temp_queue.top();
      before_counts[thread->priority]++;
      temp_queue.pop();
  }

  auto next_thread = priority_queue.top();
  priority_queue.pop();

  int after_counts[4] = {0, 0, 0, 0};
  temp_queue = priority_queue;
  while (!temp_queue.empty()) {
      auto thread = temp_queue.top();
      after_counts[thread->priority]++;
      temp_queue.pop();
  }

  auto decision = std::make_shared<SchedulingDecision>();
  decision->thread = next_thread;
  decision->time_slice = -1;

  decision->explanation =
      "[S: " + std::to_string(before_counts[0]) + " I: " + std::to_string(before_counts[1]) +
      " N: " + std::to_string(before_counts[2]) + " B: " + std::to_string(before_counts[3]) +
      "] -> [S: " + std::to_string(after_counts[0]) + " I: " + std::to_string(after_counts[1]) +
      " N: " + std::to_string(after_counts[2]) + " B: " + std::to_string(after_counts[3]) +
      "]. Will run to completion of burst.";

  return decision;
}

void PRIORITYScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
  priority_queue.push(thread->priority, thread);
}

size_t PRIORITYScheduler::size() const {
  return priority_queue.size();
}
