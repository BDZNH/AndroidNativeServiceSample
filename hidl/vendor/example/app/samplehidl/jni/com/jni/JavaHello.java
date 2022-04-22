package com.jni;

public class JavaHello{
    private static JavaHello instance;
    private JavaHello(){
        
    }

    public static JavaHello getInstance() {
        if(instance == null) {
            synchronized (JavaHello.class){
                if(instance == null){
                    instance = new JavaHello();
                }
            }
        }
        return instance;
    }

    public native String hello();
    public native void registerRemoteCallBack();
    public native void unregisterRemoteCallBack();
    static {
        System.loadLibrary("samplehidl_jni");
    }

    // Native callback.
    private void nativeCallBack(int a,int b,int c){
        System.out.println("a: " + a + " b:" + b + " c:" +c);
    }
}