#pragma once


// Text color
#define SET_COLOR_BLACK_TEXT    "\033[30m" // Set text color to black
#define SET_COLOR_RED_TEXT      "\033[31m" // Set text color to red
#define SET_COLOR_GREEN_TEXT    "\033[32m" // Set text color to green
#define SET_COLOR_YELLOW_TEXT   "\033[33m" // Set text color to yellow
#define SET_COLOR_BLUE_TEXT     "\033[34m" // Set text color to blue
#define SET_COLOR_MAGENTA_TEXT  "\033[35m" // Set text color to magenta
#define SET_COLOR_CYAN_TEXT     "\033[36m" // Set text color to cyan
#define SET_COLOR_WHITE_TEXT    "\033[37m" // Set text color to white
#define SET_COLOR_RESET_TEXT    "\033[39m" // Reset text color to default

// Background color
#define SET_COLOR_BLACK_BACKGROUND "\033[40m" // Set background color to black
#define SET_COLOR_RED_BACKGROUND "\033[41m" // Set background color to red
#define SET_COLOR_GREEN_BACKGROUND "\033[42m" // Set background color to green
#define SET_COLOR_YELLOW_BACKGROUND "\033[43m" // Set background color to yellow
#define SET_COLOR_BLUE_BACKGROUND "\033[44m" // Set background color to blue
#define SET_COLOR_MAGENTA_BACKGROUND "\033[45m" // Set background color to magenta
#define SET_COLOR_CYAN_BACKGROUND "\033[46m" // Set background color to cyan
#define SET_COLOR_WHITE_BACKGROUND "\033[47m" // Set background color to white

#define SET_COLOR_RESET_BACKGROUND "\033[49m" // Reset background color to default
#define SET_COLOR_RESET_TEXT    "\033[39m"
#define SET_COLOR(x)  std::cout << (x)
#define RST_COLOR() SET_COLOR(SET_COLOR_RESET_BACKGROUND + SET_COLOR_RESET_TEXT)  


/*

#define "\033[0m" // Reset all text attributes to default
#define "\033[1m" // Bold on
#define "\033[2m" // Faint off
#define "\033[3m" // Italic on
#define "\033[4m" // Underline on
#define "\033[5m" // Slow blink on
#define "\033[6m" // Rapid blink on
#define "\033[7m" // Reverse video on
#define "\033[8m" // Conceal on
#define "\033[9m" // Crossed-out on
#define "\033[22m" // Bold off
#define "\033[23m" // Italic off
#define "\033[24m" // Underline off
#define "\033[25m" // Blink off
#define "\033[27m" // Reverse video off
#define "\033[28m" // Conceal off
#define "\033[29m" // Crossed-out off

*/