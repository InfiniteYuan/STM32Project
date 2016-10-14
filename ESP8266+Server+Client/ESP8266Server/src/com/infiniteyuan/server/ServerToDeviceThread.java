package com.infiniteyuan.server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import com.infiniteyuan.communication.ToMotor;
import com.infiniteyuan.tool.Date_TimeStamp;
import com.infiniteyuan.tool.StringRelated;

public class ServerToDeviceThread extends Thread {

	protected Socket socket = null;
	protected DataInputStream mDataInputStream = null;
	protected DataOutputStream mDataOutputStream = null;
	public ToMotor mToMotor = null;
	public String device = null;

	public ServerToDeviceThread(Socket socket) {
		this.socket = socket;
		device = "1:2:3:4:5:a";
	}

	public void run() {

		long KeepAliveTime = Date_TimeStamp.timeStamp();
		if (!getIn_Out_stream()) {
			System.out.println("ServerToDeviceThread 打开输入输出字节流失败！");
			Close();
			return;
		} else {
			System.out.println("ServerToDeviceThread 打开输入输出字节流成功！");
		}
		while (true) {
			// try {
			// byte data[] = new byte[50];
			// while (mDataInputStream.read(data) != 0) {
			// System.out.println(data.length);
			// PrintBytes(data,data.length);
			// if (!socket.isClosed()) {
			// mDataOutputStream.write(data);
			// }
			// }
			// } catch (IOException e) {
			// // TODO Auto-generated catch block
			// System.out.println("错误!!");
			// Close();
			// e.printStackTrace();
			// }

			// System.out.println(Date_TimeStamp.timeStamp());
			// 心跳保持，五分钟一次
			if (Date_TimeStamp.timeStamp() - KeepAliveTime >= 10) {
				KeepAliveTime = Date_TimeStamp.timeStamp();
				if (!mToMotor.KeepAlive(device)) {
					System.out.println("ServerToDeviceThread 心跳保持失败");
					if (!mToMotor.KeepAlive(device)) {
						System.out.println("ServerToDeviceThread 第二次心跳保持失败!!失去连接!!!");
						Close();
						break;
					}
				}
				System.out.println("ServerToDeviceThread 链路保持成功!!!!");
			}
		}

	}

	private boolean getIn_Out_stream() {
		try {
			mDataInputStream = new DataInputStream(socket.getInputStream());
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
			System.out.println("打开失败");
			return false;
		}
		try {
			mDataOutputStream = new DataOutputStream(socket.getOutputStream());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("打开失败");
			return false;
		}
		mToMotor = new ToMotor(mDataInputStream, mDataOutputStream);
		return true;
	}

	protected void Close() {
		try {
			if (mDataOutputStream != null)
				mDataOutputStream.close();
			if (mDataInputStream != null)
				mDataInputStream.close();
			if (socket != null)
				socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			System.out.println("ServerToDeviceThread Abnormal , Socket : 关闭socket资源异常!!!");
			e.printStackTrace();
		}
	}

	public static byte[] charToByte(char[] c) {
		byte[] b = new byte[c.length];
		for (int i = 0; i < c.length; ++i) {
			b[i] = (byte) (c[i] & 0x00ff);
		}
		return b;
	}

	public boolean IsAlive() {
		return !socket.isClosed();
	}

	public static void PrintBytes(byte[] c, int size) {
		for (int i = 0; i < size; ++i)
			System.out.print(Integer.toHexString(c[i] & 0xff) + " ");
		System.out.println();
	}

}
