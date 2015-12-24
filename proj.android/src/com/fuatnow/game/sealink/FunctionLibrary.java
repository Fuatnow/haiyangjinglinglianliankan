//package com.fuatnow.game.sealink;
//
//
//import java.io.File;
//import java.io.FileInputStream;
//import java.io.FileOutputStream;
//import java.io.InputStream;
//
//import android.app.Activity;
//import android.content.Intent;
//import android.content.res.Configuration;
//import android.media.MediaScannerConnection;
//import android.net.Uri;
//import android.os.Environment;
//import android.util.Log;
//import android.view.Gravity;
//import android.view.View;
//import android.widget.FrameLayout;
//
//import com.google.android.gms.ads.AdListener;
//import com.google.android.gms.ads.AdRequest;
//import com.google.android.gms.ads.AdSize;
//import com.google.android.gms.ads.AdView;
//import com.google.android.gms.ads.InterstitialAd;
//public class FunctionLibrary
//{
//	//private static com.facebook.ads.AdView m_facebookBanner = null;
//	private static Activity m_targetView = null;
//	private static AdView m_adView = null;
//	private static FrameLayout m_frameTarget = null;
//	private static FrameLayout.LayoutParams m_layoutInfo;
//	private static String m_googldBannerId;
//	private static String m_googleFullAdId;
//	private static String m_facebookAdId;
//	private static String m_umengId;
//	private static boolean m_isCallBannerShow;
//	private static boolean m_isFullAdFinished = false;
//	private static InterstitialAd m_fullAdUnit = null;
//	private static boolean m_isBannerAtTop = false;
////	private static FeedbackAgent m_feedbackView; 
//	
//	public static void initData(Activity target,FrameLayout frame,String bannerId,String fullId,String umengId,String facebookid)
//	{
//		m_targetView = target;
//		m_frameTarget = frame;
//		m_googldBannerId = bannerId;
//		m_googleFullAdId = fullId;
//		m_umengId = umengId;
//		m_isCallBannerShow = false;
//		m_isFullAdFinished = false;
//		m_facebookAdId  = facebookid;
////		AnalyticsConfig.setAppkey(m_umengId);
////	    AnalyticsConfig.setChannel("defaultID");
//	    
//
//		 
////	    if(facebookid.length()>0)
////	    {
////		    doLoadFacebookBanner();
////	    }
////	    else
//	    {
////	    	doLoadAdmobBannerView();
//	    }
//	     
//	     //init full ad
//	    m_fullAdUnit = new InterstitialAd( m_targetView);
// 		m_fullAdUnit.setAdUnitId(m_googleFullAdId);
// 		m_fullAdUnit.setAdListener(new AdListener() 
// 		  {
// 		      public void onAdLoaded()
// 		      {
// 		    	 m_isFullAdFinished = true;
// 		  	     Log.e("111", "full onAdLoaded");
// 		      }
//
// 		      public void onAdFailedToLoad(int errorCode)
// 		      {
// 		    	 
// 		    	 m_isFullAdFinished = false;
// 		    	 String errorReason = "";
// 		        switch(errorCode) 
// 		        {
// 		          case AdRequest.ERROR_CODE_INTERNAL_ERROR:
// 		            errorReason = "Internal error";
// 		            break;
// 		          case AdRequest.ERROR_CODE_INVALID_REQUEST:
// 		            errorReason = "Invalid request";
// 		            break;
// 		          case AdRequest.ERROR_CODE_NETWORK_ERROR:
// 		            errorReason = "Network Error";
// 		            break;
// 		          case AdRequest.ERROR_CODE_NO_FILL:
// 		            errorReason = "No fill";
// 		            break;
// 		        }
// 		    	Log.e("111", "full screen faild ->reason:"+ errorReason);
// 		    	doRequestFullAd();
// 		      }
// 
// 		    });
// 		doRequestFullAd();
//	}
//	 public static boolean isScreenOriatationPortrait()
//	 { 
//		 return m_targetView.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT;  
//	} 
//	 
//	public static void doLoadAdmobBannerView()
//	{
//		if(m_adView!=null)
//		{
//			m_frameTarget.removeView(m_adView);
//		}
//		m_adView = new AdView(m_targetView);
//	    m_adView.setAdUnitId(m_googldBannerId);
//	    m_adView.setAdSize(isScreenOriatationPortrait()? AdSize.SMART_BANNER:AdSize.BANNER);
//		if(m_layoutInfo == null)
//		{
//			m_layoutInfo = new FrameLayout.LayoutParams(
//					FrameLayout.LayoutParams.WRAP_CONTENT,
//					FrameLayout.LayoutParams.WRAP_CONTENT);
//			m_layoutInfo.gravity = Gravity.CENTER_HORIZONTAL | Gravity.BOTTOM;
//		}
//		
//		m_adView.setLayoutParams(m_layoutInfo);
//		m_frameTarget.addView(m_adView);
//		
//	    m_adView.setAdListener(new AdListener()
//	    {
//	    	 public void onAdLoaded()
//	    	 {  
//	    		 if(!m_isCallBannerShow)
//	    		 {
//	    			 m_isCallBannerShow = true;
//	    			 super.onAdLoaded();
//
//		    		 m_targetView.runOnUiThread(new Runnable()
//		    		 {
//		    	            public void run()
//		    	            {
//		    	            	 Log.e("111", " banner recv");
//		    	            	m_frameTarget.requestLayout();
//		    	            }
//		    	        });
//	    			// FunctionLibrary.onBannerShow();
//	    		 }
//	    	 } 
//	    	  public void onAdFailedToLoad(int errorCode)
//	    	  {
//	    		  String errorReason = "";
//	 		        switch(errorCode) 
//	 		        {
//	 		          case AdRequest.ERROR_CODE_INTERNAL_ERROR:
//	 		            errorReason = "Internal error";
//	 		            break;
//	 		          case AdRequest.ERROR_CODE_INVALID_REQUEST:
//	 		            errorReason = "Invalid request";
//	 		            break;
//	 		          case AdRequest.ERROR_CODE_NETWORK_ERROR:
//	 		            errorReason = "Network Error";
//	 		            break;
//	 		          case AdRequest.ERROR_CODE_NO_FILL:
//	 		            errorReason = "No fill";
//	 		            break;
//	 		        }
//	 		    	Log.e("111", "admob banner faild->reason:"+ errorReason);
//	 		    	
//	    		  if(!m_isCallBannerShow)
//		          { 
//	    			  doRequestAdmobBanner();
//		    	  }
//	    	  }
//	    	});
//	    doRequestAdmobBanner();
//	}
//	
//	
//	public static  void doRequestAdmobBanner()
//	{ 
//	     AdRequest adRequest = new AdRequest.Builder().build();
//	     m_adView.loadAd(adRequest);
//	}
//	
//	
//	
////	public static void doLoadFacebookBanner()
////	{ 
////		m_facebookBanner = new com.facebook.ads.AdView(m_targetView,m_facebookAdId,  com.facebook.ads.AdSize.BANNER_320_50);
////	 
////		if(m_layoutInfo == null)
////		{
////			m_layoutInfo = new FrameLayout.LayoutParams(
////					FrameLayout.LayoutParams.WRAP_CONTENT,
////					FrameLayout.LayoutParams.WRAP_CONTENT);
////			m_layoutInfo.gravity = Gravity.CENTER_HORIZONTAL | Gravity.BOTTOM;
////		}
////		
////		m_facebookBanner.setLayoutParams(m_layoutInfo);
////		m_frameTarget.addView(m_facebookBanner);
////		m_facebookBanner.setAdListener( new com.facebook.ads.AdListener()
////		{
////			public void onAdClicked(Ad ad)
////			{ 
////				
////			} 
////			
////			public void onAdLoaded(Ad ad) 
////			{ 
////
////				if(!m_isCallBannerShow)
////	    		 {
////	    			 m_isCallBannerShow = true; 
////
////		    		 m_targetView.runOnUiThread(new Runnable()
////		    		 {
////		    	            public void run()
////		    	            { 
////		    	            	m_frameTarget.requestLayout();
////		    	            }
////		    	        });
////	    			 FunctionLibrary.onBannerShow();
////	    		 }
////				Log.e("111", "facebook  banner ad loaded");
////			}
////			public void onError(Ad ad, AdError error)
////			{ 
////				Log.e("111", "facebook  error:"+error.getErrorMessage()+", load admob banner");
////				m_facebookBanner.setAdListener(null);
////				m_frameTarget.removeView(m_facebookBanner); 
////				m_facebookBanner.destroy();
////				m_facebookBanner=null;
////				if(m_adView==null)
////				{
////					doLoadAdmobBannerView();	
////				}
////			}
////		});
////		m_facebookBanner.loadAd(); 
////	}
////	 
//	public static native void onBannerShow();
//	
//	public static boolean copyToPhoto(final String srcFile,final String destName)
//	{
//		String path=null;
//		if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) 
//		{
//	        final String sdcardPath = Environment.getExternalStorageDirectory().getAbsolutePath();
//	            if (null == sdcardPath) 
//	            {	 
//	            	return false;
//	            }
//	            path = sdcardPath + "/DCIM/Screenshots/";
//		 }
//		  if (null == path ) 
//          {	 
//          	return false;
//          }
//		File targetDir = new File(path);
//  
//		if(!targetDir.exists())
//		{
//			targetDir.mkdirs();
//		}
//		 try
//		 {   
//	           int bytesum = 0;   
//	           int byteread = 0;   
//	           File oldfile = new File(srcFile);   
//	           if (oldfile.exists()) 
//	           { 
//	               InputStream inStream = new FileInputStream(srcFile);   
//	               FileOutputStream fs = new FileOutputStream(path+destName);   
//	               byte[] buffer = new byte[1444];   
//	               int length;   
//	               while ( (byteread = inStream.read(buffer)) != -1)
//	               {   
//	                   bytesum += byteread; 
//	                   System.out.println(bytesum);   
//	                   fs.write(buffer, 0, byteread);   
//	               }   
//	               inStream.close();   
//	               try
//	      		   {   
//	            	 if(android.os.Build.VERSION.SDK_INT < 19 )
//		        	 {
//		        		 m_targetView.sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, Uri.fromFile(new File(path+destName))));	 
//		        	 }
//		        	 else
//		        	 {
//		        		  MediaScannerConnection.scanFile(m_targetView, new String[]{path+destName}, null, null);
//		        	 }
//	            	 oldfile.delete();
//	            	 return true;
//	      		 }  
//	               catch (Exception e)
//		  	       {    
//		      			 return false;
//		  	       } 
//	           } 
//	           else
//	           { 
//	               return false;
//	           }
//	       }   
//	       catch (Exception e)
//	       {    
//	    		return false;
//	       }
//	}
//	
//	public static void doRate()
//	{
//		String mAddress = "market://details?id=" + m_targetView.getPackageName(); 
//	    Intent marketIntent = new Intent("android.intent.action.VIEW");  
//	    marketIntent .setData(Uri.parse(mAddress ));  
//	    m_targetView.startActivity(marketIntent ); 
//	}
//	
//	//»´∆¡π„∏Ê
//	public static void doShowFullAd(final boolean s)
//	{
//		 m_targetView.runOnUiThread(new Runnable()
//		 {
//	            public void run()
//	            {
//	        		if (m_isFullAdFinished && m_fullAdUnit.isLoaded())
//	        		{
//	        			m_fullAdUnit.show();
//	        			m_isFullAdFinished = false;
//	        			Log.e("111","do show full ad");
//	        		}
//	        		else
//	        		{
//	        			
//	        			Log.e("111","no full ad");
//	        		}
//	        		doRequestFullAd();
//	            }
//	        });
//	}
//	
//	public static void doRequestFullAd()
//	{
//		if(!m_isFullAdFinished)
//		{
//			m_targetView.runOnUiThread(new Runnable()
//			 {
//		            public void run()
//		            {
//		        	    AdRequest adRequest = new AdRequest.Builder().build();
//		        	    m_fullAdUnit.loadAd(adRequest);
//		            }
//		        });
//
//			 Log.e("111", "doRequestFullAd");
//		
//		}
//	}
//
//	public static boolean isFullAdFinished()
//	{
//		 return true;
//	}
//
//	//banner
//	public static void setBannerIsTop(final boolean b)
//	{
//		
//		 m_targetView.runOnUiThread(new Runnable()
//		 {
//	            public void run()
//	            {
//	            	m_layoutInfo = new FrameLayout.LayoutParams(
//							FrameLayout.LayoutParams.WRAP_CONTENT,
//							FrameLayout.LayoutParams.WRAP_CONTENT);
//					if( b )
//					{
//						m_layoutInfo.gravity =m_layoutInfo.gravity | Gravity.TOP;
//					}
//					else
//					{
//						m_layoutInfo.gravity = m_layoutInfo.gravity | Gravity.BOTTOM;
//					}
//					m_isBannerAtTop = b;
//					updateLayout(m_layoutInfo);
//	            }
//	            });
//
//	}
//	
//	public static void setBannerVisible(final boolean b)
//	{
//    	 Log.e("111", b ? "VISIBLE" : "INVISIBLE");
//    	
//    	 m_targetView.runOnUiThread(new Runnable()
//		 {
//	            public void run()
//	            {
//	            	if(m_adView != null)
//	            	{
//	            		m_adView.setVisibility(b ? View.VISIBLE:View.GONE);	
//	            	}
////	            	if(m_facebookBanner!=null)
////	            	{
////	            		m_facebookBanner.setVisibility(b ? View.VISIBLE:View.GONE);
////	            	}
//	            }
//	        });
//	}
//	
////	//”—√À ¬º˛Õ≥º∆
////	public static void doEventByName(final String  s)
////	{
////		MobclickAgent.onEvent(m_targetView,s);
////	}
//
//	//…ÃµÍ¥Úø™”¶”√
//	public static void doOpenStoreUrl(final String s)
//	{
//		String mAddress = "market://details?id=" +s; 
//	    Intent marketIntent = new Intent("android.intent.action.VIEW");  
//	    marketIntent.setData(Uri.parse(mAddress ));  
//	    m_targetView.startActivity(marketIntent );
//	}
//
//	//∑¢” º˛
//	public static void doSendEmail()
//	{
//		//m_feedbackView.startFeedbackActivity();
//	}
//	private static void updateLayout(FrameLayout.LayoutParams v)
//	 {
//		 if(m_adView != null)
//			{
//				m_adView.setLayoutParams(v);
//			}
////			if(m_facebookBanner!=null)
////			{
////				m_facebookBanner.setLayoutParams(v);
////			}
//	 }
//
//	public static void setHorizontalAlignment(final int s)
//	{
//		m_targetView.runOnUiThread(new Runnable()
//		 {
//	            public void run()
//	            {
//	        		switch( s )
//	        		{
//	        		case 0:
//	        			m_layoutInfo.gravity = Gravity.LEFT | (m_isBannerAtTop? Gravity.TOP:Gravity.BOTTOM);
//	        			updateLayout(m_layoutInfo);
//	        			break;
//	        		case 1:
//	        			m_layoutInfo.gravity = Gravity.CENTER | (m_isBannerAtTop? Gravity.TOP:Gravity.BOTTOM);
//	        			updateLayout(m_layoutInfo);
//	        			break;
//	        		case 2:
//	        			m_layoutInfo.gravity = Gravity.RIGHT | (m_isBannerAtTop? Gravity.TOP:Gravity.BOTTOM);
//	        			updateLayout(m_layoutInfo);
//	        			break;
//	        		case 3:
//	        			m_layoutInfo.gravity = Gravity.FILL_HORIZONTAL | (m_isBannerAtTop? Gravity.TOP:Gravity.BOTTOM);
//	        			updateLayout(m_layoutInfo);
//	        			break;
//	        		}
//	            }
//	        });
//	}
//
//	
//}