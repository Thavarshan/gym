#pragma once

#include "gym/domain.hpp"

#include <sqlite3.h>

#include <cstdint>
#include <string>

namespace gym::sqlite {

class Statement final {
 public:
  Statement(sqlite3* database, const char* sql) {
    sqlite3_prepare_v2(database, sql, -1, &value, nullptr);
  }

  ~Statement() {
    if (value != nullptr) {
      sqlite3_finalize(value);
    }
  }

  Statement(const Statement&) = delete;
  Statement& operator=(const Statement&) = delete;

  [[nodiscard]] bool valid() const {
    return value != nullptr;
  }
  explicit operator bool() const {
    return valid();
  }

  sqlite3_stmt* value{nullptr};
};

[[nodiscard]] inline Result<void> bind(sqlite3_stmt* statement, int index,
                                       const std::string& value) {
  if (sqlite3_bind_text(statement, index, value.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
    return std::unexpected(Error{"database bind failed"});
  }
  return {};
}

[[nodiscard]] inline Result<void> bind(sqlite3_stmt* statement, int index, std::int64_t value) {
  if (sqlite3_bind_int64(statement, index, value) != SQLITE_OK) {
    return std::unexpected(Error{"database bind failed"});
  }
  return {};
}

[[nodiscard]] inline Result<void> bind_null(sqlite3_stmt* statement, int index) {
  if (sqlite3_bind_null(statement, index) != SQLITE_OK) {
    return std::unexpected(Error{"database bind failed"});
  }
  return {};
}

[[nodiscard]] inline Result<void> step_done(sqlite3_stmt* statement) {
  if (sqlite3_step(statement) != SQLITE_DONE) {
    return std::unexpected(Error{sqlite3_errmsg(sqlite3_db_handle(statement))});
  }
  return {};
}

[[nodiscard]] inline std::string column(sqlite3_stmt* statement, int index) {
  const auto* text = sqlite3_column_text(statement, index);
  return text == nullptr ? "" : reinterpret_cast<const char*>(text);
}

}  // namespace gym::sqlite
