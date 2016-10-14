package com.infiniteyuan.communication;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import com.infiniteyuan.device.Motor;
import com.infiniteyuan.tool.CRC;
import com.infiniteyuan.tool.StringRelated;

public class ToUser {

	private DataInputStream mDataInputStream = null;
	private DataOutputStream mDataOutputStream = null;

	public ToUser(DataInputStream mDataInputStream, DataOutputStream mDataOutputStream) {
		super();
		this.mDataInputStream = mDataInputStream;
		this.mDataOutputStream = mDataOutputStream;
	}

	/**
	 * @param datatype
	 *            设备类型
	 * @param operationType
	 *            操作类型
	 * @param dataLength
	 * @param data
	 * @return
	 */
	private boolean SendToUser(Short datatype, byte operationType, Short dataLength, byte[] data) {
		byte[] dataToWrite = new byte[15 + dataLength];
		// byte[] sessionBytes ;
		dataToWrite[0] = (byte) 0xab;
		dataToWrite[1] = (byte) 0xac;
		dataToWrite[2] = (byte) (datatype >> 8 & 0xff);
		dataToWrite[3] = (byte) (datatype & 0xff);
		dataToWrite[4] = (byte) operationType;
		dataToWrite[11] = (byte) (dataLength >> 8 & 0xff);
		dataToWrite[12] = (byte) (dataLength & 0xff);
		System.arraycopy(data, 0, dataToWrite, 13, dataLength);
		long crc16 = CRC.CRC16Calculate(dataToWrite, dataLength + 13);
		dataToWrite[13 + dataLength] = (byte) (crc16 >> 8 & 0xff);
		dataToWrite[14 + dataLength] = (byte) (crc16 & 0xff);
		try {
			mDataOutputStream.write(dataToWrite, 0, 15 + dataLength);
			mDataOutputStream.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return false;
		}
		return true;
	}

	public boolean ControlMotor(ToMotor toMotor, String device, Motor motor) {
		System.out.println("ToUser进入控制函数");
		if (!toMotor.ControlMotor(device, motor))
			return false;
		byte deviceBytes[] = StringRelated.MacToBytes(device);
		byte[] data = { 0, 0, 0, 0, 0, 0, (byte) motor.mMotor[0], (byte) motor.mMotor[1],
				(byte) motor.mMotor[2], (byte) motor.mMotor[3], (byte) motor.mMotor[4], (byte) motor.mMotor[5], 0, 0 };
		System.arraycopy(deviceBytes, 0, data, 0, 6);
		
		StringRelated.PrintBytes(motor.mMotor, 6);
		System.out.println("ToUser退出控制函数");
		return SendToUser((short) 0x0001, (byte) 2, (short) 14, data);
	}

	public boolean QueryMotor(ToMotor toMotor, String device, Motor motor) {
		boolean result = false;
		result = toMotor.QueryMotor(device, motor);
		if (!result)
			return false;
		byte deviceBytes[] = StringRelated.MacToBytes(device);
		byte[] data = { 0, 0, 0, 0, 0, 0, (byte) motor.mMotor[0], (byte) motor.mMotor[1], // 设备号（6位）、名称（4位）状态（6位）、备注长度（2位）、备注（n位）
				(byte) motor.mMotor[2], (byte) motor.mMotor[3], (byte) motor.mMotor[4], (byte) motor.mMotor[5], 0, 0 };
		System.arraycopy(deviceBytes, 0, data, 0, 6);
		
		StringRelated.PrintBytes(motor.mMotor, 6);
		
		return SendToUser((short) 0x0003, (byte) 2, (short) 14, data);
	}

	public boolean VerifyFrame(byte[] dataToRead) {
		if (dataToRead.length < 15)
			return false;
		// 帧头
		if (((short) dataToRead[0] & 0xff) != 0xab || ((short) dataToRead[1] & 0xff) != 0xac)
			return false;
		int datalength = (short) dataToRead[11] << 8 | dataToRead[12];
		// CRC校验
		int parity = CRC.CRC16Calculate(dataToRead, datalength + 13);
		int parity2 = (dataToRead[13 + datalength] << 8 | dataToRead[14 + datalength] & 0xff) & 0xffff;
		if (parity != parity2)
			return false;
		return true;
	}

}
