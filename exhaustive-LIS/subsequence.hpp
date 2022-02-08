///////////////////////////////////////////////////////////////////////////////
// subsequence.hpp
//
// An exhaustive optimization algorithm for solving 
// the longest increasing subsequence problem.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <sstream>
#include <vector>

using sequence = std::vector<int>;

// Convert a sequence into a human-readable string useful for pretty-printing
// or debugging.
std::string sequence_to_string(const sequence& seq) {
  std::stringstream ss;
  ss << "[";
  bool first = true;
  for (auto& x : seq) {
    if (!first) {
      ss << ", ";
    }
    ss << x;
    first = false;
  }
  ss << "]";
  return ss.str();
}

// Generate a pseudorandom sequence of the given size, using the given
// seed, where all elements are in the range [0, max_element]. max_element
// must be non-negative.
sequence random_sequence(size_t size, unsigned seed, int max_element) {

    assert(max_element >= 0);

    sequence result;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(0, max_element);

    for (size_t i = 0; i < size; ++i) {
        result.push_back(dist(gen));
    }

    return result;
}

bool is_increasing(const sequence& A) {
  for (size_t i = 1; i < A.size(); ++i) {   // iterates through the elements of A

      if (A[i] <= A[i-1]){    // if any element is less than or equal to its previous element, the condition no longer holds
        return false;
      }
  }
  return true;
}

sequence longest_increasing_powerset(const sequence& A) {
  const size_t n = A.size();
  sequence best;  // initally sequence{0}
  std::vector<size_t> stack(n+1, 0);
  size_t k = 0;
  while (true) {  // runs 2^n times generating each member of the powerset

    if (stack[k] < n) {
      stack[k+1] = stack[k] + 1;
      ++k;
    } else {
      stack[k-1]++;
      k--;
    }

    if (k == 0) {   // will only reach this when the algorithm is finished
      break;
    }

    sequence candidate;
    for (size_t i = 1; i <= k; ++i) {   // each element of the current set is pasted into the candidate
      candidate.push_back(A[stack[i]-1]);
    }

    if (is_increasing(candidate))   // verifier
    {
      if (best == sequence{0} || candidate.size() > best.size())  // optimization comparison
      {
        best = candidate;   // replace with the current longest subsequence
      }
    }
    }

  return best;  // all possible subsequences within the sequence have been analyzed
}
