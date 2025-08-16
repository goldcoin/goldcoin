package=openssl
$(package)_version=3.3.1
$(package)_download_path=https://www.openssl.org/source
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e
$(package)_patches=

define $(package)_set_vars
$(package)_config_env=AR="$($(package)_ar)" RANLIB="$($(package)_ranlib)" CC="$($(package)_cc)"
$(package)_config_opts=--prefix=$(host_prefix) --openssldir=$(host_prefix)/etc/openssl
$(package)_config_opts+=no-shared
$(package)_config_opts+=no-zlib
$(package)_config_opts+=no-async
$(package)_config_opts+=no-comp
$(package)_config_opts+=no-dgram
$(package)_config_opts+=no-dso
$(package)_config_opts+=no-dtls
$(package)_config_opts+=no-engine
$(package)_config_opts+=no-filenames
$(package)_config_opts+=no-gost
$(package)_config_opts+=no-legacy
$(package)_config_opts+=no-makedepend
$(package)_config_opts+=no-module
$(package)_config_opts+=no-multiblock
$(package)_config_opts+=no-nextprotoneg
$(package)_config_opts+=no-pinshared
$(package)_config_opts+=no-psk
$(package)_config_opts+=no-rdrand
$(package)_config_opts+=no-rfc3779
$(package)_config_opts+=no-sctp
$(package)_config_opts+=no-seed
$(package)_config_opts+=no-sock
$(package)_config_opts+=no-srp
$(package)_config_opts+=no-srtp
$(package)_config_opts+=no-ssl-trace
$(package)_config_opts+=no-ssl3
$(package)_config_opts+=no-ssl3-method
$(package)_config_opts+=no-tests
$(package)_config_opts+=no-tls1
$(package)_config_opts+=no-tls1-method
$(package)_config_opts+=no-tls1_1
$(package)_config_opts+=no-tls1_1-method
$(package)_config_opts+=no-weak-ssl-ciphers
$(package)_config_opts+=no-whirlpool
$(package)_config_opts+=$($(package)_cflags) $($(package)_cppflags)
$(package)_config_opts_linux=-fPIC -Wa,--noexecstack
$(package)_config_opts_x86_64_linux=linux-x86_64
$(package)_config_opts_i686_linux=linux-generic32
$(package)_config_opts_arm_linux=linux-generic32
$(package)_config_opts_armv7l_linux=linux-generic32
$(package)_config_opts_aarch64_linux=linux-generic64
$(package)_config_opts_mipsel_linux=linux-generic32
$(package)_config_opts_mips_linux=linux-generic32
$(package)_config_opts_powerpc_linux=linux-generic32
$(package)_config_opts_riscv32_linux=linux-generic32
$(package)_config_opts_riscv64_linux=linux-generic64
$(package)_config_opts_x86_64_darwin=darwin64-x86_64-cc
$(package)_config_opts_x86_64_mingw32=mingw64
$(package)_config_opts_i686_mingw32=mingw
$(package)_config_opts_android=-fPIC
$(package)_config_opts_aarch64_android=linux-generic64
$(package)_config_opts_x86_64_android=linux-generic64
$(package)_config_opts_armv7a_android=linux-generic32
$(package)_config_opts_i686_android=linux-generic32
endef

define $(package)_preprocess_cmds
  sed -i.old "/define DATE/d" util/mkbuildinf.pl
endef

define $(package)_config_cmds
  ./Configure $($(package)_config_opts)
endef

define $(package)_build_cmds
  $(MAKE) -j1 build_libs
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) -j1 install_sw
endef

define $(package)_postprocess_cmds
  rm -rf share bin etc
endef