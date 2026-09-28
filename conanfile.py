from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy
import os

class BencodeLibConan(ConanFile):
    name = "bencode_lib"
    version = "1.3.0"
    license = "MIT"
    author = "Bencode_Lib Contributors"
    url = "https://github.com/clockworkengineer/Bencode_Lib"
    description = "High-performance, modern C++23 Bencode encoding and decoding library"
    topics = ("bencode", "bittorrent", "serialization", "parser", "cpp23")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_file_io": [True, False],
        "enable_exceptions": [True, False],
        "enable_json_stringify": [True, False],
        "enable_xml_stringify": [True, False],
        "enable_yaml_stringify": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "enable_file_io": True,
        "enable_exceptions": True,
        "enable_json_stringify": True,
        "enable_xml_stringify": True,
        "enable_yaml_stringify": True
    }

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BENCODE_ENABLE_FILE_IO"] = self.options.enable_file_io
        tc.variables["BENCODE_ENABLE_EXCEPTIONS"] = self.options.enable_exceptions
        tc.variables["BENCODE_ENABLE_JSON_STRINGIFY"] = self.options.enable_json_stringify
        tc.variables["BENCODE_ENABLE_XML_STRINGIFY"] = self.options.enable_xml_stringify
        tc.variables["BENCODE_ENABLE_YAML_STRINGIFY"] = self.options.enable_yaml_stringify
        tc.variables["BENCODE_BUILD_TESTS"] = False
        tc.variables["BENCODE_BUILD_EXAMPLES"] = False
        tc.variables["BENCODE_BUILD_BENCHMARKS"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE.txt", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["Bencode_Lib"]
        self.cpp_info.set_property("cmake_file_name", "Bencode_Lib")
        self.cpp_info.set_property("cmake_target_name", "Bencode_Lib::Bencode_Lib")
        self.cpp_info.set_property("pkg_config_name", "bencode_lib")
