package com.update.tools;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import org.json.JSONArray;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

public class UpdateManager 
{
	private String curVersion;  
    private String newVersion;  
    private int curVersionCode;  
    private int newVersionCode;  
    private String updateInfo;  
    private UpdateCallback callback;  
    private Context ctx;  
     
    private int progress;    
    private Boolean hasNewVersion;  
    private Boolean canceled;
    
    
    private String newVerCode;
    private String newVerName;
    private String newAppURL;
    private float memmorySize;
 
    //��Ÿ���APK�ļ���·��  
     public static final String UPDATE_DOWNURL = "http://www.baidu.com/update/update_test.apk";  
    //��Ÿ���APK�ļ���Ӧ�İ汾˵��·��  
    public static final String UPDATE_CHECKURL = "http://www.baidu.com/update/update_verson.txt";  
    public static final String UPDATE_APKNAME = "update_test.apk";  
    //public static final String UPDATE_VERJSON = "ver.txt";  
    public static final String UPDATE_SAVENAME = "updateapk.apk";  
    private static final int UPDATE_CHECKCOMPLETED = 1;  
    private static final int UPDATE_DOWNLOADING = 2;   
    private static final int UPDATE_DOWNLOAD_ERROR = 3;   
    private static final int UPDATE_DOWNLOAD_COMPLETED = 4;   
    private static final int UPDATE_DOWNLOAD_CANCELED = 5;  
    //�ӷ�����������apk����ļ���  
    private String savefolder = "/mnt/innerDisk/";  
    private String dir = "sealink";
    public UpdateManager(Context context, UpdateCallback updateCallback) 
   {  
       ctx = context;  
       callback = updateCallback;   
       canceled = false; 
       //下载目录
       savefolder = Environment.getExternalStorageDirectory() + "/"+dir+"/";
       getCurVersion();
   }  

   public String getNewVersionName()  
   {  
       return newVersion;  
   }  
     
   public String getUpdateInfo()  
   {  
       return updateInfo;  
   }  
 
   public void changeUpdateInfo(String diamondNum,String description)
   {
	   updateInfo = "检测到您当前的版本"+curVersion+"过低,请更新至最新版本版本"+newVerName+
			   ",更新之后系统送你"+diamondNum+"个钻石.\n"+"(如果不想下载最新版,断开网络重新进入游戏即可)\n"+description;
   }
   
   private void getCurVersion() 
   {  
       try 
       {  
           PackageInfo pInfo = ctx.getPackageManager().getPackageInfo(  
                   ctx.getPackageName(), 0);  
           curVersion = pInfo.versionName;  
           curVersionCode = pInfo.versionCode;
           Log.e(curVersion, curVersionCode+"");
       } catch (NameNotFoundException e) 
       {  
           Log.e("update", e.getMessage());  
           curVersion = "1111.1000";  
           curVersionCode = 111000;  
       }  
   } 
   
   
   public String getCurVersionName()
   {
	   return curVersion;
   }
   
   public String getCurVersionCode()
   {
	   return curVersionCode+"";
   }
   
   public String getPackageSize()
   {
	   float showSize = memmorySize/4;
	   String packageSize = String.format("%.2fM",showSize);
	   
	   return packageSize;
   }
   
   public void setNewVersionName(String newVersionName)
   {
	   newVerName = newVersionName;
   }
   
   public void setNewVersionCode(String newVersionCode)
   {
	   newVerCode = newVersionCode;
   }
 
