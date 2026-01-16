## RSA encryption

This repository contains a small and simple implementation of the RSA algorithm in C++. 

The program randomly generates two different prime numbers $p$ and $q$ using a deterministic Miller-Rabin primality test for 32-bit `long int` values (on platforms where `long int` is larger, the test would need to be extended). Alternatively, $p$ and $q$ can be set manually by the user. It computes the RSA keys, and then encrypts and decrypts a message from the user character by character.

This project is inteded for learning purposes only and does not reflect how RSA is used in real cryptographic systems.

## Known Vulnerabilities
- **No Padding:** Because characters are encrypted one by one, identical letters always result in the same output. This makes the encryption easy to break using statistical analysis.
- **Small Key Sizes:** The implementation uses standard integer types (`long int`). Real security requires massive prime numbers, which would need specialized mathematical libraries.
- **Timing Attacks:** The calculation time depends directly on the value of the private key. An attacker could measure the execution time to reconstruct the secret key bit by bit.
