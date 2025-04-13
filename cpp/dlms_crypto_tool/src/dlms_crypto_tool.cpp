#include "dlms_crypto_tool.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string generate_key()
{

    unsigned char key[16];

    if(!RAND_bytes(key, sizeof(key)))
    {

        throw std::runtime_error("Failed to generate encryption key");

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

    std::vector<unsigned char> iv;

    iv.insert(iv.end(), system_title.begin(), system_title.end());
    iv.insert(iv.end(), frame_counter.begin(), frame_counter.end());

    return iv;

}

std::string encrypt_apdu(const std::string& system_title, const std::string& frame_counter,
                         const std::string& encryption_key, const std::string& additional_auth_data,
                         const std::string& plaintext)
{

    // Prepare key and iv
    unsigned char key[16];

    std::vector<unsigned char> iv = create_iv(system_title, frame_counter);

    if(iv.size() != 12)
    {

        throw std::invalid_argument("IV must be 12 bytes");

    }

    for (size_t i = 0; i < encryption_key.length() / 2; ++i)
    {
        
        key[i] = std::stoi(encryption_key.substr(2 * i, 2), nullptr, 16);

    }
    
    // Prepare plaitext and AAD
    std::vector<unsigned char> plaintext_bytes(plaintext.begin(), plaintext.end());
    std::string aad_str = SECURITY_HEADER_DATA + additional_auth_data;
    std::vector<unsigned char> aad(aad_str.begin(), aad_str.end());

    // Prepare buffers for ciphertext and tag
    std::vector<unsigned char> ciphertext(plaintext_bytes.size());
    unsigned char tag[16];

    // Perform AES-GCM encryption
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx)
    {

        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

    }

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr) ||
    !EVP_EncryptInit_ex(ctx, nullptr, nullptr, key, iv.data()) ||
    !EVP_EncryptUpdate(ctx, nullptr, nullptr, aad.data(), aad.size()) ||
    !EVP_EncryptUpdate(ctx, ciphertext.data(), nullptr, plaintext_bytes.data(), plaintext_bytes.size()) ||
    !EVP_EncryptFinal_ex(ctx, nullptr, nullptr) ||
    !EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, sizeof(tag), tag))
    {
        
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("AES-GCM encryption failed");

    }

    EVP_CIPHER_CTX_free(ctx);

    // Convert ciphertext and tag to hex
    std::ostringstream oss;

    for (unsigned char byte : ciphertext)
    {
        
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;

    } 
    
    for (int i = 0; i < 16; ++i)
    {
        
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)tag[i];
        
    }
    
    return oss.str();

}