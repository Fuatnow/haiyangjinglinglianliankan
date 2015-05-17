/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.fuatnow.game.sealink;

import java.text.ParseException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.fuatnow.tools.JsonHelper;
import com.ktplay.open.KTPlay;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;
enum CMD_ID
{
    SIGN_UP,//注册
    LOGIN,//登陆
};

public class AppActivity extends Cocos2dxActivity 
{

	public static AppActivity instance = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		// TODO Auto-generated method stub
		System.loadLibrary("KTPlay");
    	System.loadLibrary("KTAccountmanager");
    	System.loadLibrary("KTFriendship");
    	System.loadLibrary("KTLeaderboard");
		super.onCreate(savedInstanceState);
		instance = this;
		KTPlay.startWithAppKey(getContext(), "3Z12Fe", "ecf9969e2ba6104d7976d96dad2d1f573352121c");
//		Bmob.initialize(this, "dbe2508462089b1db80759f0b4c6d31c");
	}

	public static Object getInstance() 
	{
		Log.e("instance", "instance create Already");
		return instance;
	}

	private void jni_called(int STATE, int num)
	{
		Message msg = mHandler.obtainMessage();
		msg.arg1 = num;
		msg.what = STATE;
		msg.sendToTarget();
		Log.e("state:" + STATE, "num:" + num);
	}

	private String jni_called(String jsonStr)throws JSONException,ParseException  
	{
		Log.e("jni_called", jsonStr);
		JSONObject jo = new JSONObject(jsonStr);
	    JSONArray infoArray = jo.getJSONArray("info");
	    JSONObject cmdObject = infoArray.getJSONObject(0);
        String cmdStr = cmdObject.getString("cmdId");
        String infoStr = infoArray.getString(1);
//        System.out.println(cmdStr);
//        System.out.println(infoStr);
        Message msg = mHandler.obtainMessage();
		msg.what = Integer.parseInt(cmdStr);
		msg.obj = infoStr;
		msg.sendToTarget();
		return "Success";
	}

	private Handler mHandler = new Handler() 
	{
		@Override
		public void handleMessage(Message msg) 
		{
			CMD_ID cmdId = CMD_ID.values()[msg.what];
			String infoStr = (String)msg.obj;
			switch (cmdId) 
			{
			case SIGN_UP:
				try {
					signUp(infoStr);
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;
			}
		}
	};

	void signUp(String jsonStr) throws JSONException
	{
		Map<String, String> map= JsonHelper.toMap(jsonStr);
//
//		for (Entry<String, String> entry: map.entrySet()) 
//		{
//		    String key = entry.getKey();
//		    String value = entry.getValue();
//		    System.out.println(key + "   :  "+value);
//		}
		String userName = map.get("userName");
		String password = map.get("password");
	}
	
	
	public void toast(String msg)
	{
		Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
	}
	
	
	public void backPressed() 
	{
		AlertDialog.Builder builder = new Builder(AppActivity.this);
		builder.setMessage("ȷ���˳���");
		builder.setTitle("��ʾ");
		builder.setPositiveButton("ȷ��", new OnClickListener() 
		{
			@Override
			public void onClick(DialogInterface dialog, int which) 
			{
				dialog.dismiss();
				AppActivity.this.finish();
			}
		});
		builder.setNegativeButton("ȡ��", new OnClickListener() 
		{
			@Override
			public void onClick(DialogInterface dialog, int which) 
			{
				dialog.dismiss();
			}
		});
		builder.create().show();
	}

	
	@Override
	protected void onPause() 
	{
		// TODO Auto-generated method stub
		super.onPause();
		KTPlay.onPause(getContext());
	}
	
	@Override
	protected void onResume() 
	{
		// TODO Auto-generated method stub
		super.onResume();
		KTPlay.onResume(getContext());
	}
}
