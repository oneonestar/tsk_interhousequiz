import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonObject;
import javax.json.JsonReader;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.ArrayList;

public class MainWindow {
    JFrame mainframe;
    WelcomePanel welcome;
    JPanel cardstack;
    JPanel QuestionList;
	HouseChooser houseList;
	SetChooser setChooser;
	JPanel QuestionList2;
    public MainWindow(){
    	mainframe=new JFrame("TSK Interhouse Quiz - Admin Control Panel");
    	cardstack=new JPanel(new RXCardLayout());
    	welcome=new WelcomePanel(this);
    	setChooser=new SetChooser(this);
    	houseList=new HouseChooser(this,setChooser);
    	QuestionList=new JPanel(new RXCardLayout());
    	QuestionList2=new JPanel(new RXCardLayout());
    	
    	
    }
    public void go(){
    	mainframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    	
    	cardstack.add(welcome,"WELCOME");
    	cardstack.add(houseList,"HOUSE");
    	cardstack.add(setChooser,"SET");
    	cardstack.add(QuestionList,"QL");
    	cardstack.add(QuestionList2,"QL2");
    	QuestionList.add(new QuestionPage(this,welcome,"1+1=?"),"Q1");
    	QuestionList.add(new QuestionPage(this,welcome,"Who is the president of Legislative Council ?"),"Q2");
    	QuestionList.add(new QuestionPage(this,welcome,"How many teachers in TSK"),"Q3");
    	QuestionList.add(new QuestionPage(this,welcome,"Which of the following programming language is not object-oriented?"),"Q4");
    	QuestionList.add(new QuestionPage(this,welcome,"Solve: x^2+5x+10=0"),"Q5");
    	QuestionList.add(new QuestionPage(this,welcome,"Which bone in Human is the smallest"),"Q6");
    	QuestionList.add(new QuestionPage(this,welcome, "A+B"),"Q7");
    	QuestionList.add(new QuestionPage(this,welcome, "F+G"),"Q8");
    	QuestionList.add(new QuestionPage(this,welcome, "O+P"),"Q9");
    	
    	QuestionList2.add(new QuestionPage(this,welcome,"1+1=?"),"Q21");
    	QuestionList2.add(new QuestionPage(this,welcome,"Who is the president of Legislative Council ?"),"Q22");
    	QuestionList2.add(new QuestionPage(this,welcome,"How many teachers in TSK"),"Q23");
    	QuestionList2.add(new QuestionPage(this,welcome,"Which of the following programming language is not object-oriented?"),"Q24");
    	QuestionList2.add(new QuestionPage(this,welcome,"Solve: x^2+5x+10=0"),"Q25");
    	QuestionList2.add(new QuestionPage(this,welcome,"Which bone in Human is the smallest"),"Q26");
    	QuestionList2.add(new QuestionPage(this,welcome, "A+B"),"Q27");
    	QuestionList2.add(new QuestionPage(this,welcome, "F+G"),"Q28");
    	QuestionList2.add(new QuestionPage(this,welcome, "O+P"),"Q29");
    	
    	mainframe.add(cardstack);
    	mainframe.setSize(800,600);
    	mainframe.setVisible(true);
    }
	public static void main(String[] args) {
		// TODO Auto-generated method stub
        new MainWindow().go();
	}
	public void parseQuestions(File f){
		try {
			FileInputStream in=new FileInputStream(f);
			JsonReader jsonReader = Json.createReader(in);
			JsonObject jsonObject = jsonReader.readObject();
			jsonReader.close();
	        in.close();
	        int total=jsonObject.getInt("Total");
	        JsonArray questionArray = jsonObject.getJsonArray("Questions");
	        ArrayList<Question> qArray=new ArrayList<Question>();
	        qArray.add(new Question());
	        qArray.add(new Question());
	        
	        int index=0;
	        for(;index<total;index++){
	        	JsonObject jo=questionArray.getJsonObject(index);
	        	qArray.get(index).setID(jo.getInt("id"));
	        	qArray.get(index).setQuestion(jo.getString("Question"));
	        	qArray.get(index).setMCstate(jo.getBoolean("isMC"));
	        
	        	JsonObject option=jo.getJsonObject("option");
	        
	      
	        	qArray.get(index).setOption(option.getString("A"),0);
	        	qArray.get(index).setOption(option.getString("B"),1);
	        	qArray.get(index).setOption(option.getString("C"),2);
	        	qArray.get(index).setOption(option.getString("D"),3);
	        
	        	qArray.get(index).setCorrectAns(jo.getString("CorrectAns"));
	        	qArray.get(index).setImgPath(jo.getString("imgPath"));
	        
	        	System.out.println(qArray.get(index));
	        }
	        
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
