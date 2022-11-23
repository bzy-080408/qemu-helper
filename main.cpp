

#include <bits/stdc++.h>

using namespace std;

#define CMD_RESULT_BUF_SIZE 1024 //调用系统函数tmp大小
typedef unsigned char UBIT8;
#define UNICODE
// PATH_CONFIG
string qemu = "qemu-system";
string ARCH[4] = {"i386", "x86_64", "aarch64", "arm"};
string CONFIG_PATH = "config/";
string TMP_PATH = "tmp/";

// main.conf
// string language;
// bool newstart = false;

//虚拟机设置
string configname;  //配置名称
string virtualarch; //虚拟机架构
string virtualM;    //设备类型
string virtualm;    //内存大小
string virtualcpu;  // cpu类型
string virtualsmp;  // cpu线程数
bool enableKVM;     //是否开启虚拟化
bool enableTCG;     //是否开启TCG加速
//硬盘
string hdafile;
bool hda = false;
string hdbfile;
bool hdb = false;
string hdcfile;
bool hdc = false;
string hddfile;
bool hdd = false;
//其他设备
vector<string> virtualdevices;

void systemcall(string cmd, string file) // have bug
{                                        //调用系统指令
    ofstream outfile(file);
    char line[100];
    FILE *fp;
    // string cmd = "ls -l";

    //  system call
    const char *sysCommand = cmd.data();
    if ((fp = popen(sysCommand, "r")) == NULL)
    {
        cout << "debug: system called error:" << cmd << endl;
        exit(0);
    }
    while (fgets(line, sizeof(line) - 1, fp) != NULL)
    {
        outfile << line << endl;
    }
    outfile.close();
    pclose(fp);
}

// int systemcall(const char *cmd, char *result)
// {
//     int iRet = -1;
//     char buf_ps[CMD_RESULT_BUF_SIZE];
//     char ps[CMD_RESULT_BUF_SIZE] = {0};
//     FILE *ptr;
//     strcpy(ps, cmd);

//     if ((ptr = popen(ps, "r")) != NULL)
//     {
//         while (fgets(buf_ps, sizeof(buf_ps), ptr) != NULL)
//         {
//             strcat(result, buf_ps);
//             if (strlen(result) > CMD_RESULT_BUF_SIZE)
//             {
//                 break;
//             }
//         }
//         pclose(ptr);
//         ptr = NULL;
//         iRet = 0; // 处理成功
//     }
//     else
//     {
//         printf("popen %s error\n", ps);
//         iRet = -1; // 处理失败
//     }

//     return iRet;
// }

// void mainconfupdate() //读入主配置文件
// {
//     string input = CONFIG_PATH + "main.conf";
//     ifstream mainconfig(input);
//     mainconfig >> newstart >> language;
//     mainconfig.close();
//     return;
// }

// void mainconfupdate() //保存主配置文件
// {
//     systemcall("rm " + CONFIG_PATH + "main.conf");
//     ofstream mainconfig(CONFIG_PATH + "main.conf");
//     mainconfig << newstart << endl
//                << language << endl;
//     mainconfig.close();
//     return;
// }

void virtualconfigupdate(string virtualconfigname) //读入虚拟机配置文件
{
    virtualconfigname = CONFIG_PATH + "virtualsystem/" + virtualconfigname;
    ifstream configin(virtualconfigname);
    getline(configin, virtualarch);
    getline(configin, virtualM);
    getline(configin, virtualm);
    getline(configin, virtualcpu);
    getline(configin, virtualsmp);
    configin >> enableKVM >> enableTCG;
    getline(configin, hdafile);
    configin >> hda;
    getline(configin, hdbfile);
    configin >> hdb;
    getline(configin, hdcfile);
    configin >> hdc;
    getline(configin, hddfile);
    configin >> hdd;
    virtualdevices.clear();
    string tmp;
    cout << "debug: #1" << endl;
    int devicesize;
    cin >> devicesize;
    cout << "debug: devicesize=" << devicesize << endl;
    for (int i = 1; i <= devicesize; i++)
    {
        cin >> tmp;
        virtualdevices.push_back(tmp);
    }
    cout << "debug: #2" << endl;
    return;
}

void virtualconfigupgrade(string virtualconfigname) //保存虚拟机配置文件
{
    virtualconfigname = CONFIG_PATH + "virtualsystem/" + virtualconfigname;
    // systemcall("rm " + virtualconfigname, "tmp/cmd.tmp"); //删除就配置
    ofstream configout(virtualconfigname);
    configout << virtualarch << endl //输入新配置
              << virtualM << endl
              << virtualm << endl
              << virtualcpu << endl
              << virtualsmp << endl
              << enableKVM << endl
              << enableTCG << endl
              << hdafile << endl
              << hda << endl
              << hdbfile << endl
              << hdb << endl
              << hdcfile << endl
              << hdc << endl
              << hddfile << endl
              << hdd << endl
              << virtualdevices.size() << endl;
    for (int i = 0; i < virtualdevices.size(); i++)
    {
        configout << virtualdevices[i] << endl;
    }
    configout.close();
    return;
}

