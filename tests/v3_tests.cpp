#include "gym/application.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>

int main() {
  using namespace gym;
  auto total = Money::multiply(Money{150000}, 2);
  assert(total && total->minor() == 300000);
  assert(!validate_positive_quantity(0));
  auto invalid = normalize_email(" STAFF@GYM.COM ");
  assert(!invalid && invalid.error().message == "email must not contain whitespace");
  auto email = normalize_email("STAFF@GYM.COM");
  assert(email && *email == "staff@gym.com");
  PricedLine line{"PLAN",        "Monthly", 1,
                  Money{500000}, 1000,      {Discount::Kind::fixed, 50000, "promotion"}};
  auto invoice = calculate_invoice({line}, {});
  assert(invoice);
  assert(invoice->subtotal.minor() == 500000);
  assert(invoice->discount.minor() == 50000);
  assert(invoice->tax.minor() == 45000);
  assert(invoice->total.minor() == 495000);
  auto zero_tax = calculate_invoice({{"ZERO", "Zero tax", 1, Money{100}, 0, {}}}, {});
  assert(zero_tax && zero_tax->total.minor() == 100);

  const auto root = std::filesystem::temp_directory_path() / ("gym-cli-test-" + random_id());
  const RuntimePaths paths{root, root / "gym.sqlite3", root / "invoices", root / "exports",
                           root / "logs"};
  assert(create_runtime_directories(paths));
  Database database(paths.database);
  assert(database.open());
  assert(database.migrate());
  SodiumPasswordHasher password_hasher{SodiumPasswordHasher::Profile::test};
  GymService service(database, paths, password_hasher);
  assert(service.setup({"Test Gym", "Admin", "admin@test.invalid", "verysecurepassword"}));
  assert(service.login("admin@test.invalid", "verysecurepassword"));
  assert(service.add_product({"WHEY", "Whey", 250000, 0, 1}));
  assert(service.adjust_stock("WHEY", 3, "opening"));
  auto issued = service.checkout({"", "Walk-in", {{"product", "WHEY", 2, {}}}, {}});
  assert(issued && std::filesystem::exists(paths.invoices / (*issued + ".pdf")));
  assert(service.void_invoice(*issued, "customer correction"));
  auto status = database.scalar("SELECT status FROM invoices WHERE number='" + *issued + "';");
  assert(status && *status == "voided");
  std::filesystem::remove_all(root);
  std::cout << "gym domain tests passed\n";
}
