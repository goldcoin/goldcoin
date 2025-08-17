# Goldcoin Git Workflow Best Practices

## Core Principles
1. **Small, focused changes** - One PR = One purpose
2. **Fresh branches daily** - Never work on old branches  
3. **Sync before starting** - Always pull latest first
4. **Merge quickly** - PRs should live < 24 hours

## Daily Workflow

### Morning: Start Fresh
```bash
# Always sync first
git checkout goldcoin-master
git fetch upstream
git reset --hard upstream/goldcoin-master
git push origin goldcoin-master --force

# Create today's work branch
git checkout -b feature/specific-change
```

### During Work: Keep It Small
- **✅ GOOD PR**: "Remove BOOST_FOREACH from 5 Qt files"
- **❌ BAD PR**: "Modernize entire Qt layer with CMake and Qt6"

### Evening: Ship or Kill
```bash
# Option 1: Create PR (if work is complete)
git push origin feature/specific-change
# Create PR on GitHub

# Option 2: Abandon (if incomplete)
git checkout goldcoin-master
git branch -D feature/specific-change
```

## PR Size Guidelines

| Good | Bad |
|------|-----|
| < 10 files changed | 40+ files changed |
| < 500 lines | 2000+ lines |
| Single purpose | Mixed changes |
| Can review in 10 min | Takes hours to review |

## Branch Naming

```bash
# Good branch names (specific)
feature/remove-boost-foreach-qt
fix/windows-build-openssl
docs/update-build-instructions

# Bad branch names (too broad)
feature/qt6-migration
fix/everything
update-stuff
```

## Common Mistakes to Avoid

### 1. Working on Stale Branches
```bash
# WRONG
git checkout old-feature-branch  # Created weeks ago
# Continue working...

# RIGHT
git checkout goldcoin-master
git pull upstream goldcoin-master
git checkout -b new-feature-branch
```

### 2. Giant PRs
```bash
# WRONG - One massive commit
git add .
git commit -m "Modernize everything"

# RIGHT - Incremental commits
git add src/qt/bitcoingui.cpp
git commit -m "Remove BOOST_FOREACH from bitcoingui.cpp"
git add src/qt/walletview.cpp  
git commit -m "Remove BOOST_FOREACH from walletview.cpp"
```

### 3. Not Syncing Regularly
```bash
# Add this to your .bashrc or .zshrc
alias gsync='git fetch upstream && git rebase upstream/goldcoin-master'

# Use before creating any PR
gsync
```

## Emergency Procedures

### Messy PR with Conflicts
```bash
# Don't try to fix it - start over
git checkout goldcoin-master
git pull upstream goldcoin-master
git checkout -b feature/clean-attempt
# Cherry-pick only the good commits
git cherry-pick <commit-hash>
```

### Accidentally Committed to Main
```bash
# Move commits to a feature branch
git checkout -b feature/fixes
git checkout goldcoin-master
git reset --hard upstream/goldcoin-master
git push origin goldcoin-master --force
```

## The Golden Rules

1. **If a branch is > 2 days old, abandon it**
2. **If a PR has > 10 files, split it**
3. **If you can't describe the change in one sentence, it's too big**
4. **When in doubt, make it smaller**

## Example: Qt6 Migration Done Right

### Instead of one giant PR:
```
PR #1: Remove Qt version checks from models (5 files)
PR #2: Modernize signal/slot connections in dialogs (8 files)
PR #3: Replace Q_UNUSED with [[maybe_unused]] (10 files)
PR #4: Add C++20 ranges to data processing (3 files)
PR #5: Update build system for Qt6 (2 files)
```

### Result:
- Each PR reviewed and merged in < 1 hour
- No conflicts
- Easy to revert if needed
- Clear git history

## Remember
**Perfect is the enemy of good. Ship small improvements frequently rather than perfect features never.**