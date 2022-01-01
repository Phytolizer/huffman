#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include "huffman/state.h"
#include <stdio.h>

void huffman_compress(huffman_state_t *state, FILE *rfd, FILE *wfd);

#endif
