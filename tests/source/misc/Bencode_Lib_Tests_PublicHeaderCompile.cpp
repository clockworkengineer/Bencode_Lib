#include "Bencode.hpp"
#include "Bencode_Core.hpp"

int main() {
  Bencode_Lib::Bencode b;
  Bencode_Lib::BufferSource source("i42e");
#if BENCODE_ENABLE_EXCEPTIONS
  b.parse(source);
#else
  const auto status = b.parse(source);
  (void)status;
#endif
  return 0;
}
