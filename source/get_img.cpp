// get_img.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

unsigned int flag_error;
char time_now[30];
unsigned long fs;
typedef struct r_node
{
	char data[19];
	struct r_node* p_next;
};
struct r_node *tett;

unsigned long num_p(const char *name)
{
	unsigned int l = strlen(name);
	if (l <= 7) return 0;
	char flag1 = 0, flag2 = 0;
	int i;
	unsigned long s = 1, re = 0;
	unsigned int s_b = 0;
	for (i = l - 1; i >= 0; i--)
	{
		if (name[i] >= '0' && name[i] <= '9')
		{
			re = re + (name[i] - '0')*s;
			s = s * 10;
		}
		else
		{
			if (name[i] == '_') flag1 = 1;
			if (name[i] == 'p') flag2 = 1;
			++s_b;
		}
	}
	if (flag1 != 1 || flag2 != 1) return 0;
	if (s_b>6) return 0;
	else return re;
}

void p_dir(char *_dir, int depth)
{
	char dir_t[260];
	strcpy(dir_t, _dir);
	strcat(dir_t, "*.*");
	struct _finddata_t f1;
	long handle = _findfirst(dir_t, &f1);
	do
	{
		if (f1.attrib == _A_SUBDIR && strcmp(f1.name, ".") != 0 && strcmp(f1.name, "..") != 0)
		{
			int i;
			//for(i=0; i<depth; i++)
			//printf(" ");
			//printf("-");
			//printf("DIR:%s\n",f1.name);
			char dir_t1[260];
			strcpy(dir_t1, _dir);
			strcat(dir_t1, f1.name);
			strcat(dir_t1, "\\");
			p_dir(dir_t1, depth + 1);
		}
		else
		{
			if (strcmp(f1.name, ".") != 0 && strcmp(f1.name, "..") != 0)
			{
				int i;
				int end = strlen(f1.name);
				for (i = 0; i<end; i++)
				{
					if (f1.name[i] == '.') break;
				}
				if ((f1.name[i + 1] == 'j' && f1.name[i + 2] == 'p' && f1.name[i + 3] == 'g') || (f1.name[i + 1] == 'p' && f1.name[i + 2] == 'n' && f1.name[i + 3] == 'g'))
				{
					unsigned long pid = num_p(f1.name);
					if (pid != 0)
					{
						unsigned int hash = pid % 11111;
						//printf("PID:%ld\nHASH:%d\n",pid,hash);
						if (strcmp("", tett[hash].data))
						{
							//printf("Hash conflict!\nNow Processing...\n");
							struct r_node *last = &tett[hash];

							while ((*last).p_next != NULL) last = (*last).p_next;

							struct r_node *temp;
							temp = (struct r_node*)malloc(sizeof(*temp));

							memset((*temp).data, '\0', sizeof((*temp).data));
							(*temp).p_next = NULL;

							(*last).p_next = temp;
							strcpy((*temp).data, f1.name);

						}
						else
						{
							strcpy(tett[hash].data, f1.name);
						}
						//for(i=0; i<depth; i++)
						//printf(" ");
						//printf("-");
						//printf("%s\n",f1.name);
						++fs;
						if (fs % 500 == 0) printf("%ld OVER.\n", fs);
						//system("pause");
					}
				}
			}
		}
	} while (_findnext(handle, &f1) != -1);
	_findclose(handle);
}

void Read_data(FILE *data)
{
	tett = (struct r_node*)malloc(sizeof(struct r_node) * 11111);
	int i, j = 0;
	for (i = 0; i<11111; i++)
	{
		memset(tett[i].data, '\0', 19);
		tett[i].p_next = NULL;
	}
	char dat[300];
	memset(dat, '\0', 300);
	for (i = 0; i<11111; i++)
	{
		fscanf(data, "%s", dat);
		if (strcmp("->NULL", dat) != 0)
		{
			unsigned int len = strlen(dat);
			char fname[260];
			int j = 0, js;
			while (j<len)
			{
				memset(fname, '\0', 260);
				js = 0;
				while (dat[j] != '-')
				{
					fname[js] = dat[j];
					++js;
					++j;
				}
				if (strcmp("", tett[i].data))
				{
					//printf("Hash conflict!\nNow Processing...\n");
					struct r_node *last = &tett[i];

					while ((*last).p_next != NULL) last = (*last).p_next;

					struct r_node *temp;
					temp = (struct r_node*)malloc(sizeof(*temp));

					memset((*temp).data, '\0', sizeof((*temp).data));
					(*temp).p_next = NULL;

					(*last).p_next = temp;
					strcpy((*temp).data, fname);

				}
				else
				{
					strcpy(tett[i].data, fname);
				}
				if (len - j<7)
				{
					j = len;
				}
				else
				{
					j = j + 2;
				}
			}
		}
	}
}


