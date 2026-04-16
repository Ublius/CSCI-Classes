#include "algorithms/spn/spn_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

/*
    Here is where you should define the logic for the SPN algorithm.
*/

SPNScheduler::SPNScheduler(int slice) {
  if (slice != -1) {
    throw("SPN must have a timeslice of -1");
  }
}

std::shared_ptr<SchedulingDecision> SPNScheduler::get_next_thread() {
  if (spn_queue.empty()) {
    auto decision = std::make_shared<SchedulingDecision>();
    decision->explanation = "No threads available for scheduling.";
    return decision;
  }

  auto next_thread = spn_queue.top();
  spn_queue.pop();

  auto decision = std::make_shared<SchedulingDecision>();
  decision->thread = next_thread;
  decision->time_slice = -1;
  decision->explanation = "Selected from " + std::to_string(this->size() + 1) + " threads. Will run to completion of burst.";

  return decision;
}

void SPNScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
  spn_queue.push(thread->get_next_burst(CPU)->length, thread);
}

size_t SPNScheduler::size() const {
  return spn_queue.size();
}
