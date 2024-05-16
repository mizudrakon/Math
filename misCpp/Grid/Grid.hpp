
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
#include <Windows.h>
#define WINVERSION
#else
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif

#include <chrono>
#include <string>
#include <vector>

constexpr int getSpace(int n){
    int result = 1;
    for (;n > 0; n/=10)
        ++result;
    return result;
}

class Grid
{
    const int spacing;
    const size_t    nScreenW,
                    nScreenH,
                    x_start,
                    x_end,
                    y_start,
                    y_end;

    std::vector<short>
    wchar_t* screen;
public:
    Grid(const size_t sh, const size_t sw, const int sp, const size_t mh, const size_t mw)
    :nScreenW(sw),nScreenH(sh),screen(new wchar_t[sw*sh]),spacing(sp),
    x_start(w - sp*w)
    {

    }
};