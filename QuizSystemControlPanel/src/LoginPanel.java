import java.awt.CardLayout;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;


import javax.swing.*;


public class LoginPanel extends JPanel implements ActionListener {
	JButton login;
	JTextField hostname;
	JTextField port;
	JLabel hostl;
	JLabel portl;
	int portn;
	String addr;
	Communicator c;
	MainWindow window;
	private QuestionListPage qlp;
	private MarksPanel mp;
	private JPanel paneladdr;
	private JPanel panelport;
	
	public LoginPanel(MainWindow mw){
		window=mw;
		this.setBackground(Color.GRAY);
		hostname=new JTextField(20);
		hostname.setText("localhost");
		hostname.setSize(20, 5);
		port=new JTextField(20);
		port.setText("5432");
		port.setSize(20, 5);
		login=new JButton("Login");
		portl=new JLabel("Port: ");
		hostl=new JLabel("Host: ");
		
		
		login.addActionListener(this);
		
		paneladdr=new JPanel();
		panelport=new JPanel();
		paneladdr.setBackground(Color.gray);
		panelport.setBackground(Color.gray);
		paneladdr.add(hostl);
		paneladdr.add(hostname);
		panelport.add(portl);
		panelport.add(port);
		this.add(paneladdr);
		this.add(panelport);
		this.add(login);
		
		//qArray=new ArrayList<Question>();
	}
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		c=new Communicator(window, hostname.getText(), Integer.parseInt(port.getText()));
		c.start();
		mp=new MarksPanel(c);
		qlp=new QuestionListPage(c,mp);
		window.cardstack.add(qlp,"qlp");
		CardLayout cl=(CardLayout) window.cardstack.getLayout();
		cl.show(window.cardstack, "qlp");
		window.mainframe.pack();
		
	}
	/*
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
	}*/

	
	
}
