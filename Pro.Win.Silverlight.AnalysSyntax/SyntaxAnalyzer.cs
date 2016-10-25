using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Collections.Generic;
using System.Text;

namespace 语法分析器
{
    public class SyntaxAnalyzer
    {
        MainPage page;

        bool flag;
        List<char> visited = new List<char>();
        Dictionary<char, List<char>> first = new Dictionary<char, List<char>>(), follow = new Dictionary<char, List<char>>();
        char[,] graphMatrix;
        int graphMatrixCount;

        List<EnclosedState> allStates = new List<EnclosedState>();
        Dictionary<int, Dictionary<char, string>> analyzeList = new Dictionary<int, Dictionary<char, string>>();
        Dictionary<string, int> getIndexByProduct = new Dictionary<string, int>();

        bool isTerminator(char c)
        {
            return page.terminator.Text.Contains(c.ToString());
        }

        void first_dfs(char nterminator)
        {
            if (!visited.Contains(nterminator))
                visited.Add(nterminator);
            if (!first.ContainsKey(nterminator))
                first[nterminator] = new List<char>();
            foreach (string line in page.lstProduce.Items)
                if (line[0] == nterminator) // 寻找所有左部为该终结符的产生式
                {
                    for (int i = 4; i < line.Length; i++) // 对产生式右部从左向右
                        if (isTerminator(line[i]) || line[i] == 'ε') // 遇到终结符或空则直接添加并结束
                        {
                            if (!first[nterminator].Contains(line[i]))
                            {
                                first[nterminator].Add(line[i]);
                                flag = true;
                            }
                            break;
                        }
                        else if (line[i] != nterminator) // 不是自身
                        {
                            if (!visited.Contains(line[i])) // 遇到非终结符并且这个非终结符未求则递归先求
                                first_dfs(line[i]);
                            List<char> ins = first[line[i]];
                            foreach (char c in ins) // 将这个非终结符FIRST集所有非空元素直接添加
                                if (c != 'ε' && !first[nterminator].Contains(c))
                                {
                                    first[nterminator].Add(c);
                                    flag = true;
                                }
                            if (!ins.Contains('ε')) // 若这个非终结符FIRST集不含ε则结束
                                break;
                            if (i + 1 == line.Length && !first[nterminator].Contains('ε')) // 若这个非终结符是最后一个而且含ε则加入ε
                            {
                                first[nterminator].Add('ε');
                                flag = true;
                            }
                        }
                        else if (!first[nterminator].Contains('ε'))
                            break;
                }
        }

        void follow_dfs(char nterminator)
        {
            if (!visited.Contains(nterminator))
                visited.Add(nterminator);
            if (!follow.ContainsKey(nterminator))
                follow[nterminator] = new List<char>();
            foreach (string text in page.lstProduce.Items)
            {
                int prev = 4, pos = text.IndexOf(nterminator, prev);
                while (pos != -1)
                {
                    if (pos == text.Length - 1) // 若该元素是结尾则添加左部的follow集
                    {
                        if (!visited.Contains(text[0])) // 未推则递归求解
                            follow_dfs(text[0]);
                        foreach (char c in follow[text[0]])
                            if (!follow[nterminator].Contains(c))
                            {
                                follow[nterminator].Add(c);
                                flag = true;
                            }
                    }
                    for (int i = pos + 1; i < text.Length; i++)
                    {
                        if (isTerminator(text[i])) // 如果是终结符则添加并结束
                        {
                            if (!follow[nterminator].Contains(text[i]))
                            {
                                follow[nterminator].Add(text[i]);
                                flag = true;
                            }
                            break;
                        }
                        else if (text[i] != nterminator) // 忽略本身
                        {
                            bool blank = false;
                            foreach (char c in first[text[i]]) // 将非ε的first集元素添加至follow集
                                if (c == 'ε')
                                    blank = true;
                                else if (!follow[nterminator].Contains(c))
                                {
                                    follow[nterminator].Add(c);
                                    flag = true;
                                }
                            if (!blank) // 非ε则结束
                                break;
                        }
                        if (i == text.Length - 1) // 可推至结尾则添加左部的follow集
                        {
                            if (!visited.Contains(text[0])) // 未推则递归求解
                                follow_dfs(text[0]);
                            foreach (char c in follow[text[0]])
                                if (!follow[nterminator].Contains(c))
                                {
                                    follow[nterminator].Add(c);
                                    flag = true;
                                }
                        }
                    }
                    prev = pos + 1;
                    pos = text.IndexOf(nterminator, prev);
                }
            }
        }

