package mysql;

import java.sql.*;
import java.util.ArrayList;
import javax.swing.JOptionPane;

public class SQLServer {
    
    private static Connection conn = open();
    
    private static Connection open() {
        try {
            return DriverManager.getConnection("jdbc:mysql://localhost/test", "root", "root");
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(null, "数据库访问出错：" + ex.getMessage());
            return null;
        }
    }
    
    public synchronized static void insert(String text) {
        synchronized (conn) {
            try {
                Statement stmt = conn.createStatement();
                stmt.executeUpdate("create table if not exists monsqldb (text varchar(1000))");
                stmt.executeUpdate("insert into monsqldb values('" + text + "')");
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(null, "数据库访问出错：" + ex.getMessage());
            }
        }
    }
    
    public static ArrayList<String> query() {
        ArrayList<String> ret = new ArrayList<String>();
        synchronized (conn) {
            try {
                Statement stmt = conn.createStatement();
                stmt.executeUpdate("create table if not exists monsqldb (text varchar(1000))");
                ResultSet rst = stmt.executeQuery("select text from monsqldb");
                while (rst.next()) {
                    ret.add(rst.getString("text"));
                }
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(null, "数据库访问出错：" + ex.getMessage());
            }
        }
        return ret;
    }
}
