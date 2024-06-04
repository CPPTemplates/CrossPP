#include "color.h"
//https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
//hue: 0 to 360
//all other values: 0 to 1
colorf rgb2hsv(const colorf& in)
{
    colorf         out;
    out.a() = in.a();

    cfp min = math::minimum(math::minimum(in.r(), in.g()), in.b());//the value of the darkest channel
    cfp max = math::maximum(math::maximum(in.r(), in.g()), in.b());//the value of the brightest channel

    out.v() = max;
    cfp delta = max - min;//the difference between the brightest and darkest channel
    if (delta < 0.00001)
    {
        out.s() = 0;
        out.h() = 0; // undefined, maybe nan?
        return out;
    }
    if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s() = (delta / max);                  // s
    }
    else
    {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s() = 0.0;
        out.h() = NAN;                            // its now undefined
        return out;
    }
    if (in.r() >= max)                           // > is bogus, just keeps compilor happy
    {
        out.h() = (in.g() - in.b()) / delta;        // between yellow & magenta: output range: -1 to 1
        if (out.h() < 0) //make positive
        {
            out.h() += 6;//output range: <= 1 or >= 5
        }
    }
    else
    {
        if (in.g() >= max)
        {
            out.h() = 2.0 + (in.b() - in.r()) / delta;  // between cyan & yellow: output range: 1 to 3
        }
        else
        {
            out.h() = 4.0 + (in.r() - in.g()) / delta;  // between magenta & cyan: output range: 3 to 5
        }
    }
    out.h() *= 60.0;                              // degrees
    return out;
}

colorf hsv2rgb(const colorf& in)
{
    double      hh, p, q, t, ff;
    long        i;
    colorf         out;
    out.a() = in.a();

    if (in.s() <= 0.0) {       // < is bogus, just shuts up warnings
        out.r() = in.v();
        out.g() = in.v();
        out.b() = in.v();
        return out;
    }
    hh = in.h();
    //if (hh >= 360.0) 
    hh = math::mod(hh, 360.0);
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v() * (1.0 - in.s());
    q = in.v() * (1.0 - (in.s() * ff));
    t = in.v() * (1.0 - (in.s() * (1.0 - ff)));

    switch (i) {
    case 0:
        out.r() = in.v();
        out.g() = t;
        out.b() = p;
        break;
    case 1:
        out.r() = q;
        out.g() = in.v();
        out.b() = p;
        break;
    case 2:
        out.r() = p;
        out.g() = in.v();
        out.b() = t;
        break;

    case 3:
        out.r() = p;
        out.g() = q;
        out.b() = in.v();
        break;
    case 4:
        out.r() = t;
        out.g() = p;
        out.b() = in.v();
        break;
    case 5:
    default:
        out.r() = in.v();
        out.g() = p;
        out.b() = q;
        break;
    }
    return out;
}
