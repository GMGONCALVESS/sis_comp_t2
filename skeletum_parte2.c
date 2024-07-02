#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    // Verifique se temos os argumentos mínimos necessários
    if (argc < 2) {
        printf("Uso:\n");
        printf("\t%s -c <arquivo_de_saida> <arquivo1> <arquivo2> ... <arquivoN> - Cria arquivo.\n", argv[0]);
        printf("\t%s -l - Lista arquivos.\n", argv[0]);
        printf("\t%s -e <arquivo> - Extrai conteúdo.\n", argv[0]);
        return 1;
    }

    // O primeiro argumento é o arquivo de saída ou opção de listagem -l
    char *opt = argv[1];

    if (strcmp(opt, "-l") == 0) {
        // Listar todos os arquivos e tamanhos no arquivamento
        return 0;
    }

    // Criar arquivo. 
    if (strcmp(opt, "-c") == 0){
        char *output_file = argv[1];
        char **input_files = &argv[2];
        int num_input_files = argc - 3;

        printf("Arquivo de saída: %s\n", output_file);
        for(int i=0; i < num_input_files; i++) {
            printf("Arquivo de entrada %d: %s\n", i, input_files[i]);
        }
    }

    // Extrair arquivo. 
    if (strcmp(opt, "-e") == 0){
        char *extract_file = argv[2];
        printf("Arquivo para extrair: %s\n", extract_file);
    }

    return 0;
}
