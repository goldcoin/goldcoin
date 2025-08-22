package=xcb_proto
$(package)_version=1.17.0
$(package)_download_path=https://xcb.freedesktop.org/dist
$(package)_file_name=xcb-proto-$($(package)_version).tar.xz
$(package)_sha256_hash=2c1bacd2110f4799f74de6ebb714b94cf6f80fb112316b1219480fd22562148c

define $(package)_set_vars
  $(package)_config_opts=--disable-shared
  $(package)_config_opts_linux=--with-pic
endef

define $(package)_config_cmds
  $($(package)_autoconf)
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  find -name "*.pyc" -delete && \
  find -name "*.pyo" -delete
endef
