#include "filesystem.h"

int style = 1;			//文件的类型
int file_array_head;	//文件表组头
int physic[100];		//文件地址缓冲区
string cur_user;		//当前用户
int file_array[20] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };  //打开文件表组
node temp_file;
string temp_write;

/*创建文件*/
void create_file(char filename[], int length, int userid, int limit, int privilege){
	int i, j;
	for (i = 0; i<640; i++)
	{
		//判断重名
		if (strcmp(filename, root[i].file_name) == 0)
		{
			printf("已经存在同名文件，不允许建立重名的文件\n");
			return;
		}
	}
	for (i = 0; i<640; i++)
	{
		//找到空闲块
		if (root[i].i_num == -1)//相当于该i节点空闲
		{
			root[i].i_num = i;//i_num用来表示ID号，如果为-1，则表示该ID对应的块没有被分配
			strcpy(root[i].file_name, filename);
			strcpy(root[i].dir_name, cur_dir[login_userid]);  //把当前目录名 给新建立的文件，也就是上一级目录名
			i_node[i].file_style = style;//style==0 说明文件是目录文件，style==1是文件
			i_node[i].file_length = length;
			i_node[i].limit = limit;
			i_node[i].privilege = privilege;
			i_node[i].file_UserId = userid;
			i_node[i].use_length=0;
			allot(length);
			for (j = 0; j<length; j++)
			{
				i_node[i].file_address[j] = physic[j];
			}

			//初始化文件
			for (int add = 0; add < i_node[i].file_length; add++){
					memory[i_node[root[i].i_num].file_address[add]].content = '\0';
				}
			int u;
			for (u = 0; u<100; u++)//分配完清空缓冲区
			{
				physic[u] = -1;
			}
			break;
		}
	}
}

/*删除文件*/
void del_file(char filename[]){
	int i, j, k;
	for (i = 0; i<640; i++)
	{

		if ((strcmp(filename, root[i].file_name) == 0) && (strcmp(cur_dir[login_userid], root[i].dir_name) == 0) && (i_node[root[i].i_num].file_UserId == login_userid))
		{
			int add, c;
			for (add = 0; add<i_node[root[i].i_num].file_length; add++)//文件内容清空
			{
                memory[i_node[root[i].i_num].file_address[add]].content = '\0';
			}
			k = root[i].i_num;
			i_node[k].file_UserId = -1;
			i_node[k].limit = -1;
			i_node[k].privilege = -1;
			i_node[k].use_length = -1;
			temp_write="";
			for (j = 0; j<i_node[k].file_length; j++)
			{
				physic[j] = i_node[k].file_address[j];
			}
			callback(i_node[k].file_length); //调用 回收函数
			int u;//回收完情空缓存区
			for (u = 0; u<100; u++)
			{
				physic[u] = -1;
			}
			for (j = 0; j<i_node[k].file_length; j++)     //删除文件后要将文件属性和目录项的各个值恢复初值
			{
				i_node[k].file_address[j] = -1; //文件占用的块号地址恢复初值
			}
			strcpy(root[i].file_name, "");  //文件名恢复初值
			root[i].i_num = -1;				//目录项的I结点信息恢复初值
			strcpy(root[i].dir_name, "");	//目录项的文件目录信息恢复初值
			i_node[k].file_length = -1;		//文件长度恢复
			i_node[k].file_style = -1;		//文件类型恢复初值
			break;
		}
	}
	if (i == 640)
	{
		printf("登录用户的该目录下不存在这个文件\n");
	}
}

/*打开文件*/
int open(char filename[10]){
	int i;
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1 && i_node[root[i].i_num].file_UserId == login_userid)//找到该文件
		{
			if (file_array_head<20)//最多可支持打开8个文件
			{
				file_array[file_array_head] = root[i].i_num;//将该文件的ID号存到file_array中，表示文件已经打开
				file_array_head++;
			}
			else
			{
				printf("打开的文件已达上限，无法打开本文件\n");
			}
			return root[i].i_num;
		}
	}
	if (i = 640)
	{
		printf("您要打开的文件不存在或不属于该用户\n");
	}
	return 0;
}

/*关闭文件*/
void close(char filename[10]){
	int i;
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1 && i_node[root[i].i_num].file_UserId == login_userid)//找到该文件
		{
			int j;
			for (j = 0; j<file_array_head; j++)
			{
				if (root[i].i_num == file_array[j])//在file_array中找到该文件ID号，表示该文件已经被打开
				{
					int m;
					for (m = j; m<file_array_head; m++)
					{
						file_array[m] = file_array[m + 1];//在filr_array中将该ID号删除，并将后面打开的ID号提前一个位置
					}
					file_array_head--;
					return;
				}
			}
			if (j == file_array_head)
			{
				printf("您要关闭的文件未打开过！\n");
			}
			return;
		}
	}
	if (i = 640)
	{
		printf("您要关闭的文件不存在或不属于该用户\n");
	}
	return;
}

/*读取文件*/
int read(char filename[10]){
	int i;
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1)//找到该文件
		{
			int j;
			char temp_read[i_node[root[i].i_num].file_length];
			for (j = 0; j<8; j++)
			{
				if (root[i].i_num == file_array[j])//在已经打开的文件中找到该文件的ID号
				{
					if (i_node[root[i].i_num].limit == 0 || i_node[root[i].i_num].limit == 1)//拥有相关权限
					{
						int c, add;
						for (add = 0; add < i_node[root[i].i_num].file_length; add++) {
							temp_read[add]= memory[i_node[root[i].i_num].file_address[add]].content;
						}
						cout << endl;
                        cout << "文件内容为：";
	                    cout << temp_read << endl;
					}
					else
					{
						printf("你没有权限读取文件内容！！\n");
					}
					return 0;
				}
			}
			if (j == 8)
			{
				printf("\n  该文件未打开，请先打开文件再进行读写操作!!\n");
			}
			return 0;
		}
	}
	if (i == 640)
	{
		printf("您要读取的文件不存在\n");
	}
	return 0;
}

