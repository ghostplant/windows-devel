package frame;

import java.awt.BorderLayout;
import java.util.ArrayList;
import javax.swing.*;

public class HistoryFrame extends JFrame {

    public DefaultListModel model = new DefaultListModel();
    private JList list;

    public HistoryFrame() {
        super("数据库系统日志");

        list = new JList(model);
        ArrayList<String> history = mysql.SQLServer.query();
        for (String item : history) {
            model.addElement(item);
        }

        JScrollPane scrollpane = new JScrollPane(list);
        add(scrollpane, BorderLayout.CENTER);

        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setSize(480, 360);
        setLocationRelativeTo(null);
        setVisible(true);
    }
}
