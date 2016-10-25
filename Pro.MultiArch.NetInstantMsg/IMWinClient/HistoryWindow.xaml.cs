/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.3
 * 负责人：HJWAJ
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.ComponentModel;
using System.IO;
using System.Windows.Markup;
using CommonLibrary;
using System.Diagnostics;
using Microsoft.Win32;

namespace IMWinClient
{
    /// <summary>
    /// HistoryWindow.xaml 的交互逻辑
    /// </summary>
    public partial class HistoryWindow : Window
    {
        MainWindow mw;
        String key;
        public HistoryWindow(MainWindow mw,String key)
        {
            this.mw = mw;
            this.key = key;
            mw.getHistoryItemById.Add(key,this);
            this.Closed += new EventHandler(HistoryWindow_Closed);
            InitializeComponent();
        }

        void HistoryWindow_Closed(object sender, EventArgs e)
        {
            mw.getHistoryItemById.Remove(this.key);
        }

        public HistoryWindow()
        {
            InitializeComponent();
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            mw.getHistoryItemById.Remove(this.key);
            this.Close();
        }

        private void ExportButton_Click(object sender, EventArgs e)
        {
            Microsoft.Win32.SaveFileDialog sfd = new Microsoft.Win32.SaveFileDialog();
            sfd.Filter = "Text Files (*.txt; *.xaml; *.rtf)|*.txt;*.xaml;*.rtf";
            if (sfd.ShowDialog() == true)
            {
                SaveFile(sfd.SafeFileName, historyRecRichTextBox);
            }
        }

        private static void SaveFile(string filename, RichTextBox richTextBox)
        {
            if (string.IsNullOrEmpty(filename))
            {
                throw new ArgumentNullException();
            }
            using (FileStream stream = File.OpenWrite(filename))
            {
                TextRange documentTextRange = new TextRange(richTextBox.Document.ContentStart, richTextBox.Document.ContentEnd);
                string dataFormat = DataFormats.Text;
                string ext = System.IO.Path.GetExtension(filename);
                if (String.Compare(ext, ".xaml", true) == 0)
                {
                    dataFormat = DataFormats.Xaml;
                }
                else if (String.Compare(ext, ".rtf", true) == 0)
                {
                    dataFormat = DataFormats.Rtf;
                }
                documentTextRange.Save(stream, dataFormat);
            }
        }
    }
}
