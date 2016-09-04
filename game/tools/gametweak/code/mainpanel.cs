//================================================================================================
// USINGS
//================================================================================================

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Xml;
using System.Threading;
using System.Diagnostics;

namespace GameTweak
{
    //================================================================================================
    // CLASS
    //================================================================================================
    public partial class MainPanel : Form
    {
        enum NetworkMessages
        {
            NETWORK_ACKNOWLEDGE,
            NETWORK_MESSAGE_SHADER_RELOAD,
            NETWORK_MESSAGE_MAX
        };

        //================================================================================================
        // MEMBERS
        //================================================================================================
        private const int mPort = 5001;
        private Socket mClient;
        private IPEndPoint mRemoteEP;
        private Dictionary<string, DateTime> mFileTimeMap = new Dictionary<string, DateTime>();

        private static RichTextBox mOutputText;
        private static Button mConnectButton;
        private static System.Timers.Timer mTimer = new System.Timers.Timer();

        //================================================================================================
        // FUNCTIONS
        //================================================================================================

        //
        // Thread safe set text function
        //
        delegate void AppendTextOutputCallback(string text, Color textColor);
        private void AppendTextOutput(string text, Color textColor)
        {
            if (mOutputText.InvokeRequired)
            {
                AppendTextOutputCallback d = new AppendTextOutputCallback(AppendTextOutput);
                this.Invoke(d, new object[] { text, textColor });
            }
            else
            {
                mOutputText.SelectionColor = textColor;
                
                string str = Convert.ToString(System.DateTime.Now);
                mOutputText.AppendText(str);
                mOutputText.AppendText(": ");

                mOutputText.AppendText(text);
                mOutputText.SelectionStart = mOutputText.Text.Length;
                mOutputText.Focus();
                mOutputText.ScrollToCaret();
            }
        }

        //================================================================================================

        delegate void SetButtonTextCallback(string text);
        private void SetButtonText(string text)
        {
            if (mConnectButton.InvokeRequired)
            {
                SetButtonTextCallback d = new SetButtonTextCallback(SetButtonText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                mConnectButton.Text = text;
            }
        }

        //================================================================================================

        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                Socket client = (Socket)ar.AsyncState;
                client.EndConnect(ar);

                mTimer.Start();

                AppendTextOutput("Connected...\n", Color.Black);

                SetButtonText("Disconnect");
            }
            catch(System.Exception ex)
            {
                AppendTextOutput("Did not connect...\n", Color.Red);
				Console.WriteLine(ex.ToString());
            }
        }

        //================================================================================================

        private static void SendCallback(IAsyncResult ar)
        {
            try
            {
                Socket client = (Socket)ar.AsyncState;
                int bytesSent = client.EndSend(ar);

                Console.WriteLine("Sent {0} bytes to server.", bytesSent);
            }
            catch
            {
            }
        }

        //================================================================================================

        private bool Send(Socket client, byte[] bytes)
        {
            try
            {
                client.BeginSend(bytes, 0, bytes.Length, 0, new AsyncCallback(SendCallback), client);
                return true;
            }
            catch
            {
                AppendTextOutput("Did not send message...\n\n", Color.Red);
                return false;
            }
        }

        //================================================================================================