void Read_files()
{
	tett = (struct r_node*)malloc(sizeof(struct r_node) * 11111);
	int i, j = 0;
	for (i = 0; i<11111; i++)
	{
		memset(tett[i].data, '\0', 19);
		tett[i].p_next = NULL;
	}
	char p[300];
	memset(p, '\0', sizeof(p));


	printf("请输入文件夹路径（直接复制地址栏处即可）：");
	fgets(p, 300, stdin);
	char p_1[300];
	memset(p_1, '\0', sizeof(p_1));
	int s = strlen(p);
	for (i = 0; i<s - 1; i++)
	{
		if (p[i] != 0x5c)
		{
			p_1[j] = p[i];
			++j;
		}
		else
		{
			p_1[j] = 0x5c;
			++j;
			p_1[j] = 0x5c;
			++j;
		}
	}
	strcat(p_1, "\\");

	printf("Now reading files and save to the hash table...\n");
	p_dir(p_1, 0);
	printf("Read file over, TOTAL:%ld\n", fs);
}

static size_t DownLoad(void *ptr, size_t size, size_t nmemb, char *userdata)
{
	//FILE *fp = (FILE*)userdata;
	//printf("%s\n",userdata);
	FILE *fp = fopen(userdata, "ab");
	if (flag_error == 0)
	{
		fclose(fp);
		FILE *fp = fopen(userdata, "wb");
	}
	fwrite(ptr, size, nmemb, fp);
	fclose(fp);
	//fp=NULL;
	//return written;
	flag_error++;
	return size*nmemb;
}

void Get_img(char *url, char *file_name)
{
	CURL *curl;
	CURLcode res;
	char time_error[30];
	time_t rawtime;
	struct tm *timeinfo;
	int times = 0;
	do
	{
		struct curl_slist *list = NULL;
		printf("Try %d times...\n", times);
		times++;
		flag_error = 0;

		curl_global_init(CURL_GLOBAL_ALL);

		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1);

		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 120);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		list = curl_slist_append(list, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0");
		list = curl_slist_append(list, "Referer: http://www.pixiv.net/member_illust.php?mode=medium&illust_id=43428018");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownLoad);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file_name);

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);


		res = curl_easy_perform(curl);
		curl_slist_free_all(list);


		if (res != CURLE_OK)
		{
			FILE *error_f;
			error_f = fopen("error.log", "a");
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			sprintf(time_error, "%4d-%02d-%02d %02d:%02d:%02d", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			fprintf(error_f, "%s Error:%s at %s\n", time_error, curl_easy_strerror(res), url);
			fclose(error_f);
			error_f = NULL;
			curl_easy_cleanup(curl);
		}
		if (times>20)
		{
			curl_easy_cleanup(curl);
			printf("ERROR!\n");
			return;
		}
	} while (res != CURLE_OK);
	curl_easy_cleanup(curl);
}
void Get_page(char *url, char *file_name)
{
	CURL *curl;
	CURLcode res;
	char time_error[30];
	time_t rawtime;
	struct tm *timeinfo;
	int times = 0;
	do
	{
		struct curl_slist *list = NULL;
		printf("Try %d times...\n", times);
		times++;
		flag_error = 0;

		curl_global_init(CURL_GLOBAL_ALL);

		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1);

		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 40);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 40);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		list = curl_slist_append(list, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0");
		list = curl_slist_append(list, "Referer: http://www.pixiv.net/");
		list = curl_slist_append(list, "Cookie: cookie=cookie");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownLoad);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file_name);

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);


		res = curl_easy_perform(curl);
		curl_slist_free_all(list);


		if (res != CURLE_OK)
		{
			FILE *error_f;
			error_f = fopen("error.log", "a");
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			sprintf(time_error, "%4d-%02d-%02d %02d:%02d:%02d", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			fprintf(error_f, "%s Error:%s at %s\n", time_error, curl_easy_strerror(res), url);
			fclose(error_f);
			error_f = NULL;
			curl_easy_cleanup(curl);
		}
		if (times>5)
		{
			curl_easy_cleanup(curl);
			printf("ERROR!\n");
			return;
		}
	} while (res != CURLE_OK);
	curl_easy_cleanup(curl);
}

