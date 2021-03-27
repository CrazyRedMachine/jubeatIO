#ifndef JUBEATIO_DEVICE_H
#define JUBEATIO_DEVICE_H

/* Jubeat Knit/Copious has a couple differences, uncomment this define to make the dll compatible with these games instead */
//#define OLD_FORMAT

extern "C" {
__declspec(dllexport) int __cdecl device_check_secplug(int a1);
__declspec(dllexport) short __cdecl device_dispose_coinstock();
__declspec(dllexport) int __cdecl device_finalize(int a1, int a2);
__declspec(dllexport) void __cdecl device_get_coinstock(unsigned short *coin1, unsigned short *coin2);
__declspec(dllexport) void __cdecl device_get_coinstock_all(unsigned short *coin1, unsigned short *coin2);
__declspec(dllexport) long __cdecl device_get_input(int a1);
__declspec(dllexport) long __cdecl device_get_input_time();
__declspec(dllexport) char __cdecl device_get_dipsw();
__declspec(dllexport) int *__cdecl device_get_jamma();
__declspec(dllexport) int __cdecl device_get_panel_trg_off(int a1, int a2, int a3);
__declspec(dllexport) int __cdecl device_get_panel_trg_on(int a1, int a2, int a3);
__declspec(dllexport) int __cdecl device_get_panel_trg_short_on(int a1, int a2, int a3);
__declspec(dllexport) int __cdecl device_get_racecount();
__declspec(dllexport) int __cdecl device_get_secplug_error(int a1);
__declspec(dllexport) int __cdecl device_get_secplug_hist(int a1, int a2, char *a3);
__declspec(dllexport) void __cdecl device_poweroff();
__declspec(dllexport) void __cdecl device_set_mute();
__declspec(dllexport) int __cdecl device_get_status();
__declspec(dllexport) int __cdecl device_get_subboard_version(void *data, unsigned int size);
__declspec(dllexport) int __cdecl device_initialize(int a1, int a2);
__declspec(dllexport) bool __cdecl device_is_initialized();
__declspec(dllexport) int __cdecl device_set_coinblocker_open(char number, char open);
__declspec(dllexport) void __cdecl device_set_coincounter_merge();
__declspec(dllexport) void __cdecl device_set_coincounter_controllable();
__declspec(dllexport) int __cdecl device_set_coincounter_work();
__declspec(dllexport) void __cdecl device_set_portoutbit();
__declspec(dllexport) int __cdecl device_set_panel_mode(int mode);
__declspec(dllexport) int __cdecl device_set_watchdog_timer(int a1);
__declspec(dllexport) void __cdecl device_update();
__declspec(dllexport) void __cdecl device_update_secplug();
}

#endif //JUBEATIO_DEVICE_H