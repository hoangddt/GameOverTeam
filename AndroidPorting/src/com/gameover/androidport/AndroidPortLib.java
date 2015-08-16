package com.gameover.androidport;

//Wrapper for native library

public class AndroidPortLib {

  static {
      System.loadLibrary("AndroidPort");
  }

 /**
  * @param width the current view width
  * @param height the current view height
  */
  public static native void init(int width, int height);
  public static native void step();
  public static native void destroy();
  public static native void onTouch(int TYPE, int xCoord, int yCoord);
  public static native void pause();
}
