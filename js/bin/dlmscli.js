const crypto = require("crypto");
const {program} = require("commander");
const fs = require("fs");
const {encryptAPDU, decryptAPDU, authAPDU} = require("../lib/dlmsCryptoTool");

program
    .version('0.0.1')
    .description('CLI tool for encrypting, decrypting, and authenticating DLMS APDU');

program
    .command('key')
    .description('Generate a random 16 bytes encryption key')
    .action(() => {
        const key = crypto.randomBytes(16).toString('hex');
        console.log(`Encryption Key: ${key}`)
    });

program
    .command('encrypt <system_title> <frame_counter> <encryption_key> <aad> <apdu>')
    .option('--infile <path>', 'Read APDU from a file')
    .option('--outfile <path>', 'Write output to a file')
    .option('-v, --verbose', 'Enables verbose mode')
    .description('Encrypt an APDU message')
    .action((system_title, frame_counter, encryption_key, aad, apdu, options) => {

        if (options.infile) {

            apdu = fs.readFileSync(options.infile, 'utf8').trim();
            if (options.verbose) console.log(`Read APDU from ${options.infile}`);
        }

        const result = encryptAPDU(system_title, frame_counter, encryption_key, aad, apdu);
        // Split ciphertext & tag (tag is last 32 hex characters corresponding to 16 bytes).
        const ciphertext = result.slice(0, result.length - 32);
        const tag = result.slice(result.length - 32);
        const output = `Encrypted APDU: ${ciphertext}\nAuthentication TAG: ${tag}`;

        if (options.outfile) {

            fs.writeFileSync(options.outfile, output);
            if (options.verbose) console.log(`Written output to ${options.outfile}`);

        } else {

            console.log(output);

        }

    });

program
    .command('decrypt <system_title> <frame_counter> <encryption_key> <aad> <ciphertext>')
    .option('--infile <path>', 'Read ciphertext from a file')
    .option('--outfile <path>', 'Write output to a file')
    .description('Decrypt an APDU message')
    .action((system_title, frame_counter, encryption_key, aad, ciphertext, options) => {
        if (options.infile) {
            ciphertext = fs.readFileSync(options.infile, 'utf8').trim();
        }
        try {
            const plaintext = decryptAPDU(system_title, frame_counter, encryption_key, aad, ciphertext);
            const output = `Decrypted APDU: ${plaintext}`;
            if (options.outfile) {
                fs.writeFileSync(options.outfile, output);
            } else {
                console.log(output);
            }
        } catch (err) {
            console.error('Decryption failed:', err.message);
        }
    });

program
    .command('auth <system_title> <frame_counter> <encryption_key> <authentication_key> <stoc>')
    .description('Generate an authentication tag for an APDU')
    .action((system_title, frame_counter, encryption_key, authentication_key, stoc) => {
        const tag = authAPDU(system_title, frame_counter, encryption_key, authentication_key, stoc);
        console.log(`Authentication TAG: ${tag}`);
    });

program.parse(process.argv);