package=bdb
$(package)_version=18.1.40
$(package)_download_path=http://download.oracle.com/berkeley-db
$(package)_file_name=db-$($(package)_version).tar.gz
$(package)_sha256_hash=0cecb2ef0c67b166de93732769abdeba0555086d51de1090df325e18ee8da9c8
$(package)_build_subdir=build_unix
$(package)_patches=bdb-18.1.40-win32-mutex-complete.patch

define $(package)_set_vars
$(package)_config_opts=--disable-shared --enable-cxx --disable-replication
$(package)_config_opts_mingw32=--enable-mingw
$(package)_config_opts_linux=--with-pic
$(package)_config_opts_darwin=--with-mutex=POSIX
$(package)_config_env_darwin=ac_cv_mutex=POSIX/pthreads/library
$(package)_config_env_mingw32=CC=x86_64-w64-mingw32-gcc-posix CXX=x86_64-w64-mingw32-g++-posix
$(package)_cxxflags=-std=c++20
$(package)_cppflags_mingw32=-D_WIN32_WINNT=0x0601
$(package)_ldflags_mingw32=-lpthread
endef

define $(package)_preprocess_cmds
  sed -i 's/WinIoCtl.h/winioctl.h/g' src/dbinc/win_db.h && \
  sed -i 's/WINCE_ATOMIC_MAGIC(&mutexp->sharecount);/\/* WINCE_ATOMIC_MAGIC(&mutexp->sharecount); *\//g' src/mutex/mut_win32.c && \
  patch -p1 < $($(package)_patch_dir)/bdb-18.1.40-win32-mutex-complete.patch && \
  cp -f $(BASEDIR)/config.guess $(BASEDIR)/config.sub dist
endef

define $(package)_config_cmds
  ../dist/$($(package)_autoconf)
endef

define $(package)_build_cmds
  $(MAKE) libdb_cxx-18.1.a libdb-18.1.a
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install_lib install_include
endef
