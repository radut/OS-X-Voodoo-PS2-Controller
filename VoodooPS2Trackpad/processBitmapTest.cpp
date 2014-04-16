#include <iostream>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* abs */

using namespace std;
#define BITS_PER_unsigned 8



int processBitmap(unsigned int xMap, unsigned int yMap, int *x1, int *y1, int *x2, int *y2);
int main(){

	int x1,x2,y1,y2;

	processBitmap(190262,111111,&x1,&y1,&x2,&y2);


	return 0;
}





int processBitmap(unsigned int xMap, unsigned int yMap, int *x1, int *y1, int *x2, int *y2) {
	struct alps_data {
		/* these are autodetected when the device is identified */
		int addr_command;
	    // sensible defaults required during initialization
		int x_max = 1360;
		int y_max = 660;
		int x_bits=23;
		int y_bits=12;

		int prev_fin;
		int multi_packet;
		unsigned int multi_data[6];
		int x1, x2, y1, y2;
		int fingers;
		unsigned int quirks;

	    int pktsize = 6;
	};
	static alps_data modelData;
    struct alps_bitmap_point {
        int start_bit;
        int num_bits;
    };

    int fingers_x = 0, fingers_y = 0, fingers;
    int i, bit, prev_bit;
    struct alps_bitmap_point xLow = {0,}, x_high = {0,};
    struct alps_bitmap_point yLow = {0,}, y_high = {0,};
    struct alps_bitmap_point *point;

    if (!xMap || !yMap) {
        return 0;
    }

    *x1 = *y1 = *x2 = *y2 = 0;

    prev_bit = 0;
    point = &xLow;
    for (i = 0; xMap != 0; i++, xMap >>= 1) {
        bit = xMap & 1;
        if (bit) {
            if (!prev_bit) {
                point->start_bit = i;
                fingers_x++;
            }
            point->num_bits++;
        } else {
            if (prev_bit)
                point = &x_high;
            else
                point->num_bits = 0;
        }
        prev_bit = bit;
    }

    /*
     * y bitmap is reversed for what we need (lower positions are in
     * higher bits), so we process from the top end.
     */
    yMap = yMap << (sizeof(yMap) * BITS_PER_unsigned  - modelData.y_bits);
    prev_bit = 0;
    point = &yLow;
    for (i = 0; yMap != 0; i++, yMap <<= 1) {
        bit = yMap & (1 << (sizeof(yMap) * BITS_PER_unsigned  - 1));
        if (bit) {
            if (!prev_bit) {
                point->start_bit = i;
                fingers_y++;
            }
            point->num_bits++;
        } else {
            if (prev_bit) {
                point = &y_high;
            } else {
                point->num_bits = 0;
            }
        }
        prev_bit = bit;
    }

    /*
     * Fingers can overlap, so we use the maximum count of fingers
     * on either axis as the finger count.
     */
    fingers = max(fingers_x, fingers_y);

    /*
     * If total fingers is > 1 but either axis reports only a single
     * contact, we have overlapping or adjacent fingers. For the
     * purposes of creating a bounding box, divide the single contact
     * (roughly) equally between the two points.
     */
    if (fingers > 1) {
        if (fingers_x == 1) {
            i = xLow.num_bits / 2;
            xLow.num_bits = xLow.num_bits - i;
            x_high.start_bit = xLow.start_bit + i;
            x_high.num_bits = max(i, 1);
        } else if (fingers_y == 1) {
            i = yLow.num_bits / 2;
            yLow.num_bits = yLow.num_bits - i;
            y_high.start_bit = yLow.start_bit + i;
            y_high.num_bits = max(i, 1);
        }
    }

    *x1 = (modelData.x_max * (2 * xLow.start_bit + xLow.num_bits - 1)) /
    (2 * (modelData.x_bits - 1));
    *y1 = (modelData.y_max * (2 * yLow.start_bit + yLow.num_bits - 1)) /
    (2 * (modelData.y_bits - 1));

    if (fingers > 1) {
        *x2 = (modelData.x_max * (2 * x_high.start_bit + x_high.num_bits - 1)) /
        (2 * (modelData.x_bits - 1));
        *y2 = (modelData.y_max * (2 * y_high.start_bit + y_high.num_bits - 1)) /
        (2 * (modelData.y_bits - 1));
    }

    printf("ps2: Process bitmap, fingers=%d, x1=%d, x2=%d, y1=%d, y2=%d, area = %d\n", fingers, *x1, *x2, *y1, *y2, abs(*x1 - *x2) * abs(*y1 - *y2));

    return fingers;
}
