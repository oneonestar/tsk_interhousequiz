package hk.edu.tsk.tskinter_housequizcontrolpanel.QuestionSet;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by chichungshum on 8/9/14.
 */
public class QuestionSets {

    public static ArrayList<QuestionSet> SETS = new ArrayList<QuestionSet>();

    public static HashMap<String, QuestionSet> SETSMAP = new HashMap<String, QuestionSet>();

    public static void addItem(String name, QuestionSet set){
        SETS.add(set);
        SETSMAP.put(name,set);

    }

    public class QuestionSet{
        private String name;
        private HashMap<String, ArrayList<Question>> questions;//Hashmap of arraylist of questions, which allow subset of questions, identified by the key

        public QuestionSet(String name){
            this.name = name;
            questions = new HashMap<String, ArrayList<Question>>();
        }

        public void newQuestionSubSet(String name, ArrayList<Question> subset){
            questions.put(name, subset);
        }

        public void addMoreQuestionToSubset(String name, Question q){
            questions.get(name).add(q);
        }
    }
}
