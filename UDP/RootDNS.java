package UDP;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.io.*;
import java.net.*;


public class RootDNS {
    private final static HashMap <String, String> map = new HashMap<String, String>();
    public static void main(String[] args){
        try{
            DatagramSocket ds = new DatagramSocket(7007);//7007 rootdns port
    
            ExecutorService pool = Executors.newFixedThreadPool(20);
            while(true){
                byte[] bf = new byte[300];
                DatagramPacket dp = new DatagramPacket(bf,  bf.length);

                System.out.println("Waiting for a packet reception..");		
                ds.receive(dp);

                pool.execute(new Rootdns(ds,dp,bf));
            }
        }
        catch(IOException e){

        }
    }

    public static class Rootdns implements Runnable{

        private DatagramSocket socket;
        private DatagramPacket packet;
        private String message;

        Rootdns(DatagramSocket socket,DatagramPacket dp,byte[] bf){
            this.socket = socket;
            this.packet = dp;
            String rs1 = new String(bf);
			String rs2 = rs1.trim();
            this.message = rs2;
        }
    
        public void run(){
            map.put(".com","7071");
            map.put(".kr","7072");
            map.put(".net","7073");
            map.put(".uk","7074");

            
            //만약 .com TLDserver port 번호 보내주기
            if(map.containsKey(message) == true){
                try{
                    DatagramSocket ds = new DatagramSocket();
			        InetAddress ia = InetAddress.getByName("localhost");
			        String msg = map.get(message);
			        byte[] bf = msg.getBytes();
                    int port_num = this.packet.getPort();					
			        DatagramPacket dp = new DatagramPacket(bf, bf.length, ia, port_num);
			        ds.send(dp);
                }
                catch(IOException e){}
            }
            else{
                try{	
                    DatagramSocket ds = new DatagramSocket();
			        InetAddress ia = InetAddress.getByName("localhost");
			        String msg = "0";
			        byte[] bf = msg.getBytes();
                    int port_num = this.packet.getPort();					
			        DatagramPacket dp = new DatagramPacket(bf, bf.length, ia, port_num);
			        ds.send(dp);
                }
                catch(IOException e){}
            }
        }
    }
}
