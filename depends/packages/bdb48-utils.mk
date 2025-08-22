package=bdb48-utils
$(package)_version=4.8.30.NC
$(package)_download_path=https://download.oracle.com/berkeley-db
$(package)_file_name=db-$($(package)_version).tar.gz
$(package)_sha256_hash=12edc0df75bf9abd7f82f821795bcee50f42cb2e5f76a6a281b85732798364ef
$(package)_build_subdir=build_unix

define $(package)_set_vars
$(package)_config_opts=--disable-shared --enable-cxx --disable-replication --enable-compat185
$(package)_config_opts_mingw32=--enable-mingw
$(package)_config_opts_linux=--with-pic
$(package)_cxxflags=-std=c++17
endef

define $(package)_config_cmds
  ../dist/configure $($(package)_config_opts)
endef

define $(package)_build_cmds
  $(MAKE) -C . && \
  cd .. && \
  $(MAKE) -C db_dump && \
  $(MAKE) -C db_load && \
  $(MAKE) -C db_verify
endef

define $(package)_stage_cmds
  mkdir -p $($(package)_staging_prefix_dir)/bin && \
  cp -f db_dump $($(package)_staging_prefix_dir)/bin/db48_dump && \
  cp -f db_load $($(package)_staging_prefix_dir)/bin/db48_load && \
  cp -f db_verify $($(package)_staging_prefix_dir)/bin/db48_verify
endef