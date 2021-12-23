#ifndef ARIFMETICALCODEC_H
#define ARIFMETICALCODEC_H

#include <QObject>
#include "Structures.h"
class ArifmeticalCodec : QObject
{
   ///////////////////////////////////////////////// Q_OBJECT
private:
    int max_freqency = 16383;

    void bitsPlusFollow(bool bit, int bits_to_follow, bitset<8>& temp_bits, int& ptr_of_temp_bits, vector<unsigned char>& compressed_buffer);

    void freqencyFullUpdate(vector< pair <int, unsigned char> >& vect, long long sum_of_letters);
    //cout << endl <<frequency.size() << endl;
    long long maxUint = 65535;
    //max_uint /= 10000000;
    long long start = 0;
    long long end = maxUint;
    long long newStart;
    long long newEnd = 0;
    long long firstQtr = ((long long)maxUint) / 4+1;
    long long half = firstQtr * 2;
    long long thirdQtr = firstQtr * 3;
    long long bitsToFollow = 0;
    bitset<8> tempBits;
    int ptrOfTempBits = 0;
    int letterNum = 0;


    frequencyStructure tempStructure;
    double prDTempFreqency = 0;
    friquency_structure_double dTempFreqency;

    unsigned long long t_start = 0;
    unsigned long long t_end = maxUint;

    vector <pair<long long, unsigned char>> tdecode_d_frequency;
    vector <pair<long long, unsigned char>> decode_d_frequency;
public:
    ArifmeticalCodec();
    void encode(vector<unsigned char> buffer, vector<unsigned char>& compressed_buffer,vector<  frequencyStructure >& frequency);
    void decode(vector<unsigned char>& buffer, vector<unsigned char> compressed_buffer, vector<  frequencyStructure > frequency, int uncompressed_bufer_size);
};

#endif // ARIFMETICALCODEC_H
