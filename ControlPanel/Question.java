import java.io.*;
import java.util.ArrayList;

import javax.json.*;
import javax.json.stream.*;
import javax.json.stream.JsonParser.Event;

public class Question{
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
    public int getID(){
    	return id;
    }
    public String getQuestion(){
    	return question;
    }
    public boolean getMCstate(){
    	return mc;
    }
    public String getOption(int i){
    	return option[i];
    }
  
    public String getImgPath(){
    	return imgPath;
    }
    public String getCorrectAns(){
    	return correctAns;
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
    
    public void lmain(String args[]){
    	File f=new File("D:\\java and android\\QuizSystemControlPanel\\src\\Questions.json");
    	try {
			FileInputStream in=new FileInputStream(f);
			
			JsonReader jsonReader = Json.createReader(in);
			JsonObject jsonObject = jsonReader.readObject();
			jsonReader.close();
	        JsonArray questionSetArray = jsonObject.getJsonArray("QuestionSets");
	        
	        ArrayList<ArrayList<Question>> qSet=new  ArrayList<ArrayList<Question>>();
	        for(int i=0;i<6;i++){
	        qSet.add(new ArrayList<Question>());
	        }
	        ArrayList<JsonObject> jqset=new ArrayList<JsonObject>();
	        for(int i=0;i<6;i++){
		        jqset.add(questionSetArray.getJsonObject(i));
		    }
	        //int total=jqset.get(0).getInt("Total");
	        //System.out.println(total);
	        
	        for(int i=0;i<6;i++){
	        	int total=jqset.get(i).getInt("Total");
	        	JsonArray jqarray=jqset.get(i).getJsonArray("Questions");
	        	for(int k=0;k<total;k++){
		        	qSet.get(i).add(new Question());
		        }
	        	for(int k=0;k<total;k++){
	  	        	JsonObject jo=jqarray.getJsonObject(k);
	  	        	qSet.get(i).get(k).setID(jo.getInt("id"));
	  	        	qSet.get(i).get(k).setQuestion(jo.getString("Question"));
	  		        qSet.get(i).get(k).setMCstate(jo.getBoolean("isMC"));
	  		        
	  		        JsonObject option=jo.getJsonObject("option");
	  		        
	  		      
	  		        qSet.get(i).get(k).setOption(option.getString("A"),0);
	  		        qSet.get(i).get(k).setOption(option.getString("B"),1);
	  		        qSet.get(i).get(k).setOption(option.getString("C"),2);
	  		        qSet.get(i).get(k).setOption(option.getString("D"),3);
	  		        
	  		        qSet.get(i).get(k).setCorrectAns(jo.getString("CorrectAns"));
	  		        qSet.get(i).get(k).setImgPath(jo.getString("imgPath"));
	  		        
	  		         System.out.println(qSet.get(i).get(k));
	  	        }
	        }
	        
	        
	        /*
	        int index=0;
	        for(;index<2;index++){
	        
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
	        */
	        
	        
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
}
