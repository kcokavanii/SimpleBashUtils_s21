#include "s21_cat.h"

#include <stdio.h>
#include <string.h>

void determine_flags(int argc, char *argv[], Flags *flags);
int check_file(FILE *file);
void implementation_flags(FILE *file, Flags *flags);
void flag_v(Flags *flags, int chr);

int main(int argc, char *argv[]) {
  Flags flags = {0};
  flags.num_of_lines = 1;
  flags.were_enter_printed = 1;
  determine_flags(argc, argv, &flags);
  for (int i = flags.index_file + 1; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (check_file(file) == 1) {
      implementation_flags(file, &flags);
      fclose(file);
    }
  }
  return 0;
}

void determine_flags(int argc, char *argv[], Flags *flags) {
  for (int i = 1; i < argc && argv[i][0] == '-'; i++) {
    if (strcmp(argv[i], "-b") == 0 ||
        strcmp(argv[i], "--number-nonblank") == 0) {
      flags->b = 1;
    } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0) {
      flags->n = 1;
    } else if (strcmp(argv[i], "-s") == 0 ||
               strcmp(argv[i], "--squeeze-blank") == 0) {
      flags->s = 1;
    } else if (strcmp(argv[i], "-T") == 0 ||
               strcmp(argv[i], "--show-tabs") == 0) {
      flags->T = 1;
    } else if (strcmp(argv[i], "-E") == 0 ||
               strcmp(argv[i], "--show-ends") == 0) {
      flags->E = 1;
    } else if (strcmp(argv[i], "-v") == 0 ||
               strcmp(argv[i], "--show-nonprinting") == 0) {
      flags->v = 1;
    } else if (strcmp(argv[i], "-t") == 0) {
      flags->v = 1;
      flags->T = 1;
    } else if (strcmp(argv[i], "-e") == 0) {
      flags->v = 1;
      flags->E = 1;
    } else if (strcmp(argv[i], "-A") == 0) {
      flags->v = 1;
      flags->E = 1;
      flags->T = 1;
    }
    flags->index_file = i;
  }
  if (flags->b == 1) {
    flags->n = 0;
  }
}

int check_file(FILE *file) {
  int res = 1;
  if (file == NULL) {
    res = 0;
    fprintf(stderr, "File is not opened!\n");
  }
  return res;
}

void implementation_flags(FILE *file, Flags *flags) {
  int chr;
  int num_enters = 1;

  while ((chr = fgetc(file)) != EOF) {
    if (((flags->n == 1 && flags->were_enter_printed == 1) ||
         (flags->b && flags->were_enter_printed && chr != '\n')) &&
        (flags->s != 1 || (flags->s && num_enters <= 2))) {
      printf("%6d\t", flags->num_of_lines);
    }
    if ((flags->b && flags->were_enter_printed && chr == '\n') &&
        (flags->s && num_enters == 2)) {
      printf("%6d\t", flags->num_of_lines);
    }

    if (chr == '\n') {
      flags->num_of_lines++;
      num_enters++;
      if ((flags->b == 1 && flags->were_enter_printed == 1) &&
          (flags->s != 1 || (flags->s && num_enters >= 2))) {
        flags->num_of_lines--;
      }
      if ((flags->n == 1 && flags->were_enter_printed == 1) &&
          (flags->s && num_enters > 2)) {
        flags->num_of_lines--;
      }
      if (flags->E == 1 && (flags->s != 1 || (flags->s && num_enters <= 2))) {
        printf("$\n");
        flags->were_enter_printed = 1;
      } else if (flags->s != 1 || (flags->s && num_enters <= 2)) {
        printf("\n");
        flags->were_enter_printed = 1;
      }
    }

    if (chr != '\n') {
      num_enters = 0;
      flags->were_enter_printed = 0;
      if (flags->T == 1 && chr == '\t') {
        printf("^I");
      }
      flag_v(flags, chr);
    }
  }
}

void flag_v(Flags *flags, int chr) {
  if (flags->v == 1 && chr < 32 && chr != 9 && chr != 10) {
    printf("^%c", (chr + 64));
  } else if (flags->v == 1 && chr == 127) {
    printf("^?");
  } else if (flags->v == 1 && chr == 255) {
    printf("M-^?");
  } else if (flags->v == 1 && chr > 127 && chr < 160) {
    printf("M-^%c", (chr - 64));
  } else if (flags->v == 1 && chr > 159 && chr < 255) {
    printf("M-%c", (chr - 128));
  } else if (!(flags->T == 1 && chr == '\t')) {
    printf("%c", chr);
  }
}