        // 计算FIRST,FOLLOW集
        private void calcFirstAndFollow()
        {
            // 求 FIRST 集
            flag = true;
            while (flag)
            {
                flag = false;
                visited.Clear();
                foreach (char c in page.nterminator.Text)
                    first_dfs(c);
            }

            // 求 FOLLOW 集
            follow[(char)page.cbBegin.SelectedItem] = new List<char>() { '#' };

            flag = true;
            while (flag)
            {
                flag = false;
                visited.Clear();
                foreach (char c in page.nterminator.Text)
                    follow_dfs(c);
            }

            // 构造二维表并显示
            var dicFfset = new List<KeyValuePair<string, List<string>>>();
            List<string> ntList = new List<string>(), firstList = new List<string>(),
                followList = new List<string>();

            foreach (char c in page.nterminator.Text)
            {
                // 列表
                ntList.Add(c.ToString());
                StringBuilder sb = new StringBuilder();
                // 构造FIRST集
                sb.Remove(0, sb.Length);
                first[c].Sort();
                foreach (char t in first[c])
                {
                    sb.Append(t);
                    sb.Append(' ');
                }
                firstList.Add(sb.ToString());
                // 构造FOLLOW集
                sb.Remove(0, sb.Length);
                follow[c].Sort();
                foreach (char t in follow[c])
                {
                    sb.Append(t);
                    sb.Append(' ');
                }
                followList.Add(sb.ToString());
            }

            dicFfset.Add(new KeyValuePair<string, List<string>>("非终结符", ntList));
            dicFfset.Add(new KeyValuePair<string, List<string>>("FIRST 集", firstList));
            dicFfset.Add(new KeyValuePair<string, List<string>>("FOLLOW 集", followList));
            page.tbFfset.ItemsSource = DataSourceCreator.ToDataSource(dicFfset);
        }

