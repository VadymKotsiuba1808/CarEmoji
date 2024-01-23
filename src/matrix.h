// #include <MatrixHardware_ESP32_V0.h> // This file contains multiple ESP32 hardware configurations, edit the file to define GPIOPINOUT (or add #define GPIOPINOUT with a hardcoded number before this #include)
// //#include "MatrixHardware_Custom.h"                  // Copy an existing MatrixHardware file to your Sketch directory, rename, customize, and you can include it like this
// #include <SmartMatrix.h>
// //#include "gimpbitmap.h"
// #define COLOR_DEPTH 24                                         // Choose the color depth used for storing pixels in the layers: 24 or 48 (24 is good for most sketches - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24)
// const uint16_t kMatrixWidth = 32;                              // Set to the width of your display, must be a multiple of 8
// const uint16_t kMatrixHeight = 32;                             // Set to the height of your display
// const uint8_t kRefreshDepth = 36;                              // Tradeoff of color quality vs refresh rate, max brightness, and RAM usage.  36 is typically good, drop down to 24 if you need to.  On Teensy, multiples of 3, up to 48: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48.  On ESP32: 24, 36, 48
// const uint8_t kDmaBufferRows = 4;                              // known working: 2-4, use 2 to save RAM, more to keep from dropping frames and automatically lowering refresh rate.  (This isn't used on ESP32, leave as default)
// const uint8_t kPanelType = SM_PANELTYPE_HUB75_32ROW_MOD16SCAN; // Choose the configuration that matches your panels.  See more details in MatrixCommonHub75.h and the docs: https://github.com/pixelmatix/SmartMatrix/wiki
// const uint32_t kMatrixOptions = (SM_HUB75_OPTIONS_NONE);       // see docs for options: https://github.com/pixelmatix/SmartMatrix/wiki
// const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
// const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
// const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

// SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);

// SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

// SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

// SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

// const int defaultBrightness = (100 * 255) / 100; // full (100%) brightness
// // const int defaultBrightness = (15*255)/100;       // dim: 15% brightness
// const int defaultScrollOffset = 6;
// const rgb24 defaultBackgroundColor = {0x40, 0, 0};

// // void drawBitmap(int16_t x, int16_t y, const gimp32x32bitmap* bitmap) {
// //   for(unsigned int i=0; i < bitmap->height; i++) {
// //     for(unsigned int j=0; j < bitmap->width; j++) {
// //       rgb24 pixel = { bitmap->pixel_data[(i*bitmap->width + j)*3 + 0],
// //                       bitmap->pixel_data[(i*bitmap->width + j)*3 + 1],
// //                       bitmap->pixel_data[(i*bitmap->width + j)*3 + 2] };

// //       backgroundLayer.drawPixel(x + j, y + i, pixel);
// //     }
// //   }
// // }

// void init_matrix()
// {
//   matrix.addLayer(&backgroundLayer);
//   matrix.addLayer(&scrollingLayer);
//   matrix.addLayer(&indexedLayer);
//   matrix.begin();

//   matrix.setBrightness(defaultBrightness);

//   scrollingLayer.setOffsetFromTop(defaultScrollOffset);

//   backgroundLayer.enableColorCorrection(true);

//   backgroundLayer.fillScreen(defaultBackgroundColor);
//   backgroundLayer.swapBuffers();
// }