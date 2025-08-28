#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

#define BUFFER_SIZE 8192

No* criarNo(unsigned char data, int freq) {
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) return NULL;
    
    novo->data = data;
    novo->freq = freq;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

void liberarArvore(No* node) {
    if (node == NULL) return;
    liberarArvore(node->esq);
    liberarArvore(node->dir);
    free(node);
}

No* construirArvore(unsigned char* dados, int* frequencias, int tamanho) {
    No *esq, *dir, *top;
    
    No** nos = (No**)malloc(tamanho * sizeof(No*));
    for (int i = 0; i < tamanho; i++) {
        nos[i] = criarNo(dados[i], frequencias[i]);
    }
    
    while (tamanho != 1) {
        int primeiro = 0, segundo = 1;
        if (nos[primeiro]->freq > nos[segundo]->freq) {
            primeiro = 1;
            segundo = 0;
        }
        
        for (int i = 2; i < tamanho; i++) {
            if (nos[i]->freq < nos[primeiro]->freq) {
                segundo = primeiro;
                primeiro = i;
            } else if (nos[i]->freq < nos[segundo]->freq) {
                segundo = i;
            }
        }
        
        esq = nos[primeiro];
        dir = nos[segundo];
        top = criarNo('$', esq->freq + dir->freq);
        top->esq = esq;
        top->dir = dir;
        
        nos[primeiro] = top;
        nos[segundo] = nos[tamanho - 1];
        tamanho--;
    }
    
    No* raiz = nos[0];
    free(nos);
    return raiz;
}


static void gerarCodigosAux(No* raiz, Codigo* codigoAtual, HuffmanCodes* huffmanCodes) {
    if (raiz->esq == NULL && raiz->dir == NULL) {
        huffmanCodes->codigos[raiz->data] = (Codigo*)malloc(sizeof(Codigo));
        novo_codigo(huffmanCodes->codigos[raiz->data]);
        clone(*codigoAtual, huffmanCodes->codigos[raiz->data]);
        return;
    }
    
    if (raiz->esq) {
        adiciona_bit(codigoAtual, 0);
        gerarCodigosAux(raiz->esq, codigoAtual, huffmanCodes);
        joga_fora_bit(codigoAtual);
    }
    
    if (raiz->dir) {
        adiciona_bit(codigoAtual, 1);
        gerarCodigosAux(raiz->dir, codigoAtual, huffmanCodes);
        joga_fora_bit(codigoAtual);
    }
}

//caminha pela árvore e constroi o dicionario.
void gerarCodigos(No* raiz, HuffmanCodes* huffmanCodes) {
    if (raiz == NULL) return;
    
    for (int i = 0; i < 256; i++) {
        huffmanCodes->codigos[i] = NULL;
    }
    
    Codigo codigoAtual;
    novo_codigo(&codigoAtual);
    gerarCodigosAux(raiz, &codigoAtual, huffmanCodes);
    free_codigo(&codigoAtual);
}

// Função auxiliar para escrever árvore recursivamente
static void escreverArvoreRec(FILE* arquivo, No* raiz) {
    if (raiz == NULL) {
        fputc('X', arquivo); // Marca nulo
        return;
    }

    if (raiz->esq == NULL && raiz->dir == NULL) {
        fputc('L', arquivo); // 'L' de folha (Leaf)
        fwrite(&raiz->data, sizeof(unsigned char), 1, arquivo);
    } else {
        fputc('I', arquivo); // 'I' de nó interno (Internal)
        escreverArvoreRec(arquivo, raiz->esq);
        escreverArvoreRec(arquivo, raiz->dir);
    }
}

void escreverArvore(FILE* arquivo, No* raiz, const char* extOrig) {
    // Escreve cabeçalho estruturado
    fprintf(arquivo, "HUFFv2|EXT=%s|TREE=", extOrig);
    
    // Serialização da árvore
    escreverArvoreRec(arquivo, raiz);
    
    // Delimitador final
    fprintf(arquivo, "|ENDTREE|");
}

// Função auxiliar para ler árvore recursivamente
static No* lerArvoreRec(FILE* arquivo) {
    int tipo = fgetc(arquivo);
    if (tipo == EOF) return NULL;

    switch (tipo) {
        case 'L': {
            unsigned char data;
            if (fread(&data, 1, 1, arquivo) != 1) return NULL;
            return criarNo(data, 0);
        }
        case 'I': {
            No* no = criarNo('$', 0);
            if (!no) return NULL;
            
            no->esq = lerArvoreRec(arquivo);
            no->dir = lerArvoreRec(arquivo);
            
            if (!no->esq || !no->dir) {
                liberarArvore(no);
                return NULL;
            }
            return no;
        }
        case 'X':
            return NULL;
        default:
            return NULL;
    }
}

