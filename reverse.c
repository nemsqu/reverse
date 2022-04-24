#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct line
{
	char *string;
	struct line *pPrev;
	struct line *pNext;
};

typedef struct line LINE;

LINE *read_file(LINE *pline, FILE *pfile){
    
    LINE *ptr;
    char *buffer = NULL;
    size_t len = 0;
    size_t read;
    
    while((read = getline(&buffer, &len, pfile)) != -1){
        // exit input mode if user inserts an empty line
        if((pfile == stdin) && strcmp(buffer, "\n") == 0){
            break;
        }
        if(pline == NULL){
            if ((pline = ptr = malloc(sizeof(struct line))) == NULL) {
				fprintf(stderr, "Malloc failed.\n");
				exit(1);
			}
			ptr->pPrev = ptr->pNext = NULL;
        } else {
			if ((ptr->pNext = malloc(sizeof(struct line))) == NULL) {
				fprintf(stderr, "Malloc failed.\n");
				exit(1);
			}

            // add line to the end of the list, link both ways
			if(pline->pNext == NULL){
				pline->pNext = ptr->pNext;
				ptr = ptr->pNext;
				ptr->pPrev = pline;
			} else {
				ptr->pNext->pPrev = ptr;
				ptr->pNext->pNext = NULL;
				ptr=ptr->pNext;
			}

		}

        // add line
        if ((ptr->string = malloc(strlen(buffer) + 1)) == NULL) {
			fprintf(stderr, "Malloc failed.\n");
			exit(1);
		}
		strcpy(ptr->string, buffer);
    }
    if(buffer != NULL){
        free(buffer);
    }
    return pline;
}

void reverse_print(LINE *pline, FILE *pfile){
    LINE *ptr;

    ptr = pline;
    
    // find last item
	while(ptr->pNext != NULL){
		ptr = ptr->pNext;
	}

    // iterate and print in reverse order
	while(ptr != NULL){
		fprintf(pfile, "%s", ptr->string);
		ptr = ptr->pPrev;
	}
}

void empty_list(LINE *pline){
    LINE *ptr;

    while(pline != NULL){
        ptr = pline->pNext;
        free(pline);
        pline = ptr;
    }
}

int main(int argc, char *argv[]){
    FILE *pfile;
    LINE *pline = NULL;

    // use stdin and stdout
    if(argc == 1){
        pline = read_file(pline, stdin);
    }
    // use file and stdout
    else if(argc == 2){
        if((pfile = fopen(argv[1], "r")) == NULL){
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        pline = read_file(pline, pfile);
        fclose(pfile);
    }
    // use files
    else if(argc == 3){
        if(strcmp(argv[1], argv[2]) == 0){
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        if((pfile = fopen(argv[1], "r")) == NULL){
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        pline = read_file(pline, pfile);
        fclose(pfile);
    }
    // too many arguments
    else{
        fprintf(stdout, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // end program if there was no input
    if(pline == NULL){
        fprintf(stdout, "Nothing to reverse.\n");
        return 0;
    }

    // no output file --> print to stdout
    if(argc != 3){
        reverse_print(pline, stdout);
    }
    else {
        if((pfile = fopen(argv[2], "w")) == NULL){
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
        reverse_print(pline, pfile);
        fclose(pfile);
        fprintf(stdout, "Reversed text written to the output file.\n");
    }

    empty_list(pline);

    return 0;
}
