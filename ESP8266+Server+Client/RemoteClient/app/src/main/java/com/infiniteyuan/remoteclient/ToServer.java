package com.infiniteyuan.remoteclient;

import com.infiniteyuan.Entity.Motor;
import com.infiniteyuan.Entity.ServerInfo;
import com.infiniteyuan.tool.CRC;
import com.infiniteyuan.tool.StringRelated;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * Created by Infin on 2016/5/22.
 */
public class ToServer {

    private boolean SendRequest(Short type, byte operationType, Short dataLength, RequestData data) {
        byte[] DataToWrite = new byte[15 + dataLength];
        DataToWrite[0] = (byte) 0xab;
        DataToWrite[1] = (byte) 0xac;
        DataToWrite[2] = (byte) (type >> 8 & 0xff);//假设舍弃高8位
        DataToWrite[3] = (byte) (type & 0xff);
        DataToWrite[4] = operationType;
        DataToWrite[11] = (byte) 0x00;
        DataToWrite[12] = (byte) 0x0e;
        System.arraycopy(data.mData, 0, DataToWrite, 13, dataLength);
        long Crc16 = CRC.CRC16Calculate(DataToWrite, dataLength + 13);
        DataToWrite[13 + dataLength] = (byte) (Crc16 >> 8 & 0xff);
        DataToWrite[14 + dataLength] = (byte) (Crc16 & 0xff);
        try {
            Socket socket = new Socket(ServerInfo.mAddress, ServerInfo.mPort);
            socket.setSoTimeout(20000);
            DataInputStream dataInputStream = new DataInputStream(socket.getInputStream());
            DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
            dataOutputStream.write(DataToWrite, 0, 15 + dataLength);
            int size = 0;
            size = dataInputStream.read(DataToWrite);
            if (size <= 0) {
                return false;
            }
            if (!VerifyFrame(DataToWrite)) {
                return false;
            }
            System.arraycopy(DataToWrite, 13, data.mData, 0, dataLength);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }

    protected boolean ControlMotor(String deviceName, Motor motor) {
        RequestData requestData = new RequestData();
        requestData.mData = new byte[14];
        byte[] deviceNameByte = StringRelated.MacToBytes(deviceName);
        System.arraycopy(deviceNameByte, 0, requestData.mData, 0, 6);
        requestData.mData[6] = motor.Motor[0];
        requestData.mData[7] = motor.Motor[1];
        requestData.mData[8] = motor.Motor[2];
        requestData.mData[9] = motor.Motor[3];
        requestData.mData[10] = motor.Motor[4];
        requestData.mData[11] = motor.Motor[5];
        requestData.mData[12] = (byte) 0x00;
        requestData.mData[13] = (byte) 0x00;
        if (!SendRequest((short) 0x0001, (byte) 0x01, (short) 14, requestData))
            return false;
        return true;
    }

    public boolean QueryMotor(String deviceName, Motor motor) {
        RequestData requestData = new RequestData();
        requestData.mData = new byte[14];
        byte[] deviceNameByte = StringRelated.MacToBytes(deviceName);
        System.arraycopy(deviceNameByte, 0, requestData.mData, 0, 6);
        requestData.mData[6] = motor.Motor[0];
        requestData.mData[7] = motor.Motor[1];
        requestData.mData[8] = motor.Motor[2];
        requestData.mData[9] = motor.Motor[3];
        requestData.mData[10] = motor.Motor[4];
        requestData.mData[11] = motor.Motor[5];
        requestData.mData[12] = (byte) 0x00;
        requestData.mData[13] = (byte) 0x00;
        if (!SendRequest((short) 0x0001, (byte) 0x03, (short) 14, requestData))
            return false;
        else {
            motor.Motor[0] = requestData.mData[6];
            motor.Motor[1] = requestData.mData[7];
            motor.Motor[2] = requestData.mData[8];
            motor.Motor[3] = requestData.mData[9];
            motor.Motor[4] = requestData.mData[10];
            motor.Motor[5] = requestData.mData[11];
        }
        return true;
    }

    private boolean VerifyFrame(byte[] dataToRead) {
        if (dataToRead.length < 15)
            return false;
        //帧头
        if (((short) dataToRead[0] & 0xff) != 0xab || ((short) dataToRead[1] & 0xff) != 0xac)
            return false;
        int datalength = (short) dataToRead[11] << 8 | dataToRead[12];
        //CRC校验
        int parity = CRC.CRC16Calculate(dataToRead, datalength + 13);
        int parity2 = ((int) (dataToRead[13 + datalength] << 8 | dataToRead[14 + datalength] & 0xff) & 0xffff);
        if (parity != parity2)
            return false;
        return true;
    }

    class RequestData {
        public byte[] mData;
    }
}
