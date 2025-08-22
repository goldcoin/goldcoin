package=freetype
$(package)_version=2.13.3
$(package)_download_path=http://download.savannah.gnu.org/releases/$(package)
$(package)_file_name=$(package)-$($(package)_version).tar.xz
$(package)_sha256_hash=0550350666d427c74daeb85d5ac7bb353acba5f76956395995311a9c6f063289

define $(package)_set_vars
  $(package)_config_opts=--without-zlib --without-png --disable-static
  $(package)_config_opts_linux=--with-pic
  $(package)_cflags=-Wno-dangling-pointer -Wno-stringop-overflow
  $(package)_cxxflags=-Wno-dangling-pointer -Wno-stringop-overflow
  # Force libtool to use CC tag for GCC 15 compatibility
  $(package)_config_env=lt_cv_prog_compiler_c_o=yes lt_cv_path_NM=nm
endef

define $(package)_config_cmds
  $($(package)_autoconf) && \
  echo "LIBTOOLFLAGS = --tag=CC" >> builds/unix/unix-def.mk && \
  sed -i 's|./builds/unix/libtool|./builds/unix/libtool --tag=CC|g' builds/freetype.mk || true
endef

define $(package)_build_cmds
  $(MAKE) LIBTOOL="./builds/unix/libtool --tag=CC"
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef
