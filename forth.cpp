#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<ctime>
#include<algorithm>  //����sort����
using namespace std;
static int frame_num = 3;
int pid1, pid2;
float yes = 0, no = 0;
int S;//���������
int N;//�ڴ�ҳ����
int judge;//�ж��Ƿ�����

struct one_frame
{//��¼�ڴ�ҳ���ʹ�����
	int page_no;      //ҳ���
	int num;          //��¼ҳ������ڴ��ʱ�䣬ֵԽС��ʾ���ڴ�ʱ��Խ��
};


bool cmp(one_frame a, one_frame b)
{
	return a.num<b.num;
}

int main()
{
	printf("������������е�ҳ����:");
	//printf("Acess_Series_number:");
	scanf("%d", &S);
	printf("������Ϊһ�����̷���������ڴ�ҳ����:");
	scanf("%d", &N);
	struct one_frame M_Frame[N];    //�ṹ�������¼Ϊ���̷�����ڴ�ҳ���ʹ�����
	struct one_frame Copy_Frame[N];
	int ran_num;
	int j = 0;
	int Acess_Series[S];    //���ҳ��������е�����
	srand((unsigned)time(NULL));
	printf("ҳ���������Ϊ��");
	for (int s = 0; s<S; s++)
	{
		Acess_Series[s] = rand() % 5;
		cout << Acess_Series[s] << " ";
	}
	cout << endl;
	for (int s = 0; s<N; s++)
	{//��ʼ���ṹ������
		M_Frame[s].page_no = 8;
		M_Frame[s].num = 8;
	}
	if ((pid1 = fork()) == -1) { printf("failed!"); } //�����ӽ���1
	if (pid1 == 0)
	{//��Ϊ�ӽ���1
		printf("������LRU�㷨��ִ�н����\n");
		for (int i = 0; i<S; i++)
		{
			if (j<N)
			{//�ڴ�ҳ��δռ��
				judge = 0;
				for (int a = 0; a<N; a++)
				{//���У��˳�ѭ��������������һ��ҳ��
					if (M_Frame[a].page_no == Acess_Series[i])
					{
						judge = 1; cout << "��ǰ����ҳ��" << Acess_Series[i] << "����" << endl; yes = yes + 1;
					}
				}
				if (judge == 0)
				{
					M_Frame[j].page_no = Acess_Series[i];//δ���У�����ȱҳ��ҳ���
					M_Frame[j].num = N;                  //��ʼ��ģ��Ĵ�����ֵ
					for (int k = 0; k<j; k++) { M_Frame[k].num--; }//�ڴ�������ҳ��ļĴ���ֵ��һ
					cout << "��ǰ����ҳ��" << Acess_Series[i] << "δ���У������ڴ�" << endl;
					j++;
				}
			}
			else
			{//�ڴ�ҳ��ռ��
				judge = 0;
				for (int a = 0; a<N; a++)
				{
					Copy_Frame[a].page_no = M_Frame[a].page_no;
					Copy_Frame[a].num = M_Frame[a].num;
					if (M_Frame[a].page_no == Acess_Series[i])
					{//����
						M_Frame[a].num = N;//LRU�㷨������ʱ��Ӧҳ���ó�ֵ������״̬
						cout << "��ǰ����ҳ��" << M_Frame[a].page_no << "���У����¼Ĵ�����ֵ" << endl;
						for (int i = 0; i<N; i++)
						{//�ڴ�������ҳ��ļĴ���ֵ��һ
							if (M_Frame[i].page_no != M_Frame[a].page_no) { M_Frame[i].num--; }
						}
						yes = yes + 1;//���д�����һ
						judge = 1;   //�����б�־
					}
				}
				if (judge == 0)
				{//δ����
					int tihuan;
					sort(Copy_Frame, Copy_Frame + N, cmp);//���ڴ���ҳ��ļĴ���ֵ����������С��Ϊ������δʹ��ҳ�棬��̭
					tihuan = Copy_Frame[0].page_no;      //Ӧ�ñ��滻��ҳ���
					for (int b = 0; b<N; b++)
					{
						if (M_Frame[b].page_no == tihuan)
						{//������δʹ��ҳ���滻Ϊ��ǰ���ʵ�ҳ��
							M_Frame[b].page_no = Acess_Series[i];
							M_Frame[b].num = N;
						}
						else { M_Frame[b].num--; }//����ҳ��Ĵ���ֵ��һ
					}
					cout << "��ǰ����ҳ��" << Acess_Series[i] << ",ȱҳ����ҳ��" << tihuan << "�û����ڴ�" << endl;
				}
			}
			for (int i = 0; i<j; i++)
			{//����ڴ���ҳ���ʵʱ�����ҳ���--�Ĵ���ֵ
				printf("%d--%d  ", M_Frame[i].page_no, M_Frame[i].num);
			}
			printf("\n");
		}
		printf("����ҳ����:%f δ����ҳ����:%f\n", yes, S - yes);
		printf("������:%f\n", yes / S);
		printf("ȱҳ��:%f\n", 1 - yes / S);
	}

	else {
		if ((pid2 = fork()) == -1) { printf("failed!"); }//�����ӽ���2
		if (pid2 == 0)
		{ //��Ϊ�ӽ���2
			printf("������FIFO�㷨��ִ�н��!\n");
			for (int i = 0; i<S; i++)
			{
				if (j<N)
				{//�ڴ�ҳ��δռ��
					judge = 0;
					for (int a = 0; a<N; a++)
					{//���У��˳�ѭ��������������һ��ҳ��
						if (M_Frame[a].page_no == Acess_Series[i])
						{
							judge = 1; cout << "��ǰ����ҳ��" << Acess_Series[i] << "����" << endl; yes = yes + 1;
						}
					}
					if (judge == 0)
					{
						M_Frame[j].page_no = Acess_Series[i];//δ���У�����ȱҳ��ҳ���
						M_Frame[j].num = N;                   //��ʼ��ģ��Ĵ�����ֵ
						for (int k = 0; k<j; k++) { M_Frame[k].num--; }//�ڴ�������ҳ��ļĴ���ֵ��һ
						cout << "��ǰ����ҳ��" << Acess_Series[i] << "δ���У������ڴ�" << endl;
						j++;
					}
				}
				else
				{//�ڴ�ҳ��ռ��
					judge = 0;
					for (int a = 0; a<N; a++)
					{
						Copy_Frame[a].page_no = M_Frame[a].page_no;
						Copy_Frame[a].num = M_Frame[a].num;
						if (M_Frame[a].page_no == Acess_Series[i])
						{//����
							cout << "��ǰ����ҳ��" << M_Frame[a].page_no << "����" << endl;
							yes = yes + 1;//���д�����һ
							judge = 1;    //�����б�־
						}
					}
					if (judge == 0)
					{//δ����
						int tihuan;
						sort(Copy_Frame, Copy_Frame + N, cmp);//���ڴ���ҳ��ļĴ���ֵ����������С��Ϊ������δʹ��ҳ�棬��̭
						tihuan = Copy_Frame[0].page_no;//Ӧ�ñ��滻��ҳ���
						for (int b = 0; b<N; b++)
						{
							if (M_Frame[b].page_no == tihuan)
							{//������δʹ��ҳ���滻Ϊ��ǰ���ʵ�ҳ��
								M_Frame[b].page_no = Acess_Series[i];
								M_Frame[b].num = N;
							}
							else { M_Frame[b].num--; }//����ҳ��Ĵ���ֵ��һ
						}
						cout << "��ǰ����ҳ��" << Acess_Series[i] << ",ȱҳ����ҳ��" << tihuan << "�û����ڴ�" << endl;
					}
				}
				for (int i = 0; i<j; i++)
				{//����ڴ���ҳ���ʵʱ�����ҳ���--�Ĵ���ֵ
					printf("%d--%d  ", M_Frame[i].page_no, M_Frame[i].num);
				}
				printf("\n");
			}
			printf("����ҳ����:%f δ����ҳ����:%f\n", yes, S - yes);
			printf("������:%f\n", yes / S);
			printf("ȱҳ��:%f\n", 1 - yes / S);

		}
	}
	return 0;
}
