using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SerialControl;
using System.IO;
using System.Threading;

namespace _3DGimbal
{
    public partial class MainForm : Form
    {
        SerialCom com;
        bool _switch;
        byte prechecksum;
        byte packetnum;
        int sendindex;
        int filelength;
        private byte[] binContent = new byte[10 * 1024];
        /// <summary>
        /// 左侧连接按钮是否被点击
        /// </summary>
        private bool isConnectClick = false;
        /// <summary>
        /// 左侧参数按钮是否被点击
        /// </summary>
        private bool isDataClick = false;
        /// <summary>
        /// 鼠标移动位置变量
        /// </summary>
        private Point mouseOff;
        /// <summary>
        /// 标签是否为左键
        /// </summary>
        private bool leftFlag;

        public MainForm()
        {
            InitializeComponent();
            com = new SerialCom();
            com.Visible = false;
            _switch = false;
            packetnum = 1;
            this.Controls.Add(com);
            pictureBox2.SendToBack();
            new Thread(image).Start();
            seconddatapanel.Visible = false;
            textBox36.Text = "7.0";
            textBox37.Text = "0.0";
            textBox38.Text = "0.0";
            textBox39.Text = "5.0";
            textBox40.Text = "0.0";
            textBox41.Text = "0.1";
            tbMotorVel.Text = "0";
            label5.Text = "请按照打开文件、下载程序、开始程序进行操作!";
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void image()
        {
            int X = 0;
            int X2 = 0;
            int Y = pictureBox2.Location.Y;
            Random radom = new Random();
            while (true)
            {
                X += 5;
                X2 += 10;
                //X += radom.Next(20);
                //Y += radom.Next(20);
                if (X > 680)
                {
                    X = radom.Next(400);
                }
                if (X2 > 680)
                {
                    X2 = 0;
                }
                if (Y > 500)
                {
                    Y = radom.Next(200);
                }
                Point _mouseOff = new Point(X, Y);
                Point _mouse = new Point(X2,Y);
                if (pictureBox2.InvokeRequired)
                {
                    Action<Point> actionDelegate = (x) => { this.pictureBox2.Location = x; };
                    this.pictureBox2.Invoke(actionDelegate, _mouseOff);
                }
                else
                {
                    pictureBox2.Location = _mouseOff;
                }
                if (pictureBox3.InvokeRequired)
                {
                    Action<Point> actionDelegate = (x) => { this.pictureBox3.Location = x; };
                    this.pictureBox3.Invoke(actionDelegate, _mouse);
                }
                else
                {
                    pictureBox3.Location = _mouse;
                }
                Thread.Sleep(30);
            }
        }

        private void timCheckFrame_Tick(object sender, EventArgs e)
        {
            if (com.CheckFrame())
            {
                switch (com.rxFrame.fnCode)
                {
                    case FunctionCode.FC_DEV_STA:
                        //txtMotorCurVel.Text = (BitConverter.ToSingle(com.rxFrame.data, 0)).ToString("f3");
                        textBox1.Text = com.rxFrame.data[0] == 1 ? "在" : "不在";
                        textBox2.Text = com.rxFrame.data[1] == 1 ? "在" : "不在";
                        textBox3.Text = com.rxFrame.data[2] == 1 ? "在" : "不在";
                        break;
                    case FunctionCode.FC_IMU_RAW:
                        textBox4.Text = (BitConverter.ToSingle(com.rxFrame.data, 0)).ToString();
                        textBox5.Text = (BitConverter.ToSingle(com.rxFrame.data, 4)).ToString();
                        textBox6.Text = (BitConverter.ToSingle(com.rxFrame.data, 8)).ToString();
                        textBox7.Text = (BitConverter.ToSingle(com.rxFrame.data, 12)).ToString();
                        textBox8.Text = (BitConverter.ToSingle(com.rxFrame.data, 16)).ToString();
                        textBox9.Text = (BitConverter.ToSingle(com.rxFrame.data, 20)).ToString();
                        textBox10.Text = (BitConverter.ToSingle(com.rxFrame.data, 24)).ToString();
                        textBox11.Text = (BitConverter.ToSingle(com.rxFrame.data, 28)).ToString();
                        textBox12.Text = (BitConverter.ToSingle(com.rxFrame.data, 32)).ToString();
                        break;
                    case FunctionCode.FC_GPS_RAW:
                        textBox13.Text = com.rxFrame.data[0].ToString();
                        textBox14.Text = (BitConverter.ToDouble(com.rxFrame.data, 1)).ToString();
                        textBox15.Text = (BitConverter.ToDouble(com.rxFrame.data, 9)).ToString();
                        textBox16.Text = (BitConverter.ToDouble(com.rxFrame.data, 17)).ToString();
                        textBox17.Text = (BitConverter.ToDouble(com.rxFrame.data, 25)).ToString();
                        textBox18.Text = (BitConverter.ToDouble(com.rxFrame.data, 33)).ToString();
                        break;
                    case FunctionCode.FC_RC_RAW:
                        textBox19.Text = (BitConverter.ToSingle(com.rxFrame.data, 0)).ToString();
                        textBox20.Text = (BitConverter.ToSingle(com.rxFrame.data, 4)).ToString();
                        textBox21.Text = (BitConverter.ToSingle(com.rxFrame.data, 8)).ToString();
                        textBox22.Text = (BitConverter.ToSingle(com.rxFrame.data, 12)).ToString();
                        break;
                    case FunctionCode.FC_CIRCLE_CONTROL:
                        textBox23.Text = (BitConverter.ToSingle(com.rxFrame.data, 0) * 57.296).ToString();
                        textBox24.Text = (BitConverter.ToSingle(com.rxFrame.data, 4) * 57.296).ToString();
                        textBox25.Text = (BitConverter.ToSingle(com.rxFrame.data, 8) * 57.296).ToString();
                        textBox26.Text = (BitConverter.ToSingle(com.rxFrame.data, 12)).ToString();
                        textBox27.Text = (BitConverter.ToSingle(com.rxFrame.data, 16)).ToString();
                        textBox28.Text = (BitConverter.ToSingle(com.rxFrame.data, 20)).ToString();
                        textBox29.Text = (BitConverter.ToSingle(com.rxFrame.data, 24)).ToString();
                        textBox30.Text = (BitConverter.ToSingle(com.rxFrame.data, 28)).ToString();
                        textBox31.Text = (BitConverter.ToSingle(com.rxFrame.data, 32)).ToString();
                        textBox32.Text = (BitConverter.ToSingle(com.rxFrame.data, 36)).ToString();
                        textBox33.Text = com.rxFrame.data[40].ToString();
                        break;
                    case FunctionCode.FC_IAP_UPDATE:
                        com.txFrame.fnCode = FunctionCode.FC_IAP_UPDATE;
                        if (1 == com.rxFrame.data[0])
                        {
                            if (filelength != 0)
                            {
                                byte length = (byte)((filelength >= 50 - 1) ? 50 - 2 : filelength);
                                com.txFrame.data[0] = length;
                                com.txFrame.data[1] = packetnum++;
                                for (int i = 0; i < length; i++)
                                {
                                    com.txFrame.data[i + 2] = binContent[sendindex++];
                                }
                                filelength -= length;
                                label5.Text = "剩余" + filelength + "字节!";
                                com.txFrame.isUpdated = true;
                                com.SendFrame();
                                prechecksum = com.txFrame.checkSum;
                            }
                            else
                            {

                            }
                        }
                        else
                        {
                            if (filelength != 0)
                            {
                                com.txFrame.fnCode = FunctionCode.FC_IAP_UPDATE;
                                com.txFrame.isUpdated = true;
                                com.SendFrame();
                            }
                        }
                        break;
                }
            }



        }

        private int getInfoCount = 0;
        private void timGetInfo_Tick(object sender, EventArgs e)
        {

            /*com.txFrame.isUpdated = true;
            switch (getInfoCount++)
            {
                case 0:
                case 2:
                case 4:
                case 6:
                case 8:
                case 10:
                case 12:
                case 14:
                case 16:
                case 18:
                case 20:
                    com.txFrame.fnCode = FunctionCode.FC_GET_IMU_ANG;
                    break;
                case 1:
                    com.txFrame.fnCode = FunctionCode.FC_GET_TAG_ANG;
                    break;
                case 3:
                    com.txFrame.fnCode = FunctionCode.FC_RC_GET_POS;
                    break;
                case 5:
                    com.txFrame.fnCode = FunctionCode.FC_GET_PIT_PID;
                    break;
                case 7:
                    com.txFrame.fnCode = FunctionCode.FC_GET_ROL_PID;
                    break;
                case 9:
                    com.txFrame.fnCode = FunctionCode.FC_GET_YAW_PID;
                    break;
                
                default:
                    com.txFrame.isUpdated = false;
                    break;
            }
            com.SendFrame();
            if (getInfoCount >= 20)
                getInfoCount = 0;*/
        }

        private void tkMotorVel_ValueChanged(object sender, EventArgs e)
        {
            tbMotorVel.Text = ((float)(tkMotorVel.Value) / 100.0f).ToString();
        }

        private void btnSetVelocity_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_DEV_STA;
            //BitConverter.GetBytes((float)tkMotorVel.Value/100.0f).CopyTo(com.txFrame.data, 0);
            com.txFrame.isUpdated = true;
            com.SendFrame();
        }

