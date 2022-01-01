#include "huffman/state.h"
#include "huffman/node.h"
#include <stdlib.h>
#include <string.h>

huffman_state_t *huffman_state_new(void)
{
	huffman_state_t *state = malloc(sizeof(huffman_state_t));
	if (state == NULL)
		return NULL;
	huffman_state_init(state);
	return state;
}

void huffman_state_init(huffman_state_t *state)
{
	memset(state, 0, sizeof(huffman_state_t));
	for (uint16_t c = 0; c < CHAR_COUNT; c++)
	{
		huffman_node_init_leaf(&state->tree.nodes[c], c, HUFFMAN_NODE_NONE);
	}
}
