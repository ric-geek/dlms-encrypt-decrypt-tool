#include <iostream>
#include "dlms_crypto_tool.h"

int main(int argc, char* argv[]) 
{

    if (argc < 2) 
    {

        std::cout << "Usage:\n";
        std::cout << "  dlmscli key\n";
        std::cout << "  dlmscli encrypt <system_title> <frame_counter> <encryption_key> <aad> <plaintext>\n";
        std::cout << "  dlmscli decrypt <system_title> <frame_counter> <encryption_key> <aad> <ciphertext_with_tag>\n";
        std::cout << "  dlmscli auth <system_title> <frame_counter> <encryption_key> <authentication_key> <stoc>\n";

        return 1;

    }

    std::string command = argv[1];

    try
    {
    
        if (command == "key")
        {
        
            std::string key = generate_key();
            std::cout << "Generated Key: " << key << std::endl;
        
        }
        else if (command == "encrypt")
        {
         
            if (argc < 7)
            {
            
                std::cerr << "Usage: dlmscli encrypt <system_title> <frame_counter> <encryption_key> <aad> <plaintext>\n";
            
                return 1;
            }
            
            std::string system_title = argv[2];
            std::string frame_counter = argv[3];
            std::string encryption_key = argv[4];
            std::string aad = argv[5];
            std::string plaintext = argv[6];
            std::string encrypted = encrypt_apdu(system_title, frame_counter, encryption_key, aad, plaintext);
            std::cout << "Encrypted Output (ciphertext+tag): " << encrypted << std::endl;
        
        } 
        else if (command == "decrypt")
        {
        
            if (argc < 7)
            {

                std::cerr << "Usage: dlmscli decrypt <system_title> <frame_counter> <encryption_key> <aad> <ciphertext_with_tag>\n";
                
                return 1;
            
            }
        
            std::string system_title = argv[2];
            std::string frame_counter = argv[3];
            std::string encryption_key = argv[4];
            std::string aad = argv[5];
            std::string ciphertext_with_tag = argv[6];
            std::string decrypted = decrypt_apdu(system_title, frame_counter, encryption_key, aad, ciphertext_with_tag);
            std::cout << "Decrypted Output (plaintext in hex): " << decrypted << std::endl;
        
        } 
        else if (command == "auth") 
        {
            
            if (argc < 7) 
            {
            
                std::cerr << "Usage: dlmscli auth <system_title> <frame_counter> <encryption_key> <authentication_key> <stoc>\n";
            
                return 1;
            
            }

            std::string system_title = argv[2];
            std::string frame_counter = argv[3];
            std::string encryption_key = argv[4];
            std::string authentication_key = argv[5];
            std::string stoc = argv[6];
            std::string tag = auth_apdu(system_title, frame_counter, encryption_key, authentication_key, stoc);
            std::cout << "Authentication Tag: " << tag << std::endl;
        
        } 
        else
        {
        
            std::cerr << "Unknown command: " << command << std::endl;
            return 1;
    
        }
    } catch (const std::exception &ex)
    {

        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;

    }

    return 0;

}