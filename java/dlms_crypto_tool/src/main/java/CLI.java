public class CLI {

    /**
     * Prints usage information for this CLI application.
     */
    private static void printUsage() {
        String usage = "\nUsage:\n"
                + "  java CLI encrypt <systemTitle> <frameCounter> <encryptionKey> <additionalAuthData> <plaintext>\n"
                + "  java CLI decrypt <systemTitle> <frameCounter> <encryptionKey> <additionalAuthData> <ciphertextWithTag>\n"
                + "  java CLI auth    <systemTitle> <frameCounter> <encryptionKey> <authenticationKey> <stoc>\n\n"
                + "Note: All parameters must be provided as hex strings.\n"
                + "      The IV is created by concatenating systemTitle and frameCounter and should be exactly 12 bytes (24 hex characters).\n";
        System.out.println(usage);
    }

    /**
     * Main entry point
     */
    public static void main(String[] args) {

        if (args.length == 0) {

            printUsage();

            System.exit(1);

        }

        String command = args[0].toLowerCase();

        try {

            switch (command) {

                case "encrypt":

                    if (args.length != 6) {

                        printUsage();

                        System.exit(1);

                    }

                    // Parameters: encrypt systemTitle frameCounter encryptionKey additionalAuthData plaintext
                    String encSystemTitle = args[1];
                    String encFrameCounter = args[2];
                    String encKey = args[3];
                    String encAdditionalAuthData = args[4];
                    String plaintext = args[5];
                    String cipherText = DLMSCryptoTool.encryptApdu(encSystemTitle, encFrameCounter, encKey,
                            encAdditionalAuthData, plaintext);

                    System.out.println("Encrypted APDU: " + cipherText);

                    break;

                case "decrypt":

                    if (args.length != 6) {

                        printUsage();

                        System.exit(1);

                    }

                    // Parameters: decrypt systemTitle frameCounter encryptionKey additionalAuthData ciphertextWithTag
                    String decSystemTitle = args[1];
                    String decFrameCounter = args[2];
                    String decKey = args[3];
                    String decAdditionalAuthData = args[4];
                    String cipherTextWithTag = args[5];
                    String decrypted = DLMSCryptoTool.decryptApdu(decSystemTitle, decFrameCounter, decKey,
                            decAdditionalAuthData, cipherTextWithTag);

                    System.out.println("Decrypted APDU: " + decrypted);

                    break;

                case "auth":

                    if (args.length != 6) {

                        printUsage();

                        System.exit(1);

                    }

                    // Parameters: auth systemTitle frameCounter encryptionKey authenticationKey stoc
                    String authSystemTitle = args[1];
                    String authFrameCounter = args[2];
                    String authEncryptionKey = args[3];
                    String authenticationKey = args[4];
                    String stoc = args[5];
                    String authTag = DLMSCryptoTool.authApdu(authSystemTitle, authFrameCounter,
                            authEncryptionKey, authenticationKey, stoc);

                    System.out.println("Authentication Tag: " + authTag);

                    break;

                default:

                    printUsage();

                    System.exit(1);

            }
        } catch (Exception e) {

            System.err.println("Error encountered while executing command '" + command + "': " + e.getMessage());

            e.printStackTrace();

            System.exit(1);

        }

    }

}