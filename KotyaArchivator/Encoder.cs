using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text;
using System.IO;
using System.Linq;
using System.Collections;

namespace KotyaArchivator
{ 
	class Encoder
    {

		//private symbolInfo[] symbols1;
		private Dictionary<char, symbolInfo> symbols;
		private uint charCount = 0;
		private Dictionary<int, double> bytes; //key: unsigned char  value: frequency char
		private BitArray dataInBits;
		private uint amountBits = 0;
		private uint amountAllBits = (uint)Math.Pow(2, 16);

        public Encoder()
        {
			symbols = new Dictionary<char, symbolInfo>();
			bytes = new Dictionary<int, double>();//Comparer<double>.Create((x, y) => y.CompareTo(x))); //Компаратор для сортировки списка по Ключу в по убыванию
			
		}
		private void createTable(string filename) 
		{
			countSymbols(filename);
			calcSize();
			devide(bytes, charCount, 1, false);

			foreach(KeyValuePair<char, symbolInfo> pair in symbols)
			{
				pair.Value.bits = new BitArray(BitConverter.GetBytes(pair.Value.code));
			}
			bytes = bytes.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
			
		}

		private void countSymbols(string filename) 
		{
			FileStream file = new FileStream(filename, FileMode.Open, FileAccess.Read);

			StreamReader reader = new StreamReader(file, Encoding.Default);

			foreach(char ch in reader.ReadToEnd())
            {
				if (!symbols.ContainsKey(ch))
					symbols.Add(ch, new symbolInfo());

				symbols[ch].amount++;
				symbols[ch].symbol = ch;
				charCount++;
			}

			

			reader.Close(); //закрываем поток
		}
		private void calcSize() 
		{


			foreach (KeyValuePair<char, symbolInfo> pair in symbols)
			{
				double p = (double)pair.Value.amount / charCount;
				pair.Value.size = -Math.Log(p, 2);
				bytes.Add(pair.Key, p);
			}
		}
		private string codeToString(int code, int amountBits)
		{
			string res = "";

			return res;
		}

		private void devide(Dictionary<int, double> group, uint sumSize, uint numNote, bool right)
		{
			Dictionary<int, double> leftPart = new Dictionary<int, double>();//(Comparer<double>.Create((x, y) => y.CompareTo(x)));
			Dictionary<int, double> rightPart = new Dictionary<int, double>();// (Comparer<double>.Create((x, y) => y.CompareTo(x)));
			

			//SortedList<double, int>::iterator it = group.begin();
			foreach (KeyValuePair<int, double> p in group)
			{
				char c = (char)p.Key;
				symbols[c].code = symbols[c].code << 1;
				if (right)
					symbols[c].code += 1;
			}

			if (group.Count == 1)
			{
				char c = (char)group.First().Key;
				symbols[c].amountBits = numNote - 1;
				return;
			}



			uint sumLeft = 0;
			uint sumRight = 0;

			if (group.Count > 2)
			{
				foreach (KeyValuePair<int, double> p in group)
				{

					char c = (char)p.Key;
					if ((sumLeft + symbols[c].amount) <= sumSize / 2)
					{
						leftPart.Add(p.Key, p.Value);
							
						sumLeft += symbols[c].amount;

					}
					else
					{
						rightPart.Add(p.Key, p.Value);
						sumRight += symbols[c].amount;
					}

					
				}
			}
			else
			{
				
				leftPart.Add(group.First().Key, group.First().Value);
				char c = (char)group.First().Key;
				sumLeft += symbols[c].amount;
				
				rightPart.Add(group.Last().Key, group.Last().Value);
				c = (char)group.Last().Key;
				sumRight += symbols[c].amount;
			}
			//printGroup(leftPart, 0);
			leftPart = leftPart.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
			devide(leftPart, sumLeft, numNote + 1, false);

			rightPart = rightPart.OrderByDescending(pair => pair.Value).ToDictionary(pair => pair.Key, pair => pair.Value);
			//printGroup(rightPart, 1);
			devide(rightPart, sumRight, numNote + 1, true);
		}
		private void printGroup(Dictionary<int, double> group, bool right)
		{ 
			
		}

		public bool encode(string filename)
		{
			createTable(filename);
			getAmountBits();

			dataInBits = new BitArray((int)amountBits);

			FileStream file = new FileStream(filename, FileMode.Open, FileAccess.Read);

			StreamReader reader = new StreamReader(file, Encoding.Default);

			int indexBits = 0; // Хранит текущее положение индекса
			foreach (char ch in reader.ReadToEnd())
			{
				BitArray bitArray = new BitArray(BitConverter.GetBytes(symbols[ch].code));
				for(int i= (int)symbols[ch].amountBits-1; i >= 0; i-- , indexBits++)
                {
					dataInBits.Set(indexBits, symbols[ch].bits.Get(i));
                }
				
			}

			reader.Close(); //закрываем поток




    //        foreach (KeyValuePair<char, symbolInfo> sym in symbols)
    //        {

    //            Console.Write(sym.Key.ToString() + " ");

				//int count = 0;
    //            foreach (bool bit in sym.Value.bits)
    //            {
    //                if (bit)
    //                    Console.Write(1);
    //                else
    //                    Console.Write(0);
				//	count++;
				//	if(count >= sym.Value.amountBits)
				//		break;
					
    //            }
    //            Console.WriteLine();
    //        }

            //foreach (KeyValuePair<int, double> byt in bytes)
            //{
            //	Console.WriteLine(byt);
            //}
            //Console.WriteLine();
            return false;
		}

		private void getAmountBits()
        {
			amountBits = 0;
			foreach (KeyValuePair<char, symbolInfo> sym in symbols)
			{
				amountBits += sym.Value.amountBits * sym.Value.amount;
			}
		}

		public BitArray getDatainBits()
        {
			return dataInBits;
        }
		public void printTable()
		{ 
			
		}
		
	}
}
