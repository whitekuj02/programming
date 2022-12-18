package UDP;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.io.*;
import java.net.*;


public class UDPLocalDNS {
    private final static HashMap <String, String> cashing = new HashMap<String, String>();
    public static void main(String[] args){
        try{
            DatagramSocket ds = new DatagramSocket(7077);//7077 localdns port
    
            ExecutorService pool = Executors.newFixedThreadPool(20);
            while(true){
                byte[] bf = new byte[300];
                DatagramPacket dp = new DatagramPacket(bf,  bf.length);

                System.out.println("Waiting for a packet reception..");		
                ds.receive(dp);

                pool.execute(new Localdns(ds,dp,bf));
            }
        }
        catch(IOException e){

        }
    }

    public static class Localdns implements Runnable{

        private DatagramSocket socket;
        private DatagramPacket packet;
        private String client_message;

        Localdns(DatagramSocket socket,DatagramPacket dp,byte[] bf){
            this.socket = socket;
            this.packet = dp;
            String rs1 = new String(bf);
			String rs2 = rs1.trim();
            this.client_message = rs2;
        }

        @Override
        public void run(){
            if(cashing.containsKey(client_message) == true){
                try{
                    System.out.println("cashing hit");
                    String ip = cashing.get(client_message);
                    DatagramSocket ds = new DatagramSocket();
                    InetAddress ia = InetAddress.getByName("localhost");
                    byte[] bf_send = ip.getBytes();
                    DatagramPacket dp_send = new DatagramPacket(bf_send, bf_send.length, ia, this.packet.getPort());
                    ds.send(dp_send);
                }
                catch(IOException e){

                }
            }
            else{
                try {
                    System.out.println("cashing miss");
                    DatagramSocket ds = new DatagramSocket();
                    InetAddress ia = InetAddress.getByName("localhost");
                    String msg = new String();
                    if(client_message.contains(".com") == true){
                        msg = ".com";
                    }
                    else if(client_message.contains(".kr") == true){
                        msg = ".kr";
                    }
                    else if(client_message.contains(".net") == true){
                        msg = ".net";
                    }
                    else if(client_message.contains(".uk") == true){
                        msg = ".uk";
                    }
                    else{
                        msg = "0";
                    }
                    byte[] bf_send = msg.getBytes();					
                    DatagramPacket dp_send = new DatagramPacket(bf_send, bf_send.length, ia, 7007);
                    ds.send(dp_send);
                    //root로 보내기
    
                    byte[] bf_receive = new byte[300];					
                    DatagramPacket dp_receive = new DatagramPacket(bf_receive, bf_receive.length);
                    ds.receive(dp_receive);
                    //root에서 받기

                    String a = new String(bf_receive);
                    int root_message = Integer.parseInt(a.trim());
            
                    if(root_message != 0){
                        byte[] bf_TLDsend = this.client_message.getBytes();
                        DatagramPacket dp_TLDsend = new DatagramPacket(bf_TLDsend, bf_TLDsend.length, ia, root_message);
                        ds.send(dp_TLDsend);
                        //TLD 보내기

                        byte[] bf_TLDreceive = new byte[300];					
                        DatagramPacket dp_TLDreceive = new DatagramPacket(bf_TLDreceive, bf_TLDreceive.length);
                        ds.receive(dp_TLDreceive);
                        //TLD 받기

                        String b = new String(bf_TLDreceive);
                        String TLD_message = b.trim();

                        //client 보내기
                        byte[] bf_clientsend = TLD_message.getBytes();
                        DatagramPacket dp_clientsend = new DatagramPacket(bf_clientsend, bf_clientsend.length, ia, this.packet.getPort());
                        ds.send(dp_clientsend);

                        if(!TLD_message.equals("0")){
                            cashing.put(this.client_message,TLD_message);
                        }
                    //cashing 저장 하기
                    }
                    else{
                        String message = "0";
                        byte[] bf_clientsend = message.getBytes();
                        DatagramPacket dp_clientsend = new DatagramPacket(bf_clientsend, bf_clientsend.length, ia, this.packet.getPort());
                        ds.send(dp_clientsend);
                    }
                        
                } catch(IOException e) {}
            }
        }
    }
}
