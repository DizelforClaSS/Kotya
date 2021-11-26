#include "arifmeticalcodec.h"

ArifmeticalCodec::ArifmeticalCodec()
{

}

void ArifmeticalCodec::bitsPlusFollow(bool bit, int bitsToFollow, bitset<8>& tempBits, int& ptrOfTempBits, vector<unsigned char>& compressedBuffer)
{
    unsigned long temp_for_transform;
    tempBits[ptrOfTempBits] = bit;
    if (ptrOfTempBits == 7)
    {
        ptrOfTempBits = 0;
        temp_for_transform = tempBits.to_ulong();
        compressedBuffer.push_back(static_cast<unsigned char>(temp_for_transform));
    }
    else
    {
        ptrOfTempBits++;
    }

    for (int i = 0; i < bitsToFollow; i++)
    {
        tempBits[ptrOfTempBits] = !bit;
        if (ptrOfTempBits == 7)
        {
            ptrOfTempBits = 0;
            temp_for_transform = tempBits.to_ulong();
            compressedBuffer.push_back(static_cast<unsigned char>(temp_for_transform));
        }
        else
        {
            ptrOfTempBits++;
        }
    }

}

void ArifmeticalCodec::freqencyFullUpdate(vector< pair <int, unsigned char> >& vect, long long sumOfLetters)
{

    while (sumOfLetters >= max_freqency)
    {
        sumOfLetters = 0;
        for (int i = 0; i < 256; i++)
        {
            vect[i].first = (vect[i].first + 1) / 2;
            sumOfLetters += vect[i].first;
        }
    }
}


void ArifmeticalCodec::encode(vector<unsigned char> buffer, vector<unsigned char>& compressedBuffer,
                              vector<  frequencyStructure >& frequency)
{
    vector< pair <int, unsigned char> > vect(256);
    map<unsigned char, friquency_structure_double> d_frequency;
    for (int i = 0; i < vect.size(); i++)
    {
        vect[i].second = i;
    }
    for (int i = 0; i < buffer.size(); i++)
    {
        vect[buffer[i]].first++;
    }
    long long sumOfLetters = 0;
    for (int i = 0; i < 256; i++)
    {
        sumOfLetters += vect[i].first;
    }
    freqencyFullUpdate(vect, sumOfLetters);
    sort(vect.begin(), vect.end(), greater<>());

    frequencyStructure tempStructure;
    long long prDTempFreqency = 0;
    friquency_structure_double dTempFreqency;

    for (int i = vect.size() - 1; i >= 0; i--)
    {
        if (vect[i].first != 0)
        {
            tempStructure.amount = vect[i].first;
            tempStructure.letter = vect[i].second;
            dTempFreqency.amount = vect[i].first;
            dTempFreqency.amount += prDTempFreqency;
            dTempFreqency.prev_amount = prDTempFreqency;
            prDTempFreqency = dTempFreqency.amount;
            frequency.push_back(tempStructure);
            d_frequency.emplace(make_pair(vect[i].second, dTempFreqency));
        }
    }

    while (letterNum != buffer.size())
    {
        newStart = start + (end - start + 1) * d_frequency[buffer[letterNum]].prev_amount / prDTempFreqency;
        end = start + (end - start + 1) * d_frequency[buffer[letterNum]].amount / prDTempFreqency - 1;
        start = newStart;
        while (true)
        {
            if (end < half)
            {
                bitsPlusFollow(0, bitsToFollow, tempBits, ptrOfTempBits, compressedBuffer);
                bitsToFollow = 0;
            }
            else if (start >= half)
            {
                bitsPlusFollow(1, bitsToFollow, tempBits, ptrOfTempBits, compressedBuffer);
                bitsToFollow = 0;
                start -= half;
                end -= half;
            }
            else if (start >= firstQtr && end <= thirdQtr)
            {
                bitsToFollow++;
                start -= firstQtr;
                end -= firstQtr;
            }
            else break;
            start = 2*start;
            end = 2*end+1;

        }

        letterNum++;
    }

    if (start < firstQtr)
        bitsPlusFollow(0, ++bitsToFollow, tempBits, ptrOfTempBits, compressedBuffer);
    else
        bitsPlusFollow(1, ++bitsToFollow, tempBits, ptrOfTempBits, compressedBuffer);


    unsigned long tempForTransform;

    if (ptrOfTempBits != 0)
    {
        for (int i = ptrOfTempBits; i < 8; i++)
        {
            tempBits[i] = 0;
        }
        tempForTransform = tempBits.to_ulong();
        compressedBuffer.push_back(static_cast<unsigned char>(tempForTransform));
    }
}


