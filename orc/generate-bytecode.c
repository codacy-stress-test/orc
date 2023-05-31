
#include "config.h"

#include <orc/orc.h>
#include <orc/orcparse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int verbose = 0;
int error = 0;
int compat;

char *target = "sse";

#define ORC_VERSION(a,b,c,d) ((a)*1000000 + (b)*10000 + (c)*100 + (d))
#define REQUIRE(a,b,c,d) do { \
  if (ORC_VERSION((a),(b),(c),(d)) > compat) { \
    fprintf(stderr, "Feature used that is incompatible with --compat\n"); \
    exit (1); \
  } \
} while (0)

void help (void)
{
  printf("Usage:\n");
  printf("  generate-bytecode [OPTION...]\n");
  printf("\n");
  printf("Help Options:\n");
  printf("  -h, --help              Show help options\n");
  printf("\n");
  printf("Application Options:\n");
  printf("  -o, --output FILE       Write output to FILE\n");
  printf("      --header            Write header instead of .c file\n");
  printf("\n");

  exit (0);
}

int
main (int argc, char *argv[])
{
  char *output_file = NULL;
  char *input_file = NULL;
  FILE *output;
  int i;
  OrcOpcodeSet *opcode_set;
  int output_header = FALSE;

  orc_init ();

  for(i=1;i<argc;i++) {
    if (strcmp (argv[i], "--output") == 0 ||
        strcmp(argv[i], "-o") == 0) {
      if (i+1 < argc) {
        output_file = argv[i+1];
        i++;
      } else {
        help();
      }
    } else if (strcmp (argv[i], "--header") == 0) {
      output_header = TRUE;
    } else if (strncmp(argv[i], "-", 1) == 0) {
      printf("Unknown option: %s\n", argv[i]);
      exit (1);
    } else {
      if (input_file == NULL) {
        input_file = argv[i];
      } else {
        printf("More than one input file specified: %s\n", argv[i]);
        exit (1);
      }
    }
  }

  if (output_file == NULL) {
    output_file = output_header ? "out.h" : "out.c";
    printf("Writing to file: %s\n", output_file);
  }

  output = fopen (output_file, "w");
  if (!output) {
    printf("Could not write output file: %s\n", output_file);
    exit(1);
  }

  opcode_set = orc_opcode_set_get ("sys");

  fprintf(output, "\n");
  fprintf(output, "/* autogenerated by generate-bytecode */\n");
  fprintf(output, "\n");

  if (output_header) {
    fprintf(output, "#include <math.h>\n");
    fprintf(output, "#include <orc/orc.h>\n");
    fprintf(output, "\n");

    fprintf(output, "typedef enum {\n");
    fprintf(output, "  ORC_BC_END,\n");
    fprintf(output, "  ORC_BC_BEGIN_FUNCTION,\n");
    fprintf(output, "  ORC_BC_END_FUNCTION,\n");
    fprintf(output, "  ORC_BC_SET_CONSTANT_N,\n");
    fprintf(output, "  ORC_BC_SET_N_MULTIPLE,\n");
    fprintf(output, "  ORC_BC_SET_N_MINIMUM,\n");
    fprintf(output, "  ORC_BC_SET_N_MAXIMUM,\n");
    fprintf(output, "  ORC_BC_SET_2D,\n");
    fprintf(output, "  ORC_BC_SET_CONSTANT_M,\n");
    fprintf(output, "  ORC_BC_SET_NAME,\n");
    fprintf(output, "  ORC_BC_SET_BACKUP_FUNCTION,\n");
    fprintf(output, "  ORC_BC_ADD_DESTINATION,\n");
    fprintf(output, "  ORC_BC_ADD_SOURCE,\n");
    fprintf(output, "  ORC_BC_ADD_ACCUMULATOR,\n");
    fprintf(output, "  ORC_BC_ADD_CONSTANT,\n");
    fprintf(output, "  ORC_BC_ADD_CONSTANT_INT64,\n");
    fprintf(output, "  ORC_BC_ADD_PARAMETER,\n");
    fprintf(output, "  ORC_BC_ADD_PARAMETER_FLOAT,\n");
    fprintf(output, "  ORC_BC_ADD_PARAMETER_INT64,\n");
    fprintf(output, "  ORC_BC_ADD_PARAMETER_DOUBLE,\n");
    fprintf(output, "  ORC_BC_ADD_TEMPORARY,\n");
    fprintf(output, "  ORC_BC_INSTRUCTION_FLAGS,\n");
    for (i=22;i<32;i++){
      fprintf(output, "  ORC_BC_RESERVED_%d,\n", i);
    }
    for(i=0;i<opcode_set->n_opcodes;i++){
      OrcStaticOpcode *opcode = opcode_set->opcodes + i;

      if ((i+32)%10 == 0) {
        fprintf(output, "  /* %d */\n", i+32);
      }
      fprintf(output, "  ORC_BC_%s,\n", opcode->name);

    }
    fprintf(output, "  /* %d */\n", i+32);
    fprintf(output, "  ORC_BC_LAST\n");
    fprintf(output, "} OrcBytecodes;\n");
  } else {
    fprintf(output, "#ifdef HAVE_CONFIG_H\n");
    fprintf(output, "#include \"config.h\"\n");
    fprintf(output, "#endif\n");

    fprintf(output, "  ORC_BC_END,\n");
    fprintf(output, "  ORC_BC_BEGIN_FUNCTION,\n");
    fprintf(output, "  ORC_BC_END_FUNCTION,\n");
    fprintf(output, "  ORC_BC_SET_CONSTANT_N,\n");
    fprintf(output, "  ORC_BC_SET_N_MULTIPLE,\n");
    fprintf(output, "  ORC_BC_SET_N_MINIMUM,\n");
    fprintf(output, "  ORC_BC_SET_N_MAXIMUM,\n");
    fprintf(output, "  ORC_BC_SET_2D,\n");
    fprintf(output, "  ORC_BC_SET_CONSTANT_M,\n");
    fprintf(output, "  ORC_BC_SET_NAME,\n");
    fprintf(output, "  ORC_BC_SET_BACKUP_FUNCTION,\n");
    fprintf(output, "  ORC_BC_ADD_DESTINATION,\n");
    fprintf(output, "  ORC_BC_ADD_SOURCE,\n");
    fprintf(output, "  ORC_BC_ADD_ACCUMULATOR,\n");
    fprintf(output, "  ORC_BC_ADD_CONSTANT,\n");
    fprintf(output, "  ORC_BC_ADD_CONSTANT_INT64,\n");
    fprintf(output, "  ORC_BC_ADD_PARAMETER,\n");
    fprintf(output, "  ORC_BC_ADD_PARAMETER_FLOAT,\n");
    fprintf(output, "  ORC_BC_ADD_PARAMETER_INT64,\n");
    fprintf(output, "  ORC_BC_ADD_PARAMETER_DOUBLE,\n");
    fprintf(output, "  ORC_BC_ADD_TEMPORARY,\n");
    fprintf(output, "  ORC_BC_INSTRUCTION_FLAGS,\n");
    for (i=22;i<32;i++){
      fprintf(output, "  ORC_BC_RESERVED_%d,\n", i);
    }

    for(i=0;i<opcode_set->n_opcodes;i++){
      OrcStaticOpcode *opcode = opcode_set->opcodes + i;

      if ((i+32)%10 == 0) {
        fprintf(output, "  /* %d */\n", i+32);
      }
      fprintf(output, "  { \"%s\" },\n", opcode->name);

    }
    fprintf(output, "  /* %d */\n", i+32);
    fprintf(output, "  ORC_BC_LAST\n");
    fprintf(output, "} OrcBytecodes;\n");
  }

  fclose (output);

  if (error) exit(1);

  return 0;
}

