#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <cstdio>
#include <ctime>
#include <omp.h>

#define ASCII_A 97


bool isPrimeParallel(int a) {
    int i;
    int j = sqrt(a);
    bool result = true;
    omp_set_num_threads(8);

#pragma omp for
    for (i = 2; i <= j; i++) {
        if (a % i == 0) {
            result = false;
        }
    }
    return result;
}

bool isPrime(int a) {
    int i;
    int j = sqrt(a);
    bool result = true;
    for (i = 2; i <= j; i++) {
        if (a % i == 0) {
            result = false;
        }
    }
    return result;
}

int greatestDivider(int e, int f) {
    while (e > 0) {
        int myTemp;
        myTemp = e;
        e = f % e;
        f = myTemp;
    }

    return f;
}

int calculateEParallel(int f) {
    int tmp = -1;
    omp_set_num_threads(8);
    for (int e = 2; e < f; e++) {
        if (greatestDivider(e, f) == 1) {
            tmp = e;
            break;
        }
    }

    return tmp;
}

int calculateE(int f) {
    int tmp = -1;
    for (int e = 2; e < f; e++) {
        if (greatestDivider(e, f) == 1) {
            tmp = e;
            break;
        }
    }

    return tmp;
}

int calculateD(int e, int t) {
    int d;
    for (int k = 1 + t;; k += t) {
        if (k % e == 0) {
            d = (k / e);
            return d;
        }
    }
}

int encryptParallel(int i, int e, int n) {
    int current, result;
    int j;
    current = i - ASCII_A;
    result = 1;
    omp_set_num_threads(8);
//#pragma omp parallel reduction(*:result) private(j) shared(current)
//    {
#pragma omp for
    for (j = 0; j < e; j++) {
        result *= current;
        result %= n;
    }
    //}
    return result;
}


int encrypt(int i, int e, int n) {
    int current, result;

    current = i - ASCII_A;
    result = 1;

    for (int j = 0; j < e; j++) {
        result *= current % n;
        result = result % n;
    }

    return result;
}

int decryptParallel(int i, int d, int n) {
    int current, result;
    current = i;
    result = 1;
    int tmp = 1;
    int j = 0;
//#pragma omp parallel reduction(*:result) private(j, tmp) shared(current)
//    {
#pragma omp for
    for (j = 0; j < d; j++) {
        result *= current;
        result %= n;

    }
//    }
    return result + ASCII_A;
}

int decrypt(int i, int d, int n) {
    int current = i;
    int result = 1;

    for (int j = 0; j < d; j++) {
        result = result * current;
        result = result % n;
    }

    return result + ASCII_A;
}

void inputKoeffParallel(int &a) {
    std::cout << "Enter koeff: ";

    std::cin >> a;
    if (isPrimeParallel(a)) {
        return;
    }
    std::cout << a << " isn't prime, try again." << std::endl;
    inputKoeffParallel(a);
}

void inputKoeff(int &a) {
    std::cout << "Enter koeff: ";

    std::cin >> a;
    if (isPrime(a)) {
        return;
    }
    std::cout << a << " isn't prime, try again." << std::endl;
    inputKoeff(a);
}


void parallelRSA() {
    int p, q, n, f, e, d;

    int encryptedText[10000];
    memset(encryptedText, 0, sizeof(encryptedText));

    int decryptedText[10000];
    memset(decryptedText, 0, sizeof(decryptedText));

    std::string msg;
    inputKoeffParallel(p);
    inputKoeffParallel(q);

    n = p * q;
    f = (p - 1) * (q - 1);
    e = calculateEParallel(f);
    d = calculateD(e, f);

    std::cout << std::endl;
    std::cout << "F = " << f << ", n = " << n << ", e = " << e << ", d = " << d << std::endl;

    std::cout << "\nEnter Message: ";
    std::cin.ignore();
    std::getline(std::cin, msg);
    std::cout << "\nThe message is: " << msg << std::endl;
    omp_set_num_threads(8);

    unsigned int start = clock();
    unsigned int startEncryptTime = clock();
#pragma omp for
    for (int i = 0; i < msg.length(); i++) {
        encryptedText[i] = encryptParallel(msg[i], e, n);
    }
    unsigned int encryptTime = clock() - startEncryptTime;


    printf("Encrypted result: ");
    for (int i = 0; i < msg.length(); i++) {
        printf("%d ", encryptedText[i]);
    }
    printf("\n");

    unsigned int startDecryptTime = clock();
#pragma omp for
    for (int i = 0; i < msg.length(); i++) {
        decryptedText[i] = decryptParallel(encryptedText[i], d, n);
    }
    unsigned int decryptTime = clock() - startDecryptTime;

    printf("Decrypted result: ");
    for (int i = 0; i < msg.length(); i++) {
        printf("%c", (char) decryptedText[i]);
    }
    unsigned int time = clock() - start;

    std::cout << std::endl;
    std::cout << "Encrypting time: " << encryptTime << std::endl;
    std::cout << "Decrypting time: " << decryptTime << std::endl;
    std::cout << "Time: " << time << std::endl;
}

void simpleRSA() {
    int p, q, n, f, e, d;

    int encryptedText[10000];
    memset(encryptedText, 0, sizeof(encryptedText));

    int decryptedText[10000];
    memset(decryptedText, 0, sizeof(decryptedText));

    std::string msg;
    inputKoeff(p);
    inputKoeff(q);

    n = p * q;
    f = (p - 1) * (q - 1);
    e = calculateE(f);
    d = calculateD(e, f);

    std::cout << std::endl;
    std::cout << "F = " << f << ", n = " << n << ", e = " << e << ", d = " << d << std::endl;

    std::cout << "\nEnter Message: ";
    std::cin.ignore();
    std::getline(std::cin, msg);
    std::cout << "\nThe message is: " << msg << std::endl;

    unsigned int start = clock();

    unsigned int startEncryptTime = clock();
    for (int i = 0; i < msg.length(); i++) {
        encryptedText[i] = encrypt(msg[i], e, n);
    }
    unsigned int encryptTime = clock() - startEncryptTime;

    printf("Encrypted result: ");
    for (int i = 0; i < msg.length(); i++) {
        printf("%d ", encryptedText[i]);
    }
    printf("\n");

    unsigned int startDecryptTime = clock();
    for (int i = 0; i < msg.length(); i++) {
        decryptedText[i] = decrypt(encryptedText[i], d, n);
    }
    unsigned int decryptTime = clock() - startDecryptTime;

    printf("Decrypted result: ");
    for (int i = 0; i < msg.length(); i++) {
        printf("%c", (char) decryptedText[i]);
    }
    unsigned int time = clock() - start;
    std::cout << std::endl;
    std::cout << "Encrypting time: " << encryptTime << std::endl;
    std::cout << "Decrypting time: " << decryptTime << std::endl;
    std::cout << "Time: " << time << std::endl;
}

int main() {
    std::cout << "PARALLEL RSA " << std::endl;
    parallelRSA();
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "SIMPLE RSA " << std::endl;
    simpleRSA();
    return 0;
}

