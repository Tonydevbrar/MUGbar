//#
//# Ready to compile program
//# for more universal approach compiling code was exported to Makefile
//#

#define _POSIX_C_SOURCE 200809L
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <sys/utsname.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

int panel_scale = 20;
char bg_hex[8] = "#000000";
char accent_hex[8] = "#ff00ff";
char term_hlink[512] = "st &";
char edit_hlink[512] = "st -e vi ~/.config/coffeesys/mugbar.lua &";
char volu_hlink[512] = "pavucontrol &";
char menu_hlink[512] = "jgmenu_run &";
char config_dir[512] = {0};
char config_path[512] = {0};
char reload_trigger_char[4] = "t";
static const uint16_t term[16] ={
    0b0111111111111110,
    0b1111111111111111,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000000000001,
    0b1001000000000001,
    0b1000100000000001,
    0b1000010000000001,
    0b1000010000000001,
    0b1000100000000001,
    0b1001000111111001,
    0b1000000000000001,
    0b1000000000000001,
    0b1111111111111111,
    0b0000000000000000
};
static const uint16_t menu[16] ={
    0b0111111111111110,
    0b1111111111111111,
    0b1000000000000001,
    0b1000000000000001,
    0b1001110110111001,
    0b1011101111011101,
    0b1011101111011101,
    0b1011101111011101,
    0b1000000000000001,
    0b1011101111011101,
    0b1011101111011101,
    0b1011101111011101,
    0b1000000000000001,
    0b1000000000000001,
    0b1111111111111111,
    0b0000000000000000
};
static const uint16_t inet[16] ={
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0001110000010000,
    0b0001110000111000,
    0b0001110001111100,
    0b0001110011111110,
    0b0001110000111000,
    0b0001110000111000,
    0b0111111100111000,
    0b0011111000111000,
    0b0001110000111000,
    0b0000100000111000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000
};
static const uint16_t net[16] ={
    0b0000000000000000,
    0b0000000000000000,
    0b0001110000010000,
    0b0010001000101000,
    0b0010001001000100,
    0b0010001010000010,
    0b0010001100000001,
    0b0010001011000110,
    0b0110001101000100,
    0b1000000011000100,
    0b0100000101000100,
    0b0010001001000100,
    0b0001010001000100,
    0b0000100000111000,
    0b0000000000000000,
    0b0000000000000000
};
static const uint16_t not[16] ={
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0011100000011100,
    0b0011110000111100,
    0b0001111001111000,
    0b0111110110111110,
    0b0111110110111110,
    0b0001111001111000,
    0b0011110000111100,
    0b0011100000011100,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000
};
static const uint16_t params[16] ={
    0b0111111111111110,
    0b1111111111111111,
    0b1000000000000001,
    0b1000000000000001,
    0b1000000001000001,
    0b1011111110100001,
    0b1000000001000001,
    0b1000010000000001,
    0b1011101000000001,
    0b1000010000000001,
    0b1000000000001001,
    0b1011111111110101,
    0b1000000000001001,
    0b1000000000000001,
    0b1111111111111111,
    0b0000000000000000
};
static const uint16_t audio[16] ={
    0b0111111111111110,
    0b1111111111111111,
    0b1000000000000001,
    0b1000101111010001,
    0b1001010000101001,
    0b1010100000000001,
    0b1001000110001101,
    0b1001001111001111,
    0b1001001111001001,
    0b1001000110001001,
    0b1010100000001001,
    0b1001010001111001,
    0b1000101101110001,
    0b1000000000000001,
    0b1111111111111111,
    0b0000000000000000
};

void draw_scalable_binary_icon(XftDraw *xft_draw, XftColor *color, const uint16_t *icon_data, int dst_x, int dst_y, int target_size){
    if (target_size <= 0) return;
    double pixel_scale = (double)target_size / 16;
    for (int row = 0; row < 16; row++) {
        uint16_t line_bits = icon_data[row];
        for (int col = 0; col < 16; col++) {
            uint16_t mask = (1 << (16 - 1 - col));
            if (line_bits & mask) {
                int x_start = dst_x + (int)(col * pixel_scale);
                int y_start = dst_y + (int)(row * pixel_scale);
                int x_end = dst_x + (int)((col + 1) * pixel_scale);
                int y_end = dst_y + (int)((row + 1) * pixel_scale);
                int block_width = x_end - x_start;
                int block_height = y_end - y_start;
                if (block_width <= 0)  block_width = 1;
                if (block_height <= 0) block_height = 1;
                XftDrawRect(xft_draw, color, x_start, y_start, block_width, block_height);}}}}

