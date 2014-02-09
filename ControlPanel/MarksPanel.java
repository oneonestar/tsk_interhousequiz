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


public class MarksPanel extends JPanel implements ActionListener, DocumentListener{
	   
	   static String MARKSA="100";
	   static String MARKSD="100";
	   static String MARKSH="100";
	   static String MARKSJ="100";
	   static String MARKSL="100";
	   static String MARKSM="100";
		
	   
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
	   public MarksPanel(Communicator c){
		   this.c=c;
		   changeMarksA=new JTextField(MARKSA);
		   changeMarksD=new JTextField(MARKSD);
		   changeMarksJ=new JTextField(MARKSJ);
		   changeMarksH=new JTextField(MARKSH);
		   changeMarksL=new JTextField(MARKSL);
		   changeMarksM=new JTextField(MARKSM);
		   
		   changeMarksA.getDocument().addDocumentListener(this);
		   changeMarksD.getDocument().addDocumentListener(this);
		   changeMarksJ.getDocument().addDocumentListener(this);
		   changeMarksH.getDocument().addDocumentListener(this);
		   changeMarksL.getDocument().addDocumentListener(this);
		   changeMarksM.getDocument().addDocumentListener(this);
		   
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
		c.write("Score Update A "+MARKSA);
		c.write("Score Update D "+MARKSD);
		c.write("Score Update J "+MARKSJ);
		c.write("Score Update H "+MARKSH);
		c.write("Score Update L "+MARKSL);
		c.write("Score Update M "+MARKSM);
	}

	
	@Override
	public void changedUpdate(DocumentEvent de) {
		if(de.getDocument()==changeMarksA){
			MARKSA=changeMarksA.getText();
		}
		if(de.getDocument()==changeMarksD){
			MARKSD=changeMarksD.getText();
		}
		if(de.getDocument()==changeMarksJ){
			MARKSJ=changeMarksJ.getText();
		}
		if(de.getDocument()==changeMarksH){
			MARKSH=changeMarksH.getText();
		}
		if(de.getDocument()==changeMarksL){
			MARKSL=changeMarksL.getText();
		}
		if(de.getDocument()==changeMarksM){
			MARKSM=changeMarksM.getText();
		}
		
	}
	@Override
	public void insertUpdate(DocumentEvent arg0){}
	@Override
	public void removeUpdate(DocumentEvent arg0){}
}
