namespace ServerFormsApp
{
    partial class FormServer
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btStart = new System.Windows.Forms.Button();
            this.tbClientIP = new System.Windows.Forms.TextBox();
            this.lbClient = new System.Windows.Forms.ListBox();
            this.lbStatic = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btRefresh = new System.Windows.Forms.Button();
            this.lbHistory = new System.Windows.Forms.ListBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btStart);
            this.groupBox1.Controls.Add(this.tbClientIP);
            this.groupBox1.Controls.Add(this.lbClient);
            this.groupBox1.Controls.Add(this.lbStatic);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(391, 339);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "监听状态";
            // 
            // btStart
            // 
            this.btStart.Location = new System.Drawing.Point(303, 308);
            this.btStart.Name = "btStart";
            this.btStart.Size = new System.Drawing.Size(75, 23);
            this.btStart.TabIndex = 7;
            this.btStart.Text = "启动客户端";
            this.btStart.UseVisualStyleBackColor = true;
            this.btStart.Click += new System.EventHandler(this.btStart_Click);
            // 
            // tbClientIP
            // 
            this.tbClientIP.Location = new System.Drawing.Point(14, 310);
            this.tbClientIP.Name = "tbClientIP";
            this.tbClientIP.Size = new System.Drawing.Size(271, 21);
            this.tbClientIP.TabIndex = 6;
            this.tbClientIP.Text = "127.0.0.1";
            this.tbClientIP.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lbClient
            // 
            this.lbClient.FormattingEnabled = true;
            this.lbClient.ItemHeight = 12;
            this.lbClient.Location = new System.Drawing.Point(14, 44);
            this.lbClient.Name = "lbClient";
            this.lbClient.Size = new System.Drawing.Size(371, 256);
            this.lbClient.TabIndex = 5;
            this.lbClient.MouseUp += new System.Windows.Forms.MouseEventHandler(this.lbClient_MouseUp);
            // 
            // lbStatic
            // 
            this.lbStatic.AutoSize = true;
            this.lbStatic.Location = new System.Drawing.Point(12, 20);
            this.lbStatic.Name = "lbStatic";
            this.lbStatic.Size = new System.Drawing.Size(173, 12);
            this.lbStatic.TabIndex = 4;
            this.lbStatic.Text = "下面显示正在运行的客户端节点";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btRefresh);
            this.groupBox2.Controls.Add(this.lbHistory);
            this.groupBox2.Location = new System.Drawing.Point(409, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(289, 339);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "系统日志";
            // 
            // btRefresh
            // 
            this.btRefresh.Location = new System.Drawing.Point(6, 310);
            this.btRefresh.Name = "btRefresh";
            this.btRefresh.Size = new System.Drawing.Size(277, 23);
            this.btRefresh.TabIndex = 7;
            this.btRefresh.Text = "刷新系统日志";
            this.btRefresh.UseVisualStyleBackColor = true;
            this.btRefresh.Click += new System.EventHandler(this.btRefresh_Click);
            // 
            // lbHistory
            // 
            this.lbHistory.FormattingEnabled = true;
            this.lbHistory.HorizontalScrollbar = true;
            this.lbHistory.ItemHeight = 12;
            this.lbHistory.Location = new System.Drawing.Point(6, 20);
            this.lbHistory.Name = "lbHistory";
            this.lbHistory.Size = new System.Drawing.Size(277, 280);
            this.lbHistory.TabIndex = 6;
            // 
            // FormServer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(710, 384);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.Name = "FormServer";
            this.Text = "视频监控 - 服务器";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormServer_FormClosed);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btStart;
        private System.Windows.Forms.TextBox tbClientIP;
        private System.Windows.Forms.ListBox lbClient;
        private System.Windows.Forms.Label lbStatic;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btRefresh;
        private System.Windows.Forms.ListBox lbHistory;

    }
}

