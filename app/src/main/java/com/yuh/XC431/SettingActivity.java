package com.yuh.XC431;


import android.app.Activity;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;


public class SettingActivity extends Activity {
	
	private Button save_btnButton;
	private Button cancle_btnButton;
	private EditText vedio_EditText;
	private EditText control_EditText;
	private EditText forword_EditText;
	private EditText back_EditText;
	private EditText turnleft_EditText;
	private EditText turnright_EditText;
	private EditText djleft_EditText;
	private EditText djcenter_EditText;
	private EditText djright_EditText;
	private EditText stop_EditText;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_setting);
		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//横屏代码
		 //获取保存的数据
        SharedPreferences  share = SettingActivity.this.getSharedPreferences("perference", MODE_PRIVATE);
        
        //根据key寻找值 参数1 key 参数2 如果没有value显示的内容
        String getvideoip = share.getString("videoip", "http://192.168.1.1:8080?action=naspshot");
		String getcontrolip  = share.getString("controlip", "192.168.1.1:2001");
		String getforword  = share.getString("forword", "4");
		String getback  = share.getString("back", "2");
		String getturnleft  = share.getString("turnleft", "3");
		String getturnright  = share.getString("turnright", "1");
		String getdjleft  = share.getString("djleft", "5");
		String getdjcenter  = share.getString("djcenter", "6");
		String getdjright  = share.getString("djright", "7");
		String getstop = share.getString("stop", "0");
		
		//获取输入框
		vedio_EditText = (EditText)SettingActivity.this.findViewById(R.id.editvideo);
		control_EditText = (EditText)SettingActivity.this.findViewById(R.id.Editcontrol);
		forword_EditText = (EditText)SettingActivity.this.findViewById(R.id.Editforword);
		back_EditText = (EditText)SettingActivity.this.findViewById(R.id.Editback);
		turnleft_EditText = (EditText)SettingActivity.this.findViewById(R.id.Editleft);
		turnright_EditText = (EditText)SettingActivity.this.findViewById(R.id.Editright);
		djcenter_EditText = (EditText)SettingActivity.this.findViewById(R.id.ezhangk);
		djleft_EditText=(EditText)SettingActivity.this.findViewById(R.id.ehes);
		djright_EditText=(EditText)SettingActivity.this.findViewById(R.id.eqians);
		stop_EditText=(EditText)SettingActivity.this.findViewById(R.id.Editstop);
		//给输入框赋值，显示出来
		vedio_EditText.setText(getvideoip);
		control_EditText.setText(getcontrolip);
		forword_EditText.setText(getforword);
		back_EditText.setText(getback);
		turnleft_EditText.setText(getturnleft);
		turnright_EditText.setText(getturnright);
		djcenter_EditText.setText(getdjleft);
		djleft_EditText.setText(getdjcenter);
		djright_EditText.setText(getdjright);
		stop_EditText.setText(getstop);
		
		save_btnButton=(Button)findViewById(R.id.saveBtn);
		//点击保存按钮处理的事件
		save_btnButton.setOnClickListener(new View.OnClickListener()
        {

			@Override
			public void onClick(View v) {
				
				//获取用户输入的信息
				String videoip = vedio_EditText.getText().toString();
				String controlip  = control_EditText.getText().toString();
				String forword = forword_EditText.getText().toString();
				String back = back_EditText.getText().toString();
				String turnleft = turnleft_EditText.getText().toString();
				String turnright = turnright_EditText.getText().toString();
				String djleft = djcenter_EditText.getText().toString();
				String djcenter = djleft_EditText.getText().toString();
				String djright = djright_EditText.getText().toString();
				String stop = stop_EditText.getText().toString();

                //获取 Preferences用于储存信息					
				SharedPreferences  share = SettingActivity.this.getSharedPreferences("perference", MODE_PRIVATE);
				
				//取得编辑器
				Editor editor = share.edit();
				
				//存储配置 参数1 是key 参数2 是值 数据存入 Preferences
				editor.putString("videoip", videoip);
				editor.putString("controlip", controlip);
				editor.putString("forword", forword);
				editor.putString("back", back);
				editor.putString("turnleft", turnleft);
				editor.putString("turnright", turnright);
				editor.putString("djleft", djleft);
				editor.putString("djcenter", djcenter);
				editor.putString("djright", djright);
				editor.putString("stop", stop);
				//提交刷新数据
				editor.commit();
				
				//提示保存是否
				Toast.makeText(SettingActivity.this,"保存成功",100).show();
			}
        	
        });
	    cancle_btnButton=(Button)findViewById(R.id.cancleBtn);
	    cancle_btnButton.setOnClickListener( new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub	
				closethis();
			}
		});
	
	}
	
    private void closethis()
{
   this.finish();
   overridePendingTransition(R.anim.up_in, R.anim.up_out); 	
}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_setting, menu);
		return true;
	}

}
