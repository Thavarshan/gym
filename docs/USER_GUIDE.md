# User Guide

## First run

Run `gym` from a terminal. On an empty data directory, Gym CLI asks for the business name and the first administrator account. Choose a password with at least 12 characters and store it safely.

## Main workflow

1. Sign in as an administrator.
2. Add membership plans and supplement products.
3. Record opening stock or adjustments for supplements.
4. Add members before selling membership plans.
5. Use checkout to issue an invoice. Retail-only sales may omit a member.
6. Find invoice PDFs under the application's `invoices` directory and CSV reports under `exports`.

## Data locations

Use `gym --data-dir <path>` for a specific location, or set `GYM_DATA_DIR`. Use `gym doctor --json` to check database integrity and directory availability.

## Safe operation

- Do not edit SQLite database files while the application is running.
- Close Gym CLI before externally backing up or restoring the data directory.
- Never share databases, invoices, or logs publicly; they can contain personal data.
- Void an incorrect invoice instead of editing it. This preserves an audit trail and reverses associated stock movements.
