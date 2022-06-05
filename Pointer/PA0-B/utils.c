#include "dir_file.h"
#include "utils.h"

int open_file(char* fname, FILE** input) {
    if (access(fname, F_OK) == -1) {
        ERR_PRINT("The '%s' file does not exists\n", fname);
        return -1;
    }

    *input = fopen(fname, "r");
    if (input == NULL) {
        ERR_PRINT("Failed open '%s' file\n", fname);
        return -1;
    }

    return 1;
}

// This parse_str_to_list() split string to the tokens, and put the tokens in token_list.
// The value of return is the number of tokens.
int parse_str_to_list(const char* str, char** token_list) {
    /* Fill this function */
	int i = 0, j;
	char *temp;
	char copy[MAX_BUFFER_SIZE];
	char **temp_list;
	
	temp_list = (char**)malloc(sizeof(char*)*MAX_TOKEN_NUM);

	strcpy(copy,str);	
	
	temp = strtok(copy, "\n");
	while(temp){
		temp_list[i] = (char*)malloc(sizeof(char)*MAX_BUFFER_SIZE);
		strcpy(temp_list[i++], temp);
		temp = strtok(NULL, "\n");
	}
	temp_list[i] = NULL;
	
	i = 0;
	for(j=0;temp_list[j]!=0;j++){
	
		temp = strtok(temp_list[j],"/");
		while(temp){
			token_list[i] = (char*)malloc(sizeof(char)*MAX_BUFFER_SIZE);
			strcpy(token_list[i++],temp);
			temp = strtok(NULL,"/");
		}
	}
	return i;
}

void free_token_list(char** token_list, int num_token) {
    int index;

    for (index = 0; index < num_token; index++) {
        free(token_list[index]);
    }

    free(token_list);
}
