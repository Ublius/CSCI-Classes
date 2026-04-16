/**
 * This file contains implementations for methods in the PageTable class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */
#include <climits>
#include "page_table/page_table.h"

using namespace std;


size_t PageTable::get_present_page_count() const {
  size_t page_count = 0;
  for (size_t i = 0; i < this->rows.size(); i++) {
      if (this->rows[i].present == true) {
          page_count++;
      }
  }
return page_count;
}


size_t PageTable::get_oldest_page() const {
  size_t index = 0;
  size_t max_time = INT_MAX;
  for (size_t i = 0; i < this->rows.size(); i++) {
      if (this->rows[i].present == true && this->rows[i].loaded_at < max_time) {
          max_time = this->rows[i].loaded_at;
          index = i;
      }
  }
  return index;
}


size_t PageTable::get_least_recently_used_page() const {
  int index = 0;
  int access_time = INT_MAX;
  for (size_t i = 0; i < this->rows.size(); i++) {
      if (this->rows[i].present == true && this->rows[i].last_accessed_at < access_time) {
        access_time = this->rows[i].last_accessed_at;
          index = i;
      }
  }
  return index;
}
