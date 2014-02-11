import javax.swing.*;

import java.awt.*;
import java.awt.event.*;

public class QuestionPage extends JPanel implements ActionListener{
   static String marksA="100";
   static String marksD="100";
   static String marksH="100";
   static String marksJ="100";
   static String marksL="100";
   static String marksM="100";
	
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
   JButton next;
   JButton prev;
   JButton chooserhouse;
   JButton backmain;
   String index;
   MainWindow window;
   WelcomePanel welcome;
   ButtonGroup ans;
   JButton backtohouse;
   JButton backtomain;
   RXCardLayout cl;
   RXCardLayout clmain;
   
   String questionstatement;
   String [] option;
   String correctAns;
  JPanel panel;

   public QuestionPage(MainWindow window,WelcomePanel welcome, Question q){
	   questionstatement=q.getQuestion();
	   mc=q.getMCstate();
	   if(mc){
	   for(int i=0;i<4;i++){
		   option[i]=q.getOption(i);
	   }
	   ans=new ButtonGroup();
	  }
	   
	   this.welcome=welcome;
	   this.window=window;
	   //cl=(RXCardLayout)(window.QuestionList.getLayout());
	   clmain=(RXCardLayout)(window.cardstack.getLayout());
	   chooserhouse=new JButton("Back to choose House");
	   backmain=new JButton("Back to Main Screen");
	   GridBagLayout gridBagLayout = new GridBagLayout();
	   gridBagLayout.columnWidths = new int[]{35, 164, 101, 119, 18, 1, 201, 0};
	   gridBagLayout.rowHeights = new int[]{23, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	   gridBagLayout.columnWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
	   gridBagLayout.rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
	   setLayout(gridBagLayout);
	        question=new JLabel(questionstatement);
	        GridBagConstraints gbc_question = new GridBagConstraints();
	        gbc_question.insets = new Insets(0, 0, 5, 5);
	        gbc_question.gridx = 1;
	        gbc_question.gridy = 1;
	        add(question, gbc_question);
	        GridBagConstraints gbc_Augustin = new GridBagConstraints();
	        gbc_Augustin.anchor = GridBagConstraints.EAST;
	        gbc_Augustin.insets = new Insets(0, 0, 5, 5);
	        gbc_Augustin.gridx = 5;
	        gbc_Augustin.gridy = 1;
	        add(Augustin, gbc_Augustin);
	        
	        changeMarksA=new JTextField(marksA,5);
	        GridBagConstraints gbc_changeMarksA = new GridBagConstraints();
	        gbc_changeMarksA.insets = new Insets(0, 0, 5, 0);
	        gbc_changeMarksA.gridx = 6;
	        gbc_changeMarksA.gridy = 1;
	        add(changeMarksA, gbc_changeMarksA);
	        A=new JRadioButton("A "); //+option[0]);
	        GridBagConstraints gbc_A = new GridBagConstraints();
	        gbc_A.insets = new Insets(0, 0, 5, 5);
	        gbc_A.gridx = 1;
	        gbc_A.gridy = 2;
	        add(A, gbc_A);
	        GridBagConstraints gbc_Deusdedit = new GridBagConstraints();
	        gbc_Deusdedit.anchor = GridBagConstraints.EAST;
	        gbc_Deusdedit.insets = new Insets(0, 0, 5, 5);
	        gbc_Deusdedit.gridx = 5;
	        gbc_Deusdedit.gridy = 2;
	        add(Deusdedit, gbc_Deusdedit);
	        changeMarksD=new JTextField(marksD,5);
	        GridBagConstraints gbc_changeMarksD = new GridBagConstraints();
	        gbc_changeMarksD.insets = new Insets(0, 0, 5, 0);
	        gbc_changeMarksD.gridx = 6;
	        gbc_changeMarksD.gridy = 2;
	        add(changeMarksD, gbc_changeMarksD);
	        GridBagConstraints gbc_Justus = new GridBagConstraints();
	        gbc_Justus.anchor = GridBagConstraints.EAST;
	        gbc_Justus.insets = new Insets(0, 0, 5, 5);
	        gbc_Justus.gridx = 5;
	        gbc_Justus.gridy = 3;
	        add(Justus, gbc_Justus);
	        changeMarksJ=new JTextField(marksJ,5);
	        GridBagConstraints gbc_changeMarksJ = new GridBagConstraints();
	        gbc_changeMarksJ.insets = new Insets(0, 0, 5, 0);
	        gbc_changeMarksJ.gridx = 6;
	        gbc_changeMarksJ.gridy = 3;
	        add(changeMarksJ, gbc_changeMarksJ);
	        GridBagConstraints gbc_Honorius = new GridBagConstraints();
	        gbc_Honorius.anchor = GridBagConstraints.EAST;
	        gbc_Honorius.insets = new Insets(0, 0, 5, 5);
	        gbc_Honorius.gridx = 5;
	        gbc_Honorius.gridy = 4;
	        add(Honorius, gbc_Honorius);
	        changeMarksH=new JTextField(marksH,5);
	        GridBagConstraints gbc_changeMarksH = new GridBagConstraints();
	        gbc_changeMarksH.insets = new Insets(0, 0, 5, 0);
	        gbc_changeMarksH.gridx = 6;
	        gbc_changeMarksH.gridy = 4;
	        add(changeMarksH, gbc_changeMarksH);
	        GridBagConstraints gbc_Laurentius = new GridBagConstraints();
	        gbc_Laurentius.anchor = GridBagConstraints.EAST;
	        gbc_Laurentius.insets = new Insets(0, 0, 5, 5);
	        gbc_Laurentius.gridx = 5;
	        gbc_Laurentius.gridy = 5;
	        add(Laurentius, gbc_Laurentius);
	        changeMarksL=new JTextField(marksL,5);
	        GridBagConstraints gbc_changeMarksL = new GridBagConstraints();
	        gbc_changeMarksL.insets = new Insets(0, 0, 5, 0);
	        gbc_changeMarksL.gridx = 6;
	        gbc_changeMarksL.gridy = 5;
	        add(changeMarksL, gbc_changeMarksL);
	        GridBagConstraints gbc_Mellitus = new GridBagConstraints();
	        gbc_Mellitus.anchor = GridBagConstraints.EAST;
	        gbc_Mellitus.insets = new Insets(0, 0, 5, 5);
	        gbc_Mellitus.gridx = 5;
	        gbc_Mellitus.gridy = 6;
	        add(Mellitus, gbc_Mellitus);
	        //ans.add(A);
	        
	        
	        
	        checkAns=new JButton("Check Answer");
	        checkAns.addActionListener(new ActionListener() {
	        	public void actionPerformed(ActionEvent arg0) {
	        	}
	        });
	        changeMarksM=new JTextField(marksM,5);
	        GridBagConstraints gbc_changeMarksM = new GridBagConstraints();
	        gbc_changeMarksM.insets = new Insets(0, 0, 5, 0);
	        gbc_changeMarksM.gridx = 6;
	        gbc_changeMarksM.gridy = 6;
	        add(changeMarksM, gbc_changeMarksM);
	        GridBagConstraints gbc_checkAns = new GridBagConstraints();
	        gbc_checkAns.anchor = GridBagConstraints.WEST;
	        gbc_checkAns.insets = new Insets(0, 0, 5, 5);
	        gbc_checkAns.gridx = 1;
	        gbc_checkAns.gridy = 7;
	        add(checkAns, gbc_checkAns);
	        
	        updateMarksA=new JButton("Update");
	        GridBagConstraints gbc_updateMarksA = new GridBagConstraints();
	        gbc_updateMarksA.insets = new Insets(0, 0, 5, 0);
	        gbc_updateMarksA.gridx = 6;
	        gbc_updateMarksA.gridy = 7;
	        add(updateMarksA, gbc_updateMarksA);
	        updateMarksA.addActionListener(this);
	        
	        next=new JButton("Next Question");
	        GridBagConstraints gbc_next = new GridBagConstraints();
	        gbc_next.anchor = GridBagConstraints.WEST;
	        gbc_next.insets = new Insets(0, 0, 5, 5);
	        gbc_next.gridx = 1;
	        gbc_next.gridy = 9;
	        add(next, gbc_next);
	        next.addActionListener(this);
	        prev=new JButton("Previous Question");
	        GridBagConstraints gbc_prev = new GridBagConstraints();
	        gbc_prev.anchor = GridBagConstraints.WEST;
	        gbc_prev.insets = new Insets(0, 0, 5, 5);
	        gbc_prev.gridx = 2;
	        gbc_prev.gridy = 9;
	        add(prev, gbc_prev);
	        prev.addActionListener(this);
	        addMarks=new JButton("Add Marks to current house");
	        //addMarks.setAlignmentX(Component.CENTER_ALIGNMENT);
	        GridBagConstraints gbc_addMarks = new GridBagConstraints();
	        gbc_addMarks.anchor = GridBagConstraints.NORTHWEST;
	        gbc_addMarks.insets = new Insets(0, 0, 5, 0);
	        gbc_addMarks.gridx = 6;
	        gbc_addMarks.gridy = 9;
	        add(addMarks, gbc_addMarks);
	        
	        panel = new JPanel();
	        GridBagConstraints gbc_panel = new GridBagConstraints();
	        gbc_panel.insets = new Insets(0, 0, 5, 0);
	        gbc_panel.anchor = GridBagConstraints.NORTHWEST;
	        gbc_panel.gridx = 6;
	        gbc_panel.gridy = 10;
	        add(panel, gbc_panel);
	                     
	                      backtohouse=new JButton("Back to Select House");
	                      GridBagConstraints gbc_backtohouse = new GridBagConstraints();
	                      gbc_backtohouse.anchor = GridBagConstraints.SOUTH;
	                      gbc_backtohouse.insets = new Insets(0, 0, 0, 5);
	                      gbc_backtohouse.gridx = 5;
	                      gbc_backtohouse.gridy = 17;
	                      add(backtohouse, gbc_backtohouse);
	                      
	                      backtohouse.addActionListener(this);
	                      // GridBagLayout gbl_panel = new GridBagLayout();
	                       //gbl_panel.columnWidths = new int[]{233, 93, 133, 0};
	                       //gbl_panel.rowHeights = new int[]{23, 0};
	                       //gbl_panel.columnWeights = new double[]{0.0, 0.0, 0.0, Double.MIN_VALUE};
	                       //gbl_panel.rowWeights = new double[]{0.0, Double.MIN_VALUE};
	                       //panel.setLayout(gbl_panel);
	                        backtomain=new JButton("Back to Main");
	                        GridBagConstraints gbc_backtomain = new GridBagConstraints();
	                        gbc_backtomain.anchor = GridBagConstraints.SOUTH;
	                        gbc_backtomain.gridx = 6;
	                        gbc_backtomain.gridy = 17;
	                        add(backtomain, gbc_backtomain);
	                        backtomain.addActionListener(this);
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
	
	if(e.getSource()==backtomain){
		clmain.show(window.cardstack,"WELCOME");
	}
	if(e.getSource()==backtohouse){
		clmain.show(window.cardstack,"HOUSE");
	}
	
}

class picturePanel extends JPanel{
	public void paintComponent(Graphics g){
		
	}
}
}

