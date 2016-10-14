package com.infiniteyuan.server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

public class ServerHost {

	private static ArrayList<ServerToDeviceThread> mSocketList = new ArrayList<ServerToDeviceThread>();// 作为参数
																										// 作为地址引用
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		new Thread() {
			@Override
			public void run() {
				ServerSocket serverSocket;
				try {
					serverSocket = new ServerSocket(8090);
					System.out.println(serverSocket.getLocalSocketAddress());
					while (true) {
						Socket ToDeviceSocket = serverSocket.accept();
						ServerToDeviceThread serverToDeviceThread = new ServerToDeviceThread(ToDeviceSocket);
						System.out.println(ToDeviceSocket.getRemoteSocketAddress());
						serverToDeviceThread.start();
						mSocketList.add(serverToDeviceThread);
						System.out.println("有设备连入！");
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}.start();// 设备链接

		new Thread() {
			@Override
			public void run() {
				ServerSocket serverSocket;
				try {
					serverSocket = new ServerSocket(8099);
					System.out.println(serverSocket.getLocalSocketAddress());
					while (true) {
						Socket ToUserSocket = serverSocket.accept();
						ServerToUserThread serverToUserThread = new ServerToUserThread(mSocketList, ToUserSocket);
						serverToUserThread.start();
						DeleteOfflineDevices();
						// System.out.println(ToUserSocket.getRemoteSocketAddress());
						// System.out.println("有用户连入！");
					}
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
				}
			}
		}.start();// 用户链接
	}

	public static void DeleteOfflineDevices() {
		for (int i = 0; i < mSocketList.size(); ++i) {
			if (!mSocketList.get(i).IsAlive())
				mSocketList.remove(i);
		}
	}

	public static ArrayList<ServerToDeviceThread> getSocketList() {

		return mSocketList;
	}

}
