packages:=openssl libevent zeromq
native_packages := native_ccache

qt_native_packages = native_protobuf native_cmake qt6-tools
qt_packages = qrencode protobuf zlib

# For Qt6 builds, replace 'qt' with 'qt6' in the lines below
qt_x86_64_linux_packages:=qt6 expat dbus libxcb xcb_proto libXau xproto freetype fontconfig libX11 xextproto libXext xtrans
qt_i686_linux_packages:=$(qt_x86_64_linux_packages)

qt_darwin_packages=qt6
qt_mingw32_packages=qt6 zlib

wallet_packages=bdb bdb48-utils

upnp_packages=miniupnpc

darwin_native_packages = native_biplist native_ds_store native_mac_alias

ifneq ($(build_os),darwin)
darwin_native_packages += native_cctools native_cdrkit native_libdmg-hfsplus
endif
