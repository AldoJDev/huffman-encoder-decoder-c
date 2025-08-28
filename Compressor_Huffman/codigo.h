#ifndef CODIGO
#define CODIGO
#include "tiposonlinegdb.h"

typedef struct
{
    U8* byte; 
    U8  capacidade; 
    U8  tamanho; 
} Codigo;

bool novo_codigo(Codigo* c); 
void free_codigo(Codigo* c); 

bool adiciona_bit(Codigo* c, 
                  U8 valor); 
bool joga_fora_bit(Codigo* c); 

bool clone(Codigo original, 
           Codigo* copia); 

#endif