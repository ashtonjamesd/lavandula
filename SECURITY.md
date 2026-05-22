# Security Policy

## Reporting a Vulnerability

If you discover a security vulnerability in Lavandula, please report it by opening an issue on GitHub.

## Constant-Time Comparison Requirement

All credential comparisons in auth code MUST use `consttimeStrcmp()` (or equivalent constant-time comparison) rather than `strcmp()`. The `strcmp()` function short-circuits on the first differing byte, creating a timing oracle that can leak credential information byte-by-byte.

## Security-Sensitive Functions

The following functions handle authentication credentials and MUST use constant-time comparison:
- `checkBasicCredentials()` in `src/http/auth.c`
- Any future functions that compare password material

## Disclosure Policy

This project is in early development. Please report vulnerabilities openly via GitHub issues.
