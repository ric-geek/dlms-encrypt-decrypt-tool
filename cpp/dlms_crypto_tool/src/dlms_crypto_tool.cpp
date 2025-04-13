#include "dlms_crypto_tool.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Define the header constants
const std::string SECURITY_HEADER_AUTH = "10";
const std::string SECURITY_HEADER_DATA = "30";

// Helper: Convert hex string to bytes.
std::vector<unsigned char> hex_to_bytes(const std::string &hex)
{

    std::vector<unsigned char> bytes;
    
    if (hex.length() % 2 != 0)
    {

        throw std::runtime_error("Invalid hex string length.");

    }

    for (size_t i = 0; i < hex.length(); i += 2)
    {

        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);

    }

    return bytes;
    
}

// Helper: Convert bytes to hex string.
std::string bytes_to_hex(const std::vector<unsigned char> &bytes) {
    std::ostringstream oss;
    for (unsigned char byte : bytes)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    return oss.str();
}

std::string generate_key() 
{

    unsigned char key[16];
    
    if (!RAND_bytes(key, sizeof(key)))
    {

        throw std::runtime_error("Failed to generate random key");
    
    }

    std::ostringstream oss;

    for (int i = 0; i < 16; ++i)
    {
        
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)key[i];

    }

    return oss.str();

}

std::vector<unsigned char> create_iv(const std::string& system_title, const std::string& frame_counter) 
{

    // Concatenate the hex strings and convert to bytes.
    std::string iv_hex = system_title + frame_counter;

    return hex_to_bytes(iv_hex);

}

std::string encrypt_apdu(const std::string& system_title,
                         const std::string& frame_counter,
                         const std::string& encryption_key,
                         const std::string& additional_auth_data,
                         const std::string& plaintext) 
{

    // Convert encryption key (hex) to binary.
    std::vector<unsigned char> key_bytes = hex_to_bytes(encryption_key);

    // Create IV (expected to be 12 bytes)
    std::vector<unsigned char> iv = create_iv(system_title, frame_counter);
    
    if (iv.size() != 12)
    {

        throw std::runtime_error("IV length must be 12 bytes");

    }

    // Create AAD.
    std::string aad_str = SECURITY_HEADER_DATA + additional_auth_data;
    std::vector<unsigned char> aad(aad_str.begin(), aad_str.end());

    // Convert the plaintext (hex) to bytes.
    std::vector<unsigned char> plaintext_bytes = hex_to_bytes(plaintext);

    // Prepare buffers and context for encryption.
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if (!ctx)
    {

        throw std::runtime_error("Failed to create encryption context");

    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr)) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Encryption initialization failed");

    }

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr))
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Setting IV length failed");

    }

    if (1 != EVP_EncryptInit_ex(ctx, nullptr, nullptr, key_bytes.data(), iv.data())) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Setting key/IV failed");

    }

    int len;
    
    if (1 != EVP_EncryptUpdate(ctx, nullptr, &len, aad.data(), aad.size())) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("AAD encryption failed");

    }

    std::vector<unsigned char> ciphertext(plaintext_bytes.size());

    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext_bytes.data(), plaintext_bytes.size())) 
    {
        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Plaintext encryption failed");

    }

    int ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Finalize encryption failed");

    }

    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);

    // Retrieve the authentication tag (16 bytes).
    unsigned char tag[16];

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, sizeof(tag), tag)) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Retrieving tag failed");

    }

    EVP_CIPHER_CTX_free(ctx);

    std::string ciphertext_hex = bytes_to_hex(ciphertext);

    // Convert tag to hex.
    std::ostringstream tag_oss;
    
    for (int i = 0; i < 16; ++i)
    {

        tag_oss << std::hex << std::setw(2) << std::setfill('0') << (int)tag[i];

    }

    std::string tag_hex = tag_oss.str();

    // Return concatenation of ciphertext and tag.
    return ciphertext_hex + tag_hex;

}

