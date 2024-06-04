#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT 500

// --------- Helper Functions -----------
int _or_mask(int mask, int value){
    return mask|(1<<value);
}

int _and_mask(int mask, int value){
    return mask&(1<<value);
}

char* _strip_non_printable_characters(char* s){
    int l = strlen(s);
    char* c = malloc(l);
    int i = 0, j = 0;
    while(i < l){
        if(s[i] > 64){
            c[j] = s[i];
            j++;
        }
        i++;
    }
    return realloc(c, j);
}

char* _resize(char* s, int i){
    return realloc(s, i*1.5);
}

// -------- Playfair methods -------------

/* ------------------------------------------
 * Method 1: Accept a Secret Ket and return a 2d matrix
 * The method returns a char* array
 * ------------------------------------------
 */
char* playfair_matrix_init(char* key){
    char* matrix = malloc(sizeof(char) * 25 + 1 );
    char* k = _strip_non_printable_characters(key);
    int m = 0x0000;
    int l = strlen(k);
    int mc = 0;
    int r = 0, c = 0;
        
    // Fill in the keyword in the matrix
    for(int i = 0; i<l; i++){
        char tmp = k[i];
        
        if(tmp > 32) {
        
            if(tmp == 'J'){
                tmp = 'I';
            }
            
            if(!(_and_mask(m, (tmp)-'A'))){
                matrix[(r*5)+c] = tmp;
                m = _or_mask(m, (tmp)-'A');
                if(c == 4){
                    c = 0;
                    r++;
                } else {
                    c++;
                }
                mc++;
            }
        }
    }
    
    // Fill the rest of the alphabet
    char curr = 'A';
    while(mc < 26){
        if(curr == 74){
            curr++;
            mc++;
        } else {
            if(!(_and_mask(m,(curr-'A')))){
                matrix[(r*5)+c] = curr;
                m = _or_mask(m, (curr-'A'));   
                if(c == 4){
                    c = 0;
                    r++;
                } else {
                    c++;
                }
                mc++;
            }
                
            curr++;
        }
    }
    return matrix;
    
}

/* -----------------------------------------
 * Method 2: Accept plaintext and key matrix and generates ciphertext
 *-----------------------------------------
 */
char* playfair_cipher(char* plain, char* matrix){
    char a, b, c = '\0';
    int o = strlen(plain);
    char* p = _strip_non_printable_characters(plain);
    int l = strlen(p);
    char* cp = malloc(o + 1);
    
    int i = 0;
    int cl = 0, cpi = 0;
    while(cpi < l){
        if(c != '\0'){
            a = c;
            b = p[i];
            c = '\0';
        } else {
            a = p[i];
            b = p[++i];
        }

	if(a == '\0') return cp;

        if(a == b){
            c = b;
            if(b =='X'){
                b ='Q';
            } else {
                b = 'X';
            }
            
            l++;
	    cl++;
            // --- check text with no spaces has space ----
            if(l == o){
                strcpy(p, _resize(p, o));
                o*=1.5;
            }
        }

	if(b == '\0'){
		b = 'Z';
	}
	if(a == 'J') a = 'I';
	if(b == 'J') a = 'J';
        // ------------ find index of characters -------
        int ia = 0, ib = 0;
        for(int j = 0; j < strlen(matrix); j++){
            if(matrix[j] == a){
                ia = j;
            }
            if(matrix[j] == b){
                ib = j;
            }
        }
        
        int ra = ia/5, ca = ia%5, rb = ib/5, cb = ib%5;
        
        // Rule 1 : if letters are in the same column, move down
        if(ca == cb){
            ra = (ra+1)%5;
            rb = (rb+1)%5;
        }
        // Rule 2 : if letters are in the same row, move right
        else if(ra == rb){
            ca = (ca+1)%5;
            cb = (cb+1)%5;
        }
        // Rule 3 : if letters are in diff cols and rows, use rectangle and ret corners
        // we'll remain in the same row, but change column
        else {
            int tmp = ca;
            ca = cb;
            cb = tmp;
        }
      if(a != '\0'){ 
		memcpy(&cp[cpi],&matrix[ra*5+ca], 1);
	        memcpy(&cp[++cpi],&matrix[rb*5+cb], 1);
     }
        i++;
	cl+=2;
        cpi++;
        
    }
    
    return cp;
}

/* ---------------------------------------
 * Method 3: Accept ciphertext and key matrix and generate plaintext
 * ---------------------------------------
 */
 char* playfair_decipher(char* cipher, char* matrix){
     int o = strlen(cipher);
     char* pt = malloc(o+1);
     
     char a, b;
     int i = 0, cout = 0;
     while(cout < o){
         a = cipher[i];
         b = cipher[++i];
         
        // ----- find index ----
        int ia = 0, ib = 0;
        for(int j = 0; j < strlen(matrix); j++){
            if(matrix[j] == a){
                ia = j;
            }
            if(matrix[j] == b){
                ib = j;
            }
        }
        
        int ra = ia/5, ca = ia%5, rb = ib/5, cb = ib%5;
        
        // Rule 1 : if letters are in the same column, move up
        if(ca == cb){
            ra = (ra-1);
            if(ra == -1) ra = 4;
            rb = (rb-1);
            if(rb == -1) rb = 4;
        }
        // Rule 2 : if letters are in the same row, move left
        else if(ra == rb){
            ca = (ca-1);
            if(ca == -1) ca = 4;
            cb = (cb-1);
            if(cb == -1) cb = 4;
        }
        // Rule 3 : if letters are in diff cols and rows, use rectangle and ret corners
        // we'll remain in the same row, but change column
        else {
            int tmp = ca;
            ca = cb;
            cb = tmp;
        }
        
        memcpy(&pt[cout], &matrix[ra*5+ca], 1);
        memcpy(&pt[++cout], &matrix[rb*5+cb], 1);
        i++;
        cout++;
     }
     
     char *ret = malloc(sizeof(char)*cout +1);
     memcpy(ret,pt,cout);
     return pt;
 }
 
int main() {
    char* key = malloc(sizeof(char)* DEFAULT);

    printf("Playfair cipher : insert key\n");
    scanf("%s", key);

    fflush(stdin);

    char* m = playfair_matrix_init(key);
    char* path = malloc(sizeof(char) * DEFAULT);
    printf("Insert path for file :\n");
    scanf("%s", path);

    fflush(stdin);

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
    while((getline(&line,&size,ptr)) > -1){
	    buffer[buffer_lines] = malloc(sizeof(char) * DEFAULT);
	    strcpy(buffer[buffer_lines], line);
	    buffer_lines++;
    }
    
    fclose(ptr);
    
    FILE *ciph  = fopen("cipher_playfair", "w");
    FILE *deciph = fopen ("decipher_playfair", "w");
    for(int i = 0; i < buffer_lines; i++){
	char *t = playfair_cipher(buffer[i], m);
	    fprintf(ciph, "%s\n", t);
	    fprintf(deciph, "%s\n", playfair_decipher(t, m));
	    fflush(stdout);
	    free(t);
    }
    fclose(ciph);
    fclose(deciph);
    return 0;
}
