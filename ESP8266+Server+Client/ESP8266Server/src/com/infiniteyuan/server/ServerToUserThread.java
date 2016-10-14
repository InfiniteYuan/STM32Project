package com.infiniteyuan.server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.util.ArrayList;

import com.infiniteyuan.communication.ToMotor;
import com.infiniteyuan.communication.ToUser;
import com.infiniteyuan.device.Motor;
import com.infiniteyuan.tool.Date_TimeStamp;
import com.infiniteyuan.tool.StringRelated;

public class ServerToUserThread extends Thread {

	protected Socket mSocket = null;
	protected ArrayList<ServerToDeviceThread> mServerToDeviceThreads = null;
	private DataInputStream mDataInputStream = null;
	private DataOutputStream mDataOutputStream = null;
	private ToUser mToUser = null;
	private byte[] mReadData = new byte[2048];

	public ServerToUserThread(ArrayList<ServerToDeviceThread> mScocketList, Socket socket) {
		mServerToDeviceThreads = mScocketList;
		mSocket = socket;
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		super.run();
		long startTime = Date_TimeStamp.timeStamp();
		if (!getInOutStream()) {
			System.out.println("ServerToUserThread 打开输入输出流失败");
			Close();
			return;
		}
		try {
			mSocket.setSoTimeout(20000);
			int datasize = mDataInputStream.read(mReadData);
			if (datasize > 0) {
				if (!mToUser.VerifyFrame(mReadData)) {
					System.out.println("ServerToUserThread 移动终端数据包错误！");
					Close();
					return;
				}
				String device = StringRelated.Byte6ToMac(mReadData, 13);

				// System.out.println(device);

				ToMotor toMotor = null;
				for (int i = 0; i < mServerToDeviceThreads.size(); i++) {
					if (!mServerToDeviceThreads.get(i).IsAlive()) {
						mServerToDeviceThreads.remove(i);
					} else {
						if (mServerToDeviceThreads.get(i).device.equals(device)) {
							toMotor = mServerToDeviceThreads.get(i).mToMotor;
							break;
						}
					}
				}
				if (toMotor == null) {
					System.out.println("ServerToUserThread 移动终端设备已经掉线!");
					Close();
					return;
				}
				Motor motor = new Motor();
				if (mReadData[4] == 0x01) {// 控制请求
					System.out.println("ServerToUserThread 移动终端控制请求!!!");
					int type = ((int) mReadData[2] << 8 & 0xff00) | ((int) mReadData[3] & 0xff);
					switch (type) {
					case Motor.DEVICE_TYPE:
						int timeout = mSocket.getSoTimeout();
						mSocket.setSoTimeout(30000);
						motor.mMotor[0] = (byte) mReadData[19];
						motor.mMotor[1] = (byte) mReadData[20];
						motor.mMotor[2] = (byte) mReadData[21];
						motor.mMotor[3] = (byte) mReadData[22];
						motor.mMotor[4] = (byte) mReadData[23];
						motor.mMotor[5] = (byte) mReadData[24];
						System.out.println("从移动中断收到的数据!");
						StringRelated.PrintBytes(mReadData, 29);
						if (!mToUser.ControlMotor(toMotor, device, motor)) {
							System.out.println("ServerToUserThread 移动终端控制失败!!");
						} else {
							System.out.println("ServerToUserThread 移动终端控制成功!!");
						}
						mSocket.setSoTimeout(timeout);
						break;
					default:
						System.out.println("ServerToUserThread 移动终端意外错误!");
						break;
					}
				} else if (mReadData[4] == 0x02) {// 信息响应
					System.out.println("ServerToUserThread 移动终端响应消息!!!");
				} else if (mReadData[4] == 0x03) {// 信息询问
					System.out.println("ServerToUserThread 移动终端询问请求!!!");
					int type = ((int) mReadData[2] << 8 & 0xff00) | ((int) mReadData[3] & 0xff);
					switch (type) {
					case Motor.DEVICE_TYPE:
						int timeout = mSocket.getSoTimeout();
						mSocket.setSoTimeout(30000);
						if (!mToUser.QueryMotor(toMotor, device, motor)) {
							System.out.println("ServerToUserThread 移动终端查询失败!!");
						} else {
							System.out.println("ServerToUserThread 移动终端查询成功!!");
						}
						mSocket.setSoTimeout(timeout);
						break;
					default:
						System.out.println("ServerToUserThread 移动终端意外错误!");
						break;
					}
				}
			}
			mDataOutputStream.flush();// 清空发送缓冲区
			Thread.sleep(2000);
			Close();
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SocketTimeoutException e) {
			System.out.println("Socket 超时!");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public boolean getInOutStream() {
		try {
			mDataInputStream = new DataInputStream(mSocket.getInputStream());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("打开失败");
			return false;
		}
		try {
			mDataOutputStream = new DataOutputStream(mSocket.getOutputStream());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("打开失败");
			return false;
		}
		mToUser = new ToUser(mDataInputStream, mDataOutputStream);
		return true;
	}

	public void Close() {
		try {
			if (mDataInputStream != null)
				mDataInputStream.close();
			if (mDataOutputStream != null)
				mDataOutputStream.close();
			if (mSocket != null)
				mSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			System.out.println("ServerToUserThread Abnormal , Socket : 关闭socket资源异常!!!");
			e.printStackTrace();
		}
	}
}
