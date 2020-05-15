#include "filesystem.h"

int style = 1;			//�ļ�������
int file_array_head;	//�ļ�����ͷ
int physic[100];		//�ļ���ַ������
string cur_user;		//��ǰ�û�
int file_array[20] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };  //���ļ�����
node temp_file;
string temp_write;

/*�����ļ�*/
void create_file(char filename[], int length, int userid, int limit, int privilege){
	int i, j;
	for (i = 0; i<640; i++)
	{
		//�ж�����
		if (strcmp(filename, root[i].file_name) == 0)
		{
			printf("�Ѿ�����ͬ���ļ������������������ļ�\n");
			return;
		}
	}
	for (i = 0; i<640; i++)
	{
		//�ҵ����п�
		if (root[i].i_num == -1)//�൱�ڸ�i�ڵ����
		{
			root[i].i_num = i;//i_num������ʾID�ţ����Ϊ-1�����ʾ��ID��Ӧ�Ŀ�û�б�����
			strcpy(root[i].file_name, filename);
			strcpy(root[i].dir_name, cur_dir[login_userid]);  //�ѵ�ǰĿ¼�� ���½������ļ���Ҳ������һ��Ŀ¼��
			i_node[i].file_style = style;//style==0 ˵���ļ���Ŀ¼�ļ���style==1���ļ�
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

			//��ʼ���ļ�
			for (int add = 0; add < i_node[i].file_length; add++){
					memory[i_node[root[i].i_num].file_address[add]].content = '\0';
				}
			int u;
			for (u = 0; u<100; u++)//��������ջ�����
			{
				physic[u] = -1;
			}
			break;
		}
	}
}

/*ɾ���ļ�*/
void del_file(char filename[]){
	int i, j, k;
	for (i = 0; i<640; i++)
	{

		if ((strcmp(filename, root[i].file_name) == 0) && (strcmp(cur_dir[login_userid], root[i].dir_name) == 0) && (i_node[root[i].i_num].file_UserId == login_userid))
		{
			int add, c;
			for (add = 0; add<i_node[root[i].i_num].file_length; add++)//�ļ��������
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
			callback(i_node[k].file_length); //���� ���պ���
			int u;//��������ջ�����
			for (u = 0; u<100; u++)
			{
				physic[u] = -1;
			}
			for (j = 0; j<i_node[k].file_length; j++)     //ɾ���ļ���Ҫ���ļ����Ժ�Ŀ¼��ĸ���ֵ�ָ���ֵ
			{
				i_node[k].file_address[j] = -1; //�ļ�ռ�õĿ�ŵ�ַ�ָ���ֵ
			}
			strcpy(root[i].file_name, "");  //�ļ����ָ���ֵ
			root[i].i_num = -1;				//Ŀ¼���I�����Ϣ�ָ���ֵ
			strcpy(root[i].dir_name, "");	//Ŀ¼����ļ�Ŀ¼��Ϣ�ָ���ֵ
			i_node[k].file_length = -1;		//�ļ����Ȼָ�
			i_node[k].file_style = -1;		//�ļ����ͻָ���ֵ
			break;
		}
	}
	if (i == 640)
	{
		printf("��¼�û��ĸ�Ŀ¼�²���������ļ�\n");
	}
}

/*���ļ�*/
int open(char filename[10]){
	int i;
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1 && i_node[root[i].i_num].file_UserId == login_userid)//�ҵ����ļ�
		{
			if (file_array_head<20)//����֧�ִ�8���ļ�
			{
				file_array[file_array_head] = root[i].i_num;//�����ļ���ID�Ŵ浽file_array�У���ʾ�ļ��Ѿ���
				file_array_head++;
			}
			else
			{
				printf("�򿪵��ļ��Ѵ����ޣ��޷��򿪱��ļ�\n");
			}
			return root[i].i_num;
		}
	}
	if (i = 640)
	{
		printf("��Ҫ�򿪵��ļ������ڻ����ڸ��û�\n");
	}
	return 0;
}

/*�ر��ļ�*/
void close(char filename[10]){
	int i;
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1 && i_node[root[i].i_num].file_UserId == login_userid)//�ҵ����ļ�
		{
			int j;
			for (j = 0; j<file_array_head; j++)
			{
				if (root[i].i_num == file_array[j])//��file_array���ҵ����ļ�ID�ţ���ʾ���ļ��Ѿ�����
				{
					int m;
					for (m = j; m<file_array_head; m++)
					{
						file_array[m] = file_array[m + 1];//��filr_array�н���ID��ɾ������������򿪵�ID����ǰһ��λ��
					}
					file_array_head--;
					return;
				}
			}
			if (j == file_array_head)
			{
				printf("��Ҫ�رյ��ļ�δ�򿪹���\n");
			}
			return;
		}
	}
	if (i = 640)
	{
		printf("��Ҫ�رյ��ļ������ڻ����ڸ��û�\n");
	}
	return;
}

/*��ȡ�ļ�*/
int read(char filename[10]){
	int i;
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1)//�ҵ����ļ�
		{
			int j;
			char temp_read[i_node[root[i].i_num].file_length];
			for (j = 0; j<8; j++)
			{
				if (root[i].i_num == file_array[j])//���Ѿ��򿪵��ļ����ҵ����ļ���ID��
				{
					if (i_node[root[i].i_num].limit == 0 || i_node[root[i].i_num].limit == 1)//ӵ�����Ȩ��
					{
						int c, add;
						for (add = 0; add < i_node[root[i].i_num].file_length; add++) {
							temp_read[add]= memory[i_node[root[i].i_num].file_address[add]].content;
						}
						cout << endl;
                        cout << "�ļ�����Ϊ��";
	                    cout << temp_read << endl;
					}
					else
					{
						printf("��û��Ȩ�޶�ȡ�ļ����ݣ���\n");
					}
					return 0;
				}
			}
			if (j == 8)
			{
				printf("\n  ���ļ�δ�򿪣����ȴ��ļ��ٽ��ж�д����!!\n");
			}
			return 0;
		}
	}
	if (i == 640)
	{
		printf("��Ҫ��ȡ���ļ�������\n");
	}
	return 0;
}

