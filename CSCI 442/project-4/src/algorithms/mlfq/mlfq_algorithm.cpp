#include "algorithms/mlfq/mlfq_algorithm.hpp"

#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

/*
    Here is where you should define the logic for the MLFQ algorithm.
*/

MLFQScheduler::MLFQScheduler(int slice) {
  if (slice != -1) {
    throw("MLFQ does NOT take a customizable time slice");
  }
}



std::shared_ptr<SchedulingDecision> MLFQScheduler::get_next_thread() {

  for (int i = 0; i < 10; ++i) {
    if (!mlfq_queue[i].empty()) {
      std::shared_ptr<SchedulingDecision> next_decision = std::make_shared<SchedulingDecision>();
      next_decision->thread = mlfq_queue[i].top();
      mlfq_queue[i].pop();
      this->time_slice = pow(2, i);

      next_decision->explanation = "Selected from queue " +
            std::to_string(i) + " (priority = " +
            std::string(PROCESS_PRIORITY_MAP[next_decision->thread->priority]) + ", runtime = " +
            std::to_string(next_decision->thread->queue_total_time) + "). Will run for at most " +
            std::to_string(time_slice) + " ticks.";
      next_decision->thread->queue_total_time += next_decision->thread->get_next_burst(CPU)->length;
      return next_decision;
    }
  }

  std::shared_ptr<SchedulingDecision> next_decision = std::make_shared<SchedulingDecision>();
  next_decision->explanation = "No threads available for scheduling.";
  return next_decision;
}

void MLFQScheduler::add_to_ready_queue(std::shared_ptr<Thread> thread) {
  if(thread->queue_total_time >= pow(2, thread->queue_num) && thread->queue_num != (n-1)){
    thread->queue_num ++;
    thread->queue_total_time=0;
  }
  mlfq_queue[thread->queue_num].push(thread->priority, thread);
}

size_t MLFQScheduler::size() const {
  size_t total_size = 0;

  for (int i = 0; i < 10; ++i) {
    if (mlfq_queue[i].empty() == false){
      total_size += mlfq_queue[i].size();
    }
  }

  return total_size;
}
