#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define RENDER_WIDTH   640
#define RENDER_HEIGHT  480

typedef struct pixel_s {
    uint8_t r, g, b;
} pixel_t;

typedef struct image_s {
    pixel_t *pixels;
    int width, height;
} image_t;

static image_t *image_new(int width, int height) {
    image_t *i;

    i = malloc(sizeof (*i));
    if (i == NULL) {
        fprintf(stderr, "image_new(): malloc(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    i->width  = width;
    i->height = height;

    i->pixels = calloc(1, width * height * sizeof (pixel_t));
    if (i->pixels == NULL) {
        fprintf(stderr, "image_new(): malloc(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return (i);
}

static void image_fwrite(FILE *fp, const image_t *img) {
    size_t ret;
    size_t pixels_sz;

    fprintf(fp, "P6\n%d %d\n255\n", img->width, img->height);

    pixels_sz = img->width * img->height * sizeof (pixel_t);

    ret = fwrite(img->pixels, pixels_sz, 1, fp);
    if (ret != 1) {
        fprintf(stderr, "fwrite_image(): fwrite(): could not write data.\n");
        exit(EXIT_FAILURE);
    }
}

static void image_save_file(const image_t *img, const char *file) {
    FILE *fp;
    int ret;

    fp = fopen(file, "w");
    if (fp == NULL) {
        fprintf(stderr,
                "save_image_file(): fopen(): ERROR %i: %s.\n",
                errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    image_fwrite(fp, img);

    ret = fclose(fp);
    if (ret != 0) {
        fprintf(stderr,
                "save_image_file(): fclose(): ERROR %i: %s.\n",
                errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Wrote %s\n", file);
}

static void image_save(const image_t *img) {
    static int count = 0;
    char filename[16];

    snprintf(filename, sizeof (filename),
             "output-%03i.ppm", count);
    image_save_file(img, filename);
    count++;
}

/* Some GLSL-like functions */

typedef struct vec2_s {
    float x, y;
} vec2;

typedef struct vec3_s {
    float x, y, z;
} vec3;

static float mix(float x, float y, float a) {
    return ((x * (1.0 - a)) + (y * a));
}

static float fract(float x) {
    return (x - floorf(x));
}

static uint8_t u8_from_float(float x) {
    if (x < 0.0) return (0);
    if (x > 1.0) return (UINT8_MAX);
    return (uint8_t)(x * (float)(UINT8_MAX));
}

/* Pixel/Fragment Shader Code */

static const int maxIter = 32;
static const vec2 center = { -1., 0.};
static const vec3 outCol1 = { 0.2, 0.2, 0.4 };
static const vec3 outCol2 = { 0.8, 0.8, 1.0 };

static pixel_t
render_fragment(int frag_x, int frag_y, int res_x, int res_y, float time)
{
    float scale = 1.0 / (1.5 + sinf(time * 2.0));
    vec2 pos, z, c;

    pos.x = (2.0 * frag_x / res_x) - 1.0;
    pos.y = (2.0 * frag_y / res_y) - 1.0;

    z.x = pos.x * scale + center.x;
    z.y = pos.y * scale + center.y;

    c.x = z.x;
    c.y = z.y;

    float r2 = 0.0;
    float iterf = 0.0;

    int it;
    for (it = 0; it < maxIter; ++it) {
        float tmpr = z.x;
        z.x = (tmpr * tmpr) - (z.y * z.y) + c.x;
        z.y = 2.0 * tmpr * z.y + c.y;
        r2 = z.x * z.x + z.y * z.y;

        if (r2 > 4.0)
            break ;

        iterf += 1.;
    }

    vec3 color;
    if (r2 < 4.0) {
        color.x = color.y = color.z = 0.0;
    }
    else {
        color.x = mix(outCol1.x, outCol2.x, fract(iterf * 0.032));
        color.y = mix(outCol1.y, outCol2.y, fract(iterf * 0.032));
        color.z = mix(outCol1.z, outCol2.z, fract(iterf * 0.032));
    }

    pixel_t output;
    output.r = u8_from_float(color.x);
    output.g = u8_from_float(color.y);
    output.b = u8_from_float(color.z);

    return (output);
}

static void render_image(image_t *img, float time) {
    int x, y;
    int w = img->width;
    int h = img->height;
    pixel_t *pixels = img->pixels;

#ifdef _OPENMP
# pragma omp parallel for private(y, x) collapse(2)
#endif
    for (y = 0; y < h; ++y)
        for (x = 0; x < w; ++x)
            pixels[y * w + x] = render_fragment(x, y, w, h, time);
}

static void render_animation(float from, float to, float step) {
    image_t *img = image_new(RENDER_WIDTH, RENDER_HEIGHT);

    for (float t = from; t < to; t += step) {
        render_image(img, t);
        image_save(img);
    }
}

int main(int argc, char *argv[]) {
    render_animation(0.0, 2.0, 0.25);
    return (0);
}
