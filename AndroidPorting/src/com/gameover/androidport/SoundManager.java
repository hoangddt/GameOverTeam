package com.gameover.androidport;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import android.util.Log;

public class SoundManager {
	private static String TAG = "SOUND_MANAGER";
	public static String rootPath;
	
	private static ArrayList<Sound> listsound;
	
	private static boolean isMute = false;
	
	public static void setMute(boolean mute) {
		Log.i(TAG, "Java setMute is called: " + mute);
		isMute = mute;
		if (isMute)
		{
			stopAll();
		}
	}
	
	public static void init(String path) {
		rootPath = path + "/";
		Log.i(TAG, "root Path is : " + rootPath);
		File folder = new File(rootPath);
		File[] files = folder.listFiles();
		if (files != null) {
			listsound = new ArrayList<Sound>();
			
			for (File file : files) {
				if (file.isFile()) {
					try {
						listsound.add(new Sound(rootPath ,file.getName()));
					} catch (IOException e) {
						Log.e(TAG, "File not found !");
						e.printStackTrace();
					}
				}
			}
		} else {
			Log.e(TAG, "Files are null !");
		}
		
	}
	
	public static void play(String filename, boolean isLoop) {
		if (listsound!=null && !isMute) {
			Log.i(TAG, "play() is called: "+filename);
			for (Sound sound : listsound) {
				if (filename.equalsIgnoreCase(sound.getFilename())) {
					sound.play(isLoop);
				}
			}
		} else {
			if (listsound == null) Log.e(TAG, "play() is called: listsound is null");
		}
	}
	
	public static void stop(String filename) {
		if (listsound != null) {
			Log.i(TAG, "stop() is called: " + filename);
			for (Sound sound : listsound) {
				if (filename.equalsIgnoreCase(sound.getFilename())) {
					sound.stop();
				}
			}
		} else {
			Log.e(TAG, "stop() is called: listsound is null");
		}
	}
	
	public static void release() {
		if (listsound != null) {
			Log.i(TAG, "release() is called");
			for (Sound sound : listsound) {
				sound.release();
			}
		} else {
			Log.e(TAG, "release() is called: listsound is null");
		}
	}

	public static void stopAll() {
		if (listsound != null) {
			Log.i(TAG, "stopAll() is called");
			for (Sound sound : listsound) {
				sound.stop();
			}
		} else {
			Log.e(TAG, "stopAll() is called: listsound is null");
		}
	}
}