#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Status.hpp"
#include "Bencode_Optional_Stringify.hpp"

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
