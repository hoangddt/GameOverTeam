package com.gameover.androidport;

import java.io.IOException;

import android.media.MediaPlayer;
import android.util.Log;


public class Sound {
	private String rootPath;
	private String filename;
	private MediaPlayer player;
	
	public static String TAG = "SOUND";
	
	public Sound(String rootPath, String filename) throws IOException{
		this.rootPath = rootPath;
		this.filename = filename;
		this.player = new MediaPlayer();
		try {
			Log.i(TAG, "init() is called !");
			
			this.player.reset();
			this.player.setDataSource(this.rootPath.concat(filename));
			this.player.prepare();
			
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (IllegalStateException e) {
			e.printStackTrace();
		}
	}
	
	public String getFilename() {
		return filename;
	}

	public void setFilename(String filename) {
		this.filename = filename;
	}

	public MediaPlayer getPlayer() {
		return player;
	}
	
	public void setRootPath(String rootPath) {
		this.rootPath = rootPath;
	}

	public void play(boolean isLoop) {
		if (this.player != null) {
			Log.i(TAG, "play() is called");
			if(this.player.isPlaying()) {
				stop();
			}
			this.player.start();
			this.player.setLooping(isLoop);
		}
	}
	
	public void stop() {
		if (this.player != null && this.player.isPlaying()) {
			this.player.pause();
			this.player.seekTo(0);
		}
	}
	
	public void  release() {
		if (this.player != null) {
			this.player.release();
		}
	}
	
}