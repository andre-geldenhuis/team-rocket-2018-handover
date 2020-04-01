#include "armed.h"
#if IS_REMOTE

static void check_state_change(void)
{
    next_state();
    // check for next state()
}

void armed_loop(void)
{
	// debugger.println("Log:" + String(loop_cnt));
    check_state_change();

}
#endif