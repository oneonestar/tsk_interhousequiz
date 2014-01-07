import javax.swing.*;

import java.awt.*;
import java.awt.event.*;

public class WelcomePanel extends JPanel implements ActionListener{
       JButton comp;
       JButton chal;
       JLabel welcome;
       MainWindow window;
       Font f;
       boolean isChallenge;
       public WelcomePanel(MainWindow window){
    	   
    	   f=new Font("Big",1,36);
    	   this.window=window;
    	   welcome=new JLabel("Start Screen");
    	   welcome.setFont(f);
    	   comp=new JButton("Compulsory");
    	   chal=new JButton("Challenge");
    	   comp.addActionListener(this);
    	   chal.addActionListener(this);
    	   comp.setFont(f);
    	   chal.setFont(f);
    	  // this.add(welcome,BorderLayout.NORTH);
    	   this.add(comp,BorderLayout.CENTER);
    	   this.add(chal,BorderLayout.SOUTH);
       }
       
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		RXCardLayout cl=(RXCardLayout)(window.cardstack.getLayout());
		if(e.getSource()==comp){
			cl.show(window.cardstack, "HOUSE" );
			isChallenge=false;
		}
		if(e.getSource()==chal){
			cl.show(window.cardstack, "QL2" );
			isChallenge=true;
		}
	}
       
}