        private void textBox25_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox24_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox23_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_IMU_RAW;
            //BitConverter.GetBytes((float)tkMotorVel.Value/100.0f).CopyTo(com.txFrame.data, 0);
            com.txFrame.isUpdated = true;
            com.SendFrame();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_GPS_RAW;
            //BitConverter.GetBytes((float)tkMotorVel.Value/100.0f).CopyTo(com.txFrame.data, 0);
            com.txFrame.isUpdated = true;
            com.SendFrame();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_RC_RAW;
            //BitConverter.GetBytes((float)tkMotorVel.Value/100.0f).CopyTo(com.txFrame.data, 0);
            com.txFrame.isUpdated = true;
            com.SendFrame();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_CIRCLE_CONTROL;
            //BitConverter.GetBytes((float)tkMotorVel.Value/100.0f).CopyTo(com.txFrame.data, 0);
            _switch = !_switch;
            com.txFrame.data[0] = (byte)(_switch ? 1 : 0);
            textBox35.Text = _switch ? "开" : "关";
            com.txFrame.isUpdated = true;
            com.SendFrame();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Multiselect = true;
            fileDialog.Title = "请选择文件";
            fileDialog.Filter = "bin文件|*.bin";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                binContent = File.ReadAllBytes(fileDialog.FileName);
                filelength = binContent.Length;
                packetnum = 1;
                textBox34.Text = binContent.Length.ToString();
                label5.Text = "剩余" + filelength + "字节!";
                //MessageBox.Show("已选择文件:" + file, "选择文件提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            int packetlength = filelength/48;
            if (filelength % 48 != 0)
            {
                packetlength += 1;
            }
            com.txFrame.fnCode = FunctionCode.FC_IAP_INFO;
            com.txFrame.data[0] = 0xFF;
            com.txFrame.data[1] = 0xFF;
            com.txFrame.data[2] = (byte)packetlength;
            com.txFrame.isUpdated = true;
            com.SendFrame();

