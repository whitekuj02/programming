package UDP;

import java.net.*;
import java.io.*;
import java.util.Scanner;

public class UDPclient {
	public static void main(String args[]) {
		while(true){
            try {
                DatagramSocket ds = new DatagramSocket();
                InetAddress ia = InetAddress.getByName("localhost");
    
                Scanner keybored = new Scanner(System.in);
                
                String msg = keybored.nextLine();
                if(msg.equalsIgnoreCase("end")){
                    break;
                }
                byte[] bf = msg.getBytes();					
                DatagramPacket dp = new DatagramPacket(bf, bf.length, ia, 7077);
                ds.send(dp);
                
                byte[] bf_receive = new byte[300];					
                DatagramPacket dp_receive = new DatagramPacket(bf_receive, bf_receive.length);
                ds.receive(dp_receive);
    
                String a = new String(bf_receive);
                String ip = a.trim();
                if(ip.equals("0")){
                    System.out.println("not found");
                }
                else{
                    System.out.println(ip);
                }
            } catch(IOException e) {}
        }
	}
}
