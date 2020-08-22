/* TO BE COMPILED WITH VISUAL STUDIO SO THAT EXPORTS ARE PROPERLY MANGLED */
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
extern "C" {
// This file is in the Windows DDK available from Microsoft.
#include <hidsdi.h>
}
#include <cstdint>
#include <fstream>
#include "device.h"

//#define DEBUG

HANDLE   g_hid_handle;
#ifdef DEBUG
FILE * g_logfile;
#endif

// settings
const char DEVICE_SUBBOARD_VERSION[] = "4.2.0:0";

// state
static bool DEVICE_INITIALIZED = false;
static int DEVICE_INPUT_STATE;

/**
 * Initialize the g_hid_handle global variable
 * It will attempt to read paths from devicepath.dat file if it exists until it finds one which works.
 * If nothing is found, as a last resort it will try the two default paths from the Due and Leonardo
 * versions of the firmware.
 *
 * @return 0 on success, -1 on error
 */
static int controller_init(){
    FILE *file;
    char  path[256];

    file = fopen("devicepath.dat", "r");
    if (file == NULL)
    {
        goto last_resort;
    }

    while ( fgets(path,256,file) != NULL )
    {
        path[strcspn(path, "\r\n")] = 0;
        g_hid_handle = CreateFile(path, GENERIC_READ|GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, 0, NULL);
        if ( g_hid_handle != INVALID_HANDLE_VALUE )
            break;
    }
    fclose(file);

    if ( g_hid_handle == INVALID_HANDLE_VALUE )
    {
        goto last_resort;
    }
    return 0;

    last_resort:
    g_hid_handle = CreateFile("\\\\?\\HID#VID_1D50&PID_6080#6&119e5a1b&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}",
                              GENERIC_READ|GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, 0, NULL);
    if ( g_hid_handle != INVALID_HANDLE_VALUE )
        return 0;
#ifdef DEBUG
    fprintf(g_logfile, "Couldn't open device. Make sure devicepath.dat exists and contains the correct path.\r\n");
#endif
    return -1;
}

/**
 * Read buttons state from the arduino and converts it into jubeat format
 *
 * jubeat processes button states as a int32 bitfield with the following format :
 * bit 0 (LSB) : nothing
 * bits 1 to 7 : buttons 9 to 15
 * bits 9 to 15 : buttons 1 to 7
 * bit 16 : button 16
 * bit 20 : button 8
 * bit 24 : coin mech
 * bit 25 : service
 * bit 28 : test
 *
 * older versions (knit, copious) use a different format, and uses 0 for active rather than 1 :
 * bits 0 (LSB) to 7 : nothing
 * bits 9 to 15 : buttons 9 to 15
 * bits 17 to 23 : buttons 1 to 7
 * bit 24 : button 16
 * bit 26 : button 8
 * bit 28 : test
 * bit 29 : coin mech
 * bit 30 : service
 */
static void controller_read_buttons(){
    DWORD	      BytesRead = 0;
    byte*         dis = (byte *) &DEVICE_INPUT_STATE;
    unsigned char buf[10]; // gamepad report length is 5 bytes in firmware, doubled because NumInputBuffer is set to 2

    buf[0] = 0x00; // gamepad report ID is 0 in firmware

    ReadFile(g_hid_handle, buf, 10, &BytesRead, NULL);
    // BytesRead should either be 10 (if it successfully read 2 reports) or 5 (only one)
    if ( BytesRead != 5 && BytesRead != 10)
    {
#ifdef DEBUG
        fprintf(g_logfile,"controller_read_buttons error (%u bytes read)\n",BytesRead);
#endif
        return;
    }
#ifdef DEBUG
    fprintf(g_logfile,"controller_read_buttons read %x %x %x %x %x\n",buf[0],buf[1],buf[2],buf[3],buf[4]);
#endif
    /* HID read ok, convert latest report bytes to pop'n bitfield */
    DEVICE_INPUT_STATE = 0;
    dis[0] = buf[BytesRead-4] << 1;
    dis[1] = buf[BytesRead-3] << 1;
    dis[2] = ((buf[BytesRead-3] >> 7)<<4)|(buf[BytesRead-4] >> 7);
    dis[3] = buf[BytesRead-2]|((buf[BytesRead-2]>>2)<<4);

#ifdef DEBUG
    for (int i = 0; i<32; i++){
        fprintf(g_logfile,"%c",( (DEVICE_INPUT_STATE>>(31-i))&1)?'1':'0');
        if (((i+1)%4)==0) fprintf(g_logfile," ");
    }
    fprintf(g_logfile,"\n");
    fprintf(g_logfile,"controller_read_buttons ok (pad_bits = %x)\n",DEVICE_INPUT_STATE);
#endif

#if OLD_FORMAT == 1
    /* convert to old format */
    dis[3] = (((dis[3] | (dis[3]<<4))&0xF0)|(dis[2]|dis[2]>>2));
    dis[2] = dis[1];
    dis[1] = dis[0];
    dis[0] = 0;
    DEVICE_INPUT_STATE = ~DEVICE_INPUT_STATE;
#endif
    return;
}

