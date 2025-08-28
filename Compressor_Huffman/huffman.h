#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "tiposonlinegdb.h" // U8 is defined here
#include "codigo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Size constants
#define MAX_EXT_LEN 10
#define MAX_PATH_LEN_HUFFMAN 1024

// Node structure
typedef struct No {
    unsigned char data;  // Changed from U8 to unsigned char for consistency
    int freq;
    struct No *esq, *dir;
} No;

// Huffman codes structure
typedef struct HuffmanCodes {
    Codigo* codigos[256];
} HuffmanCodes;

// Huffman tree structure
typedef struct ArvoreHuffman {
    No* raiz;
    HuffmanCodes huffmanCodes;
} ArvoreHuffman;

// Function prototypes
No* criarNo(unsigned char data, int freq);  // Changed U8 to unsigned char
void liberarArvore(No* node);
No* construirArvore(unsigned char* dados, int* frequencias, int tamanho);  // Changed U8 to unsigned char
void gerarCodigos(No* raiz, HuffmanCodes* huffmanCodes);
void escreverArvore(FILE* arquivo, No* raiz, const char* extOrig);
No* lerArvore(FILE* arquivo);
int comprimir(const char* inputFile, const char* outputFile);
int descomprimir(const char* compressedFile, const char* decompressedFile);  // Fixed parameter name
void getFileExtension(const char* filename, char* extension, size_t extSize);

#endif