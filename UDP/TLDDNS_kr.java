package UDP;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.io.*;
import java.net.*;


public class TLDDNS_kr {
    private final static HashMap <String, String> map = new HashMap<String, String>();
    public static void main(String[] args){
        map.put("yawon.kr","125.209.222.142");
        map.put("gachon.ac.kr","135.205.232.142");
        map.put("twitch.kr","125.55.221.121");
        //값추가
        try{
            DatagramSocket ds = new DatagramSocket(7072);//7072 TLDdns_kr port
    
            ExecutorService pool = Executors.newFixedThreadPool(20);
            while(true){
                byte[] bf = new byte[300];
                DatagramPacket dp = new DatagramPacket(bf,  bf.length);

                System.out.println("Waiting for a packet reception..");		
                ds.receive(dp);
                
                pool.execute(new TLDdns_kr(ds,dp,bf));
            }
        }
        catch(IOException e){

        }
    }

    public static class TLDdns_kr implements Runnable{

        private DatagramSocket socket;
        private DatagramPacket packet;
        private String message;

        TLDdns_kr(DatagramSocket socket,DatagramPacket dp,byte[] bf){
            this.socket = socket;
            this.packet = dp;
            String rs1 = new String(bf);
			String rs2 = rs1.trim();
            this.message = rs2;
        }

        @Override
        public void run(){
            
            if(map.containsKey(message) == true){
                try{
                    System.out.println("send");	
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
                    System.out.println("send");	
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
