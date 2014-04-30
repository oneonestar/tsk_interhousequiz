import javax.swing.*;

import java.awt.CardLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
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
	QuestionListPage qlp;
//	ArrayList<QuestionPage> QuestionSets;
//	ArrayList<Question> qArray;
	Socket socket;
	LoginPanel login;
//	Question q;
	Communicator c;
	MarksPanel mp;
	MarksPanel mpa;
    public MainWindow(){
    	
    	mainframe=new JFrame("TSK Interhouse Quiz - Admin Control Panel");
    	cardstack=new JPanel(new CardLayout());
    	
    	login=new LoginPanel(this);
    	welcome=new WelcomePanel(this,c);
    	setChooser=new SetChooser(this);
    	houseList=new HouseChooser(this,setChooser);
    	
    	
    	mp=new MarksPanel(c);
    	mpa=new MarksPanel(c);
    	qlp=new QuestionListPage(c,mpa);
    }
    public void go(){
    	//Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    	mainframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    	login=new LoginPanel(this);
    	cardstack.add(login,"LOGIN");
    	
    	//cardstack.add(qlp,"qlp");
    	
    	
    	mainframe.getContentPane().add(cardstack);
    	mainframe.setSize(1024,768);
    	mainframe.setVisible(true);
    	//c.start();
    }
	public static void main(String[] args){
        new MainWindow().go();
	}
	

}
