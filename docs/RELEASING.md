# Releasing Gym CLI

## Preconditions

- All required CI checks on `main` are green.
- The changelog documents the release.
- The CMake project version and release tag agree.
- No unresolved security advisories block the release.

## Publish

1. Create and push an annotated semantic-version tag, for example `v3.0.1`.
2. The Release workflow validates the tag, builds/tests Linux, macOS, and Windows packages, and generates a ZIP or tar.gz package for each platform.
3. The workflow verifies packages, generates SHA-256 checksums and an SPDX SBOM, attests the materials, and creates the GitHub Release.
4. Verify the release notes, attached packages, `SHA256SUMS`, SBOM, and provenance attestation before announcing it.

Releases are not code-signed until platform signing certificates are available. Consumers should verify SHA-256 checksums and GitHub attestations.
