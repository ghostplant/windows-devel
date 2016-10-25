using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.IO;
using System.Windows.Media.Imaging;

namespace IMWebClient
{
    public partial class ChildHistoryWin : ChildWindow
    {
        ChatDialog cd;

        public ChildHistoryWin(ChatDialog cd)
        {
            this.cd = cd;
            this.Title = "与 " + cd.mp.getPersonByPid[cd.fid].Text + " 的历史记录";
            cd.mp.historyWin = this;
            InitializeComponent();
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void ChildWindow_Closed(object sender, EventArgs e)
        {
            cd.mp.historyWin = null;
        }

        public void InsertText(string fname, string time, string data)
        {
            Bold info_run = new Bold();
            Run msg_run = new Run();
            info_run.Inlines.Add(fname + " [" + time + "]");
            msg_run.Text = "  " + data;

            Paragraph info_p = new Paragraph();
            info_p.Inlines.Add(info_run);
            msgRecRichTextBox.Blocks.Add(info_p);

            Paragraph msg_p = new Paragraph();
            msg_p.Inlines.Add(msg_run);
            msgRecRichTextBox.Blocks.Add(msg_p);

            Paragraph blank_p = new Paragraph();
            blank_p.Inlines.Add("");
            msgRecRichTextBox.Blocks.Add(blank_p);
        }

        public void InsertImage(string fname, string time, byte[] data)
        {
            Bold info_run = new Bold();
            info_run.Inlines.Add(fname + " [" + time + "]\n");

            Paragraph info_p = new Paragraph();
            info_p.Inlines.Add(info_run);
            msgRecRichTextBox.Blocks.Add(info_p);

            Stream stream = new MemoryStream(data);
            BitmapImage bImg = new BitmapImage();
            bImg.SetSource(stream);
            Image img = new Image();
            img.Source = bImg;

            img.Height = (img.Width = 200) * 1.2;

            InlineUIContainer container = new InlineUIContainer();
            container.Child = img;

            TextPointer EndofContent = msgRecRichTextBox.ContentEnd.GetNextInsertionPosition(LogicalDirection.Backward);
            msgRecRichTextBox.Selection.Select(EndofContent, EndofContent);
            msgRecRichTextBox.Selection.Insert(container);

            Paragraph blank_p = new Paragraph();
            blank_p.Inlines.Add("");
            msgRecRichTextBox.Blocks.Add(blank_p);
        }
    }
}

