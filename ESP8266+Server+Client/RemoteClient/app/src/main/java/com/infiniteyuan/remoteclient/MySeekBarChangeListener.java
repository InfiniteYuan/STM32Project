package com.infiniteyuan.remoteclient;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.SeekBar;
import android.widget.Toast;

import com.infiniteyuan.Entity.Motor;

/**
 * Created by Infin on 2016/5/22.
 */
public class MySeekBarChangeListener implements SeekBar.OnSeekBarChangeListener {

    private int mMotorNum;
    private Motor mMotor;
    private ToServer mToServer;
    private String deviceName;
    private Context mContext;
    private byte motor;

    public MySeekBarChangeListener(Motor motor, int motorNum, ToServer toServer, Context context) {
        mMotorNum = motorNum;
        mMotor = motor;
        mToServer = toServer;
        deviceName = "1:2:3:4:5:a";
        mContext = context;
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        motor = mMotor.Motor[mMotorNum - 1];
        mMotor.Motor[mMotorNum - 1] = (byte) (seekBar.getProgress() + 10);
        new Thread(ControlMotorRunnable).start();//控制线程
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        Toast.makeText(mContext, "开始控制!", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        new Thread(QueryMotorThread).start();//查询线程
//        motor = mMotor.Motor[mMotorNum - 1];
//        mMotor.Motor[mMotorNum - 1] = (byte) (seekBar.getProgress() + 10);
//        new Thread(ControlMotorRunnable).start();//控制线程
        Toast.makeText(mContext, "停止操作!" + (byte) seekBar.getProgress(), Toast.LENGTH_SHORT).show();
    }

    Runnable ControlMotorRunnable = new Runnable() {
        @Override
        public void run() {
            Message message = new Message();
            Bundle bundle = new Bundle();
            bundle.putBoolean("Result", mToServer.ControlMotor(deviceName, mMotor));
            message.setData(bundle);
            ControlMotorHandler.sendMessage(message);
        }
    };

    Handler ControlMotorHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Bundle bundle = new Bundle();
            boolean result = bundle.getBoolean("Result");
            if (result) {
                Toast.makeText(mContext, "控制操作成功", Toast.LENGTH_SHORT).show();
            } else {
                mMotor.Motor[mMotorNum - 1] = motor;
                Toast.makeText(mContext, "控制操作失败", Toast.LENGTH_SHORT).show();
            }
        }
    };

    Runnable QueryMotorThread = new Runnable() {
        @Override
        public void run() {
            Message message = new Message();
            Bundle bundle = new Bundle();
            bundle.putBoolean("Result", mToServer.QueryMotor(deviceName, mMotor));
            message.setData(bundle);
            QueryMotorHandler.sendMessage(message);
        }
    };

    Handler QueryMotorHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Bundle bundle = new Bundle();
            boolean result = bundle.getBoolean("Result");
            if (result) {
                Toast.makeText(mContext, "查询操作成功", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(mContext, "查询操作失败", Toast.LENGTH_SHORT).show();
            }
        }
    };

}
