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

typedef struct MAC_S //如果使用char * 会因为在传参过程中，char字符类型的字节大小限制，超出16个字节的字符无法正确地传递。
{
char mac_v[20];
}pmac;

/*********************************
功能：将整型转换为带：号的MAC地址字符串
输入：整型数字 unsigned long
输出：mac字符串 pmac 
***********************************/
 int INTtoMac( unsigned long nummac,pmac * macfinal);
 /*********************************
功能：将MAC地址字符串，转换为整型数字
输入： longmac字符串 pmac 
输出：整型数字 unsigned long
***********************************/
unsigned long MacToHex(pmac *macstr);

 /*********************************
功能：将16进制数字字符串，转换为数字
输入：16进制字符串
输出：整型数字 unsigned long 
***********************************/

unsigned long HexSTRtoNum(pmac *hexstr);

 /*********************************
功能：判断两个MAC地址是否同一个MAC地址段
输入：mac1,mac2
输出：
***********************************/

int comp_2mac_head(pmac *mac1,pmac *mac2);


