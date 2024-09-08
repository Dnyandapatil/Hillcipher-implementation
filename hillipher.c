#include <stdio.h>
#include <stdlib.h>
#define MATRIX_SIZE 8

int determinant(int [][MATRIX_SIZE],int);
void cofactor(int [][MATRIX_SIZE], int,char *,char *);
void transpose(int [][MATRIX_SIZE], int [][MATRIX_SIZE],int,char *,char *);

void display_key_matrix(int (*key_matrix)[MATRIX_SIZE]) {
    printf("Key Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", key_matrix[i][j]);
        }
        printf("\n");
    }
}

void create_key_matrix(int (*key_matrix)[MATRIX_SIZE]) {
    int values[MATRIX_SIZE][MATRIX_SIZE] = {
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1}
    };
    // Copy values to key_matrix
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            key_matrix[i][j] = values[i][j];
        }
    }
}

void cipher(char* original_msg, int (*key_matrix)[MATRIX_SIZE], char* encoded_msg) {
    int msg[MATRIX_SIZE][1];
    int result[MATRIX_SIZE][1];
    
    // Convert the original message to a column vector
    for (int i = 0; i < MATRIX_SIZE; i++) {
        msg[i][0] = (int)original_msg[i];
    }

    // Perform matrix multiplication and store the result in a 2D array
    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[i][0] = 0;
        for (int k = 0; k < MATRIX_SIZE; k++) {
            result[i][0] += key_matrix[i][k] * msg[k][0];
        }
        result[i][0] %= 26; // Handle negative values after the modulo operation
    }

    // Convert the result back to characters and store in encoded_msg
    for (int i = 0; i < MATRIX_SIZE; i++) {
        encoded_msg[i] = (char)(result[i][0] + 'A');
    }
}

void decipher(char* encoded_msg, int (*key_matrix)[MATRIX_SIZE], char* decoded_msg) {
    int inverse_key[MATRIX_SIZE][MATRIX_SIZE];
    // Calculate the inverse of the key matrix
    int d = determinant(key_matrix, MATRIX_SIZE);
    if (d == 0)
        printf("\nInverse of Entered Matrix is not possible\n");
    else if(d % 2 == 0)
        printf("\nDeterminant of key matrix must be odd\n");
    else
        cofactor(key_matrix, MATRIX_SIZE, encoded_msg, decoded_msg);
}

int determinant(int key_matrix[MATRIX_SIZE][MATRIX_SIZE], int k) {
    int s = 1, det = 0, b[MATRIX_SIZE][MATRIX_SIZE];
    int i, j, m, n, c;
    if (k == 1)
        return key_matrix[0][0];
    else {
        det = 0;
        for (c = 0; c < k; c++) {
            m = 0;
            n = 0;
            for (i = 0; i < k; i++) {
                for (j = 0; j < k; j++) {
                    if (i != 0 && j != c) {
                        b[m][n] = key_matrix[i][j];
                        if (n < (k - 2))
                            n++;
                        else {
                            n = 0;
                            m++;
                        }
                    }
                }
            }
            det = det + s * (key_matrix[0][c] * determinant(b, k - 1));
            s = -1 * s;
        }
    }
    return det;
}

void cofactor(int num[MATRIX_SIZE][MATRIX_SIZE], int f, char(*encoded_msg), char(*decoded_msg)) {
    int b[MATRIX_SIZE][MATRIX_SIZE], fac[MATRIX_SIZE][MATRIX_SIZE];
    int p, q, m, n, i, j;
    for (q = 0; q < f; q++) {
        for (p = 0; p < f; p++) {
            m = 0;
            n = 0;
            for (i = 0; i < f; i++) {
                for (j = 0; j < f; j++) {
                    if (i != q && j != p) {
                        b[m][n] = num[i][j];
                        if (n < (f - 2))
                            n++;
                        else {
                            n = 0;
                            m++;
                        }
                    }
                }
            }
            fac[q][p] = (q + p) % 2 == 0 ? determinant(b, f - 1) : -determinant(b, f - 1);
        }
    }
    transpose(num, fac, f, encoded_msg, decoded_msg);
}

void transpose(int num[MATRIX_SIZE][MATRIX_SIZE], int fac[MATRIX_SIZE][MATRIX_SIZE], int r, char(*encoded_msg), char(*decoded_msg)) {
    int i, j;
    int b[MATRIX_SIZE][MATRIX_SIZE], inverse[MATRIX_SIZE][MATRIX_SIZE], d;

    for (i = 0; i < r; i++) {
        for (j = 0; j < r; j++) {
            b[i][j] = fac[j][i];
        }
    }

    d = determinant(num, r);
    for (i = 0; i < r; i++) {
        for (j = 0; j < r; j++) {
            inverse[i][j] = b[i][j] / d;
        }
    }

    printf("\n\n\nThe inverse of matrix is : \n");
    for (i = 0; i < r; i++) {
        for (j = 0; j < r; j++) {
            printf("\t%d", inverse[i][j]);
        }
        printf("\n");
    }

    int msg[MATRIX_SIZE][1];
    int result[MATRIX_SIZE][1];

    for (int i = 0; i < MATRIX_SIZE; i++) {
        msg[i][0] = (int)encoded_msg[i];
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[i][0] = 0;
        for (int k = 0; k < MATRIX_SIZE; k++) {
            result[i][0] += inverse[i][k] * msg[k][0];
        }
        result[i][0] %= 26;
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        decoded_msg[i] = (char)(result[i][0] + 'A');
    }
}

int main() {
    int key_matrix[MATRIX_SIZE][MATRIX_SIZE];
    char original_msg[MATRIX_SIZE];
    char encoded_msg[MATRIX_SIZE];
    char decoded_msg[MATRIX_SIZE];

    create_key_matrix(key_matrix);
    printf("Enter the original message (8 characters): ");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        scanf(" %c", &original_msg[i]);
    }

    display_key_matrix(key_matrix);
    cipher(original_msg, key_matrix, encoded_msg);
    printf("Encoded Message: ");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("%c", encoded_msg[i]);
    }

    decipher(encoded_msg, key_matrix, decoded_msg);
    printf("\nDecoded Message: ");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("%c", decoded_msg[i]);
    }
    printf("\n");
    return 0;
}
