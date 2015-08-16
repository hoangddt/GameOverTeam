package com.gameover.androidport;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;


public class CopyAssetToSDcard
{
	
	private static CopyAssetToSDcard mInstance = new CopyAssetToSDcard();
	
	private CopyAssetToSDcard()
	{
		Log.i(TAG, "CopyAssetToSDcard is called");
	}
	
	public static CopyAssetToSDcard getInstance()
	{
		return mInstance;
	}
	
	private AssetManager mAssetManager;
	private final static String TAG = "MY_COPY_DATA";
	private final String mRootDirectory = "Resources";
	private String mNativeRootPath = Environment.getExternalStorageDirectory().toString();;
	
	public void init(AssetManager as)
	{
		if (alreadyHaveData())
		{
			Log.i(TAG, "Data is unpack before, don't need to unpack again");
			return;
		}
		else
		{
			// unpack data to sd card
			Log.i(TAG, "Data isn't exist, we need to unpack");
			Log.i(TAG, "Enter init of CopyAssetToSDcard class");
			mAssetManager = as;
			this.copyFileOrDir(mRootDirectory);
			Log.i(TAG, "End init of CopyAssetToSDcard class");
		}
	}
	
	public boolean alreadyHaveData() {
		String theLastFile = getAbsoluteNativePath() + "/" + "zTheLastFile.txt";
		File file = new File(theLastFile);
		if (file.exists())
		{
			return true;
		}
		{
			return false;
		}
	}

	public String getAbsoluteNativePath()
	{
		String path = mNativeRootPath + "/" + mRootDirectory;
		return path;
	}
	
	private void copyFileOrDir(String path)
	{
	    String assets[] = null;
	    try
	    {
	        assets = mAssetManager.list(path);
	        if (assets.length == 0)
	        {
	            copyFile(path);
	        }
	        else
	        {
	            // folder name, if folder not exist create it
	            // String fullPath = "/data/data/" + this.getPackageName() + "/" + path;
	        	String fullPath = mNativeRootPath + "/" + path;
	            File dir = new File(fullPath);
	            if (!dir.exists())
	            {
	                dir.mkdir();
	                Log.i(TAG, "create dir: " + fullPath);
	            }
	            for (int i = 0; i < assets.length; ++i)
	            {
	                copyFileOrDir(path + "/" + assets[i]);
	            }
	        }
	    }
	    catch (IOException ex)
	    {
	        Log.e("tag", "I/O Exception", ex);
	    }
	}

	private void copyFile(String filename)
	{
		Log.i(TAG, "Copying file: " + filename);
	    InputStream in = null;
	    OutputStream out = null;
	    try 
	    {
	        in = mAssetManager.open(filename);
	        String newFileName = mNativeRootPath + "/" + filename;
	        out = new FileOutputStream(newFileName);

	        copyFile(in, out);

	        in.close();
	        in = null;
	        out.flush();
	        out.close();
	        out = null;
	    }
	    catch (Exception e)
	    {
	        Log.e(TAG, e.getMessage());
	    }

	}

	private void CopyData(String folderName)
	{
		Log.i(TAG, "in folder: " + folderName);
		String[] files = null;
        try
        {
            files = mAssetManager.list(folderName);
        }
        catch (IOException e)
        {
            Log.e(TAG, e.getMessage());
        }
        
        String sdcardPath = Environment.getExternalStorageDirectory().toString();
        
        for(String filename : files)
        {
        	Log.i(TAG, "File name => " + filename);
        	// if (true) continue;
            InputStream in = null;
            OutputStream out = null;
            try
            {
			  String filePath = folderName + "/" + filename;
			  String absolutePath = sdcardPath + "/" + mRootDirectory + "/" + filePath;
			  
			  Log.i(TAG, "relative path in phone to open is: " + filePath);
			  Log.i(TAG, "Absolute path in phone is: " + absolutePath);
			  
			  in = mAssetManager.open(filePath);
			  Log.i(TAG, "init input success");
			  out = new FileOutputStream(absolutePath);
			  Log.i(TAG, "init output success");
			  copyFile(in, out);
			  in.close();
			  in = null;
			  out.flush();
			  out.close();
			  out = null;
            }
            catch(Exception e)
            {
                Log.e("ERROR_IN_OUT", e.getMessage());
            }       
        }
        Log.i(TAG, "End of folder: " + folderName);
	}

	private void copyFile(InputStream in, OutputStream out) throws IOException
	{
		byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1)
        {
          out.write(buffer, 0, read);
        }
		
	}


}
