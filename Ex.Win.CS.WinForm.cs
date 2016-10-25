
using System;
using System.Windows.Forms;

namespace WinForm {

	public class Program {
		private static TextBox textbox=new TextBox();
		private static Button button=new Button();

		private static void btn_Click(object sender,EventArgs e) {
			if (sender==button)
				MessageBox.Show(textbox.Text);
		}

		public static void Main(string[] args) {
			Application.EnableVisualStyles(); // 启用视觉样式
			Form form=new Form();
			form.Text = "Windows 应用程序";
			button.Text="按钮(&B)";
			button.Location = new System.Drawing.Point(100, 0);
			button.Click+=new EventHandler(btn_Click);
			form.Controls.Add(textbox);
			form.Controls.Add(button);
			form.ShowDialog();
		}
	}
}
