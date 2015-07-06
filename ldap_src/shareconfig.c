#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ldap.h>
#include <json-c/json.h>
#include <string.h>



/*int ReadEveryFile(char *smbfilepath)
{
	
char *t = "include = /etc/samba/smb.conf.%U";
char one_line[1024];
FILE *fin;
 
int i,j,k,L,Lt,N=0;

Lt=strlen(t);
printf("Lt=%d\n",Lt);
fin=fopen(smbfilepath,"r");
while(1){
if (fgets(one_line,1023,fin)==NULL) break;
N++;
 L = strlen(one_line);
 for(i=0;i<=L-Lt;i++){
 if (strncmp(&one_line[i],t,Lt)==0){
	
 printf("found target at Line %d column %d \nThe Line: %s\n",N,i,one_line);
	return 1;
 }

 }; //for i
} //while

fclose(fin);	
	return 0;
}*/





int HandleSmbFile(char* path,char* disname,char *permissions,char * sharepath )
{
	
	//printf("path = %d!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n\n\n\n",access(path,F_OK));
	int returnval = access(path,F_OK);
	FILE*fd;
	fd = fopen(path,"a+");
	char buf[1024] = "\n[";
		strcat(buf,disname);
		strcat(buf,"]\n\t");
		strcat(buf,"path = ");
		strcat(buf,sharepath);
		strcat(buf,"\n\tpublic = yes\n\t");
		strcat(buf,"browseable = yes\n\t ");
		//printf("11111111111111=%d\n",strcmp(permissions,0));
	
		if(strstr(permissions,"0") == NULL)
		{
			strcat(buf,"writeable = yes\n\t ");
		}
		else
		{			
			strcat(buf,"readonly = yes\n\t ");
		}
		int length = strlen(buf);
		char realbuf [length];
		
		//printf("length!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!=%d",length);
		strcpy(realbuf,buf); 
		if(fd == NULL)
		{
			printf("no have !!!");
			exit(0);
		}

		//printf("realbuf=%s\n",realbuf);
		int rc;

		rc = fwrite(realbuf,sizeof(char),sizeof(realbuf)/sizeof(realbuf[0]) - 1,fd);

		fclose(fd);
		return 0; 
	/*if(returnval == 0)   
    {   
        	
		char buf[1024] = "\n[";
		strcat(buf,disname);
		strcat(buf,"]\n\t");
		strcat(buf,"path = ");
		strcat(buf,sharepath);
		strcat(buf,"\n\tpublic = yes\n\t");
		strcat(buf,"browseable = yes\n\t ");
		if(strcmp(permissions,"0") == 0)
		{
			strcat(buf,"writeable = no\n\t ");
		}
		else
		{
			strcat(buf,"writeable = yes\n\t ");
		}
		int length = strlen(buf);
		char realbuf [length];
		
		//printf("length!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!=%d",length);
		strcpy(realbuf,buf); 
		if(fd == NULL)
		{
			printf("no have !!!");
			exit(0);
		}

		//printf("realbuf=%s\n",realbuf);
		int rc;

		rc = fwrite(realbuf,sizeof(char),sizeof(realbuf)/sizeof(realbuf[0]) - 1,fd);

		fclose(fd);
		return 0; 
    }   
    else  
   	{ 
		
		char buf[1024] = "[global]\n\tserver string=%h server (Samba , Ubuntu)\n\tserver role = standalone server\n\tmap to guest = Bad User\n\tobey pam restrictions = Yes\n\tpasswd program = /usr/bin/passwd %u\n\tpasswd chat = *Enter\\snew\\s*\\spassword:* %n\\n *Retype\\snew\\s*\\spassword:* %n\\n *password\\supdated\\ssuccessfully* .\n\tunix password sync = Yes\n\tsyslog = 0\n\tlog file = /var/log/samba/log.%m\n\tmax log size = 1000\n\tdns proxy = No\n\tusershare allow guests = Yes\n\tpanic action = /usr/share/samba/panic-action %d\n\tidmap config * : backend = tdb\n\n[printers]\n\tcomment = All Printers\n\tpath = /var/spool/samba\n\tcreate mask = 0700\n\tprintable = Yes\n\tprint ok = Yes\n\tbrowseable = No\n[print$]\n\tcomment = Printer Drivers\n\tpath = /var/lib/samba/printers\n"; 
		

		strcat(buf,"\n[");
		strcat(buf,disname);
		strcat(buf,"]\n\t");
		strcat(buf,"path = ");
		strcat(buf,sharepath);
		strcat(buf,"\n\tpublic = yes\n\t");
		strcat(buf,"browseable = yes\n\t ");
		if(strcmp(permissions,"0") == 0)
		{
			strcat(buf,"writeable = no\n\t ");
		}
		else
		{
			strcat(buf,"writeable = yes\n\t ");
		}
		//printf("buf1=%s\n",buf);
		int length = strlen(buf);
		char realbuf [length];
		
		//printf("length!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!=%d",length);
		strcpy(realbuf,buf); 
		if(fd == NULL)
		{
			printf("no have !!!");
			exit(0);
		}


		int rc;

		rc = fwrite(realbuf,sizeof(char),sizeof(realbuf)/sizeof(realbuf[0]) - 1,fd);

		fclose(fd);
		return 0;
    } */
	
}