long Find_String(unsigned char *source, const char *find, long where)
{
	long len_s = strlen((char*)source);
	long len_f = strlen(find);
	long table[256];
	long i, j;
	for (i = 0; i<256; i++) table[i] = len_f;
	for (i = 0; i<len_f; i++)
	{
		table[find[i]] = len_f - i;
	}
	for (i = where; i<len_s - len_f;)
	{
		for (j = i; j<i + len_f; j++)
		{
			if (source[j] != find[j - i]) break;
			
		}
		if (j == i + len_f) return i + len_f;
		else i = i + table[source[i + len_f]];
	}
	return -1;
}
unsigned char *get = NULL;
void Read_in()
{
	//read_in
	FILE *fp;
	fp = fopen("temp_html.tmp", "rb");
	fseek(fp, 0, SEEK_END);
	long f_long = ftell(fp);
	rewind(fp);
	get = (unsigned char*)malloc(sizeof(unsigned char)*f_long + 10);
	fread(get, sizeof(unsigned char), f_long, fp);
	fclose(fp);
	get[f_long] = '\0';
	//read_in end
}
void Search_get()
{
	Read_in();
	const char key[] = "lazy-image\"data-src=\"";
	long wh = 0;
	char m1[3];
	char m2_t[300];
	char name_t[20];
	char m2[300];
	char name[20];
	char img[300];
	int s = 0;
	while ((wh = Find_String(get, key, wh)) != -1)
	{
		memset(m1, '\0', sizeof(m1));
		memset(m2_t, '\0', sizeof(m2_t));
		memset(name_t, '\0', sizeof(name_t));

		//i1,2,3?...
		m1[0] = *(get + wh + 7);
		m1[1] = *(get + wh + 8);
		long i = wh + 44;
		int j = 0;
		int sn = 0;
		int sn_w = 0;
		while (get[i] != 0x5F)
		{
			if (sn_w == 7)
			{
				name_t[sn] = get[i];
				++sn;
			}
			m2_t[j] = get[i];
			if (get[i] == '/')
			{
				++sn_w;
			}
			++j;
			++i;
		}

		++s;
		int p1 = 0;
		char flag_over = 0;
		while (flag_over == 0)
		{
			char no[4];
			_itoa(p1, no, 10);

			memset(img, '\0', sizeof(img));
			strcpy(m2, m2_t);
			strcpy(name, name_t);

			strcat(name, "_p");
			strcat(name, no);
			strcat(name, ".");
			//判断重复
			char name_a[20];
			char name_b[20];
			strcpy(name_a, name);
			strcat(name_a, "jpg");
			strcpy(name_b, name);
			strcat(name_b, "png");

			unsigned long hash_s = num_p(name_a) % 11111;
			char flag = 0;

			printf("HASH:%ld\n", hash_s);
			struct r_node *last = &tett[hash_s];
			if ((!strcmp(tett[hash_s].data, name_a)) || (!strcmp(tett[hash_s].data, name_b)))
			{
				flag = 1;
				printf("Already in!\n");
			}
			else
			{
				if (flag != 1) flag = 0;
			}
			if (flag == 0)
			{
				while ((*last).p_next != NULL)
				{
					last = (*last).p_next;
					if ((!strcmp((*last).data, name_a)) || (!strcmp((*last).data, name_b)))
					{
						flag = 1;
						printf("Already in!\n");
					}
				}
			}
			//jpg

			if (flag == 0)
			{
				printf("Saving this new picture into the HASH table...\n");
				//save new picture
				if (strcmp("", tett[hash_s].data))
				{
					//printf("Hash conflict!\nNow Processing...\n");
					struct r_node *last = &tett[hash_s];

					while ((*last).p_next != NULL) last = (*last).p_next;

					struct r_node *temp;
					temp = (struct r_node*)malloc(sizeof(*temp));

					memset((*temp).data, '\0', sizeof((*temp).data));
					(*temp).p_next = NULL;

					(*last).p_next = temp;
					strcpy((*temp).data, name_a);

				}
				else
				{
					strcpy(tett[hash_s].data, name_a);
				}

				strcpy(img, "http:/");
				strcat(img, m1);
				strcat(img, ".pximg.net/img-original/img");
				strcat(img, m2);
				strcat(img, "_p");
				strcat(img, no);
				strcat(img, ".");

				char final_url[300];
				memset(final_url, '\0', sizeof(final_url));
				strcpy(final_url, img);
				strcat(final_url, "jpg");

				printf("NO.%d:%s\n", s, final_url);

				char final_name[300];
				memset(final_name, '\0', sizeof(final_name));

				char end_img[300];
				memset(end_img, '\0', sizeof(end_img));
				strcat(end_img, ".\\");
				strcat(end_img, time_now);
				strcat(end_img, "\\");
				strcat(end_img, name);

				strcpy(final_name, end_img);
				strcat(final_name, "jpg");

				Get_img(final_url, final_name);

				FILE *test;
				test = fopen(final_name, "r");
				char testc[10];
				fread(testc, sizeof(char), 10, test);
				fclose(test);
				if (testc[0] == '<')
				{
					char cmd[300];
					memset(cmd, '\0', sizeof(cmd));
					strcat(cmd, "del ");
					strcat(cmd, final_name);
					system(cmd);

					printf("NOT A JPG,TRY PNG!\n");
					strcpy(final_url, img);
					strcat(final_url, "png");
					strcpy(final_name, end_img);
					strcat(final_name, "png");

					Get_img(final_url, final_name);

					FILE *test1;
					test1 = fopen(final_name, "r");
					char testc1[10];
					fread(testc1, sizeof(char), 10, test1);
					fclose(test1);
					if (testc1[0] == '<')
					{
						memset(cmd, '\0', sizeof(cmd));
						strcat(cmd, "del ");
						strcat(cmd, final_name);
						system(cmd);
						printf("OVER or a GIF!\n");
						flag_over = -1;
					}
				}
			}
			if (flag == 1 && p1>0) flag_over = -1;
			++p1;
		}
	}
}

