package socket;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.io.*;
import java.net.*;
import java.math.*;

class TCPServer implements Runnable{
    private Socket socket;
    private int status = 0;
    private Double result = 0.0;
    private String result_string;
    
    TCPServer(Socket socket){
        this.socket = socket;
    }

    @Override
    public void run(){
        try{
            var in = new Scanner((socket.getInputStream()));
            var out= socket.getOutputStream();
            while(in.hasNextLine()){
            	String tmp = in.nextLine();
            	
            	String[] temp = tmp.split(" ");
            	String mod = temp[0];
            	String data = temp[1];
            	
            	String[] num = data.split(",");
            	Double a = 0.0,b = 0.0;
            	//byte 배열의 0번쨰 인덱스에 status를 고유의 숫자로 보냄 
            	//0: 성공 1: data 숫자 많음 2: 0으로 나눔 3: mod없을 때  4: data 숫자가 적음
            	if(num.length == 2) {
            		a = Double.parseDouble(num[0]);
            		b = Double.parseDouble(num[1]);
                    
                    switch(mod.toUpperCase()) {
                        case "ADD":
                            status = 0;
                            result = a + b;
                            
                            break;
                        case "MINUS":
                            status = 0;
                            result = a - b;
                            
                            break;
                        case "MUL":
                            status = 0;
                            result = a * b;
                            
                            break;
                        case "DIV":
                            if(b == 0) {
                                //2: error
                                status = 2;
                                //result -> 0
                                result = 0.0;
                                
                                
                            }
                            else {
                                status = 0;
                                result = a / b;
                                
                            }
                            break;
                        default:
                            //3: error
                            status = 3;
                            //result -> 0
                            result = 0.0;
                            
                            break;
                    }
            	}
                else if(num.length < 2){
            		//1: error
            		this.status = 4;
            		//result -> 0
            		result = 0.0;
            		
            	}
            	else {
            		//1: error
            		this.status = 1;
            		//result -> 0
            		result = 0.0;
            		
            	}
            	
                if(result != Math.floor(result)){
                    result_string = Double.toString(result);
                }
                else{
                    result_string = Integer.toString((int)Math.floor(result));
                }
            	//byte 배열 index 0 status, index 1~ string ASCII 값을 넣어줌 
                byte[] byte_data = new byte[result_string.length() + 2];
                byte_data[0] = (byte)(byte_data.length);
                byte_data[1] = (byte)status;
                byte[] string_split = result_string.getBytes();
                for(int i = 2;i<string_split.length + 2;i++) {
                    byte_data[i] = string_split[i-2];
                }
                // 서버 정보 보내기

                //데이터 보내기
                out.write(byte_data);
                out.flush();
                //싹다 초기화
            
                status = 0;
                result = 0.0;
                result_string = Double.toString(result);
            }
            
        }//protocol
        catch(Exception e){
            System.out.println("Error:" + socket);
        }
        finally{
            try{
                socket.close();
            }
            catch(IOException e){
            	System.out.println("IOError:" + socket);
            }
            System.out.println("Closed:" + socket);
        }
    }
}

public class TCPServer1 {

	
    public static void main(String[] args) throws Exception{
        try{
        	ServerSocket listener = new ServerSocket(7777);
        	System.out.println("The Capitalize Server is running...");


        	ExecutorService pool = Executors.newFixedThreadPool(20);
        	while(true){
        		Socket socket = listener.accept();
        		pool.execute(new TCPServer(socket));
        	}
        }
        catch(Exception e){
        	System.out.println(e);
        }
    }
}