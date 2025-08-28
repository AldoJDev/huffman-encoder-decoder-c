#include "codigo.h"
#include <stdlib.h>  //para malloc, free e NULL

bool novo_codigo(Codigo* c) // por referência
{
    c->byte = (U8*)malloc(1 * sizeof(U8));
    if (c->byte == NULL) return false;

    c->capacidade = 8;
    c->tamanho = 0;
    return true;
}

void free_codigo(Codigo* c) // por referência
{
    if (c->byte != NULL) free(c->byte);
    c->byte = NULL;
}

bool adiciona_bit(Codigo* c, U8 valor)
{
    if (c->tamanho == c->capacidade)
    {
        int novos_bytes = (c->capacidade + 8) / 8;
        U8* novo = (U8*)malloc(novos_bytes);
        if (novo == NULL) return false;

        for (int i = 0; i < c->capacidade / 8; i++)
            novo[i] = c->byte[i];

        free(c->byte);
        c->byte = novo;
        c->capacidade += 8;
    }

    int byte_atual = c->tamanho / 8;
    int bit_no_byte = 7 - (c->tamanho % 8);  // para armazenar da esquerda para direita

    if (bit_no_byte == 7) c->byte[byte_atual] = 0;  // zera o byte ao iniciar

    if (valor == 1)
        c->byte[byte_atual] |= (1 << bit_no_byte);

    c->tamanho++;
    return true;
}

bool joga_fora_bit(Codigo* c)
{
    if (c->tamanho == 0) return false;

    int byte_atual = (c->tamanho - 1) / 8;
    int bit_no_byte = 7 - ((c->tamanho - 1) % 8);
    c->byte[byte_atual] &= ~(1 << bit_no_byte); // limpa o bit

    c->tamanho--;

    if (c->capacidade > 8 && c->capacidade - c->tamanho == 8)
    {
        int novos_bytes = (c->capacidade - 8) / 8;
        U8* novo = (U8*)malloc(novos_bytes);
        if (novo == NULL) return false;

        for (int i = 0; i < novos_bytes; i++)
            novo[i] = c->byte[i];

        free(c->byte);
        c->byte = novo;

        c->capacidade -= 8;
    }

    return true;
}

bool clone(Codigo original,   // por valor
           Codigo* copia)     // por referência
{
    int bytes_needed = (original.capacidade + 7) / 8;  // Arredonda para cima

    copia->byte = (U8*)malloc(bytes_needed * sizeof(U8));
    if (copia->byte == NULL) return false;

    for (int i = 0; i < bytes_needed; i++)
        copia->byte[i] = original.byte[i];

    copia->capacidade = original.capacidade;
    copia->tamanho = original.tamanho;
    return true;
}