void makecfg(){
    mkdir(config_dir, S_IRWXU);
    if (access(config_path, F_OK) != 0) {
        FILE *default_cfg = fopen(config_path, "w");
        if (default_cfg != NULL) {
            fprintf(default_cfg, "-- ~/.config/coffeesys/mugbar.lua\n");
            fprintf(default_cfg, "palette = {background = \"#101010\", accent     = \"#33b5e5\"}\n");
            fprintf(default_cfg, "elements = {panel_size = \"20\"}\n");
            fprintf(default_cfg, "keys = {reload = \"r\"}\n");
            fprintf(default_cfg, "icons = {term = \"st &\",menu = \"jgmenu_run &\", param = \"st -e vi ~/.config/coffeesys/mugbar.lua &\", vol = \"pavucontrol &\"}");
            fprintf(default_cfg, "-- After saving changes press reload key to apply them");
            fclose(default_cfg);
            printf("mugbar: Created default configuration file at %s\n", config_path);
}}}

void readcfg() {
    lua_State *L = luaL_newstate();
    if (L != NULL) {
        luaL_openlibs(L);
        if (luaL_dofile(L, config_path) == LUA_OK) {
            lua_getglobal(L, "palette");
            if (lua_istable(L, -1)) {
                lua_getfield(L, -1, "background");
                if (lua_isstring(L, -1)) {
                    strncpy(bg_hex, lua_tostring(L, -1), sizeof(bg_hex) - 1);}
                lua_pop(L, 1);
                lua_getfield(L, -1, "accent");
                if (lua_isstring(L, -1)) {
                    strncpy(accent_hex, lua_tostring(L, -1), sizeof(accent_hex) - 1);}
                lua_pop(L, 1);
                lua_pop(L, 1);}
            else {lua_pop(L, 1);}
            lua_getglobal(L, "elements");
            if (lua_istable(L, -1)) {
                lua_getfield(L, -1, "panel_size");
                if (lua_isstring(L, -1)) {
                    panel_scale = atoi(lua_tostring(L, -1));}
                lua_pop(L, 1);
                lua_pop(L, 1);}
            else {lua_pop(L, 1);}
            lua_getglobal(L, "keys");
            if (lua_istable(L, -1)) {
                lua_getfield(L, -1, "reload");
                if (lua_isstring(L, -1)) {
                    strncpy(reload_trigger_char, lua_tostring(L, -1), sizeof(reload_trigger_char) - 1);}
                lua_pop(L, 1);
                lua_pop(L, 1);}
            else {lua_pop(L, 1);}
            lua_getglobal(L, "icons");
            if (lua_istable(L, -1)) {
                lua_getfield(L, -1, "term");
                if (lua_isstring(L, -1)) {
                    strncpy(term_hlink, lua_tostring(L, -1), sizeof(term_hlink) - 1);}
                lua_pop(L, 1);
                lua_getfield(L, -1, "menu");
                if (lua_isstring(L, -1)) {
                    strncpy(menu_hlink, lua_tostring(L, -1), sizeof(menu_hlink) - 1);}
                lua_pop(L, 1);
                lua_getfield(L, -1, "param");
                if (lua_isstring(L, -1)) {
                    strncpy(edit_hlink, lua_tostring(L, -1), sizeof(edit_hlink) - 1);}
                lua_pop(L, 1);
                lua_getfield(L, -1, "vol");
                if (lua_isstring(L, -1)) {
                    strncpy(volu_hlink, lua_tostring(L, -1), sizeof(volu_hlink) - 1);}
                lua_pop(L, 1);
                lua_pop(L, 1);}
                else {lua_pop(L, 1);}}
        printf("Mugbar parsed config successfully. Accent = %s, background = %s, scale = %d, reload key = %s, terminal = %s, parameters = %s, volume = %s\n", accent_hex, bg_hex, panel_scale, reload_trigger_char, term_hlink, edit_hlink, volu_hlink);
        lua_close(L);}}

