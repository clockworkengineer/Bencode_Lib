# Bencode_Lib

A library to perform Bencode stringify/parse to/from
a byte buffer, file or a custom source/destination with the
ISource and IDestination interfaces provided. 

At present, it will report incorrect Bencode syntax but will not be
specific about what error has occurred. This is reasoned to add too 
much overhead to the process of parsing for the requirements of this 
library. 

The library can both interrogate the BNode tree structure created after
decoding a bencode source but also modify it in-situ and also create one
from scratch using C++ operators before saving it away (encoding).

For an in-depth description of the Bencode specification, refer 
to its wiki page at https://en.wikipedia.org/wiki/Bencode.
