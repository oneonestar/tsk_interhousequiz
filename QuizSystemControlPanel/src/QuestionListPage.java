import java.awt.event.*;
import java.util.ArrayList;

import javax.swing.*;

import java.awt.Dimension;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.FlowLayout;
import java.awt.BorderLayout;

public class QuestionListPage extends JPanel{
    final static JLabel together=new JLabel("齊答題");
    final static JLabel complusory=new JLabel("必答題");
    static ArrayList<JLabel> Sets;
    final static JLabel basicComp=new JLabel("基本部分");
    final static JLabel AddComp=new JLabel("附加部分");
    final static JLabel Sci1=new JLabel("科學1");
    final static JLabel Sci2=new JLabel("科學2");
    final static JLabel IH1=new JLabel("綜合人文1");
    final static JLabel IH2=new JLabel("綜合人文2");
    final static JLabel News1=new JLabel("時事1");
    final static JLabel News2=new JLabel("時事2");
    final static JLabel commonsense=new JLabel("唔識抵死題(常識)");
    ArrayList<JButton> togetherB; 
    ArrayList<JButton> complusoryB;
    ArrayList<JButton> basicCompB;
    ArrayList<JButton> AddCompB; 
	ArrayList<JPanel> panels;
	JPanel mainPanel;
	Communicator c;
	MarksPanel mp;
	JScrollPane scrollPane;
	ButtonListener BL;
	JButton Answer;
	
