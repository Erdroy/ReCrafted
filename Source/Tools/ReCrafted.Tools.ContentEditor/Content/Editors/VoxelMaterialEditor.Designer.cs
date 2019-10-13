namespace ReCrafted.Tools.ContentEditor.Content.Editors
{
    partial class VoxelMaterialEditor
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
            this.Apply = new System.Windows.Forms.Button();
            this.Revert = new System.Windows.Forms.Button();
            this.HardnessScroll = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.HardnessValue = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.CBFar = new System.Windows.Forms.Label();
            this.CBNear = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.NSMFar = new System.Windows.Forms.Label();
            this.NSMNear = new System.Windows.Forms.Label();
            this.FarNSM = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.FarCB = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.NearNSM = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            this.NearCB = new ReCrafted.Tools.ContentEditor.Controls.TextureSelect();
            ((System.ComponentModel.ISupportInitialize)(this.HardnessScroll)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // Apply
            // 
            this.Apply.Location = new System.Drawing.Point(161, 454);
            this.Apply.Name = "Apply";
            this.Apply.Size = new System.Drawing.Size(75, 23);
            this.Apply.TabIndex = 0;
            this.Apply.Text = "Apply";
            this.Apply.UseVisualStyleBackColor = true;
            this.Apply.Click += new System.EventHandler(this.Apply_OnClick);
            // 
            // Revert
            // 
            this.Revert.Location = new System.Drawing.Point(80, 454);
            this.Revert.Name = "Revert";
            this.Revert.Size = new System.Drawing.Size(75, 23);
            this.Revert.TabIndex = 1;
            this.Revert.Text = "Revert";
            this.Revert.UseVisualStyleBackColor = true;
            this.Revert.Click += new System.EventHandler(this.Revert_OnClick);
            // 
            // HardnessScroll
            // 
            this.HardnessScroll.Location = new System.Drawing.Point(12, 25);
            this.HardnessScroll.Maximum = 255;
            this.HardnessScroll.Name = "HardnessScroll";
            this.HardnessScroll.Size = new System.Drawing.Size(224, 45);
            this.HardnessScroll.TabIndex = 2;
            this.HardnessScroll.Value = 64;
            this.HardnessScroll.Scroll += new System.EventHandler(this.HardnessScroll_Scroll);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Hardness";
            // 
            // HardnessValue
            // 
            this.HardnessValue.AutoSize = true;
            this.HardnessValue.Location = new System.Drawing.Point(188, 43);
            this.HardnessValue.Name = "HardnessValue";
            this.HardnessValue.Size = new System.Drawing.Size(48, 13);
            this.HardnessValue.TabIndex = 4;
            this.HardnessValue.Text = "64 / 255";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.CBFar);
            this.groupBox1.Controls.Add(this.NearNSM);
            this.groupBox1.Controls.Add(this.CBNear);
            this.groupBox1.Controls.Add(this.NearCB);
            this.groupBox1.Location = new System.Drawing.Point(15, 62);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(221, 189);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Color-Blend";
            // 
            // CBFar
            // 
            this.CBFar.AutoSize = true;
            this.CBFar.Location = new System.Drawing.Point(3, 105);
            this.CBFar.Name = "CBFar";
            this.CBFar.Size = new System.Drawing.Size(22, 13);
            this.CBFar.TabIndex = 8;
            this.CBFar.Text = "Far";
            // 
            // CBNear
            // 
            this.CBNear.AutoSize = true;
            this.CBNear.Location = new System.Drawing.Point(6, 21);
            this.CBNear.Name = "CBNear";
            this.CBNear.Size = new System.Drawing.Size(30, 13);
            this.CBNear.TabIndex = 6;
            this.CBNear.Text = "Near";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.NSMFar);
            this.groupBox2.Controls.Add(this.FarNSM);
            this.groupBox2.Controls.Add(this.NSMNear);
            this.groupBox2.Controls.Add(this.FarCB);
            this.groupBox2.Location = new System.Drawing.Point(12, 257);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(224, 189);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Normals-Smoothness-Metallic";
            // 
            // NSMFar
            // 
            this.NSMFar.AutoSize = true;
            this.NSMFar.Location = new System.Drawing.Point(3, 105);
            this.NSMFar.Name = "NSMFar";
            this.NSMFar.Size = new System.Drawing.Size(22, 13);
            this.NSMFar.TabIndex = 8;
            this.NSMFar.Text = "Far";
            // 
            // NSMNear
            // 
            this.NSMNear.AutoSize = true;
            this.NSMNear.Location = new System.Drawing.Point(6, 21);
            this.NSMNear.Name = "NSMNear";
            this.NSMNear.Size = new System.Drawing.Size(30, 13);
            this.NSMNear.TabIndex = 6;
            this.NSMNear.Text = "Near";
            // 
            // FarNSM
            // 
            this.FarNSM.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.FarNSM.Location = new System.Drawing.Point(6, 121);
            this.FarNSM.Name = "FarNSM";
            this.FarNSM.Size = new System.Drawing.Size(62, 62);
            this.FarNSM.TabIndex = 7;
            // 
            // FarCB
            // 
            this.FarCB.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.FarCB.Location = new System.Drawing.Point(6, 37);
            this.FarCB.Name = "FarCB";
            this.FarCB.Size = new System.Drawing.Size(62, 62);
            this.FarCB.TabIndex = 5;
            // 
            // NearNSM
            // 
            this.NearNSM.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.NearNSM.Location = new System.Drawing.Point(6, 121);
            this.NearNSM.Name = "NearNSM";
            this.NearNSM.Size = new System.Drawing.Size(62, 62);
            this.NearNSM.TabIndex = 7;
            // 
            // NearCB
            // 
            this.NearCB.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.NearCB.Location = new System.Drawing.Point(6, 37);
            this.NearCB.Name = "NearCB";
            this.NearCB.Size = new System.Drawing.Size(62, 62);
            this.NearCB.TabIndex = 5;
            // 
            // VoxelMaterialEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(248, 489);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.HardnessValue);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.HardnessScroll);
            this.Controls.Add(this.Revert);
            this.Controls.Add(this.Apply);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "VoxelMaterialEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Voxel Material Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form_Closing);
            ((System.ComponentModel.ISupportInitialize)(this.HardnessScroll)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Apply;
        private System.Windows.Forms.Button Revert;
        private System.Windows.Forms.TrackBar HardnessScroll;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label HardnessValue;
        private Controls.TextureSelect NearCB;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label CBNear;
        private System.Windows.Forms.Label CBFar;
        private Controls.TextureSelect NearNSM;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label NSMFar;
        private Controls.TextureSelect FarNSM;
        private System.Windows.Forms.Label NSMNear;
        private Controls.TextureSelect FarCB;
    }
}