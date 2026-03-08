/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "broadcast.h"
#include "stdio.h"
float Calculate_Inverse_Square_Law(float Source_Strength, float Distance_In_Meters)
{
    float Result = Source_Strength/(4*3.14159*(Distance_In_Meters*Distance_In_Meters));
    printf("Strength: %f\n", Result);
    return Result;
};

void RGBtoYUV(unsigned char R, unsigned char G, unsigned char B, YUV *yuv) {
    // Simplified BT.601 approximation for YUV (scaled from 0-255 to 0-1.0 for Y, -0.5 to 0.5 for U/V)
    double r = R / 255.0;
    double g = G / 255.0;
    double b = B / 255.0;

    yuv->Y = 0.299 * r + 0.587 * g + 0.114 * b;
    yuv->U = -0.147 * r - 0.289 * g + 0.436 * b;
    yuv->V = 0.615 * r - 0.515 * g - 0.100 * b;

    // Scale Y to be in the video range (BLACK_LEVEL to WHITE_LEVEL)
    yuv->Y = BLACK_LEVEL + yuv->Y * (WHITE_LEVEL - BLACK_LEVEL);

    // Scale U/V to fit within the modulation amplitude
    yuv->U *= 1.5; // Max modulation amplitude
    yuv->V *= 1.5;
}

void YUVtoRGB(YUV yuv, unsigned char *R, unsigned char *G, unsigned char *B) {
    // Reverse scale Y to 0-1.0
    double y = (yuv.Y - BLACK_LEVEL) / (WHITE_LEVEL - BLACK_LEVEL);

    // Clamp Y back into a safe range [0.0, 1.0]
    if (y < 0.0) y = 0.0;
    if (y > 1.0) y = 1.0;

    // Reverse scale U/V
    double u = yuv.U / 1.5;
    double v = yuv.V / 1.5;

    // Simplified BT.601 conversion back to RGB
    double r = y + 1.140 * v;
    double g = y - 0.395 * u - 0.581 * v;
    double b = y + 2.032 * u;

    // Clamp and convert back to 0-255
    *R = (unsigned char)(r < 0.0 ? 0 : (r > 1.0 ? 255 : r * 255));
    *G = (unsigned char)(g < 0.0 ? 0 : (g > 1.0 ? 255 : g * 255));
    *B = (unsigned char)(b < 0.0 ? 0 : (b > 1.0 ? 255 : b * 255));
}

// --- BMP File Loading and Saving ---

// Function to read a fixed-size 24-bit BMP (64x48)
unsigned char* read_bmp(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening input BMP file");
        return NULL;
    }

    BMPFileHeader fh;
    BMPInfoHeader ih;

    if (fread(&fh, sizeof(BMPFileHeader), 1, fp) != 1 ||
        fread(&ih, sizeof(BMPInfoHeader), 1, fp) != 1) {
        fprintf(stderr, "Error reading BMP headers.\n");
        fclose(fp);
        return NULL;
    }

    if (ih.width != WIDTH || ih.height != HEIGHT || ih.bitCount != 24) {
        fprintf(stderr, "Input BMP must be exactly %dx%d and 24-bit.\n", WIDTH, HEIGHT);
        fclose(fp);
        return NULL;
    }

    unsigned int dataSize = ih.imageSize;
    if (dataSize == 0) {
        // Calculate the raw image size (width * height * 3 bytes)
        // BMP lines are padded to a multiple of 4 bytes.
        int padding = (4 - (WIDTH * 3) % 4) % 4;
        dataSize = HEIGHT * (WIDTH * 3 + padding);
    }

    unsigned char* data = (unsigned char*)malloc(dataSize);
    if (!data) {
        fprintf(stderr, "Memory allocation failed for BMP data.\n");
        fclose(fp);
        return NULL;
    }

    fseek(fp, fh.dataOffset, SEEK_SET);
    if (fread(data, 1, dataSize, fp) != dataSize) {
        fprintf(stderr, "Error reading BMP pixel data.\n");
        free(data);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return data;
}

