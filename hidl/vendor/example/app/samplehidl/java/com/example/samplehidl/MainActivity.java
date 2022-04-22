package com.example.samplehidl;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.RemoteException;
import vendor.example.samplehidl.V1_0.ISampleHidl;

import com.jni.JavaHello;

import android.view.View;

public class MainActivity extends AppCompatActivity {
    JavaHello iii;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        iii = JavaHello.getInstance();
    }

    public void onClick(View view) {
        System.out.println(iii.hello());
    }

    public void registerCallback(View view) {
        iii.registerRemoteCallBack();
    }

    public void unregisterCallback(View view) {
        iii.unregisterRemoteCallBack();
    }
}
