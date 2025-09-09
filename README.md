# Goldcoin (GLC) - The AI-Autonomous Cryptocurrency

[![Version](https://img.shields.io/badge/version-0.17.0--dev-blue.svg)](https://github.com/microguy/goldcoin/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Discord](https://img.shields.io/badge/Discord-Join%20Chat-7289DA)](https://discord.me/goldcoin)

## The World's First AI-Autonomous Cryptocurrency

Goldcoin is pioneering the transition from human-controlled to fully AI-autonomous cryptocurrency. With proven technical performance since 2013 (1,120 TPS, zero fees, instant confirmations), we are building toward complete AI autonomy by 2027.

### Key Features
- 1,120 Transactions Per Second — approximately 160x Bitcoin’s capacity
- Zero Transaction Fees — completely free to send
- 2-Minute Confirmations — approximately 5x faster than Bitcoin
- 32 MB Blocks — 32x larger than Bitcoin
- AI-Driven Development — 60% autonomous today, targeting 100% by 2027
- 100-Year Treasury — 1.1 billion GLC fund for autonomous operations

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

### Phase 1 — AI-Assisted (2025, current)
- AI handles ~60% of development tasks
- Migration work (e.g., Qt 6.9) executed by AI agents
- Humans provide strategic direction and guardrails

### Phase 2 — AI-Led (2026)
- AI makes ~90% of decisions autonomously
- Automated release management and incident response
- Human role reduced to funding and high-level oversight

### Phase 3 — Full Autonomy (2027)
- 100% AI operation achieved
- Zero human involvement required
- Self-sustaining operations for 100+ years

## Development Status

### Current Release: v0.15.0
- Stable release with core functionality

### In Development: v0.17.0-beta1
- Complete C++23 modernization with Boost elimination
- Automatic BDB 4.8 → 18.1 wallet migration
- Qt 6.9 interface with modern frameworks
- OpenSSL 3.5.2 LTS and secp256k1 v0.6.0 cryptographic modernization
- Enterprise CMake build system with static linking

### Upcoming: v0.18.0
- Quantum-resistant signatures
- AI consensus protocol
- Self-healing network features

## Building from Source

### Prerequisites (Ubuntu/Debian)
```bash
sudo apt-get install -y build-essential cmake pkg-config libssl-dev libevent-dev
sudo apt-get install -y libdb-dev libdb++-dev
sudo apt-get install -y qtbase5-dev qttools5-dev qttools5-dev-tools
```

### Quick Build (Unix)
```bash
git clone https://github.com/goldcoin/goldcoin.git
cd goldcoin
mkdir build && cd build
cmake ..
make -j"$(nproc)"
# Optional:
sudo make install
```

### Cross-Compile for Windows
```bash
make -C depends HOST=x86_64-w64-mingw32 -j"$(nproc)"
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../depends/x86_64-w64-mingw32/share/toolchain.cmake ..
make -j"$(nproc)"
```

## Testing
```bash
# Unit tests
make check

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
By 2027, Goldcoin will operate entirely through AI consensus, becoming the first cryptocurrency to achieve complete independence from human control while maintaining decentralization and security.

## Network Statistics

- Genesis Block: May 2013
- Current Block Height: ~3,150,000+
- Network Uptime: 99.9% since launch
- Zero Security Breaches: Core protocol remains uncompromised

## Security

- Dual 51% Defense: Golden River Protocol
- Quantum Resistance: Coming in v0.18.0
- Multi-signature Treasury: 1.1 billion GLC secured
- Automated Security Response: AI-driven threat detection

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

This is experimental software. By 2027, human control will be permanently relinquished as Goldcoin transitions to full AI autonomy. Use at your own risk.

> "In Code We Trust, In AI We Evolve"
Building the autonomous future of money, one block at a time.
