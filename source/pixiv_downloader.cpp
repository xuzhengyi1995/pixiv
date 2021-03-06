// pixiv_downloader.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

unsigned int flag_error;
char time_now[30];
char *cookies;
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
	if (s_b > 6) return 0;
	else return re;
}

void p_dir(char *_dir, int depth)
{
	char dir_t[260];
	strcpy(dir_t, _dir);
	strcat(dir_t, "*.*");
	struct _finddata_t f1;
	long long handle = _findfirst(dir_t, &f1);
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
	for (i = 0; i < 11111; i++)
	{
		memset(tett[i].data, '\0', 19);
		tett[i].p_next = NULL;
	}
	char dat[300];
	memset(dat, '\0', 300);
	for (i = 0; i < 11111; i++)
	{
		fscanf(data, "%s", dat);
		if (strcmp("->NULL", dat) != 0)
		{
			unsigned int len = strlen(dat);
			char fname[260];
			int j = 0, js;
			while (j < len)
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
				if (len - j < 7)
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
	for (i = 0; i < 11111; i++)
	{
		memset(tett[i].data, '\0', 19);
		tett[i].p_next = NULL;
	}
	char p[300];
	memset(p, '\0', sizeof(p));

	printf("读取已有P站图片以防止重复，本功能需文件名为PID_p0.jpg/png形式可使用。\n");
	printf("如果没有图片或不满足条件，请按回车\n");
	printf("请输入P站图片文件夹路径（直接复制地址栏处即可）：");
	fgets(p, 300, stdin);
	char p_1[300];
	memset(p_1, '\0', sizeof(p_1));
	int s = strlen(p);
	for (i = 0; i < s - 1; i++)
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
	return size * nmemb;
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
		if (times > 20)
		{
			curl_easy_cleanup(curl);
			printf("ERROR!\n");
			return;
		}
	} while (res != CURLE_OK);
	curl_easy_cleanup(curl);
}

void Get_page(char *url, const char *file_name)
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
		// Add Cookie: cookie= to cookies
		list = curl_slist_append(list, cookies);

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
		if (times > 5)
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
	for (i = 0; i < 256; i++) table[i] = len_f;
	for (i = 0; i < len_f; i++)
	{
		table[find[i]] = len_f - i;
	}
	for (i = where; i < len_s - len_f;)
	{
		for (j = i; j < i + len_f; j++)
		{
			if (source[j] != find[j - i]) break;

		}
		if (j == i + len_f) return i + len_f;
		else i = i + table[source[i + len_f]];
	}
	return -1;
}

unsigned char *get = NULL;
void Read_in(const char *file_name = "temp_html.tmp")
{
	//read_in
	FILE *fp;
	fp = fopen(file_name, "rb");
	fseek(fp, 0, SEEK_END);
	long f_long = ftell(fp);
	rewind(fp);
	get = (unsigned char*)malloc(sizeof(unsigned char)*f_long + 10);
	int i;
	unsigned char temp;
	long real_length = 0;
	for (i = 0; i < f_long; i++)
	{
		fread(&temp, sizeof(unsigned char), 1, fp);
		if (temp != '\\')
		{
			get[real_length] = temp;
			real_length++;
		}
	}
	fclose(fp);
	get[real_length] = '\0';
	//read_in end
}

