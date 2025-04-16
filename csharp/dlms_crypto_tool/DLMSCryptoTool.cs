using System;
using System.Text;
using Org.BouncyCastle.Crypto;
using Org.BouncyCastle.Crypto.Modes;
using Org.BouncyCastle.Crypto.Engines;
using Org.BouncyCastle.Crypto.Parameters;
using Org.BouncyCastle.Security;

public static class DLMSCryptoTool
{

    // Security header constants
    private const string SECURITY_HEADER_AUTH = "10";
    private const string SECURITY_HEADER_DATA = "30";

    ///<summary>
    /// Converts a hex string to a byte array
    /// </summary>
    public static byte[] HexToBytes(string hex)
    {

        if (hex.Length % 2 != 0)
        {

            throw new ArgumentException("Invalid hex string!");
            
        }

        byte[] bytes = new byte[hex.Length / 2];

        for (int i = 0; i < bytes.Length; i++)
        {

            bytes[i] = Convert.ToByte(hex.Substring(i * 2, 2), 16);
            
        }

        return bytes;
    }

    ///<summary>
    /// Converts a byte array to a hex string
    /// </summary>
    public static string BytesToHex(byte[] bytes)
    {

        return BitConverter.ToString(bytes).Replace("-","").ToLowerInvariant();

    }

    ///<summary>
    /// generates a random encryption key and return it as a hex string
    /// </summary>
    public static string GenerateKey()
    {

        byte[] key = new byte[16];
        new SecureRandom().NextBytes(key);

        return BytesToHex(key);

    }
    
    ///<summary>
    /// Creates an IV by concatenating a system title and frame counter (both given as hex strings)
    /// and converting the result into bytes
    /// </summary>
    public static byte[] CreateIV(string system_title, string frame_counter)
    {

        string ivHex = system_title + frame_counter;

        return HexToBytes(ivHex);

    }

    /// <summary>
    /// Encrypts a DLMS APDU
    /// </summary>
    /// <param name="system_title">Hex string representing the system title (8 bytes expected).</param>
    /// <param name="frame_counter">Hex string representing the frame counter (4 bytes expected).</param>
    /// <param name="encryption_key">A 16-byte key as a hex string.</param>
    /// <param name="additional_aad">Additional authentication data.</param>
    /// <param name="plaintext">Plaintext to encrypt (as a hex string).</param>
    /// <returns>The ciphertext with the authentication tag appended as a hex string.</returns>
    public static string EncryptAPDU(string system_title, string frame_counter, string encryption_key, string additional_aad, string plaintext)
    {

        byte[] keyBytes = HexToBytes(encryption_key);
        byte[] iv = CreateIV(system_title, frame_counter);

        if (iv.Length != 12)
        {
            
            throw new Exception("IV must be 12 bytes!");

        }

        // Build AAD
        string aad_str = SECURITY_HEADER_DATA + additional_aad;
        byte[] aad = Encoding.ASCII.GetBytes(aad_str);

        byte[] plaintext_bytes = HexToBytes(plaintext);

        // Perform encryption
        GcmBlockCipher cipher = new GcmBlockCipher(new AesEngine());
        AeadParameters parameters = new AeadParameters(new KeyParameter(keyBytes), 128, iv, aad);
        cipher.Init(true, parameters);

        byte[] output = new byte[cipher.GetOutputSize(plaintext_bytes.Length)];
        int outOff = cipher.ProcessBytes(plaintext_bytes, 0, plaintext_bytes.Length, output, 0);

        cipher.DoFinal(output, outOff);

        return BytesToHex(output);

    }

    ///<summary>
    /// Decrypt a DLMS APDU
    /// </summary>
    /// <param name="system_title">Hex string representing the system title (8 bytes expected).</param>
    /// <param name="frame_counter">Hex string representing the frame counter (4 bytes expected).</param>
    /// <param name="encryption_key">A 16-byte key as a hex string.</param>
    /// <param name="additional_aad">Additional authentication data.</param>
    /// <param name="ciphertext_with_tag">Plaintext to encrypt (as a hex string).</param>
    /// <returns>The ciphertext with the authentication tag appended as a hex string.</returns>
    public static string DecryptAPDU(string system_title, string frame_counter, string encryption_key, string additional_aad, string ciphertext_with_tag)
    {

        byte[] keyBytes = HexToBytes(encryption_key);
        byte[] iv = CreateIV(system_title, frame_counter);

        if (iv.Length != 12)
        {
            
            throw new Exception("IV must be 12 bytes!");

        }

        // Build AAD
        string aad_str = SECURITY_HEADER_DATA + additional_aad;
        byte[] aad = Encoding.ASCII.GetBytes(aad_str);
        byte[] cipher_bytes = HexToBytes(ciphertext_with_tag);

        // Perform decryption
        GcmBlockCipher cipher = new GcmBlockCipher(new AesEngine());
        AeadParameters parameters = new AeadParameters(new KeyParameter(keyBytes), 128, iv, aad);
        cipher.Init(false, parameters);

        byte[] output = new byte[cipher.GetOutputSize(cipher_bytes.Length)];
        int outOff = cipher.ProcessBytes(cipher_bytes, 0, cipher_bytes.Length, output, 0);

        try
        {
            
            cipher.DoFinal(output, outOff);

        }
        catch (CryptoException ex)
        {
            
            throw new Exception("Decryption failed: " + ex.Message);

        }

        return BytesToHex(output);

    }

    /// <summary>
    /// Generates an authentication tag by encrypting an empty plaintext using custom AAD.
    /// The AAD is constructed as: SECURITY_HEADER_AUTH + authenticationKey + stoc.
    /// The tag is then truncated to the first 12 bytes (24 hex characters).
    /// </summary>
    public static string AuthAPDU(string system_title, string frame_counter, string encryption_key, string authentication_key, string stoc)
    {

        byte[] keyBytes = HexToBytes(encryption_key);
        byte[] iv = CreateIV(system_title, frame_counter);

        if (iv.Length != 12)
        {
            
            throw new Exception("IV must be 12 bytes!");

        }

        string aad_str = SECURITY_HEADER_AUTH + authentication_key + stoc;
        byte[] aad = Encoding.ASCII.GetBytes(aad_str);

        // Encrypt an empty plaintext to get the tag
        byte[] empty_plaintext = Array.Empty<byte>();

        GcmBlockCipher cipher = new GcmBlockCipher(new AesEngine());
        AeadParameters parameters = new AeadParameters(new KeyParameter(keyBytes), 128, iv, aad);
        cipher.Init(true, parameters);

        byte[] output = new byte[cipher.GetOutputSize(empty_plaintext.Length)];
        int outOff = cipher.ProcessBytes(empty_plaintext, 0, empty_plaintext.Length, output, 0);
        cipher.DoFinal(output, outOff);

        // Truncate the TAG to the first 12 bytes
        byte[] truncated_tag = new byte[12];
        Array.Copy(output, 0, truncated_tag, 0, 12);

        return BytesToHex(truncated_tag);

    }
}