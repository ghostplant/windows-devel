/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package logic;

import comapi.Base64;

/**
 *
 * @author Administrator
 */
public class BufferPool {

    private static StringBuffer pool = new StringBuffer();

    public static void push(String str, ClientThread ct) {
        for (int i = 0; i < str.length(); i++) {
            if (str.charAt(i) == ';') {
                int size = 0;
                for (int j = 0; j < pool.length(); j++) {
                    if (pool.charAt(j) == '|') {
                        ++size;
                    }
                }
                String key = null, data[] = new String[size];
                size = -1;
                StringBuffer transfer = new StringBuffer();
                for (int j = 0; j <= pool.length(); j++) {
                    if (j == pool.length() || pool.charAt(j) == '|') {
                        if (size >= 0) {
                            data[size] = transfer.toString();
                        } else {
                            key = transfer.toString();
                        }
                        transfer.delete(0, transfer.length());
                        ++size;
                    } else {
                        transfer.append(pool.charAt(j));
                    }
                }
                pool.delete(0, pool.length());
                try {
                    key = Base64.decode(key);
                    for (int k = 0; k < data.length; k++) {
                        data[k] = Base64.decode(data[k]);
                    }
                } catch (Exception ex) {
                }
                ct.execute(key, data);
            } else {
                pool.append(str.charAt(i));
            }
        }
    }
}

