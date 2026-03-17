# Bencode Library Standard Compliance Analysis

## Summary
The Bencode_Lib C++20 library implements the Bencode serialization format, commonly used in BitTorrent. While it supports standard Bencode types (integer, string, list, dictionary), it also introduces several non-standard behaviors and extensions.

## Deviations from Bencode Standard

### 1. Non-standard Types Supported
- The library accepts and encodes types not defined in the Bencode specification:
  - Floating-point numbers (`float`, `double`, `long double`): converted to integer by truncation.
  - Boolean values: converted to integer (`true` → `1`, `false` → `0`).
  - Null values (`nullptr`): encoded as integer zero (`i0e`).
  - `hole` variant: internal type not present in the Bencode standard.

### 2. Type Conversion Behavior
- Floating-point values are truncated to integer.
- Boolean values are mapped to integer.
- Null values are mapped to integer zero.

### 3. Initializer List Support
- The library allows C++ initializer lists for constructing Bencode objects, which is a convenience feature but not part of the standard.

## Compliance Recommendations
- For strict Bencode compliance (BitTorrent interoperability), use only integer, byte string, list, and dictionary types.
- Avoid using floats, bools, or nulls in encoded data.

## References
- [Bencode Specification - Wikipedia](https://en.wikipedia.org/wiki/Bencode)
- See README.md for library features and usage.

---
Generated on March 17, 2026.