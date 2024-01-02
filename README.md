# Bencode_Lib

A library to perform Bencode encoding encode/decode to/from
a byte buffer, file or a custom source/destination with the
ISource and IDestination interfaces provided. Although bencoded 
data is treated as std::byte externally this library uses char 
and std::string internally.

At present it will report incorrect Bencode syntax but will not be
specific about what error has occurred.This is reasoned to add too 
much overhead to the process of parsing for the requirements of this 
library. 

The library can both interrogate the BNode tree structure created after
decoding an bencode source but also modify it in-situ and also create one
from scratch using C++ operators before saving it away (encoding).

The examples folder only contains 

Bencode_Read_Torrent_Files - a program to read in and decode 
torrent files as so far this has been the only practical use the 
author has found for the use of Bencoded data.

Bencode_Torrent_Files_To_JSON - a program to read in and decode torrent files
and encode the tree produced to an JSON file.

Bencode_Torrent_Files_To_XML - a program to read in and decode torrent files
and encode the tree produced to an XML file.

For an in-depth description of the Bencode specification refer 
to its wiki page at https://en.wikipedia.org/wiki/Bencode.