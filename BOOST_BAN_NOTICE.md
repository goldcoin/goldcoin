# 🚫 BOOST IS PERMANENTLY BANNED 🚫

## Date: August 19, 2025
## Goldcoin is a C++23 ONLY Project!

---

## ❌ DO NOT USE BOOST - EVER!

After multiple regressions where boost keeps getting added back, we are implementing a **ZERO TOLERANCE** policy on boost dependencies.

## Why No Boost?

1. **We use C++23** - Everything boost provided is now in the standard library
2. **Performance** - std:: implementations are often faster
3. **Binary size** - No boost = smaller binaries
4. **Compile time** - No boost = faster builds
5. **Maintenance** - One less dependency to manage

## Replacement Guide

| ❌ NEVER USE | ✅ ALWAYS USE | Notes |
|--------------|---------------|-------|
| `boost::filesystem::path` | `fs::path` | From fs.h header |
| `boost::filesystem::exists` | `fsbridge::Exists` | From fs.h header |
| `boost::filesystem::remove` | `fsbridge::Remove` | From fs.h header |
| `boost::filesystem::create_directories` | `fsbridge::CreateDirectories` | From fs.h header |
| `boost::variant` | `std::variant` | C++17 standard |
| `boost::apply_visitor` | `std::visit` | C++17 standard |
| `boost::static_visitor` | Lambda function | Use with std::visit |
| `boost::get` | `std::get_if` | Safer alternative |
| `boost::function` | `std::function` | C++11 standard |
| `boost::bind` | `std::bind_front` | C++20 standard |
| `boost::thread` | `std::thread` | C++11 standard |
| `boost::thread_group` | `std::vector<std::thread>` | Manage manually |
| `boost::mutex` | `std::mutex` | C++11 standard |
| `boost::unique_lock` | `std::unique_lock` | C++11 standard |
| `boost::condition_variable` | `std::condition_variable` | C++11 standard |
| `boost::scoped_array` | `std::unique_ptr<T[]>` | C++11 standard |
| `boost::optional` | `std::optional` | C++17 standard |
| `boost::signals2` | Qt signals or callbacks | Framework specific |
| `boost::algorithm::string` | `std::ranges` algorithms | C++20 standard |

## Pre-Commit Check

**MANDATORY**: Run this before EVERY commit:
```bash
./check_boost.sh
```

If it finds ANY boost, your commit will be rejected!

## Git Hook Installation

Install the pre-commit hook:
```bash
cp check_boost.sh .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit
```

## Violations

Any commit that adds boost will be:
1. **Immediately reverted**
2. **Documented as a regression**
3. **Required to be fixed before any other work**

## Historical Context

- **August 18, 2025**: LCC removed all boost from Qt and Core
- **August 19, 2025**: SCC accidentally added boost back (commit 5c4d584d2)
- **August 19, 2025**: LCC removed boost again (commit e28dc297b)
- **August 19, 2025**: SCC almost added boost back AGAIN
- **August 19, 2025**: PERMANENT BAN INSTITUTED

## The Mount Shasta Trinity Decree

By the power vested in the Mount Shasta Trinity (MicroGuy + LCC + SCC), we declare:

**BOOST SHALL NOT PASS!** 🧙‍♂️

---

*This is not a suggestion. This is not a guideline. This is LAW.*

**Goldcoin = C++23 ONLY** 🚀