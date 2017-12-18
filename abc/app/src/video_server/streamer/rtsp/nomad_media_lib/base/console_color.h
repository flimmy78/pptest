
#ifndef _CONSOLE_COLOR_H_
#define _CONSOLE_COLOR_H_

#include "base/nml_defines.h"
#include "base/basic_types.h"
/* usage example

std::cout << green << "This text is written in green" 
            << white << std::endl;
std::cout << color(FOREGROUND_RED|FOREGROUND_GREEN) 
        << "This text has a really exiting color !" 
        << white << std::endl;
*/

#if defined(OS_WIN)
    #include <windows.h>
    #include <stdio.h>
#else
    #include <iostream>
#endif


inline std::ostream& blue(std::ostream &s)
{
#if defined(OS_WIN)    
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
        |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
#else
    s << "\033[0;34m";
#endif    
    return s;
}

inline std::ostream& red(std::ostream &s)
{
#if defined(OS_WIN)
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
        FOREGROUND_RED|FOREGROUND_INTENSITY);
#else
    s << "\033[0;31m";
#endif
    return s;
}

inline std::ostream& green(std::ostream &s)
{
#if defined(OS_WIN)
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
        FOREGROUND_GREEN|FOREGROUND_INTENSITY);
#else
    s << "\033[0;32m";
#endif
    return s;
}

inline std::ostream& yellow(std::ostream &s)
{
#if defined(OS_WIN)      
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
        FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
#else
    s << "\033[0;33m";
#endif    
    return s;
}

inline std::ostream& reset(std::ostream &s)
{
#if defined(OS_WIN)
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
        FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
#else
    s << "\033[m";
#endif
    return s;
}

struct color {
    color(uint16 attribute):color_(attribute){};
    uint16 color_;
};

template <class _Elem, class _Traits>
std::basic_ostream<_Elem,_Traits>& 
    operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c)
{
#if defined(OS_WIN)      
    HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout,c.color_);
#else
    i << "\033[40m\033[37m";
#endif  
    return i;
}

#endif // !_CONSOLE_COLOR_H_