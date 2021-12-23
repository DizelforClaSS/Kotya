#ifndef SHENONFANOCODEC_H
#define SHENONFANOCODEC_H

#include <QObject>
#include "Structures.h"
class ShenonFanoCodec : QObject
{
    ////////////////////////////////////////////////////////Q_OBJECT
private:
    void mapTransforming(map<unsigned char, vector<bool>> table, map<string, unsigned char>& table_for_decoding, vector< frequencyStructure> frequency);
    void stepOfAlgorithm(map<unsigned char, vector<bool>>& table, vector< frequencyStructure >& part, int start = 0, int end = 0);

public:
    ShenonFanoCodec();
    void encode(vector<unsigned char> buffer, vector<unsigned char>& compressed_buffer, vector<  frequencyStructure >& frequency);
    void adaptiveEncode(vector<unsigned char> buffer, vector<unsigned char>& compressed_buffer);
    void decode(vector<unsigned char>& buffer, vector<unsigned char> compressed_buffer, vector< frequencyStructure> frequency, int uncompressedSize);
    void adaptiveDecode(vector<unsigned char>& buffer, vector<unsigned char> compressed_buffer, int uncompressedSize);
};

#endif // SHENONFANOCODEC_H
