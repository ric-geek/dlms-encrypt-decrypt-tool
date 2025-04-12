# 🛡️ DLMS Crypto Tool

**DLMS Crypto Tool** is a Node.js package and command-line interface (CLI) utility for encrypting, decrypting, generate encryption key, and authenticating DLMS APDU messages.

## ✨ Features

- **🔒 Encryption:** Encrypts DLMS APDU messages.
- **🔑 Decryption:** Decrypts APDU messages.
- **🧾 Authentication:** Generates authentication tags.
- **💻 CLI Interface:** Interact with the tool via a command‑line utility (`dlmscli`).
- **📁 File I/O Support:** Optionally load input data from files and output results to files.
- **📝 Verbose Mode:** Provides detailed logging for debugging and traceability.
- **✅ Well-Tested:** Unit tests are provided with Mocha to ensure core functionality.

## 🚀 Requirements

- Node.js vs23.11.0 (tested only with this version)
- [Commander](https://www.npmjs.com/package/commander)
- [Mocha](https://mochajs.org/) if you want run Unit tests
- npm (comes with Node.js)

## 📦 Installation

You can install the package globally from npm to get the CLI:

```
npm install -g dlms_crypto_tool
```

Or install it as a dependecy in your project:

```
npm install dlms_crypto_tool
```

## How to use it

The package provide a CLI command called `dlmscli`.

🔑 Generate a Random Encryption Key

```
dlmscli key
```

🔐 Encrypt an APDU Message

System Title = 5249435249435249\
Frame Counter = 80000001\
Encryption Key = 454E4352595054494F4E4B45594B4559\
Authentication Key = 41555448454E5449434154494F4E4B45\
APDU = c001810001000060010aff0200

```
dlmscli encrypt 5249435249435249 80000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 c001810001000060010aff0200
```
*Tip:* Use --infile &lt;path&gt; to load input from a file and --outfile &lt;path&gt; to save the output

🔓 Decrypt an APDU Message

System Title = 5249435249435249\
Frame Counter = 80000001\
Encryption Key = 454E4352595054494F4E4B45594B4559\
Authentication Key = 41555448454E5449434154494F4E4B45\
APDU = 0de63f2331a09aa85e8830f5f3

```
dlmscli decrypt 5249435249435249 80000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 0de63f2331a09aa85e8830f5f3
```
Result
```
Encrypted/Decrypted APDU: c001810001000060010aff0200
```

🔎 Authenticate an APDU Message

System Title = 5249435249435249\
Frame Counter = 00000001\
Encryption Key = 454E4352595054494F4E4B45594B4559\
Authentication Key = 41555448454E5449434154494F4E4B45\
APDU = 0de63f2331a09aa85e8830f5f3
```
dlmscli auth 5249435249435249 00000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 0de63f2331a09aa85e8830f5f3
```
Result
```
TAG: 62d423292e0fe5320370881d
```

## 🧪 Running Tests

To run unit test using Mocha, follow these steps:

1. Clone the repository and install the development dependencies:

```
git clone https://github.com/YourUsername/dlms-crypto-tool.git
cd dlms-crypto-tool
npm install
```
2. Run tests:

```

npm test
```

## 🤝 Contributing

Contributions are welcome! To contribute:

1. Fork the repository.

2. Create a new branch for your changes.

3. Write tests for your modifications.

4. Submit a pull request with a detailed explanation of your changes.

## 📜 License

This project is licensed under the GNU General Public License v3.0