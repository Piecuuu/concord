#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "json-struct.c"
#include "json-scanf.h"

static
void print_usage (char * prog)
{
  fprintf(stderr,
          "Usage: %s [-h|-c|-d|-f|-C] -o output-file input-file\n"
          "      -C generate C code instead of C++ code which is the default\n"
          "      -h generate header with namespace\n"
          "      -c generate data and function definitions with namespace\n"
          "      -d generate data and function declarations without namespace\n"
          "      -f generate function definitions without namespace\n",
          prog);
  exit(EXIT_FAILURE);
}

int main (int argc, char ** argv)
{
  size_t len = 0;
  char * s;

  int opt;
  char * config_file = NULL;
  /*enum file_type type = FILE_SINGLE_FILE;*/
  struct emit_option eo = {
    .lang_C = false,
    .type = FILE_SINGLE_FILE
  };

  while ((opt = getopt(argc, argv, "hcdfWCo:")) != -1) {
    switch (opt) {
      case 'o':
        config_file = strdup(optarg);
        break;
      case 'h':
        eo.type = FILE_HEADER;
        break;
      case 'c':
        eo.type = FILE_CODE;
        break;
      case 'd':
        eo.type = FILE_DECLARATION;
        break;
      case 'f':
        eo.type = FILE_DEFINITION;
        break;
      case 'C':
        eo.lang_C = true;
        break;
      case 'W':
        eo.wrapper_only = true;
        break;
      default: /* '?' */
        print_usage(argv[0]);
    }
  }
  if (!config_file)
    print_usage(argv[0]);
  
  char * file =  argv[optind];
  s = orka_load_whole_file(file, &len);
  spec_name = file;
  spec_buffer.start = s;
  spec_buffer.size = len;

  struct jc_definition d;
  memset(&d, 0, sizeof(d));
  definition_from_json(s, len, &d);
  //print_definition(stderr, &d);
  FILE * fp = fopen(config_file, "w");
  d.spec_name = file;
  gen_definition(fp, &eo, &d);
  fclose(fp);
  return 0;
}
