
# P2 HUB75E (128x64, 1/32Scan) — ESP32‑S3 Web UI (Text + Canvas)

- HUB75E 패널을 ESP32‑S3로 구동(ESP32‑HUB75‑MatrixPanel‑DMA) https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA
- 비동기 웹서버/웹소켓/업로드(ESPAsyncWebServer) https://registry.platformio.org/libraries/ESP32Async/ESPAsyncWebServer
- PNG/JPEG 디코딩(PNGdec, TJpg_Decoder) https://github.com/bitbank2/PNGdec https://github.com/Bodmer/TJpg_Decoder
- 커스텀 폰트(Adafruit GFX + fontconvert) https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts https://github.com/adafruit/Adafruit-GFX-Library/blob/master/fontconvert/fontconvert_win.md
- 파일시스템(LittleFS) https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/file-system-considerations.html

## 빌드/배포
1) pio run -t uploadfs (data/ 웹 리소스 업로드)
2) pio run -t upload (펌웨어 업로드) → 브라우저로 IP 접속

## 폰트 생성(12/14/16pt, NotoSansKR‑Bold)
- ASCII: U+0020–U+007E, Hangul: U+AC00–U+D7A3(가~힣) 범위를 각각 생성 → include/Fonts/에 배치
- include/Fonts/fonts_config.h의 ENABLE_KR_FONTS를 1로 변경 후 헤더 포함

## 배선(예)
R1/G1/B1=42/41/40, R2/G2/B2=38/39/37, A/B/C/D/E=45/36/48/35/21, LAT/OE/CLK=47/14/2
