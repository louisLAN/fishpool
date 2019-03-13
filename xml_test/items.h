#define MAC_SIZE 		12

typedef struct MacManage_t
{
	char 	Xname[32];
	char 	devicetype[8];
	char    TotalMacStart[32];
	char    TotalMacEnd[32];
	unsigned long    tSNstart;
}pMacManage;
typedef struct UsedMacRec_t
{
	int 	nodecount;
	char 	usage[32];
	int		usenum;
	char 	MacStart[32];
	char 	MacEnd[32];
	unsigned long 	SNstart;
	unsigned long 	SNend;
}pUsedMacRec;

typedef struct MAC_S //���ʹ��char * ����Ϊ�ڴ��ι����У�char�ַ����͵��ֽڴ�С���ƣ�����16���ֽڵ��ַ��޷���ȷ�ش��ݡ�
{
char mac_v[20];
}pmac;

/*********************************
���ܣ�������ת��Ϊ�����ŵ�MAC��ַ�ַ���
���룺�������� unsigned long
�����mac�ַ��� pmac 
***********************************/
 int INTtoMac( unsigned long nummac,pmac * macfinal);
 /*********************************
���ܣ���MAC��ַ�ַ�����ת��Ϊ��������
���룺 longmac�ַ��� pmac 
������������� unsigned long
***********************************/
unsigned long MacToHex(pmac *macstr);

 /*********************************
���ܣ���16���������ַ�����ת��Ϊ����
���룺16�����ַ���
������������� unsigned long 
***********************************/

unsigned long HexSTRtoNum(pmac *hexstr);

 /*********************************
���ܣ��ж�����MAC��ַ�Ƿ�ͬһ��MAC��ַ��
���룺mac1,mac2
�����
***********************************/

int comp_2mac_head(pmac *mac1,pmac *mac2);


