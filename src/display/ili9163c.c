//
// Created by legath on 01.06.2020.
//
#include "stdarg.h"
#include "ili9163c.h"
static void ili9163c_hwreset();
static void ili9163c_writeCmd(uint8_t cmd);
static void ili9163c_writeData(uint8_t val);

static uint8_t active_width=128;
static uint8_t active_height=128;
enum Rotation {
    ROT0 = 0,	// portrait
    ROT90 = 96,	// landscape
    ROT180 = 160,	// flipped portrait
    ROT270 = 192	// flipped landscape
};
void ili9163c_setRotation(uint8_t rotation);
void ili9163c_init(){
    gpio_disp_res_pin_set(GPIO_PIN_SET);
    ili9163c_hwreset();
    ili9163c_writeCmd(ILI9163_CMD_EXIT_SLEEP_MODE);
    HAL_Delay(5);
    ili9163c_writeCmd(ILI9163_CMD_SET_PIXEL_FORMAT);
    ili9163c_writeData(0x05); // 16 bits per pixel

    ili9163c_writeCmd(ILI9163_CMD_SET_GAMMA_CURVE);
    ili9163c_writeData(0x04); // Select gamma curve 3

    ili9163c_writeCmd(ILI9163_CMD_GAM_R_SEL);
    ili9163c_writeData(0x01); // Gamma adjustment enabled

    ili9163c_writeCmd(ILI9163_CMD_POSITIVE_GAMMA_CORRECT);
    ili9163c_writeData(0x3f); // 1st Parameter
    ili9163c_writeData(0x25); // 2nd Parameter
    ili9163c_writeData(0x1c); // 3rd Parameter
    ili9163c_writeData(0x1e); // 4th Parameter
    ili9163c_writeData(0x20); // 5th Parameter
    ili9163c_writeData(0x12); // 6th Parameter
    ili9163c_writeData(0x2a); // 7th Parameter
    ili9163c_writeData(0x90); // 8th Parameter
    ili9163c_writeData(0x24); // 9th Parameter
    ili9163c_writeData(0x11); // 10th Parameter
    ili9163c_writeData(0x00); // 11th Parameter
    ili9163c_writeData(0x00); // 12th Parameter
    ili9163c_writeData(0x00); // 13th Parameter
    ili9163c_writeData(0x00); // 14th Parameter
    ili9163c_writeData(0x00); // 15th Parameter

    ili9163c_writeCmd(ILI9163_CMD_NEGATIVE_GAMMA_CORRECT);
    ili9163c_writeData(0x20); // 1st Parameter
    ili9163c_writeData(0x20); // 2nd Parameter
    ili9163c_writeData(0x20); // 3rd Parameter
    ili9163c_writeData(0x20); // 4th Parameter
    ili9163c_writeData(0x05); // 5th Parameter
    ili9163c_writeData(0x00); // 6th Parameter
    ili9163c_writeData(0x15); // 7th Parameter
    ili9163c_writeData(0xa7); // 8th Parameter
    ili9163c_writeData(0x3d); // 9th Parameter
    ili9163c_writeData(0x18); // 10th Parameter
    ili9163c_writeData(0x25); // 11th Parameter
    ili9163c_writeData(0x2a); // 12th Parameter
    ili9163c_writeData(0x2b); // 13th Parameter
    ili9163c_writeData(0x2b); // 14th Parameter
    ili9163c_writeData(0x3a); // 15th Parameter

    ili9163c_writeCmd(ILI9163_CMD_FRAME_RATE_CONTROL1);
    ili9163c_writeData(0x08); // DIVA = 8
    ili9163c_writeData(0x08); // VPA = 8

    ili9163c_writeCmd(ILI9163_CMD_DISPLAY_INVERSION);
    ili9163c_writeData(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)

    ili9163c_writeCmd(ILI9163_CMD_POWER_CONTROL1);
    ili9163c_writeData(0x0a); // VRH = 10:  GVDD = 4.30
    ili9163c_writeData(0x02); // VC = 2: VCI1 = 2.65

    ili9163c_writeCmd(ILI9163_CMD_POWER_CONTROL2);
    ili9163c_writeData(0x02); // BT = 2: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 5xVCI1, VGL = -2xVCI1

    ili9163c_writeCmd(ILI9163_CMD_VCOM_CONTROL1);
    ili9163c_writeData(0x50); // VMH = 80: VCOMH voltage = 4.5
    ili9163c_writeData(0x5b); // VML = 91: VCOML voltage = -0.225

    ili9163c_writeCmd(ILI9163_CMD_VCOM_OFFSET_CONTROL);
    ili9163c_writeData(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML

    ili9163c_writeCmd(ILI9163_CMD_SET_COLUMN_ADDRESS);
    ili9163c_writeData(0x00); // XSH
    ili9163c_writeData(0x00); // XSL
    ili9163c_writeData(0x00); // XEH
    ili9163c_writeData(0x7f); // XEL (128 pixels x)

    ili9163c_writeCmd(ILI9163_CMD_SET_PAGE_ADDRESS);
    ili9163c_writeData(0x00);
    ili9163c_writeData(0x00);
    ili9163c_writeData(0x00);
    ili9163c_writeData(0x7f); // 128 pixels y

    ili9163c_setRotation(ROT0);
    ili9163c_writeCmd(ILI9163_CMD_SET_DISPLAY_ON);
    ili9163c_writeCmd(ILI9163_CMD_WRITE_MEMORY_START);
}

static void ili9163c_hwreset(){
    gpio_disp_res_pin_set(GPIO_PIN_RESET);
    HAL_Delay(50);
    gpio_disp_res_pin_set(GPIO_PIN_SET);
    HAL_Delay(120);
}

static void ili9163c_writeCmd(uint8_t cmd){
    gpio_disp_cmd_pin_set(GPIO_PIN_RESET);
    spi1_writeByte(cmd);
}
static void ili9163c_writeData(uint8_t val) {
    gpio_disp_cmd_pin_set(GPIO_PIN_SET);
    spi1_writeByte(val);
}

static void ili9163c_writeData16(uint16_t word) {
    gpio_disp_cmd_pin_set(GPIO_PIN_SET);
    spi1_writeByte((word >> 8) & 0x00FF);
    spi1_writeByte(word & 0x00FF);
}

void ili9163c_setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) {
    ili9163c_writeCmd(ILI9163_CMD_SET_COLUMN_ADDRESS);
    ili9163c_writeData16(x1);
    ili9163c_writeData16(x2);

    ili9163c_writeCmd(ILI9163_CMD_SET_PAGE_ADDRESS);
    ili9163c_writeData16(y1+32);
    ili9163c_writeData16(y2+32);
    // memory write
    ili9163c_writeCmd(ILI9163_CMD_WRITE_MEMORY_START);
}

