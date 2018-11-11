//------------------------------------------------------------------------------
// �ļ����ƣ�MultiLanguage.h
// �ļ��汾��v1.0
// �������ڣ�2006-02-14 11:25
// ��    �ߣ�Richard Ree
// �ļ�������ʵ�ֶ������л���ͷ�ļ�
//------------------------------------------------------------------------------

// ����ID����ö�٣����ӡ���������ʱֱ���޸Ĵ�ö�ٶ���
enum enLANGUAGE
{
	LANGUAGE_GB = 0,
	LANGUAGE_BIG5,
	LANGUAGE_ENGLISH,
	LANGUAGE_BOTTOM
};

// �ַ�������IDö�٣����ӡ����ٱ��ĳ��������ַ���ʱֱ���޸Ĵ�ö�ٶ���
enum enSTRING
{
	STR_NULL = 0,
	STR_AUTO_UPDATE,
	STR_APP_ALREADY_RUNNING,

	STR_PROMPT_NEWER_VERSION_AVAILABLE,
	STR_OPTION_UPGRADE_IN_BACKGROUND,
	STR_PROMPT_UPGRADE_READY,
	STR_PROMPT_FAIL_TO_OPEN_UPDATE_CONFIG_FILE,
	STR_PROMPT_DOWNLOADING_FILE,
	STR_TOTAL_UPGRADE_PROGRESS,
	STR_PROMPT_FAIL_IN_DOWNLOADING_FILES,
	STR_PROMPT_FAIL_IN_VERIFYING_FILES,
	STR_PROMPT_FAIL_IN_UPDATING_FILES,
	STR_PROMPT_FAIL_IN_UPDATING,
	STR_PROMPT_UPGRADE_FINISHED,

	STR_OTHER,

	STR_BUTTON_START_UPGRADE,
	STR_BUTTON_CANCEL_UPGRADE,

	STR_BUTTON_OK,
	STR_BUTTON_CANCEL,
	STR_BUTTON_ABORT,
	STR_BUTTON_IGANORE,
	STR_BUTTON_RETRY,
	STR_BUTTON_CONTINUE,
	STR_BUTTON_YES,
	STR_BUTTON_NO,
	STR_BUTTON_CLOSE,
	STR_BUTTON_APPLY,

	STR_ERROR,
	STR_ERROR_MESSAGE,

	STRING_BOTTOM
};

// ���治ͬ���԰汾�ĳ����ַ����Ľṹ�����ӡ���������ʱҪ��д���캯����Set����
struct StringStru
{
	const char *Language[LANGUAGE_BOTTOM]; // ���治ͬ���԰汾�ĳ����ַ�����ָ��
	StringStru()
	{
		Language[0] = "";
		Language[1] = "";
		Language[2] = "";
	};
	void Set(const char *s1, const char *s2, const char *s3)
	{
		if (NULL != s1)
		{
			Language[0] = s1;
		}
		if (NULL != s2)
		{
			Language[1] = s2;
		}
		if (NULL != s3)
		{
			Language[2] = s3;
		}
	};
};

// ȫ���ַ������������飨�����ʼ����InitStringTable()�����н��У�
extern struct StringStru g_String[STRING_BOTTOM];

// ȫ�����Դ��루ע�⣺�����޸���ֵ���ܻᵼ�³����쳣����
extern enum enLANGUAGE g_LanguageID;

// ȡ�ó����ַ����ĺ궨�壬STRING_IDΪ�����ַ���ID��PROMPTΪ����������ʾ�ַ���������ʱ������
#define STRING(STRING_ID, PROMPT) (const char *)(g_String[STRING_ID].Language[g_LanguageID])
