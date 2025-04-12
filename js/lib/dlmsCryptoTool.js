const crypto = require("crypto");

// Security header constans
const SECURITY_HEADER = {

    auth: '10',
    data: '30'

};

// Create IV by concatenating system_title and frame_counter (hex string)
function createIV(system_title, frame_counter){

    // Concatenation of hex strings and conversion to Buffer
    return Buffer.from(system_title + frame_counter, 'hex');
}

/**
 * Encrypts an APDU message using AES-GCM.
 * @param {string} system_title - Hex string
 * @param {string} frame_counter - Hex string
 * @param {string} encryption_key - Hex string key
 * @param {string} additional_auth_data - Additional AAD without the header (hex string)
 * @param {string} plaintext - Plaintext APDU as a hex string
 * @returns {string} The ciphertext with appended auth tag as a hex string
 */
function encryptAPDU(system_title, frame_counter, encryption_key,additional_auth_data,
                     plaintext) {

    const iv = createIV(system_title, frame_counter);
    const keyBuffer = Buffer.from(encryption_key, 'hex');
    const aadBuffer = Buffer.from(SECURITY_HEADER.data + additional_auth_data, 'hex');

    const cipher = crypto.createCipheriv('aes-128-gcm', keyBuffer, iv, {authTagLength: 16});
    cipher.setAAD(aadBuffer);

    // Encrypt the plaintext (which is provided as a hex string)
    const plaitextBuffer = Buffer.from(plaintext, 'hex');
    const encrypted = Buffer.concat([cipher.update(plaitextBuffer), cipher.final()]);
    const tag = cipher.getAuthTag();

    // Return concatenated ciphertext + tag as hex string.
    return Buffer.concat([encrypted, tag]).toString('hex');
    
}

/**
 * Decrypts an APDU message using AES-GCM.
 * @param {string} system_title - Hex string representing the system title.
 * @param {string} frame_counter - Hex string representing the frame counter.
 * @param {string} encryption_key - Hex string decryption key.
 * @param {string} additional_auth_data - Hex string of additional authentication data.
 * @param {string} ciphertextWithTagHex - Hex string that contains both ciphertext and appended auth tag.
 * @returns {string} The decrypted APDU as a hex string.
 */
function decryptAPDU(system_title, frame_counter, encryption_key,
                     additional_auth_data, ciphertextWithTagHex) {

    const buffer = Buffer.from(ciphertextWithTagHex, 'hex');
    const iv = createIV(system_title, frame_counter);
    const keybuffer = Buffer.from(encryption_key, 'hex');
    const aadBuffer = Buffer.from(SECURITY_HEADER.data + additional_auth_data, 'hex');

    // Separate tag (last 16 bytes) from cipheredtext.
    const tag = buffer.slice(buffer.length - 16);
    const cipheredtext = buffer.slice(0, buffer.length - 16);

    const decipher = crypto.createDecipheriv('aes-128-gcm', keybuffer, iv, {authTagLength: 16});

    decipher.setAAD(aadBuffer);
    decipher.setAuthTag(tag);

    const decrypted = Buffer.concat([decipher.update(cipheredtext), decipher.final()]);

    return decrypted.toString('hex');

}

/**
 * Generates an authentication tag by encrypting an empty plaintext.
 * @param {string} system_title - Hex string representing the system title.
 * @param {string} frame_counter - Hex string representing the frame counter.
 * @param {string} encryption_key - Hex string encryption key.
 * @param {string} authentication_key - Hex string authentication key.
 * @param {string} stoc - Hex string
 * @returns {string} A truncated auth tag (hex string, last 8 hex characters removed).
 */
function authAPDU(system_title, frame_counter, encryption_key,authentication_key,stoc){

    const iv = createIV(system_title, frame_counter);
    const keybuffer = Buffer.from(encryption_key, 'hex');
    const aadBuffer = Buffer.from(SECURITY_HEADER.auth + authentication_key + stoc, 'hex');

    const cipher = crypto.createCipheriv('aes-128-gcm', keybuffer, iv, {authTagLength: 16});

    cipher.setAAD(aadBuffer);

    // Encrypt an empty plaintext buffer
    cipher.update(Buffer.alloc(0));
    cipher.final();

    const tag = cipher.getAuthTag().toString('hex');

    // Truncate last 8 hex characters
    return tag.slice(0, tag.length - 8);

}

module.exports = {
    createIV,
    encryptAPDU,
    decryptAPDU,
    authAPDU,
    SECURITY_HEADER,
};