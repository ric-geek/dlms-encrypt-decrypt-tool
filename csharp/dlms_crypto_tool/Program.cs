using System;

class Program
{
    static void PrintUsage()
    {
        Console.WriteLine("Usage:");
        Console.WriteLine("  dlmscli key");
        Console.WriteLine("  dlmscli encrypt <system_title> <frame_counter> <encryption_key> <aad> <plaintext>");
        Console.WriteLine("  dlmscli decrypt <system_title> <frame_counter> <encryption_key> <aad> <ciphertext_with_tag>");
        Console.WriteLine("  dlmscli auth <system_title> <frame_counter> <encryption_key> <authentication_key> <stoc>");
    }

    static void Main(string[] args)
    {
        if (args.Length < 1)
        {
            PrintUsage();
            return;
        }

        string command = args[0].ToLowerInvariant();

        try
        {
            switch (command)
            {
                case "key":
                    string key = DLMSCryptoTool.GenerateKey();
                    Console.WriteLine("Generated key: " + key);
                    break;

                case "encrypt":
                    if (args.Length != 6)
                    {
                        Console.Error.WriteLine("Invalid usage for encrypt.");
                        PrintUsage();
                        return;
                    }
                    {
                        string systemTitle = args[1];
                        string frameCounter = args[2];
                        string encryptionKey = args[3];
                        string aad = args[4];
                        string plaintext = args[5];

                        string encrypted = DLMSCryptoTool.EncryptAPDU(systemTitle, frameCounter, encryptionKey, aad, plaintext);
                        Console.WriteLine("Encrypted output (ciphertext+tag): " + encrypted);
                    }
                    break;

                case "decrypt":
                    if (args.Length != 6)
                    {
                        Console.Error.WriteLine("Invalid usage for decrypt.");
                        PrintUsage();
                        return;
                    }
                    {
                        string systemTitle = args[1];
                        string frameCounter = args[2];
                        string encryptionKey = args[3];
                        string aad = args[4];
                        string ciphertextWithTag = args[5];

                        string decrypted = DLMSCryptoTool.DecryptAPDU(systemTitle, frameCounter, encryptionKey, aad, ciphertextWithTag);
                        Console.WriteLine("Decrypted plaintext (hex): " + decrypted);
                    }
                    break;

                case "auth":
                    if (args.Length != 6)
                    {
                        Console.Error.WriteLine("Invalid usage for auth.");
                        PrintUsage();
                        return;
                    }
                    {
                        string systemTitle = args[1];
                        string frameCounter = args[2];
                        string encryptionKey = args[3];
                        string authenticationKey = args[4];
                        string stoc = args[5];

                        string authTag = DLMSCryptoTool.AuthAPDU(systemTitle, frameCounter, encryptionKey, authenticationKey, stoc);
                        Console.WriteLine("Authentication tag: " + authTag);
                    }
                    break;

                default:
                    Console.Error.WriteLine("Unknown command: " + command);
                    PrintUsage();
                    break;
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine("Error: " + ex.Message);
        }
    }
}