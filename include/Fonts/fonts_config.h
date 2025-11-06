
#pragma once
#include <Adafruit_GFX.h>
#define ENABLE_KR_FONTS 0
#if ENABLE_KR_FONTS
// #include "NotoSansKR12_ASCII.h"
// #include "NotoSansKR12_Hangul.h"
// #include "NotoSansKR14_ASCII.h"
// #include "NotoSansKR14_Hangul.h"
// #include "NotoSansKR16_ASCII.h"
// #include "NotoSansKR16_Hangul.h"
struct FontPair{const GFXfont* ascii; const GFXfont* hangul;};
extern FontPair FP12; extern FontPair FP14; extern FontPair FP16;
#else
struct FontPair{const GFXfont* ascii; const GFXfont* hangul;};
#endif