        // 构造LR(1)文法的DFA
        private void calcDefiniteFA()
        {
            int count = 0;
            List<string> extList=new List<string>();
            extList.Add(count++.ToString() + "：" + "Φ → " + page.cbBegin.SelectedItem);
            foreach (string line in page.lstProduce.Items)
                extList.Add(count++.ToString() + "：" + line[0] + " → " + line.Substring(4));
            page.lstProduce.Items.Clear();

            foreach (string line in extList)
                page.lstProduce.Items.Add(line);

            page.nterminator.Text += 'Φ';

            var init_state = new EnclosedState();
            init_state.items.Add("Φ → ●" + page.cbBegin.SelectedItem, new List<char>() { '#' });
            init_state = calcEnclosed(init_state);

            allStates.Add(init_state);

            List<List<KeyValuePair<int, char>>> release = new List<List<KeyValuePair<int, char>>>();
            // release[i][j]记录第i个状态可以通过字符release[i][j].Value推出第release[i][j].Key个状态

            for (int i = 0; i < allStates.Count; i++)
            {
                var front = allStates[i].items;
                var graphAdj = new List<KeyValuePair<int, char>>();
                var enableChr = new List<char>(); // 对推新状态的途径字符判重
                foreach (var pair in front) // 找出所有不重复可推状态
                {
                    int pos = pair.Key.IndexOf('●') + 1;
                    if (pos < pair.Key.Length && !enableChr.Contains(pair.Key[pos]))
                    {
                        char c = pair.Key[pos];
                        enableChr.Add(c); // 对推新状态的途径字符判重
                        // 对经过c字符转向新状态
                        EnclosedState esbuild = new EnclosedState();
                        foreach (var trans in front) // 查询当前状态所有产生式中，●后是c的条目
                        {
                            int np = trans.Key.IndexOf('●')+1;
                            if (np < trans.Key.Length && trans.Key[np] == c) // 找到一条符合项
                            {
                                string next = trans.Key.Remove(np - 1, 1).Insert(np, "●");
                                esbuild.items.Add(next, trans.Value);
                            }
                        }
                        esbuild = calcEnclosed(esbuild);
                        int j;
                        for (j = 0; j < allStates.Count; j++) // 新状态已存在
                            if (EnclosedState.Equals(esbuild.items, allStates[j].items)) // 判断是否完全匹配
                            {
                                graphAdj.Add(new KeyValuePair<int, char>(j,c));
                                break;
                            }
                        if (j >= allStates.Count) // 不存在新状态则创建
                        {
                            graphAdj.Add(new KeyValuePair<int, char>(j, c));
                            allStates.Add(esbuild);
                        }
                    }
                }
                release.Add(graphAdj);
            }

            graphMatrixCount=allStates.Count;
            graphMatrix = new char[graphMatrixCount, graphMatrixCount];
            for (int i = 0; i < graphMatrixCount; i++)
                foreach (var j in release[i])
                    graphMatrix[i,j.Key] = j.Value;

            int tot = allStates[0].items.Count;
            for (int i = 1; i < allStates.Count; i++)
                tot = Math.Max(tot, allStates[i].items.Count);

            // 显示DFA状态转换矩阵
            var dfaMatrix = new List<KeyValuePair<string, List<string>>>();
            List<string> stateList=new List<string>();
            for (int i = 0; i < graphMatrixCount; i++)
                stateList.Add("状态" + i);
            dfaMatrix.Add(new KeyValuePair<string, List<string>>("DFA", stateList));
            for (int i = 0; i < graphMatrixCount; i++)
            {
                stateList = new List<string>();
                for (int j = 0; j < graphMatrixCount; j++)
                {
                    if (graphMatrix[j, i] > 0)
                    {
                        StringBuilder sb = new StringBuilder();
                        sb.Append("从状态" + j + "：\n");
                        foreach (var item in allStates[j].items)
                        {
                            sb.Append(item.Key);
                            sb.Append(" ， ");
                            sb.Append(item.Value[0]);
                            for (int k = 1; k < item.Value.Count; k++)
                            {
                                sb.Append('/');
                                sb.Append(item.Value[k]);
                            }
                            sb.Append('\n');
                        }
                        for (int l = allStates[j].items.Count; l < tot;l++ )
                            sb.Append('\n');
                        sb.Append("经过字符 " + graphMatrix[j, i] + " 转变为状态" + i + "：\n");
                        foreach (var item in allStates[i].items)
                        {
                            sb.Append(item.Key);
                            sb.Append(" ， ");
                            sb.Append(item.Value[0]);
                            for (int k = 1; k < item.Value.Count; k++)
                            {
                                sb.Append('/');
                                sb.Append(item.Value[k]);
                            }
                            sb.Append('\n');
                        }
                        for (int l = allStates[i].items.Count;l < tot; l++)
                            sb.Append('\n');
                        stateList.Add(sb.ToString());
                    }
                    else
                    {
                        StringBuilder sb = new StringBuilder();
                        for (int l = 0; l <= tot; l++)
                            sb.Append("\n\n");
                        stateList.Add(sb.ToString());
                    }
                }
                dfaMatrix.Add(new KeyValuePair<string, List<string>>("状态" + i, stateList));
            }
            page.tbDfa.ItemsSource = DataSourceCreator.ToDataSource(dfaMatrix);
        }

