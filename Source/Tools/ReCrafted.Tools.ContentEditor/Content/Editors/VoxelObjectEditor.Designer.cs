namespace ReCrafted.Tools.ContentEditor.Content.Editors
{
    partial class VoxelObjectEditor
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
            this.Revert = new System.Windows.Forms.Button();
            this.Apply = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.TexBack = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.TexBottom = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.TexTop = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.TexRight = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.TexFront = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.TexLeft = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // Revert
            // 
            this.Revert.Location = new System.Drawing.Point(366, 246);
            this.Revert.Name = "Revert";
            this.Revert.Size = new System.Drawing.Size(75, 23);
            this.Revert.TabIndex = 3;
            this.Revert.Text = "Cancel";
            this.Revert.UseVisualStyleBackColor = true;
            this.Revert.Click += new System.EventHandler(this.Revert_Click);
            // 
            // Apply
            // 
            this.Apply.Location = new System.Drawing.Point(447, 246);
            this.Apply.Name = "Apply";
            this.Apply.Size = new System.Drawing.Size(75, 23);
            this.Apply.TabIndex = 2;
            this.Apply.Text = "Apply";
            this.Apply.UseVisualStyleBackColor = true;
            this.Apply.Click += new System.EventHandler(this.Apply_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(76, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "Top";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(76, 92);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "Front";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 92);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(25, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "Left";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(144, 92);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(32, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "Right";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(212, 92);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Back";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(76, 173);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(40, 13);
            this.label6.TabIndex = 15;
            this.label6.Text = "Bottom";
            // 
            // TexBack
            // 
            this.TexBack.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TexBack.Location = new System.Drawing.Point(215, 108);
            this.TexBack.Name = "TexBack";
            this.TexBack.Size = new System.Drawing.Size(62, 62);
            this.TexBack.TabIndex = 9;
            // 
            // TexBottom
            // 
            this.TexBottom.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TexBottom.Location = new System.Drawing.Point(79, 189);
            this.TexBottom.Name = "TexBottom";
            this.TexBottom.Size = new System.Drawing.Size(62, 62);
            this.TexBottom.TabIndex = 8;
            // 
            // TexTop
            // 
            this.TexTop.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TexTop.Location = new System.Drawing.Point(79, 27);
            this.TexTop.Name = "TexTop";
            this.TexTop.Size = new System.Drawing.Size(62, 62);
            this.TexTop.TabIndex = 7;
            // 
            // TexRight
            // 
            this.TexRight.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TexRight.Location = new System.Drawing.Point(147, 108);
            this.TexRight.Name = "TexRight";
            this.TexRight.Size = new System.Drawing.Size(62, 62);
            this.TexRight.TabIndex = 6;
            // 
            // TexFront
            // 
            this.TexFront.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TexFront.Location = new System.Drawing.Point(79, 108);
            this.TexFront.Name = "TexFront";
            this.TexFront.Size = new System.Drawing.Size(62, 62);
            this.TexFront.TabIndex = 5;
            // 
            // TexLeft
            // 
            this.TexLeft.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TexLeft.Location = new System.Drawing.Point(11, 108);
            this.TexLeft.Name = "TexLeft";
            this.TexLeft.Size = new System.Drawing.Size(62, 62);
            this.TexLeft.TabIndex = 4;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.TexFront);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.TexLeft);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.TexRight);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.TexTop);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.TexBottom);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.TexBack);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(289, 257);
            this.groupBox1.TabIndex = 16;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Cube height map";
            // 
            // VoxelObjectEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(534, 281);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.Revert);
            this.Controls.Add(this.Apply);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "VoxelObjectEditor";
            this.Text = "Voxel Object Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form_Closing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Revert;
        private System.Windows.Forms.Button Apply;
        private Controls.TextureSelect TexLeft;
        private Controls.TextureSelect TexFront;
        private Controls.TextureSelect TexRight;
        private Controls.TextureSelect TexTop;
        private Controls.TextureSelect TexBottom;
        private Controls.TextureSelect TexBack;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}