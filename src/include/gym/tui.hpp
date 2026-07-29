#pragma once

#include "gym/application.hpp"

namespace gym {
int run_terminal_ui(GymService& service, Database& database, const RuntimePaths& paths);
int run_doctor(Database& database, const RuntimePaths& paths, bool json);
}  // namespace gym