/*写入文件*/
void write(char filename[10], string writec, int s_len){
	int i;
	int c=0;
	char tmp_writec[s_len];
	strcpy(tmp_writec,writec.c_str());    //将writec从字符串类型转化为字符数组类型
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1)//找到该文件
		{
			int j;
			for (j = 0; j<8; j++)
			{
				if (root[i].i_num == file_array[j])//确认该文件已经打开
				{
					if (i_node[root[i].i_num].limit == 0 || i_node[root[i].i_num].limit == 2)//确认相关权限
					{
						int c, add, write_length;
						if((s_len+i_node[root[i].i_num].use_length)>i_node[root[i].i_num].file_length)
                        {
                            cout<<"文件空间已满，超出部分写入失败！"<<endl;
                        }
						for (add = i_node[root[i].i_num].use_length; add< i_node[root[i].i_num].file_length; add++)
						{
								if (memory[i_node[root[i].i_num].file_address[add]].content == '\0')    //如果该块没有被写入
								{
                                    memory[i_node[root[i].i_num].file_address[add]].content = tmp_writec[c] ;
                                    i_node[root[i].i_num].use_length++;
                                    c++;
								}
						}
						return;
					}
					else
					{
						printf("你没有权限将内容写入文件！！\n");
						return;
					}
					return;
				}
			}
			if (j == 8)
			{
				printf("\n  该文件未打开，请先打开文件再进行读写操作!!\n");
			}
			return;
		}
	}
	if (i == 640)
	{
		printf("您要写入的文件不存在\n");
	}
	return;
}

/*显示文件信息*/
void show_file(char filename[]){
	int i, j, k, p;
	cout << '\t' << "文件名字  读取权限  隐私权限  所属目录  所属用户" << endl;
	for (i = 0; i<640; i++)
	{
		k = root[i].i_num;
		if (strcmp(filename, root[i].file_name) == 0 && (i_node[k].file_style == 1))
		{
		    cout << setw(16) << root[i].file_name;     //文件名
			if(i_node[k].limit==0)
                {cout << setw(8) << "o+r+w";}
            else if(i_node[k].limit==1)
                {cout << setw(8) << "o+r";}
            else if(i_node[k].limit==2)
                {cout << setw(8) << "o+w";}
            else if(i_node[k].limit==3)
                {cout << setw(8) << "o";}         //读取权限
            else
                {cout << setw(8)<< "error";}

            if(i_node[k].privilege==0)
                {cout << setw(12) << "公开可见";}
            else if(i_node[k].privilege==1)
                {cout << setw(12) << "自己可见";}
            else
                {cout << setw(12) << "error";}               //隐私权限
            cout << setw(11) << root[i].dir_name;		//文件所在的目录
			cout << setw(6) << user_array[i_node[k].file_UserId].username << endl;//文件所属用户
			cout << "文件占用的物理地址:" << endl;
			for (j = 0; j<i_node[k].file_length; j++)   //显示物理地址
			{
			    cout << setw(3) << i_node[k].file_address[j];    //文件具体占用的盘块号
			}
			printf("\n");
			break;
		}
	}
	if (i == 640)
	{
		printf("没有这个文件 或者这个文件不是正规文件\n");
	}
}

/*修改文件信息*/
void mod_file(char filename[]){
    int i;
    char tmp_filename[10];
    char tmp_limit;
    char tmp_privilege;
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1 && i_node[root[i].i_num].file_UserId == login_userid)//找到该文件
		{
		    cout<<"请输入修改后的文件名(最长10个字符):";
		    cin>> tmp_filename;
		    cout<<"请输入修改后的读取权限(0:o+r+w;1:o+r;2:o+w;3:o):";
		    cin>> tmp_limit;
		    while(tmp_limit!='0'&&tmp_limit!='1'&&tmp_limit!='2'&&tmp_limit!='3')
            {
                cout << "请输入正确权限(0:o+r+w;1:o+r;2:o+w;3:o):";
                cin >> tmp_limit;
            }
            cout<<"请输入修改后的隐私权限(0:公开可见;1:自己可见):";
            cin>> tmp_privilege;
            while(tmp_privilege!='0'&&tmp_privilege!='1')
            {
                cout << "请输入正确权限(0:公开可见;1:自己可见):";
                cin >> tmp_privilege;
            }
            strcpy(root[i].file_name, tmp_filename);
            i_node[root[i].i_num].limit = tmp_limit;
            i_node[root[i].i_num].privilege = tmp_privilege;
            break;
		}
	}
    if (i == 640)
	{
		printf("您要修改的文件信息不存在或您没有相关权限\n");
	}
}

/*复制文件*/
void copy(char filename[10]) {
	int i;
	open(filename);
	read(filename);
	close(filename);
	for ( i= 0; i < 640; i++) {
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1)
			temp_file = i_node[root[i].i_num];
	}
}

/*粘贴*/
void paste(char filename[10]) {
	create_file(filename, temp_file.file_length, login_userid, temp_file.limit, temp_file.privilege);
	open(filename);
	write(filename, temp_write, temp_write.size());
	close(filename);
}