void ArifmeticalCodec::decode(vector<unsigned char>& buffer, vector<unsigned char> compressedBuffer,
                              vector<  frequencyStructure > frequency, int uncompressedBuferSize)
{
    //vector< pair <int, unsigned char> > vect(256);
    start = 0;
    end = maxUint;
    newEnd = maxUint;
    newStart = 0;
    //decode_d_frequency.push_back(make_pair(0, 1));
    prDTempFreqency = 0;

    for (int i = 0; i < frequency.size(); i++)
    {
        if (frequency[i].amount != 0)
        {
            dTempFreqency.amount = frequency[i].amount;
            dTempFreqency.amount += prDTempFreqency;
            dTempFreqency.prev_amount = prDTempFreqency;
            tdecode_d_frequency.push_back(make_pair(prDTempFreqency, frequency[i].letter));
            prDTempFreqency = dTempFreqency.amount;
        }
    }
    decode_d_frequency.push_back(make_pair(prDTempFreqency, 0));
    for (int i = frequency.size() - 1; i >= 0; i--)
    {
        decode_d_frequency.push_back(tdecode_d_frequency[i]);
    }
    bitset<8> temp_bits;
    long long value = 0;
    temp_bits = compressedBuffer[0];
    for (int k = 0; k < 8; k++)
    {
        value = 2 * value + temp_bits[k];
    }
    temp_bits = compressedBuffer[1];
    for (int k = 0; k < 8; k++)
    {
        value = 2 * value + temp_bits[k];
    };
    long long mid_value;
    long long letter_index;
    //temp_bits = compressed_buffer[i];
    long long bit_counter = 8;
    long long compr_buf_index = 2;
    for (int i = 0; i < uncompressedBuferSize; i++)
    {
        mid_value = (((value - start) + 1) * decode_d_frequency[0].first - 1) / (end - start + 1);
        for (letter_index = 1; decode_d_frequency[letter_index].first > mid_value; letter_index++);
        //cout << decode_d_frequency[letter_index].second;
        buffer.push_back(decode_d_frequency[letter_index].second);
        // пересчет границ
        newEnd = start + ((end - start + 1) * decode_d_frequency[letter_index - 1].first) / decode_d_frequency[0].first - 1;
        start = start + ((end - start + 1) * decode_d_frequency[letter_index].first) / decode_d_frequency[0].first;
        end = newEnd;
        // удаление очередного символа из входного потока
        while (true)
        {
            if (end < half)
            {
            }
            else if (start >= half)
            {
                value -= half;
                start -= half;
                end -= half;
            }
            else if (start >= firstQtr && end < thirdQtr)
            {
                value -= firstQtr;
                start -= firstQtr;
                end -= firstQtr;
            }
            else
                break;
            // сдвиг влево с "втягиванием очередного бита
            start += start;
            end += end + 1;
            if (bit_counter == 8)
            {
                if (compr_buf_index == compressedBuffer.size())
                {
                    temp_bits = 0;
                }
                else
                {
                    temp_bits = compressedBuffer[compr_buf_index];
                    compr_buf_index++;
                }
                bit_counter = 0;
            }
            value = 2 * value + temp_bits[bit_counter];
            bit_counter++;
        }
    }
}

