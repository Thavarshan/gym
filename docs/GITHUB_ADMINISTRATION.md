# GitHub Administration

This document records the one-time GitHub settings that cannot be enforced solely by files in the repository.

## Repository profile

Configure the repository with:

- Description: `Secure, cross-platform C++23 CLI for gym membership, inventory, invoicing, and reporting.`
- Topics: `cpp`, `cplusplus23`, `cli`, `cmake`, `sqlite`, `gym-management`, `inventory-management`, and `invoice-generator`.
- Discussions enabled for support and design conversations.
- Projects enabled for public roadmap planning.
- Wiki disabled; versioned documentation lives in `docs/`.
- Private vulnerability reporting enabled.
- Squash merging enabled; merge commits and rebase merges disabled; automatically delete merged branches.

## Main branch protection

Create a branch rule for `main` that:

- Requires a pull request before merging, at least one approval, and dismissal of stale approvals after new commits.
- Requires review from the `CODEOWNERS` owner for repository-wide changes.
- Requires conversation resolution before merging.
- Requires the `Format check`, `Clang-Tidy`, six `Verify` matrix jobs, `ASan and UBSan`, `Coverage report`, `CodeQL`, and `Dependency review` checks to pass.
- Requires branches to be up to date before merge and includes administrators.
- Prevents force pushes and branch deletion.

Enable GitHub's merge queue only after the required check names have appeared consistently in completed pull requests.

## Security and automation

- Enable code scanning and private vulnerability reporting in repository Security settings.
- Enable dependency review for pull requests; it may require GitHub Advanced Security for private repositories.
- Review all GitHub Actions workflow changes as code-owner changes.
- Do not add Dependabot configuration for this repository.

## Labels

Create the following labels if absent:

| Label | Color | Purpose |
| --- | --- | --- |
| `triage` | `D4C5F9` | Initial maintainer review needed |
| `breaking-change` | `B60205` | Requires a major-version release note |
| `security` | `EE0701` | Security-related work |
| `skip-changelog` | `6E7781` | Exclude from generated release notes |

Keep GitHub's default `bug`, `documentation`, `duplicate`, `enhancement`, `good first issue`, `help wanted`, `invalid`, `question`, and `wontfix` labels.
