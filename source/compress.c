#include "huffman/compress.h"
#include "huffman/build.h"
#include "huffman/node.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FAIL()                                                                                                         \
	do                                                                                                             \
	{                                                                                                              \
		fprintf(stderr, "\nErreur : Lors de l'écriture (putc).\n\n");                                          \
		exit(EXIT_FAILURE);                                                                                    \
	} while (false)

void huffman_compress(huffman_state_t *state, FILE *rfd, FILE *wfd)
{
	int32_t c;
	int index;
	unsigned char encode;
	char *code[CHAR_COUNT];
	char buff[CHAR_COUNT];

	for (c = 0; c < CHAR_COUNT; c++)
	{
		huffman_node_init_leaf(&state->tree.nodes[c], c, HUFFMAN_NODE_NONE);
	}
	for (c = fgetc(rfd); c != EOF; c = getc(rfd))
	{
		if (state->tree.nodes[c].freq == UINT32_MAX)
		{
			fprintf(stderr, "\nLimite UINT_MAX atteinte.\n\n");
			exit(EXIT_FAILURE);
		}
		state->tree.nodes[c].freq++;
		if (state->file_size == SIZE_MAX)
		{
			fprintf(stderr, "\nLimite UINT_MAX atteinte.\n\n");
			exit(EXIT_FAILURE);
		}
		state->file_size++;
	}
	for (c = 0; c < 256; c++)
	{
		if (state->tree.nodes[c].freq != 0)
			state->leaves[state->num_leaves++] = c;
	}
	if (!huffman_build(state))
		return;
	if (state->num_leaves == 1)
	{
		code[state->leaves[0]] = (char *)malloc(sizeof(char) * 2);
		strcpy(code[state->leaves[0]], "0");
	}
	else
	{
		huffman_calculate_code(code, buff, state->tree.root, 0);
	}
	huffman_print(code);

	/* Compression */
	rewind(rfd);
	huffman_write(wfd, state->file_size);
	if (state->num_leaves == CHAR_COUNT)
	{
		if (fputc(1, wfd) == EOF || fputc(255, wfd) == EOF)
		{
			FAIL();
		}
	}
	else
	{
		if (fputc(0, wfd) == EOF || putc(state->num_leaves & 0xff, wfd) == EOF)
		{
			FAIL();
		}
	}
	huffman_write_header(wfd, state->tree.root);

	uint8_t shift = 7;
	uint8_t tmp = 0;
	huffman_write_tree(wfd, state->tree.root, &tmp, &shift);
	if (tmp != 0)
	{
		if (fputc(tmp, wfd) == EOF)
		{
			FAIL();
		}
	}
	shift = 0;
	encode = 0;
	for (c = fgetc(rfd); c != EOF; c = getc(rfd))
	{
		index = 0;
		while (code[c][index] != '\0')
		{
			if (shift < 8)
			{
				encode = (encode << 1) + (code[c][index] == '0' ? 0 : 1);
				shift++, index++;
			}
			else
			{
				if (fputc(encode, wfd) == EOF)
				{
					FAIL();
				}
				shift = 0, encode = 0;
			}
		}
	}
	if (shift > 0)
	{
		encode <<= (8 - shift);
		if (fputc(encode, wfd) == EOF)
		{
			FAIL();
		}
	}
	for (c = 0; c < state->num_leaves; c++)
		free(code[state->leaves[c]]);
}
