package pack;

import com.opensymphony.xwork2.ActionSupport;

@SuppressWarnings("serial")
public class MyAction extends ActionSupport {
	private String message, textbox;

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public String getTextbox() {
		return textbox;
	}

	public void setTextbox(String textbox) {
		this.textbox = textbox;
	}

	public String submit() {
		setMessage(getTextbox());
		return SUCCESS;
	}
}
