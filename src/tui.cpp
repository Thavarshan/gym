#include "gym/tui.hpp"

#include <iostream>
#include <limits>

namespace gym {
namespace {
std::string prompt(const std::string& label) {
  std::cout << label;
  std::string value;
  std::getline(std::cin, value);
  return value;
}
int prompt_int(const std::string& label) {
  while (true) {
    const auto text = prompt(label);
    try {
      size_t position = 0;
      const auto value = std::stoi(text, &position);
      if (position == text.size()) return value;
    } catch (...) {
    }
    std::cout << "Enter a whole number.\n";
  }
}
std::int64_t prompt_money(const std::string& label) {
  while (true) {
    const auto text = prompt(label);
    try {
      size_t p = 0;
      auto amount = std::stod(text, &p);
      if (p == text.size() && amount >= 0) return static_cast<std::int64_t>(amount * 100.0 + 0.5);
    } catch (...) {
    }
    std::cout << "Enter a non-negative LKR amount, e.g. 1500.00.\n";
  }
}
void show_error(const Error& error) {
  std::cout << "\nError: " << error.message << "\n";
}
void pause() {
  std::cout << "\nPress Enter to continue...";
  std::string ignored;
  std::getline(std::cin, ignored);
}
void show_rows(const std::vector<std::vector<std::string>>& rows) {
  if (rows.empty()) {
    std::cout << "No records found.\n";
    return;
  }
  for (const auto& row : rows) {
    for (size_t i = 0; i < row.size(); ++i) std::cout << (i ? " | " : "") << row[i];
    std::cout << '\n';
  }
}
}  // namespace
int run_doctor(Database& database, const RuntimePaths& paths, bool json) {
  auto integrity = database.integrity_ok();
  const bool directories = std::filesystem::exists(paths.data_dir) &&
                           std::filesystem::is_directory(paths.invoices) &&
                           std::filesystem::is_directory(paths.exports);
  if (json) {
    std::cout << "{\"database\":\"" << (integrity && *integrity ? "ok" : "failed")
              << "\",\"paths\":\"" << (directories ? "ok" : "failed") << "\"}\n";
  } else {
    std::cout << "Gym CLI doctor\nDatabase: " << (integrity && *integrity ? "OK" : "FAILED")
              << "\nData directory: " << (directories ? "OK" : "FAILED")
              << "\nLocation: " << paths.data_dir << "\n";
  }
  return integrity && *integrity && directories ? 0 : 5;
}
int run_terminal_ui(GymService& service, Database& database, const RuntimePaths& paths) {
  auto users = database.has_users();
  if (!users) {
    show_error(users.error());
    return 4;
  }
  if (!*users) {
    std::cout << "\nWelcome to Gym CLI v3. Create the first administrator account.\n";
    SetupRequest setup{prompt("Business name: "), prompt("Administrator name: "),
                       prompt("Administrator email: "), prompt("Password (12+ characters): ")};
    auto created = service.setup(setup);
    if (!created) {
      show_error(created.error());
      return 3;
    }
  }
  bool authenticated = false;
  for (int attempt = 0; attempt < 5 && !authenticated; ++attempt) {
    std::cout << "\nSign in\n";
    auto logged = service.login(prompt("Email: "), prompt("Password: "));
    if (!logged) {
      show_error(logged.error());
      return 4;
    }
    authenticated = *logged;
    if (!authenticated) std::cout << "Invalid credentials.\n";
  }
  if (!authenticated) {
    std::cout << "Too many failed attempts.\n";
    return 3;
  }
  const auto business = service.business_name();
  std::cout << "\n" << (business ? *business : "Gym CLI") << " — administration console\n";
  while (std::cin) {
    std::cout << "\n1 Members  2 Plans  3 Products  4 Inventory  5 Checkout  6 "
                 "Invoices  7 Reports  8 Dashboard  0 Exit\n";
    const auto choice = prompt("Select: ");
    if (choice == "0") return 0;
    if (choice == "1") {
      const auto action = prompt("[a]dd or [l]ist: ");
      if (action == "a") {
        MemberRequest request{prompt("Full name: "), prompt("Phone: "),
                              prompt("Email (optional): ")};
        auto result = service.add_member(request);
        if (!result)
          show_error(result.error());
        else
          std::cout << "Member created.\n";
      } else {
        auto rows = service.query_rows(
            "SELECT member_number,full_name,phone,email,CASE archived WHEN 1 "
            "THEN 'archived' ELSE 'active' END FROM members ORDER BY "
            "member_number DESC");
        if (rows)
          show_rows(*rows);
        else
          show_error(rows.error());
      }
      pause();
      continue;
    }
    if (choice == "2") {
      const auto action = prompt("[a]dd or [l]ist: ");
      if (action == "a") {
        PlanRequest request{prompt("Code: "), prompt("Name: "), prompt_money("Price (LKR): "),
                            prompt_int("Duration (days): "),
                            prompt_int("Tax basis points (0-10000): ")};
        auto result = service.add_plan(request);
        if (!result)
          show_error(result.error());
        else
          std::cout << "Plan created.\n";
      } else {
        auto rows = service.query_rows(
            "SELECT "
            "code,name,price_minor,duration_days,tax_basis_points,active FROM "
            "membership_plans ORDER BY code");
        if (rows)
          show_rows(*rows);
        else
          show_error(rows.error());
      }
      pause();
      continue;
    }
    if (choice == "3") {
      const auto action = prompt("[a]dd or [l]ist: ");
      if (action == "a") {
        ProductRequest request{prompt("SKU: "), prompt("Name: "), prompt_money("Price (LKR): "),
                               prompt_int("Tax basis points (0-10000): "),
                               prompt_int("Low-stock threshold: ")};
        auto result = service.add_product(request);
        if (!result)
          show_error(result.error());
        else
          std::cout << "Product created.\n";
      } else {
        auto rows = service.query_rows(
            "SELECT "
            "sku,name,price_minor,tax_basis_points,low_stock_threshold,active "
            "FROM products ORDER BY sku");
        if (rows)
          show_rows(*rows);
        else
          show_error(rows.error());
      }
      pause();
      continue;
    }
    if (choice == "4") {
      const auto action = prompt("[a]djust or [l]ist stock: ");
      if (action == "a") {
        auto result = service.adjust_stock(prompt("SKU: "), prompt_int("Quantity (+/-): "),
                                           prompt("Reason: "));
        if (!result)
          show_error(result.error());
        else
          std::cout << "Stock adjusted.\n";
      } else {
        auto rows = service.query_rows(
            "SELECT p.sku,p.name,coalesce(sum(m.quantity),0) AS "
            "on_hand,p.low_stock_threshold FROM products p LEFT JOIN "
            "inventory_movements m ON m.product_id=p.id GROUP BY p.id ORDER BY "
            "p.sku");
        if (rows)
          show_rows(*rows);
        else
          show_error(rows.error());
      }
      pause();
      continue;
    }
    if (choice == "5") {
      CheckoutRequest checkout;
      checkout.member_number = prompt("Member number (required for plans; optional for retail): ");
      checkout.customer_name = prompt("Customer name for receipt (optional): ");
      while (true) {
        const auto kind = prompt("Line type [p]lan, p[r]oduct, or [d]one: ");
        if (kind == "d") break;
        SaleLine line;
        line.kind = kind == "p" ? "plan" : kind == "r" ? "product" : "";
        if (line.kind.empty()) {
          std::cout << "Choose p, r, or d.\n";
          continue;
        }
        line.code = prompt(line.kind == "plan" ? "Plan code: " : "Product SKU: ");
        line.quantity = prompt_int("Quantity: ");
        checkout.lines.push_back(std::move(line));
      }
      auto result = service.checkout(checkout);
      if (!result)
        show_error(result.error());
      else
        std::cout << "Invoice " << *result
                  << " issued. PDF: " << (paths.invoices / (*result + ".pdf")) << "\n";
      pause();
      continue;
    }
    if (choice == "6") {
      const auto action = prompt("[l]ist or [v]oid: ");
      if (action == "v") {
        auto result = service.void_invoice(prompt("Invoice number: "), prompt("Void reason: "));
        if (!result)
          show_error(result.error());
        else
          std::cout << "Invoice voided.\n";
      } else {
        auto rows = service.query_rows(
            "SELECT number,status,issued_at,total_minor,pdf_status FROM "
            "invoices ORDER BY issued_at DESC");
        if (rows)
          show_rows(*rows);
        else
          show_error(rows.error());
      }
      pause();
      continue;
    }
    if (choice == "7") {
      const auto name = prompt("Report [invoices|members|stock]: ");
      auto result = service.export_report(name);
      if (!result)
        show_error(result.error());
      else
        std::cout << "Exported " << (paths.exports / (name + ".csv")) << "\n";
      pause();
      continue;
    }
    if (choice == "8") {
      auto members = database.scalar("SELECT count(*) FROM members");
      auto invoices = database.scalar("SELECT count(*) FROM invoices WHERE status='issued'");
      auto stock = database.scalar("SELECT count(*) FROM products WHERE active=1");
      std::cout << "Members: " << (members ? *members : "?")
                << " | Issued invoices: " << (invoices ? *invoices : "?")
                << " | Active products: " << (stock ? *stock : "?") << "\n";
      pause();
      continue;
    }
    std::cout << "Unknown choice.\n";
  }
  return 0;
}
}  // namespace gym
