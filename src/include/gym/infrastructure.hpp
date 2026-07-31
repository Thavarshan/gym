#pragma once

#include "gym/domain.hpp"

#include <cstddef>
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

class PasswordHasher {
 public:
  virtual ~PasswordHasher() = default;
  [[nodiscard]] virtual Result<std::string> hash(const std::string& password) const = 0;
  [[nodiscard]] virtual Result<bool> verify(const std::string& password,
                                            const std::string& hash) const = 0;
};

class SodiumPasswordHasher final : public PasswordHasher {
 public:
  enum class Profile { production, test };

  explicit SodiumPasswordHasher(Profile profile = Profile::production);
  [[nodiscard]] Result<std::string> hash(const std::string& password) const override;
  [[nodiscard]] Result<bool> verify(const std::string& password,
                                    const std::string& hash) const override;

 private:
  unsigned long long opslimit_;
  std::size_t memlimit_;
};

[[nodiscard]] std::string random_id();
[[nodiscard]] Result<void> write_pdf(const std::filesystem::path& output,
                                     const std::vector<std::string>& lines);
[[nodiscard]] Result<void> write_csv(const std::filesystem::path& output,
                                     const std::vector<std::vector<std::string>>& rows);

}  // namespace gym
