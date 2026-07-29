#pragma once

#include "gym/domain.hpp"

#include <filesystem>
#include <sqlite3.h>
#include <string>
#include <vector>

namespace gym {

struct RuntimePaths {
  std::filesystem::path data_dir;
  std::filesystem::path database;
  std::filesystem::path invoices;
  std::filesystem::path exports;
  std::filesystem::path logs;
};

[[nodiscard]] RuntimePaths resolve_paths(const std::string& explicit_data_dir);
[[nodiscard]] Result<void> create_runtime_directories(const RuntimePaths& paths);

class Database {
 public:
  explicit Database(std::filesystem::path filename);
  ~Database();
  Database(const Database&) = delete;
  Database& operator=(const Database&) = delete;
  [[nodiscard]] Result<void> open();
  [[nodiscard]] Result<void> migrate();
  [[nodiscard]] Result<void> exec(const std::string& sql) const;
  [[nodiscard]] Result<std::string> scalar(const std::string& sql) const;
  [[nodiscard]] Result<bool> has_users() const;
  [[nodiscard]] Result<bool> integrity_ok() const;
  [[nodiscard]] sqlite3* handle() const {
    return db_;
  }

 private:
  std::filesystem::path filename_;
  sqlite3* db_{nullptr};
};

[[nodiscard]] Result<std::string> hash_password(const std::string& password);
[[nodiscard]] Result<bool> verify_password(const std::string& password, const std::string& hash);
[[nodiscard]] std::string random_id();
[[nodiscard]] Result<void> write_pdf(const std::filesystem::path& output,
                                     const std::vector<std::string>& lines);
[[nodiscard]] Result<void> write_csv(const std::filesystem::path& output,
                                     const std::vector<std::vector<std::string>>& rows);

}  // namespace gym
