package com.example.a;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Scanner;

import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.graphics.Color;
import android.text.method.ScrollingMovementMethod;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

	class Pair {

		public Pair(String a, String b) {
			first = a;
			second = b;
		}

		public String first, second;
	}

	private java.util.Vector<Pair> list = new java.util.Vector<Pair>();

	private String trim(String buff) {
		int start = 0;
		while (start < buff.length()) {
			if (buff.charAt(start) != ' ' && buff.charAt(start) != '\t'
					&& buff.charAt(start) != '\r')
				break;
			++start;
		}
		int end = buff.length() - 1;
		while (start < end) {
			if (buff.charAt(end) != ' ' && buff.charAt(end) != '\t'
					&& buff.charAt(end) != '\r')
				break;
			--end;
		}
		return buff.substring(start, end + 1);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		TextView text = (TextView) findViewById(R.id.editText2);
		setEditTextReadOnly(text);

		try {
			// InputStream is = this.getAssets().open("dict.txt");
			// Scanner sc = new Scanner(is);
			// is.close();

			File dict = new File(Environment.getExternalStorageDirectory()
					.getPath() + "/dict.txt");
			Scanner sc = new Scanner(dict);
			boolean rec = true;
			while (sc.hasNext()) {
				String buff = trim(sc.nextLine());
				if (buff.length() == 0)
					continue;
				String line[] = buff.split("\\ ");
				boolean first = true;
				String key = "", val = "";
				for (int i = 0; i < line.length; ++i) {
					String s = trim(line[i]);
					if (s.length() == 0)
						continue;
					if (first) {
						key = s;
						first = false;
					} else
						val += s + " ";
				}
				list.add(new Pair(key, val));
			}
			sc.close();
		} catch (Exception ex) {
			ex.printStackTrace();
			text.setText(ex.getMessage());
		}
	}

	public static void setEditTextReadOnly(TextView view) {
		view.setTextColor(Color.rgb(255, 0, 0));
		if (view instanceof android.widget.EditText) {
			view.setCursorVisible(false); // 设置输入框中的光标不可见
			view.setFocusable(false); // 无焦点
			view.setFocusableInTouchMode(false); // 触摸时也得不到焦点
		}
	}

	public void search(View v) {
		TextView key = (TextView) findViewById(R.id.editText1);
		TextView val = (TextView) findViewById(R.id.editText2);
		StringBuffer sb = new StringBuffer();
		String lookup = key.getText().toString();
		if (lookup.length() > 0) {
			if (lookup.trim().equals("all"))
				lookup = "";
			int found = 0;
			for (Pair pair : list)
				if (pair.first.contains(lookup)) {
					found++;
					sb.append("名称：" + pair.first + "\n方法：" + pair.second
							+ "\n\n");
				}
			if (found == 0)
				sb.append("没有找到任何匹配的关键字！");
			val.setText(sb);
		} else
			val.setText("请输入要查询的关键字！ ");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
