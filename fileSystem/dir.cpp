#include "filesystem.h"

node i_node[640];				//i�ڵ�
dir root[640];					//�ļ����ڿ�
char cur_dir[10][10] = {"filsystem","filsystem","filsystem","filsystem","filsystem","filsystem","filsystem","filsystem","filsystem","filsystem"};//��ǰĿ¼

/*��ʾ��ǰĿ¼�µ��ļ��б�*/
void display_curdir(){
	int i, k;
	cout << "�û�����" << user_array[login_userid].username << endl;
	cout << '\t' << "�ļ�����  �ļ�����  �ļ�����  ����Ŀ¼  �����û�" <<endl;
	for (i = 0; i<640; i++)
	{
	    if(strcmp(cur_dir[login_userid], root[i].dir_name) == 0 && i_node[root[i].i_num].privilege == 0)
        {
            k = root[i].i_num;
			cout<< setw(16) << root[i].file_name;	//�ļ���
			if(i_node[k].file_style==0)                     //�ļ�������
            {
                cout<< setw(8) <<"Ŀ¼";
            }
            else if(i_node[k].file_style==1)
            {
                cout<< setw(8) <<"�ļ�";
            }
            cout<< setw(9) <<i_node[k].file_length;		//�ļ��ĳ���
            cout<< setw(14) <<root[i].dir_name;			//�ļ����ڵ�Ŀ¼
            cout<< setw(8) <<user_array[i_node[i].file_UserId].username<<endl;    //�ļ��������û�
        }
		else if (strcmp(cur_dir[login_userid], root[i].dir_name) == 0 && i_node[root[i].i_num].file_UserId == login_userid)   //��ѯ�ļ��� ����Ŀ¼��Ϣ�͵�ǰĿ¼��Ϣ��ͬ������
		{
			k = root[i].i_num;
			cout<<'\t'<<'\t'<<"  "<< root[i].file_name <<'\t';	//�ļ���
			if(i_node[k].file_style==0)                     //�ļ�������
            {
                cout<<'\t'<<"Ŀ¼"<<'\t';
            }
            else if(i_node[k].file_style==1)
            {
                cout<<'\t'<<"�ļ�"<<'\t';
            }
            cout<<i_node[k].file_length<<'\t';		//�ļ��ĳ���
            cout<<root[i].dir_name<<'\t';			//�ļ����ڵ�Ŀ¼
		}
	}
}

/*����ָ����Ŀ¼*/
void display_dir(char filename[]){
	int i, k;
	for (i = 0; i<640; i++)
	{
		k = root[i].i_num;

		//�ж��ļ������ǲ���Ŀ¼����
		if ((strcmp(filename, root[i].file_name) == 0) && (i_node[k].file_style == 0) && (i_node[k].file_UserId == login_userid))
		{
			strcpy(cur_dir[login_userid], filename);  //��Ҫ�����ָ��Ŀ¼����Ϊ��ǰĿ¼  ��ֵ��Ҫ����strcpy(Ŀ�ģ�Դ)
			return;
		}
	}
	if (i == 640)
	{
		printf("��¼�û�û�����Ŀ¼\n");
	}
}

/*������һ��Ŀ¼*/
void back_dir(){
	int  i, k;
	for (i = 0; i<640; i++)       //��ѯ�͵�ǰĿ¼����ͬ��Ŀ¼�ļ���
	{
		k = root[i].i_num;
		if (strcmp(cur_dir[login_userid], root[i].file_name) == 0 && (i_node[k].file_style == 0))
		{
			strcpy(cur_dir[login_userid], root[i].dir_name); //����ѯ����Ŀ¼�ļ���  ���ڵ�Ŀ¼��ֵ����ǰĿ¼
		}
	}
}

/*����Ŀ¼*/
void create_dir(char filename[]){
	style = 0;         //0�����ļ�������Ŀ¼�ļ�
	create_file(filename, 4, login_userid, 0, 0);
	style = 1;         //����ָ���ֵ
}

/*ɾ��Ŀ¼*/
void del_dir(char filename[]){
	int i, j, k;
	for (i = 0; i<640; i++)       //�ж�Ҫɾ����Ŀ¼�ǲ��ǵ�ǰĿ¼
	{
		k = root[i].i_num;      //�ҵ�Ŀ¼����
		if (strcmp(root[i].file_name, filename) == 0 && strcmp(cur_dir[login_userid], filename) != 0 && (i_node[k].file_style) == 0)
		{
			for (j = 0; j<640; j++)
			{
				if (strcmp(filename, root[j].dir_name) == 0)
				{
					printf("Ŀ¼��Ϊ�ղ���ֱ��ɾ��\n");
					break;
				}
			}
			if (j == 640 || i_node[root[i].i_num].file_UserId == login_userid)
			{
				del_file(filename);
				break;
			}
			break;
		}
	}
	if (i == 640)
	{
		printf("�������Ŀ¼�ļ� �����ѵ�¼�û����������Ŀ¼,������Ҫɾ�����ǵ�ǰĿ¼\n");
	}
}
