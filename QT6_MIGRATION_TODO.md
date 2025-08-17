# Qt 6.9 Migration - Remaining Work for Windows

## Status
- **Branch**: `feature/qt6-migration`  
- **Analysis Date**: 2025-08-17
- **Current State**: Build system configured for Qt6/C++20, but source code contains Qt5 legacy code

## Critical Qt 6.9 Incompatibilities

### 1. Qt Version Conditional Code (45+ instances)
**Files affected**: Most files in `src/qt/`

- [ ] Remove all `#if QT_VERSION < 0x050000` blocks
- [ ] Remove Qt4 plugin imports (`src/qt/bitcoin.cpp:68-75`)
- [ ] Update Qt5-specific version checks for Qt6 compatibility

### 2. Deprecated API Replacements Required

| File | Line | Old API | New Qt6 API |
|------|------|---------|-------------|
| `src/qt/bitcoin.cpp` | 699 | `qInstallMsgHandler()` | `qInstallMessageHandler()` |
| `src/qt/bitcoingui.cpp` | 244 | `QWindowsStyle`/`QWindowsXPStyle` | Remove check (styles don't exist in Qt6) |
| `src/qt/guiutil.cpp` | 291 | `Qt::escape()` | `QString::toHtmlEscaped()` |
| `src/qt/guiutil.cpp` | 336,387 | `QDesktopServices::storageLocation()` | `QStandardPaths::writableLocation()` |
| `src/qt/guiutil.cpp` | 530 | `horizontalHeader()->setResizeMode()` | `setSectionResizeMode()` |
| `src/qt/addressbookpage.cpp` | 158-159 | `horizontalHeader()->setResizeMode()` | `setSectionResizeMode()` |
| `src/qt/coincontroldialog.cpp` | 123 | `header()->setClickable()` | `setSectionsClickable()` |
| `src/qt/guiutil.cpp` | 1045,1058 | `label->pixmap()` pointer | Qt6 value semantics handling |

### 3. Build Configuration Updates

- [x] `CMakeLists.txt:492` correctly specifies Qt6 6.2+ (finds any Qt 6.x version including 6.9, 6.10+)
- [ ] Verify Qt6 component requirements (Core, Gui, Widgets, Network)

### 4. Windows-Specific Files

#### `src/qt/winshutdownmonitor.h/cpp`
- [ ] Remove `#if QT_VERSION >= 0x050000` checks
- [ ] Review OpenSSL 3.x compatibility for `RAND_event()` usage
- [ ] Update for Qt6 native event handling

## Files Requiring Most Changes

1. **src/qt/bitcoin.cpp** - Main application, Qt4/5 plugin system
2. **src/qt/bitcoingui.cpp** - Main window, style checks
3. **src/qt/guiutil.cpp** - Multiple deprecated APIs
4. **src/qt/addressbookpage.cpp** - Header view APIs
5. **src/qt/coincontroldialog.cpp** - Header clickable API
6. **src/qt/winshutdownmonitor.cpp** - Windows-specific Qt5 code

## Testing Requirements

- [ ] Build with Qt 6.9 on Windows (MinGW)
- [ ] Build with Qt 6.9 on Linux (native)
- [ ] Verify OpenSSL 3.3.1 compatibility
- [ ] Test Windows shutdown handling
- [ ] Verify all UI components render correctly without legacy styles

## Migration Strategy

1. **Phase 1**: Remove Qt4 compatibility code
2. **Phase 2**: Update Qt5 deprecated APIs to Qt6
3. **Phase 3**: Windows-specific testing and fixes
4. **Phase 4**: Final testing across platforms

## Notes

- The codebase currently supports Qt 4.7 through Qt 5.x
- CMake is already configured for C++20 (required for Qt 6.9)
- CMake correctly uses `find_package(Qt6 6.2...)` which finds any Qt 6.x version (6.9, 6.10, etc.)
- OpenSSL 3.3.1 is configured in the build system
- Most deprecated APIs have direct Qt6 replacements
- Windows style checks need complete removal as these styles don't exist in Qt6

## References

- Qt6 Porting Guide: https://doc.qt.io/qt-6/portingguide.html
- Qt6 Removed APIs: https://doc.qt.io/qt-6/obsoleteclasses.html
- Qt6.9 Documentation: https://doc.qt.io/qt-6/index.html