// Function to write a fixed-size 24-bit BMP (64x48)
int write_bmp(const char* filename, unsigned char* data) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening output BMP file");
        return 0;
    }

    int row_size = WIDTH * BMP_PIXEL_SIZE;
    int padding = (4 - (row_size % 4)) % 4;
    int padded_row_size = row_size + padding;
    unsigned int imageSize = HEIGHT * padded_row_size;
    unsigned int fileSize = 14 + 40 + imageSize;

    // BMP File Header
    BMPFileHeader fh = {
        .signature = 0x4D42, // 'BM'
        .fileSize = fileSize,
        .reserved1 = 0,
        .reserved2 = 0,
        .dataOffset = 54
    };

    // BMP Info Header
    BMPInfoHeader ih = {
        .headerSize = 40,
        .width = WIDTH,
        .height = HEIGHT,
        .planes = 1,
        .bitCount = 24,
        .compression = 0,
        .imageSize = imageSize,
        .xPelsPerMeter = 2835,
        .yPelsPerMeter = 2835,
        .colorsUsed = 0,
        .colorsImportant = 0
    };

    if (fwrite(&fh, sizeof(BMPFileHeader), 1, fp) != 1 ||
        fwrite(&ih, sizeof(BMPInfoHeader), 1, fp) != 1) {
        fprintf(stderr, "Error writing BMP headers.\n");
        fclose(fp);
        return 0;
    }

    // Write pixel data with padding
    unsigned char zero_pad[3] = {0};
    for (int i = 0; i < HEIGHT; i++) {
        // BMP stores rows bottom-to-top, but our data is top-to-bottom.
        // We write the rows in reverse order (HEIGHT - 1 - i)
        int row_index = HEIGHT - 1 - i;

        // Calculate the starting position of the row in the raw data
        int data_offset = row_index * WIDTH * BMP_PIXEL_SIZE;

        // Write the pixel data for the row
        if (fwrite(data + data_offset, 1, row_size, fp) != row_size) {
            fprintf(stderr, "Error writing BMP pixel data row %d.\n", i);
            fclose(fp);
            return 0;
        }

        // Write padding bytes
       // if (padding > 0) {
         //   if (fwrite(zero_pad, 1, padding) != padding) {
         //       fprintf(stderr, "Error writing BMP padding for row %d.\n", i);
        //        fclose(fp);
        //        return 0;
        //    }
        //}
    }

    fclose(fp);
    return 1;
}

// --- WAV File I/O ---

// Function to write the WAV header
void write_wav_header(FILE* fp, unsigned int totalSamples) {
    WAVHeader header;
    unsigned int dataSize = totalSamples * (BITS_PER_SAMPLE / 8);
    unsigned int fileSize = 36 + dataSize;

    // RIFF chunk
    memcpy(header.chunkID, "RIFF", 4);
    header.chunkSize = fileSize;
    memcpy(header.format, "WAVE", 4);

    // fmt sub-chunk
    memcpy(header.subchunk1ID, "fmt ", 4);
    header.subchunk1Size = 16;
    header.audioFormat = 1; // PCM
    header.numChannels = 1; // Mono
    header.sampleRate = SAMPLE_RATE;
    header.bitsPerSample = BITS_PER_SAMPLE;
    header.byteRate = SAMPLE_RATE * 1 * (BITS_PER_SAMPLE / 8);
    header.blockAlign = 1 * (BITS_PER_SAMPLE / 8);

    // data sub-chunk
    memcpy(header.subchunk2ID, "data", 4);
    header.subchunk2Size = dataSize;

    fwrite(&header, sizeof(WAVHeader), 1, fp);
}

// --- NTSC Encoding and Decoding ---

