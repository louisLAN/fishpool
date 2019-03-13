	/****************************************
 *练习libxml库，创建MACSN xml文档，新增一个MacSN统计表，
 *如果xml文件存在，则添加一个新节点
 *如果xml文件不存在，则新建一个xml文件
 *
 *@author: liuyi  @date: 2019/01/24
 * ***************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "items.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

#define MACSN_MANAGE_FILE     "macsn_manage.xml"
int nodecount=0;

void parseTotalmacNode(xmlNodePtr totalmacnode, pMacManage *TotalMacsn)
{
	pmac t_sn;
	strncpy(TotalMacsn->Xname,	xmlGetProp(totalmacnode,"Xname"),sizeof(TotalMacsn->Xname));
	strncpy(TotalMacsn->devicetype,	xmlGetProp(totalmacnode,"devicetype"),sizeof(TotalMacsn->devicetype));
	strncpy(TotalMacsn->TotalMacStart,	xmlGetProp(totalmacnode,"TotalMacStart"),sizeof(TotalMacsn->TotalMacStart));
	strncpy(TotalMacsn->TotalMacEnd,	xmlGetProp(totalmacnode,"TotalMacEnd"),sizeof(TotalMacsn->TotalMacEnd));
	
	strcpy(&t_sn.mac_v,xmlGetProp(totalmacnode,"tSNstart"));
	printf("tSNstart=%s\n",&t_sn.mac_v);
	TotalMacsn->tSNstart=HexSTRtoNum(&t_sn);
	printf("tSNstart=%X\n",TotalMacsn->tSNstart);
}
/*获取使用过的macsn节点属性*/
void parseUsedmacNode(xmlNodePtr Usedmacnode,pUsedMacRec *Usedmacsn)
{	
	pmac usenum,sn_start,sn_end,t_count;

	strcpy(&t_count.mac_v,xmlGetProp(Usedmacnode,"nodecount"));
	Usedmacsn->nodecount		=	HexSTRtoNum(&t_count);
	strcpy(&usenum.mac_v,	xmlGetProp(Usedmacnode,"usenum"));
	Usedmacsn->usenum			=  	HexSTRtoNum(&usenum);
	strncpy(Usedmacsn->usage,	xmlGetProp(Usedmacnode,"usage"),sizeof(Usedmacsn->usage)-1);
	strncpy(Usedmacsn->MacStart,	xmlGetProp(Usedmacnode,"MacStart"),sizeof(Usedmacsn->MacStart)-1);
	strncpy(Usedmacsn->MacEnd,	xmlGetProp(Usedmacnode,"MacEnd"),sizeof(Usedmacsn->MacEnd)-1);
	
	strcpy(&sn_start.mac_v,	xmlGetProp(Usedmacnode,"SNstart"));
	Usedmacsn->SNstart			=	HexSTRtoNum(&sn_start);
	strcpy(&sn_end.mac_v,		xmlGetProp(Usedmacnode,"SNend"));
	Usedmacsn->SNend			=	HexSTRtoNum(&sn_end);
	printf("tSNend=%s\n",&sn_end.mac_v);
	printf("SNend=%X\n",Usedmacsn->SNend);
	
}


xmlNodePtr getValueFromNode(xmlNodePtr p_node,char *prop,char * attr)
{
	xmlNodePtr ddnode=NULL;
	xmlNodePtr curNode=NULL;
	curNode = p_node->xmlChildrenNode;
	if(curNode==NULL)
		{
			curNode=p_node;
			printf("no children node\n");
		}
	ddnode=curNode;
    while(curNode != NULL) {
        //查找带有属性attr的节点
        if (xmlHasProp(ddnode,BAD_CAST attr)) {
            ddnode = curNode;
			xmlChar* szAttr = xmlGetProp(ddnode,BAD_CAST attr);
			printf("szAttr=%s\n",szAttr);
			if(!xmlStrcmp(szAttr, BAD_CAST prop))
				{
					xmlFree(szAttr);
					return ddnode;
				}
        }
        curNode = curNode->next;
    }
	return NULL;
}



