#include <stdio.h>
#include <unistd.h>
#include "ANSI.h"
#include <math.h>

int main(){
    int i, r, g, b;
    SET_BOLD();
    for(i = 0; i < 666666666; i++){
        r = (int)(fabsf(sinf(0.003f * (float)i)) * 255.0f);
        g = (int)(fabsf(sinf(0.006f * (float)i)) * 255.0f);
        b = (int)(fabsf(sinf(0.009f * (float)i)) * 255.0f);
        SET_FONT_COLOR(r, g, b);
        SET_BACKGROUND_COLOR(255 - r, 255 - g, 255 - b);
        printf("我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史我TMD是史\n");
        usleep(6666);
    }
}