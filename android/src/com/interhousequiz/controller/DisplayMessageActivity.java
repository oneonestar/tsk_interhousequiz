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
import com.interhousequiz.controller.MainActivity;

public class DisplayMessageActivity extends Activity {
    private LinearLayout ll;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActionBar actionBar = getActionBar();
        actionBar.setTitle("HelloWorld"); 
        

        ScrollView sv = new ScrollView(this);
        ll = new LinearLayout(this);
        ll.setOrientation(LinearLayout.VERTICAL);
        sv.addView(ll);

        TextView tv = new TextView(this);
        Intent intent = getIntent();
        String message = intent.getStringExtra(MainActivity.EXTRA_MESSAGE);
        tv.setText(message);


        Button b = new Button(this);
        b.setText("Back A!");
        b.setOnClickListener(new View.OnClickListener() { 
            public void onClick(View v) { 
                Intent intent = new Intent();
                intent.putExtra(MainActivity.EXTRA_MESSAGE, "Back A!");
                setResult(Activity.RESULT_OK, intent);
                finish();
            }
        });
        ll.addView(b);

        Button c = new Button(this);
        c.setText("Back B!");
        c.setOnClickListener(new View.OnClickListener() { 
            public void onClick(View v) { 
                Intent intent = new Intent();
                intent.putExtra(MainActivity.EXTRA_MESSAGE, "Back B!");
                setResult(Activity.RESULT_OK, intent);
                finish();
            }
        });
        ll.addView(c);

        ll.addView(tv);
        setContentView(sv);
    }
}