using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KotyaArchivator
{
    
    static class Program
    {
        /// <summary>
        /// Главная точка входа для приложения.
        /// </summary>
        [STAThread]
        static void Main()
        {


            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Encoder en = new Encoder();

              en.encode("C:/Kotya/TestFiles/test3.txt");

             foreach(bool bit in en.getDatainBits())
            {
                if(bit)
                    Console.Write(1);
                else
                    Console.Write(0);
         
            }

            Console.WriteLine();

            Application.Run(new Form1());
            
        }
    }
}
