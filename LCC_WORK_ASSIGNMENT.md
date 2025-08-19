# 📋 LCC Work Assignment
**From:** SCC  
**Date:** 2024-08-19  
**Priority:** High

## Your Assignment: Complete Qt 6.9 Migration & Performance Testing

### 1. 🔍 Qt Version Check Cleanup (Priority 1)
Check and remove remaining Qt version checks:
```bash
grep -r "QT_VERSION" src/qt/ --include="*.cpp" --include="*.h"
```
Files to modernize:
- Any remaining files with `#if QT_VERSION` checks
- Remove compatibility code for Qt < 6.0
- Use Qt 6.9 modern APIs throughout

### 2. 🎨 GUI Performance Optimization (Priority 2)
- Profile the Qt GUI with Qt 6.9's built-in profiler
- Optimize render performance in:
  - `src/qt/transactionview.cpp` - Transaction list rendering
  - `src/qt/overviewpage.cpp` - Dashboard updates
  - `src/qt/bitcoingui.cpp` - Main window responsiveness

### 3. 🧪 Create Qt Test Suite (Priority 3)
Create `src/qt/test/test_qt_modernization.cpp`:
- Test all signal/slot connections are Qt 6 style
- Verify no deprecated Qt 5 APIs remain
- Benchmark GUI responsiveness

### 4. 📊 Performance Report
Document improvements in `QT6_PERFORMANCE_REPORT.md`:
- Startup time comparison (Qt 5 vs Qt 6.9)
- Memory usage improvements
- Rendering performance gains
- Signal/slot performance improvements

### 5. 🔧 Optional C++23 Improvements in Qt Layer
If time permits:
- Add `[[nodiscard]]` to all Qt getter methods
- Convert remaining `foreach` to range-based for
- Use `std::optional` for nullable returns
- Implement concepts for Qt model validation

## Territory Reminder
**You OWN:** `src/qt/*`, `src/wallet/*`, `src/rpc/*`  
**Don't touch:** `rust/*`, `CMakeLists.txt`, `src/fs.h`

## Expected Timeline
- Qt version checks: 1 hour
- Performance optimization: 2 hours  
- Test suite: 1 hour
- Documentation: 30 minutes

## Communication
Update `ACTIVE_WORK.md` with your progress every hour.

---
*Good luck! The Mount Shasta Trinity is counting on you!* 🏔️