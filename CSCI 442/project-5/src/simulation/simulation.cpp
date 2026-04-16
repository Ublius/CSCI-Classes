/**
 * This file contains implementations for the methods defined in the Simulation
 * class.
 *
 * You'll probably spend a lot of your time here.
 */

#include "simulation/simulation.h"
#include <stdexcept>

Simulation::Simulation(FlagOptions& flags)
{
    this->flags = flags;

    // NOTE: this initializes this->frames to have NUM_FRAMES number of elements!!!
    this->frames.resize(this->NUM_FRAMES);
}

void Simulation::run() {
    // init free frames, can be done by looping over numver of frame and init free_frames vector
    // loop over virtual_addresses
      // perform_memory_access on each virtual address
      // increment time in loop
    // if should exit is true then print summary_fmt
    // loop over process and delete the internal pages and the processes

  for (int i = 0; i < NUM_FRAMES; i++) {
    this->frames.push_back(Frame());
    this->free_frames.push_back(i);
  }

  for (auto & virtual_addre : this->virtual_addresses) {
    if (perform_memory_access(virtual_addre) == 1) {
      should_exit = true;
      break;
    }
  }

  if(should_exit == false){
    this->print_summary();
  }

  for (auto entry: this->processes)
  {
    for (auto page :  entry.second->pages) {
      delete(page);
    }
    delete(entry.second);
  }
}

char Simulation::perform_memory_access(const VirtualAddress& virtual_address) {
  // check if flag.verbose is set
  // in the case that it is print out the virtual adress
  //
  // check to make sure the page is valid, if it is good to go, if not exit segfault
  //
  // grab phyiscal address accessing virtual address in page table
  // check if page is valid if it is perform memeory accesses
  // otherwise perform replacement algorithm
  //
  // grab the data from the frame based on the page (keep track of verbose)

  Process *process =  this->processes[virtual_address.process_id];
  int page_Number = virtual_address.page;
  bool page_valid = process->is_valid_page(page_Number);
  bool page_present = process->page_table.rows[page_Number].present;

  if (this->flags.verbose == true) {
    std::cout << virtual_address << std::endl;
  }

  process->page_table.rows[page_Number].last_accessed_at = this->TIME++;

  process->memory_accesses++;

  if (page_valid == false) {
    std::cout << "SEGFAULT - INVALID PAGE" << std::endl;
    return 1;
  }
  if (page_present == false) {
    std::cout << "    -> PAGE FAULT" << std::endl;
    handle_page_fault(process, page_Number);
  }
  else {
    std::cout << "    -> IN MEMORY" << std::endl;
  }

  int frame_Number = process->page_table.rows[page_Number].frame;
  int offset = virtual_address.offset;

  std::cout << "    -> physical address " << std::bitset<10>(frame_Number) << std::bitset<6>(offset) << " [frame: " << frame_Number << "; offset: " << offset << "]\n";
  if (process->pages[page_Number]->is_valid_offset(offset) == false) {
    std::cout << "SEGFAULT - INVALID OFFSET\n";
    return 1;
  }
  std::cout << "    -> RSS: " << process->get_rss() << "\n\n";
  return 0;
}

void Simulation::handle_page_fault(Process* process, size_t page) {
    // based on LRU or FIFO AND if the the frame is free
    // if your doing FIFO, access based on oldset page table
    // if your doing LRU access based on most recently used
    // increment time at end

  if (process->get_rss() < this->flags.max_frames) {
    frames[free_frames.front()].set_page(process, page);
    process->page_table.rows[page].frame = free_frames.front();
    free_frames.pop_front();
  } else {
    int replaced_page_number = 0;
    if(this->flags.strategy == ReplacementStrategy::FIFO){
      replaced_page_number = process->page_table.get_oldest_page();
    }
    else{
      replaced_page_number = process->page_table.get_least_recently_used_page();
    }
    process->page_table.rows[replaced_page_number].present = false;
    process->page_table.rows[replaced_page_number].loaded_at = -1;
    process->page_table.rows[page].frame = process->page_table.rows[replaced_page_number].frame;
  }
  process->page_table.rows[page].present = true;
  process->page_table.rows[page].loaded_at = process->memory_accesses;
  process->page_faults++;
  this->page_faults++;
}