	public QuestionListPage(Communicator c, MarksPanel mp){
		BL=new ButtonListener();
		this.c=c;
		this.mp=mp;
		mainPanel=new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel,BoxLayout.PAGE_AXIS));
		scrollPane=new JScrollPane(mainPanel);
		Answer=new JButton("Check Answer");
		Answer.addActionListener(BL);
		Answer.setEnabled(false);
		
		panels=new ArrayList<JPanel>();
		for(int a=0;a<20;a++){
			panels.add(new JPanel());
		}
	
		togetherB=new ArrayList<JButton>();
		complusoryB=new ArrayList<JButton>();
		basicCompB=new ArrayList<JButton>();
		AddCompB=new ArrayList<JButton>();
		Sets=new ArrayList<JLabel>();
		mainPanel.add(together);
		for(int i=0;i<4;i++){
			togetherB.add(new JButton("Question "+(i+1)));
			
		}
		
		for(JButton b : togetherB){
			panels.get(0).add(b);
			//b.addActionListener(this);
		}
		mainPanel.add(panels.get(0));
		mainPanel.add(complusory);
		mainPanel.add(basicComp);
		for(int i=0;i<7;i++){
			Sets.add(new JLabel("題目集"+(i+1)));
			
		}
		
		for(int j=0;j<28;j++){
				basicCompB.add(new JButton("Question "+(j%4+1)));
			}
		int j=0;
		for(int i=0;i<7;i++){
			
			for(int k=j;k<j+4;k++){
				panels.get(i+1).add(basicCompB.get(k));
				mainPanel.add(panels.get(i+1));
				//basicCompB.get(k).addActionListener(this);
			}
			panels.get(i+1).add(Sets.get(i));
			j+=4;
		}
		
		
		AddCompB.add(new JButton("科學1 1"));
		
		AddCompB.add(new JButton("科學1 2"));
		
		AddCompB.add(new JButton("科學2 1"));
		
		AddCompB.add(new JButton("科學2 2"));
		
		AddCompB.add(new JButton("綜合人文1 1"));
		
		AddCompB.add(new JButton("綜合人文1 2"));
		
		AddCompB.add(new JButton("綜合人文2 1"));
		
		AddCompB.add(new JButton("綜合人文2 2"));
		
		AddCompB.add(new JButton("時事1 1"));
		
		AddCompB.add(new JButton("時事1 2"));
		
		AddCompB.add(new JButton("時事2 1"));
		
		AddCompB.add(new JButton("時事2 2"));
		
		AddCompB.add(new JButton("唔識抵死題(常識) 1"));
		
		AddCompB.add(new JButton("唔識抵死題(常識) 2"));
		
		
		panels.get(9).add(AddComp);
		panels.get(9).add(Sci1);
		panels.get(9).add(AddCompB.get(0));
		panels.get(9).add(AddCompB.get(1));
		panels.get(10).add(Sci2);
		panels.get(10).add(AddCompB.get(2));
		panels.get(10).add(AddCompB.get(3));
		panels.get(11).add(IH1);
		panels.get(11).add(AddCompB.get(4));
		panels.get(11).add(AddCompB.get(5));
		panels.get(12).add(IH2);
		panels.get(12).add(AddCompB.get(6));
		panels.get(12).add(AddCompB.get(7));
		panels.get(13).add(News1);
		panels.get(13).add(AddCompB.get(8));
		panels.get(13).add(AddCompB.get(9));
		panels.get(14).add(News2);
		panels.get(14).add(AddCompB.get(10));
		panels.get(14).add(AddCompB.get(11));
		panels.get(15).add(commonsense);
		panels.get(15).add(AddCompB.get(12));
		panels.get(15).add(AddCompB.get(13));
        for(int i=9;i<16;i++){
        	mainPanel.add(panels.get(i));
        }
		setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));
		this.add(scrollPane,BorderLayout.LINE_START);
		scrollPane.setPreferredSize(new Dimension(600,600));
		this.add(Answer,BorderLayout.PAGE_END);
		this.add(mp,BorderLayout.EAST);
		//this.add(new MarksPanel());
		for(JButton b : togetherB){
			b.addActionListener(BL);
		}
		for(JButton b : complusoryB){
			b.addActionListener(BL);
		}
		for(JButton b : AddCompB){
			b.addActionListener(BL);
		}
	
		
		
		
	}
	
	class ButtonListener implements ActionListener{

		@Override
		public void actionPerformed(ActionEvent e) {
			// TODO Auto-generated method stub
			if(e.getSource()==togetherB.get(0)){
				c.write("TOG 1");
				Answer.setEnabled(true);
			}if(e.getSource()==togetherB.get(1)){
				c.write("TOG 2");
				Answer.setEnabled(true);
			}if(e.getSource()==togetherB.get(2)){
				c.write("TOG 3");
				Answer.setEnabled(true);
			}if(e.getSource()==togetherB.get(3)){
				c.write("TOG 4");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(0)){
				c.write("SET1 1");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(1)){
				c.write("SET1 2");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(2)){
				c.write("SET1 3");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(3)){
				c.write("SET1 4");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(4)){
				c.write("SET2 1");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(5)){
				c.write("SET2 2");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(6)){
				c.write("SET2 3");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(7)){
				c.write("SET2 4");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(8)){
				c.write("SET3 1");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(9)){
				c.write("SET3 2");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(10)){
				c.write("SET3 3");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(11)){
				c.write("SET3 4");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(12)){
				c.write("SET4 1");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(13)){
				c.write("SET4 2");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(14)){
				c.write("SET4 3");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(15)){
				c.write("SET4 4");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(16)){
				c.write("SET5 1");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(17)){
				c.write("SET5 2");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(18)){
				c.write("SET5 3");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(19)){
				c.write("SET5 4");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(20)){
				c.write("SET6 1");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(21)){
				c.write("SET6 2");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(22)){
				c.write("SET6 3");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(23)){
				c.write("SET6 4");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(24)){
				c.write("SET7 1");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(25)){
				c.write("SET7 2");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(26)){
				c.write("SET7 3");
				Answer.setEnabled(true);
			}if(e.getSource()==complusoryB.get(27)){
				c.write("SET7 4");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(0)){
				c.write("SCI1 1");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(1)){
				c.write("SCI1 2");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(2)){
				c.write("SCI2 1");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(3)){
				c.write("SCI2 2");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(4)){
				c.write("IH1 1");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(5)){
				c.write("IH1 2");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(6)){
				c.write("IH2 1");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(7)){
				c.write("IH2 2");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(8)){
				c.write("News1 1");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(9)){
				c.write("News1 2");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(10)){
				c.write("News2 1");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(11)){
				c.write("News2 2");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(12)){
				c.write("CS 1");
				Answer.setEnabled(true);
			}if(e.getSource()==AddCompB.get(13)){
				c.write("CS 2");
				Answer.setEnabled(true);
			}if(e.getSource()==Answer){
				c.write("CHKANS");
				Answer.setEnabled(false);
			}
			
		}
		
	}

}
