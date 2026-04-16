/**
 * This file contains implementations for methods in the VirtualAddress class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "virtual_address/virtual_address.h"

using namespace std;

VirtualAddress VirtualAddress::from_string(int process_id, string address) {
  VirtualAddress virtual_address = VirtualAddress(process_id, stoi(address.substr(0, PAGE_BITS), nullptr, 2), stoi(address.substr(PAGE_BITS, ADDRESS_BITS), nullptr, 2));
  return virtual_address;
}


string VirtualAddress::to_string() const {
  return bitset<PAGE_BITS>(this->page).to_string() + bitset<OFFSET_BITS>(this->offset).to_string();
}


ostream& operator <<(ostream& out, const VirtualAddress& address) {
  return out << "PID " << std::to_string(address.process_id)  << " @ " << address.to_string() << " [page: " << std::to_string(address.page) << "; offset: " << std::to_string(address.offset) << "]";
}
