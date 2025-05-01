# 🛡️ DLMS Crypto Tool

**DLMS Crypto Tool** is a Java command-line interface (CLI) utility for encrypting, decrypting, and authenticating DLMS APDU messages.

## ✨ Features

- **🔒 Encryption:** Encrypts DLMS APDU messages.
- **🔑 Decryption:** Decrypts APDU messages.
- **🧾 Authentication:** Generates authentication tags.
- **💻 CLI Interface:** Use command-line options to perform encryption, decryption, authentication, and key generation.

## 🚀 Requirements

- Java 21

## How to use it

The package exposes a CLI command called `dlmscli` for easy interaction.

🔑 Generate a Random Encryption Key

```
java -jar dlms_crypto_tool.jar key
```

🔐 Encrypt an APDU Message

System Title = 5249435249435249\
Frame Counter = 80000001\
Encryption Key = 454E4352595054494F4E4B45594B4559\
Authentication Key = 41555448454E5449434154494F4E4B45\
APDU = c001810001000060010aff0200

```
java -jar dlms_crypto_tool.jar encrypt 5249435249435249 80000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 c001810001000060010aff0200
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
java -jar dlms_crypto_tool.jar decrypt 5249435249435249 80000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 0de63f2331a09aa85e8830f5f3
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
java -jar dlms_crypto_tool.jar auth 5249435249435249 00000001 454E4352595054494F4E4B45594B4559 41555448454E5449434154494F4E4B45 0de63f2331a09aa85e8830f5f3
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
