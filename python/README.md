# 🛡️ DLMS Crypto Tool

**DLMS Crypto Tool** is a Python package and command-line interface (CLI) utility for encrypting, decrypting, and authenticating DLMS APDU messages. It offers both a modular API for integration into your Python projects and a standalone CLI for quick operations.

## ✨ Features

- **🔒 Encryption:** Encrypts DLMS APDU messages.
- **🔑 Decryption:** Decrypts APDU messages.
- **🧾 Authentication:** Generates authentication tags.
- **💻 CLI Interface:** Use command-line options to perform encryption, decryption, authentication, and key generation.
- **📁 File I/O Support:** Optionally load input data from files and output results to files.
- **📝 Verbose Mode:** Provides detailed logging for debugging and traceability.
- **📚 Documentation:** Detailed docs generated using MkDocs to guide you through using the tool.

## 🚀 Requirements

- Python 3.6 or higher
- [cryptography](https://pypi.org/project/cryptography/)

## 📦 Installation

Install **DLMS Crypto Tool** from PyPI using pip:

```
pip install dlms_crypto_tool
```

Alternatively, clone the repository and install locally:

```
git clone https://github.com/ric-geek/dlms-encrypt-decrypt-tool.git
cd dlmscrypto
pip install .
```

## How to use it

The package exposes a CLI command called `dlmscli` for easy interaction.

🔑 Generate a Random Encryption Key

```
dlmscli -k
```

🔐 Encrypt an APDU Message

System Title = 5249435249435249\
Frame Counter = 80000001\
Encryption Key = 454E4352595054494F4E4B45594B4559\
Authentication Key = 41555448454E5449434154494F4E4B45\
APDU = c001810001000060010aff0200

```
dlmscli -e 5249435249435249 80000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 c001810001000060010aff0200
```
Result
```
Encrypted/Decrypted APDU: 0de63f2331a09aa85e8830f5f3
```

🔓 Decrypt an APDU Message

System Title = 5249435249435249\
Frame Counter = 80000001\
Encryption Key = 454E4352595054494F4E4B45594B4559\
Authentication Key = 41555448454E5449434154494F4E4B45\
APDU = 0de63f2331a09aa85e8830f5f3

```
dlmscli -d 5249435249435249 80000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 0de63f2331a09aa85e8830f5f3
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
dlmscli -a 5249435249435249 00000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 0de63f2331a09aa85e8830f5f3
```
Result
```
TAG: 62d423292e0fe5320370881d
```

## 📜 License

This project is licensed under the ### GNU General Public License v3.0

## 🙏 Acknowledgments

Special thanks to the [cryptography](https://github.com/pyca/cryptography) team for providing an excellent library.