// 1. BMP -> WAV Encoding
void encode_ntsc(unsigned char* bmp_data, short* wav_samples) {
    int sample_index = 0;
    double time = 0.0;

    // Use the defined burst phase
    double chroma_burst_phase = CHROMA_ENCODE_BURST_PHASE;

    for (int y = 0; y < HEIGHT; y++) {
        // --- 1. Horizontal Sync Pulse ---
        for (int i = 0; i < SAMPLES_H_SYNC; i++) {
            wav_samples[sample_index++] = (short)(SYNC_LEVEL * MAX_AMPLITUDE);
            time += 1.0 / SAMPLE_RATE;
        }

        // --- 2. Back Porch & Color Burst ---
        // Color Burst is a fixed-frequency, fixed-phase signal.
        for (int i = 0; i < SAMPLES_BACK_PORCH; i++) {
            // Signal Level: Blanking Level
            double signal = BLANKING_LEVEL;

            // Add Color Burst (fixed 8-cycle signal centered around blanking)
            // Note: Uses the ENCODE_CHROMA_FREQ (5512.5 Hz)
            signal += 0.1 * sin(TWO_PI * ENCODE_CHROMA_FREQ * time + chroma_burst_phase);

            // Write sample
            wav_samples[sample_index++] = (short)(signal * MAX_AMPLITUDE);
            time += 1.0 / SAMPLE_RATE;
        }

        // --- 3. Video Data (Luminance + Modulated Chroma) ---
        for (int x = 0; x < WIDTH; x++) {
            // Get the current pixel's RGB data (BMP is BGR)
            int data_offset = (y * WIDTH + x) * BMP_PIXEL_SIZE;
            unsigned char B = bmp_data[data_offset];
            unsigned char G = bmp_data[data_offset + 1];
            unsigned char R = bmp_data[data_offset + 2];

            YUV yuv;
            RGBtoYUV(R, G, B, &yuv);

            // Luminance (Y) is the base signal level (clamped between black and white level)
            double luminance = yuv.Y;

            // U and V (Chrominance) modulate the chroma carrier
            double chroma_phase = atan2(yuv.V, yuv.U); // Phase angle
            double chroma_amplitude = sqrt(yuv.U * yuv.U + yuv.V * yuv.V); // Amplitude

            // Modulate the signal over SAMPLES_PER_PIXEL
            for (int i = 0; i < SAMPLES_PER_PIXEL; i++) {
                // The instantaneous phase of the subcarrier relative to the burst reference
                double instantaneous_phase = TWO_PI * ENCODE_CHROMA_FREQ * time;

                // Add the modulated chroma component
                double signal = luminance;
                signal += chroma_amplitude * sin(instantaneous_phase + chroma_phase);

                // Clamp to prevent overflow noise (safety clamp to -1.0 to 1.0)
                if (signal > 1.0) signal = 1.0;
                if (signal < -1.0) signal = -1.0;

                wav_samples[sample_index++] = (short)(signal * MAX_AMPLITUDE);
                time += 1.0 / SAMPLE_RATE;
            }
        }

        // --- 4. Front Porch ---
        for (int i = 0; i < SAMPLES_FRONT_PORCH; i++) {
            wav_samples[sample_index++] = (short)(BLANKING_LEVEL * MAX_AMPLITUDE);
            time += 1.0 / SAMPLE_RATE;
        }
    }
}

/**
 * @brief Applies a simple 5-tap moving average filter (LPF) to the input signal.
 * @param input_samples The raw 16-bit WAV samples.
 * @param luma_output The array to store the filtered, normalized (double) signal.
 * @param num_samples The total number of samples.
 */
void apply_lpf_filter(const short* input_samples, double* luma_output, int num_samples) {
    // 5-tap simple moving average (LPF) filter: [1/5, 1/5, 1/5, 1/5, 1/5]
    for (int n = 0; n < num_samples; n++) {
        double sum = 0.0;

        // Iterate over the 5-tap window, centered at n
        for (int k = -FILTER_MID_POINT; k <= FILTER_MID_POINT; k++) {
            int index = n + k;

            // Handle boundary conditions by clamping to the nearest valid index
            if (index < 0) {
                 index = 0;
            } else if (index >= num_samples) {
                 index = num_samples - 1;
            }

            // Add the normalized sample value to the sum
            sum += (double)input_samples[index] / MAX_AMPLITUDE;
        }

        // Normalize the sum by the filter tap count (5)
        luma_output[n] = sum / FILTER_TAP_COUNT;
    }
}


