
#!/usr/bin/env bash
TTF="$1"; if [ -z "$TTF" ]; then echo "TTF path required"; exit 1; fi
FONTCONVERT_DIR=../Adafruit-GFX-Library/fontconvert
if [ ! -x "$FONTCONVERT_DIR/fontconvert" ]; then echo "Build fontconvert first"; exit 1; fi
pushd "$FONTCONVERT_DIR" >/dev/null
./fontconvert "$TTF" 12 0x20 0x7E   > ../../P2_HUB75E_ESP32S3_WebUI_Fonts_Canvas/include/Fonts/NotoSansKR12_ASCII.h
./fontconvert "$TTF" 12 0xAC00 0xD7A3 > ../../P2_HUB75E_ESP32S3_WebUI_Fonts_Canvas/include/Fonts/NotoSansKR12_Hangul.h
./fontconvert "$TTF" 14 0x20 0x7E   > ../../P2_HUB75E_ESP32S3_WebUI_Fonts_Canvas/include/Fonts/NotoSansKR14_ASCII.h
./fontconvert "$TTF" 14 0xAC00 0xD7A3 > ../../P2_HUB75E_ESP32S3_WebUI_Fonts_Canvas/include/Fonts/NotoSansKR14_Hangul.h
./fontconvert "$TTF" 16 0x20 0x7E   > ../../P2_HUB75E_ESP32S3_WebUI_Fonts_Canvas/include/Fonts/NotoSansKR16_ASCII.h
./fontconvert "$TTF" 16 0xAC00 0xD7A3 > ../../P2_HUB75E_ESP32S3_WebUI_Fonts_Canvas/include/Fonts/NotoSansKR16_Hangul.h
popd >/dev/null
