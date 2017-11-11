/*
* TLS Cipher Suites
* (C) 2004-2011,2012,2017 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_TLS_CIPHER_SUITES_H_
#define BOTAN_TLS_CIPHER_SUITES_H_

#include <botan/types.h>
#include <botan/tls_algos.h>
#include <string>
#include <vector>

namespace Botan {

namespace TLS {

/**
* Ciphersuite Information
*/
class BOTAN_PUBLIC_API(2,0) Ciphersuite final
   {
   public:
      /**
      * Convert an SSL/TLS ciphersuite to algorithm fields
      * @param suite the ciphersuite code number
      * @return ciphersuite object
      */
      static Ciphersuite by_id(uint16_t suite);

      /**
      * Returns true iff this suite is a known SCSV
      */
      static bool is_scsv(uint16_t suite);

      /**
      * Generate a static list of all known ciphersuites and return it.
      *
      * @return list of all known ciphersuites
      */
      static const std::vector<Ciphersuite>& all_known_ciphersuites();

      /**
      * Formats the ciphersuite back to an RFC-style ciphersuite string
      * @return RFC ciphersuite string identifier
      */
      std::string to_string() const { return m_iana_id; }

      /**
      * @return ciphersuite number
      */
      uint16_t ciphersuite_code() const { return m_ciphersuite_code; }

      /**
      * @return true if this is a PSK ciphersuite
      */
      bool psk_ciphersuite() const;

      /**
      * @return true if this is an ECC ciphersuite
      */
      bool ecc_ciphersuite() const;

      /**
       * @return true if this suite uses a CBC cipher
       */
      bool cbc_ciphersuite() const;

      bool signature_used() const;

      /**
      * @return key exchange algorithm used by this ciphersuite
      */
      std::string kex_algo() const { return kex_method_to_string(kex_method()); }

      Kex_Algo kex_method() const { return m_kex_algo; }

      /**
      * @return signature algorithm used by this ciphersuite
      */
      std::string sig_algo() const { return auth_method_to_string(auth_method()); }

      Auth_Method auth_method() const { return m_auth_method; }

      /**
      * @return symmetric cipher algorithm used by this ciphersuite
      */
      //std::string cipher_algo() const { return m_cipher_algo; }

      /**
      * @return message authentication algorithm used by this ciphersuite
      */
      //std::string mac_algo() const { return m_mac_algo; }

      std::string prf_algo() const
         {
         return kdf_algo_to_string(m_prf_algo);
         }

      /**
      * @return cipher key length used by this ciphersuite
      */
      //size_t cipher_keylen() const { return m_cipher_keylen; }
      //size_t mac_keylen() const { return m_mac_keylen; }

      size_t nonce_bytes_from_handshake() const;

      Nonce_Format nonce_format() const { return m_nonce_format; }

      /**
      * @return true if this is a valid/known ciphersuite
      */
      bool valid() const { return m_usable; }

      bool operator<(const Ciphersuite& o) const { return ciphersuite_code() < o.ciphersuite_code(); }
      bool operator<(const uint16_t c) const { return ciphersuite_code() < c; }

      Ciphersuite() = default;

   private:

      bool is_usable() const;

      Ciphersuite(uint16_t ciphersuite_code,
                  const char* iana_id,
                  Auth_Method auth_method,
                  Kex_Algo kex_algo,
                  Cipher_Algo cipher_algo,
                  KDF_Algo prf_algo,
                  Nonce_Format nonce_format) :
         m_ciphersuite_code(ciphersuite_code),
         m_iana_id(iana_id),
         m_auth_method(auth_method),
         m_kex_algo(kex_algo),
         m_prf_algo(prf_algo),
         m_nonce_format(nonce_format),
         m_cipher_algo(cipher_algo)
         {
         m_usable = is_usable();
         }

      uint16_t m_ciphersuite_code = 0;

      /*
      All of these const char* strings are references to compile time
      constants in tls_suite_info.cpp
      */
      const char* m_iana_id = nullptr;

      Auth_Method m_auth_method = Auth_Method::RSA;
      Kex_Algo m_kex_algo = Kex_Algo::STATIC_RSA;
      KDF_Algo m_prf_algo = KDF_Algo::SHA1;
      Nonce_Format m_nonce_format = Nonce_Format::CBC_MODE;
      Cipher_Algo m_cipher_algo = Cipher_Algo::CHACHA20_POLY1305;

      bool m_usable = false;
   };

}

}

#endif
