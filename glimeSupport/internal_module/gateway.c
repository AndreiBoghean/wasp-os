#include "py/obj.h"
#include "py/runtime.h"
#include "externApp.h"
#include "compat.h"

typedef struct {
	timer_interrupt_callback ticb;
	int period_ms;
	int waited_ms;
} timer_callback_registration_t;

touchCallback tcb;
touchCallback_xy tcb_xy;
timer_callback_registration_t ticb_registrations[4] = {{NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}};
mp_obj_t touch_handler(mp_obj_t py_eventID, mp_obj_t py_x, mp_obj_t py_y)
{
	int eventID = mp_obj_get_int(py_eventID);
	int x = mp_obj_get_int(py_x);
	int y = mp_obj_get_int(py_y);
	switch (eventID) {
		case -1:
    		// return mp_obj_new_str("unimpl event", strlen("unimpl event")); // create py string
		break;
		case 0:
			tcb(Tap);
			// tcb_xy(x, y);
		break;
		case 1:
			tcb(SwipeUp);
		break;
		case 2:
			tcb(SwipeDown);
		break;
		case 3:
			tcb(SwipeLeft);
		break;
		case 4:
			tcb(SwipeRight);
		break;
	}

	return mp_const_none;
}

mp_obj_t timer_handler(mp_obj_t py_current_delta)
{
	int current_delta = mp_obj_get_int(py_current_delta);
	// place_label("wut", 130, 10);
    // show_int(current_delta);
    // place_label("wat", 130, 55);
    // return mp_const_none;

	// for (timer_callback_registration_t* registration = &(ticb_registrations[0]) ; registration->ticb != NULL ; registration++)
	for (int i = 0 ; i < 4 ; i ++)
	{
		timer_callback_registration_t* registration = ticb_registrations+i;
		if (registration->ticb == NULL) continue;

		registration->waited_ms += current_delta;
		if (registration->waited_ms < registration->period_ms ) continue;

		registration->waited_ms = 0;
		registration->ticb();
	}

	return mp_const_none;
}

typedef struct {
	mp_obj_t delegator;
	mp_obj_t wrapApp;
	touchCallback* tcb;
	touchCallback_xy* tcb_xy;
	timer_callback_registration_t* ticb;
} setups_t;

STATIC mp_obj_t gateway_handle_main(mp_obj_t delegator, mp_obj_t wrapApp) {
	setups_t setups = {delegator, wrapApp, &tcb, &tcb_xy, ticb_registrations};
	init((void*) &setups);
	extern_main();

	return mp_const_none;
}

// bind our (1 argument) function name to a new "gateway_XMHlet_obj" python object
STATIC MP_DEFINE_CONST_FUN_OBJ_2(gateway_mainer_obj, gateway_handle_main);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(touch_handler_obj, touch_handler);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(timer_handler_obj, timer_handler);

// bind objects 4 stuff to python names 4 stuff
STATIC const mp_rom_map_elem_t gateway_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_gateway) },
    { MP_ROM_QSTR(MP_QSTR_handle_main), MP_ROM_PTR(&gateway_mainer_obj) },
    { MP_ROM_QSTR(MP_QSTR_touch_handler), MP_ROM_PTR(&touch_handler_obj) },
    { MP_ROM_QSTR(MP_QSTR_timer_handler), MP_ROM_PTR(&timer_handler_obj) },
};


STATIC MP_DEFINE_CONST_DICT(gateway_module_globals, gateway_module_globals_table);

const mp_obj_module_t gateway_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&gateway_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_gateway, gateway_user_cmodule, MODULE_GATEWAY_ENABLED);