void Search_get(const char *file_name = "temp_html.tmp", const char *uid = NULL)
{
	Read_in(file_name);
	const char key[] = "&quot;0&quot;,&quot;url&quot;:&quot;";
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
				if (uid != NULL)
				{
					strcat(end_img, uid);
					strcat(end_img, "\\");
				}
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
			if (flag == 1 && p1 > 0) flag_over = -1;
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

int count_items(cJSON *json)
{
	int s = 0;
	cJSON *_json = json->child;
	if (_json == NULL) return 0;
	while (_json->next != NULL)
	{
		s++;
		_json = _json->next;
	}
	return s;
}

void get_all_items_name(cJSON *json, char** array)
{
	int i = 0;
	cJSON *_json = json->child;
	if (_json == NULL) return;
	while (_json->next != NULL)
	{
		strcpy(array[i], _json->string);
		i++;
		_json = _json->next;
	}
}

void uid_get_by_api(char* uid, int len_list, char** pid_list)
{
	char base_url[2000], this_url[2000];
	memset(base_url, '\0', sizeof(base_url));
	strcat(base_url, "https://www.pixiv.net/ajax/user/");
	strcat(base_url, uid);
	strcat(base_url, "/profile/illusts?");
	int i = 0, j = 0;
	int times = len_list / 45;
	//int last_time_count = len_list % 45;
	int now = 0, end = 0;
	cJSON *json = 0, *word_list = 0;
	FILE *ge_file;
	ge_file = fopen("ge_temp_html.tmp", "wb");
	printf("Getting user's illusts id list...\n");
	for (i = 0; i < times + 1; i++)
	{
		strcpy(this_url, base_url);
		now = i * 45;
		end = now + 45 <= len_list ? now + 45 : len_list;
		printf("Now getting URLs of number %d to %d illusts...\n", now, end);
		for (j = now; j < end; j++)
		{
			if (j != 0) strcat(this_url, "&");
			strcat(this_url, "ids%5B%5D=");
			strcat(this_url, pid_list[j]);
		}
		strcat(this_url, "&work_category=illust&is_first_page=1");

		Get_page(this_url, "temp_html.tmp");
		Read_in();

		json = cJSON_Parse((char*)get);
		word_list = cJSON_GetObjectItem(
			cJSON_GetObjectItem(json, "body"), "works");

		char *temp_url;
		int len_url = 0, k = 0;
		for (j = now; j < end; j++)
		{
			temp_url = cJSON_GetStringValue(
				cJSON_GetObjectItem(
					cJSON_GetObjectItem(word_list, pid_list[j]), "url")
			);
			fwrite("&quot;0&quot;,&quot;url&quot;:&quot;", 1, 36, ge_file);
			len_url = strlen(temp_url);
			for (k = 0; k < len_url; k++)
			{
				if (k < 29 || k>34)
					fwrite(temp_url + k, sizeof(char), 1, ge_file);
			}
		}
		free(get);
		get = NULL;
		cJSON_free(word_list);
		cJSON_free(json);
		word_list = NULL;
		json = NULL;
	}
	fclose(ge_file);
	Search_get("ge_temp_html.tmp", uid);
}

void free_char_array(char** to_free, int len)
{
	int i = 0;
	for (i = 0; i<len; i++)
	{
		if (to_free[i] != NULL) free(to_free[i]);
	}
	free(to_free);
	to_free = NULL;
}

void uid_get_main()
{
	time_init();
	printf("请输入UID(空格或换行分离多个，最多1000条，字母T停止)：\n");
	char **uids = (char**)malloc(sizeof(char*) * 1001);
	memset(uids, NULL, sizeof(uids));
	int s = 0;
	do
	{
		uids[s] = (char*)malloc(sizeof(char) * 15);
		scanf("%s", uids[s]);
		++s;
	} while (uids[s - 1][0] != 'T' && uids[s - 1][0] != 't');

	int i_uid = 0;
	char _URL[300];
	cJSON *json = 0, *illust_list = 0;
	int sum_illust = 0;
	char **illust_list_array = 0;
	int i = 0;
	char *UID = NULL;
	char uid_dir[50];
	for (i_uid = 0; i_uid < s - 1; i_uid++)
	{
		printf("Now getting UID: %s\n", uids[i_uid]);
		UID = uids[i_uid];

		memset(uid_dir, '\0', sizeof(uid_dir));
		strcpy(uid_dir, ".\\");
		strcat(uid_dir, time_now);
		strcat(uid_dir, "\\");
		strcat(uid_dir, UID);
		_mkdir(uid_dir);
		
		memset(_URL, '\0', sizeof(_URL));
		strcat(_URL, "https://www.pixiv.net/ajax/user/");
		strcat(_URL, UID);
		strcat(_URL, "/profile/all");

		Get_page(_URL, "temp_html.tmp");

		Read_in();

		json = cJSON_Parse((char*)get);
		illust_list = cJSON_GetObjectItem(
			cJSON_GetObjectItem(json, "body"), "illusts");
		free(get);
		get = NULL;

		sum_illust = count_items(illust_list);
		printf("This user has %d illusts.\n", sum_illust);
		illust_list_array = (char**)malloc(sizeof(char*)*sum_illust);

		for (i = 0; i < sum_illust; i++)
		{
			illust_list_array[i] = (char*)malloc(sizeof(char) * 20);
		}
		get_all_items_name(illust_list, illust_list_array);

		cJSON_free(illust_list);
		cJSON_free(json);

		uid_get_by_api(UID, sum_illust, illust_list_array);
		free_char_array(illust_list_array, sum_illust);
	}
	free_char_array(uids, 1001);
}

void urlencode(char* org_utf8, char* res, int length)
{
	int i = 0;
	char temp[4] = {'\0'};
	while (org_utf8[i] != '\0' && i < length)
	{
		sprintf(temp, "%%%02X", (unsigned char)org_utf8[i]);
		strcat(res, temp);
		i++;
	}
}

void s_get_main()
{
	time_init();
	printf("输入关键字，可以输入中文日文：");
	char key_word[200];
	memset(key_word, '\0', sizeof(key_word));
	scanf("%s", key_word);

	// Change to UTF-8
	wchar_t key_word_w[200];
	memset(key_word_w, '\0', sizeof(key_word_w));
	MultiByteToWideChar(CP_ACP, 0, key_word, strlen(key_word), key_word_w, 200);
	char key_word_final[800];
	memset(key_word_final, '\0', sizeof(key_word_final));
	WideCharToMultiByte(CP_UTF8, 0, key_word_w, wcslen(key_word_w), key_word_final, 800, NULL, NULL);
	// Change to urlencode
	char key_word_encoded[800];
	memset(key_word_encoded, '\0', sizeof(key_word_encoded));
	urlencode(key_word_final, key_word_encoded, 800);

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
		strcat(now_URL, key_word_encoded);
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
	// Read in cookies
	cookies = (char*)malloc(sizeof(char) * 10000);
	memset(cookies, '\0', sizeof(cookies));
	printf("请输入Cookies:\n");
	strcpy(cookies, "Cookie: cookie=");
	scanf("%[^\n]", cookies + 15);

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
	for (i = 0; i < 11111; i++)
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
	free(cookies);
	cookies = NULL;
	for (i = 0; i < 11111; i++)
	{
		if (tett[i].p_next != NULL) clean(&tett[i]);
	}
	free(tett);
	printf("Done.\n");
	system("pause");
	return 0;
}