int get_nodecount_at_totalmac(xmlNodePtr total_node)
{
	int i=0;
	xmlNodePtr dusedMacnode=NULL;
	xmlNodePtr curNode=NULL;
	curNode = total_node->xmlChildrenNode;
	if(curNode==NULL)
		{
			printf("root_node wrong!!\n");
		}
	dusedMacnode=curNode;
    while(curNode != NULL) {
        //查找带有属性attribute的节点
        if (xmlHasProp(dusedMacnode,BAD_CAST "nodecount")) {
            dusedMacnode = curNode;
			i++;
        }
        curNode = curNode->next;
    }
	return i;
}
/*获取使用过的最新一个节点属性值*/
int findMaxUsedMac(xmlNodePtr s_node,int m_nodecount,char *devicetype,pUsedMacRec *NewUsedMac)
{
	char c_nodecount[8]={0};
	xmlNodePtr newusedmac_node=NULL;
	sprintf(c_nodecount,"%d",m_nodecount);
	newusedmac_node=getValueFromNode(s_node,c_nodecount,"nodecount");
	parseUsedmacNode(newusedmac_node,NewUsedMac);
	printf("new snend=%X\n",NewUsedMac->SNend);
	return 0;
	
}

/*根据描述，数量，devicetype在totalmac节点下添加一个新的usedmac节点*/
int addNewUsedmacNode(xmlNodePtr root_node,char * device_type,int usenum, char * usage,pUsedMacRec *MacManage_item)
{
	int gnodecount=0;
	int iRet=0;
	unsigned long macstart=0;
	unsigned long macend=0;
	unsigned long snstart=0;
	unsigned long snend=0;
	xmlNodePtr sTotalMacNode=NULL;
	pUsedMacRec *tNewUsedMac=NULL;
	pMacManage *tTotalMacsn=NULL;
   if(MacManage_item==NULL)
	{
		return -1;
	}
   	pmac t_mac,t_sn;
   	memset(&t_mac,0,sizeof(pmac));
   	

	tNewUsedMac=(pUsedMacRec *)malloc(sizeof(pUsedMacRec));
	tTotalMacsn=(pMacManage *)malloc(sizeof(pMacManage));

	sTotalMacNode=getValueFromNode(root_node,device_type,"devicetype");
	gnodecount=get_nodecount_at_totalmac(sTotalMacNode);
	strncpy(MacManage_item->usage,usage,sizeof(MacManage_item->usage)-1);
			MacManage_item->nodecount=gnodecount+1;
			MacManage_item->usenum=usenum;
	parseTotalmacNode(sTotalMacNode,tTotalMacsn);	//为什么这个地方用&取出来的和用 指针取出来的SN值不一样？	
	if(gnodecount)
		{
			iRet=findMaxUsedMac(sTotalMacNode,gnodecount,device_type,tNewUsedMac);
			printf("get snend=%X\n",tNewUsedMac->SNend);
			/*mac 地址计算*/
			macstart=MacToHex(tNewUsedMac->MacEnd)+1;
			INTtoMac(macstart, &t_mac);
			strcpy(MacManage_item->MacStart,&t_mac.mac_v);
			macend=macstart+usenum-1;
			if(macend > MacToHex(tTotalMacsn->TotalMacEnd))
				{
					printf("not enoungh mac in pool ，macend=%llx,totalmac_end= %s\n",macend,tTotalMacsn->TotalMacEnd);
					return -1;
				}
			INTtoMac(macend, &t_mac);
			strcpy(MacManage_item->MacEnd,&t_mac.mac_v);

			/*sn计算*/
			MacManage_item->SNstart=tNewUsedMac->SNend+1;
			MacManage_item->SNend=MacManage_item->SNstart+usenum-1;
			printf("sSNend=%X\n",tNewUsedMac->SNend);
			printf("snstart= %X,snend= %X\n",MacManage_item->SNstart,MacManage_item->SNend);
			
		}
	else
		{
			memset(&t_mac,0,sizeof(pmac));
			

			macstart=MacToHex(tTotalMacsn->TotalMacStart)+1;
			INTtoMac(macstart, &t_mac);
			strcpy(MacManage_item->MacStart,&t_mac.mac_v);

			macend=macstart+usenum-1;
			if(macend > MacToHex(tTotalMacsn->TotalMacEnd))
				{
					printf("not enoungh mac in pool ，macend=%x,totalmac_end= %s\n",macend,tTotalMacsn->TotalMacEnd);
					return -1;
				}
			INTtoMac(macend, &t_mac);
			strcpy(MacManage_item->MacEnd,&t_mac.mac_v);

			/*sn计算*/
			MacManage_item->SNstart=tTotalMacsn->tSNstart;
			MacManage_item->SNend=MacManage_item->SNstart+usenum-1;
			printf("snstart= %X,snend= %X\n",MacManage_item->SNstart,MacManage_item->SNend);
			
		}
	free(tNewUsedMac);
	free(tTotalMacsn);
	return 0;
}