No* lerArvore(FILE* arquivo) {
    // Verificar assinatura HUFFv2|EXT=
    char header[8];
    if (fread(header, 1, 7, arquivo) != 7) {
        fprintf(stderr, "Erro ao ler cabeçalho do arquivo\n");
        return NULL;
    }
    header[7] = '\0';
    
    if (strncmp(header, "HUFFv2|", 7) != 0) {
        fprintf(stderr, "Erro: Formato de arquivo inválido. Cabeçalho esperado: HUFFv2|\n");
        return NULL;
    }

    // Pular ate o início da árvore (depois de EXT=...|TREE=)
    int c;
    while ((c = fgetc(arquivo)) != '=' && c != EOF); // Pular EXT=
    if (c == EOF) return NULL;
    
    while ((c = fgetc(arquivo)) != '|' && c != EOF); // Pular extensão
    if (c == EOF) return NULL;
    
    while ((c = fgetc(arquivo)) != '=' && c != EOF); // Pular TREE=
    if (c == EOF) return NULL;

    // Ler árvore recursivamente
    return lerArvoreRec(arquivo);
}

int comprimir(const char* inputFile, const char* outputFile) {
    if (inputFile == NULL || outputFile == NULL) {
        fprintf(stderr, "Erro: Caminho de arquivo inválido\n");
        return 1;
    }

    // Abrir arquivo de entrada
    FILE* inFile = fopen(inputFile, "rb");
    if (!inFile) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    //Conta frequências dos bytes
    int frequencias[256] = {0};
    unsigned char byte;
    long totalBytes = 0;
    
    while (fread(&byte, sizeof(byte), 1, inFile) == 1) {
        frequencias[byte]++;
        totalBytes++;
    }
    fclose(inFile);

    if (totalBytes == 0) {
        fprintf(stderr, "Erro: Arquivo vazio\n");
        return 1;
    }

    // Construir árvore de Huffman
    unsigned char dadosUnicos[256];
    int freqsUnicas[256];
    int numUnicos = 0;
    
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            dadosUnicos[numUnicos] = (unsigned char)i;
            freqsUnicas[numUnicos] = frequencias[i];
            numUnicos++;
        }
    }

    if (numUnicos == 0) {
        fprintf(stderr, "Erro: Nenhum byte único encontrado\n");
        return 1;
    }

    No* root = construirArvore(dadosUnicos, freqsUnicas, numUnicos);
    if (!root) {
        fprintf(stderr, "Erro ao construir a árvore de Huffman\n");
        return 1;
    }

    //Gerar códigos Huffman
    HuffmanCodes huffmanCodes;
    for (int i = 0; i < 256; i++) {
        huffmanCodes.codigos[i] = NULL;
    }
    gerarCodigos(root, &huffmanCodes);

    // Extrair extensão do arquivo
    const char* ext = strrchr(inputFile, '.');
    char extOrig[MAX_EXT_LEN] = {0};
    if (ext != NULL) {
        strncpy(extOrig, ext + 1, sizeof(extOrig) - 1);
        extOrig[sizeof(extOrig) - 1] = '\0';
    }

    // Abrir arquivo de saída
    FILE* outFile = fopen(outputFile, "wb");
    if (!outFile) {
        perror("Erro ao abrir arquivo de saída");
        liberarArvore(root);
        return 1;
    }

    // Escrever árvore e tamanho
    escreverArvore(outFile, root, extOrig);
    fwrite(&totalBytes, sizeof(long), 1, outFile);

    //reabre arquivo de entrada para compressão
    inFile = fopen(inputFile, "rb");
    if (!inFile) {
        perror("Erro ao reabrir arquivo de entrada");
        fclose(outFile);
        liberarArvore(root);
        return 1;
    }

    //comprimir dados
    unsigned char buffer = 0;
    int bitCount = 0;
    
    while (fread(&byte, sizeof(byte), 1, inFile) == 1) {
        Codigo* codigo = huffmanCodes.codigos[byte];
        if (codigo) {
            for (int i = 0; i < codigo->tamanho; i++) {
                int byteIdx = i / 8;
                int bitIdx = 7 - (i % 8);
                
                if (codigo->byte[byteIdx] & (1 << bitIdx)) {
                    buffer |= (1 << (7 - bitCount));
                }
                bitCount++;
                
                if (bitCount == 8) {
                    fputc(buffer, outFile);
                    buffer = 0;
                    bitCount = 0;
                }
            }
        }
    }

    // Escrever bits restantes se houver
    if (bitCount > 0) {
        fputc(buffer, outFile);
    }

    // Liberar recursos
    fclose(inFile);
    fclose(outFile);
    liberarArvore(root);
    
    for (int i = 0; i < 256; i++) {
        if (huffmanCodes.codigos[i] != NULL) {
            free(huffmanCodes.codigos[i]);
        }
    }

    return 0;
}

