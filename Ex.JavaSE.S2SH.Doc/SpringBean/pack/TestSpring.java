package pack;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class TestSpring {
	public static void main(String[] args) {
		ApplicationContext context = new ClassPathXmlApplicationContext(
				"springbean.xml");
		User user = (User) context.getBean("ghostplant");
		System.out.println(user.getName());
	}
}