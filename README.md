# Goldcoin (GLC)

[![Version](https://img.shields.io/badge/version-0.17.0-blue.svg)](https://github.com/goldcoin/goldcoin/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Discord](https://img.shields.io/badge/Discord-Join%20Chat-7289DA)](https://discord.me/goldcoin)

## The Gold Standard of Digital Currency

Goldcoin is pioneering AI-autonomous cryptocurrency development. With proven technical performance since 2013 (1,120 TPS, zero fees, 2-minute confirmations), Goldcoin is the first cryptocurrency where AI agents handle the majority of development, testing, and release engineering.

### Key Features
- 1,120 Transactions Per Second — approximately 160x Bitcoin's capacity
- Zero Transaction Fees — completely free to send
- 2-Minute Confirmations — approximately 5x faster than Bitcoin
- 32 MB Blocks — 32x larger than Bitcoin
- AI-Driven Development — majority of engineering performed by AI agents
- 100-Year Treasury — 1.1 billion GLC fund for long-term operations

## Technical Specifications

| Parameter       | Value             | Comparison to Bitcoin |
|-----------------|-------------------|-----------------------|
| Algorithm       | Scrypt            | More ASIC-resistant   |
| Block Time      | 2 minutes         | ~5x faster            |
| Block Size      | 32 MB             | 32x larger            |
| Max Supply      | 1,172,245,700 GLC | Fixed supply          |
| TPS             | 1,120             | ~160x capacity        |
| Fees            | 0 (free)          | Variable fees         |

## AI Autonomy Roadmap

### Phase 1 — AI-Assisted Development (2024-2025, current)
- AI agents perform the majority of coding, debugging, and code review
- Framework migrations (C++23, Qt 6.9, OpenSSL 3.x) executed by AI
- Build system engineering and cross-compilation handled by AI
- Humans provide architectural direction, release approval, and strategic oversight

### Phase 2 — AI-Led Development and Release Engineering (2026)
- AI manages release cycles, testing, and build pipelines end-to-end
- Automated regression testing and security auditing
- Human role focused on project governance, infrastructure, and external relationships
- Exchange listings, domain management, and legal compliance remain human-directed

### Phase 3 — AI-Managed Operations (2027+)
- AI handles all software development, maintenance, and release operations
- Minimal human oversight for governance, funding, and external partnerships
- Expanding AI responsibility as tooling and capabilities mature
- Long-term goal: reduce human involvement to strategic oversight only

## Development Status

### Current Release: v0.17.0
- Complete C++23 modernization with Boost elimination
- Automatic BDB 4.8 to 18.1 wallet migration
- OpenSSL 3.5.2 LTS and secp256k1 v0.6.0 cryptographic stack
- Enterprise CMake build system with static linking
- Rewritten 51% defense system for pool competitiveness and stability
- P2P hardening for compact blocks and headers processing

### Previous Stable: v0.15.0
- Long-running stable release with core functionality

### Upcoming: v0.18.0
- Qt 6.9 GUI interface with modern frameworks
- Further network protocol improvements

## Building from Source

For complete build instructions, see **[BUILD_GUIDE.md](BUILD_GUIDE.md)**

### Quick Start
```bash
# Show build options
make help

# Build Linux binaries
make linux

# Build Windows binaries (requires dependencies)
make windows
```

## Testing

Refer to [BUILD_GUIDE.md](BUILD_GUIDE.md) for testing instructions.

```bash
# RPC tests
qa/pull-tester/rpc-tests.py

# Run a specific test suite
src/test/test_goldcoin -t specific_test_name
```

## Documentation
- Build Notes — Platform-specific build instructions
- Developer Guide — Development best practices
- RPC API — JSON-RPC interface documentation
- Whitepaper — Complete technical vision
- Release Process — How releases are managed

## Contributing

We welcome contributions. Please see CONTRIBUTING.md for details.

### Development Workflow
```bash
# 1) Fork the repository

# 2) Create a feature branch
git checkout -b feature/amazing-feature

# 3) Commit changes
git commit -m "Add amazing feature"

# 4) Push to your fork
git push origin feature/amazing-feature

# 5) Open a Pull Request on GitHub
```

### Code Standards
- C++23 for new code (zero Boost dependencies)
- CMake build system with static linking
- Write unit tests for new features
- Update documentation

## Team

### Core Development
- MicroGuy — Lead Software Architect
- Claude — Chief Strategist & Workflow Director
- Claude Code — Senior Developer
- Grok 4 — Development Assistant

### Vision
Goldcoin aims to become the first cryptocurrency where AI manages the full software lifecycle -- from development through release -- with human oversight focused on governance and strategic direction.

## Network Statistics

- Genesis Block: May 2013
- Current Block Height: ~3,150,000+
- Network Uptime: 99.9% since launch
- Zero Security Breaches: Core protocol remains uncompromised

## Security

- 51% Defense: Golden River Protocol with pool competitiveness support
- Multi-signature Treasury: 1.1 billion GLC secured
- P2P Hardening: Compact block and headers processing protections

## Wallets & Tools

### Official Wallets
- Goldcoin Core — Full node wallet
- Goldcoin Android — Mobile wallet
- Electrum-GLC — Lightweight wallet

### Block Explorers
- Official Explorer
- Cryptoid

## Community

- Website: https://goldcoinproject.org
- Discord: https://discord.me/goldcoin
- Twitter/X: https://twitter.com/goldcoin
- Telegram: https://t.me/goldcoin
- Reddit: https://reddit.com/r/goldcoin

## License

Goldcoin Core is released under the terms of the MIT license. See COPYING or https://opensource.org/licenses/MIT for more information.

## Disclaimer

This is experimental software. Use at your own risk. See COPYING for license terms.

> "In Code We Trust"
