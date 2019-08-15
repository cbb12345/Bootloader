
namespace Hex2Bin
{
    partial class frMain
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.btnOpenHex = new System.Windows.Forms.Button();
            this.tbHexPath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbBinPath = new System.Windows.Forms.TextBox();
            this.btnOut = new System.Windows.Forms.Button();
            this.openHexDlg = new System.Windows.Forms.OpenFileDialog();
            this.saveBinDlg = new System.Windows.Forms.SaveFileDialog();
            this.pbConvert = new System.Windows.Forms.ProgressBar();
            this.btnConvert = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Hex文件";
            // 
            // btnOpenHex
            // 
            this.btnOpenHex.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOpenHex.Location = new System.Drawing.Point(427, 12);
            this.btnOpenHex.Name = "btnOpenHex";
            this.btnOpenHex.Size = new System.Drawing.Size(62, 23);
            this.btnOpenHex.TabIndex = 1;
            this.btnOpenHex.Text = "打开";
            this.btnOpenHex.UseVisualStyleBackColor = true;
            this.btnOpenHex.Click += new System.EventHandler(this.btnOpenHex_Click);
            // 
            // tbHexPath
            // 
            this.tbHexPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbHexPath.Location = new System.Drawing.Point(64, 13);
            this.tbHexPath.Name = "tbHexPath";
            this.tbHexPath.Size = new System.Drawing.Size(346, 21);
            this.tbHexPath.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 46);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "Bin文件";
            // 
            // tbBinPath
            // 
            this.tbBinPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.tbBinPath.Location = new System.Drawing.Point(64, 42);
            this.tbBinPath.Name = "tbBinPath";
            this.tbBinPath.Size = new System.Drawing.Size(346, 21);
            this.tbBinPath.TabIndex = 2;
            // 
            // btnOut
            // 
            this.btnOut.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.btnOut.Location = new System.Drawing.Point(427, 41);
            this.btnOut.Name = "btnOut";
            this.btnOut.Size = new System.Drawing.Size(62, 23);
            this.btnOut.TabIndex = 2;
            this.btnOut.Text = "输出";
            this.btnOut.UseVisualStyleBackColor = true;
            this.btnOut.Click += new System.EventHandler(this.btnOut_Click);
            // 
            // openHexDlg
            // 
            this.openHexDlg.Filter = "HEX文件(*.hex)|*.hex";
            this.openHexDlg.Title = "选择需要转换的HEX文件";
            // 
            // saveBinDlg
            // 
            this.saveBinDlg.Filter = "BIN文件(*.Bin)|*.Bin";
            // 
            // pbConvert
            // 
            this.pbConvert.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pbConvert.Location = new System.Drawing.Point(64, 71);
            this.pbConvert.Name = "pbConvert";
            this.pbConvert.Size = new System.Drawing.Size(346, 21);
            this.pbConvert.Step = 1;
            this.pbConvert.TabIndex = 3;
            // 
            // btnConvert
            // 
            this.btnConvert.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnConvert.Location = new System.Drawing.Point(427, 70);
            this.btnConvert.Name = "btnConvert";
            this.btnConvert.Size = new System.Drawing.Size(62, 23);
            this.btnConvert.TabIndex = 4;
            this.btnConvert.Text = "转换";
            this.btnConvert.UseVisualStyleBackColor = true;
            this.btnConvert.Click += new System.EventHandler(this.btnConvert_Click);
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(11, 75);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 0;
            this.label3.Text = "进度";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(11, 104);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 12);
            this.label4.TabIndex = 5;
            this.label4.Text = "解密Key";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(64, 104);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(346, 47);
            this.textBox1.TabIndex = 6;
            // 
            // frMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(501, 154);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.btnConvert);
            this.Controls.Add(this.pbConvert);
            this.Controls.Add(this.tbBinPath);
            this.Controls.Add(this.tbHexPath);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnOut);
            this.Controls.Add(this.btnOpenHex);
            this.Controls.Add(this.label1);
            this.MaximizeBox = false;
            this.Name = "frMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Hex2Bin";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnOpenHex;
        private System.Windows.Forms.TextBox tbHexPath;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbBinPath;
        private System.Windows.Forms.Button btnOut;
        private System.Windows.Forms.OpenFileDialog openHexDlg;
        private System.Windows.Forms.SaveFileDialog saveBinDlg;
        private System.Windows.Forms.ProgressBar pbConvert;
        private System.Windows.Forms.Button btnConvert;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox1;
    }
}

