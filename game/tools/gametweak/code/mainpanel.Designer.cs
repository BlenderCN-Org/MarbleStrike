namespace GameTweak
{
    partial class MainPanel
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.Connect = new System.Windows.Forms.Button();
			this.OutputText = new System.Windows.Forms.RichTextBox();
			this.SuspendLayout();
			// 
			// Connect
			// 
			this.Connect.Location = new System.Drawing.Point(12, 18);
			this.Connect.Name = "Connect";
			this.Connect.Size = new System.Drawing.Size(132, 53);
			this.Connect.TabIndex = 0;
			this.Connect.Text = "Connect";
			this.Connect.UseVisualStyleBackColor = true;
			this.Connect.Click += new System.EventHandler(this.Connect_Click);
			// 
			// OutputText
			// 
			this.OutputText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.OutputText.Location = new System.Drawing.Point(12, 77);
			this.OutputText.Name = "OutputText";
			this.OutputText.Size = new System.Drawing.Size(335, 217);
			this.OutputText.TabIndex = 2;
			this.OutputText.Text = "";
			// 
			// MainPanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(359, 306);
			this.Controls.Add(this.OutputText);
			this.Controls.Add(this.Connect);
			this.Name = "MainPanel";
			this.Text = "MainPanel";
			this.Load += new System.EventHandler(this.MainPanel_Load);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainPanel_Closing);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.Button Connect;
        private System.Windows.Forms.RichTextBox OutputText;
    }
}