   public void setNewAppURL(String appUrl)
   {
	   newAppURL = appUrl;
	   memmorySize = 4f;
	   URL url;
		try {
			url = new URL(newAppURL);
			HttpURLConnection conn;
			try {
				conn = (HttpURLConnection) url.openConnection();
				conn.setRequestProperty("Accept-Encoding", "identity");
				conn.connect();
				int length = conn.getContentLength();
				memmorySize = length / 1048576.0f;
				Log.e("packageSize", memmorySize + "M");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
       
   }
   
   public void checkUpdate() 
   {          
       hasNewVersion = false;  
       new Thread()
       {  
           // ***************************************************************  
           @Override  
           public void run() 
           {  
               Log.i("@@@@@", ">>>>>>>>>>>>>>>>>>>>>>>>>>>getServerVerCode() ");  
               try {  
                   String verjson = NetHelper.httpStringGet(UPDATE_CHECKURL);  
                   Log.i("@@@@", verjson + "***********************************");  
                   JSONArray array = new JSONArray(verjson);  
                   if (array.length() > 0) 
                   {  
                       JSONObject obj = array.getJSONObject(0);  
                       try 
                       {  
                           newVersionCode = Integer.parseInt(obj.getString("verCode"));  
                           newVersion = obj.getString("verName");  
                           updateInfo = "";  
                           Log.i("newVerCode", newVersionCode + "@@@@@@@@@");  
                           Log.i("newVerName", newVersion + "@@@@@@@@@@@");  
                           if (newVersionCode > curVersionCode) 
                           {  
                               hasNewVersion = true;  
                           }  
                       } catch (Exception e) 
                       {  
                           newVersionCode = -1;  
                           newVersion = "";  
                           updateInfo = "";
                       }  
                   }  
               } catch (Exception e)
               {  
                   Log.e("update", e.getMessage());  
               }   
               updateHandler.sendEmptyMessage(UPDATE_CHECKCOMPLETED);  
           };  
           // ***************************************************************  
       }.start();  
   }
   
   
   public void sendUpdate()
   {
	   hasNewVersion = true;  
       new Thread()
       {  
           // ***************************************************************  
           @Override  
           public void run() 
           {   
               updateHandler.sendEmptyMessage(UPDATE_CHECKCOMPLETED);  
           };  
           // ***************************************************************  
       }.start();
   }
  
   public void update() 
   {  
       Intent intent = new Intent(Intent.ACTION_VIEW);  
       intent.setDataAndType(Uri.fromFile(new File(savefolder, UPDATE_SAVENAME)),  
               "application/vnd.android.package-archive");  
       ctx.startActivity(intent);  
   }  
 
   // ++++++++++++++++++++++++++++++++++++++++++  
   public void downloadPackage()   
   {  
       new Thread() 
       {              
            @Override    
            public void run() 
            {    
                   try {    
//                       URL url = new URL(UPDATE_DOWNURL); 
                       URL url = new URL(newAppURL);
                       HttpURLConnection conn = (HttpURLConnection)url.openConnection();
                       //在默认情况下，HttpURLConnection 使用 gzip方式获取，文件 getContentLength() 这个方法，每次read完成后可以获得，当前已经传送了多少数据，而不能用这个方法获取 
                       //需要传送多少字节的内容，当read() 返回 -1时，读取完成，由于这个压缩后的总长度我无法获取，那么进度条就没法计算值了。
                       //要取得长度则，要求http请求不要gzip压缩，具体设置如下 
                       conn .setRequestProperty("Accept-Encoding", "identity"); 
                       conn.connect();    
                       int length = conn.getContentLength();
                       InputStream is = conn.getInputStream();    
                           
                       StringBuilder sb = new StringBuilder(savefolder);
					   File folder = new File(sb.toString());
					   if (!folder.exists()) 
					   {
						   //创建文件夹
						   folder.mkdirs();
						   Log.d("folder", sb.toString());
					   }
                       File ApkFile = new File(savefolder,UPDATE_SAVENAME); 
                       if(ApkFile.exists())  
                       {  
                    	   ApkFile.delete();  
                       }  
                       FileOutputStream fos = new FileOutputStream(ApkFile); 
                       int count = 0;    
                       byte buf[] = new byte[512];
                       do{    
                           int numread = is.read(buf);    
                           count += numread;    
                           progress =(int)(((float)count / length) * 100);   
                           updateHandler.sendMessage(updateHandler.obtainMessage(UPDATE_DOWNLOADING));   
                           if(numread <= 0)
                           {        
                               updateHandler.sendEmptyMessage(UPDATE_DOWNLOAD_COMPLETED);  
                               break;    
                           }    
                           fos.write(buf,0,numread);    
                       }while(!canceled);    
                       if(canceled)  
                       {  
                           updateHandler.sendEmptyMessage(UPDATE_DOWNLOAD_CANCELED);  
                       }  
                       fos.close();    
                       is.close();    
                   } catch (MalformedURLException e) 
                   {    
                       e.printStackTrace();  
                       updateHandler.sendMessage(updateHandler.obtainMessage(UPDATE_DOWNLOAD_ERROR,e.getMessage()));  
                   } catch(IOException e)
                   {    
                       e.printStackTrace();  
                       updateHandler.sendMessage(updateHandler.obtainMessage(UPDATE_DOWNLOAD_ERROR,e.getMessage()));  
                   }    
               }   
       }.start();  
   }  
 
   public void cancelDownload()  
   {  
       canceled = true;  
   }  
     
   Handler updateHandler = new Handler()   
   {  
       @Override  
       public void handleMessage(Message msg) 
       {  
           switch (msg.what)
           {  
	           case UPDATE_CHECKCOMPLETED:
	               callback.checkUpdateCompleted(hasNewVersion, updateInfo);  
	               break;  
	           case UPDATE_DOWNLOADING:  
	               callback.downloadProgressChanged(progress);  
	               break;  
	           case UPDATE_DOWNLOAD_ERROR:  
	               callback.downloadCompleted(false, msg.obj.toString());  
	               break;  
	           case UPDATE_DOWNLOAD_COMPLETED:  
	               callback.downloadCompleted(true, "");  
	               break;  
	           case UPDATE_DOWNLOAD_CANCELED:  
	               callback.downloadCanceled();  
	           default:  
	               break;  
           }  
       }  
   };  
 
   public interface UpdateCallback 
   {  
       public void checkUpdateCompleted(Boolean hasUpdate, CharSequence updateInfo);  
       public void downloadProgressChanged(int progress);  
       public void downloadCanceled();  
       public void downloadCompleted(Boolean sucess, CharSequence errorMsg);  
   }  
}