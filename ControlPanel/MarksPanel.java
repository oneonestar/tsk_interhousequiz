import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.TextEvent;
import java.awt.event.TextListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;


public class MarksPanel extends JPanel implements ActionListener{
	   
	   static String MARKSA="1200";
	   static String MARKSD="300";
	   static String MARKSH="6000";
	   static String MARKSJ="1800";
	   static String MARKSL="10000";
	   static String MARKSM="6000";
		
	   
	   JTextField changeMarksA;
	   JTextField changeMarksD;
	   JTextField changeMarksJ;
	   JTextField changeMarksH;
	   JTextField changeMarksL;
	   JTextField changeMarksM;
	   
	   Communicator c;
	   
	   JLabel Augustin=new JLabel("Augustin");
	   JLabel Deusdedit=new JLabel("Deusdedit"); 
	   JLabel Justus=new JLabel("Justus"); 
	   JLabel Honorius=new JLabel("Honorius"); 
	   JLabel Laurentius=new JLabel("Laurentius"); 
	   JLabel Mellitus=new JLabel("Mellitus"); 
	   
	   JButton update;
	   Font f;
	   Font f2;
	   
	   public MarksPanel(Communicator c){
		   f=new Font("Big",1,36);
		   this.c=c;
		   GridBagLayout gridBagLayout = new GridBagLayout();
		   gridBagLayout.columnWidths = new int[]{150, 150, 150, 0};
		   gridBagLayout.rowHeights = new int[]{50, 50, 50, 50, 50, 43, 0, 33, 0};
		   gridBagLayout.columnWeights = new double[]{0.0, 0.0, 0.0, Double.MIN_VALUE};
		   gridBagLayout.rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		   setLayout(gridBagLayout);
		   GridBagConstraints gbc_Augustin = new GridBagConstraints();
		   gbc_Augustin.fill = GridBagConstraints.VERTICAL;
		   gbc_Augustin.insets = new Insets(0, 0, 5, 5);
		   gbc_Augustin.gridx = 0;
		   gbc_Augustin.gridy = 0;
		   this.add(Augustin, gbc_Augustin);
		   changeMarksA=new JTextField(MARKSA);
		   GridBagConstraints gbc_changeMarksA = new GridBagConstraints();
		   gbc_changeMarksA.fill = GridBagConstraints.BOTH;
		   gbc_changeMarksA.insets = new Insets(0, 0, 5, 5);
		   gbc_changeMarksA.gridx = 1;
		   gbc_changeMarksA.gridy = 0;
		   this.add(changeMarksA, gbc_changeMarksA);
		   GridBagConstraints gbc_Deusdedit = new GridBagConstraints();
		   gbc_Deusdedit.fill = GridBagConstraints.VERTICAL;
		   gbc_Deusdedit.insets = new Insets(0, 0, 5, 5);
		   gbc_Deusdedit.gridx = 0;
		   gbc_Deusdedit.gridy = 1;
		   this.add(Deusdedit, gbc_Deusdedit);
		   changeMarksD=new JTextField(MARKSD);
		   GridBagConstraints gbc_changeMarksD = new GridBagConstraints();
		   gbc_changeMarksD.fill = GridBagConstraints.BOTH;
		   gbc_changeMarksD.insets = new Insets(0, 0, 5, 5);
		   gbc_changeMarksD.gridx = 1;
		   gbc_changeMarksD.gridy = 1;
		   this.add(changeMarksD, gbc_changeMarksD);
		   GridBagConstraints gbc_Justus = new GridBagConstraints();
		   gbc_Justus.fill = GridBagConstraints.VERTICAL;
		   gbc_Justus.insets = new Insets(0, 0, 5, 5);
		   gbc_Justus.gridx = 0;
		   gbc_Justus.gridy = 2;
		   this.add(Justus, gbc_Justus);
		   changeMarksJ=new JTextField(MARKSJ);
		   GridBagConstraints gbc_changeMarksJ = new GridBagConstraints();
		   gbc_changeMarksJ.fill = GridBagConstraints.BOTH;
		   gbc_changeMarksJ.insets = new Insets(0, 0, 5, 5);
		   gbc_changeMarksJ.gridx = 1;
		   gbc_changeMarksJ.gridy = 2;
		   this.add(changeMarksJ, gbc_changeMarksJ);
		   GridBagConstraints gbc_Honorius = new GridBagConstraints();
		   gbc_Honorius.fill = GridBagConstraints.VERTICAL;
		   gbc_Honorius.insets = new Insets(0, 0, 5, 5);
		   gbc_Honorius.gridx = 0;
		   gbc_Honorius.gridy = 3;
		   this.add(Honorius, gbc_Honorius);
		   changeMarksH=new JTextField(MARKSH);
		   GridBagConstraints gbc_changeMarksH = new GridBagConstraints();
		   gbc_changeMarksH.fill = GridBagConstraints.BOTH;
		   gbc_changeMarksH.insets = new Insets(0, 0, 5, 5);
		   gbc_changeMarksH.gridx = 1;
		   gbc_changeMarksH.gridy = 3;
		   this.add(changeMarksH, gbc_changeMarksH);
		   GridBagConstraints gbc_Laurentius = new GridBagConstraints();
		   gbc_Laurentius.fill = GridBagConstraints.VERTICAL;
		   gbc_Laurentius.insets = new Insets(0, 0, 5, 5);
		   gbc_Laurentius.gridx = 0;
		   gbc_Laurentius.gridy = 4;
		   this.add(Laurentius, gbc_Laurentius);
		   changeMarksL=new JTextField(MARKSL);
		   GridBagConstraints gbc_changeMarksL = new GridBagConstraints();
		   gbc_changeMarksL.fill = GridBagConstraints.BOTH;
		   gbc_changeMarksL.insets = new Insets(0, 0, 5, 5);
		   gbc_changeMarksL.gridx = 1;
		   gbc_changeMarksL.gridy = 4;
		   this.add(changeMarksL, gbc_changeMarksL);
		   GridBagConstraints gbc_Mellitus = new GridBagConstraints();
		   gbc_Mellitus.fill = GridBagConstraints.VERTICAL;
		   gbc_Mellitus.insets = new Insets(0, 0, 5, 5);
		   gbc_Mellitus.gridx = 0;
		   gbc_Mellitus.gridy = 5;
		   this.add(Mellitus, gbc_Mellitus);
		   changeMarksM=new JTextField(MARKSM);
		   GridBagConstraints gbc_changeMarksM = new GridBagConstraints();
		   gbc_changeMarksM.fill = GridBagConstraints.BOTH;
		   gbc_changeMarksM.insets = new Insets(0, 0, 5, 5);
		   gbc_changeMarksM.gridx = 1;
		   gbc_changeMarksM.gridy = 5;
		   this.add(changeMarksM, gbc_changeMarksM);
		   
		  
		   
		   update=new JButton("Update");
		   GridBagConstraints gbc_update = new GridBagConstraints();
		   gbc_update.insets = new Insets(0, 0, 0, 5);
		   gbc_update.fill = GridBagConstraints.BOTH;
		   gbc_update.gridx = 1;
		   gbc_update.gridy = 7;
		   this.add(update, gbc_update);
		   update.addActionListener(this);
		   changeMarksA.setBackground(Color.red);
		   changeMarksD.setBackground(Color.yellow);
		   changeMarksJ.setBackground(Color.green);
		   changeMarksH.setBackground(new Color(238,173,14));
		   changeMarksL.setBackground(new Color(153,50,204));
		   changeMarksM.setBackground(Color.blue);
		   changeMarksA.setFont(f);
		   changeMarksD.setFont(f);
		   changeMarksJ.setFont(f);
		   changeMarksH.setFont(f);
		   changeMarksL.setFont(f);
		   changeMarksM.setFont(f);
		   
		   f2=new Font("Medium",1,24);
		   Augustin.setFont(f2);
		   Deusdedit.setFont(f2); 
		   Justus.setFont(f2); 
		   Honorius.setFont(f2); 
		   Laurentius.setFont(f2); 
		   Mellitus.setFont(f2);

		   
	   }
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		MARKSA=changeMarksA.getText();
		MARKSD=changeMarksD.getText();
		MARKSJ=changeMarksJ.getText();
		MARKSH=changeMarksH.getText();
		MARKSL=changeMarksL.getText();
		MARKSM=changeMarksM.getText();
		try {
		c.write("Score Update A "+MARKSA);
			Thread.sleep(5);
		c.write("Score Update D "+MARKSD);
			Thread.sleep(5);
		c.write("Score Update J "+MARKSJ);
			Thread.sleep(5);
		c.write("Score Update H "+MARKSH);
			Thread.sleep(5);
		c.write("Score Update L "+MARKSL);
			Thread.sleep(5);
		c.write("Score Update M "+MARKSM);
			Thread.sleep(5);
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}

	

	
}
