#pragma once

#if 1
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define BOSYED_IMPLEMENTATION
#endif


#ifndef _BOSYED_MALLOC
#define _BOSYED_MALLOC malloc
#endif // _BOSYED_MALLOC

#ifndef _BOSYED_MEMCPY
#define _BOSYED_MEMCPY memcpy
#endif // _BOSYED_MEMCPY

#ifndef _BOSYED_FREE
#define _BOSYED_FREE free
#endif // _BOSYED_FREE

#ifndef _BOSYED_PRINTF
#define _BOSYED_PRINTF printf
#endif // _BOSYED_PRINTF

#ifndef _BOSYED_STRLEN
#define _BOSYED_STRLEN strlen
#endif // _BOSYED_STRLEN

#ifndef _BOSYED_LICENSE
#define _BOSYED_LICENSE(li)
#endif // _BOSYED_LICENSE

_BOSYED_LICENSE("BOSYEd is released under CC0+MIT.")

typedef struct BOSYEdLines {
  struct BOSYEdLines *next;
  char *line;
  int id;
} BOSYEdLines;

typedef struct BOSYEdState {
  BOSYEdLines *lines;
} BOSYEdState;

BOSYEdState BOSYEdInit(const char *text);
BOSYEdState BOSYEdProccess(BOSYEdState state, const char *cmd);

#ifdef BOSYED_IMPLEMENTATION
BOSYEdState BOSYEdInit(const char *text) {
  BOSYEdState state = {0};
  BOSYEdLines *line = _BOSYED_MALLOC(sizeof(BOSYEdLines));
  state.lines = line;
  state.lines->id = -1;
  int cursor = 0;
  int i = 0;
  for (; text[i]; ++i) {
    if (text[i] == '\n') {
      BOSYEdLines *new_line = _BOSYED_MALLOC(sizeof(BOSYEdLines));
      new_line->id = line->id + 1;
      new_line->next = 0;
      line->next = new_line;
      line = new_line;
      line->line = _BOSYED_MALLOC(i - cursor + 1);
      line->line[i - cursor] = 0;
      _BOSYED_MEMCPY(line->line, text + cursor, i - cursor);
      cursor = i + 1;
    }
  }
  if (cursor != i) {
    BOSYEdLines *new_line = _BOSYED_MALLOC(sizeof(BOSYEdLines));
    new_line->id = line->id + 1;
    new_line->next = 0;
    line->next = new_line;
    line = new_line;
    line->line = _BOSYED_MALLOC(i - cursor + 1);
    line->line[i - cursor] = 0;
    _BOSYED_MEMCPY(line->line, text + cursor, i - cursor);
    cursor = i + 1;
  }
  BOSYEdLines *a = state.lines->next;
  _BOSYED_FREE(state.lines);
  state.lines = a;
  return state;
}
static long BOSYEdAtoi(const char *str) {
  long l = 0;
  for (int i = 0; str[i]; ++i) {
    if (str[i] >= '0' && str[i] <= '9') {
      l *= 10;
      l += str[i] - '0';
    }
  }
  return l;
}
static long BOSYEdAtoiN(const char *str, long n) {
  long l = 0;
  for (int i = 0; i < n && str[i]; ++i) {
    if (str[i] >= '0' && str[i] <= '9') {
      l *= 10;
      l += str[i] - '0';
    }
  }
  return l;
}
BOSYEdState BOSYEdProccess(BOSYEdState state, const char *cmd) {
  switch (cmd[0]) {
  case ',': {
    BOSYEdLines *curr = state.lines;
    long line = -1;
    if (cmd[1] != '\n' && cmd[1]) {
      line = BOSYEdAtoi(cmd + 1);
    }
    while (curr) {
      if (line != -1) {
	if (curr->id == line) {
        printf("%d\t\t|", curr->id);
        for (unsigned long i = 0; curr->line[i]; ++i) printf("%c", '0'+(i%10));
        printf("\n");
          printf("%d\t\t|%s\n", curr->id, curr->line);
	  break;
	}
      }
      else {
       printf("%d\t\t|%s\n", curr->id, curr->line);
      }
      curr = curr->next;
    }
    break;
  }
  case '.': {
    int lower = 1;
    for (lower = 1; cmd[lower] != ' '; ++lower);
    long line0 = BOSYEdAtoiN(cmd + 1, lower - 1);
    long line1 = BOSYEdAtoi(cmd + lower + 1);
    BOSYEdLines *curr = state.lines;
    while (curr) {
      if (curr->id >= line0 && curr->id <= line1) {
printf("%d\t\t|%s\n", curr->id, curr->line);
      }
      curr = curr->next;
    }
    break;
  }
  case 'a': {
    BOSYEdLines *curr = state.lines;
    if (!curr) {
      BOSYEdLines *new_line = _BOSYED_MALLOC(sizeof(BOSYEdLines));
      new_line->id = 0;
      new_line->next = 0;
      int size = _BOSYED_STRLEN(cmd + 1);
      new_line->line = _BOSYED_MALLOC(size + 1);
      new_line->line[size] = 0;
      _BOSYED_MEMCPY(new_line->line, cmd + 1, size);
      state.lines = new_line;
      break;
    }
    while (curr) {
      if (!curr->next) {
	BOSYEdLines *new_line = _BOSYED_MALLOC(sizeof(BOSYEdLines));
	new_line->id = curr->id + 1;
	new_line->next = 0;
	int size = _BOSYED_STRLEN(cmd + 1);
	new_line->line = _BOSYED_MALLOC(size + 1);
	new_line->line[size] = 0;
	_BOSYED_MEMCPY(new_line->line, cmd + 1, size);
	curr->next = new_line;
	break;
      }
      curr = curr->next;
    }
    break;
  }
  case 'e': {
    int lower = 1;
    for (lower = 1; cmd[lower] != ' '; ++lower);
    long line = BOSYEdAtoiN(cmd + 1, lower - 1);
    const char *text = cmd + lower + 1;
    BOSYEdLines *curr = state.lines;
    int adding = 0;
    while (curr) {
      if (curr->id == line) {
	BOSYEdLines *new_line = _BOSYED_MALLOC(sizeof(BOSYEdLines));
	new_line->id = curr->id + 1;
	new_line->next = curr->next;
	int size = _BOSYED_STRLEN(text);
	new_line->line = _BOSYED_MALLOC(size + 1);
	new_line->line[size] = 0;
	_BOSYED_MEMCPY(new_line->line, text, size);
	curr->next = new_line;
	adding = 1;
	curr = curr->next->next;
	continue;
      }
      else if (adding) {
	++curr->id;
      }
      curr = curr->next;
    }
    break;
  }
  case 'i': {
    int lower = 1;
    for (lower = 1; cmd[lower] != ' '; ++lower);
    long line = BOSYEdAtoiN(cmd + 1, lower - 1);
    ++lower;
    int slower = lower;
    for (; cmd[lower] != ' '; ++lower);
    long col = BOSYEdAtoiN(cmd + slower, lower - slower + 1);
    const char *text = cmd + lower + 1;
    BOSYEdLines *curr = state.lines;
    int adding = 0;
    while (curr) {
      if (curr->id == line) {
	int tsize = _BOSYED_STRLEN(text);
	int size = tsize + _BOSYED_STRLEN(curr->line);
	char *new = _BOSYED_MALLOC(size+1);
	_BOSYED_MEMCPY(new, curr->line, col);
	_BOSYED_MEMCPY(new+col, text, tsize);
	_BOSYED_MEMCPY(new+col+tsize, curr->line+col, size-tsize-col+1);
	_BOSYED_FREE(curr->line);
	curr->line = new;
	break;
      }
    }
    break;
  }
  case 'r': {
    int lower = 1;
    for (lower = 1; cmd[lower] != ' '; ++lower);
    long line = BOSYEdAtoiN(cmd + 1, lower - 1);
    ++lower;
    int slower = lower;
    for (; cmd[lower] != ' '; ++lower);
    long col = BOSYEdAtoiN(cmd + slower, lower - slower + 1);
    long amount = BOSYEdAtoi(cmd + lower + 1);
    BOSYEdLines *curr = state.lines;
    int adding = 0;
    while (curr) {
      if (curr->id == line) {
	int size = amount + _BOSYED_STRLEN(curr->line);
	char *new = _BOSYED_MALLOC(size+1);
	_BOSYED_MEMCPY(new, curr->line, col);
	_BOSYED_MEMCPY(new+col+amount, curr->line+col, size-amount-col+1);
	_BOSYED_FREE(curr->line);
	curr->line = new;
	break;
      }
    }
    break;
  }
  case 'd': {
    int line = BOSYEdAtoi(cmd + 1);
    BOSYEdLines *prev = 0;
    BOSYEdLines *curr = state.lines;
    int subing = 0;
    while (curr) {
      if (curr->id == line) {
	if (prev) {
	  prev->next = curr->next;
	  _BOSYED_FREE(curr);
	  curr = prev->next;
	}
	else {
	  state.lines = curr->next;
	  _BOSYED_FREE(curr);
	  curr = state.lines;
	}
	subing = 1;
	continue;
      }
      else if (subing) {
	--curr->id;
      }
      prev = curr;
      curr = curr->next;
    }
    break;
  }
  default:
    _BOSYED_PRINTF("?\n");
  };
  return state;
}
#endif // BOSYED_IMPLEMENTATION

