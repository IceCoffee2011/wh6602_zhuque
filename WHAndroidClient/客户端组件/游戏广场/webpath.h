#ifndef _WEBPATH_H__A4775701_F4E9_4d22_8B4D_856AB1BDC87C
#define _WEBPATH_H__A4775701_F4E9_4d22_8B4D_856AB1BDC87C

/*
����ʽ��ҳ��
			�ٷ���վ
			��Ϸ����
			�̳ǳ�ֵ
			�һ�����
			�ͷ�����
			ע���û������û�ʹ��Э��
			�˺Ź���
			��ֵ����
			�ͷ�����

��Ƕ��ҳ��
			�ҵı���
			��Ϸ�̳�
			����
			�ر���
			�û�����
			��Ӧ��Ϸ�����б�Ĺ��
			�������
*/

/*

#define		ONLINENET			1


#ifdef ONLINENET
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
//�û�ʹ��Э��
#define WEB_USER_DOUCOMET  (TEXT("http://www.cbwgame.com"))
//---------------------------

//�ٷ���վ
#define WEB_HOME_PAGE  (TEXT("http://www.cbwgame.com/index.php"))
//��Ϸ����
#define WEB_GAME_HELP  (TEXT("http://www.cbwgame.com/customer.php#game"))
//�ͷ�����
#define WEB_CLINET_CENTER  (TEXT("http://www.cbwgame.com/customer.php"))
//�һ�����
#define WEB_GET_PASSWORD  (TEXT("http://www.cbwgame.com/find_password.php"))
//���ⷴ��
#define WEB_ERROR_TICKLING  (TEXT("http://www.cbwgame.com/suggest.php?userid=%d&key=%s"))
//�˺Ź���
#define WEB_USER_ACCOUNTS_MANAGE (TEXT("http://www.cbwgame.com/user_center.php"))
//��¼���
#define WEB_LOGON_ADVERTISEMENT (TEXT("http://www.cbwgame.com/client/inner.php?mod=2"))
//��¼����
#define WEB_LOGON_NOTICE (TEXT("http://www.cbwgame.com/client/inner.php?mod=1"))
//���߿ͷ�
#define WEB_CONTACT_SERVICE (TEXT("http://chat56.live800.com/live800/chatClient/chatbox.jsp?companyID=170069&jid=8073229581&enterurl=http%3A%2F%2Fwww%2Ecbwgame%2Ecom%2F&pagereferrer=&tm=1321516662827"))
//��ȡ���
#define WEB_CONFIRM_GETGOLD (TEXT("http://cbwgame.com/payment.php"))
//ע���ʺ�
#define WEB_LOGON_REGISTER (TEXT("http://www.cbwgame.com/register.php"))
//�ٷ���վ(��¼)
#define WEB_LOGIN_HOME_PAGE  (TEXT("http://www.cbwgame.com/index.php?userid=%d&key=%s"))
//�˺Ź���(��¼)
#define WEB_LOGIN_ACCOUNTS_MANAGE (TEXT("http://www.cbwgame.com/user_center.php?userid=%d&key=%s"))
//�ʺų�ֵ
#define WEB_LOGIN_PAYMENT (TEXT("http://www.cbwgame.com/client/payment.php?userid=%d&key=%s"))
//��Ʒ�һ�
#define WEB_LOGIN_EXCHANGE (TEXT("http://www.cbwgame.com/shop.php?userid=%d&key=%s"))
//�����̳�
#define WEB_LOGIN_PROPSHOP (TEXT("http://www.cbwgame.com/client/propshop.php?userid=%d&key=%s"))
//�����̳�(����ʽ)
#define WEB_LOGIN_PROPSHOP_OPEN (TEXT("http://www.cbwgame.com/propshop.php?userid=%d&key=%s"))
//��������
#define WEB_LOGIN_TASK_NEW (TEXT("http://www.cbwgame.com/client/tasknewer.php?userid=%d&key=%s"))
//�����Ϣ
#define WEB_LOGIN_USERINFO (TEXT("http://www.cbwgame.com/client/inner.php?mod=5&userid=%d&key=%s"))
//ϵͳ�ʼ�
#define WEB_LOGIN_SYS_MESSAGE (TEXT("http://www.cbwgame.com/client/inner.php?mod=6&userid=%d&key=%s"))
//����
#define WEB_LOGIN_BANK (TEXT("http://www.cbwgame.com/client/inner.php?mod=7&userid=%d&key=%s"))
//����
#define WEB_LOGIN_BACKPACK (TEXT("http://www.cbwgame.com/client/inner.php?mod=8&userid=%d&key=%s"))
//������Ϸ����
#define WEB_GAME_HELP_SINGLE (TEXT("http://www.cbwgame.com/client/game_rule.php?kindID=%d"))
//ϲ����Ϣ(��Ϸ�б�������ҳ)
#define WEB_XB_MESSAGE (TEXT("http://www.cbwgame.com/client/inner.php?mod=9"))
//�����
#define WEB_NOTICE (TEXT("http://www.cbwgame.com/client/inner.php?mod=4"))
//�ײ����
#define WEB_ADVERTISEMENT (TEXT("http://www.cbwgame.com/client/inner.php?mod=3"))
//ͷ�����
#define WEB_TITLE_NOTICE (TEXT("http://www.cbwgame.com/client/inner.php?mod=10"))

//-----------------------------------���º���û�õ��ˡ�-----------------------------------
//�鿴���������Ϣ
#define WEB_LOOK_MY_MSG	(TEXT("http://www.cbw66.com/lookmoreusermsg.aspx?userid=%d&password=%s"))
//��Ϸ������ַ
#define WEB_MY_GAMEROOM_HELP (TEXT("http://www.cbw66.com/gamehelpstaill.aspx?helpstaill=%ld"))
//�ҵ��̳�
#define WEB_MY_SHOP (TEXT("http://www.cbw66.com/gameshops.aspx?hashell=1&userid=%d&password=%s"))
//�ҵ�����
#define WEB_MY_TASK (TEXT("http://www.cbw66.com/task/Task.aspx??hashell=1&userid=%d&password=%s"))
//�ҵı���
#define WEB_MY_DOTEY_BOX (TEXT("http://www.cbw66.com/UserBuyGrouy.aspx?userid=%d&password=%s"))
//���乫��
#define WEB_ROOM_AD  (TEXT("http://www.cbw66.com/line.aspx"))
//��������
#define WEB_HALL_AD  (TEXT("http://www.cbw66.com/pub.aspx"))
//��Ϸ�̳�
#define WEB_GAME_SHOP  (TEXT("http://www.cbw66.com/gameshop.aspx"))
//�û�����
#define WEB_USER_INFO  (TEXT("http://www.cbw66.com/publicimage.aspx?userid=%d&password=%s"))
//�̳ǳ�ֵ
#define WEB_DEPOSIT_LIST  (TEXT("http://www.cbw66.com/depositlist.aspx"))
//-----------------------------------------------------------------------------------------

//��������ַ
#define SERVER_IP			(TEXT("loginserver.cbwgame.com"))				//������IP
//#define SERVER_IP			(TEXT("222.240.205.246"))			//������IP

//��Ϸ���ص�ַ
#define GAME_DOWNLOAD_SERVER_IP_ADDRESS (TEXT("http://www.cbwgame.com/download/%s"))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
//�û�ʹ��Э��
#define WEB_USER_DOUCOMET  (TEXT("http://192.168.1.66/zhuque/index.php"))
//-----------------------------

//�ٷ���վ
#define WEB_HOME_PAGE  (TEXT("http://192.168.1.66/zhuque/index.php"))
//��Ϸ����
#define WEB_GAME_HELP  (TEXT("http://192.168.1.66/zhuque/customer.php#game"))
//�ͷ�����
#define WEB_CLINET_CENTER  (TEXT("http://192.168.1.66/zhuque/customer.php"))
//�һ�����
#define WEB_GET_PASSWORD  (TEXT("http://192.168.1.66/zhuque/find_password.php"))
//���ⷴ��
#define WEB_ERROR_TICKLING  (TEXT("http://192.168.1.66/zhuque/suggest.php?userid=%d&key=%s"))
//�˺Ź���
#define WEB_USER_ACCOUNTS_MANAGE (TEXT("http://192.168.1.66/zhuque/user_center.php"))
//��¼���
#define WEB_LOGON_ADVERTISEMENT (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=2"))
//��¼����
#define WEB_LOGON_NOTICE (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=1"))
//���߿ͷ�
#define WEB_CONTACT_SERVICE (TEXT("http://chat56.live800.com/live800/chatClient/chatbox.jsp?companyID=170069&jid=8073229581&enterurl=http%3A%2F%2Fwww%2Ecbwgame%2Ecom%2F&pagereferrer=&tm=1321516662827"))
//��ȡ���
#define WEB_CONFIRM_GETGOLD (TEXT("http://192.168.1.66/zhuque/payment.php"))
//ע���ʺ�
#define WEB_LOGON_REGISTER (TEXT("http://192.168.1.66/zhuque/register.php"))
//�ٷ���վ(��¼)
#define WEB_LOGIN_HOME_PAGE  (TEXT("http://192.168.1.66/zhuque/index.php?userid=%d&key=%s"))
//�˺Ź���(��¼)
#define WEB_LOGIN_ACCOUNTS_MANAGE (TEXT("http://192.168.1.66/zhuque/user_center.php?userid=%d&key=%s"))
//�ʺų�ֵ
#define WEB_LOGIN_PAYMENT (TEXT("http://192.168.1.66/zhuque/client/payment.php?userid=%d&key=%s"))
//��Ʒ�һ�
#define WEB_LOGIN_EXCHANGE (TEXT("http://192.168.1.66/zhuque/shop.php?userid=%d&key=%s"))
//�����̳�
#define WEB_LOGIN_PROPSHOP (TEXT("http://192.168.1.66/zhuque/client/propshop.php?userid=%d&key=%s"))
//�����̳�(����ʽ)
#define WEB_LOGIN_PROPSHOP_OPEN (TEXT("http://192.168.1.66/zhuque/propshop.php?userid=%d&key=%s"))
//��������
#define WEB_LOGIN_TASK_NEW (TEXT("http://192.168.1.66/zhuque/client/tasknewer.php?userid=%d&key=%s"))
//�����Ϣ
#define WEB_LOGIN_USERINFO (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=5&userid=%d&key=%s"))
//ϵͳ�ʼ�
#define WEB_LOGIN_SYS_MESSAGE (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=6&userid=%d&key=%s"))
//����
#define WEB_LOGIN_BANK (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=7&userid=%d&key=%s"))
//����
#define WEB_LOGIN_BACKPACK (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=8&userid=%d&key=%s"))
//������Ϸ����
#define WEB_GAME_HELP_SINGLE (TEXT("http://192.168.1.66/zhuque/client/game_rule.php?kindID=%d"))
//ϲ����Ϣ(��Ϸ�б�������ҳ)
#define WEB_XB_MESSAGE (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=9"))
//�����
#define WEB_NOTICE (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=4"))
//�ײ����
#define WEB_ADVERTISEMENT (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=3"))
//ͷ�����
#define WEB_TITLE_NOTICE (TEXT("http://192.168.1.66/zhuque/client/inner.php?mod=10"))


//-----------------------------------���º���û�õ��ˡ�-----------------------------------
//�鿴���������Ϣ
#define WEB_LOOK_MY_MSG	(TEXT("http://192.168.1.108/qpgame/lookmoreusermsg.aspx?userid=%d&password=%s"))
//��Ϸ������ַ
#define WEB_MY_GAMEROOM_HELP (TEXT("http://192.168.1.108/qpgame/gamehelpstaill.aspx?helpstaill=%ld"))
//�ҵ��̳�
#define WEB_MY_SHOP (TEXT("http://192.168.1.108/qpgame/gameshops.aspx?hashell=1&userid=%d&password=%s"))
//�ҵ�����
#define WEB_MY_TASK (TEXT("http://192.168.1.108/qpgame/task/Task.aspx??hashell=1&userid=%d&password=%s"))
//�ҵı���
#define WEB_MY_DOTEY_BOX (TEXT("http://192.168.1.108/qpgame/UserBuyGrouy.aspx?userid=%d&password=%s"))
//���乫��
#define WEB_ROOM_AD  (TEXT("http://192.168.1.66/zhuque"))
//��������
#define WEB_HALL_AD  (TEXT("http://192.168.1.66/zhuque"))
//��Ϸ�̳�
#define WEB_GAME_SHOP  (TEXT("http://192.168.1.66/zhuque"))
//�û�����
#define WEB_USER_INFO  (TEXT("http://192.168.1.108/qpgame/publicimage.aspx?userid=%d&password=%s"))
//�̳ǳ�ֵ
#define WEB_DEPOSIT_LIST  (TEXT("http://192.168.1.66/zhuque"))
//-----------------------------------------------------------------------------------------

//��������ַ
#define SERVER_IP			(TEXT("192.168.1.88"))				//������IP
//#define SERVER_IP			(TEXT("192.168.1.108"))				//������IP
//#define SERVER_IP			(TEXT("222.240.205.246"))			//������IP

//��Ϸ���ص�ַ
#define GAME_DOWNLOAD_SERVER_IP_ADDRESS (TEXT("http://192.168.1.66/zhuque/download/%s"))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
*/

#endif