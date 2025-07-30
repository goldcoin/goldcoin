# Goldcoin Core Deprecation Plan

## Overview

This document outlines the comprehensive plan for removing deprecated features from Goldcoin Core over a 24-month period. The goal is to reduce code bloat, improve security, and modernize the codebase while maintaining ecosystem compatibility.

## Executive Summary

- **Timeline**: 24 months (3 phases)
- **Code Reduction**: ~1000+ lines
- **Binary Size**: 5-10% reduction
- **Risk Level**: Low (phased approach)
- **Ecosystem Impact**: Managed transition with migration tools

## Current State Analysis

### Deprecated Features Identified

| Feature | Location | Lines of Code | Last Used | Replacement |
|---------|----------|---------------|-----------|-------------|
| `estimatepriority` RPC | rpc/mining.cpp:799 | ~50 | Bitcoin 0.14 | Fee estimation |
| `estimatesmartpriority` RPC | rpc/mining.cpp:863 | ~30 | Bitcoin 0.14 | Smart fee estimation |
| `getinfo` RPC | rpc/misc.cpp:47 | ~100 | Bitcoin 0.16 | getblockchaininfo, getnetworkinfo, getwalletinfo |
| Account system | rpc/wallet.cpp | ~500+ | Bitcoin 0.17 | Label system |
| Transaction priority | validation.cpp | ~200 | Network-wide | Fee-based prioritization |
| Legacy protocol versions | protocol.h | ~50 | Various | Modern protocol versions |

### Code Bloat Areas

| Component | Size | Optional | Removal Potential |
|-----------|------|----------|-------------------|
| Qt/GUI | 464MB | Yes | 50% binary reduction if disabled |
| Wallet | 796KB | Conditional | Modular compilation |
| Test data | ~50MB | No | 10-20% reduction possible |
| Boost dependencies | Various | Partial | Replace with std:: equivalents |

## Deprecation Timeline

### Phase 1: Warning Period (v0.15.1 - 3 months)

**Objective**: Educate users and provide migration notice

**Actions**:
1. Add deprecation warnings to RPC calls
2. Create migration documentation
3. Notify ecosystem stakeholders
4. Remove truly obsolete features (no warning needed)

**Impact**: Educational, no breaking changes

### Phase 2: Disabled by Default (v0.16.0 - 12 months)

**Objective**: Transition period with opt-in deprecated features

**Actions**:
1. Disable deprecated RPCs by default
2. Add compile-time flags (`--enable-deprecated-rpc`)
3. Add runtime flags (`--deprecatedrpc`)
4. Provide migration tools and scripts

**Impact**: Breaking changes for unprepared users, but recoverable

### Phase 3: Complete Removal (v0.17.0 - 24 months)

**Objective**: Clean codebase with deprecated features removed

**Actions**:
1. Remove deprecated code entirely
2. Clean up related tests and documentation
3. Optimize binary size and performance
4. Update all references and help text

**Impact**: Permanent removal, smaller and cleaner codebase

## Implementation Details

### Phase 1 Implementation

#### Immediate Removals (No Warning Period)
```cpp
// Remove from rpc/mining.cpp
- estimatepriority() function (~50 lines)
- estimatesmartpriority() function (~30 lines)

// Reason: Bitcoin removed these in 0.15, replaced by fee estimation
// Risk: None - no known usage in modern software
```

#### Add Deprecation Warnings
```cpp
// In rpc/misc.cpp
UniValue getinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0) {
        LogPrintf("WARNING: getinfo is DEPRECATED and will be removed in v0.16.0\n");
        LogPrintf("Migration: Use getblockchaininfo, getnetworkinfo, getwalletinfo\n");
        throw std::runtime_error(
            "getinfo\n"
            "\nDEPRECATED. This call will be removed in v0.16.0\n"
            "Use getblockchaininfo, getnetworkinfo, and getwalletinfo instead.\n"
        );
    }
    // ... existing implementation
}
```

