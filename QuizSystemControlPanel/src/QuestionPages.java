import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

public class QuestionPages extends JPanel implements ActionListener {
	
		   JLabel question;
		   boolean mc;
		   JRadioButton A;
		   JRadioButton B;
		   JRadioButton C;
		   JRadioButton D;
		   String currentHouseName;
		   JLabel currentHouse;
		   JLabel currentSet;
		   String index;
		   MainWindow window;
		   WelcomePanel welcome;
		   ButtonGroup ans;
		   JButton backtohouse;
		   JButton backtomain;
		   RXCardLayout cl;
		   RXCardLayout clmain;
		   JPanel qcard;
		   String questionstatement;
		   String [] option;
		   String correctAns;
		   ArrayList<QuestionPanel> qPanel;
		   MarksPanel mp;
		   SetChooser sc;
		   JPanel nextprev;
		   JButton next;
		   JButton prev;
		   JButton chooserhouse;
		   JButton backmain;
		   Communicator c;
		public QuestionPages(MainWindow window,WelcomePanel welcome, ArrayList<Question> qList, SetChooser sc, Communicator c){
			this.welcome=welcome;
			this.window=window;
			this.c=c;
			this.sc=sc;
			
			mp=new MarksPanel(c);
			this.add(mp);
			
			clmain=(RXCardLayout)(window.cardstack.getLayout());
			qPanel=new ArrayList<QuestionPanel>();
			for(Question q : qList){
				qPanel.add(new QuestionPanel(q));
			}
			String index="1";
			int intindex=1;
			for(QuestionPanel qp : qPanel){
				qcard.add(qp,index);
				intindex++;
				index=""+intindex;
			}
			
			GridBagLayout gridBagLayout = new GridBagLayout();
			gridBagLayout.columnWidths = new int[]{225, 225, 292, 0};
			gridBagLayout.rowHeights = new int[]{0, 0, 75, 278, 0, 300, 0};
			gridBagLayout.columnWeights = new double[]{0.0, 0.0, 0.0, Double.MIN_VALUE};
			gridBagLayout.rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
			setLayout(gridBagLayout);
			currentHouse=sc.currenthouse;
			currentSet=sc.currentSet;
			currentHouseName=sc.currenthouseName;
			//currentHouse=new JLabel();
			//currentHouse.setText("AAAA");
			GridBagConstraints gbc_currentHouse = new GridBagConstraints();
			gbc_currentHouse.insets = new Insets(0, 0, 5, 5);
			gbc_currentHouse.gridx = 0;
			gbc_currentHouse.gridy = 2;
			this.add(currentHouse, gbc_currentHouse);
			//currentSet=new JLabel();
			//currentSet.setText("Set 1");
			GridBagConstraints gbc_currentSet = new GridBagConstraints();
			gbc_currentSet.insets = new Insets(0, 0, 5, 5);
			gbc_currentSet.gridx = 1;
			gbc_currentSet.gridy = 2;
			this.add(currentSet, gbc_currentSet);
			next=new JButton("Next Question");
			next.addActionListener(this);
			qcard=new JPanel(new RXCardLayout());
			
			
			GridBagConstraints gbc_qcard = new GridBagConstraints();
			gbc_qcard.gridwidth = 2;
			gbc_qcard.insets = new Insets(0, 0, 5, 5);
			gbc_qcard.anchor = GridBagConstraints.WEST;
			gbc_qcard.gridy = 3;
			gbc_qcard.gridx = 0;
			this.add(qcard, gbc_qcard);
			GridBagConstraints gbc_next = new GridBagConstraints();
			gbc_next.fill = GridBagConstraints.BOTH;
			gbc_next.insets = new Insets(0, 0, 5, 5);
			gbc_next.gridx = 0;
			gbc_next.gridy = 4;
			this.add(next, gbc_next);
			prev=new JButton("Previous Question");
			prev.addActionListener(this);
			GridBagConstraints gbc_prev = new GridBagConstraints();
			gbc_prev.insets = new Insets(0, 0, 5, 5);
			gbc_prev.fill = GridBagConstraints.BOTH;
			gbc_prev.gridx = 1;
			gbc_prev.gridy = 4;
			this.add(prev, gbc_prev);
			
		} 

