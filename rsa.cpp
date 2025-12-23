#include <iostream>
#include <random>
#include <math.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

long int expmod(long int a, long int e, long int m);
bool checkprime(long int p);
long int randomprime(int bits, std::mt19937& rng);
long int gcd(long int e, long int t );
long int exponent(long int t);
long int inversemod(long int e, long int t);

int main( ) {
    long int p, q, n, f, e, d;
    bool prime, size, manual;   
    long int ciphertext[100] = {};
    long int plaintext[100] = {};
    std::string message, random;
    std::cout << "Random prime generation? (yes or no): "; // asking if primes should be generated
    std::cin >> random;
    if (random=="yes" || random=="y" || random=="YES") {
        manual = false;
    } else {
        manual = true;
    }
    std::cin.ignore(); // ignore additional newline character
    if (manual) { // asking the user to provide the primes
        do {
            std::cout << "\nEnter a prime number p: ";
            std::cin >> p;
            if (std::cin && p > 0) { // positive whole number
                prime = checkprime(p);
                if (prime==false) {
                    std::cout << "\nThis number is not prime. Try again." << std::endl;
                }
            } else { // invalid input
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nThis is not a positive whole number. Try again." << std::endl;
                prime = false;
            }
        } while (prime==false);   
        do {
            std::cout << "\nEnter a prime number q: ";
            std::cin >> q;
            if (std::cin && q > 0) { // positive whole number
                prime = checkprime(q);
                size = (p*q>256);
                if (prime==false) {
                    std::cout << "\nThis number is not prime. Try again." << std::endl;
                } else if (size==false) {
                    std::cout << "\nThe product is too small for encryption. Try again." << std::endl;
                }
                std::cin.ignore(); // ignore additional newline character
            } else { // invalid input
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nThis is not a positive whole number. Try again." << std::endl;
                prime = false;
            }
        } while (prime==false || size==false);
    } else { // random prime number generation
        std::mt19937 rng(std::random_device{}());
        int bp, bq;
        do {
            bp = std::uniform_int_distribution<int>(4, 16)(rng);
            bq = std::uniform_int_distribution<int>(4, 16)(rng);
            p = randomprime(bp, rng);
            q = randomprime(bq, rng);
        } while (p==q || std::abs(bp-bq)<=0 || std::abs(bp-bq)>=30 || p*q<256); // > 0.1 in log2 space
    }
    std::cout << "\np = " << p << ", q = " << q << std::endl;
    n = p*q; // computing the modulus n
    std::cout << "\nThe modulus n = p*q = " << n << std::endl;
    f = (p-1)*(q-1); // computing Euler's totient function φ(n)
    std::cout << "Euler's totient function phi(n) = " << f << std::endl;
    e = exponent(f); // computing the exponent for encryption (public exponent)
    d = inversemod(e, f); // computing the exponent for decryption (private exponent)
    std::cout << "\nThe public key is (e = " << e << ", n = " << n << ")" << std::endl;
    std::cout << "The private key is (d = " << d << ", n = " << n << ")" << std::endl;
    std::cout << "\nMessage to be encrypted:" << std::endl;
    std::getline(std::cin, message); // asking the user for a message
    for (long int i = 0; i<message.length(); i++) {
        ciphertext[i] = expmod(message[i], e, n); // encryption
    }
    std::cout << "\nEncrypted Message:" << std::endl; // displaying ciphertext
    for (long int i = 0; i <message.length(); i++) {
        printf("%c", (char)ciphertext[i]); 
    }
    for (long int i = 0; i<message.length(); i++) {
        plaintext[i] = expmod(ciphertext[i], d, n); // decryption
    }
    std::cout << "\n\nDecrypted Message:" << std::endl; // displaying plaintext
    for (long int i = 0; i<message.length(); i++) {
        printf("%c", (char)plaintext[i]);
    }
    std::cout << std::endl;  
    return 0;
}

long int expmod(long int a, long int e, long int m) { // modular exponentiation
    long int result = 1;
    a%=m;
    while (e>0) {
        if (e&1) {
            result = (long long)result*a%m;
        }
        a = (long long)a*a%m;
        e>>=1;
    }
    return result;
}


bool checkprime(long int n) { // deterministic Miller-Rabin test for 32-bit long int
    if (n<2 || n%2==0) {
        return n == 2;
    }
    long int d = n-1, s = 0;
    while ((d&1)==0) {
        d>>=1;
        ++s;
    }
    for (long int a : {2, 7, 61}) { // enough for 32-bit
        if (a>=n) continue;
        long int x = expmod(a, d, n);
        if (x==1 || x==n-1) continue;
        bool composite = true;
        for (long int r = 1; r<s; ++r) {
            x = (long long)x*x%n;
            if (x==n-1) { 
                composite = false; 
                break; 
            }
        }
        if (composite) {
            return false;
        }
    }
    return true;
}

long int randomprime(int bits, std::mt19937& rng) { // generating a random prime number
    long int low = (1L << (bits-1)) | 1;
    long int high = (1L << bits)-1;
    std::uniform_int_distribution<long int> dist(low, high);
    while (true) {
        long int x = dist(rng) | 1;
        if (checkprime(x)) {
            return x;
        }
    }
}

long int exponent(long int f) { // finding a suitable exponent for RSA
    long int e;   
    for (e = 2; e<f; e++) {
        if (gcd(e, f)==1) {
            return e;
        }
    }
    return -1;
}

long int gcd(long int e, long int f) { // greatest commong divisor of two integers
    while (e>0) {
        long int tmp;
        tmp = e;
        e = f%e;
        f = tmp;
    }
    return f;
}

long int inversemod(long int e, long int f) { //extended Euclidean algorithm where e and f are assumed to be coprime
    long int t0 = f, x0 = 0, d = 1;
    while (e>1) {
        long int q = e/f;
        long int tmp = f;
        f = e%f;
        e = tmp;
        tmp = x0;
        x0 = d-q*x0;
        d = tmp;
    }
    if (d<0) {
        d += t0;
    }
    return d;
}
