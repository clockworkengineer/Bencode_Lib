//
// Program: Bencode_Create_At_Runtime
//
// Description: Create Bencode programmatically at runtime using a combination
// of the Bencode class, indexing/key operators plus the use of initializer
// lists.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Bencode_Utility.hpp"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    init(plog::debug, "Bencode_Create_At_Runtime.log");
    PLOG_INFO << "Bencode_Create_At_Runtime started ...";
    // Log version
    PLOG_INFO << be::Bencode::version();
    // create an empty structure (null)
    be::Bencode bStringify;
    // add a number that is stored as double (note the implicit conversion of
    // bStringify to an object)
    bStringify["pi"] = 3.141;
    // add a Boolean that is stored as bool
    bStringify["sad"] = true;
    // add a string that is stored as std::string
    bStringify["first_name"] = "Niels";
    // add another null object by passing nullptr
    bStringify["nothing"] = nullptr;
    // add an object inside the object
    bStringify["the_answer"]["everything"] = 42;
    // add an array that is stored as std::vector (using an initializer list)
    bStringify["list"] = {1, 0, 2};
    // add another object (using an initializer list of pairs)
    bStringify["object"] = {{"currency", "USD"}, {"value", 42.99}};
    // add another object that has a nested array (Node{})
    bStringify["object2"] = {{"currency", "USD"},
                          {"array", be::Node{23.22, 33, 55, 99.99}}};
    be::BufferDestination destination;
    bStringify.stringify(destination);
    PLOG_INFO << destination.toString();
    destination.clear();
    // create Bencode using an initializer list and nesting array/objects using
    // Node{}.
    const be::Bencode bStringify2 = {
        {"pi", 3.141},
        {"sad", true},
        {"first_name", "Niels"},
        {"nothing", nullptr},
        {"the_answer", be::Node{{"everything", 42}}},
        {"list", be::Node{1, 0, 2}},
        {"object", be::Node{{"currency", "USD"},
                             {"value", be::Node{23.22, 33, 55, 99.99}}}}};
    bStringify2.stringify(destination);
    PLOG_INFO << destination.toString();
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "Bencode_Create_At_Runtime exited.";
  exit(EXIT_SUCCESS);
}