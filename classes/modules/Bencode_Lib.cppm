//
// File: Bencode_Lib.cppm
//
// Description: Primary C++23 module interface unit for Bencode_Lib.
// Provides `import Bencode_Lib;` for modern C++20/C++23 toolchains.
//

module;

#include "Bencode_Config.hpp"

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Status.hpp"
#include "Bencode_View.hpp"
#include "Bencode_Serialization.hpp"
#include "interface/ISaxHandler.hpp"
#include "interface/IParser.hpp"
#include "interface/IStringify.hpp"
#include "interface/IAction.hpp"

export module Bencode_Lib;

export namespace Bencode_Lib {
    // -------------------------------------------------------------
    // Core Facades & Document AST
    // -------------------------------------------------------------
    using Bencode_Lib::Bencode;
    using Bencode_Lib::BencodeView;

    // -------------------------------------------------------------
    // AST Nodes & Views
    // -------------------------------------------------------------
    using Bencode_Lib::Node;
    using Bencode_Lib::NodeView;
    using Bencode_Lib::DictEntry;
    using Bencode_Lib::Dictionary;
    using Bencode_Lib::List;
    using Bencode_Lib::Integer;
    using Bencode_Lib::String;

    // -------------------------------------------------------------
    // Streaming Interfaces & Protocols
    // -------------------------------------------------------------
    using Bencode_Lib::ISaxHandler;
    using Bencode_Lib::IParser;
    using Bencode_Lib::IStringify;
    using Bencode_Lib::IAction;

    // -------------------------------------------------------------
    // Error Handling & Status Codes
    // -------------------------------------------------------------
    using Bencode_Lib::ParseStatus;
    using Bencode_Lib::Error;
    using Bencode_Lib::SyntaxError;

    // -------------------------------------------------------------
    // I/O Abstractions
    // -------------------------------------------------------------
    using Bencode_Lib::ISource;
    using Bencode_Lib::IDestination;
    using Bencode_Lib::BufferSource;
    using Bencode_Lib::BufferDestination;
#if BENCODE_ENABLE_FILE_IO
    using Bencode_Lib::FileSource;
    using Bencode_Lib::FileDestination;
#endif

    // -------------------------------------------------------------
    // C++23 Serialization Customization Points & Concepts
    // -------------------------------------------------------------
    using Bencode_Lib::BencodeSerializable;
    using Bencode_Lib::BencodeDeserializable;
    using Bencode_Lib::ViewBencodeDeserializable;

    // Serialization & Deserialization hooks
    using Bencode_Lib::to_bencode;
    using Bencode_Lib::from_bencode;
    using Bencode_Lib::to_bencode_field;
    using Bencode_Lib::from_bencode_field;

    namespace detail {
        using Bencode_Lib::detail::to_bencode_adl;
        using Bencode_Lib::detail::from_bencode_adl;
    }
}
