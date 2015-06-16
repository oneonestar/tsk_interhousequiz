import javax.swing.*;

import java.awt.*;
import java.awt.event.*;

public class WelcomePanel extends JPanel implements ActionListener
{
    JButton comp;
    JButton chal;
    JButton receive;
    JLabel welcome;
    MainWindow window;
    Font f;
    boolean isChallenge;
    Communicator c;
    private JSeparator separator;
    boolean received;

    public WelcomePanel( MainWindow window, Communicator c )
    {
        this.c=c;
        f=new Font( "Big",1,36 );
        this.window=window;
        GridBagLayout gridBagLayout = new GridBagLayout();
        gridBagLayout.columnWidths = new int[] {61, 92, 116, 45, 72, 0, 0, 0, 0, 0, 0, 0, 47, 121, 0};
        gridBagLayout.rowHeights = new int[] {47, 55, 55, 0, 0, 0, 0, 0};
        gridBagLayout.columnWeights = new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
        gridBagLayout.rowWeights = new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
        setLayout( gridBagLayout );
        welcome=new JLabel( "Start Screen" );
        welcome.setFont( f );

        GridBagConstraints gbc_welcome = new GridBagConstraints();
        gbc_welcome.anchor = GridBagConstraints.NORTH;
        gbc_welcome.insets = new Insets( 0, 0, 5, 5 );
        gbc_welcome.gridx = 6;
        gbc_welcome.gridy = 0;
        this.add( welcome, gbc_welcome );
        receive=new JButton( "Receive Questions" );
        receive.addActionListener( this );
        GridBagConstraints gbc_receive = new GridBagConstraints();
        gbc_receive.insets = new Insets( 0, 0, 5, 5 );
        gbc_receive.gridx = 6;
        gbc_receive.gridy = 1;
        this.add( receive, gbc_receive );
        comp=new JButton( "Compulsory" );
        //comp.setEnabled(false);
        comp.addActionListener( this );
        comp.setFont( f );
        GridBagConstraints gbc_comp = new GridBagConstraints();
        gbc_comp.anchor = GridBagConstraints.NORTH;
        gbc_comp.insets = new Insets( 0, 0, 5, 5 );
        gbc_comp.gridx = 6;
        gbc_comp.gridy = 2;
        this.add( comp, gbc_comp );
        chal=new JButton( "Challenge" );
        chal.addActionListener( this );
        chal.setFont( f );
        GridBagConstraints gbc_chal = new GridBagConstraints();
        gbc_chal.anchor = GridBagConstraints.NORTH;
        gbc_chal.insets = new Insets( 0, 0, 5, 5 );
        gbc_chal.gridx = 6;
        gbc_chal.gridy = 4;
        this.add( chal, gbc_chal );

        separator = new JSeparator();
        GridBagConstraints gbc_separator = new GridBagConstraints();
        gbc_separator.insets = new Insets( 0, 0, 5, 5 );
        gbc_separator.gridx = 5;
        gbc_separator.gridy = 5;
        add( separator, gbc_separator );
    }

    @Override
    public void actionPerformed( ActionEvent e )
    {
        // TODO Auto-generated method stub
        if( e.getSource()==receive )
        {
            received=true;
            comp.setEnabled( true );
            c.write( "Questions" );

        }
        RXCardLayout cl=( RXCardLayout )( window.cardstack.getLayout() );
        if( e.getSource()==comp )
        {
            cl.show( window.cardstack, "qlp" );
            isChallenge=false;
        }
        if( e.getSource()==chal )
        {
            cl.show( window.cardstack, "CHALLENGE" );
            isChallenge=true;
        }
    }

}
