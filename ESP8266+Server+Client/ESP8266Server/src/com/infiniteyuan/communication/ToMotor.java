package com.infiniteyuan.communication;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import com.infiniteyuan.device.Motor;
import com.infiniteyuan.tool.CRC;
import com.infiniteyuan.tool.StringRelated;

public class ToMotor {

	private DataInputStream mDataInputStream = null;
	private DataOutputStream mDataOutputStream = null;

	public ToMotor(DataInputStream dataInputStream, DataOutputStream dataOutputStream) {
		// TODO Auto-generated constructor stub
		mDataInputStream = dataInputStream;
		mDataOutputStream = dataOutputStream;
	}

	public boolean KeepAlive(String device) {
		byte[] data = new byte[50];
		byte[] deviceNumber = StringRelated.MacToBytes(device);
		data[0] = (byte) 0xab;
		data[1] = (byte) 0xac;
		data[2] = (byte) 0x00;
		data[3] = (byte) 0x10;
		data[4] = (byte) 0x01;
		System.arraycopy(deviceNumber, 0, data, 5, 6);
		data[17] = (byte) 0x00;
		data[18] = (byte) 0x01;
		data[19] = (byte) 0x01;
		long crc16 = CRC.CRC16Calculate(data, 20);
		data[20] = (byte) (crc16 >> 8 & 0xff);
		data[21] = (byte) (crc16 & 0xff);
		try {
			mDataOutputStream.write(data, 0, 22);
			int size = mDataInputStream.read(data);
			if (size > 0) {
				if (!VerifyFrame(data))
					return false;
				// 校验成功
				if (data[4] != 0x02) // 不是响应链路保持
					return false;
				if (data[19] == 0x01) {// 设备

				} else if (data[20] == 0x02) {// 手机

				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return true;
	}

	public boolean ControlMotor(String device, Motor motor) {
		System.out.println("ToMotor进入控制函数");
		byte[] data = new byte[50];
		byte[] deviceNumber = StringRelated.MacToBytes(device);
		data[0] = (byte) 0xab;
		data[1] = (byte) 0xac;// 帧头
		data[2] = (byte) 0x00;
		data[3] = (byte) 0x01;// 舵机 操作设备 数据类型
		data[4] = (byte) 0x01;// 操作类型 控制请求
		System.arraycopy(deviceNumber, 0, data, 5, 6);
		data[17] = (byte) 0x00;
		data[18] = (byte) 0x08;// 信息长度
		data[19] = (byte) motor.mMotor[0];
		data[20] = (byte) motor.mMotor[1];
		data[21] = (byte) motor.mMotor[2];
		data[22] = (byte) motor.mMotor[3];
		data[23] = (byte) motor.mMotor[4];
		data[24] = (byte) motor.mMotor[5];
		data[25] = (byte) 0x01;
		data[26] = (byte) 0x01;// 信息
		long crc16 = CRC.CRC16Calculate(data, 27);
		data[27] = (byte) (crc16 >> 8 & 0xff);
		data[28] = (byte) (crc16 & 0xff);
		try {
			StringRelated.PrintBytes(data, 29);
			// StringRelated.PrintBytes(motor.mMotor, 6);
			mDataOutputStream.write(data, 0, 29);
			int size = mDataInputStream.read(data);
			if (size > 0) {
				if (!VerifyFrame(data)) {
					System.out.println("ToMotor退出控制函数  失败1");
					return false;
				}
				if (data[4] != 0x02 || data[26] != 0x01) {
					System.out.println("ToMotor退出控制函数  失败2");
					return false;
				}
				if ((data[19] == motor.mMotor[0]) && (data[20] == motor.mMotor[1]) && (data[21] == motor.mMotor[2])
						&& (data[22] == motor.mMotor[3]) && (data[23] == motor.mMotor[4])
						&& (data[24] == motor.mMotor[5])) {
					return true;
				} else {
					System.out.println(data[19]+" "+ motor.mMotor[0]);
					StringRelated.PrintBytes(data, 29);
					System.out.println("ToMotor退出控制函数  失败4 数据不一致!");
					return false;
				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			System.out.println("ToMotor退出控制函数  失败3");
			e.printStackTrace();
			return false;
		}
		// System.out.println("ToMotor退出控制函数 成功");
		return true;
	}

	public boolean QueryMotor(String device, Motor motor) {
		byte[] data = new byte[50];
		byte[] deviceNumber = StringRelated.MacToBytes(device);
		data[0] = (byte) 0xab;
		data[1] = (byte) 0xac;
		data[2] = (byte) 0x00;
		data[3] = (byte) 0x01;
		data[4] = (byte) 0x03;// 信息询问
		System.arraycopy(deviceNumber, 0, data, 5, 6);
		data[17] = (byte) 0x00;
		data[18] = (byte) 0x08;// 信息长度
		data[19] = (byte) motor.mMotor[0];
		data[20] = (byte) motor.mMotor[1];
		data[21] = (byte) motor.mMotor[2];
		data[22] = (byte) motor.mMotor[3];
		data[23] = (byte) motor.mMotor[4];
		data[24] = (byte) motor.mMotor[5];
		data[25] = (byte) 0x01;
		data[26] = (byte) 0x01;// 信息
		long crc16 = CRC.CRC16Calculate(data, 27);
		data[27] = (byte) (crc16 >> 8 & 0xff);
		data[28] = (byte) (crc16 & 0xff);
		try {
			mDataOutputStream.write(data, 0, 29);
			int size = mDataInputStream.read(data);
			if (size > 0) {
				if (!VerifyFrame(data)) // 验证数据
					return false;
				if (data[4] != 0x02 || data[26] != 0x01) // 不是响应信息，不是舵机信息
					return false;
				motor.mMotor[0] = (byte) data[19];
				motor.mMotor[1] = (byte) data[20];
				motor.mMotor[2] = (byte) data[21];
				motor.mMotor[3] = (byte) data[22];
				motor.mMotor[4] = (byte) data[23];
				motor.mMotor[5] = (byte) data[24];
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return false;
		}
		return true;
	}

	private boolean VerifyFrame(byte[] data) {
		// 帧头
		if (((short) data[0] & 0xff) != 0xab || ((short) data[1] & 0xff) != 0xac)
			return false;
		int datalength = (short) data[17] << 8 | data[18];
		// CRC校验
		int parity = CRC.CRC16Calculate(data, datalength + 19);
		int parity2 = (data[19 + datalength] & 0xff) << 8 | (data[20 + datalength] & 0xff);
		if (parity != parity2)
			return false;
		return true;
	}
}