// 2. WAV -> BMP Decoding
// Now accepts the desired demodulation frequency as an argument.
void decode_ntsc(short* wav_samples, unsigned char* decoded_bmp_data, double demod_freq, double Amplitude, int Interference) {
    // --- 0. Pre-filter the Luminance signal ---
    double* luma_filtered = (double*)malloc(TOTAL_SAMPLES * sizeof(double));
    if (!luma_filtered) {
        fprintf(stderr, "Failed to allocate memory for filtered Luma signal.\n");
        return;
    }

    // Apply the Low-Pass Filter (LPF)
    apply_lpf_filter(wav_samples, luma_filtered, TOTAL_SAMPLES);

    // --- PLL Variables ---
    double pll_phase = 0.0;

    for (int y = 0; y < HEIGHT; y++) {
        int line_start_sample = y * SAMPLES_PER_LINE;
        int back_porch_start = line_start_sample + SAMPLES_H_SYNC;
        int video_start = back_porch_start + SAMPLES_BACK_PORCH;

        // --- 1. PLL Tracking (Back Porch - Color Burst) ---
        // PLL is tuned to the incoming demod_freq
        for (int i = 0; i < SAMPLES_BACK_PORCH; i++) {
            int sample_index = back_porch_start + i;
            double time = (double)sample_index / SAMPLE_RATE;
            double signal_raw = (double)wav_samples[sample_index] / Amplitude;

            if(Interference > 100)
            {
                Interference = 100;
            }

            if(Interference > 0)
            {
            signal_raw += (rand() % Interference);
            }

            // The PLL reference is based on the tunable demod_freq
            double instantaneous_carrier_phase = TWO_PI * demod_freq * time;

            // Phase Detector: Mix with 90-degree shifted carrier
            double error_signal = signal_raw * cos(instantaneous_carrier_phase + pll_phase);

            // Integrator: Update the PLL phase based on the error (negative feedback)
            pll_phase += PLL_GAIN * error_signal;

            pll_phase = fmod(pll_phase, TWO_PI);
            if (pll_phase < 0) pll_phase += TWO_PI;
        }

        double tracked_chroma_phase = pll_phase;

        // Loop through pixels in the video segment
        for (int x = 0; x < WIDTH; x++) {
            int pixel_start_sample = video_start + x * SAMPLES_PER_PIXEL;

            double avg_U_demod = 0.0;
            double avg_V_demod = 0.0;
            double avg_Y_filtered = 0.0;

            for (int i = 0; i < SAMPLES_PER_PIXEL; i++) {
                int sample_index = pixel_start_sample + i;
                double time = (double)sample_index / SAMPLE_RATE;

                double signal_raw = (double)wav_samples[sample_index] / Amplitude;

                avg_Y_filtered += luma_filtered[sample_index];

                // 2. Chrominance (U/V) Demodulation
                // Uses the tunable demod_freq for the local oscillator
                double instantaneous_phase = TWO_PI * demod_freq * time;

                // Local Oscillator References, corrected by the PLL-tracked phase
                double ref_cos = cos(instantaneous_phase + tracked_chroma_phase);
                double ref_sin = sin(instantaneous_phase + tracked_chroma_phase);

                avg_U_demod += signal_raw * ref_cos;
                avg_V_demod += signal_raw * ref_sin;
            }

            // Average the accumulated values
            avg_Y_filtered /= SAMPLES_PER_PIXEL;
            avg_U_demod /= SAMPLES_PER_PIXEL;
            avg_V_demod /= SAMPLES_PER_PIXEL;

            // Reconstruct YUV
            YUV yuv;
            yuv.Y = avg_Y_filtered;

            yuv.U = avg_U_demod * CHROMA_U_DECODE_FACTOR;
            yuv.V = avg_V_demod * CHROMA_V_DECODE_FACTOR;

            if (yuv.Y < SYNC_LEVEL) yuv.Y = SYNC_LEVEL;
            if (yuv.Y > WHITE_LEVEL) yuv.Y = WHITE_LEVEL;

            // Convert back to RGB
            unsigned char R, G, B;
            YUVtoRGB(yuv, &R, &G, &B);

            // Write to the output BMP data (BGR order)
            int data_offset = (y * WIDTH + x) * BMP_PIXEL_SIZE;
            decoded_bmp_data[data_offset] = B;
            decoded_bmp_data[data_offset + 1] = G;
            decoded_bmp_data[data_offset + 2] = R;
        }
    }

    // --- 8. Cleanup ---
    free(luma_filtered);
}

struct _Broadcaster* Create_Broadcaster()
{
    struct _Broadcaster* I = (struct _Broadcaster*) calloc(1,sizeof (struct _Broadcaster));
    return I;
};

void Encode_Image(struct _Broadcaster* Broadcaster,float Frequency, float Broadcast_Strength)
{
    Broadcaster->Signal_Received = false;
    Broadcaster->Image = Create_Image();

    Broadcaster->Image->Width = WIDTH;
    Broadcaster->Image->Height = HEIGHT;

    int RGB_Image_Size = ((Broadcaster->Image->Width) * (Broadcaster->Image->Height) * 3);
    Broadcaster->Image->RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size,sizeof(unsigned char));

    double demod_frequency = Frequency;
    Broadcaster->original_bmp_data = read_bmp("input.bmp");
    Broadcaster->wav_samples = (short *)calloc(TOTAL_SAMPLES,sizeof(short));
    encode_ntsc(Broadcaster->original_bmp_data, Broadcaster->wav_samples);

    Broadcaster->No_Signal = Create_Image();
    Load_Image(Broadcaster->No_Signal, "no_signal.bmp");
};


