//
// Program: Bencode_Create_At_Runtime
//
// Description: Create Bencode programatically at runtime using a combination
// of the Bencode class, indexing/key operators plus the use of intializer
// lists.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Utility.hpp"

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Sources.hpp"
#include "Bencode_Destinations.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "Bencode_Create_At_Runtime.log");
    PLOG_INFO << "Bencode_Create_At_Runtime started ...";
    // Log version
    PLOG_INFO << be::Bencode().version();
    // create an empty structure (null)
    be::Bencode bEncode;
    // add a number that is stored as double (note the implicit conversion of
    // bEncode to an object)
    bEncode["pi"] = 3.141;
    // add a Boolean that is stored as bool
    bEncode["sad"] = true;
    // add a string that is stored as std::string
    bEncode["first_name"] = "Niels";
    // add another null object by passing nullptr
    bEncode["nothing"] = nullptr;
    // add an object inside the object
    bEncode["the_answer"]["everything"] = 42;
    // add an array that is stored as std::vector (using an initializer list)
    bEncode["list"] = {1, 0, 2};
    // add another object (using an initializer list of pairs)
    bEncode["object"] = {{"currency", "USD"}, {"value", 42.99}};
    // add another object that has a nested array (BNode{})
    bEncode["object2"] = {{"currency", "USD"},
                          {"array", be::BNode{23.22, 33, 55, 99.99}}};
    be::BufferDestination destination;
    bEncode.encode(destination);
    PLOG_INFO << Utility::bufferToString(destination.getBuffer());
    destination.clear();
    // create Bencode using an intialiser list and nesting array/objects using
    // BNode{}.
    be::Bencode bEncode2 = {
        {"pi", 3.141},
        {"sad", true},
        {"first_name", "Niels"},
        {"nothing", nullptr},
        {"the_answer", be::BNode{{"everything", 42}}},
        {"list", be::BNode{1, 0, 2}},
        {"object", be::BNode{{"currency", "USD"},
                             {"value", be::BNode{23.22, 33, 55, 99.99}}}}};
    bEncode2.encode(destination);
    PLOG_INFO << Utility::bufferToString(destination.getBuffer());
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "Bencode_Create_At_Runtime exited.";
  exit(EXIT_SUCCESS);
}