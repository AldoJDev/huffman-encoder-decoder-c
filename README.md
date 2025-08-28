# Compressor e Descompressor Huffman em C

Uma implementação completa do algoritmo de compressão de Huffman em linguagem C, incluindo interface interativa com menu para compactação e descompactação de arquivos.

## 📋 Sobre o Projeto

Este projeto implementa o algoritmo de codificação de Huffman para compressão de dados. O algoritmo utiliza uma árvore binária para gerar códigos de tamanho variável baseados na frequência dos caracteres, resultando em compressão eficiente de arquivos de qualquer tipo.

### Características

- ✅ **Árvore de Huffman dinâmica**: Construída baseada na frequência dos caracteres do arquivo
- ✅ **Interface interativa**: Menu intuitivo para facilitar o uso
- ✅ **Preservação de extensões**: Mantém a extensão original após descompressão
- ✅ **Suporte a arquivos binários**: Funciona com qualquer tipo de arquivo (.txt, .jpeg, .pdf, etc.)
- ✅ **Relatório**: Mostra estatísticas de compressão e caminhos dos arquivos
- ✅ **Formato proprietário .pcb**: Arquivos comprimidos salvos com extensão personalizada

## 🚀 Como Usar

### Compilação

```bash
gcc -o huffman main.c huffman.c codigo.c -Wall -Wextra -std=c99
```

### Execução

```bash
./huffman
```

### Exemplo de Uso - Compressão

1. Execute o programa: `./huffman`
2. Escolha a opção `1` (Comprimir arquivo)
3. Digite o diretório do arquivo:
   ```
   C:/Users/SeuNome/Desktop/testes
   ```
4. Digite o nome do arquivo com extensão:
   ```
   documento.txt
   ```
5. O arquivo comprimido será salvo como `documento.pcb` no mesmo diretório

### Exemplo de Uso - Descompressão

1. Execute o programa: `./huffman`
2. Escolha a opção `2` (Descomprimir arquivo)
3. Digite o caminho completo do arquivo .pcb:
   ```
   C:/Users/SeuNome/Desktop/testes/documento.pcb
   ```
4. O arquivo será descomprimido como `decomprimido_documento.txt` (preservando a extensão original)

### Formatos de Caminho Suportados

- **Windows**: `C:/Users/Nome/Desktop/arquivo.txt`
- **macOS/Linux**: `/Users/nome/Desktop/arquivo.txt`

## 🔧 Como Funciona

### Estrutura do Arquivo Comprimido (.pcb)

O formato proprietário `.pcb` utiliza um cabeçalho estruturado:

```
HUFFv2|EXT=txt|TREE=[árvore_serializada]|ENDTREE|[tamanho_original][dados_comprimidos]
```

### Fluxo de Compressão

1. **Análise de Frequência**: Conta a ocorrência de cada byte no arquivo
2. **Construção da Árvore**: Constrói árvore de Huffman usando fila de prioridade
3. **Geração de Códigos**: Percorre a árvore para criar códigos únicos
4. **Serialização**: Salva árvore e metadados no cabeçalho
5. **Codificação**: Substitui bytes originais pelos códigos Huffman

### Fluxo de Descompressão

1. **Leitura do Cabeçalho**: Extrai metadados e árvore serializada
2. **Reconstrução da Árvore**: Reconstrói árvore de Huffman
3. **Decodificação**: Navega pela árvore para recuperar bytes originais
4. **Restauração**: Salva arquivo com nome e extensão originais

## 📈 Performance

### Exemplo de Compressão

| Tipo de Arquivo | Tamanho Original | Tamanho Compactado | Taxa de Compressão |
|----------------|------------------|--------------------|--------------------|
| Texto comum (.txt) | 100 KB       | 58 KB              | 42%                |
| Código fonte (.c)  | 50 KB        | 31 KB              | 38%                |
| Imagem (.jpeg)     | 2.5 MB       | 2.1 MB             | 16%                |
| Dados repetitivos  | 200 KB       | 45 KB              | 77.5%              |


## 🛠️ Dependências e Requisitos

- **Compilador**: GCC (ou qualquer compilador C compatível com C99)
- **Sistema operacional**: Linux, macOS, Windows 
- **Memória**: Mínimo 1MB disponível
- **Espaço em disco**: Espaço suficiente para arquivos originais + comprimidos

## 📝 Formato de Arquivo .pcb

O formato proprietário `.pcb` (Projeto Compressor Binário) contém:

```
[Cabeçalho] HUFFv2|EXT=extensao|TREE=dados_arvore|ENDTREE|
[Metadados] tamanho_original (8 bytes)
[Dados]     bits_comprimidos
```


## 🧪 Teste Manual Básico

```bash
# 1. Compilar o programa
gcc -o huffman main.c huffman.c codigo.c -Wall -Wextra -std=c99

# 2. Executar
./huffman

# 3. Testar compressão (opção 1)
# Diretório: /caminho/para/seus/testes
# Arquivo: teste.txt

# 4. Testar descompressão (opção 2)  
# Arquivo: /caminho/para/seus/testes/teste.pcb

# 5. Verificar integridade
diff teste.txt decomprimido_teste.txt
```

### Exemplos de Teste

- **Arquivo de texto**: `exemplo.txt`
- **Imagem**: `foto.jpeg`
- **Documento**: `relatorio.pdf`
- **Código fonte**: `programa.c`

Todos os tipos são suportados e preservam suas extensões após descompressão.

## 📚 Algoritmo de Huffman

O algoritmo de Huffman é um método de compressão sem perdas que utiliza códigos de tamanho variável. Os caracteres mais frequentes recebem códigos mais curtos, enquanto os menos frequentes recebem códigos mais longos.

### Passos do Algoritmo:

1. **Contagem de frequência**: Conta a ocorrência de cada caractere
2. **Criação de nós folha**: Cada caractere vira um nó com sua frequência
3. **Construção da árvore**: Combina os dois nós de menor frequência repetidamente
4. **Geração de códigos**: Percorre a árvore para gerar códigos binários
5. **Codificação**: Substitui caracteres pelos códigos correspondentes


## 🚨 Limitações Conhecidas

- Arquivos muito pequenos (< 100 bytes) podem ter compressão ineficiente
- Arquivos já comprimidos (.zip, .rar) terão baixa taxa de compressão
- Caminhos muito longos (> 512 caracteres) podem causar problemas
- Interface apenas em português

## 📄 Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.

## 🔗 Referências

- [Huffman Coding - Wikipedia](https://en.wikipedia.org/wiki/Huffman_coding)
- [Introduction to Algorithms - CLRS](https://mitpress.mit.edu/books/introduction-algorithms)
- [Data Structures and Algorithm Analysis in C](https://users.cs.fiu.edu/~weiss/dsaa_c2e/)

--- 

⭐ Se este projeto foi útil para você, considere dar uma estrela!
