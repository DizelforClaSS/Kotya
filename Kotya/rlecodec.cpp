#include "rlecodec.h"

RleCodec::RleCodec()
{

}

void RleCodec:: addLetter(long long &letterRepeatAmount, vector<unsigned char>& compressedBuffer,
                          unsigned char delimiter, unsigned char repeatableLetter)
{
    long long tLetterAmountRepeat;
    while (letterRepeatAmount != 0)
    {
        if (letterRepeatAmount >= 4)
        {
            if (letterRepeatAmount > 256)
            {
                tLetterAmountRepeat = 255;
                letterRepeatAmount -= 256;
            }
            else
            {
                tLetterAmountRepeat = letterRepeatAmount - 1;
                letterRepeatAmount = 0;
            }
            compressedBuffer.push_back(delimiter);
            compressedBuffer.push_back(tLetterAmountRepeat);
            compressedBuffer.push_back(repeatableLetter);
        }
        else
        {
            if (repeatableLetter == delimiter)
            {
                compressedBuffer.push_back(repeatableLetter);
                compressedBuffer.push_back(letterRepeatAmount - 1);
            }
            else
            {
                for (int i = 0; i < letterRepeatAmount; i++)
                {
                    compressedBuffer.push_back(repeatableLetter);
                }
            }
            letterRepeatAmount = 0;
        }
    }
}

void RleCodec::encode(vector<unsigned char> buffer, vector<unsigned char>& compressedBuffer, unsigned char& delimiter)
{
    vector< pair <int, unsigned char> > vect(256);

    for (int i = 0; i < vect.size(); i++)
    {
        vect[i].second = i;
    }

    for (int i = 0; i < buffer.size(); i++)
    {
        vect[buffer[i]].first++;
    }

    sort(vect.begin(), vect.end());

    delimiter = vect[0].second;
    long long repeatableLetterAmount = 1;

    unsigned char repeatableLetter;
    repeatableLetter = buffer[0];

    for (int i = 1; i < buffer.size(); i++)
    {
        if (buffer[i] == repeatableLetter)
        {
            repeatableLetterAmount++;
        }
        else
        {
            addLetter(repeatableLetterAmount, compressedBuffer, delimiter, repeatableLetter);
            repeatableLetter = buffer[i];
            repeatableLetterAmount++;
        }
    }
    addLetter(repeatableLetterAmount, compressedBuffer, delimiter, repeatableLetter);
}

void RleCodec::decode(vector<unsigned char> &buffer, vector<unsigned char> compressed_buffer, unsigned char delimiter)
{
    long long number;
    int level = 0;
    for (int i = 0; i < compressed_buffer.size(); i++)
    {
        if (compressed_buffer[i] == delimiter || level != 0)
        {
            if (level == 0)
            {
                level = 1;
            }
            else if (level == 1)
            {
                number = compressed_buffer[i];
                if (number < 3)
                {
                    level = 0;
                    for (int j = 0; j <= number; j++)
                    {
                        buffer.push_back(delimiter);
                    }
                }
                else
                {
                    level = 2;
                }
            }
            else
            {
                for (int j = 0; j <= number; j++)
                {
                    buffer.push_back(compressed_buffer[i]);
                }
                level = 0;
            }

        }
        else
        {
            buffer.push_back(compressed_buffer[i]);
        }
    }
}
