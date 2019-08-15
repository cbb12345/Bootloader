using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Hex2Bin
{
    class AESDLL
    {
        /// <summary>
        /// AES加密初始化
        /// </summary>
        [DllImport("AES256.dll")]
        public static extern void aesEncInit();
        /// <summary>
        /// AES加密
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="chainBlock"></param>
        [DllImport("AES256.dll")]
        public static extern void aesEncrypt(ref byte buffer,ref byte chainBlock );
        /// <summary>
        /// AES解密初始化
        /// </summary>
        [DllImport("AES256.dll")]
        public static extern void aesDecInit();
        /// <summary>
        /// AES解密
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="chainBlock"></param>
        [DllImport("AES256.dll")]
        public static extern void aesDecrypt(ref byte buffer, ref byte chainBlock);
        /// <summary>
        /// 设置解密Key
        /// </summary>
        /// <param name="KeyBuffer"></param>
        [DllImport("AES256.dll")]
        public static extern void SetKey(ref byte KeyBuffer);

        public static string ToHexString(byte[] bytes)
        {
            string hexString = string.Empty;
            if (bytes != null)
            {
                StringBuilder strB = new StringBuilder();

                for (int i = 0; i < bytes.Length; i++)
                {
                    strB.Append(bytes[i].ToString("X2") + " ");
                }
                hexString = strB.ToString();
            }
            return hexString;
        }

        /// <summary>
        /// 将String包内容传进去,然后返回Byte[]
        /// </summary>
        /// <param name="str">需要发包的明文字符串</param>
        /// <returns></returns>
        public static byte[] StringToByteArray(string str)
        {
            List<byte> bytelist = new List<byte>();
            str = str.Replace(" ", "");//去空格
            int length = str.Length / 2;//包长度
            try
            {
                for (int i = 0; i < length; i++)
                {
                    bytelist.Add(Convert.ToByte(str.Substring(2 * i, 2), 16));
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
                return (null);
            }
            return bytelist.ToArray();
        }
    }
}
