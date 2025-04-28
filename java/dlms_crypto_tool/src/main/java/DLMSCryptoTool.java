import javax.crypto.Cipher;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class DLMSCryptoTool {

    // Security headers
    private static final String AUTH_HEADER = "10";
    private static final String DATA_HEADER = "30";

    /**
     *  Converts a hex string to a byte array
     */
    public static byte[] hexStringToByteArray(String s){

        int len = s.length();

        if (len % 2 != 0){

            throw new IllegalArgumentException("Hex string must have even length");

        }

        byte[] data = new byte[len / 2];

        for (int i = 0; i < len; i++){

            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));

        }

        return data;

    }

    /**
     *  Converts a byte array to a hex string
     */
    public static String byteArrayToHexString(byte[] bytes){

        StringBuilder sb = new StringBuilder();

        for(byte b : bytes){

            sb.append(String.format("%02x", b));

        }

        return sb.toString();

    }

    /**
     *  Create an IV by concatenating systemTitle and frameCounter
     *  Both must be hex strings
     */
    public static byte[] createIV(String systemTitle, String frameCounter){

        return hexStringToByteArray(systemTitle + frameCounter);

    }

    /**
     * Encrypts a DLMS APDU
     *
     * @param systemTitle
     * @param frameCounter
     * @param encryptionKey
     * @param additionalAuthData
     * @param plaintext
     * @return A hex string containing the ciphertext with the authentication tag appended
     * @throes Exception
     */
    public static String encryptApdu(String systemTitle, String frameCounter, String encryptionKey,
                                     String additionalAuthData, String plaintext)throws Exception{

        byte[] iv = createIV(systemTitle, frameCounter);
        SecretKeySpec keySpec = new SecretKeySpec(hexStringToByteArray(encryptionKey), "AES");
        Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");

        GCMParameterSpec gcmSpec = new GCMParameterSpec(128, iv);
        cipher.init(Cipher.ENCRYPT_MODE, keySpec, gcmSpec);

        // Prepare AAD ("30")
        byte[] aad = hexStringToByteArray(DATA_HEADER + additionalAuthData);
        cipher.updateAAD(aad);

        // Convert plaintext hex string to bytes array and encrypt
        byte[] plaintextBytes = hexStringToByteArray(plaintext);
        byte[] cipherTextWithTag = cipher.doFinal(plaintextBytes);

        return byteArrayToHexString(cipherTextWithTag);

    }

    /**
     * Decrypts a DLMS APDU
     *
     * @param systemTitle
     * @param frameCounter
     * @param encryptionKey
     * @param additionalAuthData
     * @param ciphertextWithTagHex
     * @return A hex string containing the decrypted APDU
     * @throes Exception
     */
    public static String decryptApdu(String systemTitle, String frameCounter, String encryptionKey,
                                     String additionalAuthData, String ciphertextWithTagHex)throws Exception{

        byte[] iv = createIV(systemTitle, frameCounter);
        SecretKeySpec keySpec = new SecretKeySpec(hexStringToByteArray(encryptionKey), "AES");
        Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");

        GCMParameterSpec gcmSpec = new GCMParameterSpec(128, iv);
        cipher.init(Cipher.DECRYPT_MODE, keySpec, gcmSpec);

        // Prepare AAD ("30")
        byte[] aad = hexStringToByteArray(DATA_HEADER + additionalAuthData);
        cipher.updateAAD(aad);

        // Convert ciphertextWithTagHex hex string to bytes array and encrypt
        byte[] ciphertextWithTag = hexStringToByteArray(ciphertextWithTagHex);
        byte[] plaintextBytes = cipher.doFinal(ciphertextWithTag);

        return byteArrayToHexString(plaintextBytes);

    }

    /**
     * Generate an authentication tag for a DLMS APDU
     *
     * @param systemTitle
     * @param frameCounter
     * @param encryptionKey
     * @param authenticationKey
     * @param stoc
     * @return A truncated authentication tag as a hex string
     * @throes Exception
     */
    public static String authApdu(String systemTitle, String frameCounter, String encryptionKey,
                                     String authenticationKey, String stoc)throws Exception{

        byte[] iv = createIV(systemTitle, frameCounter);
        SecretKeySpec keySpec = new SecretKeySpec(hexStringToByteArray(encryptionKey), "AES");
        Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");

        GCMParameterSpec gcmSpec = new GCMParameterSpec(128, iv);
        cipher.init(Cipher.ENCRYPT_MODE, keySpec, gcmSpec);

        // Prepare AAD ("30")
        byte[] aad = hexStringToByteArray(DATA_HEADER + authenticationKey);
        cipher.updateAAD(aad);

        // Encrypt an empty plaintext and retrieve the authentication TAG
        byte[] tagWithEmptyPlaintext = cipher.doFinal(new byte[0]);
        String TagHex = byteArrayToHexString(tagWithEmptyPlaintext);

        // Truncate the TAG
        if(TagHex.length() <= 8){

            return "";

        }else{

            return TagHex.substring(0, TagHex.length() - 8);

        }

    }

}