/* DEVICE EXPORTS */
extern "C" {

__declspec(dllexport) int __cdecl device_check_secplug(int a1) {
    // check for invalid index
    if (a1 > 1)
        return 0;
#if OLD_FORMAT == 1
        return 0x101 - a1;
#else
        return 0x100 + a1;
#endif
}

__declspec(dllexport) short __cdecl device_dispose_coinstock() {
    //eamuse_coin_consume_stock();
    return 0;
}

__declspec(dllexport) int __cdecl device_finalize(int a1, int a2) {
    return 0;
}

__declspec(dllexport) void __cdecl device_get_coinstock(unsigned short *coin1, unsigned short *coin2) {
    *coin1 = 0;//(unsigned short) eamuse_coin_get_stock();
    *coin2 = 0;

    // without this, jubeat will spawn never ending credit inserts
    //eamuse_coin_consume_stock();
}

__declspec(dllexport) char __cdecl device_get_dipsw() {
    return 0;
}

__declspec(dllexport) int *__cdecl device_get_jamma() {
    return &DEVICE_INPUT_STATE;
}

__declspec(dllexport) int __cdecl device_get_panel_trg_off(int a1, int a2, int a3) {
    return 0;
}

__declspec(dllexport) int __cdecl device_get_panel_trg_on(int a1, int a2, int a3) {
    return 0;
}

__declspec(dllexport) int __cdecl device_get_panel_trg_short_on(int a1, int a2, int a3) {
    return 0;
}

__declspec(dllexport) int __cdecl device_get_secplug_error(int a1) {
    return 0;
}

__declspec(dllexport) int __cdecl device_get_secplug_hist(int a1, int a2, char *a3) {
    *a3 = 0;
    return 0;
}

__declspec(dllexport) int __cdecl device_get_status() {
    return 0;
}

__declspec(dllexport) int __cdecl device_get_subboard_version(void *data, unsigned int size) {
    if (size < sizeof(DEVICE_SUBBOARD_VERSION)) {
        if (size>0)
            memset(data, 0, 1);
    } else {
        memcpy(data, DEVICE_SUBBOARD_VERSION, sizeof(DEVICE_SUBBOARD_VERSION));
    }
    return 0;
}

__declspec(dllexport) int __cdecl device_initialize(int a1, int a2) {
    if (controller_init() == 0)
        DEVICE_INITIALIZED = true;
    return 0;
}

__declspec(dllexport) bool __cdecl device_is_initialized() {
    return DEVICE_INITIALIZED;
}

__declspec(dllexport) int __cdecl device_set_coinblocker_open(char number, char open) {
    return 0;
}

__declspec(dllexport) int __cdecl device_set_coincounter_work() {
    return 0;
}

__declspec(dllexport) int __cdecl device_set_panel_mode(int mode) {
    return 0;
}

__declspec(dllexport) int __cdecl device_set_watchdog_timer(int a1) {
    return 0;
}

__declspec(dllexport) void __cdecl device_update() {
    controller_read_buttons();
}

__declspec(dllexport) void __cdecl device_update_secplug() {
}

}

int __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, void *) {
    if (fdwReason == 1) {
        //https://blogs.msdn.microsoft.com/larryosterman/2004/06/03/little-known-win32-apis-disablethreadlibrarycalls/
        DisableThreadLibraryCalls(hinstDLL);
    }

    return 1;
}