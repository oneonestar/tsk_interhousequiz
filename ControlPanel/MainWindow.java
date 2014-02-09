import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonObject;
import javax.json.JsonReader;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.ArrayList;

public class MainWindow {
    JFrame mainframe;
    WelcomePanel welcome;
    JPanel cardstack;
    JPanel QuestionList;
	HouseChooser houseList;
	SetChooser setChooser;
	JPanel QuestionList2;
	QuestionPages qp1;
	QuestionPages qp2;
	QuestionPages qp3;
	QuestionPages qp4;
	QuestionPages qp5;
	QuestionPages qp6;
	ArrayList<QuestionPage> QuestionSets;
	ArrayList<Question> qArray;
	Socket socket;
	LoginPanel login;
	Question q;
	Communicator c;
	MarksPanel mp;
    public MainWindow(){
    	c=new Communicator(this);
    	mainframe=new JFrame("TSK Interhouse Quiz - Admin Control Panel");
    	cardstack=new JPanel(new RXCardLayout());
    	ArrayList<Question> qArray=new ArrayList<Question>();
    	login=new LoginPanel(qArray);
    	welcome=new WelcomePanel(this,c);
    	setChooser=new SetChooser(this);
    	houseList=new HouseChooser(this,setChooser);
    	mp=new MarksPanel(c);
    	
    }
    public void go(){
    	mainframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    	
    	cardstack.add(welcome,"WELCOME");
    	cardstack.add(houseList,"HOUSE");
    	cardstack.add(setChooser,"SET");
    	cardstack.add(mp,"CHALLENGE");
    	
    	mainframe.getContentPane().add(cardstack);
    	mainframe.setSize(1024,600);
    	mainframe.setVisible(true);
    	c.start();
    }
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
        new MainWindow().go();
	}
	

}
