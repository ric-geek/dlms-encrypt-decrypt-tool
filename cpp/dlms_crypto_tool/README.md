# 🛡️ DLMS Crypto Tool (C++ version)

**DLMS Crypto Tool** is a cross-platform C++ command-line interface (CLI) utility for encrypting, decrypting, and authenticating DLMS APDU messages.

## ✨ Features

- **🔒 Encryption:** Encrypts DLMS APDU messages.
- **🔑 Decryption:** Decrypts APDU messages.
- **🧾 Authentication:** Generates authentication tags.
- **💻 CLI Interface:** Use command-line options to perform encryption, decryption, authentication, and key generation.
- **📁 File I/O Support:** Optionally load input data from files and output results to files.
- **📝 Verbose Mode:** Provides detailed logging for debugging and traceability.
- **📚 Documentation:** Detailed docs generated using MkDocs to guide you through using the tool.

## 🚀 Requirements

- A C++17 compliant compiler (e.g. GCC 7+)
- [OpenSSL](https://www.openssl.org/) development libraries
    - On Ubuntu `sudo apt-get install libss-dev`
- GNU Make

## 📦 Building the project

### Using make

1. **Clone the repository:**
  ```bash
   git clone https://github.com/YourUsername/dlms_crypto_tool.git
   cd dlms_crypto_tool
 ```

2. **Compile with make:**
  ```bash
   make
  ```

## How to use it

🔑 Generate a Random Encryption Key

```bash
./dlmscli key
```

🔐 Encrypt an APDU Message

System Title = 5249435249435249\
Frame Counter = 80000001\
Encryption Key = 454E4352595054494F4E4B45594B4559\
Authentication Key = 41555448454E5449434154494F4E4B45\
APDU = c001810001000060010aff0200

```bash
./dlmscli encrypt 5249435249435249 80000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 c001810001000060010aff0200

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

```bash
./dlmscli decrypt 5249435249435249 80000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 0de63f2331a09aa85e8830f5f3
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

```bash
./dlmscli auth 5249435249435249 00000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 0de63f2331a09aa85e8830f5f3
```
Result
```
TAG: 62d423292e0fe5320370881d
```

```
Contributions are welcome! To contribute:

1. Fork the repository.

2. Create a new branch for your changes.

3. Write tests for your modifications.

4. Submit a pull request with a detailed explanation of your changes.
```

## 📜 License

This project is licensed under the GNU General Public License v3.0

## 🙏 Acknowledgments

Special thanks to the [OpenSSL](https://www.openssl.org/) team for providing an excellent library.