//rotate screen at desired orientation
void ili9163c_setRotation(uint8_t rotation) {
    ili9163c_writeCmd(ILI9163_CMD_SET_ADDRESS_MODE);
    ili9163c_writeData(rotation);
}
//draw colour filled rectangle
void ili9163c_drawRectFilled(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour) {
    if ((x >= active_width) || (y >= active_height)) return;
    if ((x + w - 1) >= active_width) w = active_width - x;
    if ((y + h - 1) >= active_height) h = active_height - y;
    ili9163c_setAddress(x, y, x + w - 1, y + h - 1);
    for (y = h; y > 0; y--)
        for (x = w; x > 0; x--)
            ili9163c_writeData16(colour);
}

void ili9163c_drawPixel(uint8_t x, uint8_t y, uint16_t colour) {
    if ((x < 0) || (x >= active_width) || (y < 0) || (y >= active_height)) return;
    ili9163c_setAddress(x, y, x + 1, y + 1);
    ili9163c_writeData16(colour);
}
void ili9163c_drawPixelSized(uint8_t x, uint8_t y, uint8_t size, uint16_t colour) {
    if (size == 1) // default size or big size
        ili9163c_drawPixel(x, y, colour);
    else
        ili9163c_drawRectFilled(x, y, size, size, colour);
}

void ili9163c_drawChar( uint8_t x, uint8_t y, char c, uint8_t size, uint16_t colour, uint16_t bg) {
    // draw. optimisation:6th font line is set as 0, to lower font array size
    for (int8_t i=0; i < FONT_WIDTH; i++ ) {
        uint8_t line = (i == FONT_WIDTH-1)? 0 : font5x8[(c * (FONT_WIDTH - 1)) + i];
        for (int8_t j = 0; j < FONT_HEIGHT; j++) {
            if (line & 0x1) {
                if (colour != TRANSPARENT) ili9163c_drawPixelSized(x + i*size, y + j*size, size, colour);
            }
            else {
                if (bg != TRANSPARENT) ili9163c_drawPixelSized(x + i*size, y + j*size, size, bg);
            }
            line >>= 1;
        }
    }
}

// Plot a string of characters to the LCD: with the current font the screen allows up to 21x16 characters
void ili9163c_drawString(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg,  const char *string) {
    if (x == CENTERX) {
        x = (active_width - size * FONT_WIDTH * strlen(string)) / 2;
    }
    if (y == CENTERY) {
        y = (active_height - size * FONT_HEIGHT) / 2;
    }
    unsigned char c;
    while ( (c = *string++) ) {
        if (c == '\r') continue;
        if (c != '\n') ili9163c_drawChar(x, y, c, size, colour, bg);
        // new line check
        if(c == '\n' || x > active_width - 2 * FONT_WIDTH * size) {
            x = 0;
            y += size * FONT_HEIGHT;
        } else {
            x += size * FONT_WIDTH;
        }
    }
}

void ili9163c_drawStringF(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg, char *szFormat, ...) {
    char szBuffer[256]; //in this buffer we form the message
    int NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
    int LASTCHAR = NUMCHARS - 1;
    va_list pArgs;
    va_start(pArgs, szFormat);
    vsnprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
    va_end(pArgs);

    ili9163c_drawString(x, y, size, colour, bg, szBuffer);
}
uint16_t rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
    return ((b>>3) << 11) | ((g>>2) << 5) | (r>>3);
}