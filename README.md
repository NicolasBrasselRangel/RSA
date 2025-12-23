## RSA encryption

This repository contains a small and simple implementation of the RSA algorithm in C++. 

The program randomly generates two different prime numbers $p$ and $q$ using a deterministic Miller-Rabin primality test for 32-bit `long int` values (on platforms where `long int` is larger, the test would need to be extended). Alternatively, $p$ and $q$ can be set manually by the user. It computes the RSA keys, and then encrypts and decrypts a message from the user character by character.

This project is inteded for learning purposes only and does not reflect how RSA is used in real cryptographic systems.
