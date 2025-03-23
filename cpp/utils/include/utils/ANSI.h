#ifndef ANSI_H
#define ANSI_H

#define CLEAR_SCREEN() puts("\033[2J")
#define CLEAR_LINE() puts("\033[2K")
#define SET_CURSOR(row, col) printf("\033[%d;%dH", row, col)

#define SET_BOLD() puts("\033[1m")
#define SET_ITALIC() puts("\033[3m")
#define SET_UNDERLINE() puts("\033[4m")
#define SET_FONT_COLOR(r, g, b) printf("\033[38;2;%d;%d;%dm", r, g, b)
#define SET_BACKGROUND_COLOR(r, g, b) printf("\033[48;2;%d;%d;%dm", r, g, b)
#define RESET_FONT_COLOR() puts("\033[39m")
#define RESET_BACKGROUND_COLOR() puts("\033[49m")
#define RESET_ALL() puts("\033[0m")

#endif //ANSI_H
