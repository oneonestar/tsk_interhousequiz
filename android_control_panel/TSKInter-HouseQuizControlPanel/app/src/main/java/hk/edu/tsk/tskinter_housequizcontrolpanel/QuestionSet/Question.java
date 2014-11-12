package hk.edu.tsk.tskinter_housequizcontrolpanel.QuestionSet;

/**
 * Created by chichungshum on 8/9/14.
 */
public class Question {
    private String title;
    private String answer;
    private boolean isMultipleChoice;
    private String [] choices;

    public Question(){
        isMultipleChoice = false;
    }

    public void setTitle(String title){
        this.title = title;
    }

    public void setAnswer(String answer){
        this.answer = answer;
    }

    public void setMultipleChoice(String [] choices){//i 0 1 2 3 >> A B C D
        this.choices = choices;
        isMultipleChoice = true;
    }

    public String getAnswer(){
        return answer;
    }

    public boolean isMultipleChoice(){
        return isMultipleChoice;
    }

    public String [] getChoices(){
        return choices;
    }

    @Override
    public String toString() {
        return title;
    }
}