#if 1
int main(int argc, char **argv) {
  BOSYEdState state;
  if (argc != 2) state = BOSYEdInit("");
  else {
    FILE *fp = fopen(argv[1], "r");
    fseek(fp, 0, SEEK_END);
    unsigned long size = ftell(fp);
    char *buf = malloc(size);
    fseek(fp, 0, SEEK_SET);
    fread(buf, size, 1, fp);
    state = BOSYEdInit(buf);
    free(buf);
    fclose(fp);
  }
  char buf[512];
  int count;
  while (fgets(buf, 511, stdin)) {
    buf[strlen(buf)-1] = 0;
    if (!strcmp(buf, "?")) {
      printf("Commands:\n? - List\nw<FILENAME> - write\nq - quit\n!<CMD> - run system command\na<TEXT> - append to end\ne<LINE> <TEXT> - extend after line\nd<LINE> - delete line\n,[LINE] - show all lines or one\n.<FROM> <TO> - list line range\ni<LINE> <COL> <TEXT> - insert text at line after col\nr<LINE> <START> <SIZE> - eRase <SIZE> bytes from index <START> at line <LINE>\n");
      continue;
    }
    if (buf[0] == 'w') {
      if (!buf[1]) fprintf(stderr, "Usage: w<file path>\n");
      else {
	FILE *fp = fopen(buf + 1, "w");
	BOSYEdLines *line = state.lines;
	while (line) {
	  if (line->next) fprintf(fp, "%s\n", line->line);
	  else fprintf(fp, "%s", line->line);
	  line = line->next;
	}
	fclose(fp);
      }
      continue;
    }
    if (buf[0] == '!') {
      system(buf + 1);
      continue;
    }
    if (!strcmp(buf, "q")) {
      break;
    }
    state = BOSYEdProccess(state, buf);
  }
  return 0;
}
#endif // 1!
