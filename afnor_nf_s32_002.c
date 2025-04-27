/**
 * Copyright 2025 (c) iomonad <iomonad@riseup.net>
 *
 * ----------------------------------------------------------------------------
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute
 * this software, either in source code form or as a compiled binary, for any
 * purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors of
 * this software dedicate any and all copyright interest in the software to
 * the public domain. We make this dedication for the benefit of the public
 * at large and to the detriment of our heirs and successors. We intend this
 * dedication to be an overt act of relinquishment in perpetuity of all
 * present and future rights to this software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ----------------------------------------------------------------------------
 *
 * DISCLAIMER:
 * This software may enable the use of radio frequencies. In France, the use
 * of radio frequencies or radio equipment outside regulatory conditions is a
 * criminal offence punishable by up to 6 months' imprisonment and a fine of
 * up to €30,000. Use of this code for illegitimate or unauthorized radio
 * transmission is strictly prohibited. It is the user's responsibility to
 * ensure compliance with all applicable laws and regulations before using
 * this software for radio frequency operations.
 *
 * For more information, consult the ANFR (Agence nationale des fréquences)
 * and the French Postal and Electronic Communications Code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libhackrf/hackrf.h>

#define SAMPLE_RATE 0x1e8480
#define CENTER_FREQ 0x33c134e0
#define TX_GAIN     0x71

static uint8_t* generate_pb(int *sc) {
     const float pb_duration = 830e-6;
     const int samples = (int)(pb_duration * SAMPLE_RATE);
     *sc = samples;

     uint8_t *pb = NULL;

     if ((pb = (uint8_t*)malloc(samples)) == NULL) {
	  printf("pb alloc failed\n");
	  exit(EXIT_FAILURE);
     }

     const int half_period = samples / 4;

     for (int i = 0; i < samples; i++)
	  pb[i] = (i / half_period) % 2 ? 0x7f : 0;

     return pb;
}

static uint8_t* build_packet(int *total_samples) {
     const uint8_t itc[] = {0x33, 0x8A, 0x00};
     const int bits = 24;
     const int spb = 20;

     int preamble_samples;
     uint8_t *preamble = generate_pb(&preamble_samples);
     *total_samples = preamble_samples + bits * spb;

     uint8_t *samples = NULL;

     if ((samples = (uint8_t*)malloc(*total_samples)) == NULL) {
	  printf("allocation failed\n");
	  exit(EXIT_FAILURE);
     }

     memcpy(samples, preamble, preamble_samples);
     free(preamble);

     int sidx = preamble_samples;
     for (int i = 0; i < bits; i++) {
	  int bidx = i / 8;
	  int bit_idx = 7 - (i % 8);
	  uint8_t bit = (itc[bidx] >> bit_idx) & 0x01;

	  for(int j=0; j<spb; j++) {
	       samples[sidx++] = bit ? 127 : 0;
	  }
     }
     return samples;
}


static int tx_callback(hackrf_transfer *transfer) {
     static size_t bytes_remaining = 0;
     static uint8_t *samples = NULL;

     if (!bytes_remaining) {
	  int total_samples;
	  samples = build_packet(&total_samples);
	  bytes_remaining = total_samples;
     }

     size_t bytes_to_send = transfer->valid_length;
     if (bytes_to_send > bytes_remaining) {
	  bytes_to_send = bytes_remaining;
     }

     memcpy(transfer->buffer, samples, bytes_to_send);
     samples += bytes_to_send;
     bytes_remaining -= bytes_to_send;

     if(bytes_remaining == 0) {
	  free(samples - bytes_to_send);
	  return -1;
     }
     return 0;
}

int main() {
     hackrf_device *device = NULL;

     if (hackrf_init() != HACKRF_SUCCESS) {
	  return EXIT_FAILURE;
     }

     if (hackrf_open(&device) != HACKRF_SUCCESS) {
	  hackrf_exit();
	  return EXIT_FAILURE;
     }

     hackrf_set_freq(device, CENTER_FREQ);
     hackrf_set_sample_rate_manual(device, SAMPLE_RATE, 1);
     hackrf_set_txvga_gain(device, TX_GAIN);

     hackrf_start_tx(device, tx_callback, NULL);

     while (hackrf_is_streaming(device) == HACKRF_TRUE) {
	  ;;
     }

     hackrf_close(device);
     hackrf_exit();
     printf("OK\n");

     return EXIT_SUCCESS;
}
