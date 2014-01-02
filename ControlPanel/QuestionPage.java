import javax.swing.*;

import java.awt.*;
import java.awt.event.*;

public class QuestionPage extends JPanel implements ActionListener{
   static String marksA="0";
   static String marksD="0";
   static String marksH="0";
   static String marksJ="0";
   static String marksL="0";
   static String marksM="0";
	
   JLabel question;
   boolean mc;
   JRadioButton A;
   JRadioButton B;
   JRadioButton C;
   JRadioButton D;
   JButton checkAns;
   JButton addMarks;
   
   JTextField changeMarksA;
   JTextField changeMarksD;
   JTextField changeMarksJ;
   JTextField changeMarksH;
   JTextField changeMarksL;
   JTextField changeMarksM;
   
   JLabel Augustin=new JLabel("Augustin");
   JLabel Deusdedit=new JLabel("Deusdedit"); 
   JLabel Justus=new JLabel("Justus"); 
   JLabel Honorius=new JLabel("Honorius"); 
   JLabel Laurentius=new JLabel("Laurentius"); 
   JLabel Mellitus=new JLabel("Mellitus"); 
   
   JButton updateMarksA;
   JButton updateMarksD;
   JButton updateMarksJ;
   JButton updateMarksH;
   JButton updateMarksL;
   JButton updateMarksM;
   JButton next;
   JButton prev;
   JButton chooserhouse;
   JButton backmain;
   String index;
   JPanel PointsPanel;
   JPanel QuestionPanel;
   JPanel ButtonPanel;
   MainWindow window;
   WelcomePanel welcome;
   ButtonGroup ans;
   JButton backtohouse;
   JButton backtomain;
   RXCardLayout cl;
   RXCardLayout clmain;

