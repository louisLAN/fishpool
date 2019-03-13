   /****************************************
*将整型数据转换位mac地址以及将mac地址转换为
*整型数据。
*
*@author: liuyi  @date: 2019/01/28
* ***************************************/

#include "stdio.h"
#include "string.h"
#include "items.h"
#include <assert.h>


int INTtoHexSTR( unsigned long num,pmac *macstr)
{
	char hexstr[64]={0};
	sprintf(hexstr,"%llX",num);//mac最大超过32位因此要使用llX
	strncpy(macstr->mac_v,hexstr,sizeof(macstr->mac_v));
	printf("hexstr=%s\n",hexstr);
	return 0;
	
	
}
unsigned long HexSTRtoNum(pmac *hexstr)
{
	char headstr[10]={0};
	char tailstr[10]={0};
	char *p;
	int len=0;
	unsigned long finalnum,headnum=0,tailnum=0;
	
	if(strlen(hexstr->mac_v)>8)
		{
			len=strlen(hexstr->mac_v);
			len-=8;
		}
	p=hexstr->mac_v;
	p+=len;
	if(len>0)
		{
			strncpy(headstr,hexstr->mac_v,len);
			sscanf(headstr,"%llx",&headnum);
		}
		strncpy(tailstr,p,8);
		printf("headstr =%s tailstr= %s\n",headstr,tailstr);
		sscanf(tailstr,"%llx",&tailnum);
		finalnum=headnum*0x100000000+tailnum;
	return finalnum;
	
}
int addcolon(pmac *macstr,pmac *macout)
{
	int i;
		   char *q,*f;
		   char tmpmac[32]={0};
		   char finalmac[32]={0};
		   char macstd[32]={0};
		 
		   strcpy(macstd,macstr->mac_v);
		   q=macstd;
		   for(i=0;i<12;i++)
				   {
					if(i !=0&&i%2==0)
						{
							strncpy(tmpmac,q,2);
							strncat(tmpmac,":",3);
							strncat(finalmac,tmpmac,3);
							q++;
							q++;
							if(i==10)
							{
							strncat(finalmac,q,2);
							}
						}
	
				   }
		   strncpy(macout->mac_v,finalmac,sizeof(macout->mac_v)-1);
		   return 0;
}
int addcodem(pmac * macnum,pmac *macout)
{	
	assert(macnum);
	int i;
	char *allzeromac="000000000000";
	char *mactype;
	char *p;
	char tmp[MAC_SIZE]={0};
	if(strlen(macnum->mac_v)<MAC_SIZE)//在不足12位mac前面补0
		{
		p=allzeromac;
		for(i=0;i<strlen(macnum->mac_v);i++)
			{
			p++;
			}
		strncpy(tmp,p,strlen(p));
		strcat(tmp,macnum->mac_v);
		strcpy(macnum->mac_v,tmp);
		addcolon(macnum,macout);//添加冒号
		return 0;
		}
	else
		{	
			addcolon(macnum,macout);
			printf("mactype=%s\n",macout->mac_v);
			return 0;
		}

}
 int INTtoMac( unsigned long nummac, pmac *MacFinale )
{
	char *MacHex;
	pmac tmpMacFinale;
	pmac mac_std;
	
	INTtoHexSTR(nummac,&mac_std);//10进制转换为16进制再转化为字符串
	printf("machex=0x%s\n",mac_std.mac_v);
	addcodem(&mac_std,&tmpMacFinale);//在字符串内加上“：”
	strncpy(MacFinale->mac_v,(char *)&(tmpMacFinale.mac_v),sizeof(MacFinale->mac_v)-1);
	return 0;

}

 pmac * MacToHexSTR(pmac *macstr)
 {
	 char ahex[16]={0};
	 char tmp[20]={0};
	 char *p,*q;
	 int i,len;
	 unsigned long fhexnum=0x0;
	 pmac *hexCstr=NULL; //传参不能直接用char *，否则只会有8个字节大小。
	 hexCstr=(pmac *)malloc(sizeof(pmac));
	 strncpy(tmp,macstr->mac_v,sizeof(macstr->mac_v));
 
	 p=tmp;
	 q=p;
	 p++;
	 len=strlen(tmp);
	 for(i=1;i<len+1;i++)
		 {
			 if(*p==':' ||*p==0)
			 {
				 *p=0;
				 strncat(ahex,q,2);
				 q+=3;
			 }
			 p++;
		 }
	 memcpy(hexCstr->mac_v,ahex,sizeof(pmac));//使用pmac *初始化的对象和 char *同理，需要申请内存空间，否则不能被赋值。
	 return hexCstr;
 }


unsigned long MacToHex(pmac *macstr)
{
	unsigned long fhexnum=0;
	fhexnum=HexSTRtoNum((pmac *)MacToHexSTR(macstr));
	return fhexnum;
}

int comp_2mac_head(pmac *mac1,pmac *mac2)
{
	char s_head1[10]={0};
	char s_head2[10]={0};
	pmac head1,head2;
	int len1=0,len2=0;
	unsigned long headnum1=0,headnum2=0;
	if(strlen(mac1) <17 || strlen(mac2) <17)
		{
			printf("not a mac address!!\n");
		}
	memcpy(&head1,MacToHexSTR(mac1),sizeof(pmac));
	memcpy(&head2,MacToHexSTR(mac2),sizeof(pmac));
	
	if(strlen(&head1.mac_v)>6)
		{
			len1=strlen(&head1.mac_v);
			len1-=6;
			strncpy(s_head1,&head1.mac_v,len1);
			sscanf(s_head1,"%llx",&headnum1);
		}
	
	if(strlen(&head2.mac_v)>6)
		{
			len2=strlen(&head2.mac_v);
			len2-=6;
			strncpy(s_head2,&head2.mac_v,len2);
			sscanf(s_head2,"%llx",&headnum2);
		}
	printf("headnum1= %llx,headnum2 = %llx\n",headnum1,headnum2);
	if(headnum1 != headnum2)
		{
			return 1;
		}
	else
		    return 0;
	
}

