package sql;
import java.sql.*;
import sql.Student;
public class sql {
    public static void main(String[] args) {
        Connection con = null; 
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            String url = "jdbc:mysql://localhost/gcubook";
            String user = "root", passwd = "kuj20021226..";
            con = DriverManager.getConnection(url, user, passwd); 
            System.out.println(con);
        } catch (ClassNotFoundException e) { 
            e.printStackTrace();
        } catch (SQLException e) { 
            e.printStackTrace();
        }
        Statement stmt = null; 
        ResultSet rs = null;
        Student[] std = new Student[2]; 
        for(int i =0; i<std.length; i++){
            std[i] = new Student();
        } 
        try {
            stmt = con.createStatement(); 
            String sql = "select * from student"; 
            rs = stmt.executeQuery(sql);
            int i = 0;
            while (rs.next()) {
                std[i].setId( Integer.parseInt(rs.getString(1))); 
                std[i].setName(rs.getString(2)); 
                std[i].setAge(Integer.parseInt(rs.getString(3)));
                System.out.println(std[i].getId()+ " " + std[i].getName() + " "+ std[i].getAge());
                i++;
            }
            rs.close(); 
            stmt.close(); 
            con.close();
        } catch (SQLException e1) { 
            e1.printStackTrace();
        }
    }
}