        // 对语句进行分析与验证
        private void calcAnalyzeCode()
        {
            if (!flag)
            {
                page.legal.Text = "无解";
                return;
            }
            string text = page.sentence.Text;
            int nStep = 1;
            List<string> step = new List<string>() { "(" + nStep++.ToString() + ")" }, stateStack = new List<string>() { "0" },
                operStack = new List<string>() { "#" }, inputString = new List<string>() { text + '#' },
                actionOper = new List<string>(), gotoOper = new List<string>();
            // 从第一步开始求解分析过程
            while (flag)
            {
                // 当前状态s，当前字符c
                int s = Int32.Parse(stateStack[stateStack.Count - 1].Substring(stateStack[stateStack.Count - 1].LastIndexOf(',')+1));
                char c = inputString[inputString.Count - 1][0];
                string got = analyzeList[s][c];
                if (got.Length == 0)
                {
                    flag = false;
                    actionOper.Add("出错");
                    gotoOper.Add("");
                    break;
                }
                if (got.Contains("acc")) // 遇到'#'且可以为acc则一定为接受
                {
                    actionOper.Add("接受");
                    gotoOper.Add("");
                    break;
                }
                char type = got[0];
                int next = Int32.Parse(got.Substring(1));
                step.Add("(" + nStep++.ToString() + ")");
                if (type == 'S') // 移进
                {
                    actionOper.Add("移进 " + next);
                    gotoOper.Add("");
                    stateStack.Add(stateStack[stateStack.Count - 1] + "," + next);
                    operStack.Add(operStack[operStack.Count - 1] + c);
                    inputString.Add(inputString[inputString.Count - 1].Substring(1));
                }
                else // 规约
                {
                    actionOper.Add("规约 " + next);
                    string line = (string)page.lstProduce.Items[next];
                    line = line.Substring(line.IndexOf('：') + 1).Replace("ε", "");
                    char left = line[0];
                    string right = line.Substring(4);

                    string stackCut = stateStack[stateStack.Count - 1];
                    for (int i = 0; i < right.Length; i++)
                        stackCut = stackCut.Remove(stackCut.LastIndexOf(','));
                    int new_top = Int32.Parse(stackCut.Substring(stackCut.LastIndexOf(',') + 1)); // 弹斩后栈顶
                    int target = Int32.Parse(analyzeList[new_top][left]); // 获取GOTO目标
                    stateStack.Add(stackCut + "," + target);
                    if (right.Length>0)
                        operStack.Add(operStack[operStack.Count - 1].Remove(operStack[operStack.Count - 1].Length - right.Length) + left);
                    else
                        operStack.Add(operStack[operStack.Count - 1] + left);
                    inputString.Add(inputString[inputString.Count - 1]);
                    gotoOper.Add("转至 " + target);
                }
            }
            page.legal.Text = (flag) ? "正确" : "错误";
            var showAnalyze = new List<KeyValuePair<string, List<string>>>();
            showAnalyze.Add(new KeyValuePair<string, List<string>>("步骤", step));
            showAnalyze.Add(new KeyValuePair<string, List<string>>("状态栈", stateStack));
            showAnalyze.Add(new KeyValuePair<string, List<string>>("符号栈", operStack));
            showAnalyze.Add(new KeyValuePair<string, List<string>>("输入串", inputString));
            showAnalyze.Add(new KeyValuePair<string, List<string>>("ACTION", actionOper));
            showAnalyze.Add(new KeyValuePair<string, List<string>>("GOTO", gotoOper));
            page.tbAnalyze.ItemsSource = DataSourceCreator.ToDataSource(showAnalyze);
        }

