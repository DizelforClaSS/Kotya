#ifndef HEMMINGCODEC_H
#define HEMMINGCODEC_H

#include <QObject>
#include "Structures.h"
class HemmingCodec : public QObject
{
 Q_OBJECT
public:
    HemmingCodec();

    void encoding(vector<unsigned char> buffer, vector<unsigned char>& compressedBuffer);


    void fillMatrix();
    void addinationallByte(unsigned char &additionalByte, vector<int> tempBuffer);
    void fix(vector<int> &tempBuffer, int &errorBit);
    void decoding(vector<unsigned char> &buffer, vector<unsigned char> compressedBuffer, int uncompressedSize);
};

#endif // HEMMINGCODEC_H