### Phase 2 Implementation

#### Compile-Time Controls
```bash
# Add to configure.ac
AC_ARG_ENABLE([deprecated-rpc],
    [AS_HELP_STRING([--enable-deprecated-rpc],
    [Enable deprecated RPC calls (default is no)])],
    [enable_deprecated_rpc=$enableval],
    [enable_deprecated_rpc=no])

if test "x$enable_deprecated_rpc" = "xyes"; then
    AC_DEFINE([ENABLE_DEPRECATED_RPC], [1], [Define to enable deprecated RPC calls])
fi
```

#### Runtime Controls
```cpp
// In init.cpp
bool fDeprecatedRPC = gArgs.GetBoolArg("-deprecatedrpc", false);

// In rpc/server.cpp
#ifdef ENABLE_DEPRECATED_RPC
if (fDeprecatedRPC) {
    RegisterDeprecatedRPCCommands();
}
#endif
```

### Phase 3 Implementation

#### Complete Removal Checklist
- [ ] Remove deprecated function implementations
- [ ] Remove from RPC command tables
- [ ] Clean up related test cases
- [ ] Update documentation and help text
- [ ] Remove conditional compilation flags
- [ ] Update release notes

## Migration Tools and Documentation

### 1. RPC Migration Guide

Create `doc/rpc-migration.md`:
```markdown
# RPC Migration Guide

## getinfo Replacement
| getinfo field | New RPC call | New field |
|---------------|--------------|-----------|
| .version | getnetworkinfo | .version |
| .protocolversion | getnetworkinfo | .protocolversion |
| .blocks | getblockchaininfo | .blocks |
| .timeoffset | getnetworkinfo | .timeoffset |
| .connections | getnetworkinfo | .connections |
| .difficulty | getblockchaininfo | .difficulty |
| .testnet | getblockchaininfo | .chain |
| .keypoololdest | getwalletinfo | .keypoololdest |
| .keypoolsize | getwalletinfo | .keypoolsize |
| .balance | getwalletinfo | .balance |
| .errors | getnetworkinfo | .warnings |

## Account System Migration
| Old RPC | New RPC | Notes |
|---------|---------|--------|
| getaccount | getaddressesbylabel | Use address labels |
| setaccount | setlabel | Single label per address |
| getaccountaddress | getaddressesbylabel | List addresses by label |
| listaccounts | listlabels | Get all labels |
| move | N/A | Use send/receive transactions |
```

### 2. Automated Migration Scripts

Create `contrib/rpc-migration/`:
```bash
#!/bin/bash
# migrate-rpc-calls.sh
# Converts scripts using deprecated RPC calls

echo "Goldcoin RPC Migration Tool"
echo "Converting deprecated RPC calls in: $1"

if [ ! -f "$1" ]; then
    echo "Error: File not found"
    exit 1
fi

# Create backup
cp "$1" "$1.backup"

# Convert getinfo calls
sed -i 's/goldcoin-cli getinfo/echo "Use: goldcoin-cli getblockchaininfo getnetworkinfo getwalletinfo"/g' "$1"

# Convert account calls
sed -i 's/getaccount/getaddressesbylabel/g' "$1"
sed -i 's/setaccount/setlabel/g' "$1"

echo "Migration complete. Backup saved as $1.backup"
echo "Please review changes manually."
```

### 3. Python Migration Library

Create `contrib/rpc-migration/goldcoin_migrate.py`:
```python
#!/usr/bin/env python3
"""
Goldcoin RPC Migration Library
Provides compatibility layer for deprecated RPC calls
"""

class GoldcoinMigrationProxy:
    def __init__(self, rpc_connection):
        self.rpc = rpc_connection
    
    def getinfo(self):
        """Legacy getinfo implementation using new RPC calls"""
        blockchain = self.rpc.getblockchaininfo()
        network = self.rpc.getnetworkinfo()
        
        try:
            wallet = self.rpc.getwalletinfo()
        except:
            wallet = {}
        
        return {
            'version': network.get('version'),
            'protocolversion': network.get('protocolversion'),
            'blocks': blockchain.get('blocks'),
            'difficulty': blockchain.get('difficulty'),
            'connections': network.get('connections'),
            'balance': wallet.get('balance', 0),
            # Add other fields as needed
        }
```

