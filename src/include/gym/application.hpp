#pragma once

#include "gym/domain.hpp"
#include "gym/infrastructure.hpp"

#include <optional>
#include <string>
#include <vector>

namespace gym {

struct SetupRequest {
  std::string business_name;
  std::string admin_name;
  std::string email;
  std::string password;
};
struct MemberRequest {
  std::string full_name;
  std::string phone;
  std::string email;
};
struct PlanRequest {
  std::string code;
  std::string name;
  std::int64_t price_minor;
  int duration_days;
  int tax_basis_points;
};
struct ProductRequest {
  std::string sku;
  std::string name;
  std::int64_t price_minor;
  int tax_basis_points;
  int low_stock_threshold;
};
struct SaleLine {
  std::string kind;
  std::string code;
  int quantity;
  Discount discount;
};
struct CheckoutRequest {
  std::string member_number;
  std::string customer_name;
  std::vector<SaleLine> lines;
  Discount invoice_discount;
};

class GymService {
 public:
  GymService(Database& database, RuntimePaths paths, const PasswordHasher& password_hasher);
  [[nodiscard]] Result<void> setup(const SetupRequest& request);
  [[nodiscard]] Result<bool> login(const std::string& email, const std::string& password);
  [[nodiscard]] Result<void> add_member(const MemberRequest& request);
  [[nodiscard]] Result<void> add_plan(const PlanRequest& request);
  [[nodiscard]] Result<void> add_product(const ProductRequest& request);
  [[nodiscard]] Result<void> adjust_stock(const std::string& sku, int quantity,
                                          const std::string& reason);
  [[nodiscard]] Result<std::string> checkout(const CheckoutRequest& request);
  [[nodiscard]] Result<void> void_invoice(const std::string& number, const std::string& reason);
  [[nodiscard]] Result<void> export_report(const std::string& report_name);
  [[nodiscard]] Result<std::vector<std::vector<std::string>>> query_rows(
      const std::string& sql) const;
  [[nodiscard]] Result<std::string> business_name() const;

 private:
  Database& db_;
  RuntimePaths paths_;
  const PasswordHasher& password_hasher_;
  [[nodiscard]] Result<void> audit(const std::string& action, const std::string& details) const;
};

}  // namespace gym
