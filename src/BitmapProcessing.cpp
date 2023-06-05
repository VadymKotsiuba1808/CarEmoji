#include <Arduino.h>
#include <BitmapProcessing.h>
#include <MatrixHardware_ESP32_V0.h>
#include <SmartMatrix.h>
#define COLOR_DEPTH 24                                         // Choose the color depth used for storing pixels in the layers: 24 or 48 (24 is good for most sketches - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24)
const uint16_t kMatrixWidth = 32;                              // 32                                                                 // Set to the width of your display, must be a multiple of 8
const uint16_t kMatrixHeight = 64;                             // 64                                                                 // Set to the height of your display
const uint8_t kRefreshDepth = 36;                              // Tradeoff of color quality vs refresh rate, max brightness, and RAM usage.  36 is typically good, drop down to 24 if you need to.  On Teensy, multiples of 3, up to 48: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48.  On ESP32: 24, 36, 48
const uint8_t kDmaBufferRows = 2;                              // known working: 2-4, use 2 to save RAM, more to keep from dropping frames and automatically lowering refresh rate.  (This isn't used on ESP32, leave as default)
const uint8_t kPanelType = SM_PANELTYPE_HUB75_64ROW_MOD32SCAN; // SM_PANELTYPE_HUB75_64ROW_MOD32SCAN;// SM_PANELTYPE_HUB75_32ROW_MOD16SCAN   ;// SM_PANELTYPE_HUB75_64ROW_MOD32SCAN   // Choose the configuration that matches your panels.  See more details in MatrixCommonHub75.h and the docs: https://github.com/pixelmatix/SmartMatrix/wiki
const uint32_t kMatrixOptions = (SM_HUB75_OPTIONS_NONE);       // SMARTMATRIX_OPTIONS_C_SHAPE_STACKING     // see docs for options: https://github.com/pixelmatix/SmartMatrix/wiki
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);

SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

const int defaultBrightness = (50 * 255) / 100; // full (100%) brightness
// const int defaultBrightness = (15*255)/100;       // dim: 15% brightness

const int defaultScrollOffset = 6;
const rgb24 defaultBackgroundColor = {0, 0, 0};
const rgb24 redColor = {255, 0, 0};
const rgb24 grinColor = {0, 255, 0};
const rgb24 blueColor = {0, 0, 255};

void Init_matrix(int brightness)
{
    // тут може почати ребутитись після будь якого поінта як собі захоче,
    // залежить від прошивки. можна раз прошити буде постійно ребутитись після "point 1.4"
    // нічого не змінюєш в прошивці прошиваєш і воно вже починає постійно ребутитись після "point 1.2"
    //
Serial.println("point 1");
    Serial.println(esp_get_free_heap_size());
    
    Serial.println("point 1.1");
    Serial.println(esp_get_free_heap_size());
    matrix.addLayer(&backgroundLayer);
    Serial.println("point 1.2");
    Serial.println(esp_get_free_heap_size());
    matrix.addLayer(&scrollingLayer);
    Serial.println("point 1.3");
    Serial.println(esp_get_free_heap_size());
    matrix.addLayer(&indexedLayer);
    Serial.println("point 1.4");
    Serial.println(esp_get_free_heap_size());

    matrix.begin();

    matrix.setBrightness(((brightness * 255) / 100));
    Serial.println("point 1.5");
    Serial.println(esp_get_free_heap_size());
    scrollingLayer.setOffsetFromTop(defaultScrollOffset);
    Serial.println("point 1.6");
    Serial.println(esp_get_free_heap_size());
    backgroundLayer.enableColorCorrection(true);
    Serial.println("point 1.7");
    Serial.println(esp_get_free_heap_size());
    backgroundLayer.fillScreen(redColor);
    Serial.println("point 1.8");
    Serial.println(esp_get_free_heap_size());
    backgroundLayer.swapBuffers();
    Serial.println("point 1.9");
    Serial.println(esp_get_free_heap_size()); 
}

void ClearBuffer()
{
    rgb24 colorBlack = {0, 0, 0};
    backgroundLayer.fillScreen(colorBlack);
}

void DrawBitmap(uint16_t *bitmap)
{
    ClearBuffer();
    for (unsigned int i = 0, k = 0; i < 32; i++)
    {
        for (unsigned int j = 31;; j++, k++)
        {
            backgroundLayer.drawPixel(j, i, rgb16(*(bitmap + k)));
            if (j == 31)
            {
                j = -1;
            }
            if (j == 30)
            {
                k++;
                break;
            }
        }
    }
    backgroundLayer.swapBuffers();
    
    // for (size_t y = 0; y < 32; y++)
    // {
    //     for (size_t x = 31;  ; x++)
    //     {
    //         backgroundLayer.drawPixel(x,y,redColor);
    //         backgroundLayer.swapBuffers();
    //         if (x == 31)
    //         {
    //             x=-1;
    //         }
    //         if (x == 30)
    //         {
    //             break;
    //         }
    //         delay(50);
    //     }

    // }

    // backgroundLayer.drawPixel(0,0,redColor);
    // backgroundLayer.drawPixel(30,1,grinColor);
    // backgroundLayer.drawPixel(31,2,blueColor);


}

void SetBrightness(uint britnes)
{
    britnes = britnes > 100 ? 100 : britnes < 10 ? 10
                                                 : britnes;
    ;
    matrix.setBrightness(((britnes * 255) / 100));
}

void CreepingLine(String str)
{
    static unsigned long timer = 0;
    int arrSize = str.length() + 1;
    char charArr[arrSize] = {0};
    str.toCharArray(charArr, arrSize);
    rgb24 color = {(uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256)}; // random color
    // rgb24 color = {255, 0, 0};

    /* Fonts:
    font3x5
    font5x7
    font6x10
    font8x13
    gohufont11   - 5x7
    gohufont11b  - 5x7(border)
    */
    backgroundLayer.setFont(font8x13);

    int X_weith = 0;
    int Y_Height = 9;
    if (str.length() > 4)
    {

        for (int i = 0, X = 32; i < (str.length() * 8) + 32; i++, X--)
        {
            ClearBuffer();
            backgroundLayer.drawString(X, Y_Height, color, charArr);
            backgroundLayer.swapBuffers();
            vTaskDelay(15);
        }
    }
    else
    {
        ClearBuffer();
        backgroundLayer.drawString(X_weith, Y_Height, color, charArr);
        backgroundLayer.swapBuffers();
    }
}

void CreepingLine(int X, int Y, char *arr)
{
    ClearBuffer();
    backgroundLayer.setFont(font8x13);
    rgb24 color = {255, 0, 0};
    backgroundLayer.drawString(X, Y, color, arr);
    backgroundLayer.swapBuffers();
}

void SetBackGround(uint8_t r, uint8_t g, uint8_t b)
{
    rgb24 color = {r, g, b};
    backgroundLayer.fillScreen(color);
    backgroundLayer.swapBuffers();
}
