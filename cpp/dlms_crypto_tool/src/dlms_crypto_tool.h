#ifndef DLMS_CRYPTO_TOOL_H
#define DLMS_CRYPTO_TOOL_H

#include <string>
#include <vector>

// Security header constants
extern const std::string SECURITY_HEADER_AUTH;
extern const std::string SECURITY_HEADER_DATA;

// Function to generate a random 16 byte encryption key
std::string generate_key();

// Function to create an IV by concatenating system_title and frame_counter
std::vector<unsigned char> create_iv(const std::string& system_title, const std::string& frame_counter);

// Function to encrypt an APDU message
std::string encrypt_apdu(const std::string& system_title, const std::string& frame_counter,
                         const std::string& encryption_key, const std::string& additional_auth_data,
                         const std::string& plaintext);

// Function to decrypt an APDU message
std::string decrypt_apdu(const std::string& system_title, const std::string& frame_counter,
                         const std::string& encryption_key, const std::string& additional_auth_data,
                         const std::string& ciphertext_with_tag);

// Function to authenticate an APDU
std::string decrypt_apdu(const std::string& system_title, const std::string& frame_counter,
    const std::string& encryption_key, const std::string& authentication_key,
    const std::string& stoc);

// Generate an authentication tag by encrypting an empty plaintext with a custom AAD
std::string auth_apdu(const std::string& system_title,
    const std::string& frame_counter,
    const std::string& encryption_key,
    const std::string& authentication_key,
    const std::string& stoc);

#endif // DLMS_CRYPTO_TOOL_H