void configlistupdate() //更新虚拟机列表
{
    string dir = CONFIG_PATH + "virtualsystem/";
    systemcall("ls " + dir + ">" + dir + "virtualsystem_file.conf", "tmp/cmd.tmp");
    return;
}

void configlistprint()
{ //输出虚拟机配置列表
    configlistupdate();
    string dir = CONFIG_PATH + "virtualsystem/";
    ifstream infile(dir + "virtualsystem_file.conf");
    cout << "虚拟机配置文件列表：" << endl;
    string tmp;
    while (infile >> tmp)
    {
        cout << tmp << endl;
    }
    infile.close();
    return;
}

void configprint()
{
    cout << "虚拟机架构：" << virtualarch << endl;
    cout << "设备类型：" << virtualM << endl;
    cout << "内存大小:" << virtualm << endl;
    cout << "cpu:" << virtualcpu << endl;
    cout << "CPU线程数:" << virtualsmp << endl;
    cout << "硬盘配置：" << endl;
    cout << "hda:";
    if (!hda)
    {
        cout << "disable";
    }
    else
    {
        cout << hdafile;
    }
    cout << endl;
    cout << "hdb:";
    if (!hdb)
    {
        cout << "disable";
    }
    else
    {
        cout << hdbfile;
    }
    cout << endl;
    cout << "hdc:";
    if (!hdc)
    {
        cout << "disable";
    }
    else
    {
        cout << hdcfile;
    }
    cout << endl;
    cout << "hdd:";
    if (!hdd)
    {
        cout << "disable";
    }
    else
    {
        cout << hddfile;
    }
    cout << endl;
    return;
}

void configprint(string virtualconfigname) //输出虚拟机配置
{
    virtualconfigupdate(virtualconfigname);
    return;
}

void configdevicesprint()
{ //输出选择的所有设备
    cout << endl;
    for (int i = 0; i < virtualdevices.size(); i++)
    {
        cout << i << " " << virtualdevices[i] << endl;
    }
    return;
}

