# BencodeLib

A library to perform Bencode encoding encode/decode to/from
a byte buffer or file. It is also  possible to customize this with the
ISource and IDestination interfaces if required. Although bencoded
data is treated as std::byte externally this library uses char and
std::string internally.Note: At present it will report incorrect Bencode
syntax but will not be specific about what error has occurred; this
is reasoned to add too much overhead to the process of parsing for the
requirements of this library (this might change in future versions).
For an indepth desciption of the Bencoding specification refer to its
wiki page at https://en.wikipedia.org/wiki/Bencode.