package UDP;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.io.*;
import java.net.*;


public class TLDDNS_net {
    private final static HashMap <String, String> map = new HashMap<String, String>();
    
    public static void main(String[] args){
        map.put("uijin.net","112.239.122.243");
        map.put("noa.net","129.253.172.153");
        map.put("dns.net","235.239.197.143");
        //값추가
        try{
            DatagramSocket ds = new DatagramSocket(7073);//7073 TLDdns_net port
    
            ExecutorService pool = Executors.newFixedThreadPool(20);
            while(true){
                byte[] bf = new byte[300];
                DatagramPacket dp = new DatagramPacket(bf,  bf.length);

                System.out.println("Waiting for a packet reception..");		
                ds.receive(dp);
                
                pool.execute(new TLDdns_net(ds,dp,bf));
            }
        }
        catch(IOException e){

        }
    }

    public static class TLDdns_net implements Runnable{

        private DatagramSocket socket;
        private DatagramPacket packet;
        private String message;

        TLDdns_net(DatagramSocket socket,DatagramPacket dp,byte[] bf){
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
