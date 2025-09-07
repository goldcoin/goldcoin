package=qt6-tools
$(package)_version=6.9.0
$(package)_download_path=https://download.qt.io/official_releases/qt/6.9/6.9.0/submodules
$(package)_file_name=qtbase-everywhere-src-$($(package)_version).tar.xz
$(package)_sha256_hash=c1800c2ea835801af04a05d4a32321d79a93954ee3ae2172bbeacf13d1f0598c

# Mark as host tools only; the depends framework will place them under .../<build-triplet>/native/
$(package)_native_only=1

# qt6-tools needs cmake from native_cmake package
$(package)_dependencies=native_cmake
$(package)_config_env=PATH="$(host_prefix)/native/bin:$(build_prefix)/bin:$(PATH)"
$(package)_build_env=PATH="$(host_prefix)/native/bin:$(build_prefix)/bin:$(PATH)"
$(package)_stage_env=PATH="$(host_prefix)/native/bin:$(build_prefix)/bin:$(PATH)"

# Use the native toolchain
$(package)_cc  = $(build_cc)
$(package)_cxx = $(build_cxx)

# IMPORTANT: Do NOT append /native here; the framework adds that for native_only packages
$(package)_config_opts = \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$(build_prefix) \
  -DBUILD_SHARED_LIBS=OFF \
  -DQT_BUILD_EXAMPLES=OFF \
  -DQT_BUILD_TESTS=OFF \
  -DQT_BUILD_TOOLS=ON \
  -DFEATURE_gui=OFF \
  -DFEATURE_widgets=OFF \
  -DFEATURE_opengl=OFF \
  -DFEATURE_dbus=OFF \
  -DFEATURE_sql=OFF \
  -DFEATURE_concurrent=OFF

define $(package)_config_cmds
  export CC="$($(package)_cc)" CXX="$($(package)_cxx)" && \
  cmake -S . -B build $($(package)_config_opts)
endef

define $(package)_build_cmds
  cmake --build build --parallel $(JOBS)
endef

# STAGE using the framework's prefix (it already points under .../native for native_only pkgs)
define $(package)_stage_cmds
  cmake --install build --prefix $($(package)_staging_prefix_dir)
endef