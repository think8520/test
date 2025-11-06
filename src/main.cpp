
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <LittleFS.h>
#include <PNGdec.h>
#include <TJpg_Decoder.h>
#include <Adafruit_GFX.h>
#include "Fonts/fonts_config.h"

HUB75_I2S_CFG cfg(128,64,1);
MatrixPanel_I2S_DMA *mx = nullptr;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
PNG png;

static uint32_t decodeUtf8(const String&s,size_t&i){uint8_t c=(uint8_t)s[i++];if(c<0x80)return c; if((c>>5)==0x6){return ((c&0x1F)<<6)|((uint8_t)s[i++]&0x3F);} if((c>>4)==0xE){return ((c&0x0F)<<12)|(((uint8_t)s[i++]&0x3F)<<6)|((uint8_t)s[i++]&0x3F);} if((c>>3)==0x1E){return ((c&0x07)<<18)|(((uint8_t)s[i++]&0x3F)<<12)|(((uint8_t)s[i++]&0x3F)<<6)|((uint8_t)s[i++]&0x3F);} return c;}
#if ENABLE_KR_FONTS
static void drawUtf8(MatrixPanel_I2S_DMA*mx,const String&s,int x,int y,uint16_t col,const FontPair&fp){int16_t cx=x;for(size_t i=0;i<s.length();){uint32_t cp=decodeUtf8(s,i);const GFXfont* f=(cp>=0xAC00&&cp<=0xD7A3)?fp.hangul:fp.ascii; if(!f||cp<f->first||cp>f->last){cx+=4;continue;} const GFXglyph*g=&f->glyph[cp-f->first];const uint8_t*bm=f->bitmap+g->bitmapOffset;int w=g->width,h=g->height,xa=g->xAdvance,xo=g->xOffset,yo=g->yOffset;int16_t gx=cx+xo,gy=y+yo;int bpr=(w+7)/8;for(int yy=0;yy<h;yy++){for(int xx=0;xx<w;xx++){if(bm[yy*bpr+(xx>>3)]&(0x80>>(xx&7)))mx->drawPixel(gx+xx,gy+yy,col);}}cx+=xa;}}
#endif

static void renderLast(){if(LittleFS.exists("/last.png")){File f=LittleFS.open("/last.png","r");size_t sz=f.size();uint8_t*buf=(uint8_t*)ps_malloc(sz);if(!buf){f.close();return;}f.read(buf,sz);f.close(); if(png.openRAM(buf,sz,[](PNGDRAW*pd){for(int i=0;i<pd->iWidth;i++){uint8_t r,g,b; png.getLineColor(pd,i,&r,&g,&b); mx->drawPixel(pd->x+i,pd->y,mx->color565(r,g,b));}return 1;})){png.decode(NULL,0);png.close();mx->flipDMABuffer();}free(buf);} else if(LittleFS.exists("/last.jpg")){File f=LittleFS.open("/last.jpg","r");TJpgDec.setSwapBytes(true);TJpgDec.setCallback([](int16_t x,int16_t y,uint16_t w,uint16_t h,uint16_t*bmp){for(int j=0;j<h;j++){for(int i=0;i<w;i++){mx->drawPixel(x+i,y+j,bmp[j*w+i]);}}return true;});TJpgDec.drawJpg(0,0,f);f.close();mx->flipDMABuffer();}}

void setup(){Serial.begin(115200);WiFi.begin("YOUR_SSID","YOUR_PASS");while(WiFi.status()!=WL_CONNECTED){delay(300);Serial.print('.');}Serial.println();Serial.println(WiFi.localIP());LittleFS.begin();mx=new MatrixPanel_I2S_DMA(cfg);mx->begin();mx->setBrightness8(140);mx->fillScreen(0);
server.serveStatic("/",LittleFS,"/").setDefaultFile("index.html");
ws.onEvent([](AsyncWebSocket*s,AsyncWebSocketClient*c,AwsEventType t,void*arg,uint8_t*d,size_t l){if(t==WS_EVT_DATA){StaticJsonDocument<256> doc; if(deserializeJson(doc,d,l))return; String text=doc["text"]|"";int x=doc["x"]|0,y=doc["y"]|16;int pt=doc["pt"]|16;String hex=doc["color"]|"#ffffff";uint32_t rgb=strtoul(hex.c_str()+1,nullptr,16);uint16_t col=mx->color565((rgb>>16)&255,(rgb>>8)&255,rgb&255);mx->fillScreen(0);
#if ENABLE_KR_FONTS
const FontPair&fp=(pt==12?FP12:pt==14?FP14:FP16);drawUtf8(mx,text,x,y,col,fp);
#else
mx->setTextColor(col);mx->setCursor(x,y);mx->print(text);
#endif
mx->flipDMABuffer();}});server.addHandler(&ws);
File up;server.on("/upload",HTTP_POST,[](AsyncWebServerRequest*r){r->send(200,"text/plain","OK");},[&](AsyncWebServerRequest*r,const String&fn,size_t idx,uint8_t*data,size_t len,bool final){if(idx==0){String path=fn.endsWith(".png")?"/last.png":"/last.jpg";up=LittleFS.open(path,"w");}if(up)up.write(data,len);if(final){up.close();renderLast();}});
server.begin();}
void loop(){}