int descomprimir(const char* compressedFile, const char* decompressedFile) {
    if (!compressedFile || !decompressedFile) {
        fprintf(stderr, "[ERRO] Caminho de arquivo inválido\n");
        return 1;
    }

    if (strlen(compressedFile) >= MAX_PATH_LEN_HUFFMAN || 
        strlen(decompressedFile) >= MAX_PATH_LEN_HUFFMAN) {
        fprintf(stderr, "[ERRO] Caminho muito longo (máximo %d caracteres)\n", 
                MAX_PATH_LEN_HUFFMAN-1);
        return 1;
    }

    // Abrir arquivo comprimido
    FILE* inFile = fopen(compressedFile, "rb");
    if (!inFile) {
        perror("[ERRO] Falha ao abrir arquivo comprimido");
        return 1;
    }

    // Verificar assinatura
    char magic[8] = {0};
    if (fread(magic, 1, 7, inFile) != 7) {
        fprintf(stderr, "[ERRO] Arquivo corrompido (cabeçalho faltando)\n");
        fclose(inFile);
        return 1;
    }
    magic[7] = '\0';

    if (strncmp(magic, "HUFFv2|", 7) != 0) {
        fprintf(stderr, "[ERRO] Formato inválido (esperado: HUFFv2|)\n");
        fclose(inFile);
        return 1;
    }

    // Ler extensão
    char extOrig[MAX_EXT_LEN] = {0};
    int c, i = 0;
    
    // Pular até EXT=
    while ((c = fgetc(inFile)) != '=' && c != EOF) {
        if (c == EOF) {
            fprintf(stderr, "[ERRO] Cabeçalho EXT= não encontrado\n");
            fclose(inFile);
            return 1;
        }
    }
    
    // Ler extensão
    while ((c = fgetc(inFile)) != '|' && c != EOF && i < MAX_EXT_LEN-1) {
        extOrig[i++] = c;
    }
    extOrig[i] = '\0';

    if (c == EOF) {
        fprintf(stderr, "[ERRO] Cabeçalho incompleto\n");
        fclose(inFile);
        return 1;
    }

    // Pular até TREE=
    while ((c = fgetc(inFile)) != '=' && c != EOF) {
        if (c == EOF) {
            fprintf(stderr, "[ERRO] Cabeçalho TREE= não encontrado\n");
            fclose(inFile);
            return 1;
        }
    }

    // Ler árvore
    No* root = lerArvoreRec(inFile);
    if (!root) {
        fprintf(stderr, "[ERRO] Árvore de Huffman corrompida\n");
        fclose(inFile);
        return 1;
    }

    // Pular delimitador |ENDTREE|
    char endtree[10];
    if (fread(endtree, 1, 9, inFile) != 9 || strncmp(endtree, "|ENDTREE|", 9) != 0) {
        fprintf(stderr, "[ERRO] Delimitador |ENDTREE| não encontrado\n");
        liberarArvore(root);
        fclose(inFile);
        return 1;
    }

    // Ler tamanho original do arquivo
    long originalSize;
    if (fread(&originalSize, sizeof(long), 1, inFile) != 1) {
        fprintf(stderr, "[ERRO] Falha ao ler tamanho original\n");
        liberarArvore(root);
        fclose(inFile);
        return 1;
    }

    // Criar caminho de saída
    char outputPath[MAX_PATH_LEN_HUFFMAN];
    int needed;
    
    if (strlen(extOrig) > 0) {
        needed = snprintf(outputPath, MAX_PATH_LEN_HUFFMAN, "%s.%s", 
                         decompressedFile, extOrig);
    } else {
        needed = snprintf(outputPath, MAX_PATH_LEN_HUFFMAN, "%s", decompressedFile);
    }
    
    if (needed >= MAX_PATH_LEN_HUFFMAN) {
        fprintf(stderr, "[ERRO] Caminho de saída muito longo\n");
        liberarArvore(root);
        fclose(inFile);
        return 1;
    }

    // Abrir arquivo de saída
    FILE* outFile = fopen(outputPath, "wb");
    if (!outFile) {
        perror("[ERRO] Falha ao criar arquivo de saída");
        liberarArvore(root);
        fclose(inFile);
        return 1;
    }

    // Processar dados comprimidos
    No* current = root;
    int byte;
    long bytes_written = 0;
    
    while ((byte = fgetc(inFile)) != EOF && bytes_written < originalSize) {
        for (int i = 7; i >= 0 && bytes_written < originalSize; i--) {
            if (!current) {
                fprintf(stderr, "[ERRO] Nó atual é NULL durante descompressão\n");
                liberarArvore(root);
                fclose(inFile);
                fclose(outFile);
                return 1;
            }
            
            // Navegar na árvore
            current = (byte & (1 << i)) ? current->dir : current->esq;
            
            // Verificar se chegou a uma folha
            if (current && !current->esq && !current->dir) {
                if (fputc(current->data, outFile) == EOF) {
                    perror("[ERRO] Falha ao escrever dados descomprimidos");
                    liberarArvore(root);
                    fclose(inFile);
                    fclose(outFile);
                    return 1;
                }
                bytes_written++;
                current = root;
            }
        }
    }

    // Verificar integridade
    if (current != root) {
        fprintf(stderr, "[AVISO] Dados incompletos (último símbolo não finalizado)\n");
    }

    // Liberar recursos
    fclose(inFile);
    fclose(outFile);
    liberarArvore(root);

    return 0;
}

// Users/aldo/00 - Development/00-GitHub/compactor-descompactador/Projeto-C-Huffman-main/Compressor_Huffman/test

// ./huff