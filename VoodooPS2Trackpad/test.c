int ApplePS2ALPSGlidePoint::processBitmap(unsigned int xMap, unsigned int yMap, int *x1, int *y1, int *x2, int *y2) {
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
    yMap = yMap << (sizeof(yMap) * BITS_PER_BYTE - modelData.y_bits);
    prev_bit = 0;
    point = &yLow;
    for (i = 0; yMap != 0; i++, yMap <<= 1) {
        bit = yMap & (1 << (sizeof(yMap) * BITS_PER_BYTE - 1));
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
    fingers = MAX(fingers_x, fingers_y);

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
            x_high.num_bits = MAX(i, 1);
        } else if (fingers_y == 1) {
            i = yLow.num_bits / 2;
            yLow.num_bits = yLow.num_bits - i;
            y_high.start_bit = yLow.start_bit + i;
            y_high.num_bits = MAX(i, 1);
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

    DEBUG_LOG("ps2: Process bitmap, fingers=%d, x1=%d, x2=%d, y1=%d, y2=%d, area = %d\n", fingers, *x1, *x2, *y1, *y2, abs(*x1 - *x2) * abs(*y1 - *y2));

    return fingers;
}

void ApplePS2ALPSMultiTouch::process_mt( UInt8 * packet, UInt32  packetSize )
{

#if APPLESDK
	clock_get_uptime(&now);
#else
	clock_get_uptime( (uint64_t*) &now );
#endif

	int v[10], x1, x2, y1, y2, bit, x, y, fingers;
	if (_touchPadVersion == 3) {
		fingers = ( packet[5] & 0x3 ) + 1;
		x = ( ( st[1] & 0x7f ) << 4 ) | ( ( st[4] & 0x30 ) >> 2 )
				| ( ( st[0] & 0x30 ) >> 4 );
		y = ( ( st[2] & 0x7f ) << 4 ) | ( st[4] & 0x0f );
	} else if (_touchPadVersion == 6) {
		fingers = ( ( packet[0] & 0x06 ) >> 1 ) | ( ( packet[0] & 0x10 ) >> 2 );
		x = ( st[1] & 0x7f ) | ( ( st[4] & 0x0f ) << 7 );
		y = ( st[2] & 0x7f ) | ( ( st[4] & 0xf0 ) << 3 );
	}

	switch (fingers) {
		case 2:
			if (!action2)	// right button
			{
				l_t = mach_absolute_time();
				if (action < 2)
					action = 2;
			}
			action2 = 1;

			UInt64 x_map, y_map;
			int x_max;

			for (int i = 0; i < 10; i++)
				v[i] = 0;

			if (_touchPadVersion == 3) {
				x_map = ( ( packet[4] & 0x7e ) << 8 )
						| ( ( packet[1] & 0x7f ) << 2 )
						| ( ( packet[0] & 0x30 ) >> 4 );
				x_max = 15;
			} else if (_touchPadVersion == 6) {
				x_map = ( ( packet[2] & 0x60 ) >> 5 )
						| ( ( packet[4] & 0x7f ) << 2 )
						| ( ( packet[5] & 0x7f ) << 9 )
						| ( ( packet[3] & 0x07 ) << 16 )
						| ( ( packet[3] & 0x70 ) << 15 )
						| ( ( packet[0] & 0x01 ) << 22 );
				x_max = 23;
			}

			for (int i = 0, find = 0; i <= x_max; i++) {
				if (( ( x_map >> i & 1 ) != bit ) && find % 2)
					v[find++] = x_max - i + 1;
				else if (( x_map >> i & 1 ) != bit)
					v[find++] = x_max - i;
				bit = x_map >> i & 1;
			}

			if (!v[2] && !v[3]) {
				x1 = v[1] + ( v[0] - v[1] ) * 1 / 4;
				x2 = v[1] + ( v[0] - v[1] ) * 3 / 4;
			} else {
				x1 = ( v[3] + v[2] ) / 2;
				x2 = ( v[1] + v[0] ) / 2;
			}

			for (int i = 0; i < 10; i++)
				v[i] = 0;

			if (_touchPadVersion == 3)
				y_map = ( ( packet[2] & 0x7f ) << 1 )
						| ( ( packet[3] & 0x70 ) << 4 ) | ( packet[4] & 0x01 );
			else if (_touchPadVersion == 6)
				y_map = ( packet[1] & 0x7f ) | ( ( packet[2] & 0x1f ) << 7 );

			for (int i = 0, find = 0; i <= 12; i++) {
				if (( ( y_map >> i & 1 ) != bit ) && find % 2)
					v[find++] = i + 1;
				else if (( y_map >> i & 1 ) != bit)
					v[find++] = i;
				bit = y_map >> i & 1;
			}

			if (!v[2] && !v[3]) {
				y1 = v[1] + ( v[0] - v[1] ) * 3 / 4;
				y2 = v[1] + ( v[0] - v[1] ) * 1 / 4;
			} else {
				y1 = ( v[1] + v[0] ) / 2;
				y2 = ( v[3] + v[2] ) / 2;
			}

			if (last_fingers != 2) {
				i_x = x;
				i_y = y;
				i_x1 = x1;
				i_x2 = x2;
				prec = 50;
			} else {
				//vert scroll
				if (( x2 - x1 ) == ( last_x2 - last_x1 ) && ( y - l_y ) < prec
						&& ( y - l_y ) > -prec
						&& ( ( y - i_y ) < -prec * 2 || ( y - i_y ) > prec * 2 )
						&& _vscroll && mach_absolute_time() - l_t > 1 << 26) {
					action = 0;
					dispatchScrollWheelEvent(
							( l_y - y ) / 2
									* (double) ( _saccell / 8192 / 8 + 1 ), 0,
							0, now );
					i_x = x;
					i_y = y;
					prec = 25;
				}

				//horiz scroll
				if (( y2 - y1 ) == ( last_y2 - last_y1 ) && ( x - l_x ) < prec
						&& ( x - l_x ) > -prec
						&& ( ( x - i_x ) < -prec * 2 || ( x - i_x ) > prec * 2 )
						&& _hscroll && mach_absolute_time() - l_t > 1 << 26) {
					action = 0;
					dispatchScrollWheelEvent( 0,
							( l_x - x ) / 2
									* (double) ( _saccell / 8192 / 8 + 1 ), 0,
							now );
					i_x = x;
					i_y = y;
					prec = 25;
				}

				if (( x1 == i_x1 && x2 - i_x2 > 1 )
						|| ( x1 < i_x1 && x2 > i_x2 )
						|| ( x1 - i_x1 < -1 && x2 == i_x1 ))  // zoom in
						{
					bool enb;
					_device->dispatchKeyboardMessage( 2, &enb );
					i_x1 = x1;
					i_x2 = x2;
				}
				if (( x1 == i_x1 && x2 - i_x2 < -1 )
						|| ( x1 > i_x1 && x2 < i_x2 )
						|| ( x1 - i_x1 > 1 && x2 == i_x1 ))  // zoom out
						{
					bool enb;
					_device->dispatchKeyboardMessage( 3, &enb );
					i_x1 = x1;
					i_x2 = x2;
				}
			}

			last_x1 = x1;
			last_y1 = y1;
			last_x2 = x2;
			last_y2 = y2;

			break; //2 fngers
		case 3:
			if (!action3)	// middle button
			{
				l_t = mach_absolute_time();
				if (action < 4)
					action = 4;
				i_x = x;
			}
			action3 = 1;

			if (last_fingers != 3) {
				i_x = x;
				i_y = y;
			} else {
				if (x - i_x < -250 && action == 4) // back
						{
					bool enb;
					_device->dispatchKeyboardMessage( 4, &enb );
					action = 0;
				}
				if (x - i_x > 250 && action == 4) // forward
						{
					bool enb;
					_device->dispatchKeyboardMessage( 5, &enb );
					action = 0;
				}
			}
			break;
	}
	l_x = x;
	l_y = y;
	last_fingers = fingers;
	return;
}





void ApplePS2ALPSMultiTouch::process_st(UInt8* packet, UInt32 packetSize)
{
#if APPLESDK
	clock_get_uptime(&now);
#else
	clock_get_uptime((uint64_t*)&now);
#endif
    int x, y;

    UInt32 buttons = (packet[3] & 0x03)==0x03 ? 0x04 : packet[3] & 0x03;
	if (_touchPadVersion==3)
	{	x = ((packet[1] & 0x7f) << 4) | ((packet[4] & 0x30) >> 2) | ((packet[0] & 0x30) >> 4);
		y = ((packet[2] & 0x7f) << 4) | (packet[4] & 0x0f);
	} else if (_touchPadVersion==6)
	{   x = (packet[1] & 0x7f) | ((packet[4] & 0x0f) << 7);
		y = (packet[2] & 0x7f) | ((packet[4] & 0xf0) << 3);
	}
	//	int z = packet[5] & 0x7f;

    if(!x && !y) // no touch / buttons only
    {   // tap to click
        if (_clicking && mach_absolute_time()-l_t < 100*1000*1000 && action)
        {   dispatchRelativePointerEvent(0, 0, action, now);
           // dispatchRelativePointerEvent(0, 0, 0, now);
            l_t=mach_absolute_time(); // drag
        }
        action = 0;
        action2 = 0;
        action3 = 0;
        hold=0;

        // buttons
        dispatchRelativePointerEvent(0, 0, buttons, now);
        last_x1 = x, last_y1 = y;
    }
    else if (x && y)
    {   prec = 25;

        // drag
        if (!action)
        {   if ( mach_absolute_time()-l_t < 150*1000*1000)
                {hold=1; i_x=x; i_y=y;}

            // tap to click
            if (action<1) action=1;
            l_t=mach_absolute_time();
        }

        int prec2=8;
        if (_dragging && hold && (i_x-x>prec2 || i_x-x<-prec2 || i_y-y>prec2 || i_y-y<-prec2) ) hold =2;
        if (_dragging && hold==2 ) buttons = 1;

        // move pointer
        if (x-last_x1<prec && x-last_x1>-prec && y-last_y1<prec && y-last_y1>-prec && _touchPadVersion==3)
            dispatchRelativePointerEvent( (x - last_x1)/3, (y - last_y1)/3, buttons, now);
        else if (x-last_x1<prec && x-last_x1>-prec && y-last_y1<prec && y-last_y1>-prec && _touchPadVersion==6)
            dispatchRelativePointerEvent( x - last_x1, y - last_y1, buttons, now);
        last_x1 = x, last_y1 = y;
    }
	return;
}

void ApplePS2ALPSGlidePoint::processTrackstickPacketV3(UInt8 *packet) {
    int x, y, z, left, right, middle;
    uint64_t now_abs;
    UInt32 buttons = 0, raw_buttons = 0;

    if (!(packet[0] & 0x40)) {
        DEBUG_LOG("ps2: bad trackstick packet, disregarding...\n");
        return;
    }

    /* There is a special packet that seems to indicate the end
     * of a stream of trackstick data. Filter these out
     */
    if (packet[1] == 0x7f && packet[2] == 0x7f && packet[3] == 0x7f) {
        DEBUG_LOG("ps2: ignoring trackstick packet that indicates end of stream\n");
        return;
    }

    x = (SInt8) (((packet[0] & 0x20) << 2) | (packet[1] & 0x7f));
    y = (SInt8) (((packet[0] & 0x10) << 3) | (packet[2] & 0x7f));
    z = (packet[4] & 0x7c) >> 2;

    // TODO: separate divisor for trackstick
    //    x /= divisorx;
    //    y /= divisory;

    clock_get_uptime(&now_abs);

    left = packet[3] & 0x01;
    right = packet[3] & 0x02;
    middle = packet[3] & 0x04;

    if (!(modelData.quirks & ALPS_QUIRK_TRACKSTICK_BUTTONS) &&
        (left || middle || right)) {
        modelData.quirks |= ALPS_QUIRK_TRACKSTICK_BUTTONS;
    }

    raw_buttons |= left ? 0x01 : 0;
    raw_buttons |= right ? 0x02 : 0;
    raw_buttons |= middle ? 0x04 : 0;

    // Reverse y value to get proper movement direction
    y = -y;

    // Sometimes, a big value can spit out, so we must remove it...
    if ((abs(x) >= 0x7f) && (abs(y) >= 0x7f)) {
        x = y = 0;
    }
    // Button status can appear in normal packet...
    if (0 == raw_buttons) {
        buttons = lastbuttons;
    } else {
        buttons = raw_buttons;
        lastbuttons = buttons;
    }

    lastx2 = x; lasty2 = y;

    ignoreall = FALSE;
    if ((0 != x) || (0 != y)) {
        ignoreall = TRUE;
    }

    // normal mode: middle button is not pressed or no movement made
    if ( ((0 == x) && (0 == y)) || (0 == (buttons & 0x04))) {
        y += y >> 1; x += x >> 1;
        DEBUG_LOG("ps2: trackStick: dispatch relative pointer with x=%d, y=%d, tbuttons = %d, buttons=%d, (z=%d, not reported)\n",
                  x, y, raw_buttons, buttons, z);
        dispatchRelativePointerEventX(x, y, buttons, now_abs);
    } else {
        // scroll mode
        y = -y; x = -x;
        DEBUG_LOG("ps2: trackStick: dispatchScrollWheelEventX: dv=%d, dh=%d\n", y, x);
        dispatchScrollWheelEventX(y, x, 0, now_abs);
    }
}

void ApplePS2ALPSGlidePoint::processTouchpadPacketV3(UInt8 *packet) {
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int fingers = 0, bmapFingers;
    UInt32 buttons = 0;
    uint64_t now_abs;
    struct alps_fields f;

    clock_get_uptime(&now_abs);

    (this->*decode_fields)(&f, packet);

    /*
     * There's no single feature of touchpad position and bitmap packets
     * that can be used to distinguish between them. We rely on the fact
     * that a bitmap packet should always follow a position packet with
     * bit 6 of packet[4] set.
     */
    if (modelData.multi_packet) {
        DEBUG_LOG("ps2: trackPad: handling multi-packet\n");
        /*
         * Sometimes a position packet will indicate a multi-packet
         * sequence, but then what follows is another position
         * packet. Check for this, and when it happens process the
         * position packet as usual.
         */
        if (f.is_mp) {
            fingers = f.fingers;
            bmapFingers = processBitmap(f.x_map, f.y_map, &x1, &y1, &x2, &y2);

            DEBUG_LOG("ps2: trackPad: fingers=%d, bmapFingers=%d\n", fingers, bmapFingers);

            /*
             * We shouldn't report more than one finger if
             * we don't have two coordinates.
             */
            if (fingers > 1 && bmapFingers < 2) {
                fingers = bmapFingers;
            }

            (this->*decode_fields)(&f, modelData.multi_data);
        } else {
            modelData.multi_packet = 0;
        }
    }

    /*
     * Bit 6 of byte 0 is not usually set in position packets. The only
     * times it seems to be set is in situations where the data is
     * suspect anyway, e.g. a palm resting flat on the touchpad. Given
     * this combined with the fact that this bit is useful for filtering
     * out misidentified bitmap packets, we reject anything with this
     * bit set.
     */
    if (f.is_mp) {
        return;
    }

    if (!modelData.multi_packet && (f.first_mp)) {
        DEBUG_LOG("ps2: trackPad: detected multi-packet first packet, waiting to handle\n");
        modelData.multi_packet = 1;
        memcpy(modelData.multi_data, packet, sizeof(modelData.multi_data));
        return;
    }

    modelData.multi_packet = 0;

    /*
     * Sometimes the hardware sends a single packet with z = 0
     * in the middle of a stream. Real releases generate packets
     * with x, y, and z all zero, so these seem to be flukes.
     * Ignore them.
     */
    if (f.x && f.y && !f.z) {
        return;
    }

    /*
     * If we don't have MT data or the bitmaps were empty, we have
     * to rely on ST data.
     */
    if (!fingers) {
        x1 = f.x;
        y1 = f.y;
        fingers = f.z > 0 ? 1 : 0;
    }

    buttons |= f.left ? 0x01 : 0;
    buttons |= f.right ? 0x02 : 0;
    buttons |= f.middle ? 0x04 : 0;

    if (!(modelData.quirks & ALPS_QUIRK_TRACKSTICK_BUTTONS)) {
        buttons |= f.ts_left ? 0x01 : 0;
        buttons |= f.ts_right ? 0x02 : 0;
        buttons |= f.ts_middle ? 0x04 : 0;
    }

    dispatchEventsWithInfo(f.x, f.y, f.z, fingers, buttons);
}

void ApplePS2ALPSGlidePoint::decodeButtonsV3(struct alps_fields *fields, UInt8 *packet) {
    fields->left = !!(packet[3] & 0x01);
    fields->right = !!(packet[3] & 0x02);
    fields->middle = !!(packet[3] & 0x04);

    fields->ts_left = !!(packet[3] & 0x10);
    fields->ts_right = !!(packet[3] & 0x20);
    fields->ts_middle = !!(packet[3] & 0x40);
}
