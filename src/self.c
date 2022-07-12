#include <alloca.h>
#include <stdio.h>
#include <string.h>

#include <parser.h>

int main(int argc, char **argv) {			
	char *line = alloca(LINE_SIZE_MAX); 	
	line[0] = '\0';

	while (strncmp(line, "QUIT", LINE_SIZE_MAX) != 0) {
		char *rc = fgets(line, LINE_SIZE_MAX, stdin);
		if (rc == NULL) {
			// TODO Improve error handling
			printf("Error reading from stdin\n");
			continue;
		}

		// DEBUG code
		printf("line is: %s\n", line);
		
		parse_chunk(line);
	}
	
	return 0;
}
