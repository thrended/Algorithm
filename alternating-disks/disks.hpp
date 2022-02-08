///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(light_count > 0);

      for (size_t i = 0; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is dark, the second disk at index 1
  // is light, and so on for the entire row of disks.
  bool is_alternating() const {
    for (size_t i = 0; i < _colors.size(); i += 2) {
        if (!(_colors[i] == DISK_DARK && _colors[i+1] == DISK_LIGHT)) return false;     // every 2 disks must follow the same light-dark pattern as the previous 2
    }
    return true;    // alternating = true if it makes it to the end of the check
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the left (low indices) and all dark disks on the right (high
  // indices).
  bool is_sorted() const {
    if (_colors.size() % 2 ){   // odd number of disks cannot be sorted
      return false;
    }
    auto n = _colors.size();
    for (size_t i = 0; i < n/2; i ++) 
    {
        if (_colors[i] == DISK_DARK || _colors[i + n/2] == DISK_LIGHT) return false;    // the first n/2 disks must be light, and second n/2 disks must be dark
    }
    return true;    // sorted = true if it makes it to the end of the check
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
  disk_state L = before;
  const auto n = L.total_count();
  auto nswaps = 0;
  std::clock_t start;
  double duration;

  start = std::clock();
  for (size_t k = 1; k < n; k++)      // runs up to as many times as there are disks
  {
    bool swapped = false;
    for (size_t i = 0; i < n-k; i++)      // range of indices that could be unsorted shrinks as the sorting progresses
    {
        if (L.get(i) == DISK_DARK && L.get(i+1) == DISK_LIGHT) {      // move light disks toward the left and dark disks toward the right as needed
          L.swap(i);
          swapped = true;
          nswaps++;     // swap counter increment
        }
    }
    if (!swapped) {
      duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

      std::cout<< n << " disks: "<< duration << " seconds" <<'\n';
      return sorted_disks(L, nswaps);     // early exit if no swaps were made in a loop cycle (already sorted)
    }
  }
  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  std::cout<< n <<" disks:"<< duration << " seconds" <<'\n';
  return sorted_disks(L, nswaps);     // return the sorted disks and swap counter
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  disk_state L = before;
  const auto n = L.total_count();
  auto nswaps = 0;
  std::clock_t start;
  double duration;

  start = std::clock();
  for (size_t k = 1; k < n/2; k++)      // runs up to (n/2 - 1) times
  {
    bool swapped = false;
    for (size_t i = 0; i < n - 2*k + 1; i++)      // lawnmower going left to right; range of indices that could be unsorted shrinks as the sorting progresses
    {
        if (L.get(i) == DISK_DARK && L.get(i+1) == DISK_LIGHT) {      // move light disks toward the left and dark disks toward the right as needed
          L.swap(i);
          swapped = true;
          nswaps++;     // swap counter increment
        }
    }
    if (!swapped) {
      duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

      std::cout<< n << " disks: "<< duration << " seconds" <<'\n';
      return sorted_disks(L, nswaps);
    }
    for (size_t i = n - 2*k; i > 1; i--)          // lawnmower going right to left; range of indices that could be unsorted shrinks as the sorting progresses
    {
        if (L.get(i-1) == DISK_DARK && L.get(i) == DISK_LIGHT) {      // move light disks toward the left and dark disks toward the right as needed
          L.swap(i-1);
          swapped = true;
          nswaps++;     // swap counter increment
        }
    }
    if (!swapped) {
      duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

      std::cout<< n << " disks: "<< duration << " seconds" <<'\n';
      return sorted_disks(L, nswaps);     // early exit if no swaps were made in a loop cycle (already sorted)
    }
  }
  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  std::cout<< n << " disks: "<< duration << " seconds" <<'\n';
  return sorted_disks(L, nswaps);     // return the sorted disks and swap counter
}
