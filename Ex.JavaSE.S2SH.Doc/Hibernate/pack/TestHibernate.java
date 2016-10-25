package pack;

import java.util.List;

public class TestHibernate {

	public static void main(String[] args) throws Exception {
		HibernateUtil.open();
		User user = new User();
		user.setName("ghostplant");
		user.setPassword("password");
		HibernateUtil.insert(user);
		List<User> lists = (List<User>) HibernateUtil.getList(User.class);
		for (User each : lists) {
			System.out.println(each.getPassword());
			each.setPassword("ghostplant");
			HibernateUtil.update(each);
			String ret = ((User) HibernateUtil.get(User.class, "ghostplant"))
					.getPassword();
			System.out.println(ret);
			HibernateUtil.delete(each);
		}
		// HibernateUtil.close();
	}
}