#include "utils.h"
#include <stdlib.h>
#include <string.h>

void initString(String *str) {
  str->text = malloc(BUFFER);
  str->text[0] = '\0';
  str->text[1] = '\0';
  str->key = malloc(BUFFER);
  str->val = malloc(BUFFER);
  str->len = 0;
  str->cap = BUFFER;
}

void assignText(String *str, char *txt) {
  str->text = realloc(str->text, strlen(txt) + 1);
  strcpy(str->text, txt);
}

void assignKey(String *str, String *key) {
  str->key = realloc(str->key, strlen(key->text) + 1);
  strcpy(str->key, key->text);
}

void assignVal(String *str, String *val) {
  str->val = realloc(str->val, strlen(val->text) + 1);
  strcpy(str->val, val->text);
}

void appendString(String *str, char c) {
  if (str->len + 1 >= str->cap) {
    str->cap *= 2;
    str->text = realloc(str->text, str->cap);
  }
  str->text[str->len++] = c;
  str->text[str->len] = '\0';
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

void freeString(String *str) {
  free(str->text);
}

void initEntry(Entry *ent) {
  ent->lines = malloc(sizeof(String *) * BUFFER);
  ent->len = 0;
  ent->cap = BUFFER;
}

void appendEntry(Entry *ent, String *line) {
  if (ent->len >= ent->cap) {
    ent->cap *= 2;
    ent->lines = realloc(ent->lines, sizeof(String *) * ent->cap);
  }
  ent->lines[ent->len++] = line;
}

Entry *cloneEntry(Entry *ent) {
  Entry *clone = malloc(sizeof(Entry));
  clone->len = ent->len;
  clone->cap = ent->cap;
  clone->lines = malloc(sizeof(String *) * clone->cap);
  for (size_t i = 0; i < ent->len; i++) {
    clone->lines[i] = cloneString(ent->lines[i]);
  }
  return clone;
}

void freeEntry(Entry *ent) {
  free(ent->lines);
}

void initDocument(Document *doc) {
  doc->body = malloc(sizeof(String));
  initString(doc->body);
  doc->lines = malloc(sizeof(String *) * BUFFER);
  doc->llen = 0;
  doc->lcap = BUFFER;
  doc->entries = malloc(sizeof(Entry *) * BUFFER);
  doc->elen = 0;
  doc->ecap = BUFFER;
}

void appendDocumentLine(Document *doc, String *line) {
  if (doc->llen >= doc->lcap) {
    doc->lcap *= 2;
    doc->lines = realloc(doc->lines, sizeof(String *) * doc->lcap);
  }
  doc->lines[doc->llen++] = line;
}

void appendDocumentEntry(Document *doc, Entry *ent) {
  if (doc->elen >= doc->ecap) {
    doc->ecap *= 2;
    doc->entries = realloc(doc->entries, sizeof(Entry *) * doc->ecap);
  }
  doc->entries[doc->elen++] = ent;
}

void freeDocument(Document *doc) {
  free(doc->lines);
}