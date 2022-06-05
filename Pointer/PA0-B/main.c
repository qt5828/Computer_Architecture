#include "utils.h"
#include "dir_file.h"

int main(int argc, char **argv) {
    char buff[MAX_BUFFER_SIZE];
    char *dir_fname, *file_fname;
    directory_t *dir, *root_dir = NULL;
    FILE *file_input = NULL, *dir_input = NULL;
    int ret;

    if (argc != 3) {
        ERR_PRINT("Usage: main <file_input file> <dir_input file>");
        exit(1);
    }

    file_fname = argv[1];
    dir_fname = argv[2];

    ret = open_file(file_fname, &file_input);
    if(ret == -1) {
        ERR_PRINT("Failed open_file() for '%s' file", file_fname);
        goto exit;
    }

    ret = open_file(dir_fname, &dir_input);
    if (ret == -1) {
        ERR_PRINT("Failed open_file() for '%s' file", dir_fname);
        goto exit;
    }

    root_dir = create_dir("root");

    // Parse the string written in file_input and create a hierarchical structure of directories and files.
    while (fgets(buff, MAX_BUFFER_SIZE, file_input) != NULL) {
        char **token_list = NULL;
        int num_token = 0;

        token_list = (char**)malloc(sizeof(char*)*MAX_TOKEN_NUM);

        num_token = parse_str_to_list(buff, token_list);

        make_dir_and_file(root_dir, token_list, num_token);

        free_token_list(token_list, num_token);
    }

    // Find the location of directory written in dir_input and print the file in it.
    while (fgets(buff, MAX_BUFFER_SIZE, dir_input) != NULL) {
        char **token_list;
        int num_token = 0;

        dir = NULL;

        token_list = (char**)malloc(sizeof(char*)*MAX_TOKEN_NUM);

        num_token = parse_str_to_list(buff, token_list);

        dir = find_target_dir(root_dir, token_list, num_token);
        if (dir == NULL) {
            free_token_list(token_list, num_token);
            continue;
        }

        print_file_on_dir(dir);

        free_token_list(token_list, num_token);
    }

exit:
    if (file_input != NULL)
        fclose(file_input);

    if (dir_input != NULL)
        fclose(dir_input);

    if (root_dir != NULL)
        free_dir_and_file(root_dir);

    return 0;
}
