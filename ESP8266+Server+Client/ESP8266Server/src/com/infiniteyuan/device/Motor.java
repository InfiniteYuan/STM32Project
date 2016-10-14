package com.infiniteyuan.device;

public class Motor {

	static public final short DEVICE_TYPE = 0x0001;

	public byte mMotor[] = new byte[6];

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return Integer.toHexString(mMotor[0] & 0xff) + " " + Integer.toHexString(mMotor[1] & 0xff) + " "
				+ Integer.toHexString(mMotor[2] & 0xff) + " " + Integer.toHexString(mMotor[3] & 0xff) + " "
				+ Integer.toHexString(mMotor[4] & 0xff) + " " + Integer.toHexString(mMotor[5] & 0xff);
	}

}
