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
    std::vector<unsigned char> aad = SECURITY_HEADER_DATA + additional_auth_data; // TODO fix error

    // Prepare buffers for ciphertext and tag
    std::vector<unsigned char> cipheretext(plaintext_bytes.size());
    unsigned char tag[16];

    
}