## Communication Strategy

### Stakeholder Notification Timeline

| Timeframe | Stakeholders | Method | Content |
|-----------|--------------|--------|---------|
| Phase 1 Start | Major exchanges | Direct email | Deprecation schedule, migration guide |
| Phase 1 Start | Mining pools | GitHub issue + email | Technical details, timeline |
| Phase 1 Start | Developer community | GitHub announcement | Full deprecation plan |
| Phase 1 Start | Users | Release notes | User-facing changes |
| Phase 2 Start | All stakeholders | Reminder notifications | 12-month warning |
| Phase 3 Start | All stakeholders | Final notice | 6-month countdown |

### Support Resources

1. **Migration Support Channel**
   - Dedicated GitHub discussions thread
   - Technical assistance for integrators
   - Community-driven migration examples

2. **Documentation Updates**
   - Updated RPC documentation
   - Migration examples for common use cases
   - Best practices for new implementations

3. **Testing Support**
   - Test cases for migration verification
   - Compatibility test suite
   - Integration testing guidance

## Risk Assessment and Mitigation

### High Risk Scenarios
1. **Critical Exchange Infrastructure**
   - Risk: Exchange unable to migrate before deadline
   - Mitigation: Direct support, extended timeline if needed

2. **Mining Pool Dependencies**
   - Risk: Pool software relies on deprecated features
   - Mitigation: Early engagement, custom migration assistance

3. **User Script Breakage**
   - Risk: Widespread script failures after removal
   - Mitigation: Comprehensive migration tools, clear warnings

### Fallback Plan
If critical infrastructure cannot migrate by the deadline:
1. Extend timeline by 6 months for specific features
2. Provide compatibility shim library
3. Offer direct technical assistance to affected parties
4. Consider feature-specific extensions on case-by-case basis

## Success Metrics

### Quantitative Metrics
- Lines of code removed: Target 1000+
- Binary size reduction: Target 5-10%
- Compilation time improvement: Target 5-15%
- Test suite execution time: Target improvement

### Qualitative Metrics
- Community feedback on migration process
- Ecosystem readiness assessment
- Developer satisfaction with cleaner codebase
- Reduced maintenance burden

## Testing Strategy

### Phase 1 Testing
- Verify deprecation warnings appear correctly
- Ensure deprecated features still function
- Test migration documentation accuracy

### Phase 2 Testing
- Verify compilation with/without deprecated features
- Test runtime flag functionality
- Validate migration tool effectiveness

### Phase 3 Testing
- Confirm complete removal of deprecated code
- Verify no regression in core functionality
- Test binary size and performance improvements

## Documentation Updates Required

1. **User Documentation**
   - Update RPC API documentation
   - Create migration guides
   - Update example scripts

2. **Developer Documentation**
   - Update build instructions
   - Document new compilation flags
   - Update contribution guidelines

3. **Release Documentation**
   - Comprehensive release notes for each phase
   - Migration timeline documentation
   - Breaking changes documentation

## Conclusion

This deprecation plan provides a structured, low-risk approach to removing deprecated features from Goldcoin Core. The phased approach ensures ecosystem compatibility while achieving the goal of a cleaner, more maintainable codebase.

The 24-month timeline provides ample opportunity for stakeholders to migrate, while the provided tools and documentation minimize the burden of transition. Regular communication and support throughout the process will ensure successful adoption of the changes.

---

**Document Version**: 1.0  
**Last Updated**: 2025-07-28  
**Next Review**: Phase 1 implementation completion  
**Approved By**: Lead Developer  