void Simulation::print_summary() {
    if (!this->flags.csv) {
        boost::format process_fmt(
            "Process %3d:  "
            "ACCESSES: %-6lu "
            "FAULTS: %-6lu "
            "FAULT RATE: %-8.2f "
            "RSS: %-6lu\n");

        for (auto entry : this->processes) {
            std::cout << process_fmt
                % entry.first
                % entry.second->memory_accesses
                % entry.second->page_faults
                % entry.second->get_fault_percent()
                % entry.second->get_rss();
        }

        // Print statistics.
        boost::format summary_fmt(
            "\n%-25s %12lu\n"
            "%-25s %12lu\n"
            "%-25s %12lu\n");

        std::cout << summary_fmt
            % "Total memory accesses:"
            % this->virtual_addresses.size()
            % "Total page faults:"
            % this->page_faults
            % "Free frames remaining:"
            % this->free_frames.size();
    }

    if (this->flags.csv) {
        boost::format process_fmt(
            "%d,"
            "%lu,"
            "%lu,"
            "%.2f,"
            "%lu\n");

        for (auto entry : processes) {
            std::cout << process_fmt
                % entry.first
                % entry.second->memory_accesses
                % entry.second->page_faults
                % entry.second->get_fault_percent()
                % entry.second->get_rss();
        }

        // Print statistics.
        boost::format summary_fmt(
            "%lu,,,,\n"
            "%lu,,,,\n"
            "%lu,,,,\n");

        std::cout << summary_fmt
            % this->virtual_addresses.size()
            % this->page_faults
            % this->free_frames.size();
    }
}

int Simulation::read_processes(std::istream& simulation_file) {
    int num_processes;
    simulation_file >> num_processes;

    for (int i = 0; i < num_processes; ++i) {
        int pid;
        std::string process_image_path;

        simulation_file >> pid >> process_image_path;

        std::ifstream proc_img_file(process_image_path);

        if (!proc_img_file) {
            std::cerr << "Unable to read file for PID " << pid << ": " << process_image_path << std::endl;
            return 1;
        }
        this->processes[pid] = Process::read_from_input(proc_img_file);
    }
    return 0;
}

int Simulation::read_addresses(std::istream& simulation_file) {
    int pid;
    std::string virtual_address;

    try {
        while (simulation_file >> pid >> virtual_address) {
            this->virtual_addresses.push_back(VirtualAddress::from_string(pid, virtual_address));
        }
    } catch (const std::exception& except) {
        std::cerr << "Error reading virtual addresses." << std::endl;
        std::cerr << except.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Error reading virtual addresses." << std::endl;
        return 1;
    }
    return 0;
}

int Simulation::read_simulation_file() {
    std::ifstream simulation_file(this->flags.filename);
    // this->simulation_file.open(this->flags.filename);

    if (!simulation_file) {
        std::cerr << "Unable to open file: " << this->flags.filename << std::endl;
        return -1;
    }
    int error = 0;
    error = this->read_processes(simulation_file);

    if (error) {
        std::cerr << "Error reading processes. Exit: " << error << std::endl;
        return error;
    }

    error = this->read_addresses(simulation_file);

    if (error) {
        std::cerr << "Error reading addresses." << std::endl;
        return error;
    }

    if (this->flags.file_verbose) {
        for (auto entry: this->processes) {
            std::cout << "Process " << entry.first << ": Size: " << entry.second->size() << std::endl;
        }

        for (auto entry : this->virtual_addresses) {
            std::cout << entry << std::endl;
        }
    }

    return 0;
}
