/**
 * This file contains implementations for methods in the Process class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "process/process.h"

using namespace std;


Process* Process::read_from_input(std::istream& in) {
  Page* page;
  vector<Page*> pages;
  size_t num_bytes = 0;
  while ((page = Page::read_from_input(in)) != nullptr){
    pages.push_back(page);
    num_bytes += page->size();
  }
  return new Process(num_bytes, pages);
}

size_t Process::size() const
{
  return this->num_bytes;
}

bool Process::is_valid_page(size_t index) const
{
  return index < pages.size() ? true : false;
}

size_t Process::get_rss() const
{
  size_t rss = 0;
  for (size_t i = 0; i < this->page_table.rows.size(); ++i) {
      if (this->page_table.rows[i].present == true) {
          ++rss;
      }
  }
  return rss;
}

double Process::get_fault_percent() const
{
  if (this->memory_accesses > 0.0) {
    return (double)this->page_faults / this->memory_accesses * 100;
  }
  return 0.0;
}
