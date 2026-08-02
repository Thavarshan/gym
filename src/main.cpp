#include "gym/infrastructure.hpp"
#include "gym/tui.hpp"

#include <iostream>

int main(int argc, char** argv) {
  std::string data_dir;
  bool doctor = false;
  bool json = false;
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--help") {
      std::cout << "Gym CLI v3\nUsage: gym [--data-dir PATH] [--log-level "
                   "LEVEL]\n       gym doctor [--json] [--data-dir PATH]\n";
      return 0;
    }
    if (arg == "--version") {
      std::cout << "gym 3.0.0\n";
      return 0;
    }
    if (arg == "doctor") {
      doctor = true;
      continue;
    }
    if (arg == "--json") {
      json = true;
      continue;
    }
    if (arg == "--data-dir" && i + 1 < argc) {
      data_dir = argv[++i];
      continue;
    }
    if (arg == "--log-level" && i + 1 < argc) {
      ++i;
      continue;
    }
    std::cerr << "Invalid argument: " << arg << "\n";
    return 2;
  }
  auto paths = gym::resolve_paths(data_dir);
  auto directories = gym::create_runtime_directories(paths);
  if (!directories) {
    std::cerr << directories.error().message << '\n';
    return 3;
  }
  gym::Database database(paths.database);
  auto opened = database.open();
  if (!opened) {
    std::cerr << opened.error().message << '\n';
    return 4;
  }
  if (!doctor) {
    auto migrated = database.migrate();
    if (!migrated) {
      std::cerr << migrated.error().message << '\n';
      return 4;
    }
  }
  if (doctor) return gym::run_doctor(database, paths, json);
  gym::SodiumPasswordHasher password_hasher;
  gym::GymService service(database, paths, password_hasher);
  return gym::run_terminal_ui(service, database, paths);
}
