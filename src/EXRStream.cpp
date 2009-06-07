
#include "EXRStream.h"

#include <QIODevice>

EXRStream::EXRStream (QIODevice* dev, const QString& fname) :
    Imf::IStream(qPrintable(fname)),
    Imf::OStream(qPrintable(fname)),
    dev(dev)
{
}

// istream
bool EXRStream::read (char c[], int n)
{
    dev->read(c, n);
    return !dev->atEnd();
}

Imf::Int64 EXRStream::tellg ()
{
    return dev->pos();
}

void EXRStream::seekg (Imf::Int64 pos)
{
    dev->seek(pos);
}


// ostream
void EXRStream::write (const char c[], int n)
{
    dev->write(c, n);
}

Imf::Int64 EXRStream::tellp ()
{
    return dev->pos();
}

void EXRStream::seekp (Imf::Int64 pos)
{
    dev->seek(pos);
}
