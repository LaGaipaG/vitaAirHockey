#ifndef UTILS_H
#define UTILS_H

#define align_mem(addr, align) (((addr) + ((align) - 1)) & ~((align) - 1))
#define lerp(value, from_max, to_max) ((((value*10) * (to_max*10))/(from_max*10))/10)
#define abs(x) ((x) < 0 ? (-x) : (x))

//Colors
#define BLACK RGBA8(0, 0, 0, 255)
#define DARK_GREY RGBA8(100, 100, 100, 255)
#define GREY RGBA8(150, 150, 150, 255)
#define WHITE RGBA8(255, 255, 255, 255)
#define RED   RGBA8(255, 0,   0,   255)
#define GREEN RGBA8(0,   255, 0,   255)
#define BLUE  RGBA8(0,   0,   255, 255)

void delay (unsigned int mseconds);

#endif