static int set_Origin_item(pMacManage *MacManage_item)
{
   if(MacManage_item==NULL)
	{
		return -1;
	}
   pmac test;
    unsigned long mac=0x1212ffff9fff;
   	INTtoMac(mac,&test);
	mac=MacToHex(&test);
    sprintf(MacManage_item->Xname, "%s", "中文表示");
    sprintf(MacManage_item->devicetype, "%s", "H2r-1");
	sprintf(MacManage_item->TotalMacStart,"%s","00:E4:00:00:00:00");
	sprintf(MacManage_item->TotalMacEnd,"%s","00:e4:00:18:69:da");
	MacManage_item->tSNstart=0xA4100000;
	printf("hexnum= %llx\n",mac);
}
static int set_UsedMac_item(xmlNodePtr root_node,char * device_type,int usenum, char * usage,pUsedMacRec *MacManage_item)
{
	int gnodecount=0;
	xmlNodePtr sTotalMacNode=NULL;
   if(MacManage_item==NULL)
	{
		return -1;
	}
   pmac test;
   	sTotalMacNode=getValueFromNode(root_node,device_type,"devicetype");
	gnodecount=get_nodecount_at_totalmac(sTotalMacNode);
    unsigned long mac=0x1212ffff9fff;
   	INTtoMac(mac,&test);
	mac=MacToHex(&test);
	MacManage_item->nodecount=gnodecount;
    sprintf(MacManage_item->usage, "%s", "测试节点1");
    MacManage_item->usenum=usenum;
	sprintf(MacManage_item->MacStart,"%s","00:00:00:00:00:00");
	sprintf(MacManage_item->MacEnd,"%s",&test.mac_v);
	MacManage_item->SNstart=0xA4000000;
	MacManage_item->SNend=MacManage_item->SNstart + MacManage_item->usenum;
	printf("hexnum= %llx\n",mac);
}

