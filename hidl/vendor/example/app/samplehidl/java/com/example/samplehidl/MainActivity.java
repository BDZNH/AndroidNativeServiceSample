package com.example.samplehidl;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.RemoteException;
import vendor.example.samplehidl.V1_0.ISampleHidl;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
	try{
	ISampleHidl server = ISampleHidl.getService();
	if(server != null){
	    System.out.println("get service success");
	    String result = server.hello("msg from android app");
	    System.out.println("received service msg: " + result);
	}
	} catch (RemoteException e){
	    e.printStackTrace();
	}
    }
}
