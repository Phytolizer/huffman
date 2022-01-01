#include "huffman/sort.h"
#include <stdbool.h>
#include <stdint.h>

static void sift(huffman_state_t *state, uint16_t *array, uint16_t pos, uint16_t size);
static bool freq_less(huffman_state_t *state, uint16_t i, uint16_t j);

void huffman_heapsort(huffman_state_t *state, uint16_t *array, uint16_t size)
{
	int i, tmp;

	for (i = size / 2; i >= 0; i--)
		sift(state, array, i, size);
	for (i = size; i > 0; i--)
	{
		tmp = array[i], array[i] = array[0], array[0] = tmp;
		sift(state, array, 0, i - 1);
	}
}

static void sift(huffman_state_t *state, uint16_t *array, uint16_t pos, uint16_t size)
{
	int j, tmp;

	j = 2 * pos;
	while (j <= size)
	{
		if (j < size && freq_less(state, j, j + 1))
			j++;
		if (freq_less(state, pos, j))
		{
			tmp = array[pos], array[pos] = array[j], array[j] = tmp;
			pos = j, j = 2 * pos;
		}
		else
			j = size + 1;
	}
}

static bool freq_less(huffman_state_t *state, uint16_t i, uint16_t j)
{
	uint32_t freq_i = state->tree.nodes[state->leaves[i]].freq;
	uint32_t freq_j = state->tree.nodes[state->leaves[j]].freq;
	return freq_i < freq_j;
}