        // 创建LR(1)文法的分析表
        private void calcAnalyzeList()
        {
            // 求 移进 列
            var anaList = new List<KeyValuePair<string, List<string>>>();
            List<string> rowData = new List<string>() { };
            rowData.Add("符号");
            for (int i = 0; i < graphMatrixCount; i++)
                rowData.Add(i.ToString());
            anaList.Add(new KeyValuePair<string, List<string>>("状态",rowData));
            List<char> text = new List<char>();
            foreach (char c in page.terminator.Text)
                text.Add(c);
            text.Sort();
            text.Add('#');
            int indexCount=0;
            foreach (char c in text) // 枚举每个非终结符（对应ACTION列）
            {
                rowData = new List<string>() { };
                rowData.Add(c.ToString());
                for (int i = 0; i < graphMatrixCount; i++)
                    rowData.Add("");
                for (int i = 0; i < graphMatrixCount; i++)
                    for (int j = 0; j < graphMatrixCount; j++)
                        if (graphMatrix[i, j] == c)
                            rowData[i + 1] = "S" + j;
                anaList.Add(new KeyValuePair<string, List<string>>("ACTION " + indexCount++, rowData));
            }

            // 求 规约 列
            foreach (string line in page.lstProduce.Items) // HASH产生式方便由产生式确定产生式所在下标
            {
                int pos = line.IndexOf('：');
                getIndexByProduct[line.Substring(pos + 1).Replace("ε", "")] = Int32.Parse(line.Substring(0, pos));
            }

            flag = true;

            for (int i = 0; i < allStates.Count;i++ )
                foreach (var item in allStates[i].items)
                    if (item.Key.EndsWith("●"))
                    {
                        int index = getIndexByProduct[item.Key.Substring(0, item.Key.Length - 1)];
                        foreach (char c in item.Value)
                        {
                            string append = (index > 0) ? ("r" + index) : "acc";
                            int j;
                            for (j = 0; j < anaList.Count; j++)
                                if (anaList[j].Value[0].Equals(c.ToString()))
                                    break;
                            if (anaList[j].Value[i+1].Length > 0)
                            {
                                anaList[j].Value[i+1] += "/" + append;
                                if (c != '#' || i+1 != 1) // 不是接受项则产生冲突
                                    flag = false;
                            }
                            else
                                anaList[j].Value[i+1] = append;
                        }
                    }

            // 求 GOTO 列
            text.Clear();
            for (int i = 1; i < page.nterminator.Text.Length; i++)
                text.Add(page.nterminator.Text[i-1]);
            foreach (char c in text) // 与移进处理类似
            {
                rowData = new List<string>() { };
                rowData.Add(c.ToString());
                for (int i = 0; i < graphMatrixCount; i++)
                    rowData.Add("");
                for (int i = 0; i < graphMatrixCount; i++)
                    for (int j = 0; j < graphMatrixCount; j++)
                        if (graphMatrix[i, j] == c)
                            rowData[i+1] = j.ToString();
                anaList.Add(new KeyValuePair<string, List<string>>("GOTO " + indexCount++, rowData));
            }
            
            // 作为HashMap、转置拷贝到全局（用于分析例句）

            foreach (var pair in anaList)
                if (!pair.Key.Equals("状态"))
                    for (int i = 1; i < pair.Value.Count; i++)
                    {
                        if (!analyzeList.ContainsKey(i-1))
                            analyzeList[i-1] = new Dictionary<char, string>();
                        analyzeList[i-1][pair.Value[0][0]] = pair.Value[i];
                    }


            page.tbLRList.ItemsSource = DataSourceCreator.ToDataSource(anaList);
            if (!flag)
                MessagePage.Show("由于冲突的产生，所以该文法不符合LR(1)文法！");
        }

        //LR(1)分析过程方法框架
        public SyntaxAnalyzer(MainPage page)
        {
            this.page = page;
            calcFirstAndFollow();
            calcDefiniteFA();
            calcAnalyzeList();
            calcAnalyzeCode();
        }

