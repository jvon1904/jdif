#include "utils.h"
#include <stdlib.h>
#include <string.h>

// Misc
char *strip(char *str) {
  int istart = 0;
  int iend = strlen(str);
  
  for (int i = istart; i < strlen(str); i++) {
    if (str[i] != SP) {
      istart = i;
      break;
    }
  }

  for (int i = iend; i > istart; i--) {
    if (str[i - 1] != SP) {
      iend = i;
      break;
    }
  }

  int bufferLength = iend - istart;
  char *buffer = malloc(bufferLength + 1);
  strncpy(buffer, str + istart, bufferLength);
  buffer[bufferLength] = '\0';
  return buffer;
}

// String
void initString(String *str) {
  str->text = malloc(BUFFER);
  str->text[0] = '\0';
  str->len = 0;
  str->cap = BUFFER;
}

void prependString(String *str, char c) {
  if (str->len + 1 >= str->cap) {
    str->cap *= 2;
    str->text = realloc(str->text, str->cap);
  }
  memmove(str->text + 1, str->text, str->len + 1);
  str->text[0] = c;
  str->len++;
}

void appendString(String *str, char c) {
  if (str->len + 1 >= str->cap) {
    str->cap *= 2;
    str->text = realloc(str->text, str->cap);
  }
  str->text[str->len++] = c;
  str->text[str->len] = '\0';
}

void assignString(String *str, const char *text) {
  size_t len = strlen(text);
  if (len + 1 >= str->cap) {
    str->cap = strlen(text) + 1;
    str->text = realloc(str->text, str->cap);
  }
  strcpy(str->text, text);
  str->len = len;
}

void freeString(String *str) {
  free(str->text);
}

String *cloneString(String *str) {
  String *clone = malloc(sizeof(String));
  clone->len = str->len;
  clone->cap = str->cap;
  clone->text = malloc(str->cap);
  memcpy(clone->text, str->text, str->len);
  clone->text[str->len] = '\0';
  return clone;
}

// Line
void initLine(Line *lin) {
  lin->text = malloc(BUFFER);
  lin->text[0] = '\0';
  lin->len = 0;
  lin->cap = BUFFER;
  lin->key = malloc(sizeof(String));
  initString(lin->key);
  lin->vals = malloc(sizeof(String *) * BUFFER);
  lin->vlen = 0;
  lin->vcap = BUFFER;
}

void appendLine(Line *lin, char c) {
  if (lin->len + 1 >= lin->cap) {
    lin->cap *= 2;
    lin->text = realloc(lin->text, lin->cap);
  }
  lin->text[lin->len++] = c;
  lin->text[lin->len] = '\0';
}

void assignText(Line *lin, char *txt) {
  size_t l = strlen(txt);
  if (l + 1 >= lin->cap) {
    lin->cap = l + 1;
    lin->text = realloc(lin->text, lin->cap);
  }
  memcpy(lin->text, txt, l + 1);
  lin->len = l;
}

void assignKey(Line *lin, String *key) {
  char *cleaned = strip(key->text);
  assignString(lin->key, cleaned);
  free(cleaned);
}

void appendVal(Line *lin, String *val) {
  if (lin->vlen >= lin->vcap) {
    lin->vcap *= 2;
    lin->vals = realloc(lin->vals, sizeof(String *) * lin->vcap);
  }
  char *cleaned = strip(val->text);
  String *newVal = malloc(sizeof(String));
  initString(newVal);
  assignString(newVal, cleaned);
  free(cleaned);
  lin->vals[lin->vlen++] = newVal;
}

void freeLine(Line *lin) {
  if (lin->key) {
    freeString(lin->key);
    free(lin->key);
  }

  if (lin->vals) {
    for (size_t i = 0; i < lin->vlen; i++) {
      if (lin->vals[i]) {
        freeString(lin->vals[i]);
        free(lin->vals[i]);
      }
    }
    free(lin->vals);
  }
  free(lin->text);
}

Line *cloneLine(Line *lin) {
  Line *clone = malloc(sizeof(Line));
  initLine(clone);

  // Clone the text
  if (lin->len + 1 > clone->cap) {
    clone->cap = lin->len + 1;
    clone->text = realloc(clone->text, clone->cap);
  }
  memcpy(clone->text, lin->text, lin->len + 1);
  clone->len = lin->len;

  // Clone the key
  if (lin->key) {
    assignString(clone->key, lin->key->text);
  }

  // Clone each val
  for (size_t i = 0; i < lin->vlen; i++) {
    appendVal(clone, lin->vals[i]);
  }

  return clone;
}

//Entry
void initEntry(Entry *ent) {
  ent->lines = malloc(sizeof(Line *) * BUFFER);
  ent->len = 0;
  ent->cap = BUFFER;
}

void appendEntry(Entry *ent, Line *lin) {
  if (ent->len >= ent->cap) {
    ent->cap *= 2;
    ent->lines = realloc(ent->lines, sizeof(Line *) * ent->cap);
  }
  ent->lines[ent->len++] = lin;
}

Entry *cloneEntry(Entry *ent) {
  Entry *clone = malloc(sizeof(Entry));
  clone->len = ent->len;
  clone->cap = ent->cap;
  clone->lines = malloc(sizeof(Line *) * clone->cap);
  for (int i = 0; i < ent->len; i++) {
    clone->lines[i] = cloneLine(ent->lines[i]);
  }
  return clone;
}

void freeEntry(Entry *ent) {
  if (ent->lines) {
    for (size_t i = 0; i < ent->len; i++) {
      if (ent->lines[i]) {
        freeLine(ent->lines[i]);
        free(ent-> lines[i]);
      }
    }
    free(ent->lines);
  }
}

//Document
void initDocument(Document *doc) {
  doc->body = malloc(sizeof(String));
  initString(doc->body);
  doc->lines = malloc(sizeof(Line *) * BUFFER);
  doc->llen = 0;
  doc->lcap = BUFFER;
  doc->entries = malloc(sizeof(Entry *) * BUFFER);
  doc->elen = 0;
  doc->ecap = BUFFER;
}

void appendDocumentLine(Document *doc, Line *lin) {
  if (doc->llen >= doc->lcap) {
    doc->lcap *= 2;
    doc->lines = realloc(doc->lines, sizeof(Line *) * doc->lcap);
  }
  doc->lines[doc->llen++] = lin;
}

void appendDocumentEntry(Document *doc, Entry *ent) {
  if (doc->elen >= doc->ecap) {
    doc->ecap *= 2;
    doc->entries = realloc(doc->entries, sizeof(Entry *) * doc->ecap);
  }
  doc->entries[doc->elen++] = ent;
}

void freeDocument(Document *doc) {
  if (doc->lines) {
    for (size_t i = 0; i < doc->llen; i++) {
      if (doc->lines[i]) {
        freeLine(doc->lines[i]);
        free(doc->lines[i]);
      }
    }
    free(doc->lines);
  }

  if (doc->entries) {
    for (size_t i = 0; i < doc->elen; i++) {
      if (doc->entries[i]) {
        freeEntry(doc->entries[i]);
        free(doc->entries[i]);
      }
    }
    free(doc->entries);
  }

  if (doc->body) {
    freeString(doc->body);
    free(doc->body);
  }
}