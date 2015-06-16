import java.awt.event.*;
import java.util.ArrayList;

import javax.swing.*;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.FlowLayout;
import java.awt.BorderLayout;

public class QuestionListPage extends JPanel
{
    private static ArrayList<JLabel> Sets;
    private ArrayList<JButton> togetherB;
    private ArrayList<JButton> complusoryB;
    private ArrayList<JButton> basicCompB;
    private ArrayList<JButton> AddCompB;
    private ArrayList<JButton> quickB;
    private ArrayList<JPanel> panels;
    private JPanel mainPanel;
    private Communicator c;
    private MarksPanel mp;
    private JScrollPane scrollPane;
    private ButtonListener BL;
    private JButton Answer;
    private JTabbedPane tabbedPane;
    private JPanel addPanel;
    private JTextField quickQuestion;
    private Font f2;

    public QuestionListPage( Communicator c, MarksPanel mp )
    {
        //Settings
        togetherB=new ArrayList<JButton>();
        complusoryB=new ArrayList<JButton>();
        basicCompB=new ArrayList<JButton>();
        AddCompB=new ArrayList<JButton>();
        Sets=new ArrayList<JLabel>();

        this.setBackground( Color.GRAY );
        BL=new ButtonListener();//create button listener

        this.c=c;//get instance of communicator
        this.mp=mp;//get instance of marks panel

        Answer=new JButton( "Check Answer" );
        Answer.addActionListener( BL );
        Answer.setEnabled( false );

        tabbedPane=new JTabbedPane();
        tabbedPane.setBackground( Color.gray );
        this.add( tabbedPane );

        //new JPanel as containers for buttons
        panels=new ArrayList<JPanel>();
        for( int a=0; a<20; a++ )
        {
            panels.add( new JPanel() );
            panels.get( a ).setBackground( Color.gray );
        }

        //齊答題
        //4 Questions
        //Button x 4 => Panel => tabbedPane
        for( int i=0; i<4; i++ )
            togetherB.add( new JButton( "Question "+( i+1 ) ) );
        for( JButton b : togetherB )
            panels.get( 0 ).add( b );
        tabbedPane.add( "齊答題",panels.get( 0 ) );

        //基本部分
        //7 題目集
        //7 Label, 28 Button => 7 Panels => "mainPanel" => tabbedPane
        for( int i=0; i<7; i++ )
        {
            Sets.add( new JLabel( "題目集"+( i+1 ) ) );
        }
        for( int j=0; j<28; j++ )
        {
            basicCompB.add( new JButton( "Question "+( j%4+1 ) ) );
        }
        mainPanel=new JPanel();
        mainPanel.setBackground( Color.gray );
        mainPanel.setLayout( new BoxLayout( mainPanel,BoxLayout.PAGE_AXIS ) );
        int j=0;
        for( int i=0; i<7; i++ )
        {
            for( int k=j; k<j+4; k++ )
            {
                panels.get( i+1 ).add( basicCompB.get( k ) );
                mainPanel.add( panels.get( i+1 ) );
            }
            panels.get( i+1 ).add( Sets.get( i ) );
            j+=4;
        }
        tabbedPane.add( "基本部分",mainPanel );
        
        //附加部分
        //7 題目集
        //7 Labels, 14 Buttons => 7 Panels => "addPanel"
        AddCompB.add( new JButton( "科學1 1" ) );
        AddCompB.add( new JButton( "科學1 2" ) );
        AddCompB.add( new JButton( "科學2 1" ) );
        AddCompB.add( new JButton( "科學2 2" ) );
        AddCompB.add( new JButton( "綜合人文1 1" ) );
        AddCompB.add( new JButton( "綜合人文1 2" ) );
        AddCompB.add( new JButton( "綜合人文2 1" ) );
        AddCompB.add( new JButton( "綜合人文2 2" ) );
        AddCompB.add( new JButton( "時事1 1" ) );
        AddCompB.add( new JButton( "時事1 2" ) );
        AddCompB.add( new JButton( "時事2 1" ) );
        AddCompB.add( new JButton( "時事2 2" ) );
        AddCompB.add( new JButton( "唔識抵死題(常識) 1" ) );
        AddCompB.add( new JButton( "唔識抵死題(常識) 2" ) );
        panels.get( 9 ).add( new JLabel( "科學1" ) );
        panels.get( 9 ).add( AddCompB.get( 0 ) );
        panels.get( 9 ).add( AddCompB.get( 1 ) );
        panels.get( 10 ).add( new JLabel( "科學2" ) );
        panels.get( 10 ).add( AddCompB.get( 2 ) );
        panels.get( 10 ).add( AddCompB.get( 3 ) );
        panels.get( 11 ).add( new JLabel( "綜合人文1" ) );
        panels.get( 11 ).add( AddCompB.get( 4 ) );
        panels.get( 11 ).add( AddCompB.get( 5 ) );
        panels.get( 12 ).add( new JLabel( "綜合人文2" ) );
        panels.get( 12 ).add( AddCompB.get( 6 ) );
        panels.get( 12 ).add( AddCompB.get( 7 ) );
        panels.get( 13 ).add( new JLabel( "時事1" ) );
        panels.get( 13 ).add( AddCompB.get( 8 ) );
        panels.get( 13 ).add( AddCompB.get( 9 ) );
        panels.get( 14 ).add( new JLabel( "時事2" ) );
        panels.get( 14 ).add( AddCompB.get( 10 ) );
        panels.get( 14 ).add( AddCompB.get( 11 ) );
        panels.get( 15 ).add( new JLabel( "唔識抵死題(常識)" ) );
        panels.get( 15 ).add( AddCompB.get( 12 ) );
        panels.get( 15 ).add( AddCompB.get( 13 ) );
        addPanel=new JPanel();
        addPanel.setBackground( Color.gray );
        addPanel.setLayout( new BoxLayout( addPanel, BoxLayout.Y_AXIS ) );
        for( int i=9; i<=15; i++ )
        {
            addPanel.add( panels.get( i ) );
        }
        tabbedPane.add( "附加部分", addPanel );

        //搶答題
        quickQuestion = new JTextField("0", 3);
        quickQuestion.setFont( new Font( "Big",1,36 ) );
        quickB = new ArrayList<JButton>();
        quickB.add(new JButton("-"));
        quickB.add(new JButton("+"));
        quickB.add(new JButton("Go"));
        panels.get( 16 ).add(quickQuestion);
        panels.get( 16 ).add(quickB.get(0));
        panels.get( 16 ).add(quickB.get(1));
        panels.get( 16 ).add(quickB.get(2));
        tabbedPane.add( "搶答題", panels.get( 16 ) );


        setLayout( new BoxLayout( this, BoxLayout.LINE_AXIS ) );
        this.add( tabbedPane,BorderLayout.LINE_START );
        //scrollPane.setPreferredSize(new Dimension(600,600));
        this.add( Answer,BorderLayout.PAGE_END );
        this.add( mp,BorderLayout.EAST );
        tabbedPane.setTabPlacement( SwingConstants.LEFT );
        //this.add(new MarksPanel());
        f2=new Font( "Medium",1,24 );
        tabbedPane.setFont( f2 );
        for( JPanel p : panels )
        {
            p.setAlignmentY( LEFT_ALIGNMENT );
        }
        for( JButton b : togetherB )
        {
            b.addActionListener( BL );
        }
        for( JButton b : basicCompB )
        {
            b.addActionListener( BL );
        }
        for( JButton b : AddCompB )
        {
            b.addActionListener( BL );
        }
        for( JButton b : quickB )
        {
            b.addActionListener( BL );
        }
    }