int main() {
    char *home = getenv("HOME");
    if (home == NULL) {fprintf(stderr,"Couldnt start, variable $HOME isnt available");exit(1);}
    snprintf(config_dir, sizeof(config_dir), "%s/.config/coffeesys", home);
    snprintf(config_path, sizeof(config_path), "%s/.config/coffeesys/mugbar.lua", home);
    struct utsname buffer;
    char system_string[256];
    if (uname(&buffer) != 0) {
        perror("uname");
        return 1;}
    //# Test for display connection
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Couldn't connect to display\n");
        exit(1);}
    int screen_num = DefaultScreen(display);
    Visual *visual = DefaultVisual(display, screen_num);
    Colormap cmap = DefaultColormap(display, screen_num);
    //# Create file and directory if it doesn't exist, initialize and run the standard embedded Lua runtime state parser
    makecfg(); readcfg();
    //# Colors
    XftColor background_color, accent_color, gray;
    XftColorAllocName(display, visual, cmap, bg_hex, &background_color);
    XftColorAllocName(display, visual, cmap, accent_hex, &accent_color);
    Screen* screen_size = DefaultScreenOfDisplay(display);
    int swidth = screen_size->width;
    int window_xpos = 0, window_ypos = 0;
    Window mugbar = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, None);
    XStoreName(display, mugbar, "Mugbar");
    XSelectInput(display, mugbar, ExposureMask | KeyPressMask);
    XSetWindowBackgroundPixmap(display, mugbar, None);
    XMapWindow(display, mugbar);
    XFlush(display);
    GC gc = XCreateGC(display, mugbar, 0, NULL);
    XWindowAttributes gwa;
    XGetWindowAttributes(display, mugbar, &gwa);
    //# Font and its fallback
    int font_size = panel_scale - 4;
    if (font_size < 8) font_size = 8;
    char font_desc[128];
    snprintf(font_desc, sizeof(font_desc), "Roboto:size=%d:antialias=true:bold", font_size);
    XftFont *font1 = XftFontOpenName(display, screen_num, font_desc);
    if (!font1) {
        snprintf(font_desc, sizeof(font_desc), "System-ui:size=%d:antialias=true", font_size);
        font1 = XftFontOpenName(display, screen_num, font_desc);
    }
    //# Setting type to dock
    long hints[5] = {2, 0, 0, 0, 0};
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom wm_above = XInternAtom(display, "_NET_WM_STATE_BELOW", False);
    Atom wm_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    Atom type_dock = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
    XChangeProperty(display, mugbar, wm_type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&type_dock, 1);
    XEvent xev;
    xev.type = ClientMessage;   
    xev.xclient.type = ClientMessage;
    xev.xclient.window = mugbar;    
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;    
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = wm_above;   
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 0;  
    xev.xclient.data.l[4] = 0;
    XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    //# Extra, hide window controls
    Atom motif_hints = XInternAtom(display, "_MOTIF_WM_HINTS", False);
    XChangeProperty(display, mugbar, motif_hints, motif_hints, 32, PropModeReplace, (unsigned char *)hints, 5);
    //# move and show window
    XMoveResizeWindow(display, mugbar, window_xpos, window_ypos, swidth, panel_scale);
    XMapWindow(display, mugbar);
    XftDraw *draw = XftDrawCreate(display, mugbar, visual, cmap);
    Cursor cursor = XCreateFontCursor(display, XC_arrow);
    XftColorAllocName(display, visual, cmap, "DimGray", &gray);
    XDefineCursor(display, mugbar, cursor);
    XFreeCursor(display, cursor);
    unsigned long strut[12] = {
        0, 0,           // left, right
        panel_scale, 0, // top, bottom
        0, 0,           // left_start_y, left_end_y
        0, 0,           // right_start_y, right_end_y
        0, swidth - 1,  // top_start_x, top_end_x
        0, 0};          // bottom_start_x, bottom_end_x
    Atom strut_atom = XInternAtom(display, "_NET_WM_STRUT", False);
    Atom partial_strut_atom = XInternAtom(display, "_NET_WM_STRUT_PARTIAL", False);
    XChangeProperty(display, mugbar, strut_atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)strut, 4);
    XChangeProperty(display, mugbar, partial_strut_atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)strut, 12);
    int osinfo = snprintf(system_string, sizeof(system_string), "%s %s %s", buffer.sysname, buffer.release, buffer.machine);
    XSetWindowAttributes wa;
    wa.override_redirect = True;
    wa.background_pixmap = ParentRelative;
    wa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask;
    XChangeWindowAttributes(display, mugbar, CWOverrideRedirect | CWBackPixmap | CWEventMask, &wa);
    time_t t;
    struct tm *tmp;
    char time_str[9];
    int ping_counter = 0;
    bool has_internet = false;
    has_internet = (system("ping -c 1 -w 1 8.8.8.8 > /dev/null 2>&1") == 0);
    bool has_network = false;
    has_network = (system("ping -c 1 -w 1 192.168.1.1 > /dev/null 2>&1") == 0);
    ping_counter = 1;
    while (1) {
        while (XPending(display)){
            XEvent event;
            XNextEvent(display, &event);
        if (event.type == KeyPress){char key_buf[8] = {0}; 
            XLookupString(&event.xkey, key_buf, sizeof(key_buf) - 1, NULL, NULL);
            if (key_buf[0] == reload_trigger_char[0]) {
                printf("Reloading Mugbar config...\n");
                readcfg();
                XftColorFree(display, visual, cmap, &background_color);
                XftColorFree(display, visual, cmap, &accent_color);
                XftColorAllocName(display, visual, cmap, bg_hex, &background_color);
                XftColorAllocName(display, visual, cmap, accent_hex, &accent_color);
                XMoveResizeWindow(display, mugbar, window_xpos, window_ypos, swidth, panel_scale);
                font_size = panel_scale - 4;
                if (font_size < 8) font_size = 8;
                XftFontClose(display, font1);
                snprintf(font_desc, sizeof(font_desc), "Roboto:size=%d:antialias=true:bold", font_size);
                font1 = XftFontOpenName(display, screen_num, font_desc);
                if (!font1) {
                    snprintf(font_desc, sizeof(font_desc), "System-ui:size=%d:antialias=true", font_size);
                    font1 = XftFontOpenName(display, screen_num, font_desc);}}}
        if (event.type == ButtonPress) {
            int click_x = event.xbutton.x;
            if (click_x >= 0 && click_x <= (panel_scale+4)) {
                if (fork() == 0) {
                    setsid();
                    system(term_hlink); 
                    _exit(0);}}
            if (click_x >= (swidth-panel_scale)-4 && click_x <= (swidth)) {
                    printf("DEBUG: Попытка запустить команду из конфига: [%s]\n", edit_hlink);
                if (fork() == 0) {
                    setsid();
                    system(edit_hlink); 
                    _exit(0);}}
            if (click_x >= swidth-(panel_scale*5)-4 && click_x <= swidth-(panel_scale*4)-4) {
                    printf("DEBUG: Попытка запустить команду из конфига: [%s]\n", volu_hlink);
                if (fork() == 0) {
                    setsid();
                    system(volu_hlink); 
                    _exit(0);}}
            if (click_x >= 4+(panel_scale*1) && click_x <= 4+(panel_scale*2)) {
                    printf("DEBUG: Попытка запустить команду из конфига: [%s]\n", menu_hlink);
                if (fork() == 0) {
                    setsid();
                    system(menu_hlink); 
                    _exit(0);}}}}
        XGetWindowAttributes(display, mugbar, &gwa);
        //# Get time
        time(&t);
        tmp = localtime(&t);
        strftime(time_str, sizeof(time_str), "%H:%M", tmp);
        XClearWindow(display, mugbar);
        XSetForeground(display, gc, background_color.pixel);
        XFillRectangle(display, mugbar, gc, 0, 0, swidth-1, panel_scale);
        XSetForeground(display, gc, accent_color.pixel);
        XFillRectangle(display, mugbar, gc, 2, panel_scale-1, swidth-3, panel_scale);
        draw_scalable_binary_icon(draw, &accent_color, term, 4, 2, panel_scale-4);
        draw_scalable_binary_icon(draw, &accent_color, menu, 4+panel_scale, 2, panel_scale-4);
        draw_scalable_binary_icon(draw, &accent_color, params, swidth-panel_scale, 2, panel_scale-4);
        draw_scalable_binary_icon(draw, &accent_color, audio, swidth-(panel_scale*5)-4, 2, panel_scale-4);
        XftDrawString8(draw, &accent_color, font1, (panel_scale*2)+4, panel_scale*0.8, (unsigned char *)system_string, osinfo);
        //# Net check
        if (ping_counter == 0) has_internet = (system("ping -c 1 -w 1 8.8.8.8 > /dev/null 2>&1") == 0);
        if (ping_counter == 0) has_network = (system("ping -c 1 -w 1 192.168.1.1 > /dev/null 2>&1") == 0);
        ping_counter = (ping_counter + 1) % 60;
        if (has_internet) draw_scalable_binary_icon(draw, &accent_color, inet, (swidth-(panel_scale*4))-4, 2, panel_scale-4);
        else if (has_network) draw_scalable_binary_icon(draw, &accent_color, net, (swidth-(panel_scale*4))-4, 2, panel_scale-4);
        else {draw_scalable_binary_icon(draw, &accent_color, not, (swidth-(panel_scale*4))-4, 2, panel_scale-4);}
        //# Show time
        XftDrawString8(draw, &accent_color, font1, (swidth-(panel_scale*3))-6, panel_scale*0.8, (XftChar8 *)time_str, strlen(time_str));
        XFlush(display);
        struct timespec delay;
        delay.tv_sec = 0;
        delay.tv_nsec = 500000000;
        nanosleep(&delay , NULL);}
    XftFontClose(display, font1);
    XftDrawDestroy(draw);
    XDestroyWindow(display, mugbar);
    XCloseDisplay(display);
    return 0;}
