#include "gym/domain.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>

namespace gym {
namespace {
Result<std::int64_t> checked_add(std::int64_t a, std::int64_t b) {
  if ((b > 0 && a > std::numeric_limits<std::int64_t>::max() - b) ||
      (b < 0 && a < std::numeric_limits<std::int64_t>::min() - b))
    return std::unexpected(Error{"money overflow"});
  return a + b;
}
Result<Money> discount_for(Money subtotal, const Discount& discount) {
  if (discount.kind == Discount::Kind::none) return Money{};
  if (discount.value < 0 || discount.reason.empty())
    return std::unexpected(Error{"discount requires a positive value and reason"});
  auto value = discount.kind == Discount::Kind::fixed
                   ? Result<Money>{Money{discount.value}}
                   : Money::percentage(subtotal, static_cast<int>(discount.value));
  if (!value) return std::unexpected(value.error());
  if (value->minor() > subtotal.minor()) return std::unexpected(Error{"discount exceeds subtotal"});
  return *value;
}
}  // namespace

Result<Money> Money::add(Money left, Money right) {
  auto result = checked_add(left.minor_, right.minor_);
  if (!result) return std::unexpected(result.error());
  return Money{*result};
}
Result<Money> Money::multiply(Money price, int quantity) {
  if (quantity < 0) return std::unexpected(Error{"quantity cannot be negative"});
  if (quantity != 0 && (price.minor_ > std::numeric_limits<std::int64_t>::max() / quantity ||
                        price.minor_ < std::numeric_limits<std::int64_t>::min() / quantity))
    return std::unexpected(Error{"money overflow"});
  return Money{price.minor_ * quantity};
}
Result<Money> Money::percentage(Money amount, int basis_points) {
  if (basis_points < 0 || basis_points > 10000)
    return std::unexpected(Error{"percentage must be between 0 and 10000 basis points"});
  if (basis_points == 0 || amount.minor_ == 0) return Money{};
  if (amount.minor_ > std::numeric_limits<std::int64_t>::max() / basis_points)
    return std::unexpected(Error{"money overflow"});
  const auto numerator = amount.minor_ * basis_points;
  return Money{(numerator >= 0 ? numerator + 5000 : numerator - 5000) / 10000};
}
std::string Money::format_lkr() const {
  std::ostringstream out;
  const auto absolute = minor_ < 0 ? -minor_ : minor_;
  out << (minor_ < 0 ? "-LKR " : "LKR ") << absolute / 100 << '.' << std::setw(2)
      << std::setfill('0') << absolute % 100;
  return out.str();
}
Result<void> validate_positive_quantity(int quantity) {
  if (quantity <= 0) return std::unexpected(Error{"quantity must be positive"});
  return {};
}
Result<std::string> normalize_email(const std::string& email) {
  if (email.size() < 3 || email.size() > 254 || email.find('@') == std::string::npos)
    return std::unexpected(Error{"invalid email address"});
  std::string normalized;
  normalized.reserve(email.size());
  for (const char character : email) {
    if (std::isspace(static_cast<unsigned char>(character)))
      return std::unexpected(Error{"email must not contain whitespace"});
    normalized.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(character))));
  }
  return normalized;
}
Result<InvoiceTotals> calculate_invoice(const std::vector<PricedLine>& lines,
                                        const Discount& invoice_discount) {
  if (lines.empty()) return std::unexpected(Error{"an invoice needs at least one line"});
  InvoiceTotals invoice;
  for (const auto& line : lines) {
    auto valid_quantity = validate_positive_quantity(line.quantity);
    if (!valid_quantity) return std::unexpected(valid_quantity.error());
    auto subtotal = Money::multiply(line.unit_price, line.quantity);
    if (!subtotal) return std::unexpected(subtotal.error());
    auto line_discount = discount_for(*subtotal, line.discount);
    if (!line_discount) return std::unexpected(line_discount.error());
    auto net = Money::add(*subtotal, Money{-line_discount->minor()});
    if (!net) return std::unexpected(net.error());
    auto tax = Money::percentage(*net, line.tax_basis_points);
    if (!tax) return std::unexpected(tax.error());
    auto total = Money::add(*net, *tax);
    if (!total) return std::unexpected(total.error());
    invoice.lines.push_back({line, *subtotal, *line_discount, *tax, *total});
    auto subtotal_total = Money::add(invoice.subtotal, *subtotal);
    if (!subtotal_total) return std::unexpected(subtotal_total.error());
    invoice.subtotal = *subtotal_total;
    auto discount_total = Money::add(invoice.discount, *line_discount);
    if (!discount_total) return std::unexpected(discount_total.error());
    invoice.discount = *discount_total;
  }
  auto additional_discount = discount_for(invoice.subtotal, invoice_discount);
  if (!additional_discount) return std::unexpected(additional_discount.error());
  if (additional_discount->minor() > 0) {
    std::int64_t remaining = additional_discount->minor();
    std::int64_t denominator = invoice.subtotal.minor();
    for (size_t index = 0; index < invoice.lines.size(); ++index) {
      auto& line = invoice.lines[index];
      const auto allocation =
          index + 1 == invoice.lines.size()
              ? remaining
              : (additional_discount->minor() * line.subtotal.minor()) / denominator;
      remaining -= allocation;
      auto discount = Money::add(line.discount, Money{allocation});
      if (!discount) return std::unexpected(discount.error());
      line.discount = *discount;
      auto net = Money::add(line.subtotal, Money{-line.discount.minor()});
      if (!net) return std::unexpected(net.error());
      auto tax = Money::percentage(*net, line.source.tax_basis_points);
      if (!tax) return std::unexpected(tax.error());
      line.tax = *tax;
      auto total = Money::add(*net, line.tax);
      if (!total) return std::unexpected(total.error());
      line.total = *total;
    }
    auto discount_total = Money::add(invoice.discount, *additional_discount);
    if (!discount_total) return std::unexpected(discount_total.error());
    invoice.discount = *discount_total;
  }
  for (const auto& line : invoice.lines) {
    auto tax = Money::add(invoice.tax, line.tax);
    if (!tax) return std::unexpected(tax.error());
    invoice.tax = *tax;
    auto total = Money::add(invoice.total, line.total);
    if (!total) return std::unexpected(total.error());
    invoice.total = *total;
  }
  return invoice;
}
}  // namespace gym
