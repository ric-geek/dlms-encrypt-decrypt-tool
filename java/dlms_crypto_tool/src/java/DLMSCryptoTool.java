import javax.crypto.Chipher;
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

            data[i / 2] = (byte) ((Character.digits(s.charAt(i), 16) << 4) + Character.digits(s.charAt(i + 1), 16));

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
}