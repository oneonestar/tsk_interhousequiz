package com.interhousequiz.controller;

import android.app.Activity;
import android.app.ActionBar;
import android.os.Bundle;

import android.widget.ScrollView;
import android.widget.LinearLayout;
import android.view.View;

import android.widget.Button;
import android.widget.TextView;
import android.widget.EditText;
import android.widget.CheckBox;
import android.widget.RadioButton;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.widget.RadioGroup;
import android.content.DialogInterface;
import android.content.Intent;
import com.interhousequiz.controller.DisplayMessageActivity;

public class MainActivity extends Activity
{
	public final static String EXTRA_MESSAGE = "com.example.myfirstapp.MESSAGE";
	private LinearLayout ll;
	RadioGroup radioGroup;
    /** Called when the activity is first created. */

    protected void ShowDialog(int id) {
    	AlertDialog.Builder builder = new Builder(this);
    	builder.setMessage(Integer.toString(id));
    	builder.setTitle("Pressed!");
    	builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				dialog.dismiss();
			}
		});
		builder.create().show();
	}

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
    	ActionBar actionBar = getActionBar();
		actionBar.setTitle("HelloWorld"); 
        

        ScrollView sv = new ScrollView(this);
		ll = new LinearLayout(this);
		ll.setOrientation(LinearLayout.VERTICAL);
		sv.addView(ll);

		TextView tv = new TextView(this);
		tv.setText("Dynamic layouts");
		ll.addView(tv);


		Button c = new Button(this);
		c.setText("Intent!");
		c.setOnClickListener(new View.OnClickListener() { 
			public void onClick(View v) { 
				Intent intent = new Intent(MainActivity.this, DisplayMessageActivity.class);
				String message = Integer.toString(radioGroup.getCheckedRadioButtonId());
				intent.putExtra(EXTRA_MESSAGE, message);
				//startActivity(intent);
				startActivityForResult(intent, 1);
			}
		});
		ll.addView(c);

		Button b = new Button(this);
		b.setText("Add Button");
		ll.addView(b);

		radioGroup = new RadioGroup(this);

		RadioButton R_1 = new RadioButton(this);
		R_1.setText("1");
		radioGroup.addView(R_1);
		RadioButton R_2 = new RadioButton(this);
		R_2.setText("2");
		radioGroup.addView(R_2);
		RadioButton R_3 = new RadioButton(this);
		R_3.setText("3");
		radioGroup.addView(R_3);
		R_1.setId(1);
		R_2.setId(2);
		R_3.setId(3);

		ll.addView(radioGroup);

		b.setOnClickListener(new View.OnClickListener() { 
			public void onClick(View v) { 
				Button cb = new Button(MainActivity.this);
				cb.setText(Integer.toString(MainActivity.this.radioGroup.getCheckedRadioButtonId()));
				cb.setOnClickListener(new View.OnClickListener() { 
					public void onClick(View v) {
						MainActivity.this.ShowDialog(MainActivity.this.radioGroup.getCheckedRadioButtonId());
					}
				});
				MainActivity.this.ll.addView(cb);
			}
		});

		super.onCreate(savedInstanceState);
        setContentView(sv);
    }
     @Override  
    protected void onActivityResult(int requestCode, int resultCode, Intent data)  
    {
		String returnMsg = data.getStringExtra(MainActivity.EXTRA_MESSAGE);
		TextView r = new TextView(MainActivity.this);
		r.setText(returnMsg);
		MainActivity.this.ll.addView(r);
	}
}
