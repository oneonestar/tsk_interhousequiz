import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class HouseChooser extends JPanel implements ActionListener{
   JButton A;
   JButton D;
   JButton H;
   JButton J;
   JButton L;
   JButton M;
   JButton back;
   JLabel empty;
   MainWindow window;
   SetChooser chooser;
   Font f;
   String ChosenHouse="";
   public HouseChooser(MainWindow window, SetChooser chooser){
	   this.window=window;
	   this.chooser=chooser;
	   f=new Font("Big",1,36);
	   empty=new JLabel();
	   
	   A=new JButton("Augustin");
	   D=new JButton("Deusdedit");
	   H=new JButton("Honorius");
	   J=new JButton("Justus");
	   L=new JButton("Laurentius");
	   M=new JButton("Mellitus");
	   A.setFont(f);
	   D.setFont(f);
	   H.setFont(f);
	   J.setFont(f);
	   L.setFont(f);
	   M.setFont(f);
	   A.setBackground(Color.RED);
	   D.setBackground(Color.YELLOW);
	   H.setBackground(new Color(238,173,14));
	   J.setBackground(Color.GREEN);
	   L.setBackground(new Color(153,50,204));
	   M.setBackground(Color.BLUE);
	   back=new JButton("Back");
	   back.setFont(f);
	   this.setLayout(new GridLayout(3,2));
	   this.add(A);
	   this.add(D);
	   this.add(H);
	   this.add(J);
	   this.add(L);
	   this.add(M);
	   this.add(empty);
	   this.add(back);
	   
	   A.addActionListener(this);
	   D.addActionListener(this);
	   H.addActionListener(this);
	   J.addActionListener(this);
	   L.addActionListener(this);
	   M.addActionListener(this);
	   back.addActionListener(this);
	   
	   
   }

@Override
public void actionPerformed(ActionEvent e) {
	CardLayout cl=(CardLayout)(window.cardstack.getLayout());
	if(e.getSource()==back){
		cl.show(window.cardstack, "WELCOME");
	}
	if(e.getSource()==A){
		cl.show(window.cardstack, "SET");
		chooser.setCurrentHouse("Augustin");
	}
	if(e.getSource()==D){
		cl.show(window.cardstack, "SET");
		chooser.setCurrentHouse("Deusdedit");
	}
	if(e.getSource()==H){
		cl.show(window.cardstack, "SET");
		chooser.setCurrentHouse("Honorius");
	}
	if(e.getSource()==J){
		cl.show(window.cardstack, "SET");
		chooser.setCurrentHouse("Justus");
	}
	if(e.getSource()==L){
		cl.show(window.cardstack, "SET");
		chooser.setCurrentHouse("Laurentius");
	}
	if(e.getSource()==M){
		cl.show(window.cardstack, "SET");
		chooser.setCurrentHouse("Mellitus");
	}
	
}
}