            string StringOut = "";
            for (int i = 0; i < filelength; i++)
            {
                StringOut = StringOut + String.Format("{0:X2} ", binContent[i]);
            }
            textBox43.Text = StringOut;
            textBox42.Text = fileDialog.FileName;
        }

        private void button6_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_IAP_UPDATE;
            sendindex = 0;
            byte length = (byte)((filelength >= 50 - 1) ? 50 - 2 : filelength);
            com.txFrame.data[0] = length;
            com.txFrame.data[1] = packetnum++;
            for (int i = 0; i < length; i++)
            {
                com.txFrame.data[i + 2] = binContent[sendindex++];
            }
            filelength -= length;
            com.txFrame.isUpdated = true;
            com.SendFrame();
            prechecksum = com.txFrame.checkSum;
        }

        private void button7_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_IAP_INFO;
            com.txFrame.data[0] = 0xFF;
            com.txFrame.data[1] = 0xFF;
            com.txFrame.data[2] = 0xFF;
            com.txFrame.isUpdated = true;
            com.SendFrame();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_SET_PIT_PID;
            BitConverter.GetBytes(float.Parse(textBox36.Text)).CopyTo(com.txFrame.data, 0);
            BitConverter.GetBytes(float.Parse(textBox37.Text)).CopyTo(com.txFrame.data, 4);
            BitConverter.GetBytes(float.Parse(textBox38.Text)).CopyTo(com.txFrame.data, 8);
            com.txFrame.isUpdated = true;
            com.SendFrame();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            com.txFrame.fnCode = FunctionCode.FC_SET_RATE_PIT_PID;
            BitConverter.GetBytes(float.Parse(textBox39.Text)).CopyTo(com.txFrame.data, 0);
            BitConverter.GetBytes(float.Parse(textBox40.Text)).CopyTo(com.txFrame.data, 4);
            BitConverter.GetBytes(float.Parse(textBox41.Text)).CopyTo(com.txFrame.data, 8);
            com.txFrame.isUpdated = true;
            com.SendFrame();
        }

        #region 拖动
        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                mouseOff = new Point(-e.X, -e.Y); //得到变量的值
                leftFlag = true;                  //点击左键按下时标注为true;
            }
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (leftFlag)
            {
                Point mouseSet = Control.MousePosition;
                mouseSet.Offset(mouseOff.X - leftpanel.Width, mouseOff.Y);  //设置移动后的位置
                Location = mouseSet;
            }
        }

        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {
            if (leftFlag)
            {
                leftFlag = false;//释放鼠标后标注为false;
            }
        }
        #endregion

        #region 左侧菜单属性值设置
        /// <summary>
        /// 设置鼠标进入面板时字体的颜色
        /// </summary>
        /// <returns></returns>
        private Color setMouseInLabelTextColor()
        {
            return Color.Yellow;
        }
        /// <summary>
        /// 设置左侧菜单字体面板默认的字体颜色
        /// </summary>
        /// <returns></returns>
        private Color setMouseDefaultLabelTextColor()
        {
            return SystemColors.ActiveCaption;
        }
        /// <summary>
        /// 设置鼠标点击面板时字体的颜色
        /// </summary>
        /// <returns></returns>
        private Color setMouseClickLabelTextColor()
        {
            return Color.Pink;
        }
        /// <summary>
        /// 设置左边菜单每一行面板的高度
        /// </summary>
        /// <returns></returns>
        private int getLeftItemPanelHeight()
        {
            return 50;
        }
        /// <summary>
        /// 设置左边菜单参数面板的高度
        /// </summary>
        /// <returns></returns>
        private int getLeftDataPanelHeight()
        {
            return 100;
        }
        #endregion

        #region 菜单点击事件
        /// <summary>
        /// 点击了连接按钮时，显示连接的控件
        /// </summary>
        private void connect_Click_Position()
        {
            if (isDataClick)
            {
                closeDataPanel();
                isDataClick = !isDataClick;
            }
            /*if (isPlaybackWaveClick)
            {
                closePlaybackPanel();
                isPlaybackWaveClick = !isPlaybackWaveClick;
            }
            if (isTechnicalSupportClick)
            {
                closeTechnicalSupportPanel();
                isTechnicalSupportClick = !isTechnicalSupportClick;
            }*/
            isConnectClick = !isConnectClick;
            if (isConnectClick)
            {
                openConnectPanel();
            }
            else
            {
                closeConnectPanel();
            }
        }
        /// <summary>
        /// 连接面板打开时，对界面的布局
        /// </summary>
        private void openConnectPanel()
        {
            int connectTabPanelSizeX = connectTabPanel.Location.X;
            connectTabPanel.Size = new System.Drawing.Size(connectTabPanelSizeX, com.Size.Height + connectTabPanel.Size.Height);
            com.Parent = connectTabPanel;
            com.Location = new Point(connectTabPanel.Margin.Left-2, getLeftItemPanelHeight());
            com.Visible = true;
            Image img = connectPicBox.Image;
            img.RotateFlip(RotateFlipType.Rotate90FlipNone);
            connectPicBox.Image = img;
        }
        /// <summary>
        /// 关闭连接面板，界面的布局发生相应的变化
        /// </summary>
        private void closeConnectPanel()
        {
            int connectTabPanelSizeX = connectTabPanel.Location.X;
            connectTabPanel.Size = new System.Drawing.Size(connectTabPanelSizeX, getLeftItemPanelHeight());

            Image img = connectPicBox.Image;
            img.RotateFlip(RotateFlipType.Rotate270FlipNone);
            connectPicBox.Image = img;
        }

        private void connectTabPanel_Click(object sender, EventArgs e)
        {
            connectLabelText.ForeColor = setMouseClickLabelTextColor();
            connect_Click_Position();
            tabControl1.SelectedIndex = 0;
        }

        private void connectTabPanel_MouseEnter(object sender, EventArgs e)
        {
            connectLabelText.ForeColor = setMouseInLabelTextColor();
        }

        private void connectTabPanel_MouseLeave(object sender, EventArgs e)
        {
            connectLabelText.ForeColor = setMouseDefaultLabelTextColor();
        }

        private void exitLabelText_Click(object sender, EventArgs e)
        {
            exitLabelText.ForeColor = setMouseClickLabelTextColor();
            System.Environment.Exit(0);
        }
        private void exitLabelText_MouseEnter(object sender, EventArgs e)
        {
            exitLabelText.ForeColor = setMouseInLabelTextColor();
        }

        private void exitLabelText_MouseLeave(object sender, EventArgs e)
        {
            exitLabelText.ForeColor = setMouseDefaultLabelTextColor();
        }

        private void update_Click_Position()
        {
            if(isConnectClick)
            {
                isConnectClick = !isConnectClick;
                closeConnectPanel();
            }
            if (isDataClick)
            {
                isDataClick = !isDataClick;
                closeDataPanel();
            }
        }
        /// <summary>
        /// 点击了固件更新按钮时，显示参数的控件
        /// </summary>
        private void updatelabel_Click(object sender, EventArgs e)
        {
            updatelabel.ForeColor = setMouseClickLabelTextColor();
            tabControl1.SelectedIndex = 1;
            update_Click_Position();
        }

        private void updatelabel_MouseEnter(object sender, EventArgs e)
        {
            updatelabel.ForeColor = setMouseInLabelTextColor();
        }

        private void updatelabel_MouseLeave(object sender, EventArgs e)
        {
            updatelabel.ForeColor = setMouseDefaultLabelTextColor();
        }
        /// <summary>
        /// 点击了参数按钮时，显示参数的控件
        /// </summary>
        private void data_Click_Position()
        {
            if (isConnectClick)
            {
                isConnectClick = !isConnectClick;
                closeConnectPanel();
            }
            /*if (isWaveClick)
            {
                closeWavePanel();
                isWaveClick = !isWaveClick;
            }
            if (isPlaybackWaveClick)
            {
                closePlaybackPanel();
                isPlaybackWaveClick = !isPlaybackWaveClick;
            }*/
            isDataClick = !isDataClick;
            if (isDataClick)
            {
                openDataPanel();
            }
            else
            {
                closeDataPanel();
            }
        }
        /// <summary>
        /// 参数面板打开时，对界面的布局
        /// </summary>
        private void openDataPanel()
        {
            int waveTabPanelSizeX = datapanel.Location.X;
            datapanel.Size = new System.Drawing.Size(waveTabPanelSizeX, getLeftDataPanelHeight());

            Image img = datapictureBox.Image;
            img.RotateFlip(RotateFlipType.Rotate90FlipNone);
            datapictureBox.Image = img;

            seconddatapanel.Parent = datapanel;
            seconddatapanel.Location = new System.Drawing.Point(2, getLeftItemPanelHeight());
            seconddatapanel.Visible = true;
        }
        /// <summary>
        /// 关闭参数面板，界面的布局发生相应的变化
        /// </summary>
        private void closeDataPanel()
        {
            int waveTabPanelSizeX = datapanel.Location.X;
            datapanel.Size = new System.Drawing.Size(waveTabPanelSizeX, getLeftItemPanelHeight());

            Image img = datapictureBox.Image;
            img.RotateFlip(RotateFlipType.Rotate270FlipNone);
            datapictureBox.Image = img;
        }

        private void datalabel_Click(object sender, EventArgs e)
        {
            datalabel.ForeColor = setMouseClickLabelTextColor();
            data_Click_Position();
            tabControl1.SelectedIndex = 2;
        }

        private void datalabel_MouseEnter(object sender, EventArgs e)
        {
            datalabel.ForeColor = setMouseInLabelTextColor();
        }

        private void datalabel_MouseLeave(object sender, EventArgs e)
        {
            datalabel.ForeColor = setMouseDefaultLabelTextColor();
        }
        #endregion

    }
}
