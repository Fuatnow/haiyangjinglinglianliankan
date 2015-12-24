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
import java.util.Map;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.ProgressDialog;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;
import cn.bmob.v3.AsyncCustomEndpoints;
import cn.bmob.v3.Bmob;
import cn.bmob.v3.listener.CloudCodeListener;
import cn.sharesdk.ShareSDKUtils;

import com.fuatnow.tools.JsonHelper;
import com.fuatnow.zggg.Tcenfe;
import com.ktplay.open.KTPlay;
import com.tencent.android.tpush.XGPushManager;
import com.tencent.bugly.crashreport.CrashReport;
import com.tencent.bugly.crashreport.CrashReport.UserStrategy;
import com.umeng.fb.FeedbackAgent;
import com.umeng.mobclickcpp.MobClickCppHelper;
import com.update.tools.DialogHelper;
import com.update.tools.UpdateManager;
//import com.bmob.pay.tool.BmobPay;
//import com.bmob.pay.tool.PayListener;


enum CMD_ID
{
    SIGN_UP,//注册
    pay_diamond,//购买钻石
    loadAD,//加载广告
    key_back,//点击返回键
    down_app,//下载新版本
    fore_down_app,//强制下载最新版本
    exit_game,//退出游戏
    get_channel,//获取渠道
    get_reputation,//获取好评
    feedback,//游戏反馈
};



public class AppActivity extends Cocos2dxActivity 
{
	public static String CHANNEL_360 = "360";         //1
	public static String CHANNEL_BAIDU = "BAIDU";     //2
	public static String CHANNEL_DEFAULT = "DEFAULT"; //3
	public static String CHANNEL_SELF= "SELF";        //99
	public static String CHANNELID = CHANNEL_360;
	public static String downAppURL = "http://fuatnow.bmob.cn/";
	
	public static String promotion_360 = "http://zhushou.360.cn/detail/index/soft_id/2954111";
	public static String promotion_BAIDU = "http://shouji.baidu.com/game/item?docid=7750833&from=as";
	public static String promotion_DEFAULT = "http://a.app.qq.com/o/simple.jsp?pkgname=com.fuatnow.game.sealink";
	
	public static AppActivity instance = null;
//	BmobPay bmobPay;
	ProgressDialog dialog;
	private UpdateManager updateMan;  
    private ProgressDialog updateProgressDialog;
    private String newVersionName; 
    private int newVersionCode;
    private boolean canShowAD = false;
    private String updateCancelInfo = "暂不更新";
    private String isMarketVersion = "1";
	
