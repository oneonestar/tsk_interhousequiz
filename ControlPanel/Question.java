
import java.io.*;
import java.util.ArrayList;

import javax.json.*;
import javax.json.stream.*;
import javax.json.stream.JsonParser.Event;
public class Question {
    private int id;
    private String question;
    private boolean mc;
    private String [] option;
    private String imgPath;
    private String correctAns;
    public void setID(int id){
    	this.id=id;
    }
    public void setQuestion(String question){
    	this.question=question;
    }
    public void setMCstate(boolean mc){
    	this.mc=mc;
    }
    public void setOption(String option, int index){
    	this.option[index]=option;
    }
  
    public void setImgPath(String imgPath){
    	this.imgPath=imgPath;
    }
    public void setCorrectAns(String correctAns){
    	this.correctAns=correctAns;
    }
    public Question(){
    	option=new String[4] ;
    }
    public String toString(){
    	String wquestion="";
    	wquestion+=id+": ";
    	wquestion+=question;
    	wquestion+="\n";
    	wquestion+=("A:"+option[0]+"\n");
    	wquestion+=("B:"+option[1]+"\n");
    	wquestion+=("C:"+option[2]+"\n");
    	wquestion+=("D:"+option[3]+"\n");
    	wquestion+=("correct answer:"+correctAns+"\n");
    	wquestion+=("image location:"+imgPath+"\n");
		return wquestion;
    	
    }
    
  /*  public static void main(String args[]){
    	File f=new File("D:\\java and android\\QuizSystemControlPanel\\src\\Questions.json");
    	try {
			FileInputStream in=new FileInputStream(f);
			JsonReader jsonReader = Json.createReader(in);
			JsonObject jsonObject = jsonReader.readObject();
			jsonReader.close();
	        in.close();
	        JsonArray questionArray = jsonObject.getJsonArray("Questions");
	        ArrayList<Question> qArray=new ArrayList<Question>();
	        qArray.add(new Question());
	        qArray.add(new Question());
	        
	        int index=0;
	        for(;index<2;index++){
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
    */
}
