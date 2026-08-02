#pragma once

#include <cstdint>
#include <expected>
#include <string>
#include <vector>

namespace gym {

struct Error {
  std::string message;
};

template <typename T>
using Result = std::expected<T, Error>;

class Money {
 public:
  explicit Money(std::int64_t minor = 0) : minor_(minor) {}
  [[nodiscard]] std::int64_t minor() const {
    return minor_;
  }
  [[nodiscard]] static Result<Money> add(Money left, Money right);
  [[nodiscard]] static Result<Money> multiply(Money price, int quantity);
  [[nodiscard]] static Result<Money> percentage(Money amount, int basis_points);
  [[nodiscard]] std::string format_lkr() const;

 private:
  std::int64_t minor_;
};

struct Discount {
  enum class Kind { none, fixed, percentage };
  Kind kind{Kind::none};
  std::int64_t value{0};
  std::string reason;
};

struct PricedLine {
  std::string code;
  std::string name;
  int quantity{1};
  Money unit_price{};
  int tax_basis_points{0};
  Discount discount{};
};

struct CalculatedLine {
  PricedLine source;
  Money subtotal{};
  Money discount{};
  Money tax{};
  Money total{};
};

struct InvoiceTotals {
  std::vector<CalculatedLine> lines;
  Money subtotal{};
  Money discount{};
  Money tax{};
  Money total{};
};

[[nodiscard]] Result<InvoiceTotals> calculate_invoice(const std::vector<PricedLine>& lines,
                                                      const Discount& invoice_discount);
[[nodiscard]] Result<std::string> normalize_email(const std::string& email);
[[nodiscard]] Result<void> validate_positive_quantity(int quantity);

}  // namespace gym