        // 扩充计算当前产生式组的闭包
        public EnclosedState calcEnclosed(EnclosedState es)
        {
            List<KeyValuePair<string, List<char>>> queue = new List<KeyValuePair<string, List<char>>>();
            foreach (var item in es.items) // 所有产生式入队
                queue.Add(item);
            for (int i = 0; i < queue.Count;i++)
            {
                var item = queue[i];
                int pos = item.Key.IndexOf('●') + 1;
                if (pos < item.Key.Length)
                {
                    if (!isTerminator(item.Key[pos])) // 非终结符item.Key[pos]扩充产生式
                    {
                        foreach (string text in page.lstProduce.Items)
                        {
                            if (text[text.IndexOf('：') + 1] == item.Key[pos]) // 要扩充的一项产生式
                            {
                                string extend = text.Substring(text.IndexOf('：') + 1).Replace(" → ", " → ●").Replace("ε", "");
                                List<char> suffix = new List<char>();
                                bool append = false;
                                if (pos + 1 == item.Key.Length) // 该非终结符在结尾，则对于'ba'要添加'a'
                                    append = true;
                                for (int j = pos + 1; j < item.Key.Length; j++)
                                {
                                    if (isTerminator(item.Key[j]) && !suffix.Contains(item.Key[j])) // 如果是终结符则直接确定附加项
                                    {
                                        suffix.Add(item.Key[j]);
                                        break;
                                    }
                                    else // 如果是非终结符则添加FIRST集到附加项(包含ε则继续)
                                    {
                                        bool blank = false;
                                        foreach (char c in first[item.Key[j]])
                                            if (c == 'ε')
                                                blank = true;
                                            else if (!suffix.Contains(c))
                                                suffix.Add(c);
                                        if (!blank)
                                            break;
                                        if (j + 1 == item.Key.Length) // 最后一个可行非终结符还有ε
                                            append = true;
                                    }
                                }
                                if (append) // 原有附加项并到扩展项
                                    foreach (char c in item.Value)
                                        if (!suffix.Contains(c))
                                            suffix.Add(c);
                                int k;
                                for (k = 0; k < queue.Count; k++) // 寻找完全重复项
                                    if (queue[k].Key.Equals(extend) && queue[k].Value.Count==suffix.Count)
                                    {
                                        int l;
                                        suffix.Sort();
                                        for (l = 0; l < suffix.Count; l++)
                                            if (suffix[l] != queue[k].Value[l])
                                                break;
                                        if (l >= suffix.Count) // 完全相等
                                            break;
                                    }
                                if (k >= queue.Count) // 没有找到完全相等项，确定扩展一项（队列顺序限制，包括对“半重复”产生式也要进队）
                                {
                                    queue.Add(new KeyValuePair<string, List<char>>(extend, suffix));
                                }
                            }
                        }
                    }
                }
            }
            EnclosedState esbuild = new EnclosedState();
            for (int i = 0; i < queue.Count; i++)
            {
                if (esbuild.items.ContainsKey(queue[i].Key)) // 压缩队中产生式，若含有相同递推式则合并附加项，否则新建【产生式|向前搜索符】元组
                {
                    foreach (char c in queue[i].Value)
                        if (!esbuild.items[queue[i].Key].Contains(c))
                            esbuild.items[queue[i].Key].Add(c);
                }
                else
                    esbuild.items.Add(queue[i].Key, queue[i].Value);
            }
            return esbuild;
        }
    }

    public class EnclosedState
    {
        public Dictionary<string, List<char>> items = new Dictionary<string, List<char>>();

        public static bool Equals(Dictionary<string, List<char>> x, Dictionary<string, List<char>> y)
        {
            if (x.Count != y.Count)
                return false;
            foreach (var pair in x)
                if (y.ContainsKey(pair.Key))
                {
                    List<char> a = y[pair.Key], b = pair.Value;
                    if (a.Count != b.Count)
                        return false;
                    for (int i = 0; i < a.Count; i++)
                        if (a[i] != b[i])
                            return false;
                }
                else
                    return false;
            return true;
        }
    }

}
