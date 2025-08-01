# Goldcoin Architecture Decisions Log

## Version: v0.16.1-dev

### 2025-07-30: Deprecated Code Removal Strategy

**Decision**: Complete removal of wallet account system
**Rationale**: 
- The account system has been deprecated for multiple versions
- It adds unnecessary complexity to the wallet code
- Modern wallets use label-based organization instead
- Removing 10+ deprecated RPCs will simplify the codebase

**Implementation Plan**:
1. Remove all account-related RPCs from src/wallet/rpcwallet.cpp
2. Remove account parameter from remaining wallet RPCs
3. Update wallet database to remove account fields
4. Update tests to remove account-based test cases

---

### 2025-07-30: Boost Dependency Modernization

**Decision**: Systematic replacement of Boost with std:: equivalents
**Priority Order**:
1. boost::filesystem → std::filesystem (C++17)
2. boost::bind → std::bind or lambdas
3. boost::function → std::function
4. boost::shared_ptr → std::shared_ptr

**Rationale**:
- Reduces external dependencies
- Improves compilation times
- Better compiler optimization opportunities
- Future-proofs the codebase for C++17/20

**Challenges**:
- std::filesystem requires C++17 (currently on C++11)
- Need to ensure backward compatibility
- May need conditional compilation for older systems

---

### 2025-07-30: Gitian Build Resource Management

**Decision**: Limit Gitian builds to 2 cores and 2GB RAM
**Rationale**:
- Unlimited resource usage was causing 114% CPU usage
- This was crashing SSH sessions during builds
- Conservative limits ensure system stability

**Implementation**:
```bash
gbuild --num-make 2 --memory 2000
```

---

### 2025-07-29: Python Modernization

**Decision**: Upgrade to Python 3.12 with native crypto bindings
**Rationale**:
- External goldcoin_scrypt package incompatible with Python 3.10+
- Native pybind11 implementation provides better control
- Eliminates dependency on unmaintained external package

**Implementation**:
- Created python-bindings/goldcoin_crypto.cpp
- Uses pybind11 for Python/C++ interface
- Falls back to SHA256 if native extension unavailable

---

### Future Architecture Considerations

1. **Quantum Resistance**:
   - Planning post-quantum cryptography implementation
   - Will require new signature schemes
   - Need to maintain backward compatibility

2. **Modular Architecture**:
   - Consider splitting monolithic files
   - Improve separation of concerns
   - Make testing easier

3. **API Modernization**:
   - Design RESTful API to replace some RPC methods
   - Consider GraphQL for complex queries
   - Improve developer experience