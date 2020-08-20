#include <iostream>
#include <vector>

char *encode(char *plaintext, unsigned long key);
char *decode(char *ciphertext, unsigned long key);
unsigned long theSize(char *text);

#ifndef MARMOSET_TESTING
int main() {
    char str0[] = "Hello world!";
    char str1[] = "A Elbereth Gilthoniel\nsilivren penna miriel\n""o menel aglar elenath!\nNa-chaered palan-diriel\n""o galadhremmin ennorath,\nFanuilos, le linnathon\n""nef aear, si nef aearon!";
    std::cout << "\"" << str0 << "\"" << std::endl;
    char *ciphertext = encode( str0, 51323 );
    std::cout<< "\"" << ciphertext << "\"" << std::endl;
    decode(ciphertext, 51323);
    char *plaintext = decode(ciphertext, 51323);
    std::cout<< "\"" << plaintext << "\"" << std::endl;
    return 0;
}
#endif

char *encode(char *plaintext, unsigned long key) {
    unsigned int size = theSize(plaintext);

    //Create and randomize 8 bit array
    unsigned char S[256];
    for (unsigned int x = 0; x < 256; x++) {
        S[x] = x;
    }
    unsigned int j = 0,k;
    unsigned int hold;
    for (unsigned int i = 0; i < 256; i++) {
        k = i%64;
        j = (j + S[i] + (key >> k & 1L) )%256;
        hold = S[i];
        S[i] = S[j];
        S[j] = hold;
    }


    unsigned int a = 0, i = 0, r, R;
    while (plaintext[a] != '\0') {
        i = (i + 1)%256;
        j = (j + S[i])%256;
        hold = S[i];
        S[i] = S[j];
        S[j] = hold;
        r = (S[i] + S[j])%256;
        R = S[r];
        plaintext[a] = plaintext[a]^R;
        a++;
    }

    std::vector<char> ascii;



    for (unsigned int x = 0; x < size; x++) {
        ascii.push_back(plaintext[x]);
    }
    int holder = (ascii.size() - 1)%4;
    if (holder != 0) {
        for (int x = 4 - holder; x > 0; x--) {
            ascii.push_back('\0');
        }
    }
    std::vector<unsigned char> ciphertext(5*((ascii.size()-1)/4)+1);


    unsigned int bignumber;
    unsigned int a1, b1, c1, d1, e1;
    int y = 0;
    unsigned int x = 0;
    for ( ; x < ascii.size()-1; ) {
        bignumber = (((((static_cast<unsigned char>(ascii[x]) << 8) + static_cast<unsigned char>(ascii[x + 1])) << 8) + static_cast<unsigned char>(ascii[x + 2])) << 8) + static_cast<unsigned char>(ascii[x + 3]);

        e1 = bignumber%85;
        bignumber /= 85;
        d1 = bignumber%85;
        bignumber /= 85;
        c1 = bignumber%85;
        bignumber /= 85;
        b1 = bignumber%85;
        bignumber /= 85;
        a1 = bignumber%85;


        ciphertext[y] = a1 + '!';
        ciphertext[y + 1] = b1 + '!';
        ciphertext[y + 2] = c1 + '!';
        ciphertext[y + 3] = d1 + '!';
        ciphertext[y + 4] = e1 + '!';

        x += 4;
        y += 5;
    }

    char* cipherpoint = new char[ciphertext.size()];
    for (int end = 0; end < ciphertext.size(); end++) {
        cipherpoint[end] = ciphertext[end];
    }

    return cipherpoint;
}

char *decode(char *ciphertext, unsigned long key) {
    std::vector<unsigned char> ciphervector;
    for (int x = 0; ciphertext[x] != '\0'; x++) {
        ciphervector.push_back(ciphertext[x]);
    }
    ciphervector.push_back('\0');

    std::vector<unsigned char> ascii;
    unsigned int bignumber;
    int y = 0;
    unsigned int x = 0;
    for ( ; x < ciphervector.size()-1; ) {
        bignumber = (((((ciphervector[x] - '!') * 85) + (ciphervector[x + 1]) - '!') * 85 + (ciphervector[x + 2]) - '!') * 85 + (ciphervector[x + 3]) - '!') * 85 + (ciphervector[x + 4] - '!');
        ascii.push_back((bignumber >> 24) & 255);
        ascii.push_back((bignumber >> 16) & 255);
        ascii.push_back((bignumber >> 8) & 255);
        ascii.push_back(bignumber & 255);
        x += 5;
    }

    unsigned char S[256];
    for (unsigned int x = 0; x < 256; x++) {
        S[x] = x;
    }
    unsigned int j = 0,k;
    unsigned int hold;
    for (unsigned int i = 0; i < 256; i++) {
        k = i%64;
        j = (j + S[i] + (key >> k & 1L) )%256;
        hold = S[i];
        S[i] = S[j];
        S[j] = hold;
    }


    unsigned int a = 0, i = 0, r, R;
    while (ascii[a] != '\0') {
        i = (i + 1)%256;
        j = (j + S[i])%256;
        hold = S[i];
        S[i] = S[j];
        S[j] = hold;
        r = (S[i] + S[j])%256;
        R = S[r];
        ascii[a] = ascii[a]^R;
        a++;
    }




    char* plainpoint = new char[ascii.size()];
    for (int end = 0; end < ascii.size(); end++) {
        plainpoint[end] = ascii[end];
    }

    return plainpoint;
}

unsigned long theSize(char *text) {
    unsigned long size = 0;
    for (unsigned long x = 0; text[x] != '\0'; x++) {
        size++;
    }
    return size;
}
