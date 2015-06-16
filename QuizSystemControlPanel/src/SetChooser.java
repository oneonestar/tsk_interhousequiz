import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class SetChooser extends JPanel implements ActionListener
{
    JButton set1;
    JButton set2;
    JButton set3;
    JButton set4;
    JButton set5;
    JButton set6;
    JButton back;
    JLabel currenthouse;
    JLabel currentSet;
    String currenthouseName;
    String currentSetNo;
    JLabel empty;
    boolean set1used=false;
    boolean set2used=false;
    boolean set3used=false;
    boolean set4used=false;
    boolean set5used=false;
    boolean set6used=false;
    MainWindow window;
    HouseChooser houseList;
    Font f;

    public void setCurrentHouse( String HouseName )
    {
        currenthouseName=HouseName;
        currenthouse.setText( HouseName );
    }
    public SetChooser( MainWindow window )
    {
        this.window=window;
        f=new Font( "Big",1,36 );
        empty=new JLabel();
        this.setLayout( new GridLayout( 3,2 ) );
        set1=new JButton( "Set 1" );
        set2=new JButton( "Set 2" );
        set3=new JButton( "Set 3" );
        set4=new JButton( "Set 4" );
        set5=new JButton( "Set 5" );
        set6=new JButton( "Set 6" );
        back=new JButton( "back" );
        currenthouse=new JLabel();
        currenthouse.setHorizontalAlignment( SwingConstants.CENTER );
        currentSet=new JLabel();
        //this.add(empty);
        set1.setFont( f );
        set2.setFont( f );
        set3.setFont( f );
        set4.setFont( f );
        set5.setFont( f );
        set6.setFont( f );
        back.setFont( f );
        currenthouse.setFont( f );
        //this.add(empty);
        this.add( set1 );
        this.add( set2 );
        this.add( set3 );
        this.add( set4 );
        this.add( set5 );
        this.add( set6 );
        this.add( currenthouse );
        this.add( back );

        set1.addActionListener( this );
        set2.addActionListener( this );
        set3.addActionListener( this );
        set4.addActionListener( this );
        set5.addActionListener( this );
        set6.addActionListener( this );
        back.addActionListener( this );
    }
    @Override
    public void actionPerformed( ActionEvent e )
    {
        CardLayout cl=( CardLayout )( window.cardstack.getLayout() );
        if( e.getSource()==back )
        {
            cl.show( window.cardstack, "HOUSE" );
        }
        if( e.getSource()==set1 )
        {
            cl.show( window.cardstack, "qSet0" );
            set1.setEnabled( false );
            currentSetNo="Set 1";
            currentSet.setText( currentSetNo );
        }
        if( e.getSource()==set2 )
        {
            cl.show( window.cardstack, "qSet1" );
            set2.setEnabled( false );
            currentSetNo="Set 2";
            currentSet.setText( currentSetNo );
        }
        if( e.getSource()==set3 )
        {
            cl.show( window.cardstack, "qSet2" );
            set3.setEnabled( false );
            currentSetNo="Set 3";
            currentSet.setText( currentSetNo );
        }
        if( e.getSource()==set4 )
        {
            cl.show( window.cardstack, "qSet3" );
            set4.setEnabled( false );
            currentSetNo="Set 4";
            currentSet.setText( currentSetNo );
        }
        if( e.getSource()==set5 )
        {
            cl.show( window.cardstack, "qSet4" );
            set5.setEnabled( false );
            currentSetNo="Set 5";
            currentSet.setText( currentSetNo );
        }
        if( e.getSource()==set6 )
        {
            cl.show( window.cardstack, "qSet5" );
            set6.setEnabled( false );
            currentSetNo="Set 6";
            currentSet.setText( currentSetNo );
        }


    }
}