	public static native void AndroidCallBack(String jsonStr);
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		// TODO Auto-generated method stub
		System.loadLibrary("KTPlay");
    	System.loadLibrary("KTAccountmanager");
    	System.loadLibrary("KTFriendship");
    	System.loadLibrary("KTLeaderboard");
		super.onCreate(savedInstanceState);
		initData();
	}
	
	void initData()
	{
		instance = this;
		updateMan = new UpdateManager(this, appUpdateCb); 
		initThirdSDK();
	}

	void initThirdSDK()
	{
		intKtPlay();
		initBomb();
		initXG();
		initUMeng();
		initAD();
		initShareSDK();
		initBugly();
//	    final Uri uri = Uri.parse("https://saddleback.v2s.mobi/");          
//	    final Intent it = new Intent(Intent.ACTION_VIEW, uri);          
//	    startActivity(it);
	}
	
	void initUMeng()
	{
		Log.e("initUMeng", "initUMeng");
		MobClickCppHelper.init(this);
	}
	
	void intKtPlay()
	{
		KTPlay.startWithAppKey(getContext(), "3Z12Fe", "ecf9969e2ba6104d7976d96dad2d1f573352121c");
	}
	
	void initBomb()
	{
		Bmob.initialize(this, "efe456b3044789a7be41e62e262da7c1");
		// 初始化BmobPay对象,可以在支付时再初始化
//		bmobPay = new BmobPay(this);
		dialog = null;
	}
	
	void initXG()
	{
		Context context = getApplicationContext();
		XGPushManager.registerPush(context);	
	}
	
	void initAD()
	{
		initAdmonAd();
	}
	
	void initShareSDK()
	{
		ShareSDKUtils.prepare();
	}
	
	void initBugly()
	{
		
		UserStrategy strategy = new UserStrategy(getApplicationContext()); //App的策略Bean
		
		strategy.setAppChannel( CHANNELID);     //设置渠道
		strategy.setAppVersion( updateMan.getCurVersionName());      //App的版本
		strategy.setAppReportDelay(5000);  //设置SDK处理延时，毫秒
		Context appContext =  this.getApplicationContext();         
        String appId = "900005622";   //上Bugly(bugly.qq.com)注册产品获取的AppId
        boolean isDebug = false ;  //true代表App处于调试阶段，false代表App发布阶段
        CrashReport.initCrashReport(appContext , appId ,isDebug,strategy);  //初始化SDK  
	}
	
	void initAdmonAd()
	{
//记得吧manest.xml的bomb配置加上
//		FunctionLibrary.initData(this,mFrameLayout,
//
//	        		"ca-app-pub-1039662318264516/2165133587",
//	        		"ca-app-pub-1039662318264516/9243441582",
//	        		"55c85083e0f55afe930049b2","");
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

	public String jni_called(String jsonStr)throws JSONException,ParseException  
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
        int cmd = Integer.parseInt(cmdStr);
		msg.what = cmd;
		msg.obj = infoStr;
		msg.sendToTarget();
		return retStr(cmd);
	}
	
	String retStr(int cmd)
	{
		String returnStr = "Success";
		if(CMD_ID.values()[cmd] == CMD_ID.get_channel)
		{
			if(CHANNEL_360 ==  CHANNELID) returnStr = "1";
			if(CHANNEL_BAIDU ==  CHANNELID) returnStr = "2";
			if(CHANNEL_DEFAULT ==  CHANNELID) returnStr = "3";
			if(CHANNEL_SELF ==  CHANNELID) returnStr = "99";
		}
		return returnStr;
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
			case pay_diamond:
				try {
					pay_diamond(infoStr);
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;
			case loadAD:
				try {
					loadAD(infoStr);
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;
			case key_back:
				try {
					keyBack(infoStr);
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;
			case down_app:
				try {
					downNewApp(infoStr);
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;
			case fore_down_app:
				foreUpdateApp(infoStr);
				break;
			case get_reputation:
				getReputation(infoStr);
				break;
			case feedback:
				feedBack(infoStr);
				break;
			default:
					break;
			}
		}
	};

	
	void downNewApp(String jsonStr) throws JSONException
	{
		AsyncCustomEndpoints ace = new AsyncCustomEndpoints();
		JSONObject jo = new JSONObject();
		JSONObject obj = new JSONObject();
		try
		{
			JSONObject cppJo = new JSONObject(jsonStr);
			isMarketVersion = cppJo.getString("isMarketVersion");
			String curVersionName = updateMan.getCurVersionName();
			String curVersionCode = updateMan.getCurVersionCode();
			jo.put("cloudCmd", "5");
			obj.put("curVersionName", curVersionName);
			obj.put("curVersionCode", curVersionCode);
			obj.put("channel_ID", CHANNELID);
			obj.put("isMarketVersion", isMarketVersion);
			jo.put("info",obj);
			Log.e("jo", jo.toString());
		} catch (JSONException e1) 
		{
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		ace.callEndpoint(this, "mainMethod", jo, new CloudCodeListener()
		{
			final JSONObject jsonObj = new JSONObject();
			final JSONObject jo = new JSONObject();
			@Override
			public void onSuccess(Object object) 
			{
				// TODO Auto-generated method stub
				String result = object.toString();
				Log.e("result", result);
//				toast("云端方法返回：" + result);
			
				try 
				{
					final JSONObject resultObj = new JSONObject(result);
					String needDownStr = resultObj.getString("needDownStr");
					String foreUpdateStr = resultObj.getString("foreUpdateStr");
					String showAdStr = resultObj.getString("canShowAD");
					String newVersionCode = resultObj.getString("newVersionCode");
					String newVersionName = resultObj.getString("newVersionName");
					String diamondNum = resultObj.getString("diamondNum");
					String description = resultObj.getString("description");
					String appURL = resultObj.getString("appURL");
					updateMan.setNewVersionCode(newVersionCode);
					updateMan.setNewVersionName(newVersionName);
					updateMan.setNewAppURL(appURL);
					updateMan.changeUpdateInfo(diamondNum, description);
					if(showAdStr.equals("1")) canShowAD = true;
					if(foreUpdateStr.equals("1")) updateCancelInfo = "取消";
					jsonObj.put("needDownStr",needDownStr);
					jsonObj.put("foreUpdateStr",foreUpdateStr);
					jsonObj.put("diamondNum",diamondNum);
					jo.put("cmdId","4");
					jo.put("info",jsonObj);
				} catch (JSONException e1)
				{
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				String jsonStr = jo.toString();
				Log.e("downNewApp", jsonStr);
				AndroidCallBack(jsonStr);
			}
			
			@Override
			public void onFailure(int code, String msg) 
			{
				// TODO Auto-generated method stub
				//toast("云端方法返回：" + msg);
				try 
				{
					jsonObj.put("needDownStr",String.valueOf(0));
					jsonObj.put("foreUpdateStr",String.valueOf(0));
					jsonObj.put("diamondNum",String.valueOf(0));
					jo.put("cmdId","4");
					jo.put("info",jsonObj);
				} catch (JSONException e1)
				{
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				String jsonStr = jo.toString();
				Log.e("downNewApp", jsonStr);
				AndroidCallBack(jsonStr);
			}
		});
	}
	
	void foreUpdateApp(String jsonStr)
	{ 
	    updateMan.sendUpdate(); 
	}
	
	/**
	 * 云端代码
	 */
	private void cloudCode(String jsonStr) throws JSONException
	{
		
		//带请求参数
		AsyncCustomEndpoints ace = new AsyncCustomEndpoints();
		JSONObject obj = new JSONObject();
		try 
		{
			obj.put("cmd", "1");
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		ace.callEndpoint(this, "updateNewApp", obj, new CloudCodeListener() 
		{
			
			@Override
			public void onSuccess(Object object) 
			{
				// TODO Auto-generated method stub
				String result = object.toString();
				toast("云端usertest方法返回:" + result);
			}
			
			@Override
			public void onFailure(int code, String msg) {
				// TODO Auto-generated method stub
				toast("访问云端usertest方法失败:" + msg);
			}
		});
//		//不带请求的云端代码
//		ace.callEndpoint(MainActivity.this, "testJSONObject", new CloudCodeListener() {
//
//			@Override
//			public void onSuccess(Object object) {
//				toast("云端usertest方法返回:" + object.toString());
//				String json = object.toString();
//				try {
//					JSONObject obj = new JSONObject(json);
//					String ud = obj.getString("ud");
//					toast("云端usertest方法返回ud:" + ud);
//				} catch (Exception e) {
//					// TODO: handle exception
//					toast("云端usertest方法返回错误:" + e.getMessage());
//				}
//			}
//
//			@Override
//			public void onFailure(int code, String msg) {
//				// TODO Auto-generated method stub
//				toast("访问云端usertest方法失败:" + msg);
//			}
//		});
	}
	
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
	
	void pay_diamond(String jsonStr) throws JSONException
	{
		Map<String, String> map= JsonHelper.toMap(jsonStr);
		String name = map.get("name");
		String body = map.get("body");
		String payId = map.get("payId");
		double price =  Double.parseDouble(map.get("price"));
		payByAli(price,name,body,payId);
	}
	
	void loadAD(String jsonStr) throws JSONException
	{
		Map<String, String> map= JsonHelper.toMap(jsonStr);
		String adType = map.get("adType");
		String maxLevelStr = map.get("maxLevel");
		String needForeUpdate = map.get("needForeUpdate");
		Log.e("adType", adType);
		Log.e("canShowAD", canShowAD+"");
		showAD_foreUpdateAPP(maxLevelStr,needForeUpdate);
		if(canShowAD == false)
		{
			return;
		}
		if(adType.equals("ZhangYun"))
		{
			//FunctionLibrary.doShowFullAd(true);

			Tcenfe pm = Tcenfe.getInstance(getApplicationContext(),"60e76d566811853da8f1b4c61107cc3b");
			pm.load();//可预加载提前调用缓存广告至本地
			pm.c();
			pm.t();
			pm.show(AppActivity.this);//激活插屏广告
		}
	}
	
	//是否能够加载广告或者强制更新app
	void showAD_foreUpdateAPP(String maxLevelStr,String needForeUpdate)
	{
		String SHOW_AD_FOREUFPATE_APP = "6";
		AsyncCustomEndpoints ace = new AsyncCustomEndpoints();
		JSONObject jo = new JSONObject();
		JSONObject obj = new JSONObject();
		try
		{
			jo.put("cloudCmd", SHOW_AD_FOREUFPATE_APP);
			obj.put("maxLevel", maxLevelStr);
			obj.put("needForeUpdate", needForeUpdate);
			String curVersionName = updateMan.getCurVersionName();
			String curVersionCode = updateMan.getCurVersionCode();
			obj.put("curVersionName", curVersionName);
			obj.put("curVersionCode", curVersionCode);
			obj.put("channel_ID", CHANNELID);
			obj.put("isMarketVersion", isMarketVersion);
			jo.put("info",obj);
			Log.e("jo", jo.toString());
		} catch (JSONException e1) 
		{
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		ace.callEndpoint(this, "mainMethod", jo, new CloudCodeListener()
		{
			@Override
			public void onSuccess(Object object) 
			{
				// TODO Auto-generated method stub
				String result = object.toString();
				Log.e("result", result);
//				toast("云端方法返回：" + result);
			
				try 
				{
					final JSONObject resultObj = new JSONObject(result);
					String showAdStr = resultObj.getString("canShowAD");
					String foreUpdateStr = resultObj.getString("foreUpdateStr");
					if(showAdStr.equals("1")) canShowAD = true;
					if(foreUpdateStr.equals("1")) updateMan.sendUpdate();
				} catch (JSONException e1)
				{
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
			
			@Override
			public void onFailure(int code, String msg) 
			{
				// TODO Auto-generated method stub
				//toast("云端方法返回：" + msg);
				canShowAD = false;
			}
		});
	}
	
	void getReputation(String jsonStr)
	{
		JSONObject resultObj;
		try {
			resultObj = new JSONObject(jsonStr);
			String levelStr = resultObj.getString("curLevel");
			int level = Integer.parseInt(levelStr);
			if(level == 5 || level % 15 == 0)
			{
				
				
				
				AlertDialog.Builder builder = new Builder(AppActivity.this);
				builder.setMessage("开发者夜以继日的开发游戏，\n您愿意花30秒的时间支持一下开发者,\n让更多的朋友一起加入我们吗？");
				builder.setTitle("哈喽");
				builder.setPositiveButton("确定", new OnClickListener() 
				{
					@Override
					public void onClick(DialogInterface dialog, int which) 
					{
						 try{
							     Uri uri = Uri.parse("market://details?id="+getPackageName());  
							     Intent intent = new Intent(Intent.ACTION_VIEW,uri);  
							     intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);          
							     startActivity(intent);
							 }catch(ActivityNotFoundException e){
							     Toast.makeText(getContext(), "未发现安卓手机助手", Toast.LENGTH_SHORT).show();
							}
						 dialog.dismiss();
					}
				});
				builder.create().show();
			}
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	void keyBack(String jsonStr) throws JSONException
	{
		backPressed();
	}
	
	// 调用支付宝支付
	void payByAli(double price , String name , String body,String payId)
	{		
		showDialog("正在获取订单...");
		final JSONObject jsonObj = new JSONObject();
		final JSONObject jo = new JSONObject();
		try {
			jo.put("cmdId",String.valueOf(CMD_ID.pay_diamond.ordinal()));
			jsonObj.put("payId",payId);
		} catch (JSONException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
//		bmobPay.pay(price, name,body, new PayListener() 
//		{
//			// 因为网络等原因,支付结果未知(小概率事件),出于保险起见稍后手动查询
//			@Override
//			public void unknow() 
//			{
//				try {
//					jsonObj.put("status", "2");
//				} catch (JSONException e1) {
//					// TODO Auto-generated catch block
//					e1.printStackTrace();
//				}
//				toast("支付结果未知,请稍后手动查询");
//				hideDialog();
//				
////				try 
////				{
////					jsonObj.put("info",jsonObj);
////				} catch (JSONException e)
////				{
////					// TODO Auto-generated catch block
////					e.printStackTrace();
////				}
////				String jsonStr = jsonObj.toString();
////				Log.e("payInfo", jsonStr);
////				AndroidCallBack(jsonStr);
//			}
//
//			// 支付成功,如果金额较大请手动查询确认
//			@Override
//			public void succeed() 
//			{
//				try {
//					jsonObj.put("status", "1");
//				} catch (JSONException e1) {
//					// TODO Auto-generated catch block
//					e1.printStackTrace();
//				}
//				toast("支付成功!");
//				hideDialog();
//				
//				try 
//				{
////					JSONArray jsonArray = new JSONArray();
////					jsonArray.put(jsonObj);
//					jo.put("info",jsonObj);
//				} catch (JSONException e)
//				{
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
//				String jsonStr = jo.toString();
//				Log.e("payInfo", jsonStr);
//				AndroidCallBack(jsonStr);
//			}
//
//			// 无论成功与否,返回订单号
//			@Override
//			public void orderId(String orderId) 
//			{
//				// 此处应该保存订单号,比如保存进数据库等,以便以后查询
////				order.setText(orderId);
//				try {
//					jsonObj.put("orderId", orderId);
//				} catch (JSONException e1) {
//					// TODO Auto-generated catch block
//					e1.printStackTrace();
//				}
//				showDialog("获取订单成功!请等待跳转到支付页面~");
//				
//				try 
//				{
////					JSONArray jsonArray = new JSONArray();
////					jsonArray.put(jsonObj);
//					jo.put("info",jsonObj);
//				} catch (JSONException e)
//				{
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
//				String jsonStr = jo.toString();
//				Log.e("payInfo", jsonStr);
////				AndroidCallBack(jsonStr);
//			}
//
//			// 支付失败,原因可能是用户中断支付操作,也可能是网络原因
//			@Override
//			public void fail(int code, String reason)
//			{
//				try {
//					jsonObj.put("status", "0");
//				} catch (JSONException e1) {
//					// TODO Auto-generated catch block
//					e1.printStackTrace();
//				}
//				toast("支付中断");
//				hideDialog();
//				
//				try 
//				{
////					JSONArray jsonArray = new JSONArray();
////					jsonArray.put(jsonObj);
//					jo.put("info",jsonObj);
//				} catch (JSONException e)
//				{
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
//				String jsonStr = jo.toString();
//				Log.e("payInfo", jsonStr);
//				AndroidCallBack(jsonStr);
//			}
//		});
	}
	

	void feedBack(String jsonStr) 
	{
		FeedbackAgent agent = new FeedbackAgent(getContext());
		agent.sync();
		agent.startFeedbackActivity();
	}
	
	void showDialog(String message)
	{
		if (dialog == null) 
		{
			dialog = new ProgressDialog(this);
			dialog.setCancelable(false);
		}
		dialog.setMessage(message);
		dialog.show();
	}

	void hideDialog() 
	{
		if (dialog != null && dialog.isShowing())
			dialog.dismiss();
	}
	
	
	public void toast(String msg)
	{
		Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
	}
	
	
	public void backPressed() 
	{	
		AlertDialog.Builder builder = new Builder(AppActivity.this);
		builder.setMessage("确定要退出吗？");
		builder.setTitle("提示");
		builder.setPositiveButton("确定", new OnClickListener() 
		{
			@Override
			public void onClick(DialogInterface dialog, int which) 
			{
				//3.x之后变成进入后台了
				//exitGame();
				//AppActivity.this.finish();
				System.exit(0);
				dialog.dismiss();
			}
		});
		builder.setNegativeButton("取消", new OnClickListener() 
		{
			@Override
			public void onClick(DialogInterface dialog, int which) 
			{
				dialog.dismiss();
			}
		});
		builder.create().show();
	}
	
	
	void exitGame()
	{
		final JSONObject jsonObj = new JSONObject();
		final JSONObject jo = new JSONObject();
		try {
			jsonObj.put("exitGame","exitGame");
			jo.put("cmdId","6");
			jo.put("info",jsonObj);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String jsonStr = jo.toString();
		Log.e("exitGame", jsonStr);
		AndroidCallBack(jsonStr);
	}
	
	// UpdateManager 回调
    UpdateManager.UpdateCallback appUpdateCb = new UpdateManager.UpdateCallback() 
    {

        public void downloadProgressChanged(int progress) 
        {
            if (updateProgressDialog != null && updateProgressDialog.isShowing())
            {
                updateProgressDialog.setProgress(progress);
            }

        }

        public void downloadCompleted(Boolean sucess, CharSequence errorMsg) 
        {
            if (updateProgressDialog != null && updateProgressDialog.isShowing()) 
            {
                updateProgressDialog.dismiss();
                toast("下载完成");
            }
            if (sucess)
            {
                updateMan.update();
            }
            else 
            {
            	//如果下载失败就进入游戏官网自己下载
				Intent intent= new Intent();        
			    intent.setAction("android.intent.action.VIEW");    
			    Uri content_url = Uri.parse(downAppURL);   
			    intent.setData(content_url);  
			    startActivity(intent);
            }
        }

        public void downloadCanceled() 
        {
            // TODO Auto-generated method stub

        }       

        public void checkUpdateCompleted(Boolean hasUpdate,CharSequence updateInfo) 
        {
            if (hasUpdate)
            {
                DialogHelper.Confirm(AppActivity.this,
                		"提示",updateInfo,
                		"更新",
                        new DialogInterface.OnClickListener()
                		{

                            public void onClick(DialogInterface dialog,int which)
                            {
                            	String packageSize = updateMan.getPackageSize();
                                updateProgressDialog = new ProgressDialog(
                                		AppActivity.this);
                                updateProgressDialog
                                        .setMessage("占用空间"+packageSize+",请保持网络畅通");
                                updateProgressDialog.setIndeterminate(false);
                                updateProgressDialog.setIcon(R.drawable.icon);
                                updateProgressDialog.setCanceledOnTouchOutside(false);
                                updateProgressDialog.setCancelable(false);
                                updateProgressDialog
                                        .setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
                                updateProgressDialog.setMax(100);
                                updateProgressDialog.setProgress(0);
                                updateProgressDialog.show();
                                updateMan.downloadPackage();
                            }
                        },updateCancelInfo, null);
            }

        }
    };
	
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
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		// TODO Auto-generated method stub
		// 授权回调    
		super.onActivityResult(requestCode, resultCode, data);
	}
	
}
