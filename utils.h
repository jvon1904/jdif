#include <stdlib.h>
#include <string.h>

#define BUFFER 64

// Define byte codes for various characters
#define CR 0x0a
#define SP 0x20

// JSON output indents
#define IDNT2 "  "
#define IDNT4 "    "

// Key
typedef struct {
  char *text;
  size_t len;
  size_t cap;
} Key;

// Value
typedef struct {
  char *text;
  size_t len;
  size_t cap;
} Value;

// Line
typedef struct {
  char *text;
  char *key;
  char *value;
  size_t len;
  size_t cap;
} Line;

// Entry
typedef struct {
  Line **lines;
  size_t len;
  size_t cap;  
} Entry;

// Document
typedef struct {
  Line **lines;
  size_t llen;
  size_t lcap;
  Entry **entries;
  size_t elen;
  size_t ecap;
} Document;

void initLine(Line *line) {
  line->text = malloc(BUFFER);
  line->text[0] = '\0';
  line->len = 0;
  line->cap = BUFFER;
}

void appendLine(Line *line, char c) {
  if (line->len + 1 >= line->cap) {
    line->cap *= 2;
    line->text = realloc(line->text, line->cap);
  }
  line->text[line->len++] = c;
  line->text[line->len] = '\0';
}

Line *cloneLine(Line *line) {
  Line *clone = malloc(sizeof(Line));
  clone->len = line->len;
  clone->cap = line->cap;
  clone->text = malloc(line->cap);
  memcpy(clone->text, line->text, line->len);
  clone->text[line->len] = '\0';
  return clone;
}

void freeLine(Line *line) {
  free(line->text);
}

void initEntry(Entry *ent) {
  ent->lines = malloc(sizeof(Line *) * BUFFER);
  ent->len = 0;
  ent->cap = BUFFER;
}

void appendEntry(Entry *ent, Line *line) {
  if (ent->len >= ent->cap) {
    ent->cap *= 2;
    ent->lines = realloc(ent->lines, sizeof(Line *) * ent->cap);
  }
  ent->lines[ent->len++] = line;
}

Entry *cloneEntry(Entry *ent) {
  Entry *clone = malloc(sizeof(Entry));
  clone->len = ent->len;
  clone->cap = ent->cap;
  clone->lines = malloc(sizeof(Line *) * clone->cap);
  for (size_t i = 0; i < ent->len; i++) {
    clone->lines[i] = cloneLine(ent->lines[i]);
  }
  return clone;
}

void freeEntry(Entry *ent) {
  free(ent->lines);
}

void initDocument(Document *doc) {
  doc->lines = malloc(sizeof(Line *) * BUFFER);
  doc->llen = 0;
  doc->lcap = BUFFER;
  doc->entries = malloc(sizeof(Entry *) * BUFFER);
  doc->elen = 0;
  doc->ecap = BUFFER;
}

void appendDocumentLine(Document *doc, Line *line) {
  if (doc->llen >= doc->lcap) {
    doc->lcap *= 2;
    doc->lines = realloc(doc->lines, sizeof(Line *) * doc->lcap);
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
