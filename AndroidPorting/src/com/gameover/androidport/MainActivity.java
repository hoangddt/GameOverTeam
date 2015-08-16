package com.gameover.androidport;

import java.io.IOException;

import android.app.Activity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

/**
 * The Application flow:
 * 
 * 1. Load C++ library:
 *   
 *   - When Android execute this Activity, the static block of this class
 *     will be called first, and JNI_OnLoad function in C++ (SoundManager.cpp) is executed.
 *     It get `jVM` // variables reference to current java virtual machine.
 *     We use this variable to get class, method of java from C++.
 *   - After load library. onCreate() method will be executed
 *   
 * 2. Create Activity:
 *   - First, it remove title bar and make our app in full-screen mode.
 *   - Check if Data exist? If not, copy data to sdcard
 *   - Retrieve the path to data and prepare to pass that path to C++
 *   - mView object is created and is set to ContentView
 *   - Pass a reference of this Object to C++ that we can call non-static method
 *   
 * 3. Init and load data:
 *   - When onSurfaceChanged() in AndroidPortView Trigger, it call init() function in C++
 *   - The C++ side receive screenWidth and screenHeight size
 *   - C++ itself will call an Java method to get the path to data
 *   - When get the path, it set to FileManager class and call SoundManager in Java to init too
 * 
 * 4. Render:
 *   - After 3. C++ code have enough information about data,... to bootstrap
 *   - When onDrawFrame() method of mView trigger it call draw in C++
 *     and now the C++ do the rest of the Game
 *   
 * @author hoangddt
 *
 */


public class MainActivity extends Activity 
{
	static {
	      System.loadLibrary("AndroidPort");
	}

	
	AndroidPortView mView;
	private static String TAG = "MY_MainActivity";
	private static String TAG_ON = "MY_ON_METHOD";
	
	private boolean doubleBackToExitPressedOnce;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		Log.i(TAG_ON, "onCreate() method");
		
		// remove title bar
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		// remove notification bar
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		
		Log.i(TAG, "getInstance of CopyAssetToSDcard");
		AssetManager as = getAssets();
		
		// init method itself automatic handle if file is exist or not
		// if not, it will unpack data to sdcard
		CopyAssetToSDcard.getInstance().init(as);
		Log.i(TAG, "The Path will pass to C++ is : " + CopyAssetToSDcard.getInstance().getAbsoluteNativePath());
		
		mView = new AndroidPortView(getApplication());
		setContentView(mView);
		
		// pass this Object to C++ to call exitApplication
		nativeInit(this);
		
	}

	@Override protected void onPause() 
	{
        super.onPause();
        Log.i(TAG_ON, "onPause() method");
        SoundManager.stopAll();
        AndroidPortLib.pause();
        mView.setPreserveEGLContextOnPause(true);
        mView.onPause();
    }

    @Override protected void onResume() 
    {
        super.onResume();
        Log.i(TAG_ON, "onResume() method");
        mView.onResume();
    }
    
    @Override protected void onStart()
    {
    	Log.i(TAG_ON, "onStart() method");
    	super.onStart();
    }
    
    @Override protected void onRestart()
    {
    	super.onRestart();
    	Log.i(TAG_ON, "onRestart() method");
    }
    
    @Override protected void onStop()
    {
    	super.onStop();
    	Log.i(TAG_ON, "onStop() method");
    }
    
    @Override protected void onDestroy()
    {
    	Log.i(TAG_ON, "onDestroy() method");
    	super.onDestroy();
    	AndroidPortLib.destroy();
    }
    
    @Override
	public void onBackPressed() {
		if (doubleBackToExitPressedOnce) {
			super.onBackPressed();
			return;
		}
		
		this.doubleBackToExitPressedOnce = true;
		
		Toast.makeText(this, "Please click BACK again to exit !", Toast.LENGTH_SHORT).show();
		
		new Handler().postDelayed(new Runnable() {
			
			@Override
			public void run() {
				doubleBackToExitPressedOnce = false;
			}
		}, 2000);
		
	}

	public void exitApplication() {
    	Log.i(TAG, "exitApplication() method");
		finish();
		System.exit(0);
	}
    
	public static String getDataPath()
	{
		String absolutePathPassToCpp = CopyAssetToSDcard.getInstance().getAbsoluteNativePath();
		return absolutePathPassToCpp;
	}
	
    @Override
    public boolean onTouchEvent(MotionEvent event) 
    {
		 //Toast.makeText(this, "X: "+event.getX()+ " \nY: "+event.getY(), Toast.LENGTH_SHORT).show();
		 String action = "";
		 
		 // default is MotionEvent.ACTION_CANCEL
		 int MotionAction = MotionEvent.ACTION_CANCEL;
		 int xCoord = (int) event.getX();
		 int yCoord = (int) event.getY();
		 
		 switch (event.getAction())
		 {
		      case MotionEvent.ACTION_DOWN:
				   action = "ACTION_DOWN";
				   MotionAction = MotionEvent.ACTION_DOWN;
			   break;
			   
		      case MotionEvent.ACTION_UP:
			       action = "ACTION_UP";
			       MotionAction = MotionEvent.ACTION_UP;
			       break;
			       
			  case MotionEvent.ACTION_MOVE:
			       action = "ACTION_MOVE";
			       MotionAction = MotionEvent.ACTION_MOVE;
			       break;
			   
			  default:
			       action = "ACTION_NONE";
			       MotionAction = MotionEvent.ACTION_CANCEL;
			       break;
		 
		 }
		 AndroidPortLib.onTouch(MotionAction, xCoord, yCoord);
		 Log.i(TAG, action +"   X: "+ xCoord + " Y: "+ yCoord);
		 return super.onTouchEvent(event);
    }
    public static native void nativeInit(Object activity); // use in main.cpp
    
}
