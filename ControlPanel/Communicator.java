import java.net.*;
import java.util.ArrayList;
import java.io.*;

import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonObject;
import javax.json.JsonReader;
import javax.swing.*;


public class Communicator extends Thread{
       Socket s;
       FileInputStream fin;
       InputStreamReader is;
       static int PORT=9000;
       static String ADDR="192.168.0.101";
       PrintWriter pw;
       int port;
       String addr;
       MainWindow window;
       public Communicator(MainWindow window){
    	   this.window=window;
       }
       public void setUpNetworking(){
    	   try {
   			s=new Socket(ADDR,PORT);
   			is=new InputStreamReader(s.getInputStream());
   			pw=new PrintWriter(s.getOutputStream());
   			
   			System.out.println("Connected");
   			
   			
   		} catch (IOException e) {
   			// TODO Auto-generated catch block
   			e.printStackTrace();
   		}
       }
       public void run(){
    	   setUpNetworking();
    	   new InReader(is).start();
    	   
       }
       public void write(String instruction){
    	   pw.println(instruction);
    	   pw.flush();
       }
       
       class InReader extends Thread{
   		BufferedReader br;
   	    InputStreamReader is;
   	    public InReader(InputStreamReader is){
   	    	this.is=is;
   	    	br=new BufferedReader(is);
   	    }
   		public void read(){
   			try {
   				while(true){
   					String message=br.readLine();
   					if(message=="questions"){
   						message=br.readLine();
   						this.QuestionParser(message);
   					}
   					if(message==""){}
   					if(message==""){}
   				   
   				}
   					
   				
   			} catch (IOException e) {
   				
   			    //JOptionPane.showMessageDialog(win,
   					//	"Host has gone!","Connection Dropped",JOptionPane.ERROR_MESSAGE);
   				System.exit(0);
   				e.printStackTrace();
   			}
   		}
   		public void run(){
   			read();
   		}
   		public void QuestionParser(String JsonString){
   			JsonReader jsonReader = Json.createReader(new StringReader(JsonString));
			JsonObject jsonObject = jsonReader.readObject();
			jsonReader.close();
	        JsonArray questionSetArray = jsonObject.getJsonArray("QuestionSets");
	        
	        ArrayList<ArrayList<Question>> qSet=new  ArrayList<ArrayList<Question>>();
	        for(int i=0;i<6;i++){
	        	qSet.add(new ArrayList<Question>());
	        }
	        ArrayList<JsonObject> jqset=new ArrayList<JsonObject>();
	        for(int i=0;i<6;i++){
		        jqset.add(questionSetArray.getJsonObject(i));
		    }
	       
	        for(int i=0;i<6;i++){
	        	int total=jqset.get(i).getInt("Total");
	        	JsonArray jqarray=jqset.get(i).getJsonArray("Questions");
	        	for(int j=0;j<total;j++){
		        	qSet.get(i).add(new Question());
		        }
	        	for(int j=0;i<total;j++){
	  	        	JsonObject jo=jqarray.getJsonObject(j);
	  	        	qSet.get(i).get(j).setID(jo.getInt("id"));
	  	        	qSet.get(i).get(j).setQuestion(jo.getString("Question"));
	  		        qSet.get(i).get(j).setMCstate(jo.getBoolean("isMC"));
	  		        
	  		        JsonObject option=jo.getJsonObject("option");
	  		        
	  		      
	  		        qSet.get(i).get(j).setOption(option.getString("A"),0);
	  		        qSet.get(i).get(j).setOption(option.getString("B"),1);
	  		        qSet.get(i).get(j).setOption(option.getString("C"),2);
	  		        qSet.get(i).get(j).setOption(option.getString("D"),3);
	  		        
	  		        qSet.get(i).get(j).setCorrectAns(jo.getString("CorrectAns"));
	  		        qSet.get(i).get(j).setImgPath(jo.getString("imgPath"));
	  		        
	  		         System.out.println(qSet.get(i).get(j));
	  	        }
	        }
	        
   		}
   	}
   	
}
