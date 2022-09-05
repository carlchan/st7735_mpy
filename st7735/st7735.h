#ifndef __ST7735_H__
#define __ST7735_H__

#ifdef __cplusplus
extern "C" {
#endif

// Enumerate the different configurations
#define INITR_GREENTAB 0x0
#define INITR_REDTAB 0x1
#define INITR_BLACKTAB 0x2	// Display with no offsets
#define INITR_GREENTAB2 0x3 // Use if you get random pixels on two edges of green tab display
#define INITR_GREENTAB3 0x4 // Use if you get random pixels on edge(s) of 128x128 screen
#define INITR_GREENTAB4 0x5 // Use if you only get part of 128x128 screen in rotation 0 & 1
#define INITR_GREENTAB5 0x6 // Use if you only get part of 128x128 screen in rotation 0 & 1
#define INITR_REDTAB2 0x7	// Added for https://www.aliexpress.com/item/ShengYang-1pcs-IPS-0-96-inch-7P-SPI-HD-65K-Full-Color-OLED-Module-ST7735-Drive/32918394604.html
#define INITB 0xB

// color modes
#define COLOR_MODE_65K 0x50
#define COLOR_MODE_262K 0x60
#define COLOR_MODE_12BIT 0x03
#define COLOR_MODE_16BIT 0x05
#define COLOR_MODE_18BIT 0x06
#define COLOR_MODE_16M 0x07

// commands
#define ST7735_NOP 0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN 0x10
#define ST7735_SLPOUT 0x11
#define ST7735_PTLON 0x12
#define ST7735_NORON 0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_PTLAR 0x30
#define ST7735_VSCRDEF 0x33
#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36
#define ST7735_VSCSAD 0x37

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define ST7735_MADCTL_MY 0x80 // Page Address Order
#define ST7735_MADCTL_MX 0x40 // Column Address Order
#define ST7735_MADCTL_MV 0x20 // Page/Column Order
#define ST7735_MADCTL_ML 0x10 // Line Address Order
#define ST7735_MADCTL_MH 0x04 // Display Data Latch Order
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define OPTIONS_WRAP_V 0x01
#define OPTIONS_WRAP_H 0x02
#define OPTIONS_WRAP 0x03

typedef struct _Point {
	mp_float_t x;
	mp_float_t y;
} Point;

typedef struct _Polygon {
	int	   length;
	Point *points;
} Polygon;

typedef struct _st7735_rotation_t {
	uint8_t	 madctl;
	uint16_t width;
	uint16_t height;
	uint16_t colstart;
	uint16_t rowstart;
} st7735_rotation_t;

typedef struct _st7735_cmd_t {
	uint8_t cmd;
	uint8_t data[16];
	uint8_t databytes; // No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} st7735_cmd_t;

// this is the actual C-structure for our new object
typedef struct _st7735_ST7735_obj_t {
	mp_obj_base_t base;

	mp_obj_base_t	  *spi_obj;
	mp_file_t		  *fp;			   // file object
	uint16_t			 *i2c_buffer;	   // resident buffer if buffer_size given
	void			  *work;		   // work buffer for jpg decoding
	size_t			   buffer_size;	   // resident buffer size, 0=dynamic
	uint16_t		   display_width;  // physical width
	uint16_t		   width;		   // logical width (after rotation)
	uint16_t		   display_height; // physical width
	uint16_t		   height;		   // logical height (after rotation)
	uint8_t			   colstart;
	uint8_t			   rowstart;
	uint8_t			   rotation;
	st7735_rotation_t *rotations;	  // list of rotation tuples [(madctl, colstart, rowstart)]
	uint8_t			   rotations_len; // number of rotations
	uint8_t			   color_order;
	bool			   inversion;
	uint8_t			   madctl;
	uint8_t			   options; // options bit array
	mp_hal_pin_obj_t   reset;
	mp_hal_pin_obj_t   dc;
	mp_hal_pin_obj_t   cs;
	mp_hal_pin_obj_t   backlight;

	uint8_t	 bounding;
	uint16_t min_x;
	uint16_t min_y;
	uint16_t max_x;
	uint16_t max_y;

	uint16_t module;
	uint8_t	 r0_colstart;
	uint8_t	 r0_rowstart;
} st7735_ST7735_obj_t;

mp_obj_t st7735_ST7735_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

extern void draw_pixel(st7735_ST7735_obj_t *self, int16_t x, int16_t y, uint16_t color);
extern void fast_hline(st7735_ST7735_obj_t *self, int16_t x, int16_t y, int16_t w, uint16_t color);

#ifdef __cplusplus
}
#endif /*  __cplusplus */

#endif /*  __ST7735_H__ */
