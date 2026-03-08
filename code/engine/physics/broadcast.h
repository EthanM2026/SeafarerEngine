/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BROADCAST_H_INCLUDED
#define BROADCAST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../graphics/image.h"
#include <stdio.h>
#include "../engine.h"

// --- Configuration Constants ---
#define WIDTH 320            // Image width (must be 64 for this simplified model)
#define HEIGHT 240           // Image height (must be 48 for this simplified model)
#define BMP_PIXEL_SIZE 3    // 24-bit BMP

#define TEX_WIDTH 320
#define TEX_HEIGHT 240

// WAV File Constants
#define SAMPLE_RATE 44100
#define BITS_PER_SAMPLE 16
#define MAX_AMPLITUDE 32767

// Signal Constants (Highly Simplified NTSC Emulation)
// This is the FIXED FREQUENCY used by the encoder (the "broadcast channel").
#define ENCODE_CHROMA_FREQ (SAMPLE_RATE / 8.0) // 5512.5 Hz
#define TWO_PI (2 * M_PI)

// Encoding Adjustment
// Phase of the color burst in the encoder. Changing this is the primary way to fix hue shifts.
#define CHROMA_ENCODE_BURST_PHASE 0.0

// Decoding Adjustments
// Factors used to scale the recovered U/V components independently to adjust color balance.
#define CHROMA_U_DECODE_FACTOR 2.0
#define CHROMA_V_DECODE_FACTOR 2.0

// Filter Constants
#define FILTER_TAP_COUNT 5 // Number of taps for the simple FIR Low-Pass Filter
#define FILTER_MID_POINT 2 // (FILTER_TAP_COUNT - 1) / 2

// PLL Constants
#define PLL_GAIN 0.05 // Gain factor for the Phase-Locked Loop (Lower value = more noise rejection)

// Line Timing (in samples @ 44.1kHz)
#define SAMPLES_PER_PIXEL 4 // Number of samples dedicated to encode one pixel's color
#define SAMPLES_PER_VIDEO (WIDTH * SAMPLES_PER_PIXEL) // 256 samples
#define SAMPLES_H_SYNC 50   // Horizontal Sync Pulse (Lowest voltage)
#define SAMPLES_BACK_PORCH 50 // Includes simplified color burst reference
#define SAMPLES_FRONT_PORCH 10
#define SAMPLES_PER_LINE (SAMPLES_H_SYNC + SAMPLES_BACK_PORCH + SAMPLES_PER_VIDEO + SAMPLES_FRONT_PORCH)
#define TOTAL_SAMPLES (HEIGHT * SAMPLES_PER_LINE)

// Analog Voltage Mapping (from 0-100 IRE units, mapped to -1.0 to 1.0 amplitude)
#define SYNC_LEVEL -1.0      // Lowest (H-Sync)
#define BLACK_LEVEL -0.1     // 7.5 IRE
#define WHITE_LEVEL 0.7      // 100 IRE
#define BLANKING_LEVEL 0.0   // 0 IRE (Back Porch)

// --- Structs for File Headers ---

// BMP File Header (14 bytes)
#pragma pack(push, 1)
typedef struct {
    unsigned short signature; // 'BM'
    unsigned int fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int dataOffset;
} BMPFileHeader;

// BMP Info Header (40 bytes)
typedef struct {
    unsigned int headerSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitCount; // 24
    unsigned int compression; // 0
    unsigned int imageSize;
    int xPelsPerMeter;
    int yPelsPerMeter;
    unsigned int colorsUsed;
    unsigned int colorsImportant;
} BMPInfoHeader;
#pragma pack(pop)

// WAV File Header (44 bytes)
#pragma pack(push, 1)
typedef struct {
    char chunkID[4];
    unsigned int chunkSize;
    char format[4];
    char subchunk1ID[4];
    unsigned int subchunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char subchunk2ID[4];
    unsigned int subchunk2Size;
} WAVHeader;
#pragma pack(pop)

// --- Color Space Conversion (Simplified YUV/YIQ Approximation) ---
typedef struct {
    double Y, U, V;
} YUV;

void RGBtoYUV(unsigned char R, unsigned char G, unsigned char B, YUV *yuv);
void YUVtoRGB(YUV yuv, unsigned char *R, unsigned char *G, unsigned char *B);
unsigned char* read_bmp(const char* filename);
// Function to write a fixed-size 24-bit BMP (64x48)
int write_bmp(const char* filename, unsigned char* data);

// --- WAV File I/O ---

// Function to write the WAV header
void write_wav_header(FILE* fp, unsigned int totalSamples);

// --- NTSC Encoding and Decoding ---

// 1. BMP -> WAV Encoding
void encode_ntsc(unsigned char* bmp_data, short* wav_samples);

/**
 * @brief Applies a simple 5-tap moving average filter (LPF) to the input signal.
 * @param input_samples The raw 16-bit WAV samples.
 * @param luma_output The array to store the filtered, normalized (double) signal.
 * @param num_samples The total number of samples.
 */
void apply_lpf_filter(const short* input_samples, double* luma_output, int num_samples);


// 2. WAV -> BMP Decoding
// Now accepts the desired demodulation frequency as an argument.
void decode_ntsc(short* wav_samples, unsigned char* decoded_bmp_data, double demod_freq, double Amplitude, int Interference);
// --- Main Program ---
float Calculate_Inverse_Square_Law(float Source_Strength, float Distance_In_Meters);

struct _Broadcaster
{
    struct _Image* Image;
    struct _Image* No_Signal;

    struct _Model* Screen;

    float Frequency;
    float Gain;
    bool Signal_Received;

    short *wav_samples;
    unsigned char *decoded_bmp_data;
    unsigned char *original_bmp_data;

    GLuint fboId;           // The Framebuffer Object
    GLuint textureId;       // The texture object to render into
    GLuint rboId;           // The Renderbuffer Object (for depth)
};

struct _Broadcaster* Create_Broadcaster();

void Encode_Image(struct _Broadcaster* Broadcaster, float Frequency, float Broadcast_Strength);
void Receive_Image(struct _Broadcaster* Broadcaster, float Frequency, float Broadcast_Strength, double Amplitude, int Interference);

void initFBO(struct _Broadcaster* Broadcaster);
void setupFBO(struct _Engine* Engine);

void Draw_Camera_Scene(struct _Engine* Engine);
void Render_Screen_Model(struct _MD2_Model* Model, GLuint textureId, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B);


#endif // BROADCAST_H_INCLUDED

