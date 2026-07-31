#include "gym/application.hpp"
#include "gym/sqlite.hpp"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace gym {
namespace {
std::string now_year() {
  const auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
  const std::chrono::year_month_day date{today};
  return std::to_string(static_cast<int>(date.year()));
}
std::string today_iso() {
  const auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
  const std::chrono::year_month_day date{today};
  std::ostringstream out;
  out << static_cast<int>(date.year()) << '-' << std::setw(2) << std::setfill('0')
      << static_cast<unsigned>(date.month()) << '-' << std::setw(2)
      << static_cast<unsigned>(date.day());
  return out.str();
}
std::string end_date_after(int days) {
  const auto start = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
  const std::chrono::year_month_day date{start + std::chrono::days{days - 1}};
  std::ostringstream out;
  out << static_cast<int>(date.year()) << '-' << std::setw(2) << std::setfill('0')
      << static_cast<unsigned>(date.month()) << '-' << std::setw(2)
      << static_cast<unsigned>(date.day());
  return out.str();
}
}  // namespace

GymService::GymService(Database& database, RuntimePaths paths,
                       const PasswordHasher& password_hasher)
    : db_(database), paths_(std::move(paths)), password_hasher_(password_hasher) {}
Result<void> GymService::audit(const std::string& action, const std::string& details) const {
  sqlite::Statement query(db_.handle(),
                          "INSERT INTO audit_events(id,action,details) VALUES(?,?,?)");
  if (!query) return std::unexpected(Error{"cannot prepare audit statement"});
  auto a = sqlite::bind(query.value, 1, random_id());
  auto b = sqlite::bind(query.value, 2, action);
  auto c = sqlite::bind(query.value, 3, details);
  if (!a) return a;
  if (!b) return b;
  if (!c) return c;
  return sqlite::step_done(query.value);
}
Result<void> GymService::setup(const SetupRequest& request) {
  if (request.business_name.empty() || request.admin_name.empty())
    return std::unexpected(Error{"business and administrator names are required"});
  auto email = normalize_email(request.email);
  if (!email) return std::unexpected(email.error());
  auto password = password_hasher_.hash(request.password);
  if (!password) return std::unexpected(password.error());
  auto begin = db_.exec("BEGIN IMMEDIATE;");
  if (!begin) return begin;
  sqlite::Statement business(db_.handle(), "INSERT INTO business_profile(id,name) VALUES(1,?)");
  sqlite::Statement user(db_.handle(),
                         "INSERT INTO users(id,display_name,email,password_hash) VALUES(?,?,?,?)");
  if (!business || !user) {
    (void)db_.exec("ROLLBACK;");
    return std::unexpected(Error{"cannot prepare setup"});
  }
  auto b = sqlite::bind(business.value, 1, request.business_name);
  auto u1 = sqlite::bind(user.value, 1, random_id());
  auto u2 = sqlite::bind(user.value, 2, request.admin_name);
  auto u3 = sqlite::bind(user.value, 3, *email);
  auto u4 = sqlite::bind(user.value, 4, *password);
  if (!b || !u1 || !u2 || !u3 || !u4 || !sqlite::step_done(business.value) ||
      !sqlite::step_done(user.value)) {
    (void)db_.exec("ROLLBACK;");
    return std::unexpected(Error{"cannot save setup; email may already exist"});
  }
  auto audit_result = audit("setup", "first administrator created");
  if (!audit_result) {
    (void)db_.exec("ROLLBACK;");
    return audit_result;
  }
  return db_.exec("COMMIT;");
}
Result<bool> GymService::login(const std::string& email, const std::string& password) {
  auto normalized = normalize_email(email);
  if (!normalized) return std::unexpected(normalized.error());
  sqlite::Statement query(db_.handle(),
                          "SELECT password_hash FROM users WHERE email=? AND active=1");
  if (!query) return std::unexpected(Error{"cannot prepare login"});
  auto b = sqlite::bind(query.value, 1, *normalized);
  if (!b) return std::unexpected(b.error());
  if (sqlite3_step(query.value) != SQLITE_ROW) return false;
  auto verified = password_hasher_.verify(password, sqlite::column(query.value, 0));
  if (!verified) return std::unexpected(verified.error());
  if (*verified) {
    [[maybe_unused]] auto logged = audit("login", "successful login");
  }
  return *verified;
}
Result<void> GymService::add_member(const MemberRequest& request) {
  if (request.full_name.empty() || request.phone.empty())
    return std::unexpected(Error{"member name and phone are required"});
  std::string email;
  if (!request.email.empty()) {
    auto checked = normalize_email(request.email);
    if (!checked) return std::unexpected(checked.error());
    email = *checked;
  }
  auto count = db_.scalar("SELECT count(*)+1 FROM members;");
  if (!count) return std::unexpected(count.error());
  sqlite::Statement query(db_.handle(),
                          "INSERT INTO members(id,member_number,full_name,phone,email) "
                          "VALUES(?,?,?,?,?)");
  if (!query) return std::unexpected(Error{"cannot prepare member"});
  const auto number = "MEM-" + std::string(6 - std::min<size_t>(6, count->size()), '0') + *count;
  for (auto result :
       {sqlite::bind(query.value, 1, random_id()), sqlite::bind(query.value, 2, number),
        sqlite::bind(query.value, 3, request.full_name),
        sqlite::bind(query.value, 4, request.phone), sqlite::bind(query.value, 5, email)})
    if (!result) return result;
  auto saved = sqlite::step_done(query.value);
  if (!saved) return saved;
  return audit("member.created", number);
}
Result<void> GymService::add_plan(const PlanRequest& request) {
  if (request.code.empty() || request.name.empty() || request.price_minor < 0 ||
      request.duration_days <= 0 || request.tax_basis_points < 0 ||
      request.tax_basis_points > 10000)
    return std::unexpected(Error{"invalid plan details"});
  sqlite::Statement query(db_.handle(),
                          "INSERT INTO "
                          "membership_plans(id,code,name,price_minor,duration_days,tax_"
                          "basis_points) VALUES(?,?,?,?,?,?)");
  if (!query) return std::unexpected(Error{"cannot prepare plan"});
  for (auto result :
       {sqlite::bind(query.value, 1, random_id()), sqlite::bind(query.value, 2, request.code),
        sqlite::bind(query.value, 3, request.name),
        sqlite::bind(query.value, 4, request.price_minor),
        sqlite::bind(query.value, 5, static_cast<std::int64_t>(request.duration_days)),
        sqlite::bind(query.value, 6, static_cast<std::int64_t>(request.tax_basis_points))})
    if (!result) return result;
  auto saved = sqlite::step_done(query.value);
  if (!saved) return saved;
  return audit("plan.created", request.code);
}
Result<void> GymService::add_product(const ProductRequest& request) {
  if (request.sku.empty() || request.name.empty() || request.price_minor < 0 ||
      request.tax_basis_points < 0 || request.tax_basis_points > 10000 ||
      request.low_stock_threshold < 0)
    return std::unexpected(Error{"invalid product details"});
  sqlite::Statement query(db_.handle(),
                          "INSERT INTO "
                          "products(id,sku,name,price_minor,tax_basis_points,low_stock_"
                          "threshold) VALUES(?,?,?,?,?,?)");
  if (!query) return std::unexpected(Error{"cannot prepare product"});
  for (auto result :
       {sqlite::bind(query.value, 1, random_id()), sqlite::bind(query.value, 2, request.sku),
        sqlite::bind(query.value, 3, request.name),
        sqlite::bind(query.value, 4, request.price_minor),
        sqlite::bind(query.value, 5, static_cast<std::int64_t>(request.tax_basis_points)),
        sqlite::bind(query.value, 6, static_cast<std::int64_t>(request.low_stock_threshold))})
    if (!result) return result;
  auto saved = sqlite::step_done(query.value);
  if (!saved) return saved;
  return audit("product.created", request.sku);
}
Result<void> GymService::adjust_stock(const std::string& sku, int quantity,
                                      const std::string& reason) {
  if (quantity == 0 || reason.empty())
    return std::unexpected(Error{"quantity and reason are required"});
  sqlite::Statement lookup(db_.handle(), "SELECT id FROM products WHERE sku=? AND active=1");
  if (!lookup) return std::unexpected(Error{"cannot prepare stock lookup"});
  auto b = sqlite::bind(lookup.value, 1, sku);
  if (!b) return b;
  if (sqlite3_step(lookup.value) != SQLITE_ROW)
    return std::unexpected(Error{"active product not found"});
  const auto id = sqlite::column(lookup.value, 0);
  sqlite::Statement stock(db_.handle(),
                          "SELECT coalesce(sum(quantity),0) FROM "
                          "inventory_movements WHERE product_id=?");
  auto stock_binding = sqlite::bind(stock.value, 1, id);
  if (!stock_binding) return stock_binding;
  if (sqlite3_step(stock.value) != SQLITE_ROW) return std::unexpected(Error{"cannot read stock"});
  if (sqlite3_column_int(stock.value, 0) + quantity < 0)
    return std::unexpected(Error{"adjustment would make stock negative"});
  sqlite::Statement insert(db_.handle(),
                           "INSERT INTO inventory_movements(id,product_id,quantity,type,reason) "
                           "VALUES(?,?,?,?,?)");
  for (auto r :
       {sqlite::bind(insert.value, 1, random_id()), sqlite::bind(insert.value, 2, id),
        sqlite::bind(insert.value, 3, static_cast<std::int64_t>(quantity)),
        sqlite::bind(insert.value, 4, "adjustment"), sqlite::bind(insert.value, 5, reason)})
    if (!r) return r;
  auto done = sqlite::step_done(insert.value);
  if (!done) return done;
  return audit("inventory.adjusted", sku + ":" + std::to_string(quantity));
}
Result<std::string> GymService::checkout(const CheckoutRequest& request) {
  auto begin = db_.exec("BEGIN IMMEDIATE;");
  if (!begin) return std::unexpected(begin.error());
  auto rollback = [&](Error error) -> Result<std::string> {
    (void)db_.exec("ROLLBACK;");
    return std::unexpected(std::move(error));
  };
  std::string member_id;
  if (!request.member_number.empty()) {
    sqlite::Statement member(db_.handle(),
                             "SELECT id FROM members WHERE member_number=? AND archived=0");
    if (!member) return rollback({"cannot prepare member lookup"});
    auto member_binding = sqlite::bind(member.value, 1, request.member_number);
    if (!member_binding) return rollback(member_binding.error());
    if (sqlite3_step(member.value) != SQLITE_ROW) return rollback({"member not found"});
    member_id = sqlite::column(member.value, 0);
  }
  if (request.lines.empty()) return rollback({"sale needs at least one line"});
  struct Source {
    SaleLine sale;
    std::string id;
    std::string name;
    std::int64_t price;
    int tax;
    int duration;
  };
  std::vector<Source> sources;
  std::vector<PricedLine> priced;
  for (const auto& sale : request.lines) {
    auto valid = validate_positive_quantity(sale.quantity);
    if (!valid) return rollback(valid.error());
    const char* sql = sale.kind == "plan"
                          ? "SELECT id,name,price_minor,tax_basis_points,duration_days FROM "
                            "membership_plans WHERE code=? AND active=1"
                          : "SELECT id,name,price_minor,tax_basis_points,0 FROM products "
                            "WHERE sku=? AND active=1";
    sqlite::Statement lookup(db_.handle(), sql);
    if (!lookup) return rollback({"cannot prepare catalog lookup"});
    auto item_binding = sqlite::bind(lookup.value, 1, sale.code);
    if (!item_binding) return rollback(item_binding.error());
    if (sqlite3_step(lookup.value) != SQLITE_ROW)
      return rollback({"active catalog item not found: " + sale.code});
    Source source{sale,
                  sqlite::column(lookup.value, 0),
                  sqlite::column(lookup.value, 1),
                  sqlite3_column_int64(lookup.value, 2),
                  sqlite3_column_int(lookup.value, 3),
                  sqlite3_column_int(lookup.value, 4)};
    if (sale.kind == "plan" && member_id.empty())
      return rollback({"membership plan sales require a member"});
    if (sale.kind == "plan" && sale.quantity != 1)
      return rollback({"membership plans must have quantity one"});
    if (sale.kind != "plan" && sale.kind != "product")
      return rollback({"line kind must be plan or product"});
    priced.push_back(
        {sale.code, source.name, sale.quantity, Money{source.price}, source.tax, sale.discount});
    sources.push_back(std::move(source));
  }
  auto totals = calculate_invoice(priced, request.invoice_discount);
  if (!totals) return rollback(totals.error());
  for (const auto& source : sources)
    if (source.sale.kind == "product") {
      sqlite::Statement stock(db_.handle(),
                              "SELECT coalesce(sum(quantity),0) FROM "
                              "inventory_movements WHERE product_id=?");
      auto stock_binding = sqlite::bind(stock.value, 1, source.id);
      if (!stock_binding) return rollback(stock_binding.error());
      if (sqlite3_step(stock.value) != SQLITE_ROW ||
          sqlite3_column_int(stock.value, 0) < source.sale.quantity)
        return rollback({"insufficient stock for " + source.sale.code});
    }
  const int year = std::stoi(now_year());
  sqlite::Statement sequence(db_.handle(),
                             "INSERT INTO invoice_number_sequences(year,next_value) "
                             "VALUES(?,2) ON CONFLICT(year) DO UPDATE SET "
                             "next_value=next_value+1 RETURNING next_value-1");
  auto sequence_binding = sqlite::bind(sequence.value, 1, static_cast<std::int64_t>(year));
  if (!sequence_binding) return rollback(sequence_binding.error());
  if (sqlite3_step(sequence.value) != SQLITE_ROW)
    return rollback({"cannot allocate invoice number"});
  std::ostringstream formatted;
  formatted << "INV-" << year << '-' << std::setw(6) << std::setfill('0')
            << sqlite3_column_int(sequence.value, 0);
  const auto number = formatted.str();
  sqlite3_reset(sequence.value);
  const auto invoice_id = random_id();
  sqlite::Statement invoice(db_.handle(),
                            "INSERT INTO "
                            "invoices(id,number,member_id,customer_name,status,"
                            "subtotal_minor,discount_minor,tax_minor,total_minor) "
                            "VALUES(?,?,?,?, 'issued',?,?,?,?)");
  auto invoice_member = member_id.empty() ? sqlite::bind_null(invoice.value, 3)
                                          : sqlite::bind(invoice.value, 3, member_id);
  for (auto r : {sqlite::bind(invoice.value, 1, invoice_id), sqlite::bind(invoice.value, 2, number),
                 invoice_member, sqlite::bind(invoice.value, 4, request.customer_name),
                 sqlite::bind(invoice.value, 5, totals->subtotal.minor()),
                 sqlite::bind(invoice.value, 6, totals->discount.minor()),
                 sqlite::bind(invoice.value, 7, totals->tax.minor()),
                 sqlite::bind(invoice.value, 8, totals->total.minor())})
    if (!r) return rollback(r.error());
  auto saved = sqlite::step_done(invoice.value);
  if (!saved) return rollback(saved.error());
  for (size_t i = 0; i < sources.size(); ++i) {
    const auto& source = sources[i];
    const auto& line = totals->lines[i];
    sqlite::Statement line_insert(
        db_.handle(),
        "INSERT INTO "
        "invoice_lines(id,invoice_id,kind,reference_id,code,name,quantity,unit_"
        "price_minor,discount_minor,tax_basis_points,tax_minor,total_minor) "
        "VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
    for (auto r :
         {sqlite::bind(line_insert.value, 1, random_id()),
          sqlite::bind(line_insert.value, 2, invoice_id),
          sqlite::bind(line_insert.value, 3, source.sale.kind),
          sqlite::bind(line_insert.value, 4, source.id),
          sqlite::bind(line_insert.value, 5, source.sale.code),
          sqlite::bind(line_insert.value, 6, source.name),
          sqlite::bind(line_insert.value, 7, static_cast<std::int64_t>(source.sale.quantity)),
          sqlite::bind(line_insert.value, 8, source.price),
          sqlite::bind(line_insert.value, 9, line.discount.minor()),
          sqlite::bind(line_insert.value, 10, static_cast<std::int64_t>(source.tax)),
          sqlite::bind(line_insert.value, 11, line.tax.minor()),
          sqlite::bind(line_insert.value, 12, line.total.minor())})
      if (!r) return rollback(r.error());
    auto line_saved = sqlite::step_done(line_insert.value);
    if (!line_saved) return rollback(line_saved.error());
    if (source.sale.kind == "product") {
      sqlite::Statement movement(
          db_.handle(),
          "INSERT INTO "
          "inventory_movements(id,product_id,quantity,type,reason,invoice_id) "
          "VALUES(?,?,?,'sale','invoice sale',?)");
      for (auto r :
           {sqlite::bind(movement.value, 1, random_id()),
            sqlite::bind(movement.value, 2, source.id),
            sqlite::bind(movement.value, 3, static_cast<std::int64_t>(-source.sale.quantity)),
            sqlite::bind(movement.value, 4, invoice_id)})
        if (!r) return rollback(r.error());
      auto moved = sqlite::step_done(movement.value);
      if (!moved) return rollback(moved.error());
    } else {
      sqlite::Statement membership(db_.handle(),
                                   "INSERT INTO "
                                   "memberships(id,member_id,plan_id,start_date,end_date,"
                                   "status,invoice_id) VALUES(?,?,?,?,?,'active',?)");
      for (auto r : {sqlite::bind(membership.value, 1, random_id()),
                     sqlite::bind(membership.value, 2, member_id),
                     sqlite::bind(membership.value, 3, source.id),
                     sqlite::bind(membership.value, 4, today_iso()),
                     sqlite::bind(membership.value, 5, end_date_after(source.duration)),
                     sqlite::bind(membership.value, 6, invoice_id)})
        if (!r) return rollback(r.error());
      auto created = sqlite::step_done(membership.value);
      if (!created) return rollback(created.error());
    }
  }
  auto logged = audit("invoice.issued", number);
  if (!logged) return rollback(logged.error());
  auto commit = db_.exec("COMMIT;");
  if (!commit) return std::unexpected(commit.error());
  std::vector<std::string> document{
      "GYM INVOICE " + number,
      "Customer: " +
          (request.customer_name.empty() ? request.member_number : request.customer_name),
      ""};
  for (const auto& line : totals->lines)
    document.push_back(line.source.name + " x" + std::to_string(line.source.quantity) + "  " +
                       line.total.format_lkr());
  document.push_back("Subtotal: " + totals->subtotal.format_lkr());
  document.push_back("Discount: " + totals->discount.format_lkr());
  document.push_back("Tax: " + totals->tax.format_lkr());
  document.push_back("Total: " + totals->total.format_lkr());
  auto pdf = write_pdf(paths_.invoices / (number + ".pdf"), document);
  if (!pdf) {
    sqlite::Statement failed(db_.handle(), "UPDATE invoices SET pdf_status='failed' WHERE id=?");
    [[maybe_unused]] const auto mark_failed = sqlite::bind(failed.value, 1, invoice_id);
    [[maybe_unused]] const auto persist_failure = sqlite::step_done(failed.value);
    return std::unexpected(pdf.error());
  }
  sqlite::Statement ready(db_.handle(), "UPDATE invoices SET pdf_status='ready' WHERE id=?");
  auto ready_binding = sqlite::bind(ready.value, 1, invoice_id);
  if (!ready_binding) return std::unexpected(ready_binding.error());
  auto mark_ready = sqlite::step_done(ready.value);
  if (!mark_ready) return std::unexpected(mark_ready.error());
  return number;
}
Result<void> GymService::void_invoice(const std::string& number, const std::string& reason) {
  if (reason.empty()) return std::unexpected(Error{"void reason is required"});
  auto begin = db_.exec("BEGIN IMMEDIATE;");
  if (!begin) return begin;
  auto fail = [&](Error e) -> Result<void> {
    (void)db_.exec("ROLLBACK;");
    return std::unexpected(std::move(e));
  };
  sqlite::Statement invoice(db_.handle(), "SELECT id,status FROM invoices WHERE number=?");
  (void)sqlite::bind(invoice.value, 1, number);
  if (sqlite3_step(invoice.value) != SQLITE_ROW) return fail({"invoice not found"});
  const auto id = sqlite::column(invoice.value, 0);
  if (sqlite::column(invoice.value, 1) != "issued") return fail({"invoice is already voided"});
  sqlite::Statement products(db_.handle(),
                             "SELECT reference_id,quantity FROM invoice_lines WHERE "
                             "invoice_id=? AND kind='product'");
  (void)sqlite::bind(products.value, 1, id);
  while (sqlite3_step(products.value) == SQLITE_ROW) {
    sqlite::Statement reversal(db_.handle(),
                               "INSERT INTO "
                               "inventory_movements(id,product_id,quantity,type,reason,"
                               "invoice_id) VALUES(?,?,?,'void_reversal',?,?)");
    for (auto r : {sqlite::bind(reversal.value, 1, random_id()),
                   sqlite::bind(reversal.value, 2, sqlite::column(products.value, 0)),
                   sqlite::bind(reversal.value, 3,
                                static_cast<std::int64_t>(sqlite3_column_int(products.value, 1))),
                   sqlite::bind(reversal.value, 4, reason), sqlite::bind(reversal.value, 5, id)})
      if (!r) return fail(r.error());
    auto done = sqlite::step_done(reversal.value);
    if (!done) return fail(done.error());
  }
  sqlite::Statement update(db_.handle(),
                           "UPDATE invoices SET status='voided',void_reason=?,pdf_status='pending' "
                           "WHERE id=?");
  (void)sqlite::bind(update.value, 1, reason);
  (void)sqlite::bind(update.value, 2, id);
  auto updated = sqlite::step_done(update.value);
  if (!updated) return fail(updated.error());
  sqlite::Statement memberships(
      db_.handle(),
      "UPDATE memberships SET status='cancelled',cancellation_reason=? WHERE "
      "invoice_id=?");
  (void)sqlite::bind(memberships.value, 1, reason);
  (void)sqlite::bind(memberships.value, 2, id);
  auto cancelled = sqlite::step_done(memberships.value);
  if (!cancelled) return fail(cancelled.error());
  auto logged = audit("invoice.voided", number);
  if (!logged) return fail(logged.error());
  return db_.exec("COMMIT;");
}
Result<std::vector<std::vector<std::string>>> GymService::query_rows(const std::string& sql) const {
  sqlite::Statement query(db_.handle(), sql.c_str());
  if (!query) return std::unexpected(Error{"invalid report query"});
  std::vector<std::vector<std::string>> rows;
  const int columns = sqlite3_column_count(query.value);
  while (sqlite3_step(query.value) == SQLITE_ROW) {
    std::vector<std::string> row;
    for (int i = 0; i < columns; ++i) row.push_back(sqlite::column(query.value, i));
    rows.push_back(std::move(row));
  }
  return rows;
}
Result<std::string> GymService::business_name() const {
  return db_.scalar("SELECT name FROM business_profile WHERE id=1;");
}
Result<void> GymService::export_report(const std::string& name) {
  std::string sql;
  if (name == "invoices")
    sql =
        "SELECT number,status,issued_at,total_minor FROM invoices ORDER BY "
        "issued_at DESC";
  else if (name == "members")
    sql =
        "SELECT member_number,full_name,phone,email,archived FROM members "
        "ORDER BY member_number";
  else if (name == "stock")
    sql =
        "SELECT p.sku,p.name,coalesce(sum(m.quantity),0) AS "
        "on_hand,p.low_stock_threshold FROM products p LEFT JOIN "
        "inventory_movements m ON m.product_id=p.id GROUP BY p.id ORDER BY "
        "p.sku";
  else
    return std::unexpected(Error{"unknown report"});
  auto rows = query_rows(sql);
  if (!rows) return std::unexpected(rows.error());
  std::vector<std::vector<std::string>> data;
  if (name == "invoices") data.push_back({"number", "status", "issued_at", "total_minor"});
  if (name == "members")
    data.push_back({"member_number", "full_name", "phone", "email", "archived"});
  if (name == "stock") data.push_back({"sku", "name", "on_hand", "low_stock_threshold"});
  data.insert(data.end(), rows->begin(), rows->end());
  auto result = write_csv(paths_.exports / (name + ".csv"), data);
  if (!result) return result;
  return audit("report.exported", name);
}
}  // namespace gym