//创建TotalMac节点
static xmlNodePtr create_TotalMac_node(const pMacManage *MacManage_item)
{
        assert(MacManage_item);
    xmlNodePtr TotalMac_node = NULL;
    char Xname[32]={0};
	char TotalMacStart[32]={0};
	char TotalMacEnd[32]={0};
	char tSNstart[16]={0};
	
    TotalMac_node = xmlNewNode(NULL, BAD_CAST"TotalMac");
    if (TotalMac_node == NULL) {
    fprintf(stderr, "Failed to create new node.\n");
    return NULL;
    }
    //设置属性
    sprintf(Xname, "%s", "中文表示");
    xmlNewProp(TotalMac_node, BAD_CAST"Xname", (xmlChar*)Xname);

    xmlNewProp(TotalMac_node, BAD_CAST"devicetype", (xmlChar *)MacManage_item->devicetype);
	
    xmlNewProp(TotalMac_node, BAD_CAST"TotalMacStart", (xmlChar *)MacManage_item->TotalMacStart);
    xmlNewProp(TotalMac_node, BAD_CAST"TotalMacEnd", (xmlChar *)MacManage_item->TotalMacEnd);

	sprintf(tSNstart,"%X",MacManage_item->tSNstart);
	xmlNewProp(TotalMac_node, BAD_CAST"tSNstart", (xmlChar *)tSNstart);
	
    return TotalMac_node;
}
//创建usedmac节点
static xmlNodePtr create_UsedMac_node(const pUsedMacRec *MacManage_item)
{
        assert(MacManage_item);
    xmlNodePtr UsedMac_node = NULL;
    char usenum[32]={0};
	char TotalMacStart[32]={0};
	char TotalMacEnd[32]={0};
	char tSNstart[16]={0};
	char tSNend[16]={0};
	
    UsedMac_node = xmlNewNode(NULL, BAD_CAST"UsedMac");
    if (UsedMac_node == NULL) {
    fprintf(stderr, "Failed to create new node.\n");
    return NULL;
    }
    //设置属性
	sprintf(usenum,"%d",MacManage_item->nodecount);
	xmlNewProp(UsedMac_node,BAD_CAST"nodecount", (xmlChar*)usenum);
    	
    sprintf(usenum,"%d",MacManage_item->usenum);
	xmlNewProp(UsedMac_node,BAD_CAST"usenum", (xmlChar *)usenum);
    xmlNewProp(UsedMac_node,BAD_CAST"usage", (xmlChar*)MacManage_item->usage);
	
    xmlNewProp(UsedMac_node, BAD_CAST"MacStart", (xmlChar *)MacManage_item->MacStart);
    xmlNewProp(UsedMac_node, BAD_CAST"MacEnd", (xmlChar *)MacManage_item->MacEnd);

	sprintf(tSNstart,"%X",MacManage_item->SNstart);
	xmlNewProp(UsedMac_node, BAD_CAST"SNstart", (xmlChar *)tSNstart);

	sprintf(tSNend,"%X",MacManage_item->SNend);
	xmlNewProp(UsedMac_node, BAD_CAST"SNend", (xmlChar *)tSNend);
	
    return UsedMac_node;
}
/*向总mac地址节点添加已使用mac地址节点*/
int add_usedmac_node_to_totalmacnode(xmlNodePtr totalmacnode, pUsedMacRec *new_used_mac)
{
	xmlNodePtr usedmac_node = NULL;
	//创建一个Used_macsn节点
    usedmac_node = create_UsedMac_node(new_used_mac); 
    if (usedmac_node == NULL) {
    fprintf(stderr, "Failed to create usedmac node.\n");
    goto FAILED;
    }
    //totalmac节点添加一个子节点
    xmlAddChild(totalmacnode, usedmac_node);

    return 0;
FAILED:
    if (new_used_mac){
    free(new_used_mac);
    }
    return -1;	
}

int add_usednode_to_totalmacnode_devicetype(xmlNodePtr root_node , char * devicetype , pUsedMacRec *new_used_mac)
{
	int ret=0;
	xmlNodePtr gtotalmacnode=NULL;
	/*根据devicetype找到对应totalmac节点*/
	gtotalmacnode=getValueFromNode(root_node,devicetype,"devicetype");
	/*根据节点添加新的usedmac节点*/
	ret=add_usedmac_node_to_totalmacnode(gtotalmacnode,new_used_mac);
	return ret;
	
}

//向根节点中添加一个TotalMac节点
static int add_totalmac_node_to_root(xmlNodePtr root_node,pMacManage *totalmac_item)
{
    xmlNodePtr totalmac_node = NULL;

	if(comp_2mac_head(totalmac_item->TotalMacStart,totalmac_item->TotalMacEnd))
		{
			printf("MAC format error!!\n");
			return -1;
		}
    //创建一个macsn节点
    totalmac_node = create_TotalMac_node(totalmac_item); 
    if (totalmac_node == NULL) {
    fprintf(stderr, "Failed to create phone node.\n");
    goto FAILED;
    }
    //根节点添加一个子节点
    xmlAddChild(root_node, totalmac_node);
    free(totalmac_item);
	
    return 0;
FAILED:
    if (totalmac_item){
    free(totalmac_item);
    }
    return -1;
}

