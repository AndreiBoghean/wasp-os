# SPDX-License-Identifier: MY-LICENSE
# Copyright (C) YEAR(S), AUTHOR

import wasp
import gateway
import ppg

import binascii


def rgb888_to_rgb565(red8, green8, blue8):
    # Convert 8-bit red to 5-bit red.
    red5 = round(red8 / 255 * 31)
    # Convert 8-bit green to 6-bit green.
    green6 = round(green8 / 255 * 63)
    # Convert 8-bit blue to 5-bit blue.
    blue5 = round(blue8 / 255 * 31)

    # Shift the red value to the left by 11 bits.
    red5_shifted = red5 << 11
    # Shift the green value to the left by 5 bits.
    green6_shifted = green6 << 5

    # Combine the red, green, and blue values.
    rgb565 = red5_shifted | green6_shifted | blue5

    return rgb565

def delegate(*_args):
    operation = _args[0]
    args = _args[1:]
    print("delag called for", operation, "with args: '" + str(args) + "'")

    if operation == "show_int":
        wasp.watch.drawable.string("Sh:" + str(args[0]), 0, 108, width=240)
    elif operation == "set_colours":
        parsed_fg = rgb888_to_rgb565(args[0] >> 8*2, (args[0] >> 8) & 0xFF, args[0] & 0xFF)
        parsed_bg = rgb888_to_rgb565(args[1] >> 8*2, (args[1] >> 8) & 0xFF, args[1] & 0xFF)
        print("converted fg", args[0], "->", parsed_fg, "and bg", args[1], "->", parsed_bg)
        wasp.watch.drawable.set_color(parsed_fg, parsed_bg)
    elif operation == "place_label":
        wasp.watch.drawable.string(str(args[0]), args[1], args[2])
    elif operation == "set_brightness":
        print("setting brightness to", str(args[0]+1))
        wasp.system.brightness = args[0]+1
    elif operation == "clear_screen":
        print("clearing screen... colours are", wasp.watch.drawable._bgfg, "(", hex(wasp.watch.drawable._bgfg), ")")
        wasp.watch.drawable.fill()
    elif operation == "draw_rect":
        w = args[2] - args[0] + 1
        h = args[3] - args[1] + 1
        tempCol = bg = wasp.watch.drawable._bgfg & 0xFFFF # just the foreground colour.
        wasp.watch.drawable.fill(bg=tempCol, x=args[0], y=args[1], w=w, h=h)
    elif operation == "register_timer_interrupt":
        current_delta = args[0]
        print("registering timer for", current_delta)
        wasp.system.request_tick(current_delta)

    elif operation == "start_read_hr":
        wp = args[0]
        wasp.watch.hrs.enable()
        wp._hrdata = ppg.PPG(wasp.watch.hrs.read_hrs())
    elif operation == "stop_read_hr":
        wp = args[0]
        wp._hrdata = None
    elif operation == "get_hr_bpm":
        wp = args[0]
        if len(wp._hrdata.data) < 240: return 0
        val = wp._hrdata.get_heart_rate()
        if val == None: val = 0
        return int(val)
    elif operation == "ping_hr_data":
        wp = args[0]
        # delegate("show_int", len(wp._hrdata.data)) # uncomment to see progress out of 240 on hr measuring
        spl = wp._hrdata.preprocess(wasp.watch.hrs.read_hrs())


class WrapApp():
    NAME = "Wrap"

    def __init__(self):
        self.current_delta = 500
        self._hrdata = None

    def foreground(self):
        self._draw()
        wasp.system.request_event(wasp.EventMask.TOUCH | wasp.EventMask.SWIPE_LEFTRIGHT | wasp.EventMask.SWIPE_UPDOWN)

    def _draw(self):
        draw = wasp.watch.drawable
        draw.fill()
        # test(draw)
        # test2(draw)

        # external.exern_main_handler(lambda : draw.string("Hello, world!", 0, 108, width=240) )
        # gateway.handle_main(draw.string)
        gateway.handle_main(delegate, self)

    
    def touch(self, event):
        # print("we've been touched", "|"+event[1]+"|", "|"+event[2]+"|")
        print("we've been touched", str(event))
        # print("0:", event[0])
        # print("0:", type(event[0]))
        # print("1:", event[1])
        # print("1:", type(event[1]))
        # print("2:", event[2])
        # print("2:", type(event[2]))
        gateway.touch_handler(0, int(event[1]), int(event[2]))
        
        wasp.watch.touch.reset_touch_data()


    def swipe(self, event):
        print("we've been swiped")
        # get an ID representation for the tocuh event, which we pass to our function,
        # which converts it to a C enum and then gives it to the user-provided event listener.
        gateway.touch_handler({wasp.EventType.UP: 1, wasp.EventType.DOWN: 2, wasp.EventType.LEFT: 3, wasp.EventType.RIGHT: 4}.get(event[0], -1), -1, -1)

    def tick(self, ticks):
        wasp.system.keep_awake()
        print("ticked with delta", wasp.system.tick_period_ms)
        gateway.timer_handler(wasp.system.tick_period_ms)