/*д���ļ�*/
void write(char filename[10], string writec, int s_len){
	int i;
	int c=0;
	char tmp_writec[s_len];
	strcpy(tmp_writec,writec.c_str());    //��writec���ַ�������ת��Ϊ�ַ���������
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1)//�ҵ����ļ�
		{
			int j;
			for (j = 0; j<8; j++)
			{
				if (root[i].i_num == file_array[j])//ȷ�ϸ��ļ��Ѿ���
				{
					if (i_node[root[i].i_num].limit == 0 || i_node[root[i].i_num].limit == 2)//ȷ�����Ȩ��
					{
						int c, add, write_length;
						if((s_len+i_node[root[i].i_num].use_length)>i_node[root[i].i_num].file_length)
                        {
                            cout<<"�ļ��ռ���������������д��ʧ�ܣ�"<<endl;
                        }
						for (add = i_node[root[i].i_num].use_length; add< i_node[root[i].i_num].file_length; add++)
						{
								if (memory[i_node[root[i].i_num].file_address[add]].content == '\0')    //����ÿ�û�б�д��
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
						printf("��û��Ȩ�޽�����д���ļ�����\n");
						return;
					}
					return;
				}
			}
			if (j == 8)
			{
				printf("\n  ���ļ�δ�򿪣����ȴ��ļ��ٽ��ж�д����!!\n");
			}
			return;
		}
	}
	if (i == 640)
	{
		printf("��Ҫд����ļ�������\n");
	}
	return;
}

/*��ʾ�ļ���Ϣ*/
void show_file(char filename[]){
	int i, j, k, p;
	cout << '\t' << "�ļ�����  ��ȡȨ��  ��˽Ȩ��  ����Ŀ¼  �����û�" << endl;
	for (i = 0; i<640; i++)
	{
		k = root[i].i_num;
		if (strcmp(filename, root[i].file_name) == 0 && (i_node[k].file_style == 1))
		{
		    cout << setw(16) << root[i].file_name;     //�ļ���
			if(i_node[k].limit==0)
                {cout << setw(8) << "o+r+w";}
            else if(i_node[k].limit==1)
                {cout << setw(8) << "o+r";}
            else if(i_node[k].limit==2)
                {cout << setw(8) << "o+w";}
            else if(i_node[k].limit==3)
                {cout << setw(8) << "o";}         //��ȡȨ��
            else
                {cout << setw(8)<< "error";}

            if(i_node[k].privilege==0)
                {cout << setw(12) << "�����ɼ�";}
            else if(i_node[k].privilege==1)
                {cout << setw(12) << "�Լ��ɼ�";}
            else
                {cout << setw(12) << "error";}               //��˽Ȩ��
            cout << setw(11) << root[i].dir_name;		//�ļ����ڵ�Ŀ¼
			cout << setw(6) << user_array[i_node[k].file_UserId].username << endl;//�ļ������û�
			cout << "�ļ�ռ�õ������ַ:" << endl;
			for (j = 0; j<i_node[k].file_length; j++)   //��ʾ�����ַ
			{
			    cout << setw(3) << i_node[k].file_address[j];    //�ļ�����ռ�õ��̿��
			}
			printf("\n");
			break;
		}
	}
	if (i == 640)
	{
		printf("û������ļ� ��������ļ����������ļ�\n");
	}
}

/*�޸��ļ���Ϣ*/
void mod_file(char filename[]){
    int i;
    char tmp_filename[10];
    char tmp_limit;
    char tmp_privilege;
	for (i = 0; i<640; i++)
	{
		if (strcmp(root[i].file_name, filename) == 0 && i_node[root[i].i_num].file_style == 1 && i_node[root[i].i_num].file_UserId == login_userid)//�ҵ����ļ�
		{
		    cout<<"�������޸ĺ���ļ���(�10���ַ�):";
		    cin>> tmp_filename;
		    cout<<"�������޸ĺ�Ķ�ȡȨ��(0:o+r+w;1:o+r;2:o+w;3:o):";
		    cin>> tmp_limit;
		    while(tmp_limit!='0'&&tmp_limit!='1'&&tmp_limit!='2'&&tmp_limit!='3')
            {
                cout << "��������ȷȨ��(0:o+r+w;1:o+r;2:o+w;3:o):";
                cin >> tmp_limit;
            }
            cout<<"�������޸ĺ����˽Ȩ��(0:�����ɼ�;1:�Լ��ɼ�):";
            cin>> tmp_privilege;
            while(tmp_privilege!='0'&&tmp_privilege!='1')
            {
                cout << "��������ȷȨ��(0:�����ɼ�;1:�Լ��ɼ�):";
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
		printf("��Ҫ�޸ĵ��ļ���Ϣ�����ڻ���û�����Ȩ��\n");
	}
}

/*�����ļ�*/
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

/*ճ��*/
void paste(char filename[10]) {
	create_file(filename, temp_file.file_length, login_userid, temp_file.limit, temp_file.privilege);
	open(filename);
	write(filename, temp_write, temp_write.size());
	close(filename);
}