        private void OnChanged(object source, FileSystemEventArgs e)
        {
            if (
                e.FullPath.EndsWith(".vert") ||
                e.FullPath.EndsWith(".frag") ||
                e.FullPath.EndsWith(".psh") ||
                e.FullPath.EndsWith(".vsh")
                )
            {
                bool executeChange = true;
                if (mFileTimeMap.ContainsKey(e.FullPath))
                {
                    DateTime lastFileLastWriteTime = mFileTimeMap[e.FullPath];
                    FileInfo fi = new FileInfo(e.FullPath);
                    DateTime currentFileLastWriteTime = fi.LastWriteTime;

                    executeChange = false;
                    if (currentFileLastWriteTime > lastFileLastWriteTime)
                    {
                        executeChange = true;
                        mFileTimeMap[e.FullPath] = currentFileLastWriteTime;
                    }
                }
                else
                {
                    FileInfo fi = new FileInfo(e.FullPath);
                    DateTime currentFileLastWriteTime = fi.LastWriteTime;
                    mFileTimeMap.Add(e.FullPath, currentFileLastWriteTime);
                }

                if (executeChange)
                {
                    Console.WriteLine(e.FullPath.ToString());

                    string currentDirectory = Directory.GetCurrentDirectory();
                    string assetDirectory = currentDirectory + "\\..\\..\\..\\..\\assets";
                    string deployScript = currentDirectory + "\\..\\..\\..\\..\\assets\\deploypcdata.py";

                    StreamReader sr;
                    StreamReader err;

                    Process p = new Process();
                    p.StartInfo.UseShellExecute = false;
                    p.StartInfo.RedirectStandardOutput = true;
                    p.StartInfo.RedirectStandardError = true;
                    p.StartInfo.CreateNoWindow = true;
                    p.StartInfo.WorkingDirectory = assetDirectory;
                    p.StartInfo.FileName = "C:\\Python26\\pythonw.exe";
                    p.StartInfo.Arguments = deployScript;
                    p.Start();

                    sr = p.StandardOutput;
                    err = p.StandardError;

                    p.WaitForExit();

                    string standardOutput = sr.ReadToEnd();
                    string errorOutput = err.ReadToEnd();

                    standardOutput += "\n";
                    errorOutput += "\n";

                    AppendTextOutput(standardOutput, Color.Black);                    
                    AppendTextOutput(errorOutput, Color.Black);
                    
                    int messageID = (int)NetworkMessages.NETWORK_MESSAGE_SHADER_RELOAD;
                    byte[] message = BitConverter.GetBytes(messageID);
                    Send(mClient, message);
                }
            }
        }

        //================================================================================================

        private void Connect_Click(object sender, EventArgs e)
        {
            if (mClient.Connected == false)
            {
                mClient.BeginConnect(mRemoteEP, new AsyncCallback(ConnectCallback), mClient);
            }
            
            if(mClient.Connected == true)
            {
                mTimer.Stop();
                mClient.Shutdown(SocketShutdown.Both);                
                mClient.Disconnect(true);

                AppendTextOutput("Timed out...\n\n", Color.Red);
                AppendTextOutput("Disconnected...\n\n", Color.Red);

                SetButtonText("Connect");
            }
        }

        //================================================================================================

        public void OnTimerEvent(object source, EventArgs e)
        {
            int messageID = (int)NetworkMessages.NETWORK_ACKNOWLEDGE;
            byte[] message = BitConverter.GetBytes(messageID);
            if ( Send(mClient, message) == false)
            {
                mTimer.Stop();
                mClient.Shutdown(SocketShutdown.Both);
                mClient.Disconnect(true);

                AppendTextOutput("Disconnected...\n\n", Color.Red);

                SetButtonText("Connect");
            }
        }

        //================================================================================================
        // MAIN
        //================================================================================================

        public MainPanel()
        {
            InitializeComponent();
        }

        //================================================================================================

        private void MainPanel_Load(object sender, EventArgs e)
        {
            mOutputText = this.OutputText;
            mConnectButton = this.Connect;

            mRemoteEP = new IPEndPoint(IPAddress.Parse("127.0.0.1"), mPort);
			//mRemoteEP = new IPEndPoint(IPAddress.Parse("10.10.88.203"), 1000);			

            mClient = new Socket(
                    AddressFamily.InterNetwork,
                    SocketType.Stream,
                    ProtocolType.Tcp
                    );

            //
            // File Watching
            //

            string currentDirectory = Directory.GetCurrentDirectory();
            string shaderAssetDirectory = currentDirectory + "\\..\\..\\..\\..\\assets\\shaders";

            FileSystemWatcher watcher = new FileSystemWatcher();
            watcher.Path = shaderAssetDirectory;
            watcher.IncludeSubdirectories = true;
            watcher.NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.Attributes;
            watcher.Filter = "*.*";
            watcher.Changed += new FileSystemEventHandler(OnChanged);
            watcher.EnableRaisingEvents = true;

            //
            // Timer
            //
            mTimer.Elapsed += new System.Timers.ElapsedEventHandler(OnTimerEvent);
            mTimer.Interval = 2000;
        }

        //================================================================================================

        private void MainPanel_Closing(object sender, FormClosingEventArgs e)
        {
        }
    }
}