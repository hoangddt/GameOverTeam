package com.gameover.androidport;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

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
    public static native void nativeInit(Object activity);
    
}
