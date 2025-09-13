package=openssl
$(package)_version=3.5.2
$(package)_download_path=https://github.com/openssl/openssl/releases/download/openssl-$($(package)_version)
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=c53a47e5e441c930c3928cf7bf6fb00e5d129b630e0aa873b08258656e7345ec
# OpenSSL uses ./Configure, not CMake - no native dependencies needed
$(package)_native_dependencies=

define $(package)_set_vars
$(package)_config_env=AR="$($(package)_ar)" RANLIB="$($(package)_ranlib)" CC="$($(package)_cc)"
$(package)_config_env_mingw32=WINDRES="x86_64-w64-mingw32-windres"
$(package)_config_opts=--prefix=$(host_prefix) --openssldir=$(host_prefix)/etc/openssl
$(package)_config_opts+=no-camellia
$(package)_config_opts+=no-capieng
$(package)_config_opts+=no-cast
$(package)_config_opts+=no-comp
$(package)_config_opts+=no-dso
$(package)_config_opts+=no-dtls1
$(package)_config_opts+=no-ec_nistp_64_gcc_128
$(package)_config_opts+=no-gost
$(package)_config_opts+=no-idea
$(package)_config_opts+=no-md2
$(package)_config_opts+=no-mdc2
$(package)_config_opts+=no-rc4
$(package)_config_opts+=no-rc5
$(package)_config_opts+=no-rfc3779
$(package)_config_opts+=no-sctp
$(package)_config_opts+=no-seed
$(package)_config_opts+=no-shared
$(package)_config_opts+=no-ssl-trace
$(package)_config_opts+=no-ssl3
$(package)_config_opts+=no-rdrand
$(package)_config_opts+=no-unit-test
$(package)_config_opts+=no-weak-ssl-ciphers
$(package)_config_opts+=no-whirlpool
$(package)_config_opts+=no-zlib
$(package)_config_opts+=no-zlib-dynamic
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
$(package)_config_opts_x86_64_mingw32=mingw64 --libdir=lib
$(package)_config_opts_i686_mingw32=mingw --libdir=lib
$(package)_config_opts_android=-fPIC
$(package)_config_opts_aarch64_android=linux-generic64
$(package)_config_opts_x86_64_android=linux-generic64
$(package)_config_opts_armv7a_android=linux-generic32
$(package)_config_opts_i686_android=linux-generic32
endef

define $(package)_preprocess_cmds
endef

define $(package)_config_cmds
  ./Configure $($(package)_config_opts)
endef

define $(package)_build_cmds
	$(MAKE) -j1 build_libs libcrypto.pc libssl.pc openssl.pc
endef

define $(package)_stage_cmds
	$(MAKE) DESTDIR=$($(package)_staging_dir) -j1 install_sw install_dev
endef

define $(package)_postprocess_cmds
	echo "=== OPENSSL DEBUG: Configuration used ==="
	echo "Config opts: $($(package)_config_opts)"
	echo "Platform opts: $($(package)_config_opts_$(host_os))"
	echo "Host OS: $(host_os)"
	echo "staging_dir=$($(package)_staging_dir)"
	echo "host_prefix=$(host_prefix)"
	if [ "$(host_os)" = "mingw32" ]; then \
		echo "=== OPENSSL DEBUG (mingw32) ==="; \
		echo "tree (top 30):"; \
		find "$($(package)_staging_dir)$(host_prefix)" -maxdepth 4 -type f | head -30; \
		echo "all .a files under prefix:"; \
		find "$($(package)_staging_dir)$(host_prefix)" -name "*.a" -type f 2>/dev/null || true; \
		if [ -d "$($(package)_staging_dir)$(host_prefix)/lib64" ]; then \
			echo "Normalizing lib64 -> lib"; \
			mkdir -p "$($(package)_staging_dir)$(host_prefix)/lib" \
			         "$($(package)_staging_dir)$(host_prefix)/lib/pkgconfig"; \
			cp -a "$($(package)_staging_dir)$(host_prefix)/lib64/"* \
			      "$($(package)_staging_dir)$(host_prefix)/lib/"; \
			if [ -d "$($(package)_staging_dir)$(host_prefix)/lib64/pkgconfig" ]; then \
				cp -a "$($(package)_staging_dir)$(host_prefix)/lib64/pkgconfig/"* \
				      "$($(package)_staging_dir)$(host_prefix)/lib/pkgconfig/" 2>/dev/null || true; \
			fi; \
		fi; \
		test -f "$($(package)_staging_dir)$(host_prefix)/lib/libcrypto.a"; \
		test -f "$($(package)_staging_dir)$(host_prefix)/lib/libssl.a"; \
		echo "FOUND:"; \
		ls -l "$($(package)_staging_dir)$(host_prefix)/lib/libcrypto.a"; \
		ls -l "$($(package)_staging_dir)$(host_prefix)/lib/libssl.a"; \
		rm -rf "$($(package)_staging_dir)$(host_prefix)/share" \
		       "$($(package)_staging_dir)$(host_prefix)/bin" \
		       "$($(package)_staging_dir)$(host_prefix)/etc"; \
	else \
		echo "=== OPENSSL DEBUG: Testing expected paths (Linux) ==="; \
		test -f "$($(package)_staging_dir)$($(package)_prefix)/lib/libcrypto.a"; \
		test -f "$($(package)_staging_dir)$($(package)_prefix)/lib/libssl.a"; \
		rm -rf share bin etc; \
	fi
endef

