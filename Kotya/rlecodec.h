#ifndef RLECODEC_H
#define RLECODEC_H
#include "Structures.h"
#include <QObject>
class RleCodec : QObject
{
    Q_OBJECT
public:
    RleCodec();
    void encode(vector<unsigned char> buffer, vector<unsigned char>& compressed_buffer, unsigned char& delimiter);
    void decode(vector<unsigned char>& buffer, vector<unsigned char> compressed_buffer, unsigned char delimiter);

    void addLetter(long long &letterRepeatAmount, vector<unsigned char>& compressedBuffer, unsigned char delimiter, unsigned char repeatableLetter);
};

#endif // RLECODEC_H
