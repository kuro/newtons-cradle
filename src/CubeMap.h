
#pragma once

#include <QString>

class CubeMap
{
public:
    CubeMap (const QString& fname);

public:
    quint32 size;
    quint32 tex_id;
};
