#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXNAME 256

#pragma pack (push,1)
struct file_header {
 	char name [MAXNAME] ; // Nome 
 	unsigned int filesize ; // Tamanho 
};
#pragma pack (pop)


int main(int argc, char **argv) {
    // Verifique se temos os argumentos mínimos necessários
    if (argc < 2) {
        printf("Uso:\n");
        printf("\t%s -c <arquivo_de_saida> <arquivo1> <arquivo2> ... <arquivoN> - Cria arquivo.\n", argv[0]);
        printf("\t%s -l - Lista arquivos.\n", argv[0]);
        printf("\t%s -e <arquivo saida> <arquivo pesquisado> - Extrai conteúdo.\n", argv[0]);
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
        char *output_file = argv[2];
        char **input_files = &argv[3];
        int num_input_files = argc - 3;
        
        FILE *out_fp = fopen(output_file, "wb");

        printf("Arquivo de saída: %s\n", output_file);
        for(int i=0; i < num_input_files; i++) {
            printf("Arquivo de entrada %d: %s\n", i, input_files[i]);
            
            FILE *in_fp = fopen(input_files[i], "rb");
            fseek(in_fp, 0, SEEK_END);//Bloco de código para pegar o tamanho da função
            unsigned int file_size = ftell(in_fp);
	    fseek(in_fp, 0, SEEK_SET);
	    
	    struct file_header header;//Criando o cabeçalho
            strncpy(header.name, input_files[i], MAXNAME);
            header.filesize = file_size;
            
            fwrite(&header, sizeof(struct file_header), 1, out_fp);

            char *buffer = (char *)malloc(file_size);
            fread(buffer, 1, file_size, in_fp);
            fwrite(buffer, 1, file_size, out_fp);

            free(buffer);
            fclose(in_fp);
        }
        fclose(out_fp);
    }

    // Extrair arquivo. 
    if (strcmp(opt, "-e") == 0){
    
        char *extract_file = argv[2];//./saida/saida1.txt
        char *file_name = argv[3];//teste1.txt --> ./entrada/teste1.txt
        
        
        printf("Arquivo para extrair: %s\n", extract_file);
        
        
        FILE *archive_fp = fopen(extract_file, "rb");
        //int ptr1 = fseek(archive_fp, 0, SEEK_END);
        
        struct file_header cabecalho;
        
        //fread(&cabecalho, sizeof(struct file_header), 1, archive_fp)
    	while (fread(&cabecalho, sizeof(struct file_header), 1, archive_fp) == 1) {
    		//printf("%s\n",file_name);
    		//printf("%s\n",cabecalho.name);
    		
        	if (strcmp(cabecalho.name, file_name) == 0) {
            		char *buffer = (char *)malloc(cabecalho.filesize);
            		fread(buffer, 1, cabecalho.filesize, archive_fp);
            		
            		//printf("%s\n", buffer);

            		//FILE *out_fp = fopen(file_name, "wb");//./entrada/teste1.txt
            		//if (!out_fp) {
                	//	perror("Erro ao criar arquivo de saída");
                	//	free(buffer);
                	//	fclose(archive_fp);
            		//}
			
			FILE *lido = fopen("./lidos/lido.txt","wb");
			//fprintf(lido, buffer);
			if (!lido) {
                		perror("Erro ao criar arquivo de saída");
                		free(buffer);
                		fclose(archive_fp);
            		}
			
            		//fwrite(buffer, 1, cabecalho.filesize, out_fp);
			fwrite(buffer, 1, cabecalho.filesize, lido);
            		free(buffer);
            		//fclose(out_fp);//Salvar na pasta de lidos
            		fclose(lido);
            		printf("Arquivo %s extraído com sucesso.\n", file_name);
            		fclose(archive_fp);
            		return 0;
       	 	}else{
       	 		fseek(archive_fp, cabecalho.filesize, SEEK_CUR);
       	     		//int ptr2 = fseek(archive_fp, cabecalho.filesize, SEEK_CUR);
       	     		////printf("%d",*ptr1);
       	     		//printf("%d",*ptr2);
       	     		//if (*ptr2 >= *ptr1){
       	     			
       	     		//	return 1;
       	     		//}
       	     		//precisamos verificar onde esta o ponteiro
       	     		//se ftell indicar final
        	}
        	
    	}
    	printf("Erro, arquivo não encontrado");
        
    }

    return 0;
}
