package hk.edu.tsk.tskinter_housequizcontrolpanel;

import android.os.Bundle;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;


import hk.edu.tsk.tskinter_housequizcontrolpanel.QuestionSet.QuestionSets;
import hk.edu.tsk.tskinter_housequizcontrolpanel.dummy.DummyContent;

/**
 * A fragment representing a single Question Set detail screen.
 * This fragment is either contained in a {@link QuestionSetListActivity}
 * in two-pane mode (on tablets) or a {@link QuestionSetDetailActivity}
 * on handsets.
 */
public class QuestionSetDetailFragment extends Fragment {
    /**
     * The fragment argument representing the item ID that this fragment
     * represents.
     */
    public static final String ARG_SET_ID = "set_id";

    /**
     * The dummy content this fragment is presenting.
     */
    private DummyContent.DummyItem mItem;
    private QuestionSets.QuestionSet qSets;

    /**
     * Mandatory empty constructor for the fragment manager to instantiate the
     * fragment (e.g. upon screen orientation changes).
     */
    public QuestionSetDetailFragment() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (getArguments().containsKey(ARG_SET_ID)) {
            // Load the dummy content specified by the fragment
            // arguments. In a real-world scenario, use a Loader
            // to load content from a content provider.
            qSets = QuestionSets.SETSMAP.get(getArguments().getString(ARG_SET_ID));
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_questionset_detail, container, false);

        // Show the dummy content as text in a TextView.
        if (mItem != null) {
            ((TextView) rootView.findViewById(R.id.questionset_detail)).setText(mItem.content);
        }

        return rootView;
    }
}
