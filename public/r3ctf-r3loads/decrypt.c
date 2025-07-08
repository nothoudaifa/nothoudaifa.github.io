#include <stdint.h>
#include <stdio.h>



uint64_t dec(uint32_t first, uint32_t second, uint32_t *key, uint32_t c, uint32_t* shifts) {
    uint32_t S[256];
    uint32_t v168 = 0;
    uint64_t v185 = 0;

    // Initialize S-box
    for (int i = 0; i <= 255; ++i)
        S[i] = i;

    // RC4-style key scheduling
    uint32_t v170 = 0, v171 = 0;
    for (int i = 0; i <= 255; ++i) {
        uint32_t v184 = S[i];
        v170 = (v184 + v170 + key[v171]) & 0xFF;
        uint32_t tmp = S[i];
        S[i] = S[v170];
        S[v170] = tmp;

        if (++v171 > 3)
            v171 = 0;
    }
    uint32_t finalc = 0;
    for (int i = 0; i <= 0x478; i++) {
        finalc += c;
    }
    v168 = finalc;

    do {
        uint32_t v181 = (v168 >> 11) & 0xFF; // 3ndna
        uint32_t v183 = S[v181] + v168; // 3ndna

        uint32_t v179 = (first >> shifts[2]) ^ (first << shifts[3]);

        uint32_t v180 = v179 + first;

        second -= v180 ^ v183;

        v168-=c;

        uint32_t v172 = v168 & 3; // this we have, just decrement v168 
        uint32_t v175 = (S[v172] + v168); // this too we have
                                          
        uint32_t v176 = ((second << shifts[0]) ^ (second >> shifts[1])) + second; // use the decrypted second value to get this value
                                          
        first -= v175 ^ v176;  // xor this to get the decrypted  first value
        v185+=1;

    }
    while (v185 <= 0x478);
    
    uint64_t rslt = first | ((uint64_t)second << 32);
    return rslt ;

}
uint64_t decrypt(uint64_t cipher, uint32_t* key, uint32_t c, uint32_t* shifts) {
    
    uint32_t first = cipher & 0xffffffff;
    uint32_t second = cipher >> 32;
    
    return dec(first, second, key, c, shifts);
}


int main (int argc, char** argv) {
    FILE* keys = fopen("./keys", "r");
    FILE* shifts= fopen("./shifts", "r");
    FILE* cs = fopen("./c", "r");
    FILE* ciphers = fopen("./cipher", "r");
    FILE* out = fopen("./output.jpg", "w");

    uint64_t cipher;
    uint32_t key[4];
    uint32_t shift[4];
    uint32_t c;
    //printf("[");
    for (int i = 1; i < 11423+1; i++) {
        fread(key, 4, sizeof(uint32_t), keys);
        fread(shift, 4, sizeof(uint32_t), shifts);
        fread(&cipher, 1, sizeof(uint64_t), ciphers);
        fread(&c, 1, sizeof(uint32_t), cs);
        /*
        printf("%u\n", key[0]);
        printf("%u\n", key[1]);
        printf("%u\n", key[2]);
        printf("%u\n", key[3]);
        printf("%u\n", shift[0]);
        printf("%u\n", shift[1]);
        printf("%u\n", shift[2]);
        printf("%u\n", shift[3]);
        printf("0x%lx\n", cipher);
        printf("0x%x\n", c);
*/
        uint64_t rslt = decrypt(cipher, key, c, shift);
        fwrite(&rslt, 1, 8, out);
        //printf("0x%lx,", rslt);
    }
    //printf("]\n");

}