std::string decrypt_apdu(const std::string& system_title,
                         const std::string& frame_counter,
                         const std::string& encryption_key,
                         const std::string& additional_auth_data,
                         const std::string& ciphertext_with_tag)
{

    // The last 32 hex characters represent the 16-byte tag.
    if (ciphertext_with_tag.size() < 32)
    {

        throw std::runtime_error("Input too short; missing authentication tag");

    }

    size_t tag_hex_len = 32;
    std::string ciphertext_hex = ciphertext_with_tag.substr(0, ciphertext_with_tag.size() - tag_hex_len);
    std::string tag_hex = ciphertext_with_tag.substr(ciphertext_with_tag.size() - tag_hex_len);

    std::vector<unsigned char> ciphertext = hex_to_bytes(ciphertext_hex);
    std::vector<unsigned char> tag = hex_to_bytes(tag_hex);
    std::vector<unsigned char> key_bytes = hex_to_bytes(encryption_key);
    std::vector<unsigned char> iv = create_iv(system_title, frame_counter);

    std::string aad_str = SECURITY_HEADER_DATA + additional_auth_data;
    std::vector<unsigned char> aad(aad_str.begin(), aad_str.end());

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if (!ctx)
    {

        throw std::runtime_error("Failed to create decryption context");

    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr)) 
    {

        EVP_CIPHER_CTX_free(ctx);

        throw std::runtime_error("Decryption initialization failed");

    }

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr)) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Setting IV length failed");

    }

    if (1 != EVP_DecryptInit_ex(ctx, nullptr, nullptr, key_bytes.data(), iv.data())) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Setting key/IV failed");

    }

    int len;
    
    if (1 != EVP_DecryptUpdate(ctx, nullptr, &len, aad.data(), aad.size())) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("AAD decryption failed");

    }

    std::vector<unsigned char> plaintext(ciphertext.size());
    
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) 
    {

        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Ciphertext decryption failed");

    }

    int plaintext_len = len;

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tag.size(), tag.data())) 
    {
    
        EVP_CIPHER_CTX_free(ctx);
    
        throw std::runtime_error("Setting authentication tag failed");
    
    }

    int ret = EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    
    EVP_CIPHER_CTX_free(ctx);
    
    if (ret <= 0)
    {
        
        throw std::runtime_error("Decryption failed: Invalid authentication tag");

    }

    plaintext_len += len;
    plaintext.resize(plaintext_len);

    // Return the plaintext as a hex string.
    return bytes_to_hex(plaintext);

}

std::string auth_apdu(const std::string& system_title,
                      const std::string& frame_counter,
                      const std::string& encryption_key,
                      const std::string& authentication_key,
                      const std::string& stoc) 
{
    
    // The special AAD for auth generation is constructed from:
    // SECURITY_HEADER_AUTH + authentication_key + stoc.
    std::string aad_str = SECURITY_HEADER_AUTH + authentication_key + stoc;
    std::vector<unsigned char> aad(aad_str.begin(), aad_str.end());

    std::vector<unsigned char> key_bytes = hex_to_bytes(encryption_key);
    std::vector<unsigned char> iv = create_iv(system_title, frame_counter);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
   
    if (!ctx)
   {
        
        throw std::runtime_error("Failed to create auth context");

   }
    
   if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr)) 
   {
        EVP_CIPHER_CTX_free(ctx);
   
        throw std::runtime_error("Auth encryption initialization failed");
   
    }
   
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr)) {
   
        EVP_CIPHER_CTX_free(ctx);
   
        throw std::runtime_error("Auth: setting IV length failed");
   
    }
    
    if (1 != EVP_EncryptInit_ex(ctx, nullptr, nullptr, key_bytes.data(), iv.data())) 
    {
    
        EVP_CIPHER_CTX_free(ctx);
    
        throw std::runtime_error("Auth: setting key/IV failed");
    
    }
    
    int len;
    
    if (1 != EVP_EncryptUpdate(ctx, nullptr, &len, aad.data(), aad.size())) 
    {
    
        EVP_CIPHER_CTX_free(ctx);
    
        throw std::runtime_error("Auth: AAD update failed");
    
    }
    
    // Encrypt an empty plaintext.
    unsigned char outbuf[1];
    
    if (1 != EVP_EncryptUpdate(ctx, outbuf, &len, nullptr, 0)) 
    {
    
        EVP_CIPHER_CTX_free(ctx);
    
        throw std::runtime_error("Auth: encryption update failed");
    
    }
    
    if (1 != EVP_EncryptFinal_ex(ctx, outbuf, &len)) 
    {
    
        EVP_CIPHER_CTX_free(ctx);
    
        throw std::runtime_error("Auth: encryption finalization failed");

    }
    
    unsigned char tag[16];
    
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag)) 
    {
        
        EVP_CIPHER_CTX_free(ctx);
        
        throw std::runtime_error("Auth: retrieving tag failed");

    }

    EVP_CIPHER_CTX_free(ctx);

    // Convert tag to hex and then truncate to 24 hex characters.
    std::ostringstream tag_oss;

    for (int i = 0; i < 16; ++i)
    {

        tag_oss << std::hex << std::setw(2) << std::setfill('0') << (int)tag[i];

    }

    std::string full_tag = tag_oss.str();
    
    if (full_tag.size() < 24)
    {

        throw std::runtime_error("Auth tag is too short");

    }

    return full_tag.substr(0, 24);

}