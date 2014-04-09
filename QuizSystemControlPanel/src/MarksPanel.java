import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.TextEvent;
import java.awt.event.TextListener;

import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.util.ArrayList;


public class MarksPanel extends JPanel implements ActionListener{
	   
	   private static ArrayList<String> MARKS;
	  
	   private static ArrayList<Color> colors;
	   
	   private ArrayList<JTextField> changeMarks;
	   
	   private Communicator c;
	   
	   private ArrayList<JLabel> houseName; 
	   
	   private JButton update;
	   private JButton startB;
	   private JButton stopB;
	   private JButton back;
	   private Font f;
	   private Font f2;
	   private ButtonGroup group;
	   private ArrayList<JPanel> panels;
	   
	   private ArrayList<JRadioButton> current;
	   
	   public MarksPanel(Communicator c){
		   group=new ButtonGroup();
		   current=new ArrayList<JRadioButton>();
		   
		   f=new Font("Big",1,36);
		   this.c=c;
		   
		   panels=new ArrayList<JPanel>();
		   for(int i=0;i<7;i++){
			   panels.add(new JPanel());
		   }
		   MARKS=new ArrayList<String>();
		   changeMarks=new ArrayList<JTextField>();
		   for(int i=0;i<6;i++){
			   current.add(new JRadioButton());
			   group.add(current.get(i));
			   MARKS.add("500");
			   changeMarks.add(new JTextField(MARKS.get(i),3));
		   }
		   
		   houseName=new ArrayList<JLabel>();
		   houseName.add(new JLabel("Augustin"));
		   houseName.add(new JLabel("Deusdedit"));
		   houseName.add(new JLabel("Justus"));
		   houseName.add(new JLabel("Honorius"));
		   houseName.add(new JLabel("Laurentius"));
		   houseName.add(new JLabel("Mellitus"));
		   
		   colors=new ArrayList<Color>();
		   colors.add(Color.red);
		   colors.add(Color.yellow);
		   colors.add(Color.green);
		   colors.add(new Color(238,173,14));
		   colors.add(new Color(153,50,204));
		   colors.add(Color.blue);
		   
		   f2=new Font("Medium",1,24);
		   
		   for(int i=0;i<6;i++){
			   panels.get(i).add(current.get(i));
			   panels.get(i).add(houseName.get(i));
			   panels.get(i).add(changeMarks.get(i));
			   changeMarks.get(i).setBackground(colors.get(i));
			   changeMarks.get(i).setFont(f);
			   houseName.get(i).setFont(f2);
		   }
		  
		
		  
		   
		   update=new JButton("Update");
		   startB=new JButton("Start Buzzer");
		   stopB=new JButton("Stop Buzzer");
		   panels.get(6).add(update);
		   panels.get(6).add(startB);
		   panels.get(6).add(stopB);
		   
		   for(JPanel p:panels){
			   p.setAlignmentX(LEFT_ALIGNMENT);
		   }
		   update.addActionListener(this);
		   
		   
		  
		   this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		   for(int i=0;i<7;i++){
			   this.add(panels.get(i));
		   }

		   
	   }
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		for(int i=0;i<6;i++){
		MARKS.set(i,changeMarks.get(i).getText());
		}
		try {
		c.write("Score Update A "+MARKS.get(0));
			Thread.sleep(5);
		c.write("Score Update D "+MARKS.get(1));
			Thread.sleep(5);
		c.write("Score Update J "+MARKS.get(2));
			Thread.sleep(5);
		c.write("Score Update H "+MARKS.get(3));
			Thread.sleep(5);
		c.write("Score Update L "+MARKS.get(4));
			Thread.sleep(5);
		c.write("Score Update M "+MARKS.get(5));
			Thread.sleep(5);
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}

	

	
}
