#ifndef ___GAME_FRAME_NET_DEFINE_ID__H____
#define ___GAME_FRAME_NET_DEFINE_ID__H____

#define		GAMEFRAMEONLINENET				1

#ifdef GAMEFRAMEONLINENET
//����
//�ٷ���վ
#define WEB_HOME_PAGE  (TEXT("http://www.cbwgame.com/index.aspx"))
//��Ϸ������ַ
#define WEB_MY_GAME_HELP (TEXT("http://www.cbwgame.com/gamehelpstaill.aspx?helpstaill=%ld"))
//�̳���ַ
#define WEB_MY_SHOP (TEXT("http://www.cbwgame.com/gameshops.aspx?hashell=1&userid=%d&password=%s"))
//�ͷ�����
#define WEB_CLINET_CENTER  (TEXT("http://www.cbwgame.com/constmecontacts.aspx"))

#else
//����
//�ٷ���վ
#define WEB_HOME_PAGE  (TEXT("http://192.168.1.108/qpgame/index.aspx"))
//��Ϸ������ַ
#define WEB_MY_GAME_HELP (TEXT("http://192.168.1.108/qpgame/gamehelpstaill.aspx?helpstaill=%ld"))
//�̳���ַ
#define WEB_MY_SHOP (TEXT("http://192.168.1.108/qpgame/gameshops.aspx?hashell=1&userid=%d&password=%s"))
//�ͷ�����
#define WEB_CLINET_CENTER  (TEXT("http://192.168.1.108/qpgame/constmecontacts.aspx"))
#endif




#endif