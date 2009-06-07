
#include <ImfIO.h>
#include <QString>

class QIODevice;

class EXRStream : public Imf::IStream, public Imf::OStream
{
public:
    EXRStream (QIODevice* dev, const QString& fname);

public:
    // istream
    virtual bool read (char c[], int n);
    virtual Imf::Int64 tellg ();
    virtual void seekg (Imf::Int64 pos);

    // ostream
    virtual void write (const char c[], int n);
    virtual Imf::Int64 tellp ();
    virtual void seekp (Imf::Int64 pos);

protected:
    QIODevice* dev;
};
