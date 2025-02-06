#include "compat.h"
#include "py/runtime.h"
#include "py/misc.h" // memory allocation is in misc for some reason lol
#include "string.h"

typedef struct {
	timer_interrupt_callback ticb;
	int period_ms;
	int waited_ms;
} timer_callback_registration_t;

STATIC mp_obj_t delegate;
STATIC mp_obj_t wrapApp;
touchCallback* tcb_pointer;
touchCallback_xy* tcb_xy_pointer;
timer_callback_registration_t* ticb_regs;


typedef struct {
	mp_obj_t delegator;
	mp_obj_t wrapApp;
	touchCallback* tcb_pointer;
	touchCallback_xy* tcb_xy_pointer;
	timer_callback_registration_t* ticb_regs;
} setups_t;

void init(void* _setupts)
{
	setups_t* setups = (setups_t*) _setupts;
	delegate = setups->delegator;
	wrapApp = setups->wrapApp;
	tcb_pointer = setups->tcb_pointer;
	tcb_xy_pointer = setups->tcb_xy_pointer;
	ticb_regs = setups->ticb_regs ;
}

void make_label(char *text)
{
	if (text)
		return;
}

void show_int(int i)
{
    mp_obj_t* instr = mp_obj_new_str("show_int", strlen("show_int")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 2);
	args[0] = instr;
	args[1] = mp_obj_new_int(i); // options;

    mp_call_function_n_kw(delegate, 2, 0, args);
    m_del(args, args, 2); // delete args object
}

// places a label centrered at x, y.
void place_label(const char* text, int x, int y)
{
    mp_obj_t* instr = mp_obj_new_str("place_label", strlen("place_label")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 4);
	args[0] = instr;
    args[1]  = mp_obj_new_str(text, strlen(text));
	args[2] = mp_obj_new_int(x);
	args[3] = mp_obj_new_int(y);

    mp_call_function_n_kw(delegate, 4, 0, args);
    m_del(args, args, 4); // delete args object
}

void set_colours(uint32_t fg, uint32_t bg)
{
    mp_obj_t* instr = mp_obj_new_str("set_colours", strlen("set_colours")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 3);
	args[0] = instr;
	args[1] = mp_obj_new_int(fg);
	args[2] = mp_obj_new_int(bg);

    mp_call_function_n_kw(delegate, 3, 0, args);
    m_del(args, args, 3); // delete args object
}
// set the foreground and background colours for drawing.

void set_brightness(enum brightness_level bl)
{
    mp_obj_t* instr = mp_obj_new_str("set_brightness", strlen("set_brightness")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 2);
	args[0] = instr;
	args[1] = mp_obj_new_int(bl);

    mp_call_function_n_kw(delegate, 2, 0, args);
    m_del(args, args, 2); // delete args object
}

void register_global_eventListener(touchCallback e)
{
  *tcb_pointer = e;
}

void register_global_eventListener_xy(touchCallback_xy e)
{
  *tcb_xy_pointer = e;
}
// ^ callback returns a boolean according to whether it actioned on the event or not
// if it DID action, then we finalise that "event" and move on to detecting the next one.
// i.e. we use the return to decide whether to call lvgl.CancelTap();

int gcd(int a, int b)
{
	while (b != 0)
	{
		int _a = a;
		a = b;
		b = _a % b;
	}
	return a;
}

void register_timer_interrupt(timer_interrupt_callback action, int period_ms)
{
	timer_callback_registration_t registration = {action, period_ms, 0};

	if ((ticb_regs->ticb) == NULL) ticb_regs[0] = registration;
	else if ((ticb_regs+1)->ticb == NULL) ticb_regs[1] = registration;
	else if ((ticb_regs+2)->ticb == NULL) ticb_regs[2] = registration;
	else if ((ticb_regs+3)->ticb == NULL) ticb_regs[3] = registration;

	int smallest_delta = ticb_regs[0].period_ms;
	for (timer_callback_registration_t* ticb_reg = ticb_regs+1 ; ticb_reg->ticb != NULL ; ticb_reg++) smallest_delta = gcd(smallest_delta, ticb_reg->period_ms);

	// note that we still need to delegate to the python implementation, because it needs to look through and re-register the interrupt if it needs to pick a new period to go faster/slower.
    mp_obj_t* instr = mp_obj_new_str("register_timer_interrupt", strlen("register_timer_interrupt")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 2);
	args[0] = instr;
	args[1] = mp_obj_new_int(smallest_delta);
	// ^ dont need to send the callback function, because we've already communicated it by adding it to ticb
	// we also dont need to send the period_ms for the same reason, but do need to communicate what the updated period should be for the "master" timer.


    mp_call_function_n_kw(delegate, 2, 0, args);
    m_del(args, args, 2); // delete args object
}

void disable_sleep()
{

}

void clear_screen()
{
    mp_obj_t* instr = mp_obj_new_str("clear_screen", strlen("clear_screen")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 1);
	args[0] = instr;

    mp_call_function_n_kw(delegate, 1, 0, args);
    m_del(args, args, 1); // delete args object
}

void draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    mp_obj_t* instr = mp_obj_new_str("draw_rect", strlen("draw_rect")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 5);
	args[0] = instr;
	args[1] = mp_obj_new_int(x1);
	args[2] = mp_obj_new_int(y1);
	args[3] = mp_obj_new_int(x2);
	args[4] = mp_obj_new_int(y2);


    mp_call_function_n_kw(delegate, 5, 0, args);
    m_del(args, args, 5); // delete args object
}

void hr_poller()
{
    mp_obj_t* instr = mp_obj_new_str("ping_hr_data", strlen("ping_hr_data")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 2);
	args[0] = instr;
	args[1] = wrapApp;

    mp_call_function_n_kw(delegate, 2, 0, args);
    m_del(args, args, 2); // delete args object
}
void start_read_hr()
{
    mp_obj_t* instr = mp_obj_new_str("start_read_hr", strlen("start_read_hr")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 2);
	args[0] = instr;
	args[1] = wrapApp;

    mp_call_function_n_kw(delegate, 2, 0, args);
    m_del(args, args, 2); // delete args object
	
	register_timer_interrupt(hr_poller, 125);
}
void stop_read_hr()
{
    mp_obj_t* instr = mp_obj_new_str("stop_read_hr", strlen("stop_read_hr")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 2);
	args[0] = instr;
	args[1] = wrapApp;

    mp_call_function_n_kw(delegate, 2, 0, args);
    m_del(args, args, 2); // delete args object
}
int get_hr_bpm()
{
    mp_obj_t* instr = mp_obj_new_str("get_hr_bpm", strlen("get_hr_bpm")); // create py string

	mp_obj_t* args = m_new(mp_obj_t, 2);
	args[0] = instr;
	args[1] = wrapApp;

    mp_obj_t hr_py = mp_call_function_n_kw(delegate, 2, 0, args);
    m_del(args, args, 2); // delete args object
	
	return mp_obj_get_int(hr_py);
}