int main()
{
    system("chcp 65001"); //修正中文乱码 ，仅在windows下测试通过，但是主程序写给linux
    string main[20] = {
        "很好，现在开始配置虚拟机",
        "输入start启动虚拟机",
        "输入list查看虚拟机配置",
        "输入save保存虚拟机配置",
        "输入arch更改架构选项",
        "输入M更改计算机类型"
        "输入hda更改第一个ide虚拟硬盘"
        "输入hdb更改第一个ide虚拟硬盘"
        "输入hdc更改第一个ide虚拟硬盘"
        "输入hdd更改第一个ide虚拟硬盘"
        "输入cpu更改CPU选项",
        "输入smp更改CPU的线程数",
        "输入m更改内存大小",
        "输入device添加，删除或更改设备",
        "输入kvm开启或关闭虚拟化（需要设备支持）",
        "输入TCG开启或关闭TCG加速",
        "输入exit退出程序"};
    cout << "hello,请选择配置文件，或新建文件" << endl;
    configlistprint();
    cout << "输入文件名称，不要用中文，无论原有文件还行新建都直接输入，加上扩展名:";
    cin >> configname;
    virtualconfigupdate(configname);
    while (1)
    {
        for (int i = 0; i < 16; i++)
        {
            cout << main[i] << endl;
        }
        cout << "输入操作：";
        string choose;
        cin >> choose;
        if (choose == "start")
        {
            ofstream outfile("tmp/start.sh");
            outfile << "qemu-system-" + virtualarch << " -M " << virtualM << " -cpu " << virtualcpu << " -m " << virtualm;
            if (hda)
            {
                outfile << " -hda " << hdafile;
            }
            if (hdb)
            {
                outfile << " -hdb " << hdbfile;
            }
            if (hdc)
            {
                outfile << " -hdc " << hdcfile;
            }
            if (hdd)
            {
                outfile << " -hdd " << hddfile;
            }
            if (enableKVM)
            {
                outfile << " -enable-kvm ";
            }
        }
        if (choose == "list")
        {
            configprint();
            system("pause");
        }
        if (choose == "save")
        {
            virtualconfigupgrade(configname);
        }
        if (choose == "arch")
        {
            cout << "当前架构为" << virtualarch << endl;
            cout << "请选择架构：(更改架构后请更改其他选项，否则可能启动失败）" << endl;
            for (int i = 0; i <= 4; i++)
            {
                cout << ARCH[i] << " ";
            }
            cout << endl
                 << "输入对应的架构名称:";
            cin >> virtualarch;
        }
        if (choose == "M")
        {
            string systemcallstring = "qemu-system-" + virtualM + "  -M help";
            cout << "当前设备型号为：" << virtualM << endl;
            cout << "请选择设备型号：" << endl;
            systemcall(systemcallstring, "tmp/cmd.tmp");
            ifstream infile("tmp/cmd.tmp");
            string tmp;
            while (infile >> tmp)
            {
                cout << tmp << endl;
            }
            infile.close();
            cout << "请输入对应型号名称:";
            cin >> virtualM;
        }
        if (choose == "m")
        {
            cout << "当前内存大小为：" << virtualm << endl;
            cout << "请输入内存大小:";
            cin >> virtualm;
        }
        if (choose == "cpu")
        {
            string systemcallstring = "qemu-system-" + virtualarch + "  -M" + virtualM + "-cpu help";
            cout << "当前cpu型号为：" << virtualcpu << endl;
            cout << "请选择cpu型号：" << endl;
            systemcall(systemcallstring, "tmp/cmd.tmp");
            ifstream infile("tmp/cmd.tmp");
            string tmp;
            while (infile >> tmp)
            {
                cout << tmp << endl;
            }
            infile.close();
            cout << "请输入对应cpu名称:";
            cin >> virtualcpu;
        }
        if (choose == "smp")
        {
            cout << "当前线程数为：" << virtualsmp << endl;
            cout << "请输入线程数:";
            cin >> virtualsmp;
        }
        if (choose == "hda")
        {
            hda = true;
            cout << "当前虚拟硬盘文件：" << hdafile << endl;
            cout << "是否启用hda(yes/no):";
            string tmp;
            cin >> tmp;
            if (tmp == "yes")
            {
                cout << "请输入虚拟硬盘文件（请使用绝对路径+文件名+扩展名）：";
                cin >> hdafile;
            }
            else
            {
                hda = false;
            }
        }
        if (choose == "hdb")
        {
            hdb = true;
            cout << "当前虚拟硬盘文件：" << hdbfile << endl;
            cout << "是否启用hdb(yes/no):";
            string tmp;
            cin >> tmp;
            if (tmp == "yes")
            {
                cout << "请输入虚拟硬盘文件（请使用绝对路径+文件名+扩展名）：";
                cin >> hdbfile;
            }
            else
            {
                hdc = false;
            }
        }
        if (choose == "hdc")
        {
            hdc = true;
            cout << "当前虚拟硬盘文件：" << hdcfile << endl;
            cout << "是否启用hdc(yes/no):";
            string tmp;
            cin >> tmp;
            if (tmp == "yes")
            {
                cout << "请输入虚拟硬盘文件（请使用绝对路径+文件名+扩展名）：";
                cin >> hdcfile;
            }
            else
            {
                hdc = false;
            }
        }
        if (choose == "hdd")
        {
            hdd = true;
            cout << "当前虚拟硬盘文件：" << hddfile << endl;
            cout << "是否启用hdd(yes/no):";
            string tmp;
            cin >> tmp;
            if (tmp == "yes")
            {
                cout << "请输入虚拟硬盘文件（请使用绝对路径+文件名+扩展名）：";
                cin >> hddfile;
            }
            else
            {
                hdd = false;
            }
        }
        if (choose == "exit")
        {
            exit(0);
        }
        if (choose == "device")
        {
            string systemcallstring = "qemu-system-" + virtualarch + "  -M" + virtualM + " -cpu " + virtualcpu + " -device help ";
            cout << "当前的设备:" << endl;
            configdevicesprint();
            cout << "输入 H 查看设备帮助" << endl;
            cout << "输入 D 删除设备" << endl;
            cout << "输入 N 新建设备" << endl;
            cout << "输入 E 修改设备" << endl;
            cout << "请输入选项：";
            string devicechoose;
            cin >> devicechoose;
            cout << endl;
            if (devicechoose == "H")
            {
                systemcall(systemcallstring, "tmp/cmd.tmp");
                ifstream infile("tmp/cmd.tmp");
                string tmp;
                while (infile >> tmp)
                {
                    cout << tmp << endl;
                }
                infile.close();
            }
            if (devicechoose[0] == 'N')
            {
                cout << endl
                     << "请输入设备:";
                string input;
                cin >> input;
                virtualdevices.push_back(input);
                cout << endl;
            }
            if (devicechoose[0] == 'E')
            {
                int num = 0;
                cout << "请输入设备编号:";
                cin >> num;
                cout << endl
                     << "请输入设备:";
                string input;
                cin >> input;
                virtualdevices[num].clear();
                virtualdevices[num] += input;
                cout << endl;
            }
            if (devicechoose[0] == 'D')
            {
                int num = 0;
                cout << "请输入设备编号:";
                cin >> num;
                virtualdevices.erase(virtualdevices.begin() + num);
            }
        }
    }
    return 0;
}