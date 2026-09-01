# SOLID Architecture Refactoring Plan for Bencode_Lib

## Overview
This document outlines a concrete, comprehensive refactor plan for `Bencode_Lib` to achieve full compliance with SOLID principles:
- **S**ingle Responsibility Principle (SRP)
- **O**pen/Closed Principle (OCP)
- **L**iskov Substitution Principle (LSP)
- **I**nterface Segregation Principle (ISP)
- **D**ependency Inversion Principle (DIP)

Currently, `Bencode_Lib` features clean C++20 design patterns, but several architectural cross-cutting concerns break SOLID rules—such as static mutable translator pointers, static parser depth configuration, multi-responsibility facade classes, over-specified stream interfaces (`IDestination::last()`), and `if-else` type-branching during node traversal instead of visitor pattern dispatch.

---

## SOLID Violations Audit & Proposed Architecture

### 1. Single Responsibility Principle (SRP)
- **Violation in `Bencode` & `Bencode_Impl`**:
  - `Bencode` / `Bencode_Impl` manages Node tree root state, dictionary/list indexing (`operator[]`), tree traversal dispatch, parse/stringify delegation, **AND** direct filesystem I/O operations (`fromFile`, `toFile`).
  - **Refactor**: Extract file I/O helper methods into a standalone `Bencode_FileIO` service class (`Bencode_FileService`). Keep `Bencode` focused purely on DOM container management and delegation.
- **Violation in `Default_Parser`**:
  - Combines parsing state logic with global configuration management (`setMaxParserDepth`, `getMaxParserDepth`).
  - **Refactor**: Extract `ParserConfig` / `ParserOptions` struct and pass config by instance to `Default_Parser` constructor.
- **Violation in Stringifiers (`JSON_Stringify`, `XML_Stringify`, `YAML_Stringify`)**:
  - Mix stringification logic with global static translator state (`inline static std::unique_ptr<ITranslator>`).
  - `YAML_Stringify` calculates indentation while tracking previous characters.
  - **Refactor**: Move translators to instance-level member variables. Separate line tracking from serializers using a decorator/adapter pattern.

### 2. Open/Closed Principle (OCP)
- **Violation in Node Traversal & Serialization**:
  - `Bencode_Impl::traverseNodes`, `Default_Stringify::stringifyNodes`, `JSON_Stringify::stringifyNodes`, `XML_Stringify::stringifyNodes`, and `YAML_Stringify::stringifyNodes` rely on `if (isA<Dictionary>(bNode)) ... else if (isA<List>...)` type checks over `std::variant`.
  - Adding any new type variant requires modifying 5+ serializer/traverser implementation files.
- **Refactor**:
  - Introduce `INodeVisitor` (or `std::visit` visitor templates) for type-safe polymorphic dispatch across Node types.
  - Make stringifiers open for extension by allowing custom Node formatters/visitors to be plugged in.

### 3. Liskov Substitution Principle (LSP)
- **Violation in `IParser` Exception vs Non-Exception Contract**:
  - Conditional `#if BENCODE_ENABLE_EXCEPTIONS` changes the signature of `virtual parseImpl(...)` in `IParser`, shifting method behavior and interface contracts at compile time.
  - **Refactor**: Standardize `IParser` interface methods to provide consistent semantics regardless of build configuration flags.
- **Violation in `FileSource` Observer Semantics**:
  - `FileSource::current()` calls `ensurePeek()`, mutating internal state (`mutable int currentChar`, `mutable bool hasPeek`) during const observer calls.
  - **Refactor**: Ensure stream readers maintain pure observer idempotency without fragile mutable state transitions during `current()`.

### 4. Interface Segregation Principle (ISP)
- **Violation in `IDestination` Interface**:
  - `IDestination` forces all implementations to implement `last()` (returning the last written character). `last()` is a leaky abstraction required only by `YAML_Stringify` for line tracking.
  - Forces 4 redundant `add()` overloads (`std::string`, `std::string_view`, `const char*`, `char`).
  - **Refactor**:
    - Remove `last()` from `IDestination`. Introduce a `TrackingDestinationAdapter` decorator in `YAML_Stringify` to track line state independently.
    - Simplify `IDestination::add` to rely primarily on `std::string_view` and `char`.
- **Violation in `IAction` Interface**:
  - Monolithic interface with 10 virtual methods forcing callers to depend on unused node types.
  - **Refactor**: Split `IAction` into fine-grained callbacks or an `INodeVisitor` interface with default empty implementations.
- **Violation in `ISource` Interface**:
  - Combines stream reading with `reset()`. Non-seekable stream sources cannot implement `reset()`.
  - **Refactor**: Move `reset()` to `IResettableSource` sub-interface.

### 5. Dependency Inversion Principle (DIP)
- **Violation in Static Translator Dependencies**:
  - `JSON_Stringify`, `XML_Stringify`, `YAML_Stringify` use static mutable pointers (`inline static std::unique_ptr<ITranslator>`).
  - **Refactor**: Inject `std::unique_ptr<ITranslator>` or `const ITranslator&` into constructors as instance dependencies.
