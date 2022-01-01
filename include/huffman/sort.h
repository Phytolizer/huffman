#ifndef HUFFMAN_SORT_H_
#define HUFFMAN_SORT_H_

#include "huffman/state.h"
#include <stdint.h>

void huffman_heapsort(huffman_state_t *state, uint16_t *array, uint16_t size);

#endif