/*删除某个totalmac节点
*
*入参：节点指针d_totalnode
*
*/
int DelMacNode(xmlNodePtr s_rootnode,char *prop,char *attr,char *devicetype)
{
	xmlNodePtr 	F_delnode=NULL;
	xmlNodePtr	d_DelNode=NULL;
	
	if(devicetype!=NULL)
		{	
			F_delnode=getValueFromNode(s_rootnode,devicetype,"devicetype");

			d_DelNode=getValueFromNode(F_delnode,prop,attr);

		}
	else
	d_DelNode=getValueFromNode(s_rootnode,prop,attr);
		xmlReplaceNode(d_DelNode,NULL);
	return 0;
}
static int create_macsn_xml(const char *macsn_manage_file)
{
    assert(macsn_manage_file);

    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
	pMacManage *totalmac_item = NULL;
	totalmac_item= (pMacManage *)malloc(sizeof(pMacManage));
	
    //创建一个xml 文档
    doc = xmlNewDoc(BAD_CAST"1.0");
    if (doc == NULL) {
    fprintf(stderr, "Failed to new doc.\n");
    return -1;
    }

    //创建根节点
    root_node = xmlNewNode(NULL, BAD_CAST"macsn_manage");
    if (root_node == NULL) {
    fprintf(stderr, "Failed to new root node.\n");
    goto FAILED;
    }
    //将根节点添加到文档中
    xmlDocSetRootElement(doc, root_node);
	set_Origin_item(totalmac_item);
    if (add_totalmac_node_to_root(root_node,totalmac_item) != 0) {
    fprintf(stderr, "Failed to add a new phone node.\n");
    goto FAILED;
    }
	
    //将文档保存到文件中，按照utf-8编码格式保存
    xmlSaveFormatFileEnc(macsn_manage_file, doc, "UTF-8", 1);
    //xmlSaveFile("test.xml", doc);
    xmlFreeDoc(doc);

    return 0; 
FAILED:
    if (doc) {
    xmlFreeDoc(doc);
    }

    return -1;
}

static int add_macsn_node(const char *macsn_manage_file)
{
    assert(macsn_manage_file);

    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    xmlNodePtr macsn_node = NULL;
    pMacManage *totalmac_item = NULL;
	pUsedMacRec * used_item=NULL;
	totalmac_item= (pMacManage *)malloc(sizeof(pMacManage));
	   used_item = (pUsedMacRec *)malloc(sizeof(pUsedMacRec));
    if (used_item == NULL) {
    fprintf(stderr, "Failed to malloc memory.\n");
    return -1;
    }
    xmlKeepBlanksDefault(0); //不添加的话，生成的xml文件会把所有node放到一行。
    doc = xmlParseFile(macsn_manage_file);
	
    if (doc == NULL) {
    fprintf(stderr, "Failed to parser xml file:%s\n", macsn_manage_file);
    return -1;
    }

    root_node = xmlDocGetRootElement(doc);
    if (root_node == NULL) 
		{
    		fprintf(stderr, "Failed to get root node.\n");
    		goto FAILED;
    	}
		addNewUsedmacNode(root_node,"H2r-1",300,"testnode2",used_item);
	if (add_usednode_to_totalmacnode_devicetype(root_node,"H2r-1",used_item) != 0)
		{
    		fprintf(stderr, "Failed to add a new phone node.\n");
    		goto FAILED;
    	}
	DelMacNode(root_node,"2","nodecount","H2r-1");
    //将文档保存到文件中，按照utf-8编码格式保存
    xmlSaveFormatFileEnc(macsn_manage_file, doc, "UTF-8", 1);
    xmlFreeDoc(doc);

    return 0;
FAILED:
    if (doc) {
    xmlFreeDoc(doc);
    }

    return -1;
}

int main(int argc, char *argv[])
{
    char *macsn_manage_file = MACSN_MANAGE_FILE;

    if (argc == 2) {
    macsn_manage_file = argv[1];
    }

    if (access(macsn_manage_file, F_OK) == 0) {
    //文件存在，添加一个新的macsn节点
    add_macsn_node(macsn_manage_file);
    }
    else {
    //文件不存在，创建一个信息的macsn
    create_macsn_xml(macsn_manage_file);
    }
    return 0;
}


