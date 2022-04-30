# BencodeLib

A library to perform Bencode encoding encode/decode to/from
a byte buffer, file or a custom source/destination with the
ISource and IDestination interfaces provided. Although bencoded 
data is treated as std::byte externally this library uses char 
and std::string internally.

At present it will report incorrect Bencode syntax but will not be
specific about what error has occurred.This is reasoned to add too 
much overhead to the process of parsing for the requirements of this 
library.

For an indepth desciption of the Bencoding specification refer 
to its wiki page at https://en.wikipedia.org/wiki/Bencode.