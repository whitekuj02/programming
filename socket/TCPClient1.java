package socket;
import java.io.*;
import java.net.*;
import java.util.*;

public class TCPClient1 {
    public static void main(String[] args){
        try{
            String[] linee = new String[2];
            try{
                //파일 객체 생성
                File file = new File("socket//server_information.dat");
                //입력 스트림 생성
                FileReader filereader = new FileReader(file);
                //입력 버퍼 생성
                BufferedReader bufReader = new BufferedReader(filereader);
                for(int i = 0;i<2;i++){
                    linee[i] = bufReader.readLine();
                }
                       
                bufReader.close();
            }
            catch(FileNotFoundException e){
                linee[0] = "127.0.0.1";
                linee[1] = "7777";
            }

            Socket socket = new Socket(linee[0], Integer.parseInt(linee[1]));
            var in = socket.getInputStream();
            var out = new PrintWriter(socket.getOutputStream(), true);
            var console = new Scanner(System.in);
            while(true){
                System.out.println("Enter a line to be capitalized:");
                String line = console.nextLine();
                out.println(line);
                if(line.trim().equals("BYE")){
                    socket.close();
                    console.close();
                    break;
                }
                //
                byte[] input = new byte[256];
                try {
                    //파일 읽기
                	in.read(input);
                	//for(int i =0;i<input[0];i++) {
                	//	System.out.print(input[i] + " ");
                	//}
                    //input[0] = 배열 길이
                    //input[1] = status
                    //나머지 = 각 자리의 ASCII 값 

                    switch(input[1]){
                        case 0:
                            //성공
                            System.out.print("Answer: ");
                            for(int i = 2; i<input[0]; i++){
                                System.out.print((char)input[i]);
                            }
                            System.out.print("\n");
                            break;
                        case 1:
                            //data 숫자 많음
                            System.out.println("Error message: ");
                            System.out.println("too many arguments ");
                            break;
                        case 2:
                            //0으로 나눔
                            System.out.println("Error message: ");
                            System.out.println("divided by zero");
                            break;
                        case 3:
                            //mod가 없음
                            System.out.println("Error message: ");
                            System.out.println("not permit mod");
                            break;
                        case 4:
                            //data 숫자 적음
                            System.out.println("Error message: ");
                            System.out.println("too few arguments");
                            break;
                    }
                    
                }
                catch(Exception e){
                	System.out.println("error");
                }
                //
            }
        }catch (FileNotFoundException e) {
            // TODO: handle exception
            System.out.println("file not found");
        }catch(IOException e){
            System.out.println("error IO");
        }
        catch(Exception e){
            System.out.println("exception");
        }
    }
}
