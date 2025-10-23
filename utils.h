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
#define IDNT6 "      "

// String
typedef struct {
  char *text;
  size_t len;
  size_t cap;
} String;

// Line
typedef struct {
  String *key;
  String **vals;
  size_t vlen;
  size_t vcap;
  char *text;
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
  String *body;
  Line **lines;
  size_t llen;
  size_t lcap;
  Entry **entries;
  size_t elen;
  size_t ecap;
} Document;

// Misc
char *strip(char *str);

// String
void initString(String *str);

void prependString(String *str, char c);

void appendString(String *str, char c);

void assignString(String *str, const char *text);

void freeString(String *str);

String *cloneString(String *str);

// Line
void initLine(Line *lin);

void appendLine(Line *lin, char c);

void assignText(Line *lin, char *txt);

void assignKey(Line *lin, String *key);

void appendVal(Line *lin, String *val);

void freeLine(Line *lin);

Line *cloneLine(Line *lin);

//Entry
void initEntry(Entry *ent);

void appendEntry(Entry *ent, Line *lin);

Entry *cloneEntry(Entry *ent);

void freeEntry(Entry *ent);

//Document
void initDocument(Document *doc);

void appendDocumentLine(Document *doc, Line *lin);

void appendDocumentEntry(Document *doc, Entry *ent);

void freeDocument(Document *doc);

#endif