void time_init()
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(time_now, "%4d_%02d_%02d-%02d_%02d_%02d", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	_mkdir(time_now);
}


void uid_get_main()
{
	time_init();
	printf("请输入UID：");
	char UID[15];
	memset(UID, '\0', sizeof(UID));
	scanf("%s", UID);

	printf("输入开始页数：");
	int start = 0;
	int end = 5;
	scanf("%d", &start);
	printf("输入结束页数：");
	scanf("%d", &end);

	char _URL[300];
	memset(_URL, '\0', sizeof(_URL));
	strcat(_URL, "http://www.pixiv.net/member_illust.php?id=");
	strcat(_URL, UID);
	strcat(_URL, "&type=all&p=");

	char now_URL[350];
	int i = 0;
	for (i = start - 1; i<end; i++)
	{
		printf("Page%d\n", i + 1);
		char no[4];
		_itoa(i + 1, no, 10);
		strcpy(now_URL, _URL);
		strcat(now_URL, no);
		Get_page(now_URL, "temp_html.tmp");
		Search_get();
		free(get);
		get = NULL;
	}
}


void s_get_main()
{
	time_init();
	printf("输入关键字，限英文或罗马字：");
	char key_word[200];
	memset(key_word, '\0', sizeof(key_word));
	scanf("%s", key_word);

	printf("输入开始页数：");
	int start = 0;
	int end = 200;
	scanf("%d", &start);
	printf("输入结束页数：");
	scanf("%d", &end);

	int i = 0;
	for (i = start - 1; i<end; i++)
	{
		printf("Page%d\n", i + 1);
		char now_URL[500];
		memset(now_URL, '\0', sizeof(now_URL));
		strcat(now_URL, "http://www.pixiv.net/search.php?s_mode=s_tag&word=");
		strcat(now_URL, key_word);
		char no[4];
		_itoa(i + 1, no, 10);
		strcat(now_URL, "&abt=y&order=popular_d&p=");
		strcat(now_URL, no);
		Get_page(now_URL, "temp_html.tmp");
		Search_get();
		free(get);
		get = NULL;
	}
}

void clean(struct r_node *c)
{
	if (c->p_next != NULL)
		clean(c->p_next);
	else
		return;
	if (c->p_next != NULL)
	{
		free(c->p_next);
		c->p_next = NULL;
	}

}

int main()
{
	FILE *dat;
	dat = fopen("save_data.dat", "r");
	if (dat != NULL)
	{
		printf("Reading data file...\n");
		Read_data(dat);
		printf("Done.\n");
		fclose(dat);
	}
	else Read_files();
	printf("1.关键字\n2.UID\n");
	int x;
	scanf("%d", &x);
	while ((x != 1) && (x != 2))
	{
		printf("ERROR!\n");
		scanf("%d", &x);
	}
	if (x == 1) s_get_main();
	else uid_get_main();
	printf("Saving data...\n");
	int i;
	FILE *fp;
	fp = fopen("save_data.dat", "w");
	for (i = 0; i<11111; i++)
	{
		fprintf(fp, "%s->", tett[i].data);
		struct r_node *last = &tett[i];
		while ((*last).p_next != NULL)
		{
			last = (*last).p_next;
			fprintf(fp, "%s->", (*last).data);
		}
		fprintf(fp, "NULL\n");
	}
	fclose(fp);
	printf("Done.\n");
	printf("Cleaning memory...\n");
	for (i = 0; i<11111; i++)
	{
		if (tett[i].p_next != NULL) clean(&tett[i]);
	}
	free(tett);
	printf("Done.\n");
	system("pause");
	return 0;
}


