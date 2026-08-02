# Architecture

Gym CLI is organized as a dependency-directed C++ application:

```text
Terminal UI -> Application services -> Domain
                     |
                     v
               Infrastructure adapters
```

## Layers

- **Domain:** Strong value types, pricing, tax, discount, and validation rules. It does not depend on SQLite, filesystem APIs, or terminal presentation.
- **Application:** Coordinates use cases such as setup, login, catalog changes, stock adjustments, checkout, voiding, and exports.
- **Infrastructure:** SQLite migrations, password hashing, identifiers, data paths, PDF output, and CSV output.
- **Terminal UI:** Collects input and renders results. It calls application services and never embeds business calculations.

## Data integrity

The SQLite adapter enables foreign keys, WAL mode, `synchronous=FULL`, and a bounded busy timeout. Checkout writes invoice snapshots, inventory movements, memberships, and audit records inside one database transaction. Invoice lines preserve catalog names and prices at the time of sale.

## Runtime data

The app resolves a platform data directory or uses `GYM_DATA_DIR`/`--data-dir`. Runtime data never depends on the checkout directory. The directory contains the SQLite database, invoice PDFs, exports, and logs.

## Extension guidance

Add new behavior at the domain/application boundary first, then introduce an infrastructure adapter only for external concerns. Preserve immutable invoice/audit history, use prepared SQL statements, and cover rules with tests before connecting new terminal screens.
