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
using System.Windows.Data;
using System.IO;

using System.Reflection.Emit;
using System.Reflection;

namespace 语法分析器
{
    public partial class MainPage : UserControl
    {
        public MainPage()
        {
            InitializeComponent();
        }

        private void about_Click(object sender, RoutedEventArgs e)
        {
            MessagePage.Show("作者：崔嵬\n班级：09065501\n学号：0911080114\n指导老师：曹雪\n工程类型：Silverlight SyntaxAnalyzer");
        }
        
        //插入一条含逻辑或的产生式到正规文法
        private void insert_Click(object sender, RoutedEventArgs e)
        {
            if (rfrom.Text.Length == 0 || rdest.Text.Length == 0)
                MessagePage.Show("产生式左部必须是单个字符，右部至少一个字符！");
            else
            {
                string text = rdest.Text + '|';
                int prev = 0, pos = text.IndexOf('|', prev);
                while (pos != -1)
                {
                    if (pos > prev && !rdest.Text.Substring(prev, pos - prev).Equals(rfrom.Text))
                    {
                        string substr = rdest.Text.Substring(prev, pos - prev);
                        if (substr.Contains("ε") && substr.Length > 1)
                            MessagePage.Show("产生式推出空串不能与其他字符结合！");
                        else
                        {
                            substr = rfrom.Text + " → " + substr;
                            if (!lstProduce.Items.Contains(substr))
                                lstProduce.Items.Add(substr);
                            else
                                MessagePage.Show("已存在产生式 "+substr+" ，将忽略其中一个。");
                        }
                    }
                    else
                        MessagePage.Show("产生式不能自己推出自己，并且不能存在连续两个'|'！");
                    prev = pos + 1;
                    pos = text.IndexOf('|', prev);
                }
                rfrom.Text = rdest.Text = "";
            }
        }

        private void clear_Click(object sender, RoutedEventArgs e)
        {
            insert.IsEnabled = true;
            analyze.IsEnabled = true;
            rfrom.IsEnabled = true;
            rdest.IsEnabled = true;
            cbBegin.IsEnabled = true;

            tbDfa.ItemsSource = tbFfset.ItemsSource = tbLRList.ItemsSource = tbAnalyze.ItemsSource = null;
            legal.Text = "就绪";
            sentence.Text = rfrom.Text = rdest.Text = "";
            terminator.Text = nterminator.Text = "";
            lstProduce.Items.Clear();
            cbBegin.Items.Clear();
        }

        private void rfrom_TextChanged(object sender, TextChangedEventArgs e)
        {
            judgeTerminator();

            string text = rfrom.Text;
            if (text.Contains(' ') || text.Contains('\t') || text.Contains('|') || text.Contains('ε') || text.Contains('Φ') || text.Contains('→') || text.Contains('●') || text.Contains('#'))
            {
                rfrom.Text = "";
                MessagePage.Show("不允许含有非法空字符以及'Φ'、'→'、'|'、'ε'、'●'、'#'！");
                return;
            }
            if (text.Length > 1)
            {
                ((TextBox)sender).Text = text.Substring(0, 1);
                MessagePage.Show("非终结符必须是单个字符！");
            }
            else if (text.Length == 1)
                judgeTerminator();
        }

        private void rdest_TextChanged(object sender, TextChangedEventArgs e)
        {
            string text = rdest.Text;
            if (text.Contains(' ') || text.Contains('\t') || text.Contains('Φ') || text.Contains('→') || text.Contains('●') || text.Contains('#'))
            {
                rdest.Text = "";
                MessagePage.Show("不允许含有非法空字符以及'Φ'、'→'、'●'、'#'！");
                return;
            }
            judgeTerminator();
        }

        private void nterminator_TextChanged(object sender, TextChangedEventArgs e)
        {
            cbBegin.Items.Clear();
            foreach (string c in lstProduce.Items)
                if (!cbBegin.Items.Contains(c[0]))
                    cbBegin.Items.Add(c[0]);
            if (cbBegin.Items.Count > 0)
                cbBegin.SelectedIndex = 0;
        }

