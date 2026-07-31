#include "gym/infrastructure.hpp"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <sodium.h>
#include <sstream>

namespace gym {
namespace {
std::string sqlite_error(sqlite3* db) {
  return db == nullptr ? "SQLite unavailable" : sqlite3_errmsg(db);
}
const char* migration_v1 = R"SQL(
CREATE TABLE IF NOT EXISTS schema_migrations(version INTEGER PRIMARY KEY, applied_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);
CREATE TABLE IF NOT EXISTS business_profile(id INTEGER PRIMARY KEY CHECK(id=1), name TEXT NOT NULL, currency TEXT NOT NULL DEFAULT 'LKR', created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);
CREATE TABLE IF NOT EXISTS users(id TEXT PRIMARY KEY, display_name TEXT NOT NULL, email TEXT NOT NULL UNIQUE, password_hash TEXT NOT NULL, active INTEGER NOT NULL DEFAULT 1, created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);
CREATE TABLE IF NOT EXISTS members(id TEXT PRIMARY KEY, member_number TEXT NOT NULL UNIQUE, full_name TEXT NOT NULL, phone TEXT NOT NULL, email TEXT, archived INTEGER NOT NULL DEFAULT 0, created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);
CREATE TABLE IF NOT EXISTS tax_rates(id TEXT PRIMARY KEY, name TEXT NOT NULL UNIQUE, basis_points INTEGER NOT NULL CHECK(basis_points BETWEEN 0 AND 10000), active INTEGER NOT NULL DEFAULT 1);
CREATE TABLE IF NOT EXISTS membership_plans(id TEXT PRIMARY KEY, code TEXT NOT NULL UNIQUE, name TEXT NOT NULL, price_minor INTEGER NOT NULL CHECK(price_minor>=0), duration_days INTEGER NOT NULL CHECK(duration_days>0), tax_basis_points INTEGER NOT NULL DEFAULT 0, active INTEGER NOT NULL DEFAULT 1);
CREATE TABLE IF NOT EXISTS products(id TEXT PRIMARY KEY, sku TEXT NOT NULL UNIQUE, name TEXT NOT NULL, price_minor INTEGER NOT NULL CHECK(price_minor>=0), tax_basis_points INTEGER NOT NULL DEFAULT 0, low_stock_threshold INTEGER NOT NULL DEFAULT 0, active INTEGER NOT NULL DEFAULT 1);
CREATE TABLE IF NOT EXISTS memberships(id TEXT PRIMARY KEY, member_id TEXT NOT NULL REFERENCES members(id), plan_id TEXT NOT NULL REFERENCES membership_plans(id), start_date TEXT NOT NULL, end_date TEXT NOT NULL, status TEXT NOT NULL CHECK(status IN ('scheduled','active','frozen','expired','cancelled')), invoice_id TEXT, cancellation_reason TEXT, created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);
CREATE TABLE IF NOT EXISTS membership_freezes(id TEXT PRIMARY KEY, membership_id TEXT NOT NULL REFERENCES memberships(id), start_date TEXT NOT NULL, end_date TEXT NOT NULL, reason TEXT NOT NULL);
CREATE TABLE IF NOT EXISTS invoices(id TEXT PRIMARY KEY, number TEXT NOT NULL UNIQUE, member_id TEXT REFERENCES members(id), customer_name TEXT, status TEXT NOT NULL CHECK(status IN ('issued','voided')), pdf_status TEXT NOT NULL DEFAULT 'pending', subtotal_minor INTEGER NOT NULL, discount_minor INTEGER NOT NULL, tax_minor INTEGER NOT NULL, total_minor INTEGER NOT NULL, void_reason TEXT, issued_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);
CREATE TABLE IF NOT EXISTS invoice_lines(id TEXT PRIMARY KEY, invoice_id TEXT NOT NULL REFERENCES invoices(id), kind TEXT NOT NULL CHECK(kind IN ('plan','product')), reference_id TEXT, code TEXT NOT NULL, name TEXT NOT NULL, quantity INTEGER NOT NULL CHECK(quantity>0), unit_price_minor INTEGER NOT NULL, discount_minor INTEGER NOT NULL, tax_basis_points INTEGER NOT NULL, tax_minor INTEGER NOT NULL, total_minor INTEGER NOT NULL);
CREATE TABLE IF NOT EXISTS invoice_number_sequences(year INTEGER PRIMARY KEY, next_value INTEGER NOT NULL);
CREATE TABLE IF NOT EXISTS inventory_movements(id TEXT PRIMARY KEY, product_id TEXT NOT NULL REFERENCES products(id), quantity INTEGER NOT NULL, type TEXT NOT NULL CHECK(type IN ('opening','receipt','adjustment','sale','void_reversal')), reason TEXT NOT NULL, invoice_id TEXT REFERENCES invoices(id), created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);
CREATE TABLE IF NOT EXISTS audit_events(id TEXT PRIMARY KEY, action TEXT NOT NULL, details TEXT NOT NULL, created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);
CREATE INDEX IF NOT EXISTS memberships_member_dates ON memberships(member_id,start_date,end_date);
CREATE INDEX IF NOT EXISTS invoice_lines_invoice ON invoice_lines(invoice_id);
CREATE INDEX IF NOT EXISTS inventory_product ON inventory_movements(product_id);
)SQL";
std::string pdf_escape(std::string value) {
  for (size_t p = 0; (p = value.find_first_of("()\\", p)) != std::string::npos; p += 2)
    value.insert(p, "\\");
  return value;
}
}  // namespace

