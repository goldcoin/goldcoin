package=native_cmake
$(package)_version=3.28.1
$(package)_download_path=https://github.com/Kitware/CMake/releases/download/v$($(package)_version)
$(package)_file_name=cmake-$($(package)_version).tar.gz
$(package)_sha256_hash=15e94f83e647f7d620a140a7a5da76349fc47a1bfed66d0f5cdee8e7344079ad

define $(package)_set_vars
$(package)_config_opts = --prefix=$(build_prefix)
$(package)_config_opts += --parallel=$(JOBS)
$(package)_config_opts += -- -DCMAKE_USE_OPENSSL=OFF
$(package)_config_opts += -DBUILD_TESTING=OFF
$(package)_config_opts += -DBUILD_CursesDialog=OFF
endef

define $(package)_config_cmds
  ./bootstrap $($(package)_config_opts)
endef

define $(package)_build_cmds
  $(MAKE) -j$(JOBS)
endef

define $(package)_stage_cmds
  $(MAKE) install DESTDIR=$($(package)_staging_dir)
endef