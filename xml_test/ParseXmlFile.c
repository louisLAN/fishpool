#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "items.h"

/*获取总的macsn节点属性*/

/*1.根据devicetype确定使用哪个总mac节点
2.获取目前已使用的macsn，比较节点号大小，选择最大的节点号，获取节点属性，生成新的usedmac节点。
*/
xmlNodePtr get_totalmacnode_by_devicetype(xmlNodePtr root_node,char *devicetype)
{
	xmlNodePtr dTotalMacnode=NULL;
	xmlNodePtr curNode=NULL;
	curNode = root_node->xmlChildrenNode;
	dTotalMacnode=curNode;
    while(curNode != NULL) {
        //查找带有属性attribute的节点
        if (xmlHasProp(dTotalMacnode,BAD_CAST "devicetype")) {
            dTotalMacnode = curNode;
			xmlChar* szAttr = xmlGetProp(dTotalMacnode,BAD_CAST "devicetype");
			if(!xmlStrcmp(szAttr, BAD_CAST devicetype))
				{
					xmlFree(szAttr);
					break;
				}
        }
        curNode = curNode->next;
    }
	return dTotalMacnode;
}
int main(int argc, char* argv[])
{
    xmlDocPtr doc;           //定义解析文件指针
    xmlNodePtr curNode;      //定义结点指针
    xmlChar *szKey;          //临时字符串变量
    char *szDocName;
    if (argc <= 1) {
        printf("Usage: %s docname", argv[0]);
        return(0);
    }
    szDocName = argv[1];
    doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER);
    //解析文件
    //检查解析文档是否成功，如果不成功，libxml将报错并停止解析。
    //一个常见错误是不适当的编码，XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存
    if (NULL == doc) {
        fprintf(stderr,"Document not parsed successfully.");
        return -1;
    }
    //获取根节点
    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        fprintf(stderr,"empty document");
        xmlFreeDoc(doc);
        return -1;
    }
    //确认根元素名字是否符合
    if (xmlStrcmp(curNode->name, BAD_CAST "root")) {
        fprintf(stderr,"document of the wrong type, root node != root");
        xmlFreeDoc(doc);
        return -1;
    }
    curNode = curNode->xmlChildrenNode;
    xmlNodePtr propNodePtr = curNode;
    while(curNode != NULL) {
        //取出节点中的内容
        if ((!xmlStrcmp(curNode->name, (const xmlChar *) "newNode1"))) {
            szKey = xmlNodeGetContent(curNode);
            printf("newNode1: %s\n", szKey);
            xmlFree(szKey);
        }
        //查找带有属性attribute的节点
        if (xmlHasProp(curNode,BAD_CAST "attribute")) {
            propNodePtr = curNode;
        }
        curNode = curNode->next;
    }
 
    //查找属性
    xmlAttrPtr attrPtr = propNodePtr->properties;
    while (attrPtr != NULL) {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "attribute")) {
            xmlChar* szAttr = xmlGetProp(propNodePtr,BAD_CAST "attribute");
            printf("get attribute=%s\n", szAttr) ;
            xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
    }
    xmlFreeDoc(doc);
    return 0;
}
