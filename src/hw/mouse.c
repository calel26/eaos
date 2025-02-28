#include "mouse.h"
#include "eaos.h"
#include "io.h"
#include "irq.h"
#include "log.h"
#include "mem/util.h"
#include "ui/mouse.h"

#define MOUSE_PIC 12
#define LEFT_CLICK 1 << 0
#define RIGHT_CLICK 1 << 1
#define MIDDLE_CLICK 1 << 2
#define SYNCHRONIZE 1 << 3
#define X_NEG 1 << 3
#define Y_NEG 1 << 4
#define X_OFLOW 1 << 6
#define Y_OFLOW 1 << 7

#define PORT_PS2_CMD 0x64
#define PORT_PS2_DATA 0x60

INTR(mouse_handle);

static bool mouse_configured = false;

void mouse_init(void) {
    kinfo("setting up the mouse...");

    // Enable the second PS/2 port
    outb(PORT_PS2_CMD, 0xA8);
    io_wait();
    
    // Enable IRQ 12 (mouse interrupt)
    outb(PORT_PS2_CMD, 0x20); // Read current config byte
    io_wait();
    uint8_t status = inb(PORT_PS2_DATA);
    io_wait();
    
    status |= 0x02; // Set bit 1 to enable IRQ12
    
    outb(PORT_PS2_CMD, 0x60); // Write updated config byte
    io_wait();
    outb(PORT_PS2_DATA, status);
    io_wait();

    // Tell the mouse to enable itself
    outb(PORT_PS2_CMD, 0xD4);
    io_wait();
    outb(PORT_PS2_DATA, 0xF4);
    io_wait();

    // Wait for ACK (0xFA)
    uint8_t response = inb(PORT_PS2_DATA);
    io_wait();
    if (response != 0xFA) {
        kerr("PS2 mouse initialization failed! Did not get ACK, got:");
        print_number(response);
        return;
    }

    kinfo("Mouse configured successfully!");

    mouse_configured = true;

    irq_handle(MOUSE_PIC, mouse_handle_entry);
}

bool is_mouse_configured() {
    return mouse_configured;
}

struct cursor {
    usize x, y;
};

static struct cursor cursor = {
    .x = 200,
    .y = 200
};

// each mouse event generates 3 bytes
static u8 packet[3] = {0};
static u8 packet_index = 0;

static u32 colors[] = {
    0xFF0000,
    0x00FF00,
    0x0000FF,
};
static u8 color_index = 0;

static void handle_full_packet(u8 info, i8 x, i8 y) {
    // check for invalid data...
    if (info & X_OFLOW || info & Y_OFLOW) return;

    bool l = info & LEFT_CLICK;
    bool r = info & RIGHT_CLICK;
    bool mid = info & MIDDLE_CLICK;

    x = x - ((info << 4) & 0x100);
	y = y - ((info << 3) & 0x100);

    if (l) {
        color_index = 1;
    } else if (r) {
        color_index = 2;
    }

    cursor.x += x;
    cursor.y -= y;
    // fb_set_px(log_getterm(), cursor.x, cursor.y, colors[color_index]);
    draw_mouse(cursor.x, cursor.y);
}

void mouse_handle(void) {
    u8 byte = inb(PORT_PS2_DATA);

    if (packet_index == 0 && !(byte & SYNCHRONIZE)) {
        // go to next packet.
        goto done;
    }

    packet[packet_index++] = byte;

    if (packet_index == 3) {
        packet_index = 0;
        handle_full_packet(packet[0], (i8) packet[1], (i8) packet[2]);
    }

done:
    pic_eoi(MOUSE_PIC);
}