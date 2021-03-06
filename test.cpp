#include <cassert>
#include "monad.h"

using std::cout;
using namespace monad;

Maybe<int> half(int x) {
  if (x != 0) {
    return x / 2;
  } else {
    return Nothing;
  }
}

int main() {
  auto a = Maybe<int>(20) | half;
  assert(*a == 10);
  cout << "Test 1 passed\n";
  a = a | half | half | half | half | half;
  assert(!a);
  cout << "Test 2 passed\n";
  return 0;
}