   public QuestionPage(MainWindow window,WelcomePanel welcome, String Question){
	   this.welcome=welcome;
	   question=new JLabel(Question);
	   this.window=window;
	   cl=(RXCardLayout)(window.QuestionList.getLayout());
	   clmain=(RXCardLayout)(window.cardstack.getLayout());
	   A=new JRadioButton("A");
	   B=new JRadioButton("B");
	   C=new JRadioButton("C");
	   D=new JRadioButton("D");
	   backtohouse=new JButton("Back to Select House");
	   backtomain=new JButton("Back to Main");
	   
	   ans=new ButtonGroup();
	   ans.add(A);
	   ans.add(B);
	   ans.add(C);
	   ans.add(D);
	   
	   checkAns=new JButton("Check Answer");
	   addMarks=new JButton("Add Marks to current house");
	   
	   changeMarksA=new JTextField(marksA,5);
	   changeMarksD=new JTextField(marksA,5);
	   changeMarksJ=new JTextField(marksA,5);
	   changeMarksH=new JTextField(marksA,5);
	   changeMarksL=new JTextField(marksA,5);
	   changeMarksM=new JTextField(marksA,5);
	   
	   updateMarksA=new JButton("Update");
	   updateMarksD=new JButton("Update");
	   updateMarksJ=new JButton("Update");
	   updateMarksH=new JButton("Update");
	   updateMarksL=new JButton("Update");
	   updateMarksM=new JButton("Update");
	   
	   next=new JButton("Next Question");
	   prev=new JButton("Previous Question");
	   chooserhouse=new JButton("Back to choose House");
	   backmain=new JButton("Back to Main Screen");
	   
	   PointsPanel=new JPanel(new GridLayout(6,3));
	   PointsPanel.add(updateMarksA);
	   PointsPanel.add(Augustin);
	   PointsPanel.add(changeMarksA);
	   
	   PointsPanel.add(updateMarksD);
	   PointsPanel.add(Deusdedit);
	   PointsPanel.add(changeMarksD);
	   
	   PointsPanel.add(updateMarksJ);
	   PointsPanel.add(Justus);
	   PointsPanel.add(changeMarksJ);
	   
	   PointsPanel.add(updateMarksH);
	   PointsPanel.add(Honorius);
	   PointsPanel.add(changeMarksH);
	   
	   PointsPanel.add(updateMarksL);
	   PointsPanel.add(Laurentius);
	   PointsPanel.add(changeMarksL);
	   
	   PointsPanel.add(updateMarksM);
	   PointsPanel.add(Mellitus);
	   PointsPanel.add(changeMarksM);
	   
	
	   QuestionPanel=new JPanel();
	   QuestionPanel.setLayout(new BoxLayout(QuestionPanel,BoxLayout.PAGE_AXIS));
	   QuestionPanel.add(question);
	   QuestionPanel.add(A);
	   QuestionPanel.add(B);
	   QuestionPanel.add(C);
	   QuestionPanel.add(D);
	   
	   ButtonPanel=new JPanel();
	   ButtonPanel.add(checkAns);
	   ButtonPanel.add(addMarks);
	   ButtonPanel.add(next);
	   ButtonPanel.add(prev);
	   QuestionPanel.add(ButtonPanel);
	   this.add(QuestionPanel,BorderLayout.WEST);
	   this.add(PointsPanel,BorderLayout.EAST);
	   JPanel back=new JPanel();
	   back.add(backtohouse);
	   back.add(backtomain);
	  
	   this.add(back,BorderLayout.SOUTH);
	   
	   backtohouse.addActionListener(this);
	   backtomain.addActionListener(this);
	   next.addActionListener(this);
	   prev.addActionListener(this);
	   updateMarksA.addActionListener(this);
	   updateMarksD.addActionListener(this);
	   updateMarksJ.addActionListener(this);
	   updateMarksH.addActionListener(this);
	   updateMarksL.addActionListener(this);
	   updateMarksM.addActionListener(this);
   }

@Override
public void actionPerformed(ActionEvent e) {
	
	if(e.getSource()==next){
		if(cl.isNextCardAvailable())
		cl.next(window.QuestionList);
		else{
		JOptionPane.showMessageDialog(window.mainframe, "Arrived to last Question", "Set Completed",JOptionPane.PLAIN_MESSAGE);
		if(welcome.isChallenge)
		clmain.show(window.cardstack, "WELCOME");
		else
		clmain.show(window.cardstack, "HOUSE");
	}
	}
	if(e.getSource()==prev){
		if(cl.isPreviousCardAvailable())
		cl.previous(window.QuestionList);
		else
		JOptionPane.showMessageDialog(window.mainframe, "This the first question");
	}
	if(e.getSource()==updateMarksA){
		JOptionPane.showMessageDialog(window.mainframe, "Augustin Updated, "+changeMarksA.getText()+" Points");
		QuestionPage.marksA=changeMarksA.getText();
	}
	if(e.getSource()==updateMarksD){
		JOptionPane.showMessageDialog(window.mainframe, "Deusdedit Updated, "+changeMarksD.getText()+" Points");
		QuestionPage.marksD=changeMarksD.getText();
	}
	if(e.getSource()==updateMarksJ){
		JOptionPane.showMessageDialog(window.mainframe, "Justus Updated, "+changeMarksJ.getText()+" Points");
		QuestionPage.marksJ=changeMarksJ.getText();
	}
	if(e.getSource()==updateMarksH){
		JOptionPane.showMessageDialog(window.mainframe, "Honorius Updated, "+changeMarksH.getText()+" Points");
		QuestionPage.marksH=changeMarksH.getText();
	}
	if(e.getSource()==updateMarksL){
		JOptionPane.showMessageDialog(window.mainframe, "Laurentius Updated, "+changeMarksL.getText()+" Points");
		QuestionPage.marksL=changeMarksL.getText();
	}
	if(e.getSource()==updateMarksM){
		JOptionPane.showMessageDialog(window.mainframe, "Mellitus Updated, "+changeMarksM.getText()+" Points");
		QuestionPage.marksM=changeMarksM.getText();
	}
	
	if(e.getSource()==backtomain){
		clmain.show(window.cardstack,"WELCOME");
	}
	if(e.getSource()==backtohouse){
		clmain.show(window.cardstack,"HOUSE");
	}
	
}
}

