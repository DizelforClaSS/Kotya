using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
namespace KotyaArchivator
{
    class symbolInfo
    {
        public uint amount = 0;
        public uint amountBits = 0;
        public uint code; // code in Integer
        public double size = 0;
        public char symbol;
        public BitArray bits; //code in Bits

        public symbolInfo()
        {

        }
    }
}
