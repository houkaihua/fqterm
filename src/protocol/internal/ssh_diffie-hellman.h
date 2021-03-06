#ifndef SSH_DH_H
#define SSH_DH_H

#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

#if OPENSSL_VERSION_NUMBER >= 0x10100000L && \
	!defined(LIBRESSL_VERSION_NUMBER)
#define ssh_md_ctx_new EVP_MD_CTX_new
#define ssh_md_ctx_free EVP_MD_CTX_free
#else
#define ssh_md_ctx_new EVP_MD_CTX_create
#define ssh_md_ctx_free EVP_MD_CTX_destroy
#endif

#ifdef __cplusplus
extern "C" {
#endif

	typedef unsigned char* (*hash_t)(const unsigned char *, size_t, unsigned char *);

	typedef struct
	{
		EVP_MD_CTX *mdctx;
		const EVP_MD *md;
		unsigned int hashlen;
	} evp_md_t;

	typedef struct ssh_diffie_hellman
	{
		const char *name;
		union {
			struct {
				BIGNUM *g; // generator
				BIGNUM *p; // prime
				BIGNUM *bn_x; /* random number */
				BN_CTX *ctx;
			} dh;
			unsigned char privkey[32];
		} priv;
		int e_len;
		unsigned char *mpint_e; /* g^x mod p */
		int secret_len;
		unsigned char *secret;
		evp_md_t digest;
	} SSH_DH;

	void ssh_dh_free(SSH_DH*);
	void ssh_dh_hash(SSH_DH*, const unsigned char* data, unsigned char*, size_t len);
	int ssh_dh_compute_secret(SSH_DH*, const unsigned char *, int);

	typedef SSH_DH*(*NEW_DH)(void);
	extern const char all_dh_list[];
	NEW_DH search_dh(const char *s);

#ifdef __cplusplus
}
#endif

#endif
