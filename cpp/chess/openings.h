
#ifndef OPENINGS_H
#define OPENINGS_H
#include "helpers.h"

constexpr auto royLopez = "e2-e4 e7-e5 Kng1-f3 Knb8-c6 Bf1-b5 a7-a6";
constexpr auto italian = "e2-e4 e7-e5 Kng1-f3 Knb8-c6 Bf1-c4 Bf8-c5";
constexpr auto sicilian = "e2-e4 c7-c5 Kng1-f3 Knb8-c6";
constexpr auto french = "e2-e4 e7-e6 d2-d4 d7-d5";
constexpr auto caroKann = "e2-e4 c7-c6 d2-d4 d7-d5";
constexpr auto queensPawn = "d2-d4 d7-d5";
constexpr auto english = "c2-c4 e7-e5";
constexpr auto reti = "Kng1-f3 d7-d5";
constexpr auto openings = {royLopez, italian,    sicilian, french,
                           caroKann, queensPawn, english,  reti};

#endif