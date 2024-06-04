#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_LENGTH 10
#define DEFAULT 500
// -------- Helper functions -------
char* _replace_chars(char* s, char c, char n){
    int l = strlen(s);
    char* r = malloc(l +1 );
    for(int i = 0; i < l; i++){
        if(s[i] == c){
            r[i] = n;
        } else {
            r[i] = s[i];
        }
    }
    return r;
}

char* _fill_for_matrix(char* s, char c, int l){
    int sl = strlen(s);
    int missing = (l - sl%l);
    char* r = malloc(sl + missing + 1);
    
    memcpy(r, s, sl);
    for(int i = sl; i < sl + missing; i++){
        r[i] = c;
    }
    return r;
}

void _randomize_int_array(int a[]){
    srand(time(NULL));
    
    int l = sizeof(&a)/sizeof(a[0]);
    
    for(int i = 0; i < l; i++){
        int j = rand() % (10);
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

// ------- Transposition Matrix ------------

char* matrix_cipher(char* plain, int key[]){
    int l = strlen(plain);
    
    // --------- get matrix ---------------
    char* m = _fill_for_matrix(_replace_chars(plain, ' ', '%'), '%', MATRIX_LENGTH);

     // Fill ciphertext
     int ml = strlen(m);
     char* c = malloc(ml + 1);
     
     int mr = ml/MATRIX_LENGTH;
     int cout = 0;
     for(int i = 0; i < MATRIX_LENGTH; i++){
         for(int j = 0; j < mr; j++){
             c[cout++] = m[(j*MATRIX_LENGTH)+(key[i]-1)];
         }
     }
     
     return c;
}

char* matrix_decipher(char* cipher, int key[]){
    int l = strlen(cipher);
    char p [l];
    char* r = malloc(l + 1);
    int pr = l/MATRIX_LENGTH;
    
    int cout = 0;
    for(int i = 0; i < MATRIX_LENGTH; i++){
        for(int j = 0; j < pr; j++){
            p[(j*MATRIX_LENGTH) + key[i]-1] = cipher[cout++];
            
        }
    }
    for(int i = 0; i < l; i++){
        r[i] = p[i];
    }
    
    char* ret = _replace_chars(r, '%', ' ');
    return ret;
}

int main() {
    int key [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    _randomize_int_array(key);

    char* path = malloc(sizeof(char) * DEFAULT);
    printf("Insert path for plaintext message :\n");
    scanf("%s", path);

    FILE *ptr = fopen(path, "r");

    if(ptr == NULL){
	    printf("file not found");
	    return 0;
    }

    char *line = NULL;
    size_t chars = 0L, size = DEFAULT;
    char **buffer = malloc(sizeof(char) * DEFAULT);
    size_t buffer_size = 0L;
    size_t buffer_lines = 0L;
    while((chars = getline(&line,&size,ptr)) != -1){
	    buffer[buffer_lines] = malloc(sizeof(char) * chars);
	    strcpy(buffer[buffer_lines], line);
            buffer_size += chars;
	    buffer_lines++;
    }

    fclose(ptr);
    
    char* message = malloc(sizeof(char)*buffer_size +1);
    strcpy(message, buffer[0]);
    for(int i = 1; i < buffer_lines; i++){
	    strcat(message, buffer[i]);
    }

    char* cipher = matrix_cipher(message, key);
    char* decipher = matrix_decipher(cipher, key);

    FILE *ciph = fopen("cipher_transposition", "w");
    FILE *dech = fopen("decipher_transposition", "w");

    fprintf(ciph, "%s", cipher);
    fprintf(dech, "%s", decipher);

    fclose(ciph);
    fclose(dech);
    
}
