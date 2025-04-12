const assert = require("assert");
const {
    createIV,
    encryptAPDU,
    decryptAPDU,
    authAPDU,
} = require("../lib/dlmsCryptoTool");

describe("DLMS Crypto Tool", function() {

    // Set up test inputs
    const system_title = "5249435249435249";
    const frame_counter = "80000001";
    const encryption_key = "454E4352595054494F4E4B45594B4559";
    const aad = "41555448454E5449434154494F4E4B45";
    const plaintext = "c001810001000060010aff0200";

    it("should  create a valid IV (12 bytes)", function() {

        const iv = createIV(system_title, frame_counter, encryptAPDU, authAPDU);

        assert.strictEqual(iv.length, 12, "IV length should be 12 bytes");

    });

    it("should encrypt and decrypt the APDU correctly", function(){

        // Encrypt the plaintext into a ciphertext + tag hex string
        const encryptedHex = encryptAPDU(system_title, frame_counter, encryption_key, aad, plaintext);

        // Decrypt the generated ciphertext to recover the original plaintext
        const decryptedHex = decryptAPDU(system_title, frame_counter, encryption_key, aad, encryptedHex);

        assert.strictEqual(decryptedHex, plaintext, "Decrypted plaintext does not match the original");
    });

    it("should generate an authentication tag of the expected length", function() {

        // Sample authentication_key and stoc values for testing
        const authentication_key = "41555448454E5449434154494F4E4B45";
        const stoc = "0de63f2331a09aa85e8830f5f3";
        const tag = authAPDU(system_title, frame_counter, encryption_key, authentication_key, stoc);

        // Expecting a truncated tag (24 hex characters)
        assert.strictEqual(tag.length, 24, "Authentication tag length should be 24 hex characters");
    });

});