        private void analyze_Click(object sender, RoutedEventArgs e)
        {
            if (cbBegin.SelectedItem == null)
            {
                MessagePage.Show("您必须选中一个初始状态！");
                return;
            }
            else
            {
                foreach (char c in sentence.Text)
                    if (!terminator.Text.Contains(c))
                    {
                        MessagePage.Show("待验证的语句中含有非终结符 " + c + " ！");
                        return;
                    }
            }

            insert.IsEnabled = false;
            analyze.IsEnabled = false;
            rfrom.IsEnabled = false;
            rdest.IsEnabled = false;
            cbBegin.IsEnabled = false;

            try
            {
                new SyntaxAnalyzer(this);
            }
            catch (Exception ex)
            {
                MessagePage.Show(ex.Message);
            }
        }

        //判定当前产生式的终结符与非终结符
        private void judgeTerminator()
        {
            terminator.Text = nterminator.Text = "";

            foreach (string text in lstProduce.Items)
            {
                if (!nterminator.Text.Contains(text[0])) // 非终结符
                    nterminator.Text += text[0];
                int it = terminator.Text.IndexOf(text[0]);
                if (it != -1)
                    terminator.Text = terminator.Text.Remove(it, 1);

                for (int i = 4; i < text.Length; i++) // 终结符
                    if (text[i] != 'ε' && !nterminator.Text.Contains(text[i]) && !terminator.Text.Contains(text[i]))
                        terminator.Text += text[i];
            }
            if (!nterminator.Text.Contains(rfrom.Text))
                nterminator.Text += rfrom.Text;
            int pos = terminator.Text.IndexOf(rfrom.Text);
            if (pos != -1 && rfrom.Text.Length > 0)
                terminator.Text = terminator.Text.Remove(pos, 1);

            foreach (char c in rdest.Text)
                if (c != '|' && c != 'ε' && !nterminator.Text.Contains(c) && !terminator.Text.Contains(c))
                    terminator.Text += c;
        }

        private void tbFfset_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            new ChildDataGrid(tbFfset.ItemsSource).Show();
        }

        private void tbDfa_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            new ChildDataGrid(tbDfa.ItemsSource).Show();
        }

        private void tbAnalyze_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            new ChildDataGrid(tbAnalyze.ItemsSource).Show();
        }


        private void tbLRList_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            new ChildDataGrid(tbLRList.ItemsSource).Show();
        }

        private void sample1_Click(object sender, RoutedEventArgs e)
        {
            clear_Click(sender, e);
            rfrom.Text = "E";
            rdest.Text = "E+T|T";
            insert_Click(sender, e);
            rfrom.Text = "T";
            rdest.Text = "T*F|F";
            insert_Click(sender, e);
            rfrom.Text = "F";
            rdest.Text = "(E)|i";
            insert_Click(sender, e);
            sentence.Text = "i+(i+i)*i";
        }

        private void sample2_Click(object sender, RoutedEventArgs e)
        {
            clear_Click(sender, e);
            rfrom.Text = "B";
            rdest.Text = "BoT|T";
            insert_Click(sender, e);
            rfrom.Text = "T";
            rdest.Text = "TaF|F";
            insert_Click(sender, e);
            rfrom.Text = "F";
            rdest.Text = "nF|(B)|t|f";
            insert_Click(sender, e);
            sentence.Text = "nntoft";
        }

        private void sample3_Click(object sender, RoutedEventArgs e)
        {
            clear_Click(sender, e);
            rfrom.Text = "S";
            rdest.Text = "AB|bC";
            insert_Click(sender, e);
            rfrom.Text = "A";
            rdest.Text = "b|ε";
            insert_Click(sender, e);
            rfrom.Text = "B";
            rdest.Text = "aD|ε";
            insert_Click(sender, e);
            rfrom.Text = "C";
            rdest.Text = "AD|b";
            insert_Click(sender, e);
            rfrom.Text = "D";
            rdest.Text = "aS|c";
            insert_Click(sender, e);
            sentence.Text = "ab";
        }

        private void sample4_Click(object sender, RoutedEventArgs e)
        {
            clear_Click(sender, e);
            rfrom.Text = "S";
            rdest.Text = "ε";
            insert_Click(sender, e);
            sentence.Text = "";
        }
    }

}
