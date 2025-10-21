#include "utils.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void formatLineAsJson(Line *lin) {
  // Return early if already formatted
  if (lin->vlen > 0) return;

  // Initialize key and val strings
  String key;
  String val;
  initString(&key);
  initString(&val);

  // Set flags
  int k = 1; // parsing key, initialize as true
  int colon = 0; // colon encountered

  // Iterate over each char of the line text
  for (int i = 0; i < lin->len; i++) {
    char c = lin->text[i];
    if (k) {
      // Handle colons
      if (c == COLN) {
        // First colon
        if (!colon) {
          colon = 1;

          // double colon means base64 (::)
          if (i + 1 < lin->len && lin->text[i + 1] == COLN) {
            i++;
          }

          // optional space after colon(s)
          if (i + 1 < lin->len && lin->text[i + 1] == SP) {
            i++;
          }

          k = 0; // switch to value
        } else {
          appendString(&key, c); // stray colon inside key (unlikely)
        }
      } else {
        appendString(&key, c);
      }
    } else {
      appendString(&val, c);
    }
  }

  // Assign key and value to line
  assignKey(lin, &key);
  appendVal(lin, &val);

  // Cleanup temporary strings
  freeString(&key);
  freeString(&val);
}

void printJSON(Document *doc) {
  // Output JSON array start
  printf("[\n");

  // Iterate over each entry in document
  for (int i = 0; i < doc->elen; i++) {
    printf(IDNT2);
    printf("{\n");

    // Iterate over each line in entry
    for (int j = 0; j < doc->entries[i]->len; j++) {
      char *closing;
      if (j == doc->entries[i]->len - 1) {
        closing = "\n";
      } else {
        closing = ",\n";
      }
      printf(IDNT4);
      printf("\"%s\": ", doc->entries[i]->lines[j]->key->text);

      // Check if line has multiple values
      if (doc->entries[i]->lines[j]->vlen > 1) {
        printf("[\n");
        // Iterate over each value in line
        for (int k = 0; k < doc->entries[i]->lines[j]->vlen; k++) {
          char *closing;
          if (k == doc->entries[i]->lines[j]->vlen - 1) {
            closing = "\n";
          } else {
            closing = ",\n";
          }
          printf(IDNT6);
          printf("\"%s\"%s", doc->entries[i]->lines[j]->vals[k]->text, closing);
        }
        printf(IDNT4);
        char *closing;
        if (j == doc->entries[i]->len - 1) {
          closing = "]\n";
        } else {
          closing = "],\n";
        }
        printf("%s", closing);
      } else {
        printf("\"%s\"%s", doc->entries[i]->lines[j]->vals[0]->text, closing);
      }
    }

    char *closing;
    if (i == doc->elen - 1) {
      closing = "}\n";
    } else {
      closing = "},\n";
    }

    printf(IDNT2);
    printf("%s", closing);
  }
  printf("]\n");
}
