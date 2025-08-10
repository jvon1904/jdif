#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#define BUFFER 64

// Define byte codes for various characters
#define CR 0x0a
#define SP 0x20
#define COLN 0x3A

// JSON output indents
#define IDNT2 "  "
#define IDNT4 "    "

// String
typedef struct {
  char *key;
  char *val;
  char *text;
  size_t len;
  size_t cap;
} String;

// Entry
typedef struct {
  String **lines;
  size_t len;
  size_t cap;  
} Entry;

// Document
typedef struct {
  String *body;
  String **lines;
  size_t llen;
  size_t lcap;
  Entry **entries;
  size_t elen;
  size_t ecap;
} Document;

void initString(String *str);

void assignText(String *str, char *txt);

void assignKey(String *str, String *key);

void assignVal(String *str, String *val);

void appendString(String *str, char c);

String *cloneString(String *str);

void freeString(String *str);

void initEntry(Entry *ent);

void appendEntry(Entry *ent, String *line);

Entry *cloneEntry(Entry *ent);

void freeEntry(Entry *ent);

void initDocument(Document *doc);

void appendDocumentLine(Document *doc, String *line);

void appendDocumentEntry(Document *doc, Entry *ent);

void freeDocument(Document *doc);

#endif