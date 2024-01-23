#include <iostream>
#include <vector>
#include <future>

template<typename It,typename func, size_t size = 5>
void mult_for_each(It begin, It end, func f) {
  auto current_size = std::distance(begin, end);
  if (current_size <= size) {
    std::for_each(begin, end, f);
  }
  else {
    auto mid = begin;
    std::advance(mid, current_size/2);
    auto ft_res = std::async(mult_for_each<It, func>, mid, end, f);
    auto lf_res = std::for_each(begin, mid, f);
  }
}

int main() {
    std::vector<int> vec;
    for (int i{}; i < 100; ++i) {
      vec.push_back(i);
    }
    mult_for_each(vec.begin(), vec.end(), [](int temp){std::cout << temp << " ";});
}