#include "utils.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void formatLineAsJson(String *str) {
  String key;
  String val;
  initString(&key);
  initString(&val);

  int k = 1;
  for (int i = 0; i < str->len; i++) {
    int coln = 0;
    int end = 0;
    char c = str->text[i];
    char next = str->text[i + 1];

    // Terminate key if sequence ": " is encountered
    if (k) {
      if (c == COLN) {
        k = 0;
        if (next == SP) {
          i++;
        }
      } else {
        appendString(&key, c);
      } 
    } else  {
      appendString(&val, c);
    }   
  }

  assignKey(str, &key);
  assignVal(str, &val);
  freeString(&key);
  freeString(&val);
}

