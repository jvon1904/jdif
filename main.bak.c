#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define INITIAL_CAP 32

typedef struct {
  size_t len;
  size_t cap;
  char *data;
} String;

void appendString(String *str, char c) {
  if (str->cap <= str->len + 1) {
    str->cap *= 2;
    str->data = realloc(str->data, str->cap);
  }
  str->data[str->len++] = c;
  str->data[str->len] = '\0';
}

void initString(String *str) {
  str->data = malloc(INITIAL_CAP);
  str->data[0] = '\0';
  str->len = 0;
  str->cap = INITIAL_CAP;
}

void freeString(String *str) {
  free(str->data);
}

void appendValue(String *json, String *val) {
  int is_int = 1;
  for (int i = 0; i < strlen(val->data); i++) {
    if (!(val->data[i] == '.' || 
        val->data[i] == '0' ||
        val->data[i] == '1' ||
        val->data[i] == '2' ||
        val->data[i] == '3' ||
        val->data[i] == '4' ||
        val->data[i] == '5' ||
        val->data[i] == '6' ||
        val->data[i] == '7' ||
        val->data[i] == '8' ||
        val->data[i] == '9')) {
          is_int = 0; 
        }
  }

  if (!is_int) {
    appendString(json, '"');
  }

  for (int i = 0; i < strlen(val->data); i++) {
    appendString(json, val->data[i]);
  }

  if (!is_int) {
    appendString(json, '"');
  }
}

int main(int argc, char *argv[]) {
  // Initialize variables
  FILE *fptr;
  int c;
  String json;
  String val;
  initString(&json);
  initString(&val);

  // Flags
  int fopt = 0;   // file option
  int nval = 0;   // new value started
  int skip = 0;   // skip next character
  int eof  = 0;   // end of file
  int nwln = 0;   // newline
  int cmt  = 0;   // comment started
  int cmnd = 0;   // comment ended

  // Check for command options
  if (argc > 1) {
    // --help
    if (!strcmp(argv[1], "--help")) {
      printf("J-DIF -- Convert LDIF (LDAP Data Interchange Format) to JSON.\n\n");
      printf("-f <file>\tSpecify a a file to parse.\n");
      printf("--stdin\t\tParse from standard input.\n");
      return 0;
    // --file
    } else if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "--file")) {
      if (argc < 3) {
        printf("Please specify a file path with `jdif -f /path/to/file`\n");
        return 1;
      } else {
        fptr = fopen(argv[2], "r");
        if (fptr == NULL) {
          printf("Could not open file: %s\n", argv[2]);
          return 1;
        }
        fopt = 1;
      }
    // Unsupported
    } else {
      printf("Unsupported option: %s.\n", argv[1]);
      return 1;
    }
  }

  if (!fopt && (fseek(stdin, 0, SEEK_END) == 0)) {
    return 0;
  }

  // Add the first two characters
  appendString(&json, '{');
  appendString(&json, '"');

  // Stream characters from STDIN or file
  while ((c = fopt ? fgetc(fptr) : getchar()) && !eof) {
    if (c == '#') {
      if (!nval) {
        cmt = 1;
      }
    }
    if (c == ':') {
      if (!cmt) {
        if (nval) {
          appendString(&val, c);
        } else  {
          appendString(&json, '"');
          appendString(&json, ':');
          appendString(&json, ' ');
          nval = 1;
          skip = 1;
        }
      }
    } else if (c == ' ') {
      if (!cmt) {
        if (!skip) {
          if (nval) {
            appendString(&val, ' ');
          } else {
            appendString(&json, ' ');
          }
        }
      }
    } else if (c == '\n') { // refresh everything
      if (!cmt && !cmnd) {
        if(nval) {
          appendValue(&json, &val);
          freeString(&val);
          initString(&val);
          nval = 0;
          skip = 0;
          nwln = 1;
        }
      } else {
        cmt = 0;
        cmnd = 1;
      }
    } else if (c == EOF) {
      appendValue(&json, &val);
      appendString(&json, '}');
      appendString(&json, '\n');
      eof = 1;
    } else {
      if (!cmt) {
        if (nwln) {
          appendString(&json, ',');
          appendString(&json, '"');
        }
        if (nval) {
          appendString(&val, c);
        } else {
          appendString(&json, c);
        }
        skip = 0;
        nwln = 0;
        cmnd = 0;
      }
    }
  }
  printf("%s", json.data);
  freeString(&json);
  freeString(&val);
}
