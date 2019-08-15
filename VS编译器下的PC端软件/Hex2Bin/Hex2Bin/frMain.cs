#define Encrypt
//#define Decrypt
//#define DebugOut

using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Globalization;
using System.Diagnostics;


namespace Hex2Bin
{
    public partial class frMain : Form
    {
        String szBinPath = "";
        String szHexPath = "";
        public frMain()
        {
            InitializeComponent();
        }

        private void btnOpenHex_Click(object sender, EventArgs e)
        {

            try
            {
                pbConvert.Value = 0;
                if (openHexDlg.ShowDialog() == DialogResult.OK) //打开转换的目标文件
                {
                    szHexPath = openHexDlg.FileName;
                    tbHexPath.Text = szHexPath;
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void btnConvert_Click(object sender, EventArgs e)
        {
            try
            {
                String szLine = "";
                String szHex = "";
                if (szHexPath == "")
                {
                    MessageBox.Show("请选择需要转换的目标文件!         ", "错误");
                    return;
                }

#region 解密Key提取
                byte[] KeyBuffer = AESDLL.StringToByteArray(textBox1.Text);
                if (KeyBuffer == null) return;
                if (KeyBuffer.Length != 32)
                {
                    MessageBox.Show("解密Key设置有误！");
                    return;
                }
                AESDLL.SetKey(ref KeyBuffer[0]);
    #if DebugOut
                Debug.Print(byteToHexStr(KeyBuffer));
    #endif
#endregion

                StreamReader HexReader = new StreamReader(szHexPath);
                
                while (true)
                {
                    szLine = HexReader.ReadLine(); //读取一行数据

                    if (szLine == null) //读完所有行
                    {
                        break;
                    }
                    if (szLine.Substring(0, 1) == ":") //判断第1字符是否是:
                    {
                        if (szLine.Substring(1, 8) == "00000001")   //数据结束
                        {
                            break;
                        }
                        else if (szLine.Substring(7, 2) != "00")    //非数据字段
                        {
                            continue;
                        }
                        else
                        {
                            szHex += szLine.Substring(9, szLine.Length - 11); //读取有效字符
                        }
                    }

                }

                HexReader.Close(); //关闭目标文件
                Int32 i;
                Int32 j = 0;
                Int32 Length = szHex.Length;
                byte[] szBin = new byte[Length/2];
                pbConvert.Maximum = Length / 2;

                for (i = 0; i < Length; i += 2) //两字符合并成一个16进制字节
                {
                    szBin[j] = (byte)Int16.Parse(szHex.Substring(i, 2), NumberStyles.HexNumber);
                    j++;
                    pbConvert.Increment(i);
                    
                }

#region 创建加密缓存及加密处理
    #if Encrypt
                if (szBin.Length % 16 == 0)
                {
                    Length = szBin.Length / 16;
                }
                else
                {
                    Length = szBin.Length / 16 + 1;
                }
                byte[] buffer = new byte[Length * 16];
                byte[] bufferEnc = new byte[Length * 16];
                byte[] bufferDec = new byte[Length * 16];
                byte[] bufferIn = new byte[16];
                byte[] bufferOut = new byte[16];
                for (i = 0; i < buffer.Length; i++)
                {
                    if (i < szBin.Length)
                    {
                        buffer[i] = szBin[i];
                    }
                    else
                    {
                        buffer[i] = 0xff;
                    }
                    bufferEnc[i] = 0x00;
                    bufferDec[i] = 0x00;
                }

                for (i = 0; i < 16; i++)
                {
                    bufferIn[i] = 0;
                    bufferOut[i] = 0;
                }
                AESDLL.aesEncInit();
                for (i = 0; i < Length; i++)
                {
                    for (j = 0; j < 16; j++)
                    {
                        bufferIn[j] = buffer[i * 16 + j];
                    }
                    AESDLL.aesEncrypt(ref bufferIn[0], ref bufferOut[0]);
                    for (j = 0; j < 16; j++)
                    {
                        bufferEnc[i * 16 + j] = bufferIn[j];
                    }
                }
    #region 解密数据
        #if Decrypt
                for (i = 0; i < 16; i++)
                {
                    bufferIn[i] = 0;
                    bufferOut[i] = 0;
                }
                AESDLL.aesDecInit();
                for (i = 0; i < Length; i++)
                {
                    for (j = 0; j < 16; j++)
                    {
                        bufferIn[j] = bufferEnc[i * 16 + j];
                    }
                    AESDLL.aesDecrypt(ref bufferIn[0], ref bufferOut[0]);
                    for (j = 0; j < 16; j++)
                    {
                        bufferDec[i * 16 + j] = bufferIn[j];
                    }
                }
        #endif
    #endregion
    #region 调试数据输出
        #if DebugOut
                for (i = 0; i < Length; i++)
                {
                    byte[] temp = new byte[16];
                    for (j = 0; j < 16; j++)
                    {
                        temp[j] = buffer[i * 16 + j];
                    }
                    Debug.Print(AESDLL.ToHexString(temp));

                    for (j = 0; j < 16; j++)
                    {
                        temp[j] = bufferEnc[i * 16 + j];
                    }
                    Debug.Print(AESDLL.ToHexString(temp));

                    for (j = 0; j < 16; j++)
                    {
                        temp[j] = bufferDec[i * 16 + j];
                    }
                    Debug.Print(AESDLL.ToHexString(temp));
                    Debug.Print("-------------------------------------------------------------------");
                }
        #endif
    #endregion
#endif
#endregion

                if (szBinPath == "")
                {
                    szBinPath = Path.ChangeExtension(szHexPath, "Bin");
                    tbBinPath.Text = szBinPath;
                }
                FileStream fBin = new FileStream(szBinPath, FileMode.Create); //创建文件BIN文件
                BinaryWriter BinWrite = new BinaryWriter(fBin); //二进制方式打开文件
#region 数据写入文件
    #if Encrypt
        #if Decrypt
                BinWrite.Write(bufferDec, 0, bufferDec.Length);
        #else
                BinWrite.Write(bufferEnc, 0, bufferEnc.Length); //写入数据
        #endif
    #else
                BinWrite.Write(szBin,0,szBin.Length);
    #endif
    #endregion
                BinWrite.Flush();//释放缓存
                BinWrite.Close();//关闭文件

                MessageBox.Show("文件转换完成!        ", "提示");
                pbConvert.Value = 0;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
            
        }

        private void btnOut_Click(object sender, EventArgs e)
        {
            try
            {
                if (saveBinDlg.ShowDialog() == DialogResult.OK)
                {

                    szBinPath = saveBinDlg.FileName;
                    tbBinPath.Text = szBinPath;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }
    }
}
