#include "utils.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkArgs(int argc, char *argv[], int *fopt, FILE **fptr) {
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
        *fptr = fopen(argv[2], "r");
        if (fptr == NULL) {
          printf("Could not open file: %s\n", argv[2]);
          return 1;
        }
        *fopt = 1;
      }
    // Unsupported
    } else {
      printf("Unsupported option: %s.\n", argv[1]);
      return 1;
    }
  }
  return 0;
}

void getBody(int *fopt, FILE *fptr, Document *doc) {
  int c; // Parsed input character

  while ((c = (*fopt ? fgetc(fptr) : getchar())) != EOF) {
    appendString(doc->body, c);
  }
}

void getLines(Document *doc) {
  String line;
  initString(&line);

  int go = 1;
  
  for (int i = 0; i < doc->body->len; i++) {
    char c = doc->body->text[i];
    char next = doc->body->text[i + 1];
    char nextnext;
    if (i + 2 < doc->body->len) {
      nextnext = doc->body->text[i + 2];
    } else {
      nextnext = '\0';
    }

    if (c == '\\' && next == CR && nextnext == SP) {
      go = 0;
    } else if (c == CR && next == SP) {
      go = 0;
    }


    if (go) {
      if (c == CR) {
        appendDocumentLine(doc, cloneString(&line));
        freeString(&line);
        initString(&line);
      } else {
        appendString(&line, c);
      }
    } else {
      if (c != '\\' && c != CR && c != SP) {
        appendString(&line, c);
        go = 1;
      }
    }
  }
}

void filterLines(Document *doc, char filtered) {
  for (int i = 0; i < doc->llen; i++) {
    if (doc->lines[i]->text[0] == filtered) {
      freeString(doc->lines[i]);

      for (int j = i; j < doc->llen; j++) {
        doc->lines[j] = doc->lines[j + 1];
      }

      doc->llen--;
      doc->lines[doc->llen] = NULL;
      i--;
    }
  }
}

void formatEntries(Document *doc) {
  for (int i = 0; i < doc->elen; i++) {
    for (int j = 0; j < doc->entries[i]->len; j++) {
      formatLineAsJson(doc->entries[i]->lines[j]);
    }
  }
}

void getEntries(Document *doc) {
  Entry entry;
  initEntry(&entry);
  for (int i = 0; i < doc->llen; i++) {
    if (doc->lines[i]->len == 0) {
      appendDocumentEntry(doc, cloneEntry(&entry));
      freeEntry(&entry);
      initEntry(&entry);
    } else {
      appendEntry(&entry, doc->lines[i]);
    }
  }
  
  if (entry.len > 0) {
    appendDocumentEntry(doc, cloneEntry(&entry));
    freeEntry(&entry);
  }
}

void squashEntries(Document *doc) {
  for (int i = 0; i < doc->elen; i++) {
    if (doc->entries[i]->len == 0) {
      freeEntry(doc->entries[i]);

      for (int j = i; j < doc->elen; j++) {
        doc->entries[j] = doc->entries[j + 1];
      }

      doc->elen--;
      doc->entries[doc->elen] = NULL;
      i--;
    }
  }
}

void printJSON(Document *doc) {
  printf("[\n");
  for (int i = 0; i < doc->elen; i++) {
    printf(IDNT2 "{\n");
    for (int j = 0; j < doc->entries[i]->len; j++) {
      char *closing;
      if (j == doc->entries[i]->len - 1) {
        closing = "\n";
      } else {
        closing = ",\n";
      }
      printf(IDNT4 "\"%s\": \"%s\"%s", doc->entries[i]->lines[j]->key, doc->entries[i]->lines[j]->val, closing );
    }
    char *closing;
    if (i == doc->elen - 1) {
      closing = "}\n";
    } else {
      closing = "},\n";
    }
    printf(IDNT2 "%s", closing);
  }
  printf("]\n");
}

int main(int argc, char *argv[]) {
  int fopt = 0; // File option
  FILE *fptr = NULL; // Optional file
  Document doc;
  initDocument(&doc);

  // Check for command options
  if (checkArgs(argc, argv, &fopt, &fptr)) {
    return 0;
  }

  getBody(&fopt, fptr, &doc); // Gather input data by lines
  getLines(&doc); // Gather input data by lines
  filterLines(&doc, '#'); // Remove all comment lines that start with # character
  getEntries(&doc); // Gather lines separated by NLs into entries
  squashEntries(&doc); // Remove all empty entries
  formatEntries(&doc); // Format each line into valid JSON
  printJSON(&doc); // Output formatted JSON
  return 0;
}