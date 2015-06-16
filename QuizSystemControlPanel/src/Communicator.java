import java.net.*;
import java.util.ArrayList;
import java.io.*;

import javax.swing.*;


public class Communicator extends Thread
{
    private Socket s;
    private FileInputStream fin;
    private InputStreamReader is;
    private static int PORT=9000;
    private static String ADDR="192.168.0.101";
    private PrintWriter pw;
    private int port;
    private String addr;
    private MainWindow window;

    public Communicator( MainWindow window, String ip, int port )
    {
        addr=ip;
        this.port=port;

        this.window=window;

    }
    public void setUpNetworking()
    {
        try
        {
            s=new Socket( addr,port );
            //is=new InputStreamReader( s.getInputStream() );
            pw=new PrintWriter( s.getOutputStream() );

            System.out.println( "Connected" );


        }
        catch ( IOException e )
        {
            // TODO Auto-generated catch block
            //e.printStackTrace();
            System.out.println( "setUpNetworking() IOException" );
            s = null;
            pw = null;
        }
    }
    public void run()
    {
        setUpNetworking();
        //new InReader(is).start();
    }
    public boolean reconnect()
    {
        setUpNetworking();
        if(s==null)
        {
            System.out.println("Reconnect Failed");
            return false;
        }
        return true;
    }
    public void write( String instruction )
    {
        if(s==null)
            if(!reconnect())
                return; //reconnect failed
        pw.println( instruction );
        System.out.println( instruction );
        pw.flush();
        if(pw.checkError())
        {
            System.out.println( "Print Writer Error, reconnecting" );
            if(!reconnect())
                return;   //reconnect failed
            pw.println( instruction );
            System.out.println( "Resending Info." );
            pw.flush();
        }
    }

    class InReader extends Thread
    {
        BufferedReader br;
        InputStreamReader is;
        public InReader( InputStreamReader is )
        {
            this.is=is;
            br=new BufferedReader( is );
        }
        public void read()
        {
            try
            {
                while( true )
                {
                    String message=br.readLine();
                    if( message=="questions" )
                    {
                        message=br.readLine();
                        ///this.QuestionParser(message);
                    }
                    if( message=="" ) {}
                    if( message=="" ) {}

                }


            }
            catch ( IOException e )
            {

                //JOptionPane.showMessageDialog(win,
                //	"Host has gone!","Connection Dropped",JOptionPane.ERROR_MESSAGE);
                System.exit( 0 );
                e.printStackTrace();
            }
        }
        public void run()
        {
            read();
        }
        /*
        public void QuestionParser(String JsonString){
        	JsonReader jsonReader = Json.createReader(new StringReader(JsonString));
        	JsonObject jsonObject = jsonReader.readObject();
        	jsonReader.close();
            JsonArray questionSetArray = jsonObject.getJsonArray("QuestionSets");

            //ArrayList<ArrayList<Question>> qSet=new  ArrayList<ArrayList<Question>>();
            for(int i=0;i<6;i++){
            	qSet.add(new ArrayList<Question>());
            }
            ArrayList<JsonObject> jqset=new ArrayList<JsonObject>();
            for(int i=0;i<6;i++){
                jqset.add(questionSetArray.getJsonObject(i));
            }

            for(int i=0;i<6;i++){
            	int total=jqset.get(i).getInt("Total");
            	JsonArray jqarray=jqset.get(i).getJsonArray("Questions");
            	for(int j=0;j<total;j++){
                	qSet.get(i).add(new Question());
                }
            	for(int j=0;i<total;j++){
                	JsonObject jo=jqarray.getJsonObject(j);
                	qSet.get(i).get(j).setID(jo.getInt("id"));
                	qSet.get(i).get(j).setQuestion(jo.getString("Question"));
        	        qSet.get(i).get(j).setMCstate(jo.getBoolean("isMC"));

        	        JsonObject option=jo.getJsonObject("option");


        	        qSet.get(i).get(j).setOption(option.getString("A"),0);
        	        qSet.get(i).get(j).setOption(option.getString("B"),1);
        	        qSet.get(i).get(j).setOption(option.getString("C"),2);
        	        qSet.get(i).get(j).setOption(option.getString("D"),3);

        	        qSet.get(i).get(j).setCorrectAns(jo.getString("CorrectAns"));
        	        qSet.get(i).get(j).setImgPath(jo.getString("imgPath"));

        	         System.out.println(qSet.get(i).get(j));
                }

            }
            int index=0;
            String sindex="qSet0";
            for(int l=0;l<6;l++){
            	qPages.add(new QuestionPages(window, wp, qSet.get(l), chooser , window.c));
            	window.cardstack.add(qPages.get(l),sindex);
            	index++;
            	sindex="qSet"+index;
            }

        }
        */
    }

}

