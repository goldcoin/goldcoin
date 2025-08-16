package=qt6
$(package)_version=6.9.0
$(package)_download_path=https://download.qt.io/archive/qt/6.9/$($(package)_version)/submodules
$(package)_suffix=everywhere-src-$($(package)_version).tar.xz
$(package)_file_name=qtbase-$($(package)_suffix)
$(package)_sha256_hash=c1800c2ea835801af04a05d4a32321d79a93954ee3ae2172bbeacf13d1f0598c
$(package)_dependencies=openssl zlib
$(package)_native_dependencies=native_cmake
$(package)_linux_dependencies=freetype fontconfig libxcb libX11 xproto libXext
$(package)_patches=qt6-windows-exe-suffix.patch
# Build happens in extracted directory
$(package)_qt_libs=corelib network widgets gui plugins

# Extra modules disabled for initial build
# $(package)_qttranslations_file_name=qttranslations-$($(package)_suffix)
# $(package)_qttools_file_name=qttools-$($(package)_suffix)

define $(package)_set_vars
$(package)_config_opts_release = -release
$(package)_config_opts_debug = -debug
$(package)_config_opts += -prefix $(host_prefix)
$(package)_config_opts += -opensource
$(package)_config_opts += -confirm-license
$(package)_config_opts += -static
$(package)_config_opts += -no-feature-cups
$(package)_config_opts += -no-feature-dbus
$(package)_config_opts += -no-feature-sql
$(package)_config_opts += -no-feature-testlib
# Qt6 doesn't have qml-debug feature
$(package)_config_opts += -nomake examples
$(package)_config_opts += -nomake tests
# Removed Qt5-specific options
$(package)_config_opts += -qt-libpng
$(package)_config_opts += -qt-libjpeg
$(package)_config_opts += -system-zlib
$(package)_config_opts += -no-feature-printsupport
$(package)_config_opts += -no-feature-concurrent
$(package)_config_opts += -no-feature-xml

# Platform-specific options
# Disable schannel for cross-compilation issues
$(package)_config_opts_mingw32 += -no-feature-schannel
$(package)_config_opts_linux += -no-feature-openssl
$(package)_config_opts_linux += -system-freetype
$(package)_config_opts_linux += -fontconfig
$(package)_config_opts_linux += -xcb
$(package)_config_opts_linux += -no-opengl
$(package)_config_opts_linux += -platform linux-g++

$(package)_config_opts_mingw32 += -no-opengl
# Direct2D options not needed for Qt6
$(package)_config_opts_mingw32 += -platform win32-g++
$(package)_config_opts_mingw32 += -device-option CROSS_COMPILE="$(host)-"

# Darwin options
ifneq ($(build_os),darwin)
$(package)_config_opts_darwin = -platform macx-clang-linux
$(package)_config_opts_darwin += -device-option MAC_SDK_PATH=$(OSX_SDK)
$(package)_config_opts_darwin += -device-option MAC_SDK_VERSION=$(OSX_SDK_VERSION)
$(package)_config_opts_darwin += -device-option CROSS_COMPILE="$(host)-"
$(package)_config_opts_darwin += -device-option MAC_MIN_VERSION=$(OSX_MIN_VERSION)
$(package)_config_opts_darwin += -device-option MAC_TARGET=$(host)
endif

$(package)_config_opts_arm_linux = -platform linux-g++ -xplatform $(host)
$(package)_config_opts_i686_linux = -xplatform linux-g++-32

$(package)_build_env = QT_RCC_TEST=1
$(package)_build_env += CMAKE_PREFIX_PATH="$(build_prefix)/lib/cmake"
endef

define $(package)_fetch_cmds
$(call fetch_file,$(package),$($(package)_download_path),$($(package)_download_file),$($(package)_file_name),$($(package)_sha256_hash))
endef

define $(package)_extract_cmds
  mkdir -p $($(package)_extract_dir) && \
  echo "$($(package)_sha256_hash)  $($(package)_source)" > $($(package)_extract_dir)/.$($(package)_file_name).hash && \
  $(build_SHA256SUM) -c $($(package)_extract_dir)/.$($(package)_file_name).hash && \
  tar --strip-components=1 -xf $($(package)_source)
endef

define $(package)_preprocess_cmds
  patch -p1 < $($(package)_patch_dir)/qt6-windows-exe-suffix.patch
endef

define $(package)_config_cmds
  export PKG_CONFIG_SYSROOT_DIR=/ && \
  export PKG_CONFIG_LIBDIR=$(host_prefix)/lib/pkgconfig && \
  export PKG_CONFIG_PATH=$(host_prefix)/share/pkgconfig  && \
  export CMAKE_PREFIX_PATH="$(build_prefix)/lib/cmake" && \
  export OPENSSL_LIBS="-L$(host_prefix)/lib -lssl -lcrypto" && \
  export CC="$($(package)_cc)" && \
  export CXX="$($(package)_cxx)" && \
  ./configure $($(package)_config_opts) -I $(host_prefix)/include -L $(host_prefix)/lib
endef

define $(package)_build_cmds
  $(MAKE) -j$(JOBS)
endef

define $(package)_stage_cmds
  $(MAKE) INSTALL_ROOT=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  rm -rf lib/cmake/Qt6*/Tests && \
  rm -rf lib/cmake/Qt6Test && \
  rm -rf lib/cmake/Qt6TestLib
endef