    class ButtonListener implements ActionListener
    {
        String command="Question Next x Question:";
        private void togetherCommand( int index )
        {
            //c.write("TOG "+(index+1));
            c.write( "Question Next x Question:A:1:"+( index+1 ) );
        }
        private void complusoryCommand( int index )
        {
            //index=index+1;
            if( index/4<1 )
            {
                c.write( command+"B:"+"1:"+( index+1 ) );
            }
            else if( index/4<2 )
            {
                c.write( command+"B:"+"2:"+( index-3 ) );
            }
            else if( index/4<3 )
            {
                c.write( command+"B:"+"3:"+( index-7 ) );
            }
            else if( index/4<4 )
            {
                c.write( command+"B:"+"4:"+( index-11 ) );
            }
            else if( index/4<5 )
            {
                c.write( command+"B:"+"5:"+( index-15 ) );
            }
            else if( index/4<6 )
            {
                c.write( command+"B:"+"6:"+( index-19 ) );
            }
            else if( index/4<7 )
            {
                c.write( command+"B:"+"7:"+( index-23 ) );
            }
        }
        private void AddCompCommand( int index )
        {
            //set number=index of question in the set of topic/2+1
            //e.g 0,1,2,3(index of question) correspond to Science1 q1, Science1 q2,
            //Science2 q1, Science1 q2
            //when index>=4, minus 4 or 8 to get back 0,1,2,3
            //Question index in a set = index mod 2 +1
            if( index/4<1 )
            {
                c.write( command+"C:"+( ( index )/2+1 )+":"+( ( ( index )%2 )+1 ) );
            }
            else if( index/4<2 )
            {
                c.write( command+"C:"+( ( index )/2+1 )+":"+( ( ( index )%2 )+1 ) );
            }
            else if( index/4<3 )
            {
                c.write( command+"C:"+( ( index )/2+1 )+":"+( ( ( index )%2 )+1 ) );
            }
            else
            {
                c.write( command+"C:"+"7:"+( index-11 ) );
            }
        }
        private void QuickCommand( int index )
        {
            int i = Integer.parseInt( quickQuestion.getText());
            if(i < 0 && i > 150)
                i = 0;
            if(index == 0)
                i--;
            else if(index == 1)
                i++;
            quickQuestion.setText( ""+i );
            c.write( command+"D:1:"+( i ) );
        }
        
        @Override
        public void actionPerformed( ActionEvent e )
        {
            // TODO Auto-generated method stub
            JButton b=( JButton )e.getSource();
            if( togetherB.contains( b ) )
            {
                togetherCommand( togetherB.indexOf( b ) );
                Answer.setEnabled( true );
            }
            if( basicCompB.contains( b ) )
            {
                System.out.println( basicCompB.indexOf( b ) );
                complusoryCommand( basicCompB.indexOf( b ) );
                System.out.println( "comp" );
                Answer.setEnabled( true );
            }
            if( AddCompB.contains( b ) )
            {
                AddCompCommand( AddCompB.indexOf( b ) );
                Answer.setEnabled( true );
            }
            if( quickB.contains( b ) )
            {
                QuickCommand( quickB.indexOf(b));
            }

            if( e.getSource()==Answer )
            {
                c.write( "Answer" );
                Answer.setEnabled( false );
            }

        }

    }

}