		@Override
		public void actionPerformed(ActionEvent e) {
			// TODO Auto-generated method stu
			RXCardLayout qcardLayout=(RXCardLayout)qcard.getLayout();
			if(e.getSource()==next){
				if(qcardLayout.isNextCardAvailable()){
					qcardLayout.next(qcard);
				}else{
					JOptionPane.showMessageDialog(window.mainframe, 
							"Arrived to last Question", "Set Completed",JOptionPane.PLAIN_MESSAGE);
					clmain.show(window.cardstack, "HOUSE");
			     }
			}
			if(e.getSource()==prev){
				if(qcardLayout.isPreviousCardAvailable())
					qcardLayout.previous(qcard);
					else
					JOptionPane.showMessageDialog(window.mainframe, "This the first question");
			
			}
		
		}
		class QuestionPanel extends JPanel implements ActionListener{
				String question;
				String [] option;
				String imgPath;
				int id;
				boolean mcState;
				String correctAns;
				JLabel questionLabel;
				JLabel correctAnsLabel;
				JRadioButton optionA;
				JRadioButton optionB;
				JRadioButton optionC;
				JRadioButton optionD;
				ButtonGroup ans;
				JButton correct;
				public QuestionPanel(Question q){
					correctAns=q.getCorrectAns();
					question=q.getQuestion();
					id=q.getID();
					mcState=q.getMCstate();
					imgPath=q.getImgPath();
					questionLabel=new JLabel(id+question);
					if(mcState){
					for(int i=0;i<4;i++){
						option[i]=q.getOption(i);
					}
					ans=new ButtonGroup();
					optionA=new JRadioButton(option[0]);
					optionB=new JRadioButton(option[1]);
					optionC=new JRadioButton(option[2]);
					optionD=new JRadioButton(option[3]);
					ans.add(optionA);
					ans.add(optionB);
					ans.add(optionC);
					ans.add(optionD);
					}
					correctAnsLabel=new JLabel(correctAns);
					correct=new JButton("Correct Answer; Add Marks");
				    this.setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
				    this.add(questionLabel);
				    this.add(optionA);
				    this.add(optionB);
				    this.add(optionC);
				    this.add(optionD);
				    this.add(correctAnsLabel);
				    this.add(correct);
					correct.addActionListener(this);
				}
				@Override
				public void actionPerformed(ActionEvent e) {
					// TODO Auto-generated method stub
					if(currentHouseName=="Augustin"){
						//c.write(instruction);
					}
					if(currentHouseName=="Deusdedit"){
						//c.write(instruction);
					}
					if(currentHouseName=="Honorius"){
						//c.write(instruction);
					}
					if(currentHouseName=="Justus"){
						//c.write(instruction);
					}
					if(currentHouseName=="Laurentius"){
						//c.write(instruction);
					}
					if(currentHouseName=="Mellitus"){
						//c.write(instruction);
					}
					
				}
		}
		/*
		class MarksPanel extends JPanel implements ActionListener{
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
			   
			   JButton update;
			   public MarksPanel(){
				   changeMarksA=new JTextField(MARKSA);
				   changeMarksD=new JTextField(MARKSD);
				   changeMarksJ=new JTextField(MARKSJ);
				   changeMarksH=new JTextField(MARKSH);
				   changeMarksL=new JTextField(MARKSL);
				   changeMarksM=new JTextField(MARKSM);
				   
				   update=new JButton("Update");
				   this.setLayout(new GridLayout(6,2));
				   this.add(Augustin);
				   this.add(changeMarksA);
				   this.add(Deusdedit);
				   this.add(changeMarksD);
				   this.add(Justus);
				   this.add(changeMarksJ);
				   this.add(Honorius);
				   this.add(changeMarksH);
				   this.add(Laurentius);
				   this.add(changeMarksL);
				   this.add(Mellitus);
				   this.add(changeMarksM);
				   this.add(update);
				   update.addActionListener(this);
				   
			   }
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				c.write(instruction);
				c.write(instruction);
				c.write(instruction);
				c.write(instruction);
				c.write(instruction);
				c.write(instruction);
			}
		}
		*/
}

