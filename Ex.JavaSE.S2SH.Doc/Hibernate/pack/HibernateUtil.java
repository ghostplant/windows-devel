package pack;

import java.io.*;
import java.util.List;
import org.hibernate.*;
import org.hibernate.cfg.Configuration;

public class HibernateUtil {

	private static SessionFactory sf;
	private static Session session;

	public static void open() {
		sf = new Configuration().configure().buildSessionFactory();
		session = sf.openSession();
	}

	public static void close() {
		session.close();
		sf.close();
	}

	public static Object get(Class<?> type, Serializable sl) {
		return session.get(type, sl);
	}

	public static List<?> getList(Class<?> type) {
		return session.createQuery("from " + type.getCanonicalName()).list();
	}

	public static void insert(Object tuple) {
		Transaction tx = null;
		try {
			tx = session.beginTransaction();
			session.save(tuple);
			session.flush();
			tx.commit();
		} catch (Exception ex) {
			if (tx != null)
				tx.rollback();
			ex.printStackTrace();
		}
	}

	public static void update(Object tuple) {
		Transaction tx = null;
		try {
			tx = session.beginTransaction();
			session.update(tuple);
			session.flush();
			tx.commit();
		} catch (Exception ex) {
			if (tx != null)
				tx.rollback();
			ex.printStackTrace();
		}
	}

	public static void delete(Object tuple) {
		Transaction tx = null;
		try {
			tx = session.beginTransaction();
			session.delete(tuple);
			session.flush();
			tx.commit();
		} catch (Exception ex) {
			if (tx != null)
				tx.rollback();
			ex.printStackTrace();
		}
	}
}
