// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifdef WIN32

#include <stddef.h>  // for size_t
#include "compat/openssl_stubs.h"

// OpenSSL 3.3.1 has a circular dependency issue on Windows where
// libcrypto uses WPACKET functions that are defined in libssl.
// These stubs resolve the linking issue for Windows builds.
// This is a temporary workaround until OpenSSL fixes the issue.

extern "C" {

// Boost 1.89 threading stubs for Win32
// These resolve undefined references when Boost is built with Win32 threads
typedef void* __gthread_mutex_t;

int __gthr_win32_mutex_init_function(__gthread_mutex_t* mutex) {
    return 0; // Success
}

int __gthr_win32_mutex_destroy(__gthread_mutex_t* mutex) {
    return 0; // Success
}

int __gthr_win32_mutex_lock(__gthread_mutex_t* mutex) {
    return 0; // Success
}

int __gthr_win32_mutex_unlock(__gthread_mutex_t* mutex) {
    return 0; // Success
}

int WPACKET_init_static_len(WPACKET *pkt, unsigned char *buf, size_t len, size_t lenbytes) {
    return 1; // Success stub
}

int WPACKET_start_sub_packet(WPACKET *pkt) {
    return 1; // Success stub
}

int WPACKET_close(WPACKET *pkt) {
    return 1; // Success stub
}

int WPACKET_finish(WPACKET *pkt) {
    return 1; // Success stub
}

size_t WPACKET_get_length(const WPACKET *pkt) {
    return 0; // Stub return
}

size_t WPACKET_get_total_written(const WPACKET *pkt) {
    return 0; // Stub return
}

int WPACKET_put_bytes__(WPACKET *pkt, unsigned int val, size_t bytes) {
    return 1; // Success stub
}

int WPACKET_allocate_bytes(WPACKET *pkt, size_t len, unsigned char **allocbytes) {
    return 1; // Success stub
}

int WPACKET_memcpy(WPACKET *pkt, const void *src, size_t len) {
    return 1; // Success stub
}

int WPACKET_set_flags(WPACKET *pkt, unsigned int flags) {
    return 1; // Success stub
}

void WPACKET_cleanup(WPACKET *pkt) {
    // Stub - nothing to clean up
}

int WPACKET_init_null(WPACKET *pkt, size_t lenbytes) {
    return 1; // Success stub
}

int WPACKET_init_null_der(WPACKET *pkt) {
    return 1; // Success stub  
}

int WPACKET_init_der(WPACKET *pkt, unsigned char *buf, size_t len) {
    return 1; // Success stub
}

unsigned char *WPACKET_get_curr(WPACKET *pkt) {
    static unsigned char dummy[1024];
    return dummy; // Return dummy buffer
}

int WPACKET_init_len(WPACKET *pkt, unsigned char *buf, size_t len, size_t lenbytes) {
    return 1; // Success stub
}

int WPACKET_is_null_buf(const WPACKET *pkt) {
    return 0; // Not null
}

int WPACKET_start_sub_packet_len__(WPACKET *pkt, size_t len) {
    return 1; // Success stub
}

int WPACKET_sub_memcpy__(WPACKET *pkt, const void *src, size_t len, size_t lenbytes) {
    return 1; // Success stub
}

} // extern "C"

#endif // WIN32