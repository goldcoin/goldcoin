// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_COMPAT_OPENSSL_STUBS_H
#define BITCOIN_COMPAT_OPENSSL_STUBS_H

#ifdef WIN32

// OpenSSL 3.3.1 has a circular dependency issue on Windows where
// libcrypto uses WPACKET functions that are defined in libssl.
// These declarations ensure our stubs are visible to all targets.

#ifdef __cplusplus
extern "C" {
#endif

struct wpacket_st;
typedef struct wpacket_st WPACKET;

// WPACKET function declarations
int WPACKET_init_static_len(WPACKET *pkt, unsigned char *buf, size_t len, size_t lenbytes);
int WPACKET_start_sub_packet(WPACKET *pkt);
int WPACKET_close(WPACKET *pkt);
int WPACKET_finish(WPACKET *pkt);
size_t WPACKET_get_length(const WPACKET *pkt);
size_t WPACKET_get_total_written(const WPACKET *pkt);
int WPACKET_put_bytes__(WPACKET *pkt, unsigned int val, size_t bytes);
int WPACKET_allocate_bytes(WPACKET *pkt, size_t len, unsigned char **allocbytes);
int WPACKET_memcpy(WPACKET *pkt, const void *src, size_t len);
int WPACKET_set_flags(WPACKET *pkt, unsigned int flags);
void WPACKET_cleanup(WPACKET *pkt);
int WPACKET_init_null(WPACKET *pkt, size_t lenbytes);
int WPACKET_init_null_der(WPACKET *pkt);
int WPACKET_init_der(WPACKET *pkt, unsigned char *buf, size_t len);
unsigned char *WPACKET_get_curr(WPACKET *pkt);
int WPACKET_init_len(WPACKET *pkt, unsigned char *buf, size_t len, size_t lenbytes);
int WPACKET_is_null_buf(const WPACKET *pkt);

#ifdef __cplusplus
}
#endif

#endif // WIN32

#endif // BITCOIN_COMPAT_OPENSSL_STUBS_H