void Receive_Image(struct _Broadcaster* Broadcaster, float Frequency, float Broadcast_Strength, double Amplitude, int Interference)
{
    if(Amplitude > 32768)
    {
        Amplitude = 32768;
    }
    double demod_frequency = Frequency;
    unsigned int decoded_data_size = WIDTH * HEIGHT * BMP_PIXEL_SIZE;
    Broadcaster->decoded_bmp_data = (unsigned char *)calloc(decoded_data_size, sizeof(unsigned char));
    decode_ntsc(Broadcaster->wav_samples, Broadcaster->decoded_bmp_data, demod_frequency, Amplitude, Interference);

    for(int j = 0; j < 128*128*3; j++)
    {
        //Broadcaster->decoded_bmp_data[j] = 128;
    }
};

void initFBO(struct _Broadcaster* Broadcaster)
{

};

void setupFBO(struct _Engine* Engine) {
    glGenFramebuffers(1, &Engine->On_Foot_State->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, Engine->On_Foot_State->fbo);

    glGenTextures(1, &Engine->On_Foot_State->textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, Engine->On_Foot_State->textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256,256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Engine->On_Foot_State->textureColorBuffer, 0);

    glGenRenderbuffers(1, &Engine->On_Foot_State->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, Engine->On_Foot_State->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 256,256);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Engine->On_Foot_State->rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer Error!\n");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Draw_Camera_Scene(struct _Engine* Engine)
{
    glBindFramebuffer(GL_FRAMEBUFFER, Engine->On_Foot_State->Broadcaster->fboId);
    glViewport(0, 0, TEX_WIDTH, TEX_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)TEX_WIDTH / (float)TEX_HEIGHT, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0, 10.0, 10.0,  // Eye
              0.0, 0.0, 00.0,  // Center
              0.0, 0.0, 1.0); // Up
    // Draw a spinning Teapot
    glPushMatrix();
        Render_Screen_Model(Engine->On_Foot_State->Rigid_Body_Test_Block,Engine->On_Foot_State->Broadcaster->textureId,90,90,90,0,0,0,false,1,1,1);
    glPopMatrix();

    // Unbind FBO - return to default window system framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Reset viewport to window dimensions
    glViewport(0, 0, 640, 360);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)640 / (float)360, 1.0f, 100.0f);
};


void Render_Screen_Model(struct _MD2_Model* Model, GLuint textureId, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    float Color[3];
    Color[0] = R;
    Color[1] = G;
    Color[2] = B;

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    int n = 0;

    int i, j;
    GLfloat s, t;
    Vector v;
    struct Frame *pframe;
    struct Vertex *pvert;
  /* Check if n is in a valid range */
    if ((n < 0) || (n > Model->Number_Of_Frames - 1))
    return;

  /* Enable model's texture */
    glBindTexture (GL_TEXTURE_2D, Model->Texture_ID);

  /* Draw the model */
    glBegin (GL_TRIANGLES);
    /* Draw each triangle */
    for (int m = 0; m < Model->Number_Of_Triangles; m++)
    {
	/* Draw each vertex */
	for (int j = 0; j < 3; j++)
    {
        pframe = &Model->Frame_Data[n];
        pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[j]];
	    /* Compute texture coordinates */
	    t = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].x / 256;
	    s = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].y / 256;
	    /* Pass texture coordinates to OpenGL */
	    glTexCoord2f (s, t);
	    /* Nrmal vector */
	    glNormal3fv (Model->anorms_table[pvert->Normal_Index]);
	    /* Calculate vertex real position */
	    v[0] = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
	    v[1] = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
	    v[2] = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];
	    if(Vertex_Coloring)
	    {
           // glColor3f(Color[0],Color[1],Color[2]);
	    }
	    glVertex3fv (v);
	  }
	  //drawBillboard();
      }
  glEnd();
 // glPopMatrix();

    glDisable(GL_TEXTURE_2D);
};


