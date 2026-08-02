#include "gym/application.hpp"

#include <filesystem>
#include <iostream>
#include <string_view>

namespace {

bool require(bool condition, std::string_view message) {
  if (!condition) std::cerr << "FAILED: " << message << '\n';
  return condition;
}

}  // namespace

int main() {
  using namespace gym;

  auto total = Money::multiply(Money{150000}, 2);
  if (!require(total && total->minor() == 300000, "money multiplication")) return 1;
  if (!require(!validate_positive_quantity(0), "zero quantity rejection")) return 1;
  auto invalid = normalize_email(" STAFF@GYM.COM ");
  if (!require(!invalid && invalid.error().message == "email must not contain whitespace",
               "email whitespace rejection"))
    return 1;
  auto email = normalize_email("STAFF@GYM.COM");
  if (!require(email && *email == "staff@gym.com", "email normalization")) return 1;
  PricedLine line{"PLAN",        "Monthly", 1,
                  Money{500000}, 1000,      {Discount::Kind::fixed, 50000, "promotion"}};
  auto invoice = calculate_invoice({line}, {});
  if (!require(invoice && invoice->subtotal.minor() == 500000 &&
                   invoice->discount.minor() == 50000 && invoice->tax.minor() == 45000 &&
                   invoice->total.minor() == 495000,
               "invoice calculation"))
    return 1;
  auto zero_tax = calculate_invoice({{"ZERO", "Zero tax", 1, Money{100}, 0, {}}}, {});
  if (!require(zero_tax && zero_tax->total.minor() == 100, "zero tax calculation")) return 1;

  const auto root = std::filesystem::temp_directory_path() / ("gym-cli-test-" + random_id());
  const RuntimePaths paths{root, root / "gym.sqlite3", root / "invoices", root / "exports",
                           root / "logs"};
  if (!require(static_cast<bool>(create_runtime_directories(paths)), "runtime directory creation"))
    return 1;
  {
    Database database(paths.database);
    if (!require(static_cast<bool>(database.open()), "database open")) return 1;
    if (!require(static_cast<bool>(database.migrate()), "database migration")) return 1;
    SodiumPasswordHasher password_hasher{SodiumPasswordHasher::Profile::test};
    GymService service(database, paths, password_hasher);
    if (!require(static_cast<bool>(service.setup(
                     {"Test Gym", "Admin", "admin@test.invalid", "verysecurepassword"})),
                 "initial setup"))
      return 1;
    auto logged_in = service.login("admin@test.invalid", "verysecurepassword");
    if (!require(logged_in && *logged_in, "administrator login")) return 1;
    if (!require(static_cast<bool>(service.add_product({"WHEY", "Whey", 250000, 0, 1})),
                 "product creation"))
      return 1;
    if (!require(static_cast<bool>(service.adjust_stock("WHEY", 3, "opening")), "opening stock"))
      return 1;
    auto issued = service.checkout({"", "Walk-in", {{"product", "WHEY", 2, {}}}, {}});
    if (!require(issued && std::filesystem::exists(paths.invoices / (*issued + ".pdf")),
                 "invoice issue and PDF generation"))
      return 1;
    if (!require(static_cast<bool>(service.void_invoice(*issued, "customer correction")),
                 "invoice void"))
      return 1;
    auto status = database.scalar("SELECT status FROM invoices WHERE number='" + *issued + "';");
    if (!require(status && *status == "voided", "voided invoice persistence")) return 1;
  }

  std::error_code error;
  std::filesystem::remove_all(root, error);
  if (!require(!error, "test data cleanup")) return 1;
  std::cout << "gym domain tests passed\n";
}
