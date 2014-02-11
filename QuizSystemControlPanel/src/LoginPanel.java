import java.awt.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;

import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonObject;
import javax.json.JsonReader;
import javax.swing.*;


public class LoginPanel extends JPanel {
	JButton login;
	JTextField hostname;
	JTextField port;
	JLabel hostl;
	JLabel portl;
	int portn;
	String addr;
	ArrayList<Question> qArray;
	public LoginPanel(ArrayList<Question> qArray){
		hostname=new JTextField(20);
		hostname.setText("localhost");
		hostname.setSize(20, 5);
		port=new JTextField(20);
		port.setText("5432");
		port.setSize(20, 5);
		login=new JButton("Login");
		portl=new JLabel("Port: ");
		hostl=new JLabel("Host: ");
		this.setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
		this.add(hostl);
		this.add(hostname);
		this.add(portl);
		this.add(port);
		this.add(login);
		this.setPreferredSize(new Dimension(100,100));
		qArray=new ArrayList<Question>();
	}
	public void parseQuestions(File f){
		try {
			FileInputStream in=new FileInputStream(f);
			JsonReader jsonReader = Json.createReader(in);
			JsonObject jsonObject = jsonReader.readObject();
			jsonReader.close();
	        in.close();
	        int total=jsonObject.getInt("Total");
	        JsonArray questionArray = jsonObject.getJsonArray("Questions");
	       
	        for(int i=0;i<total;i++){
	        	qArray.add(new Question());
	        }
	        
	        int index=0;
	        for(;index<total;index++){
	        	JsonObject jo=questionArray.getJsonObject(index);
	        	qArray.get(index).setID(jo.getInt("id"));
	        	qArray.get(index).setQuestion(jo.getString("Question"));
	        	qArray.get(index).setMCstate(jo.getBoolean("isMC"));
	        
	        	JsonObject option=jo.getJsonObject("option");
	        
	      
	        	qArray.get(index).setOption(option.getString("A"),0);
	        	qArray.get(index).setOption(option.getString("B"),1);
	        	qArray.get(index).setOption(option.getString("C"),2);
	        	qArray.get(index).setOption(option.getString("D"),3);
	        
	        	qArray.get(index).setCorrectAns(jo.getString("CorrectAns"));
	        	qArray.get(index).setImgPath(jo.getString("imgPath"));
	        
	        	System.out.println(qArray.get(index));
	        }
	        
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
