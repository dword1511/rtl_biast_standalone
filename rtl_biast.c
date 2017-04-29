/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#include "getopt/getopt.h"
#endif

#include <rtl-sdr.h>

#define EEPROM_SIZE   256
#define MAX_STR_SIZE  256
#define STR_OFFSET  0x09

static rtlsdr_dev_t *dev = NULL;

typedef struct rtlsdr_config {
  uint16_t vendor_id;
  uint16_t product_id;
  char manufacturer[MAX_STR_SIZE];
  char product[MAX_STR_SIZE];
  char serial[MAX_STR_SIZE];
  int have_serial;
  int enable_ir;
  int remote_wakeup;
} rtlsdr_config_t;

/* Undocumented/exported functions, only linkable against static version of librtlsdr.  */
void rtlsdr_set_gpio_bit(rtlsdr_dev_t *dev, uint8_t gpio, int val);
void rtlsdr_set_gpio_output(rtlsdr_dev_t *dev, uint8_t gpio);

/* Copied from rtlsdrblog's rtl-biast and modified for internal use. */
static void rtlsdr_set_bias_tee(rtlsdr_dev_t *dev, int on) {
  fprintf(stdout, "rtlsdr_set_bias_tee: %d.\n", on);
  rtlsdr_set_gpio_output(dev, 0);
  rtlsdr_set_gpio_bit(dev, 0, on);
}

static void usage(char *self) {
  fprintf(stderr,
    "rtl_biast, a tool for turning the RTL-SDR.com bias tee ON and OFF.\n\n"
    "Usage:\n"
    "\t%s [-d device_index (default: 0)] [-b bias_on]\n\n"
    "Example: to turn on the bias tee:\n"
    "\t%s -d 0 -b 1\n\n",
    self, self);
  exit(1);
}

int main(int argc, char *argv[]) {
  int r, opt;
  uint32_t dev_index = 0;
  int bias_on = -1;

  while ((opt = getopt(argc, argv, "d:b:h?")) != -1) {
    switch (opt) {
      case 'd': {
        dev_index = atoi(optarg);
        break;
      }
      case 'b': {
        bias_on = atoi(optarg);
        break;
      }
      default: {
        usage(argv[0]);
        break;
      }
    }
  }

  if (bias_on == -1) {
    usage(argv[0]);
  }

  r = rtlsdr_open(&dev, dev_index);

  if (r >= 0) {
    rtlsdr_set_bias_tee(dev, bias_on);
  }

  rtlsdr_close(dev);
  return r >= 0 ? r : -r;
}
