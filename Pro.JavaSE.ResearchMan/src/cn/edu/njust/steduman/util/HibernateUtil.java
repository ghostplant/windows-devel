/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.util;

import java.io.Serializable;
import java.util.List;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.Configuration;

/**
 *
 * @author Ghostplant
 */
public class HibernateUtil {

    private static SessionFactory sf;
    private static Session session;
    
    static {
        init();
    }

    public static Session getSession() {
        return session;
    }

    public static void init() {
        sf = new Configuration().configure().buildSessionFactory();
        session = sf.openSession();
    }

    public static Object get(Class type, Serializable sl) {
        return session.get(type, sl);
    }

    public static List getList(Class type) {
        return session.createQuery("from " + type.getCanonicalName()).list();
    }

    public static void save(Object tuple) {
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            session.save(tuple);
            session.flush();
            tx.commit();
        } catch (Exception ex) {
            if (tx != null) {
                tx.rollback();
            }
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
            if (tx != null) {
                tx.rollback();
            }
        }
    }

    public static void delete(Object tuple) {
        HibernateUtil.close();
        HibernateUtil.init();
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            session.delete(tuple);
            tx.commit();
        } catch (Exception ex) {
            if (tx != null) {
                tx.rollback();
            }
        }
    }

    public static void close() {
        session.close();
        sf.close();
    }
}