int main(int argc,char *argv[])
{
	
	json_object  *root;

	//printf("parsing the document....%s\n",argv[1]);
	root = json_tokener_parse(argv[1]);

	//printf("Printing the document tree…\n");
	//printf("%s…\n",json_object_to_json_string(root));
	
	/*json_object_object_foreach(root, key, val) {
    printf("\t%s: %s\n", key, json_object_to_json_string(val));
  }*/
	if (is_error(root)) {
        	if (root==NULL) {
        	printf("NO\n");
                return 0;

        } else {
        	printf("YES\n"); 
	}
}	
	//get dir json
	json_object *obj_dir = json_object_object_get(root, "dir");
	char *dir_jsonval = json_object_get_string(obj_dir);
	//printf("The value of dir is %s\n", dir_jsonval);



	//getdisplayname
	json_object *obj_disname = json_object_object_get(root, "disname");
	char *disname_jsonval = json_object_get_string(obj_disname);
	//printf("The value of disname is %s\n", disname_jsonval);



	//get user json
	json_object *obj_user = json_object_object_get(root, "users");
	char *user_jsonval = json_object_get_string(obj_user);
	//printf("The value of user is %s\n", user_jsonval);

	json_object  *user;

	//printf("parsing the user....\n");
	user = json_tokener_parse(user_jsonval);

	
	 json_object_object_foreach(user, key, val) {
	

	char pathbuf[100] ="/etc\/samba\/smb.conf";
	
	
	//char pathbuf[100] ="/home/dell/Desktop/project/smbfile/smb.conf";
	if(strcmp(key,"Guest") != 0)
	{
		strcat(pathbuf, ".");
		strcat(pathbuf, key);
	}

	
	HandleSmbFile(pathbuf,disname_jsonval ,json_object_to_json_string(val),dir_jsonval);
 
		

    //printf("\t%s: %s\n", key, json_object_to_json_string(val));
  }



	//get group json
	json_object *obj_group = json_object_object_get(root, "groups");
	char *group_jsonval = json_object_get_string(obj_group);
	//printf("The value of group is %s\n", group_jsonval);

	json_object  *group;

	//printf("parsing the user....\n");
	group = json_tokener_parse(group_jsonval);

	
	 json_object_object_foreach(group, groupkey, groupval) {
	
	char grouppathbuf[100] ="/etc\/samba\/smb.conf";
	//char pathbuf[100] ="/home/dell/Desktop/project/smbfile/smb.conf";

	if(strcmp(groupkey,"Guest") != 0)
	{
		strcat(grouppathbuf, ".");
		strcat(grouppathbuf, groupkey);
	}


	//printf("%s\n",grouppathbuf);
	
	HandleSmbFile(grouppathbuf,disname_jsonval ,json_object_to_json_string(groupval),dir_jsonval);
 
		

    //printf("\t%s: %s\n", key, json_object_to_json_string(val));
  }


	

	json_object_put(obj_user);
	json_object_put(obj_group);
	json_object_put(obj_dir);
	json_object_put(root);
	
	return 0;

}
