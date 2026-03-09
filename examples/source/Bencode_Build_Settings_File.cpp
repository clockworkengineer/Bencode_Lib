//
// Program: Bencode_Build_Settings_File
//
// Description: Demonstrates building a structured settings/config file using
// Bencode at runtime. The settings are written to a .ben file and then read
// back, accessed via key operators, and their values displayed.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Bencode_Utility.hpp"

namespace be = Bencode_Lib;

std::string settingsFilePath() {
  return (std::filesystem::current_path() / "files" / "settings.ben").string();
}

/// <summary>
/// Build a settings structure and write it to a Bencode file.
/// </summary>
void writeSettings() {
  be::Bencode settings;
  // Application section
  settings["application"]["name"] = "MyApp";
  settings["application"]["version"] = 2;
  settings["application"]["debug"] = 0;
  // Window section
  settings["window"]["width"] = 1280;
  settings["window"]["height"] = 720;
  settings["window"]["fullscreen"] = 0;
  settings["window"]["title"] = "My Application Window";
  // Network section
  settings["network"]["host"] = "localhost";
  settings["network"]["port"] = 8080;
  settings["network"]["timeout"] = 30;
  // Write to file
  settings.stringify(be::FileDestination{settingsFilePath()});
  PLOG_INFO << "Settings written to " << settingsFilePath();
}

/// <summary>
/// Read back the settings file and display values.
/// </summary>
void readSettings() {
  be::Bencode settings;
  settings.parse(be::FileSource{settingsFilePath()});

  const auto appName =
      be::NRef<be::String>(settings["application"]["name"]).value();
  const auto appVersion =
      be::NRef<be::Integer>(settings["application"]["version"]).value();
  const auto appDebug =
      be::NRef<be::Integer>(settings["application"]["debug"]).value();

  const auto winWidth =
      be::NRef<be::Integer>(settings["window"]["width"]).value();
  const auto winHeight =
      be::NRef<be::Integer>(settings["window"]["height"]).value();
  const auto winFullscreen =
      be::NRef<be::Integer>(settings["window"]["fullscreen"]).value();
  const auto winTitle =
      be::NRef<be::String>(settings["window"]["title"]).value();

  const auto netHost =
      be::NRef<be::String>(settings["network"]["host"]).value();
  const auto netPort =
      be::NRef<be::Integer>(settings["network"]["port"]).value();
  const auto netTimeout =
      be::NRef<be::Integer>(settings["network"]["timeout"]).value();

  PLOG_INFO << "=== Application ===";
  PLOG_INFO << "  name    : " << appName;
  PLOG_INFO << "  version : " << appVersion;
  PLOG_INFO << "  debug   : " << appDebug;
  PLOG_INFO << "=== Window ===";
  PLOG_INFO << "  width      : " << winWidth;
  PLOG_INFO << "  height     : " << winHeight;
  PLOG_INFO << "  fullscreen : " << winFullscreen;
  PLOG_INFO << "  title      : " << winTitle;
  PLOG_INFO << "=== Network ===";
  PLOG_INFO << "  host    : " << netHost;
  PLOG_INFO << "  port    : " << netPort;
  PLOG_INFO << "  timeout : " << netTimeout;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    init(plog::debug, "Bencode_Build_Settings_File.log");
    PLOG_INFO << "Bencode_Build_Settings_File started ...";
    PLOG_INFO << be::Bencode::version();
    writeSettings();
    readSettings();
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "Bencode_Build_Settings_File exited.";
  exit(EXIT_SUCCESS);
}
