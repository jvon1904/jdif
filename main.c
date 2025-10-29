#include "utils.h"
#include "json.h"
#include <stdio.h>
#include <unistd.h>
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
  Line line;
  initLine(&line);

  int nl = 0; // Newline flag
  
  for (int i = 0; i < doc->body->len; i++) {
    char c = doc->body->text[i];
    char next = doc->body->text[i + 1];

    if (c == CR && next == SP) {
      nl = 1;
    }
    
    if (nl && (c != SP && c != CR)) {
      nl = 0;
    }

    if (!nl) {
      if (c == CR) {
        appendDocumentLine(doc, cloneLine(&line));
        freeLine(&line);
        initLine(&line);
      } else {
        appendLine(&line, c);
      }
    }
  }

  if (line.len > 0) {
    appendDocumentLine(doc, cloneLine(&line));
    freeLine(&line);
  } else {
    freeLine(&line);
  }
}

void filterLines(Document *doc, char filtered) {
  // Filter in place
  // initialize write pointer
  size_t j = 0;

  for (size_t i = 0; i < doc->llen; i++) {
    Line *line = doc->lines[i];
    if (line->text[0] == filtered) {
      freeLine(line);
      free(line);
    } else {
      doc->lines[j++] = line;
    }
  }

  // Clean up tail
  for (size_t i = j; i < doc->llen; i++) {
    doc->lines[i] = NULL;
  }

  doc->llen = j;
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
      formatLineAsJson(doc->lines[i]);
      appendEntry(&entry, doc->lines[i]);
    }
  }
  
  if (entry.len > 0) {
    appendDocumentEntry(doc, cloneEntry(&entry));
    freeEntry(&entry);
  }
}

void squashEntries(Document *doc) {
  // Filter in place
  // initialize write pointer
  size_t j = 0;
  for (size_t i = 0; i < doc->elen; i++) {
    Entry *entry = doc->entries[i];
    if (entry->len == 0) {
      freeEntry(entry);
      free(entry);
    } else {
      doc->entries[j++] = entry;
    }
  }

  // Clean up tail
  for (size_t i = j; i < doc->elen; i++) {
    doc->entries[i] = NULL;
  }

  doc->elen = j;
}

void consolidateEntryLineVals(Document *doc) {
  // Iterate over each entry
  for (int i = 0; i < doc->elen; i++) {

    Entry *ent = doc->entries[i];

    // Iterate over each line in entry
    for (int j = 0; j < ent->len; j++) {

      // Consolidate identical values
      //
      // Iterate over each subsequent key
      for (int k = j + 1; k < ent->len; k++) {
        if (ent->lines[k]) {
          // Compare keys
          //
          // If match, append value and remove duplicate line
          if (!strcmp(ent->lines[k]->key->text, ent->lines[j]->key->text)) {
            if (ent->lines[k]->vlen > 0) {
              appendVal(ent->lines[j], ent->lines[k]->vals[0]);
            }

            // Remove duplicate line
            freeLine(ent->lines[k]);
            free(ent->lines[k]);

            // Decrement length
            ent->len--;

            // Shift remaining lines left
            for (int x = k; x < ent->len; x++) {
              ent->lines[x] = ent->lines[x + 1];
            }

            k--;
          }
        } else {
          continue;
        }
      }
    }
  }
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

  if (!fopt && isatty(STDIN_FILENO)) {
      printf("No input provided.\n");
      return 0;
  }

  getBody(&fopt, fptr, &doc); // Gather input data by lines
  getLines(&doc); // Gather input data by lines
  filterLines(&doc, '#'); // Remove all comment lines that start with # character
  getEntries(&doc); // Gather lines separated by NLs into entries
  squashEntries(&doc); // Remove all empty entries
  consolidateEntryLineVals(&doc); // Consolidate identical values in entry lines
  printJSON(&doc); // Output formatted JSON
  return 0;
}