RuntimePaths resolve_paths(const std::string& explicit_data_dir) {
  std::filesystem::path root;
  if (!explicit_data_dir.empty())
    root = explicit_data_dir;
  else if (const char* configured = std::getenv("GYM_DATA_DIR"))
    root = configured;
#ifdef __APPLE__
  else
    root = std::filesystem::path(std::getenv("HOME") ? std::getenv("HOME") : ".") /
           "Library/Application Support/GymCLI";
#elif defined(_WIN32)
  else
    root = std::filesystem::path(std::getenv("LOCALAPPDATA") ? std::getenv("LOCALAPPDATA") : ".") /
           "GymCLI";
#else
  else if (const char* xdg = std::getenv("XDG_DATA_HOME"))
    root = std::filesystem::path(xdg) / "gym-cli";
  else
    root = std::filesystem::path(std::getenv("HOME") ? std::getenv("HOME") : ".") /
           ".local/share/gym-cli";
#endif
  return {root, root / "gym.sqlite3", root / "invoices", root / "exports", root / "logs"};
}
Result<void> create_runtime_directories(const RuntimePaths& paths) {
  std::error_code error;
  for (const auto& directory : {paths.data_dir, paths.invoices, paths.exports, paths.logs})
    std::filesystem::create_directories(directory, error);
  if (error) return std::unexpected(Error{"cannot create data directory: " + error.message()});
  return {};
}
Database::Database(std::filesystem::path filename) : filename_(std::move(filename)) {}
Database::~Database() {
  if (db_ != nullptr) sqlite3_close(db_);
}
Result<void> Database::open() {
  if (sqlite3_open_v2(filename_.string().c_str(), &db_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                      nullptr) != SQLITE_OK)
    return std::unexpected(Error{sqlite_error(db_)});
  for (const char* pragma : {"PRAGMA foreign_keys=ON;", "PRAGMA journal_mode=WAL;",
                             "PRAGMA synchronous=FULL;", "PRAGMA busy_timeout=5000;"}) {
    auto result = exec(pragma);
    if (!result) return result;
  }
  return {};
}
Result<void> Database::exec(const std::string& sql) const {
  char* message = nullptr;
  if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &message) != SQLITE_OK) {
    std::string text = message ? message : sqlite_error(db_);
    sqlite3_free(message);
    return std::unexpected(Error{text});
  }
  return {};
}
Result<std::string> Database::scalar(const std::string& sql) const {
  sqlite3_stmt* statement = nullptr;
  if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, nullptr) != SQLITE_OK)
    return std::unexpected(Error{sqlite_error(db_)});
  const int result = sqlite3_step(statement);
  std::string value;
  if (result == SQLITE_ROW && sqlite3_column_text(statement, 0) != nullptr)
    value = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
  sqlite3_finalize(statement);
  if (result != SQLITE_ROW && result != SQLITE_DONE)
    return std::unexpected(Error{sqlite_error(db_)});
  return value;
}
Result<void> Database::migrate() {
  auto begin = exec("BEGIN IMMEDIATE;");
  if (!begin) return begin;
  auto schema = exec(migration_v1);
  if (!schema) {
    (void)exec("ROLLBACK;");
    return schema;
  }
  auto version = exec("INSERT OR IGNORE INTO schema_migrations(version) VALUES(1);");
  if (!version) {
    (void)exec("ROLLBACK;");
    return version;
  }
  auto defaults = exec(
      "INSERT OR IGNORE INTO tax_rates(id,name,basis_points) "
      "VALUES('system-zero','Zero tax',0);");
  if (!defaults) {
    (void)exec("ROLLBACK;");
    return defaults;
  }
  auto commit = exec("COMMIT;");
  return commit;
}
Result<bool> Database::has_users() const {
  auto count = scalar("SELECT count(*) FROM users;");
  if (!count) return std::unexpected(count.error());
  return *count != "0";
}
Result<bool> Database::integrity_ok() const {
  auto result = scalar("PRAGMA integrity_check;");
  if (!result) return std::unexpected(result.error());
  return *result == "ok";
}
SodiumPasswordHasher::SodiumPasswordHasher(Profile profile)
    : opslimit_(profile == Profile::production ? crypto_pwhash_OPSLIMIT_MODERATE
                                               : crypto_pwhash_OPSLIMIT_MIN),
      memlimit_(profile == Profile::production ? crypto_pwhash_MEMLIMIT_MODERATE
                                               : crypto_pwhash_MEMLIMIT_MIN) {}
