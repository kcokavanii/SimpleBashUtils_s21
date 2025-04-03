#include "s21_grep.h"

void determine_flags(int argc, char *argv[], Flags *flags, char pattern[8192]);
int check_file(FILE *file, Flags *flags);
void get_str(FILE *file, char *str);
void implementation_flags(FILE *file, Flags *flags, char pattern[8192],
                          char *name_file);
int is_match(char pattern[8192], char line[4096], Flags *flags);

int main(int argc, char *argv[]) {
  Flags flags = {0};
  flags.index_file = 1;
  char pattern[8192] = "";
  determine_flags(argc, argv, &flags, pattern);
  if (argc - flags.index_file > 1) flags.is_many_files = 1;
  for (int i = flags.index_file; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (check_file(file, &flags) == 1) {
      implementation_flags(file, &flags, pattern, argv[i]);
      fclose(file);
    }
  }

  return 0;
}

void determine_flags(int argc, char *argv[], Flags *flags, char pattern[8192]) {
  for (int i; (i = getopt(argc, argv, "e:ivclnhsf:o")) != -1;) {
    flags->index_file++;
    if (i == 'e') {
      flags->e = 1;
      flags->index_file++;
      if (pattern[0] == '\0') {
        strcpy(pattern, optarg);
      } else {
        strcat(pattern, "\\|");
        strcat(pattern, optarg);
      }
      flags->num_e++;
    } else if (i == 'i') {
      flags->i = 1;
    } else if (i == 'v') {
      flags->v = 1;
    } else if (i == 'c') {
      flags->c = 1;
    } else if (i == 'l') {
      flags->l = 1;
    } else if (i == 'n') {
      flags->n = 1;
    } else if (i == 'h') {
      flags->h = 1;
    } else if (i == 's') {
      flags->s = 1;
    } else if (i == 'f') {
      flags->f = 1;
    } else if (i == 'o') {
      flags->o = 1;
    }
  }
  if (flags->e == 0 && flags->f == 0) {
    strcpy(pattern, argv[flags->index_file]);
    flags->index_file++;
  }
}

void implementation_flags(FILE *file, Flags *flags, char pattern[8192],
                          char *name_file) {
  char line[4096];
  int num_match_str = 0, num_line = 0;
  get_str(file, line);

  while (strlen(line) != 0) {
    num_line++;
    if (is_match(pattern, line, flags)) {
      num_match_str++;
      if (flags->c != 1 && flags->l != 1) {
        if (flags->is_many_files && flags->h != 1) printf("%s:", name_file);
        if (flags->n) printf("%d:", num_line);
        printf("%s", line);
        if (line[strlen(line) - 1] != '\n') {
          printf("%c", '\n');
        }
      }
    }
    get_str(file, line);
  }
  if ((flags->l && num_match_str > 0) ||
      (flags->c && flags->is_many_files && flags->h != 1 && flags->l != 1))
    printf("%s", name_file);
  if (flags->c && flags->is_many_files && flags->l != 1 && flags->h != 1)
    printf(":");
  if (flags->c && flags->l != 1) printf("%d", num_match_str);
  if ((flags->c && flags->l != 1) || (flags->l && num_match_str > 0))
    printf("\n");
  num_match_str = 0;
  num_line = 0;
}

int is_match(char pattern[8192], char line[4096], Flags *flags) {
  int res = 0, len_line = strlen(line);
  regex_t regex;
  char *new_line = (char *)calloc(4096, sizeof(char));
  strcpy(new_line, line);
  if (new_line[len_line - 1] == '\n') {
    new_line[len_line - 1] = '\0';
  }
  if (!regcomp(&regex, pattern, flags->i ? REG_ICASE : 0)) {
    if (regexec(&regex, new_line, 0, NULL, 0) == 0) {
      res = 1;
    }
    regfree(&regex);
  }

  if (flags->v) res = !res;
  free(new_line);
  return res;
}

int check_file(FILE *file, Flags *flags) {
  int res = 1;
  if (file == NULL) {
    res = 0;
    if (flags->s != 1) fprintf(stderr, "File is not opened!\n");
  }
  return res;
}

void get_str(FILE *file, char *str) {
  char chr;
  int i;
  for (i = 0; (chr = fgetc(file)) != '\n' && chr != EOF; i++) {
    str[i] = chr;
  }
  if (chr == '\n') {
    str[i] = chr;
    str[i + 1] = '\0';
  } else if (chr == EOF) {
    str[i] = '\0';
  }
}