- **Violation in Global Parser Options**:
  - `Default_Parser` depends on static class member `maxParserDepth`.
  - **Refactor**: High-level modules inject `ParserConfig` / `ParserOptions` objects into `Default_Parser`.

---

## User Review Required

> [!IMPORTANT]
> **API Compatibility & Deprecations**
> 1. Static methods `Bencode::fromFile` and `Bencode::toFile` will be retained as forwarding wrappers to `Bencode_FileService` for backward compatibility, while internal implementations are decoupled.
> 2. `IDestination::last()` will be removed from the core interface and replaced with a `TrackingDestinationAdapter` used inside `YAML_Stringify`. If external users implemented custom `IDestination` classes, `last()` will no longer be required.

---

## Proposed Changes

---

### Component 1: Interfaces & Data Sinks (`classes/include/interface/`)

#### [MODIFY] [IDestination.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/interface/IDestination.hpp)
- Remove `virtual char last() = 0;`.
- Streamline `add()` overloads to `virtual void add(std::string_view bytes) = 0;` and `virtual void add(char ch) = 0;`.

#### [MODIFY] [ISource.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/interface/ISource.hpp)
- Keep `current()`, `next()`, `more()`.
- Move `reset()` into `IResettableSource` sub-interface or default non-pure virtual implementation.

#### [MODIFY] [IAction.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/interface/IAction.hpp)
- Refactor to provide `INodeVisitor` / fine-grained observer callbacks with optional default handlers.

---

### Component 2: Serializers & Translators (`classes/include/implementation/stringify/`)

#### [MODIFY] [JSON_Stringify.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/implementation/stringify/JSON_Stringify.hpp)
- Remove `inline static std::unique_ptr<ITranslator> jsonTranslator;`.
- Store `std::unique_ptr<ITranslator> jsonTranslator` as instance member.
- Replace `if-else` type branches in `stringifyNodes` with visitor pattern / `std::visit`.

#### [MODIFY] [XML_Stringify.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/implementation/stringify/XML_Stringify.hpp)
- Remove `inline static std::unique_ptr<ITranslator> xmlTranslator;`.
- Store `std::unique_ptr<ITranslator> xmlTranslator` as instance member.
- Replace `if-else` type branches in `stringifyNodes` with visitor pattern.

#### [MODIFY] [YAML_Stringify.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/implementation/stringify/YAML_Stringify.hpp)
- Remove `inline static std::unique_ptr<ITranslator> yamlTranslator;`.
- Store `std::unique_ptr<ITranslator> yamlTranslator` as instance member.
- Implement `TrackingDestinationAdapter` decorator inside `YAML_Stringify` to track `last()` character locally instead of requiring it on `IDestination`.

#### [MODIFY] [Default_Stringify.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/implementation/stringify/Default_Stringify.hpp)
- Refactor `stringifyNodes` to use visitor pattern dispatch.

---

### Component 3: Parser & Configuration (`classes/include/implementation/parser/`)

#### [NEW] [Parser_Config.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/implementation/parser/Parser_Config.hpp)
- Define `ParserConfig` struct containing `maxParserDepth` and other parser options.

#### [MODIFY] [Default_Parser.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/implementation/parser/Default_Parser.hpp)
- Remove `inline static unsigned long maxParserDepth`.
- Accept `ParserConfig` in constructor and store as instance field.

---

### Component 4: Core Implementation & File I/O (`classes/include/implementation/` & `classes/source/`)

#### [NEW] [Bencode_FileService.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/implementation/io/Bencode_FileService.hpp)
- Separate file I/O operations (`fromFile`, `toFile`) from `Bencode_Impl` into dedicated service class `Bencode_FileService`.

#### [MODIFY] [Bencode_Impl.hpp](file:///home/robt/projects/Bencode_Lib-1/classes/include/implementation/Bencode_Impl.hpp) & [Bencode_Impl.cpp](file:///home/robt/projects/Bencode_Lib-1/classes/source/implementation/Bencode_Impl.cpp)
- Delegate `fromFile` and `toFile` calls to `Bencode_FileService`.
- Replace `traverseNodes` type checks with visitor pattern dispatch.

---

## Verification Plan

### Automated Tests
1. **Unit Tests Compilation & Execution**:
   - Run standard build and Catch2 unit test suite:
     ```bash
     cmake -B build -S .
     cmake --build build
     ./build/tests/Bencode_Lib_Unit_Tests
     ```
2. **Multi-threading & Thread Safety Test**:
   - Verify that eliminating static `jsonTranslator`/`xmlTranslator`/`yamlTranslator` pointers allows thread-safe concurrent stringification.
3. **All 64 Catch2 Test Cases**:
   - Ensure all 1031+ assertions continue to pass without regressions.

### Manual Verification
- Code review verification against each of the 5 SOLID principles.
