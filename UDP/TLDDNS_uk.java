package UDP;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.io.*;
import java.net.*;


public class TLDDNS_uk {
    public static void main(String[] args){
        try{
            DatagramSocket ds = new DatagramSocket(7074);//7074 TLDdns_uk port
    
            ExecutorService pool = Executors.newFixedThreadPool(20);
            while(true){
                byte[] bf = new byte[300];
                DatagramPacket dp = new DatagramPacket(bf,  bf.length);

                System.out.println("Waiting for a packet reception..");		
                ds.receive(dp);
                
                pool.execute(new TLDdns_uk(ds,dp,bf));
            }
        }
        catch(IOException e){

        }
    }

    public static class TLDdns_uk implements Runnable{

        private DatagramSocket socket;
        private DatagramPacket packet;
        private String message;

        TLDdns_uk(DatagramSocket socket,DatagramPacket dp,byte[] bf){
            this.socket = socket;
            this.packet = dp;
            String rs1 = new String(bf);
			String rs2 = rs1.trim();
            this.message = rs2;
        }

        @Override
        public void run(){
            if(this.message.equals("gachon.ac.uk")){
                try{
                    System.out.println("send");	
                    DatagramSocket ds = new DatagramSocket();
			        InetAddress ia = InetAddress.getByName("localhost");
			        String msg = "125.209.222.143";
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
