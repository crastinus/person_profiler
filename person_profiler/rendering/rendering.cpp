/*
#include <core/SkImage.h>
#include <core/SkSurface.h>

#include <core/SkCanvas.h>
#include <core/SkPath.h>

void example(SkCanvas* canvas) {
const SkScalar scale = 256.0f;
const SkScalar R = 0.45f * scale;
const SkScalar TAU = 6.2831853f;
SkPath path;
for (int i = 0; i < 5; ++i) {
SkScalar theta = 2 * i * TAU / 5;
if (i == 0) {
path.moveTo(R * cos(theta), R * sin(theta));
}
else {
path.lineTo(R * cos(theta), R * sin(theta));
}
}
path.close();
SkPaint p;
p.setAntiAlias(true);
canvas->clear(SK_ColorWHITE);
canvas->translate(0.5f * scale, 0.5f * scale);
canvas->drawPath(path, p);
}

void raster(int width, int height, char const* path) {
sk_sp<SkSurface> rasterSurface = SkSurface::MakeRasterN32Premul(width, height);
SkCanvas* rasterCanvas = rasterSurface->getCanvas();

example(rasterCanvas);

sk_sp<SkImage> img(rasterSurface->makeImageSnapshot());
if (!img) {
return;
}


sk_sp<SkData> png(img->encodeToData());
if (!png) {
return;
}
SkFILEWStream out(path);
out.write(png->data(), png->size());

}
*/
