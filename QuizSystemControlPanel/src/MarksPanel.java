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


public class MarksPanel extends JPanel implements ActionListener
{

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

    private JButton add;
    private JButton minus;
    private JButton resetB;


    private JButton ui_Hide;
    private JButton ui_Show;
    public MarksPanel( Communicator c )
    {
        add=new JButton( "+" );
        minus=new JButton( "-" );
        this.setBackground( Color.gray );
        group=new ButtonGroup();
        current=new ArrayList<JRadioButton>();

        f=new Font( "Big",1,36 );
        this.c=c;

        panels=new ArrayList<JPanel>();
        for( int i=0; i<9; i++ )
        {
            panels.add( new JPanel() );
            panels.get( i ).setBackground( Color.GRAY );
        }
        MARKS=new ArrayList<String>();
        changeMarks=new ArrayList<JTextField>();
        for( int i=0; i<6; i++ )
        {
            current.add( new JRadioButton() );
            current.get( i ).setBackground( Color.gray );
            group.add( current.get( i ) );
            MARKS.add( "500" );
            changeMarks.add( new JTextField( MARKS.get( i ),3 ) );
        }

        houseName=new ArrayList<JLabel>();
        houseName.add( new JLabel( "Augustin" ) );
        houseName.add( new JLabel( "Deusdedit" ) );
        houseName.add( new JLabel( "Justus" ) );
        houseName.add( new JLabel( "Honorius" ) );
        houseName.add( new JLabel( "Laurentius" ) );
        houseName.add( new JLabel( "Mellitus" ) );

        colors=new ArrayList<Color>();
        colors.add( Color.red );
        colors.add( Color.yellow );
        colors.add( Color.green );
        colors.add( new Color( 238,173,14 ) );
        colors.add( new Color( 153,50,204 ) );
        colors.add( Color.blue );

        f2=new Font( "Medium",1,24 );

        for( int i=0; i<6; i++ )
        {
            panels.get( i ).add( current.get( i ) );
            panels.get( i ).add( houseName.get( i ) );
            panels.get( i ).add( changeMarks.get( i ) );
            changeMarks.get( i ).setBackground( colors.get( i ) );
            changeMarks.get( i ).setFont( f );
            houseName.get( i ).setFont( f2 );
        }




        update=new JButton( "Update" );
        startB=new JButton( "Start Buzzer" );
        stopB=new JButton( "Stop Buzzer" );
        add.addActionListener( this );
        minus.addActionListener( this );
        panels.get( 6 ).add( add );
        panels.get( 6 ).add( minus );
        panels.get( 6 ).add( update );
        panels.get( 7 ).add( startB );
        panels.get( 7 ).add( stopB );

        for( JPanel p:panels )
        {
            p.setAlignmentX( LEFT_ALIGNMENT );
        }
        update.addActionListener( this );

        stopB.addActionListener( this );
        startB.addActionListener( this );
        resetB=new JButton( "Reset Buzzer" );
        resetB.addActionListener( this );
        panels.get( 7 ).add( resetB );
        this.setLayout( new BoxLayout( this, BoxLayout.Y_AXIS ) );


        ui_Hide = new JButton("UI Hide");
        ui_Hide.addActionListener(this);
        ui_Show = new JButton("UI Show");
        ui_Show.addActionListener(this);
        panels.get( 8 ).add( ui_Hide );
        panels.get( 8 ).add( ui_Show );

        for( int i=0; i<9; i++ )
        {
            this.add( panels.get( i ) );
        }
    }
    @Override
    public void actionPerformed( ActionEvent e )
    {
        // TODO Auto-generated method stub
        if( e.getSource()==ui_Hide )
        {
            c.write( "UI Hide" );
            return;
        }
        else if( e.getSource()==ui_Show )
        {
            c.write( "UI Show" );
            return;
        }
        else if( e.getSource()==stopB )
        {
            c.write( "Buzzer 2" );
            return;
        }
        else if( e.getSource()==startB )
        {
            c.write( "Buzzer 1" );
            return;
        }
        else if( e.getSource()==resetB )
        {
            c.write( "Buzzer 0" );
            return;
        }
        else if( e.getSource()==add )
        {
            System.out.println( "add" );
            for( JRadioButton rb : current )
            {
                if( rb.isSelected() )
                {
                    int index=current.indexOf( rb );
                    MARKS.set( index, ""+( Integer.parseInt( MARKS.get( index ) )+10 ) );
                    changeMarks.get( index ).setText( MARKS.get( index ) );
                    switch ( index )
                    {
                    case 0:
                        c.write( "Score Add A 10" );
                        break;
                    case 1:
                        c.write( "Score Add D 10" );
                        break;
                    case 2:
                        c.write( "Score Add J 10" );
                        break;
                    case 3:
                        c.write( "Score Add H 10" );
                        break;
                    case 4:
                        c.write( "Score Add L 10" );
                        break;
                    case 5:
                        c.write( "Score Add M 10" );
                        break;
                    }

                }
            }
        }
        else if( e.getSource()==minus )
        {
            System.out.println( "minus" );
            for( JRadioButton rb : current )
            {
                if( rb.isSelected() )
                {
                    int index=current.indexOf( rb );
                    MARKS.set( index, ""+( Integer.parseInt( MARKS.get( index ) )-10 ) );
                    changeMarks.get( index ).setText( MARKS.get( index ) );
                    switch ( index )
                    {
                    case 0:
                        c.write( "Score Minus A 10" );
                        break;
                    case 1:
                        c.write( "Score Minus D 10" );
                        break;
                    case 2:
                        c.write( "Score Minus J 10" );
                        break;
                    case 3:
                        c.write( "Score Minus H 10" );
                        break;
                    case 4:
                        c.write( "Score Minus L 10" );
                        break;
                    case 5:
                        c.write( "Score Minus M 10" );
                        break;
                    }

                }
            }
        }
        else
        {
            for( int i=0; i<6; i++ )
            {
                MARKS.set( i,changeMarks.get( i ).getText() );
            }
            try
            {
                c.write( "Score Update A "+MARKS.get( 0 ) );
                Thread.sleep( 5 );
                c.write( "Score Update D "+MARKS.get( 1 ) );
                Thread.sleep( 5 );
                c.write( "Score Update J "+MARKS.get( 2 ) );
                Thread.sleep( 5 );
                c.write( "Score Update H "+MARKS.get( 3 ) );
                Thread.sleep( 5 );
                c.write( "Score Update L "+MARKS.get( 4 ) );
                Thread.sleep( 5 );
                c.write( "Score Update M "+MARKS.get( 5 ) );
                Thread.sleep( 5 );
            }
            catch ( InterruptedException e1 )
            {
                // TODO Auto-generated catch block
                e1.printStackTrace();
            }
        }
        /*
        System.out.println("Score Update A "+MARKS.get(0));
        System.out.println("Score Update A "+MARKS.get(1));
        System.out.println("Score Update A "+MARKS.get(2));
        System.out.println("Score Update A "+MARKS.get(3));
        System.out.println("Score Update A "+MARKS.get(4));
        System.out.println("Score Update A "+MARKS.get(5));
        */
    }




}
