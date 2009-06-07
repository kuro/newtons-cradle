
#include "CubeMap.h"
#include "EXRStream.h"

#include <GL/glew.h>

#include <ImfTiledRgbaFile.h>
#include <ImfArray.h>
#include <ImfEnvmap.h>

#include <QCoreApplication>
#include <QFile>

#include <QtDebug>

using namespace Imf;
using namespace Imath;

::CubeMap::CubeMap (const QString& fname)
{
    float* floats = NULL;

    GLenum target;

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, tex_id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

    QFile io (fname);
    io.open(QIODevice::ReadOnly);
    EXRStream exr_stream (&io, fname);

    TiledRgbaInputFile file (exr_stream);
    Box2i dw;
    int w, h;
    Array2D<Rgba> hpix;

    for (int level = 0; level < file.numLevels(); level++) {
        qDebug() << "level:" <<level;
        dw = file.dataWindowForLevel(level);
        w = dw.max.x - dw.min.x + 1;
        h = dw.max.y - dw.min.y + 1;

        if (w * 6 != h) {
            break;
        }

        if (!GLEW_NV_half_float && level == 0) {
            floats = new float[w*w*3];
        }

        hpix.resizeErase(h, w);
        file.setFrameBuffer(&hpix[0][0], 1, w);
        file.readTiles(0, file.numXTiles(level) - 1, 0,
                       file.numYTiles(level) - 1, level);
        for (int i = 0; i < 6; i++) {
            switch (i) {
            case 0: target = GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB; break;
            case 1: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB; break;
            case 2: target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB; break;
            case 3: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB; break;
            case 4: target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB; break;
            case 5: target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB; break;
            }
            if (GLEW_NV_half_float) {
                glTexImage2D(target, level, GL_RGBA,
                        w, w, 0, GL_RGBA, GL_HALF_FLOAT_NV,
                        &hpix[0][0] + (w*w)*i);
            } else {
                for (int y = 0; y < w; y++) {
                    for (int x = 0; x < w; x++) {
                        floats[(x + y * w) * 3 + 0] = hpix[y+i*w][x].r;
                        floats[(x + y * w) * 3 + 1] = hpix[y+i*w][x].g;
                        floats[(x + y * w) * 3 + 2] = hpix[y+i*w][x].b;
                    }
                }
                glTexImage2D(target, level, GL_RGBA,
                        w, w, 0, GL_RGB, GL_FLOAT,
                        floats);
            }
        }
        qApp->processEvents();
    }

    if (floats) {
        delete[] floats;
    }
}