Result<std::string> SodiumPasswordHasher::hash(const std::string& password) const {
  if (password.size() < 12)
    return std::unexpected(Error{"password must contain at least 12 characters"});
  if (sodium_init() < 0) return std::unexpected(Error{"libsodium initialization failed"});
  std::vector<char> hash(crypto_pwhash_STRBYTES);
  if (crypto_pwhash_str(hash.data(), password.c_str(), password.size(), opslimit_, memlimit_) != 0)
    return std::unexpected(Error{"not enough memory to hash password"});
  return std::string(hash.data());
}
Result<bool> SodiumPasswordHasher::verify(const std::string& password,
                                          const std::string& hash) const {
  if (sodium_init() < 0) return std::unexpected(Error{"libsodium initialization failed"});
  return crypto_pwhash_str_verify(hash.c_str(), password.c_str(), password.size()) == 0;
}
std::string random_id() {
  if (sodium_init() >= 0) {
    unsigned char bytes[16];
    randombytes_buf(bytes, sizeof bytes);
    std::ostringstream out;
    for (auto byte : bytes)
      out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    return out.str();
  }
  return std::to_string(std::rand());
}
Result<void> write_pdf(const std::filesystem::path& output, const std::vector<std::string>& lines) {
  std::ostringstream contents;
  contents << "BT /F1 11 Tf 48 780 Td 14 TL ";
  for (const auto& line : lines) contents << '(' << pdf_escape(line) << ") Tj T* ";
  contents << "ET";
  const auto body = contents.str();
  std::ostringstream pdf;
  std::vector<long> offsets(6);
  pdf << "%PDF-1.4\n";
  offsets[1] = static_cast<long>(pdf.tellp());
  pdf << "1 0 obj<</Type/Catalog/Pages 2 0 R>>endobj\n";
  offsets[2] = static_cast<long>(pdf.tellp());
  pdf << "2 0 obj<</Type/Pages/Count 1/Kids[3 0 R]>>endobj\n";
  offsets[3] = static_cast<long>(pdf.tellp());
  pdf << "3 0 obj<</Type/Page/Parent 2 0 R/MediaBox[0 0 595 "
         "842]/Resources<</Font<</F1 4 0 R>>>>/Contents 5 0 R>>endobj\n";
  offsets[4] = static_cast<long>(pdf.tellp());
  pdf << "4 0 obj<</Type/Font/Subtype/Type1/BaseFont/Helvetica>>endobj\n";
  offsets[5] = static_cast<long>(pdf.tellp());
  pdf << "5 0 obj<</Length " << body.size() << ">>stream\n" << body << "\nendstream endobj\n";
  const auto xref = static_cast<long>(pdf.tellp());
  pdf << "xref\n0 6\n0000000000 65535 f \n";
  for (int i = 1; i <= 5; ++i)
    pdf << std::setw(10) << std::setfill('0') << offsets[static_cast<size_t>(i)] << " 00000 n \n";
  pdf << "trailer<</Size 6/Root 1 0 R>>\nstartxref\n" << xref << "\n%%EOF\n";
  const auto temporary = output.string() + ".tmp";
  std::ofstream file(temporary, std::ios::binary);
  file << pdf.str();
  file.close();
  if (!file) return std::unexpected(Error{"cannot write PDF"});
  std::error_code error;
  std::filesystem::rename(temporary, output, error);
  if (error) return std::unexpected(Error{"cannot finalize PDF: " + error.message()});
  return {};
}
Result<void> write_csv(const std::filesystem::path& output,
                       const std::vector<std::vector<std::string>>& rows) {
  std::ofstream file(output);
  if (!file) return std::unexpected(Error{"cannot write export"});
  for (const auto& row : rows) {
    for (size_t index = 0; index < row.size(); ++index) {
      if (index) file << ',';
      file << '"';
      for (char c : row[index]) {
        if (c == '"') file << '"';
        file << c;
      }
      file << '"';
    }
    file << '\n';
  }
  return {};
}
}  // namespace gym
